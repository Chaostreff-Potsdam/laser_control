#include "Circle.h"

#include <vector>
#include <cmath>
#include <cstdint>

#ifndef LASERWRAPPER_CIRCLE_POINTS
#define LASERWRAPPER_CIRCLE_POINTS (100)
#endif

laser::Circle::Circle(int x, int y, int radius, float startRatio, float endRatio)
:	Object()
{
	m_p = Point(x, y);
	m_radius = radius;
	m_startRatio = startRatio;
	m_endRatio = endRatio;
}

laser::Circle::Circle(Point p, int radius, float startRatio, float endRatio)
{
	m_p = p;
	m_radius = radius;
	m_startRatio = startRatio;
	m_endRatio = endRatio;
}

laser::EtherdreamPoints laser::Circle::points() const
{
	EtherdreamPoints ps;

	for (float rad = m_startRatio;
		 rad <= m_endRatio;
		 rad += std::abs(m_endRatio - m_startRatio)/LASERWRAPPER_CIRCLE_POINTS)
	{
		etherdream_point p;

		p.x = clamp(m_p.x() + m_radius * cos(rad), INT16_MIN, INT16_MAX);
		p.y = clamp(m_p.y() + m_radius * sin(rad), INT16_MIN, INT16_MAX);
		p.r = 0;
		p.g = UINT16_MAX;
		p.b = 0;
		ps.push_back(p);
	}

	return ps;
}

laser::EtherdreamPoints laser::Circle::startPoints() const
{
	EtherdreamPoints ps;

//	if (std::abs(fmod(m_startRatio, 2*M_PI) - fmod(m_endRatio, 2*M_PI)) < 0.01)
//		return ps;

	for (float rad = m_startRatio - M_PI_2;
		 rad < m_startRatio;
		 rad += M_PI_2 / 20)
	{
		etherdream_point p;

		p.x = clamp(m_p.x() + m_radius * cos(rad), INT16_MIN, INT16_MAX);
		p.y = clamp(m_p.y() + m_radius * sin(rad), INT16_MIN, INT16_MAX);
		p.r = 0;
		p.g = 0;
		p.b = 0;
		ps.push_back(p);
	}

	return ps;
}

laser::EtherdreamPoints laser::Circle::endPoints() const
{
	EtherdreamPoints ps;

//	if (std::abs(fmod(m_startRatio, 2*M_PI) - fmod(m_endRatio, 2*M_PI)) < 0.01)
//		return ps;

	for (float rad = m_endRatio;
		 rad < m_endRatio + M_PI_2;
		 rad += M_PI_2 / 20)
	{
		etherdream_point p;

		p.x = clamp(m_p.x() + m_radius * cos(rad), INT16_MIN, INT16_MAX);
		p.y = clamp(m_p.y() + m_radius * sin(rad), INT16_MIN, INT16_MAX);
		p.r = 0;
		p.g = 0;
		p.b = 0;
		ps.push_back(p);
	}

	return ps;
}

void laser::Circle::rotate(double rad)
{
	m_startRatio += rad;
	m_endRatio += rad;
}

void laser::Circle::move(int x, int y)
{
	m_p.setX(m_p.x() + x);
	m_p.setY(m_p.y() + y);
}

