#include "LaserLine.h"

#include <vector>

#ifndef LASERWRAPPER_LINE_POINTS
#define LASERWRAPPER_LINE_POINTS (50)
#endif

LaserLine::LaserLine(int xa, int ya, int xb, int yb, bool visible) : LaserObject()
{
	m_xa = xa;
	m_ya = ya;

	m_xb = xb;
	m_yb = yb;

	m_visible = visible;
}

LaserLine::LaserLine(LaserObjectPtr start, LaserObjectPtr end, bool visible)
{
	m_xa = start->startPoint().x;
	m_ya = start->startPoint().y;

	m_xb = end->endPoint().x;
	m_yb = end->endPoint().y;

	m_visible = visible;
}

std::vector<etherdream_point> LaserLine::points() const
{
	std::vector<etherdream_point> ps;

	for (int i = 0; i < LASERWRAPPER_LINE_POINTS; i++)
	{
		etherdream_point p;

		p.x = m_xa + ((float)i)/LASERWRAPPER_LINE_POINTS * (m_xb - m_xa);
		p.y = m_ya + ((float)i)/LASERWRAPPER_LINE_POINTS * (m_yb - m_ya);
		p.r = 0;
		p.g = m_visible ? 65535 : 0;
		p.b = 0;
		ps.push_back(p);
	}

	return ps;
}

etherdream_point LaserLine::startPoint() const
{
	etherdream_point p;

	p.x = m_xa;
	p.y = m_ya;

	return p;
}


etherdream_point LaserLine::endPoint() const
{
	etherdream_point p;

	p.x = m_xb;
	p.y = m_yb;

	return p;
}
