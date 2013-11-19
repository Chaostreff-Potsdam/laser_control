#include "EtherdreamWrapper.h"

#include <thread>
#include <mutex>
#include <chrono>

#include <iostream>

#include "etherdream.h"

EtherdreamWrapper::EtherdreamWrapper()
{
	std::cout << "Eior" << std::endl;
	m_thread = std::thread(&EtherdreamWrapper::connect, this);
}

void EtherdreamWrapper::clear()
{
	m_points.clear();
}

void EtherdreamWrapper::connect()
{
	try
	{
		// start listening for etherdreams
		etherdream_lib_start();

		std::cout << "Starting lib, will wait 1.2 s" << std::endl;

		// this will take atleast a second

		std::this_thread::sleep_for(std::chrono::milliseconds(1200));

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
	catch (...) // if the program wants us to stop
	{
		etherdream_disconnect(m_etherdream);
		return;
	}
}

void EtherdreamWrapper::writePoints()
{
	std::lock_guard<std::mutex> guard(m_pointsMutex);

	if (!m_points.empty())
	{
		etherdream_write(m_etherdream, m_points.data(), m_points.size(), 30000, 1);
	}
}

void EtherdreamWrapper::setPoints(std::vector<struct etherdream_point> p)
{
	std::lock_guard<std::mutex> guard(m_pointsMutex);

	m_points = p;
}

void EtherdreamWrapper::addPoints(const std::vector<etherdream_point> &p)
{
	std::lock_guard<std::mutex> guard(m_pointsMutex);

	m_points.insert(m_points.end(), p.begin(), p.end());
}
