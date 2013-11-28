#include "LaserPainter.h"

#include "LaserLine.h"
#include <utility>
#include <memory>

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
}

void LaserPainter::deleteObject(int id)
{
	m_objects.erase(id);
	updatePoints();
}

void LaserPainter::drawWall(int id, int Ax, int Ay, int Bx, int By, int Px, int Py)
{
	LaserObjectPtr wall = std::make_shared<LaserLine>(Ax, Ay, Bx, By);
	LaserObjectPtrPair pair = make_pair<int, LaserObjectPtr>(id, wall);
	m_objects.insert(pair);
	updatePoints();
}
