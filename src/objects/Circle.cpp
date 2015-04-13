#include "Circle.h"

#include <vector>
#include <cmath>
#include <cstdint>

#ifndef LASERWRAPPER_CIRCLE_POINTS
#define LASERWRAPPER_CIRCLE_POINTS (30)
#endif

laser::Circle::Circle(int x, int y, int radius, float startRatio, float endRatio)
:	Object()
{
	m_p = Point(x, y);
	m_radius = radius;
	m_startRatio = startRatio;
	m_endRatio = endRatio;
	m_marginPointFraction = 10;
}

laser::Circle::Circle(Point p, int radius, float startRatio, float endRatio)
{
	m_p = p;
	m_radius = radius;
	m_startRatio = startRatio;
	m_endRatio = endRatio;
	m_marginPointFraction = 10;
}

laser::EtherdreamPoints laser::Circle::points() const
{
	EtherdreamPoints ps;
	int points = LASERWRAPPER_CIRCLE_POINTS * (std::abs(m_startRatio - m_endRatio) / (2 * M_PI));

	for (float rad = m_startRatio;
		 rad <= m_endRatio;
		 rad += std::abs(m_endRatio - m_startRatio) / points)
	{
		const Point onCircle(cos(rad) * m_radius, sin(rad) * m_radius);
		ps.push_back(etherdreamPoint(m_p + onCircle));
	}

	return ps;
}

laser::EtherdreamPoints laser::Circle::startPoints() const
{
	EtherdreamPoints ps;
	int points = LASERWRAPPER_CIRCLE_POINTS * (std::abs(m_startRatio - m_endRatio) / (2 * M_PI));

//	if (std::abs(fmod(m_startRatio, 2*M_PI) - fmod(m_endRatio, 2*M_PI)) < 0.01)
//		return ps;

	for (float rad = m_startRatio - 2 * marginPointFraction()/100.0 * M_PI;
		 rad < m_startRatio;
		 rad += std::abs(m_endRatio - m_startRatio)/points)
	{
		const Point onCircle(cos(rad) * m_radius, sin(rad) * m_radius);
		ps.push_back(etherdreamPoint(m_p + onCircle, false));
	}

	return ps;
}

laser::EtherdreamPoints laser::Circle::endPoints() const
{
	EtherdreamPoints ps;
	int points = LASERWRAPPER_CIRCLE_POINTS * (std::abs(m_startRatio - m_endRatio) / (2 * M_PI));

//	if (std::abs(fmod(m_startRatio, 2*M_PI) - fmod(m_endRatio, 2*M_PI)) < 0.01)
//		return ps;

	for (float rad = m_endRatio;
		 rad < m_endRatio + 2 * marginPointFraction()/100.0 * M_PI;
		 rad += std::abs(m_endRatio - m_startRatio)/points)
	{
		const Point onCircle(cos(rad) * m_radius, sin(rad) * m_radius);
		ps.push_back(etherdreamPoint(m_p + onCircle, false));
	}

	return ps;
}
