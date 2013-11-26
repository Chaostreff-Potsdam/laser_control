#include "LaserPainter.h"

#include "LaserLine.h"

LaserPainter::LaserPainter()
{
}

void LaserPainter::paintOn(EtherdreamWrapper *e)
{
	m_canvas = e;
}

void LaserPainter::paint(std::vector<LaserObjectPtr> objects)
{
	m_objects = objects;

	updatePoints();
}

void LaserPainter::add(LaserObjectPtr object)
{

	if (m_objects.size() > 1)
	{
		m_objects.pop_back();
	}

	if (!m_canvas->empty())
	{
		LaserObjectPtr line(new LaserLine(m_objects.back(), object));
		m_objects.push_back(line);
	}

	m_objects.push_back(object);

	LaserObjectPtr line2(new LaserLine(object, m_objects.front()));
	m_objects.push_back(line2);

	updatePoints();
}

void LaserPainter::updatePoints()
{
	std::vector<etherdream_point> ps;

	for (auto it = m_objects.begin(); it < m_objects.end(); it++)
	{
		std::vector<etherdream_point> p = (*it)->points();
		ps.insert(ps.end(), p.begin(), p.end());
	}

	m_canvas->setPoints(ps);
}
