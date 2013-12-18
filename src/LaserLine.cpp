#include "LaserLine.h"

#include <vector>
#include <cmath>

#include "laser_utilities.h"

#ifndef LASERWRAPPER_LINE_POINTS
#define LASERWRAPPER_LINE_POINTS (300)
#endif

using namespace laser;

LaserLine::LaserLine(int xa, int ya, int xb, int yb, bool visible) : LaserObject()
{
	m_a = Point(xa, ya);
	m_b = Point(xb, yb);

	m_visible = visible;
}

LaserLine::LaserLine(Point a, Point b, bool visible)
{
	m_a = a;
	m_b = b;

	m_visible = visible;
}

std::vector<etherdream_point> LaserLine::points() const
{
	std::vector<etherdream_point> ps;
	if (m_visible)
	{
		int length = std::sqrt(sqr(m_a.x() - m_b.x()) + sqr(m_a.y() - m_b.y()));

		int num = length/LASERWRAPPER_LINE_POINTS;

		for (int i = 0; i < num; i++)
		{
			etherdream_point p;

			p.x = m_a.x() + ((float)i)/num * (m_b.x() - m_a.x());
			p.y = m_a.y() + ((float)i)/num * (m_b.y() - m_a.y());
			p.r = 0;
			p.g = UINT16_MAX;
			p.b = 0;
			ps.push_back(p);
		}
	}

	return ps;
}

std::vector<etherdream_point> LaserLine::startPoints() const
{
	std::vector<etherdream_point> ps;

	if (m_visible)
	{
		for (int i = -10; i < 0; i++)
		{
			etherdream_point p;

			p.x = m_a.x() + ((float)i)/LASERWRAPPER_LINE_POINTS * (m_b.x() - m_a.x());
			p.y = m_a.y() + ((float)i)/LASERWRAPPER_LINE_POINTS * (m_b.y() - m_a.y());
			p.r = 0;
			p.g = 0;
			p.b = 0;
			ps.push_back(p);
		}
	}

	return ps;
}


std::vector<etherdream_point> LaserLine::endPoints() const
{
	std::vector<etherdream_point> ps;

	if (m_visible)
	{
		for (int i = LASERWRAPPER_LINE_POINTS; i < LASERWRAPPER_LINE_POINTS + 10; i++)
		{
			etherdream_point p;

			p.x = m_a.x() + ((float)i)/LASERWRAPPER_LINE_POINTS * (m_b.x() - m_a.x());
			p.y = m_a.y() + ((float)i)/LASERWRAPPER_LINE_POINTS * (m_b.y() - m_a.y());
			p.r = 0;
			p.g = UINT16_MAX;
			p.b = 0;
			ps.push_back(p);
		}
	}

	return ps;
}

void LaserLine::rotate(double rad)
{
	int oldXa = m_a.x();
	int oldYa = m_a.y();
	int oldXb = m_b.x();
	int oldYb = m_b.y();

	m_a.setX(round(oldXa * cos(rad) - oldYa * sin(rad)));
	m_a.setY(round(oldXa * sin(rad) + oldYa * cos(rad)));

	m_b.setX(round(oldXb * cos(rad) - oldYb * sin(rad)));
	m_b.setY(round(oldXb * sin(rad) + oldYb * cos(rad)));
}

void LaserLine::move(int x, int y)
{
	m_a.setX(m_a.x() + x);
	m_a.setY(m_a.y() + y);

	m_b.setX(m_b.x() + x);
	m_b.setY(m_b.y() + y);
}
