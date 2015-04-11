#include "EtherdreamWrapper.h"

#include <chrono>
#include <thread>
#include <iostream>
#include <opencv2/highgui/highgui.hpp>

int laser::EtherdreamWrapper::pps = 20000; // Our laser operates at 30kpps
int laser::EtherdreamWrapper::framesPerSecond = 10;
int laser::EtherdreamWrapper::offset = 3;

laser::EtherdreamWrapper::EtherdreamWrapper() :
	Canvas()
#ifndef _WIN32
	,
    m_etherdream(nullptr)
#endif
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

void laser::EtherdreamWrapper::connect()
{
#ifdef _WIN32
		try
	{
		m_cardNum = -1;
		// connectToEtherdream
		if(0==EtherDreamGetCardNum()) { //it is actually card count. We use always the first laser :-)
			std::cout << "[laser_control]: No Etherdream found :(\n";
			std::cout.flush();
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

void laser::EtherdreamWrapper::shiftPoints()
{
	if (offset == 0)
		return;

	int sz = m_points.size();
	EtherdreamPoints backup;
	backup.reserve(offset);

	for (int i = 0; i < offset; i++) {
		backup.emplace_back(m_points[i]);
	}

	for (int i = 0; i < sz - offset; i++) {
		m_points[i].x = m_points[i + offset].x;
		m_points[i].y = m_points[i + offset].y;
	}

	for (int i = 0; i < offset; i++) {
		m_points[sz - offset + i].x = backup[i].x;
		m_points[sz - offset + i].y = backup[i].y;
	}
}

void laser::EtherdreamWrapper::writePoints()
{
	std::lock_guard<std::mutex> guard(m_pointsMutex);
	pps = std::max(1, framesPerSecond) * m_points.size();
	cv::setTrackbarPos("pixel per second", "Laser options", pps);
#ifdef __APPLE__
	cv::waitKey(1);
#endif

#ifdef _WIN32
	if(m_points.size() == 0) {
		etherdream_point blackPoint;
		blackPoint.x = 0;
		blackPoint.y = 0;
		blackPoint.r = 0;
		blackPoint.g = 0;
		blackPoint.b = 0;
		m_points.push_back(blackPoint);
	}
	EtherDreamWriteFrame(&m_cardNum, (EAD_Pnt_s*)m_points.data(), m_points.size()*sizeof(EAD_Pnt_s), pps, -1);
#else
	if (!m_points.empty()) {
		shiftPoints();
		etherdream_write(m_etherdream, m_points.data(), m_points.size(), 20000, -1);
	}
#endif
}
