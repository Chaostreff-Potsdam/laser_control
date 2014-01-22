#include "LaserCircle.h"

#include <vector>
#include <cmath>
#include <cstdint>

#ifndef LASERWRAPPER_CIRCLE_POINTS
#define LASERWRAPPER_CIRCLE_POINTS (50)
#endif

laser::LaserCircle::LaserCircle(int x, int y, int radius, float startRatio, float endRatio)
:	LaserObject()
{
	m_p = Point(x, y);
	m_radius = radius;
	m_startRatio = startRatio;
	m_endRatio = endRatio;
}

laser::LaserCircle::LaserCircle(Point p, int radius, float startRatio, float endRatio)
{
	m_p = p;
	m_radius = radius;
	m_startRatio = startRatio;
	m_endRatio = endRatio;
}

std::vector<etherdream_point> laser::LaserCircle::points() const
{
	std::vector<etherdream_point> ps;

	for (float rad = m_startRatio;
		 rad < m_endRatio;
		 rad += std::abs(m_endRatio - m_startRatio)/LASERWRAPPER_CIRCLE_POINTS)
	{
		etherdream_point p;

		p.x = m_p.x() + m_radius * cos(rad);
		p.y = m_p.y() + m_radius * sin(rad);
		p.r = 0;
		p.g = UINT16_MAX;
		p.b = 0;
		ps.push_back(p);
	}

	return ps;
}

std::vector<etherdream_point> laser::LaserCircle::startPoints() const
{
	std::vector<etherdream_point> ps;

//	if (std::abs(fmod(m_startRatio, 2*M_PI) - fmod(m_endRatio, 2*M_PI)) < 0.01)
//		return ps;

	for (float rad = m_startRatio - M_PI_2;
		 rad < m_startRatio;
		 rad += M_PI_2 / 20)
	{
		etherdream_point p;

		p.x = m_p.x() + m_radius * cos(rad);
		p.y = m_p.y() + m_radius * sin(rad);
		p.r = 0;
		p.g = 0;
		p.b = 0;
		ps.push_back(p);
	}

	return ps;
}

std::vector<etherdream_point> laser::LaserCircle::endPoints() const
{
	std::vector<etherdream_point> ps;

//	if (std::abs(fmod(m_startRatio, 2*M_PI) - fmod(m_endRatio, 2*M_PI)) < 0.01)
//		return ps;

	for (float rad = m_endRatio;
		 rad < m_endRatio + M_PI_2;
		 rad += M_PI_2 / 20)
	{
		etherdream_point p;

		p.x = m_p.x() + m_radius * cos(rad);
		p.y = m_p.y() + m_radius * sin(rad);
		p.r = 0;
		p.g = 0;
		p.b = 0;
		ps.push_back(p);
	}

	return ps;
}

void laser::LaserCircle::rotate(double rad)
{
	m_startRatio += rad;
	m_endRatio += rad;
}

void laser::LaserCircle::move(int x, int y)
{
	m_p.setX(m_p.x() + x);
	m_p.setY(m_p.y() + y);
}

