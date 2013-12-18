#include "EtherdreamWrapper.h"

#include <thread>
#include <mutex>
#include <chrono>

#include <opencv/cv.h>

#include <iostream>

#include "etherdream.h"

using namespace laser;

EtherdreamWrapper::EtherdreamWrapper()
{
    m_pointsMutex = std::make_shared<std::mutex>();
    m_calibration = cv::Mat::eye(3, 3, CV_32FC1);
    connect();
}

EtherdreamWrapper::EtherdreamWrapper(cv::Mat calibration)
    : m_calibration(calibration)
{
    m_pointsMutex = std::make_shared<std::mutex>();
	connect();
}

EtherdreamWrapper::~EtherdreamWrapper()
{
	if(m_etherdream)
        etherdream_disconnect(m_etherdream);
}

void EtherdreamWrapper::setCalibration(cv::Mat calibration)
{
    m_calibration = calibration;
}

bool EtherdreamWrapper::empty()
{
    std::lock_guard<std::mutex> guard(*m_pointsMutex);

	return m_points.empty();
}

void EtherdreamWrapper::clear()
{
    std::lock_guard<std::mutex> guard(*m_pointsMutex);

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
    std::lock_guard<std::mutex> guard(*m_pointsMutex);

    etherdream_write(m_etherdream, m_points.data(), m_points.size(), 30000, -1);
}

void EtherdreamWrapper::setPoints(std::vector<etherdream_point> &p)
{
    std::lock_guard<std::mutex> guard(*m_pointsMutex);

    std::vector<cv::Point2f> aux_in;
    std::vector<cv::Point2f> aux_out;

    m_points.clear();

    for (auto i : p)
    {
        aux_in.push_back(cv::Point2f(i.x, i.y));
    }

    cv::perspectiveTransform(aux_in, aux_out, m_calibration);

    // argh, that hurts...
    for (int i = 0; i < p.size(); i++)
    {
        m_points.push_back(etherdream_point { aux_out[i].x, aux_out[i].y, p[i].r, p[i].g, p[i].b } );
    }
}

void EtherdreamWrapper::addPoints(const std::vector<etherdream_point> &p)
{
    std::lock_guard<std::mutex> guard(*m_pointsMutex);

    m_points.insert(m_points.end(), p.begin(), p.end());
}
