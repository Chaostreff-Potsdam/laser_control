#include "Painter.h"

#include "Line.h"
#include "Rectangle.h"
#include "Circle.h"
#ifndef _WIN32 //as LaserCompositeObject uses variadic templates, which is not supported by MSVC11
#include "CompositeObject.h"
#endif
#include "laser_calibration/Calibration.h"
#include "Transform.h"

#include <utility>
#include <memory>
#include <cmath>
#include <mutex>
#include <iostream>
#include <algorithm>

#include <boost/date_time.hpp>



laser::Painter::Painter(bool expireObjects)
:	m_smallestFreeId(0),
	m_expireObjects(expireObjects)
{
	if (expireObjects)
	{
		m_updateLoop = std::make_shared<std::thread>(&Painter::updateLoop, this);
	}
}

void laser::Painter::aquireEtherdreamWrapper()
{
    m_canvas = std::make_shared<EtherdreamWrapper>();
}

void laser::Painter::calibrate()
{
	if (!m_canvas) aquireEtherdreamWrapper();

    Calibration calibration(m_canvas);
    calibration.start();

    m_calibration = calibration.homography();
}

void laser::Painter::paintOn(const std::shared_ptr<EtherdreamWrapper> &e)
{
	m_canvas = e;
}

void laser::Painter::paint(const ObjectPtrMap & objects)
{
	m_objects = objects;

	updatePoints();
}

void laser::Painter::add(const ObjectPtr & object)
{
	m_objects[m_smallestFreeId++] = object;

	updatePoints();
}

void laser::Painter::updatePoints()
{
	EtherdreamPoints ps;

	std::lock_guard<std::mutex> lock(m_updateMutex);

	if (m_objects.empty())
		return;

	if (m_expireObjects)
	{
		boost::posix_time::ptime now(boost::date_time::microsec_clock<boost::posix_time::ptime>::universal_time());

		ObjectPtrMap::iterator iter = m_objects.begin();
		ObjectPtrMap::iterator end = m_objects.end();

		while(iter != end)
		{

			if ((iter->second)->permanent())
			{
				++iter;
				continue;
			}

			boost::posix_time::time_duration lifetime = now - (iter->second)->started();

			if (lifetime > boost::posix_time::seconds(10))
			{
					m_objects.erase(iter++);
			}
			else
			{
					++iter;
			}
		}
	}

	for (ObjectPtrMap::iterator it = m_objects.begin(); it != m_objects.end(); it++)
	{
		appendToVector(ps, (it->second)->startPoints());
		appendToVector(ps, (it->second)->points());
		appendToVector(ps, (it->second)->endPoints());
	}

	if(!m_calibration.empty()) {
		Transform::applyInPlace(ps, cv::perspectiveTransform, m_calibration);
	}

	m_canvas->setPoints(ps);
	m_canvas->writePoints();
}

void laser::Painter::deleteObject(int id)
{
	m_objects.erase(id);
	updatePoints();
}

void laser::Painter::drawWall(int id, Point p1, Point p2)
{
	std::cout << "Draw Wall" << std::endl;
	m_objects[id] = std::make_shared<Line>(p1, p2, true);
	m_objects[id]->setPermanent(true);
	updatePoints();

	m_smallestFreeId = id + 1;
}

#ifndef _WIN32 //as LaserCompositeObject uses variadic templates, which is not supported by MSVC11
void laser::Painter::drawDoor(int id, Point p1, Point p2)
{
	std::vector<ObjectPtr> objs;

	objs.push_back(std::make_shared<Line>(p1, p2));

	int radius = sqrt(sqr(p1.x() - p2.x()) + sqr(p1.y() - p2.y()));
	float rad = atan2(p2.y() - p1.y(), p2.x() - p1.y());
	Point middle = p1;
	objs.push_back(std::make_shared<Circle>(middle, radius, rad+0.1, rad+M_PI_4 + 0.4));
	//etherdream_point circleEnd = objs.back()->points().back();
	//objs.push_back(std::make_shared<LaserLine>(Point(circleEnd.x, circleEnd.y), middle));

	ObjectPtr co = std::make_shared<CompositeObject>(objs);

	m_objects.insert(make_pair(id, co));
	updatePoints();

	m_smallestFreeId = id + 1;
}
#endif

void laser::Painter::drawTable(int id, laser::Point p1, laser::Point p2, laser::Point p3, laser::Point p4)
{
	m_objects[id] = std::make_shared<Rectangle>(p1, p2, p3, p4, false);
	updatePoints();
}

void laser::Painter::drawPlayer(int id, laser::Point p1)
{
	m_objects[id] = std::make_shared<Circle>(p1, 1000);
	m_objects[id]->setPermanent(true);
	updatePoints();
}

void laser::Painter::updateLoop()
{
	std::this_thread::sleep_for(std::chrono::seconds(5));
	while (true)
	{
		std::this_thread::sleep_for(std::chrono::seconds(1));
		updatePoints();
	}
}

