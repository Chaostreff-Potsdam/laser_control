#include "LaserPainter.h"

#include "LaserLine.h"
#include "LaserRectangle.h"
#include <utility>
#include <memory>

using namespace laser;

LaserPainter::LaserPainter() : m_smallestFreeId(0)
{
}

void LaserPainter::aquireEtherdreamWrapper()
{
	m_canvas = std::make_shared<EtherdreamWrapper>();
}

void LaserPainter::paintOn(std::shared_ptr<EtherdreamWrapper> e)
{
	m_canvas = e;
}

void LaserPainter::paint(LaserObjectPtrMap objects)
{
	m_objects = objects;

	updatePoints();
}

void LaserPainter::add(LaserObjectPtr object)
{
	LaserObjectPtrPair pair = make_pair(m_smallestFreeId++, object);

	m_objects.insert(pair);

	updatePoints();
}

void LaserPainter::updatePoints()
{
	std::vector<etherdream_point> ps;

	for (LaserObjectPtrMap::iterator it = m_objects.begin(); it != m_objects.end(); it++)
	{
		std::vector<etherdream_point> s = (it->second)->startPoints();
		ps.insert(ps.end(), s.begin(), s.end());

		std::vector<etherdream_point> p = (it->second)->points();
		ps.insert(ps.end(), p.begin(), p.end());

		std::vector<etherdream_point> e = (it->second)->endPoints();
		ps.insert(ps.end(), e.begin(), e.end());
	}

	m_canvas->setPoints(ps);
	m_canvas->writePoints();
}

void LaserPainter::deleteObject(int id)
{
	m_objects.erase(id);
	updatePoints();
}

void LaserPainter::drawWall(int id, Point p1, Point p2, Point p3, Point p4)
{
	LaserObjectPtr wall = std::make_shared<LaserRectangle>(p1, p2, p3, p4);
	LaserObjectPtrPair pair = make_pair(id, wall);
	m_objects.insert(pair);
	updatePoints();

	m_smallestFreeId = id + 1;
}
