#include "EtherdreamWrapper.h"

#include <thread>
#include <mutex>
#include <chrono>

#include <iostream>

#include "etherdream.h"

using namespace laser;

EtherdreamWrapper::EtherdreamWrapper()
{
	m_newPoints = false;
	connect();
}

EtherdreamWrapper::~EtherdreamWrapper()
{
	if(m_etherdream)
		etherdream_disconnect(m_etherdream);
}

bool EtherdreamWrapper::empty()
{
	std::lock_guard<std::mutex> guard(m_pointsMutex);

	return m_points.empty();
}

void EtherdreamWrapper::clear()
{
	std::lock_guard<std::mutex> guard(m_pointsMutex);

	m_points.clear();
}

void EtherdreamWrapper::connect()
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

void EtherdreamWrapper::writePoints()
{
	std::lock_guard<std::mutex> guard(m_pointsMutex);

	std::cout << "a" << std::endl;
	etherdream_write(m_etherdream, m_points.data(), m_points.size(), 30000, -1);
	m_newPoints = false;
}

void EtherdreamWrapper::setPoints(std::vector<etherdream_point> &p)
{
	std::cout << "setPoints" << std::endl;
	std::lock_guard<std::mutex> guard(m_pointsMutex);

	m_points = p;
	m_newPoints = true;
}

void EtherdreamWrapper::addPoints(const std::vector<etherdream_point> &p)
{
	std::cout << "addPoints" << std::endl;
	std::lock_guard<std::mutex> guard(m_pointsMutex);

	m_points.insert(m_points.end(), p.begin(), p.end());
	m_newPoints = true;
}
