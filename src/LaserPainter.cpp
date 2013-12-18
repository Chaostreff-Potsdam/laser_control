#include "LaserPainter.h"

#include "LaserLine.h"
#include "LaserRectangle.h"
#include "LaserCircle.h"
#include "LaserCompositeObject.h"
#include <utility>
#include <memory>
#include <cmath>
#include <mutex>
#include <iostream>

#include <boost/date_time.hpp>

laser::LaserPainter::LaserPainter(bool expireObjects)
:	m_smallestFreeId(0),
	m_expireObjects(expireObjects)
{
	if (expireObjects)
	{
		m_updateLoop = std::make_shared<std::thread>(&LaserPainter::updateLoop, this);
	}

	m_updateMutex = std::make_shared<std::mutex>();

}

void laser::LaserPainter::aquireEtherdreamWrapper()
{
	m_canvas = std::make_shared<EtherdreamWrapper>();
}

void laser::LaserPainter::paintOn(std::shared_ptr<EtherdreamWrapper> e)
{
	m_canvas = e;
}

void laser::LaserPainter::paint(LaserObjectPtrMap objects)
{
	m_objects = objects;

	updatePoints();
}

void laser::LaserPainter::add(LaserObjectPtr object)
{
	LaserObjectPtrPair pair = make_pair(m_smallestFreeId++, object);

	m_objects.insert(pair);

	updatePoints();
}

void laser::LaserPainter::updatePoints()
{
	std::vector<etherdream_point> ps;

	std::lock_guard<std::mutex> lock(*m_updateMutex);

	boost::posix_time::ptime now(boost::date_time::microsec_clock<boost::posix_time::ptime>::universal_time());

	for (LaserObjectPtrMap::iterator it = m_objects.begin(); it != m_objects.end(); it++)
	{
		boost::posix_time::time_duration lifetime = now - (it->second)->started();
		std::cout << lifetime << std::endl;
		if (m_expireObjects && lifetime > boost::posix_time::seconds(10))
		{
			continue;
		}
		//if (m_objects.size() != 1)
		{
			std::vector<etherdream_point> s = (it->second)->startPoints();
			ps.insert(ps.end(), s.begin(), s.end());
		}

		std::vector<etherdream_point> p = (it->second)->points();
		ps.insert(ps.end(), p.begin(), p.end());

		//if (m_objects.size() != 1)
		{
			std::vector<etherdream_point> e = (it->second)->endPoints();
			ps.insert(ps.end(), e.begin(), e.end());
		}
	}

	m_canvas->setPoints(ps);
	m_canvas->writePoints();
}

void laser::LaserPainter::deleteObject(int id)
{
	m_objects.erase(id);
	updatePoints();
}

void laser::LaserPainter::drawWall(int id, Point p1, Point p2, Point p3, Point p4)
{
	LaserObjectPtr wall = std::make_shared<LaserRectangle>(p1, p2, p3, p4);
	LaserObjectPtrPair pair = make_pair(id, wall);
	m_objects.insert(pair);
	updatePoints();

	m_smallestFreeId = id + 1;
}

void laser::LaserPainter::drawDoor(int id, Point p1, Point p2)
{
	std::vector<LaserObjectPtr> objs;

	objs.push_back(std::make_shared<LaserLine>(p1, p2));

	int radius = sqrt(sqr(p1.x() - p2.x()) + sqr(p1.y() - p2.y()))/2;
	float rad = atan2(p2.y() - p1.y(), p2.x() - p1.y());
	Point middle = Point((p2.x() - p1.x())/2, (p2.y() - p1.y())/2);
	std::cout << middle.x() << "; " << middle.y() << std::endl;
	objs.push_back(std::make_shared<LaserCircle>(middle, radius, rad, rad+M_PI_4));
	//etherdream_point circleEnd = objs.back()->points().back();
	//objs.push_back(std::make_shared<LaserLine>(Point(circleEnd.x, circleEnd.y), middle));

	LaserObjectPtr co = std::make_shared<LaserCompositeObject>(objs);

	m_objects.insert(make_pair(id, co));
	updatePoints();

	m_smallestFreeId = id + 1;
}

void laser::LaserPainter::updateLoop()
{
	std::this_thread::sleep_for(std::chrono::seconds(5));
	while (true)
	{
		std::cout << "update" << std::endl;
		std::this_thread::sleep_for(std::chrono::seconds(1));
		updatePoints();
	}
}
