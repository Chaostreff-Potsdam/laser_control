#ifndef ETHERDREAMWRAPPER_H
#define ETHERDREAMWRAPPER_H

#include <vector>
#include <thread>
#include <mutex>
#include <atomic>

#include "etherdream.h"

class EtherdreamWrapper
{
public:
	EtherdreamWrapper();
	~EtherdreamWrapper();


	void clear();
	bool empty();
	void setPoints(std::vector<etherdream_point> p);
	void addPoints(std::vector<etherdream_point> const&p);


private:
	void writePoints();
	void connect();


	std::thread m_thread;

	std::mutex m_pointsMutex;

	std::vector<etherdream_point> m_points;
	std::atomic_bool m_newPoints;

	struct etherdream *m_etherdream;
	/* data */
};

#endif
		// ETHERDREAMWRAPPER_H
