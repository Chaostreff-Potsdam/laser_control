#include "EtherdreamWrapper.h"

#include <boost/thread/thread.hpp>

#ifdef HAVE_BOOST_CHRONO
#include <boost/chrono.hpp>
#endif

#ifndef HAVE_BOOST_CHRONO
#include <boost/date_time/posix_time/posix_time_types.hpp>
#endif

#include <iostream>

EtherdreamWrapper::EtherdreamWrapper()
{
	std::cout << "Eior" << std::endl;
}

void EtherdreamWrapper::connect()
{
	try
	{
		// start listing for etherdreams
		etherdream_lib_start();

		std::cout << "Starting lib, will wait 1.2 s" << std::endl;

		// this will take atleast a second

#ifdef HAVE_BOOST_CHRONO
		boost::this_thread::sleep_for(boost::chrono::milliseconds(1200));
#endif
#ifndef HAVE_BOOST_CHRONO
		boost::this_thread::sleep(boost::posix_time::milliseconds(1200));
#endif

		// how many have we seen
		int etherdream_count = etherdream_dac_count();

		std::cout << "Saw " << etherdream_count << " DACs" << std::endl;

		// if none, stop
		if (!etherdream_count)
		{
			return;
		}

		// we will always take the first
		m_etherdream = etherdream_get(0);

		// establish TCP connection
		etherdream_connect(m_etherdream);

		while (true)
		{
			writePoints();

			// this waits till we can write more points
			// it provides us with time to change the points
			etherdream_wait_for_ready(m_etherdream);
		}
	}
	catch (boost::thread_interrupted e) // if the program wants us to stop
	{
		etherdream_disconnect(m_etherdream);
		return;
	}
}

void EtherdreamWrapper::writePoints()
{
	boost::lock_guard<boost::mutex> guard(m_pointsMutex);

	if (!m_points.empty())
	{
		etherdream_write(m_etherdream, m_points.data(), m_points.size(), 30000, 1);
	}
}

void EtherdreamWrapper::setPoints(std::vector<struct etherdream_point> p)
{
	boost::lock_guard<boost::mutex> guard(m_pointsMutex);

	m_points = p;
}

void EtherdreamWrapper::addPoints(std::vector<struct etherdream_point> p)
{
	boost::lock_guard<boost::mutex> guard(m_pointsMutex);

	m_points.insert(m_points.end(), p.begin(), p.end());
}
