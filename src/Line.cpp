#include "Line.h"

#include <vector>
#include <cmath>

#include "laser_utilities.h"

#ifndef LASERWRAPPER_LINE_POINTS
#define LASERWRAPPER_LINE_POINTS (100)
#endif

laser::Line::Line(int xa, int ya, int xb, int yb, bool visible, bool dashed)
:	Object(),
	m_visible(visible),
	m_dashed(dashed)
{
	m_a = Point(xa, ya);
	m_sAB = Point(xb, yb) - m_a;
	m_pointCount = m_sAB.abs()/LASERWRAPPER_LINE_POINTS;
}

laser::Line::Line(Point a, Point b, bool visible, bool dashed)
:	Object(),
	m_visible(visible),
	m_dashed(dashed)
{
	m_a = a;
	m_sAB = b - a;
	m_pointCount = m_sAB.abs()/LASERWRAPPER_LINE_POINTS;
}

laser::EtherdreamPoints laser::Line::points() const
{
	EtherdreamPoints ps;
	if (m_visible)
	{
		for (int i = 0; i <= m_pointCount; i++)
		{
			const Point p = m_a + m_sAB * ((float)i)/m_pointCount;
			const bool visible = !m_dashed || (m_dashed && (i % 4 < 2));

			ps.push_back(etherdreamPoint(p, visible));
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
			ps.push_back(etherdreamPoint(m_a + m_sAB * ((float)i)/LASERWRAPPER_LINE_POINTS, false));
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
			ps.push_back(etherdreamPoint(m_a + m_sAB * ((float)i)/LASERWRAPPER_LINE_POINTS, false));
		}
	}

	return ps;
}
