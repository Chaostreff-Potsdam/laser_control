#include "LaserCircle.h"

#include <vector>
#include <cmath>

#ifndef LASERWRAPPER_CIRCLE_POINTS
#define LASERWRAPPER_CIRCLE_POINTS 50
#endif

LaserCircle::LaserCircle(int x, int y, int radius) : LaserObject()
{
	m_x = x;
	m_y = y;
	m_radius = radius;
}

std::vector<struct etherdream_point> LaserCircle::points()
{
	std::vector<struct etherdream_point> ps;

	for (int i = 0; i < LASERWRAPPER_CIRCLE_POINTS; i++)
	{
		struct etherdream_point p;

		p.x = m_x + m_radius * cos(2*M_PI / i);
		p.y = m_y + m_radius * cos(2*M_PI / i);
		p.r = 0;
		p.g = 30000;
		p.b = 0;

		ps.push_back(p);
	}

	return ps;
}
