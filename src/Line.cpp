#include "Line.h"

#include <vector>
#include <cmath>

#include "laser_utilities.h"

#ifndef LASERWRAPPER_LINE_POINTS
#define LASERWRAPPER_LINE_POINTS (300)
#endif

laser::Line::Line(int xa, int ya, int xb, int yb, bool visible, bool dashed)
:	Object(),
	m_visible(visible),
	m_dashed(dashed)
{
	m_a = Point(xa, ya);
	m_b = Point(xb, yb);

	m_length = std::sqrt(sqr(m_a.x() - m_b.x()) + sqr(m_a.y() - m_b.y()));
	m_pointCount = m_length/LASERWRAPPER_LINE_POINTS;
}

laser::Line::Line(Point a, Point b, bool visible, bool dashed)
:	Object(),
	m_visible(visible),
	m_dashed(dashed)
{
	m_a = a;
	m_b = b;

	m_length = std::sqrt(sqr(m_a.x() - m_b.x()) + sqr(m_a.y() - m_b.y()));
	m_pointCount = m_length/LASERWRAPPER_LINE_POINTS;

	//std::cout << m_pointCount << std::endl;
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

			bool pointVisible = true;
			if(p.x == INT16_MIN || p.x == INT16_MAX || p.y == INT16_MIN || p.y == INT16_MAX) 
				pointVisible = false;

			p.r = 0;
			p.g = pointVisible ? (m_dashed ? ((i % 4 < 2) ? UINT16_MAX : 0) : UINT16_MAX) : 0;
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
