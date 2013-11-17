#ifndef ETHERDREAMWRAPPER_H
#define ETHERDREAMWRAPPER_H

#include <vector>

#include <boost/thread/thread.hpp>

#include "etherdream.h"

class EtherdreamWrapper
{
public:
	EtherdreamWrapper();


	void connect();

	void setPoints(std::vector<struct etherdream_point> p);
	void addPoints(std::vector<struct etherdream_point> p);


private:
	void writePoints();


	boost::mutex m_pointsMutex;

	std::vector<struct etherdream_point> m_points;

	struct etherdream *m_etherdream;
	/* data */
};

#endif
		// ETHERDREAMWRAPPER_H
