#include "Line.h"

#include <vector>
#include <cmath>

#include "laser_utilities.h"

#ifndef LASERWRAPPER_LINE_POINTS
#define LASERWRAPPER_LINE_POINTS (300)
#endif

laser::Line::Line(int xa, int ya, int xb, int yb, bool visible)
:	Object()
{
	m_a = Point(xa, ya);
	m_b = Point(xb, yb);

	m_length = std::sqrt(sqr(m_a.x() - m_b.x()) + sqr(m_a.y() - m_b.y()));
	m_pointCount = m_length/LASERWRAPPER_LINE_POINTS;

	m_visible = visible;
}

laser::Line::Line(Point a, Point b, bool visible)
:	Object()
{
	m_a = a;
	m_b = b;

	m_length = std::sqrt(sqr(m_a.x() - m_b.x()) + sqr(m_a.y() - m_b.y()));
	m_pointCount = m_length/LASERWRAPPER_LINE_POINTS;

	std::cout << m_pointCount << std::endl;

	m_visible = visible;
}

laser::EtherdreamPoints laser::Line::points() const
{
	EtherdreamPoints ps;
	if (m_visible)
	{
		for (int i = 0; i < m_pointCount; i++)
		{
			etherdream_point p;

			p.x = clamp(m_a.x() + ((float)i)/m_pointCount * (m_b.x() - m_a.x()), INT16_MIN, INT16_MAX);
			p.y = clamp(m_a.y() + ((float)i)/m_pointCount * (m_b.y() - m_a.y()), INT16_MIN, INT16_MAX);
			p.r = 0;
			p.g = UINT16_MAX;
			p.b = 0;
			ps.push_back(p);
		}
	}

	return ps;
}

laser::EtherdreamPoints laser::Line::startPoints() const
{
	EtherdreamPoints ps;

	if (m_visible)
	{
		for (int i = - m_pointCount/5; i < 0; i++)
		{
			etherdream_point p;

			p.x = clamp(m_a.x() + ((float)i)/LASERWRAPPER_LINE_POINTS * (m_b.x() - m_a.x()), INT16_MIN, INT16_MAX);
			p.y = clamp(m_a.y() + ((float)i)/LASERWRAPPER_LINE_POINTS * (m_b.y() - m_a.y()), INT16_MIN, INT16_MAX);
			p.r = 0;
			p.g = 0;
			p.b = 0;
			ps.push_back(p);
		}
	}

	return ps;
}


laser::EtherdreamPoints laser::Line::endPoints() const
{
	EtherdreamPoints ps;

	if (m_visible)
	{
		for (int i = LASERWRAPPER_LINE_POINTS; i < LASERWRAPPER_LINE_POINTS + m_pointCount/5; i++)
		{
			etherdream_point p;

			p.x = clamp(m_a.x() + ((float)i)/LASERWRAPPER_LINE_POINTS * (m_b.x() - m_a.x()), INT16_MIN, INT16_MAX);
			p.y = clamp(m_a.y() + ((float)i)/LASERWRAPPER_LINE_POINTS * (m_b.y() - m_a.y()), INT16_MIN, INT16_MAX);
			p.r = 0;
//			p.g = UINT16_MAX;
            p.g = 0;
			p.b = 0;
			ps.push_back(p);
		}
	}

	return ps;
}

void laser::Line::rotate(double rad)
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

void laser::Line::move(int x, int y)
{
	m_a.setX(m_a.x() + x);
	m_a.setY(m_a.y() + y);

	m_b.setX(m_b.x() + x);
	m_b.setY(m_b.y() + y);
}
