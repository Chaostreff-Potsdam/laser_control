#include "EtherdreamWrapper.h"

#include <thread>
#include <mutex>
#include <chrono>

#include <opencv/cv.h>

#include <iostream>

#include "etherdream.h"

laser::EtherdreamWrapper::EtherdreamWrapper()
{
    m_pointsMutex = std::make_shared<std::mutex>();
    connect();
}

laser::EtherdreamWrapper::~EtherdreamWrapper()
{
	if(m_etherdream)
        etherdream_disconnect(m_etherdream);
}

bool laser::EtherdreamWrapper::empty()
{
    std::lock_guard<std::mutex> guard(*m_pointsMutex);

	return m_points.empty();
}

void laser::EtherdreamWrapper::clear()
{
    std::lock_guard<std::mutex> guard(*m_pointsMutex);

	m_points.clear();
}

void laser::EtherdreamWrapper::connect()
{
	try
	{
		// start listening for etherdreams
		etherdream_lib_start();

		// this will take atleast a second

		std::this_thread::sleep_for(std::chrono::milliseconds(1200));

		// how many have we seen
		int etherdream_count = etherdream_dac_count();

		// if none, stop
		if (!etherdream_count)
		{
			return;
		}

		// we will always take the first
		m_etherdream = etherdream_get(0);

		// establish TCP connection
		etherdream_connect(m_etherdream);

		return;
	}
	catch (...) // if the program wants us to stop
	{
		if(m_etherdream)
			etherdream_disconnect(m_etherdream);
		return;
	}
}

void laser::EtherdreamWrapper::writePoints()
{
    std::lock_guard<std::mutex> guard(*m_pointsMutex);

    etherdream_write(m_etherdream, m_points.data(), m_points.size(), 30000, -1);
}

void laser::EtherdreamWrapper::setPoints(std::vector<etherdream_point> &p)
{
    std::cout << "setPoints" << std::endl;
    std::lock_guard<std::mutex> guard(*m_pointsMutex);

    m_points = p;
}

void laser::EtherdreamWrapper::addPoints(const std::vector<etherdream_point> &p)
{
    std::lock_guard<std::mutex> guard(*m_pointsMutex);

    m_points.insert(m_points.end(), p.begin(), p.end());
}
