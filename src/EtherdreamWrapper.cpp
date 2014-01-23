#include "EtherdreamWrapper.h"

#include <chrono>
#include <thread>

#include <iostream>

laser::EtherdreamWrapper::EtherdreamWrapper()
{
    connect();
}

laser::EtherdreamWrapper::~EtherdreamWrapper()
{
#ifdef _WIN32
	if (m_cardNum == -1) {
			EtherDreamStop(&m_cardNum);
			EtherDreamCloseDevice(&m_cardNum);
		}
	EtherDreamClose();
#else
	if(m_etherdream)
        etherdream_disconnect(m_etherdream);
#endif
}

bool laser::EtherdreamWrapper::empty()
{
	std::lock_guard<std::mutex> guard(m_pointsMutex);

	return m_points.empty();
}

void laser::EtherdreamWrapper::clear()
{
	std::lock_guard<std::mutex> guard(m_pointsMutex);

	m_points.clear();
}

void laser::EtherdreamWrapper::connect()
{
#ifdef _WIN32
		try
	{
		m_cardNum = -1;
		// connectToEtherdream
		if(0==EtherDreamGetCardNum()) { //it is actually card count. We use always the first laser :-)
			std::cout << "No Etherdream found :(\n";
			if (m_cardNum != -1) {
				EtherDreamStop(&m_cardNum);
				EtherDreamCloseDevice(&m_cardNum);
			}
			EtherDreamClose();
			return;
		}
		m_cardNum = 0;
		std::this_thread::sleep_for(std::chrono::milliseconds(1200));
		EtherDreamOpenDevice(&m_cardNum);

		return;
	}
	catch (...) // if the program wants us to stop
	{
		if (m_cardNum != -1) {
			EtherDreamStop(&m_cardNum);
			EtherDreamCloseDevice(&m_cardNum);
		}
		EtherDreamClose();
		return;
	}
#else
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
#endif
}

void laser::EtherdreamWrapper::writePoints()
{
	std::lock_guard<std::mutex> guard(m_pointsMutex);

#ifdef _WIN32
	EtherDreamWriteFrame(&m_cardNum, (EAD_Pnt_s*)m_points.data(), m_points.size()*sizeof(EAD_Pnt_s), 30000, -1);
#else
    etherdream_write(m_etherdream, m_points.data(), m_points.size(), 30000, -1);
#endif
}

void laser::EtherdreamWrapper::setPoints(const EtherdreamPoints &p)
{
	std::lock_guard<std::mutex> guard(m_pointsMutex);

    m_points = p;
}

void laser::EtherdreamWrapper::addPoints(const EtherdreamPoints &p)
{
	std::lock_guard<std::mutex> guard(m_pointsMutex);

    m_points.insert(m_points.end(), p.begin(), p.end());
}
