/* j4cDAC driver
 *
 * Copyright 2009 Andrew Kibler
 * Copyright 2011 Jacob Potter
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of either the GNU General Public License version 2
 * or 3, or the GNU Lesser General Public License version 3, as published
 * by the Free Software Foundation, at your option.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#define WINVER 0x0501

#include <stdio.h>
#include <math.h>
#include <string.h>
#include <iostream>
#include <thread>
#include <chrono>

#include "dac.h"

#include <mmsystem.h>

#define DEBUG_THRESHOLD		800

#ifdef _WIN32
#define SNPRINTF _snprintf
#else
#define SNPRINTF snprintf
#endif

extern dac_t * dac_list;

unsigned __stdcall LoopUpdate(void *d);

/* Initialize a dac_t */
int dac_init(dac_t *d) {
	memset(d, 0, sizeof(*d));
	InitializeCriticalSection(&d->buffer_lock);
	return 0;
}

/* Connect */
int dac_open_connection(dac_t *d) {
	char host[40];
	strncpy(host, inet_ntoa(d->addr), sizeof(host) - 1);
	host[sizeof(host) - 1] = 0;

	// Initialize buffer
	d->buffer_read = 0;
	d->buffer_fullness = 0;
	memset(d->buffer, sizeof(d->buffer), 0);

	// Connect to the DAC
	if (dac_connect(d, host, "7765") < 0) {
		trace(d, "!! DAC connection failed.\n");
		std::cout << "[laser_control]: !! DAC connection failed.\n";
		std::cout.flush();
		return -1;
	}

	// Fire off the worker thread
	d->loop_go = CreateEvent(NULL, FALSE, FALSE, NULL);
	d->state = ST_READY;

	d->workerthread = (HANDLE)_beginthreadex(NULL, 0, &LoopUpdate, d, 0, NULL);
	if (!d->workerthread) {
		trace(d, "!! Begin thread error: %s\n", strerror(errno));
		std::cout << "[laser_control]: !! Begin thread error:" << strerror(errno) << "\n";
		std::cout.flush();
		return -1;
	}

	trace(d, "Ready.\n");

	return 0;
}

void dac_close_connection(dac_t *d) {
	EnterCriticalSection(&d->buffer_lock);
	if (d->state == ST_READY)
		SetEvent(d->loop_go);
	d->state = ST_SHUTDOWN;
	LeaveCriticalSection(&d->buffer_lock);

	int rv = WaitForSingleObject(d->workerthread, 250);
	if (rv != WAIT_OBJECT_0) {
		trace(d, "Exit worker thread timed out.\n");
		TerminateThread(d->workerthread,-1);
	}
	CloseHandle(d->workerthread);

	dac_disconnect(d);
	d->state = ST_DISCONNECTED;
}

/* Look up a DAC by index or unique-ID
 */
dac_t * dac_get(int num) {
	dac_t *d = dac_list;
	if (num >= 0) {
		while (num--) {
			if (!d->next) {
				/* If they pass one past the last DAC,
				 * just return the last one. Ugh, buggy
				 * software. */
				if (!num) return d;
				else return NULL;
			}
			d = d->next;
		}
	} else {
		while (d) {
			if (num == d->dac_id) break;
			d = d->next;
		}
	}

	return d;
}

void dac_get_name(dac_t *d, char *buf, int max) {
	SNPRINTF(buf, max, "Ether Dream %02x%02x%02x",
		d->mac_address[3], d->mac_address[4], d->mac_address[5]);
}

/* Buffer access
 */
struct buffer_item *buf_get_write(dac_t *d) {
	EnterCriticalSection(&d->buffer_lock);
	int write = (d->buffer_read + d->buffer_fullness) % BUFFER_NFRAMES;
	LeaveCriticalSection(&d->buffer_lock);
	return &d->buffer[write];
}

void buf_advance_write(dac_t *d) {
	EnterCriticalSection(&d->buffer_lock);
	d->buffer_fullness++;
	if (d->state == ST_READY)
		SetEvent(d->loop_go);
	d->state = ST_RUNNING;
	LeaveCriticalSection(&d->buffer_lock);
}

int dac_get_status(dac_t *d) {
	EnterCriticalSection(&d->buffer_lock);
	int fullness = d->buffer_fullness;
	LeaveCriticalSection(&d->buffer_lock);

	if (fullness == BUFFER_NFRAMES) {
		return GET_STATUS_BUSY;
	} else {
		return GET_STATUS_READY;
	}
}

/* Write a frame.
 */

int do_write_frame(dac_t *d, const void * data, int bytes, int pps,
	int reps, int tryCount, int (*convert)(struct buffer_item *, const void *, int)) {
	int points = convert(NULL, NULL, bytes);

	if (reps == ((uint16_t) -1))
		reps = -1;

	/* If not ready for a new frame, <strike>bail</strike> try again. but only once. */
	if (dac_get_status(d) != GET_STATUS_READY) {
		if(tryCount > 4) {
			trace(d, "M: Frame could not be written: DAC still not ready on try %d. %d points, %d reps\n", tryCount, points, reps);
			std::cout << "[laser_control] do_write_Frame: frame could not be written: DAC still not ready on fifth try. " << points << " points.\n";
			std::cout.flush();
			return 0;
		} else {
			if(tryCount > 1)
				Sleep(3);
			do_write_frame(d, data, bytes, pps, reps, tryCount + 1, convert);
			return 0;
		}
	}

	/* Ignore 0-repeat frames */
	if (!reps)
		return 1;

	int internal_reps = 0;
	if (points != 0)
		internal_reps = 250 / points;
	char * bigdata = NULL;
	if (internal_reps) {
		bigdata = (char *)malloc(bytes * internal_reps);
		int i;
		for (i = 0; i < internal_reps; i++) {
			memcpy(bigdata + i*bytes, data, bytes);
		}
		bytes *= internal_reps;
		data = bigdata;
	}

	trace(d, "M: Writing: %d/%d points, %d reps, %d pps\n",
		points, convert(NULL, NULL, bytes), reps, pps);

	struct buffer_item *next = buf_get_write(d);
	convert(next, data, bytes);
	next->pps = pps;
	next->repeatcount = reps;

	buf_advance_write(d);

	if (bigdata) free(bigdata);

	return 1;
}

