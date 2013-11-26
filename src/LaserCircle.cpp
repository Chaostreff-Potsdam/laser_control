#include "LaserCircle.h"

#include <vector>
#include <cmath>

#ifndef LASERWRAPPER_CIRCLE_POINTS
#define LASERWRAPPER_CIRCLE_POINTS (50)
#endif

LaserCircle::LaserCircle(int x, int y, int radius) : LaserObject()
{
	m_x = x;
	m_y = y;
	m_radius = radius;
}

std::vector<etherdream_point> LaserCircle::points() const
{
	std::vector<etherdream_point> ps;

	for (int i = 0; i < LASERWRAPPER_CIRCLE_POINTS + 5; i++)
	{
		etherdream_point p;

		p.x = m_x + m_radius * cos(2*M_PI*i/LASERWRAPPER_CIRCLE_POINTS);
		p.y = m_y + m_radius * sin(2*M_PI*i/LASERWRAPPER_CIRCLE_POINTS);
		p.r = 0;
		p.g = 65535;
		p.b = 0;
		ps.push_back(p);
	}

	return ps;
}

etherdream_point LaserCircle::startPoint() const
{
	etherdream_point p;

	p.x = m_x + m_radius;
	p.y = m_y;
	p.r = 0;
	p.g = 65535;
	p.b = 0;

	return p;
}

etherdream_point LaserCircle::endPoint() const
{
	etherdream_point p;

	p.x = m_x + m_radius;
	p.y = m_y;
	p.r = 0;
	p.g = 65535;
	p.b = 0;

	return p;
}

