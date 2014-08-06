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
	m_pointCount = m_sAB.abs()/pixelsPerPoint();
}

laser::Line::Line(Point a, Point b, bool visible, bool dashed)
:	Object(),
	m_visible(visible),
	m_dashed(dashed)
{
	m_a = a;
	m_sAB = b - a;
	m_pointCount = m_sAB.abs()/pixelsPerPoint();
}

laser::EtherdreamPoints laser::Line::points() const
{
	int pointCount = m_sAB.abs()/pixelsPerPoint();
	EtherdreamPoints ps;
	if (m_visible)
	{
		for (int i = 0; i < pointCount; i++)
		{
			const Point p = m_a + m_sAB * ((float)i)/pointCount;
			const bool visible = !m_dashed || (m_dashed && (i % 4 < 2));

			ps.push_back(etherdreamPoint(p, visible));
		}
	}

	return ps;
}

laser::EtherdreamPoints laser::Line::startPoints() const
{
	int pointCount = m_sAB.abs()/pixelsPerPoint();
	EtherdreamPoints ps;

	if (m_visible)
	{
		for (int i = - pointCount * marginPointFraction()/100.0; i < 0; i++)
		{
			ps.push_back(etherdreamPoint(m_a + m_sAB * ((float)i)/pixelsPerPoint(), false));
		}
	}

	return ps;
}


laser::EtherdreamPoints laser::Line::endPoints() const
{
	int pointCount = m_sAB.abs()/pixelsPerPoint();
	EtherdreamPoints ps;

	if (m_visible)
	{
		for (int i = pixelsPerPoint(); i < pixelsPerPoint() + pointCount * marginPointFraction()/100.0; i++)
		{
			ps.push_back(etherdreamPoint(m_a + m_sAB * ((float)i)/pixelsPerPoint(), false));
		}
	}

	return ps;
}