#include <locale.h>

// This is the buffer filling thread for WriteFrame() and WriteFrameNR()
unsigned __stdcall LoopUpdate(void *dv){
	dac_t *d = (dac_t *)dv;

	int res;
	res = SetThreadPriority(GetCurrentThread(), THREAD_PRIORITY_TIME_CRITICAL);
	trace(d, "SetThreadPriority ret %d\n");

	EnterCriticalSection(&d->buffer_lock);
	
	unsigned long before = GetTickCount();
	while (1) {
		/* Wait for us to have data */
		while (d->state == ST_READY) {
			LeaveCriticalSection(&d->buffer_lock);

			trace(d, "L: Waiting...\n");

			int res = WaitForSingleObject(d->loop_go, INFINITE);

			EnterCriticalSection(&d->buffer_lock);
			if (res != WAIT_OBJECT_0) {
				trace(d, "!! WaitForSingleObject failed: %lu\n",
					GetLastError());
				std::cout << "[laser_control]: !! WaitForSingleObject failed: " << GetLastError() << "\n";
				std::cout.flush();
				d->state = ST_BROKEN;
				return 0;
			}
		}

		if (d->state != ST_RUNNING) {
			trace(d, "L: Shutting down.\n");
			std::cout << "[laser_control]: Shutting down.\n"; std::cout.flush();
			LeaveCriticalSection(&d->buffer_lock);
			return 0;
		}

		LeaveCriticalSection(&d->buffer_lock);

		struct buffer_item *b = &d->buffer[d->buffer_read];
		int cap;
		int iters = 0;

		while (1) {
			/* Estimate how much data has been consumed since the last
			 * time we got an ACK. */
			LARGE_INTEGER time;
			QueryPerformanceCounter(&time);
			LONGLONG time_diff = time.QuadPart - d->conn.last_ack_time.QuadPart;
			LONGLONG points_used = (time_diff * b->pps) / timer_freq.QuadPart;
			int iu = points_used;
			if (d->conn.resp.dac_status.playback_state != 2) iu = 0;

			int expected_fullness = d->conn.resp.dac_status.buffer_fullness \
				- iu + d->conn.unacked_points;

			/* Now, see how much data we should write. */
			cap = 1700 - expected_fullness;

			if (d->conn.resp.dac_status.buffer_fullness < DEBUG_THRESHOLD
			    || expected_fullness < DEBUG_THRESHOLD)
				trace(d, "L: b %d + %d - %d = %d, w %d om %d st %d\n",
					d->conn.resp.dac_status.buffer_fullness,
					d->conn.unacked_points,
					iu, expected_fullness, cap,
					d->conn.pending_meta_acks,
					d->conn.resp.dac_status.playback_state);
			if (cap > DAC_MIN_SEND) break;
			if (iters++ > 1) break;
			/* Wait a little. */
			int diff = DAC_MIN_SEND - cap;
			int sleeptime = 1 + (1000 * diff / b->pps);
			Sleep(sleeptime);
		}

		/* How many points can we send? */
		int b_left = b->points - b->idx;
		
		if (cap > b_left)
			cap = b_left;
		if (cap > 80)
			cap = 80;
		if (cap < 0)
			cap = 1;
		

		
		int res = dac_send_data(d, b->data + b->idx, cap, b->pps);

		//std::cout << "[laser_control]: sent " << cap << " points\n";

		if (res < 0) {
			std::cout << "[laser_control]: dac_send_data return < 0. Welp, something's wrong.\n"; std::cout.flush();
			/* Welp, something's wrong. There's not much we
			 * can do at an API level other than start throwing
			 * "error" returns up to the application... */
			EnterCriticalSection(&d->buffer_lock);
			d->state = ST_BROKEN;
			LeaveCriticalSection(&d->buffer_lock);
			return 1;
		}
		/* What next? */
		EnterCriticalSection(&d->buffer_lock);
		b->idx += res;

		if (b->idx < b->points) {
			continue;
		}

		unsigned long after = GetTickCount();

		//std::cout << ".";std::cout.flush();
		before = after;		

		b->idx = 0;

		if (b->repeatcount > 1) {
			/* Play this frame again? */
			b->repeatcount--;
		} else if (d->buffer_fullness > 1) {
			/* Move to the next frame */
			trace(d, "L: advancing - b %d\n",
				d->buffer_fullness);
			//std::cout << "advancing to new frame.\n";std::cout.flush();
			d->buffer_fullness--;
			d->buffer_read++;
			if (d->buffer_read >= BUFFER_NFRAMES)
				d->buffer_read = 0;
		} else if (b->repeatcount >= 0) {
			/* Stop playing until we get a new frame. */
			trace(d, "L: returning to idle\n");
			//std::cout << "frame sent, not repeated. returning to idle.\n";std::cout.flush();
			d->state = ST_READY;
		} else {
			/* Repeat this frame */
			//std::cout << "repeat this frame.\n";std::cout.flush();
		}

		/* If we get here without hitting any of the above cases,
		 * then repeatcount is negative and there's no new frame -
		 * so we're just supposed to keep playing this one again
		 * and again. */ 
	}

	return 0;
}
