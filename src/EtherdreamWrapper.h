#ifndef ETHERDREAMWRAPPER_H
#define ETHERDREAMWRAPPER_H

#include <vector>

#include <thread>
#include <mutex>

#include "etherdream.h"

class EtherdreamWrapper
{
public:
	EtherdreamWrapper();


	void setPoints(std::vector<struct etherdream_point> p);
	void addPoints(std::vector<struct etherdream_point> const& p);


private:
	void writePoints();
	void connect();


	std::thread m_thread;

	std::mutex m_pointsMutex;

	std::vector<struct etherdream_point> m_points;

	struct etherdream *m_etherdream;
	/* data */
};

#endif
		// ETHERDREAMWRAPPER_H
