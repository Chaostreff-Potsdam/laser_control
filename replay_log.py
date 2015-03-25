#!/usr/bin/env python

import sys
import socket
import time
from Simulation import LaserClient, defaultPort

class Synchronizer(object):

	""" Waits for a certain timestamp in ms. No wait for the first one.
	    If the program falls more than a certain threshold behind, the delay will
		be omitted.
	"""

	maxClockSkewInMs = 30

	def __init__(self, speed):
		self.speed = speed * 1000
		self.last_ts = None

		self.start_ts = None
		self.startt = time.time()

	def __elapsed(self):
		# Found no information on the unit of time.clock(), being evil now
		return (time.time() - self.startt) * self.speed

	def waitFor(self, ts):
		if self.start_ts is None:
			self.last_ts = self.start_ts = ts
		else:
			diff = (ts - self.last_ts) / self.speed
			self.last_ts = ts
			if self.__elapsed() - (ts - self.start_ts) < self.maxClockSkewInMs:
				time.sleep(diff)

def main(fd, host, port, speed):
	data = (l.strip().split(" ", 1) for l in fd.readlines())
	sync = Synchronizer(speed)
	laser = LaserClient(host, port)
	laser.deleteAll()
	for ts, cmd in data:
		sync.waitFor(float(ts))
		laser.sendBytes(cmd.encode('utf-8'))

def readArgs():
	import argparse
	parser = argparse.ArgumentParser(
			epilog="Example call: gzcat server.log.gz | %s 192.168.1.112 -s 1.5" % sys.argv[0])
	parser.add_argument("host", help="the lasers' host[:port] (default: %(default)s)", nargs="?", type=str, default="localhost:30001")
	parser.add_argument("-i", "--input", help="input file (stdin if not specified)", type=str)
	parser.add_argument("-s", "--speed", help="speedup factor", type=float, default=1.0)
	args = parser.parse_args()
	host, _, port = args.host.partition(":")
	port = int(port or defaultPort)
	fd = open(args.input) if args.input else sys.stdin
	return fd, host, port, args.speed

if __name__ == "__main__":
	main(*readArgs())
