#include "LaserCircle.h"

#include <vector>
#include <cmath>
#include <cstdint>

#ifndef LASERWRAPPER_CIRCLE_POINTS
#define LASERWRAPPER_CIRCLE_POINTS (50)
#endif

using namespace laser;

LaserCircle::LaserCircle(int x, int y, int radius) : LaserObject()
{
	m_p = Point(x, y);
	m_radius = radius;
}

LaserCircle::LaserCircle(Point p, int radius)
{
	m_p = p;
	m_radius = radius;
}

std::vector<etherdream_point> LaserCircle::points() const
{
	std::vector<etherdream_point> ps;

	for (int i = 0; i < LASERWRAPPER_CIRCLE_POINTS + 1; i++)
	{
		etherdream_point p;

		p.x = m_p.x() + m_radius * cos(2*M_PI*i/LASERWRAPPER_CIRCLE_POINTS);
		p.y = m_p.y() + m_radius * sin(2*M_PI*i/LASERWRAPPER_CIRCLE_POINTS);
		p.r = 0;
		p.g = UINT16_MAX;
		p.b = 0;
		ps.push_back(p);
	}

	return ps;
}

std::vector<etherdream_point> LaserCircle::startPoints() const
{
	std::vector<etherdream_point> ps;

	for (int i = -10; i < 0; i++)
	{
		etherdream_point p;

		p.x = m_p.x() + m_radius * cos(2*M_PI*i/LASERWRAPPER_CIRCLE_POINTS);
		p.y = m_p.y() + m_radius * sin(2*M_PI*i/LASERWRAPPER_CIRCLE_POINTS);
		p.r = 0;
		p.g = 0;
		p.b = 0;
		ps.push_back(p);
	}

	return ps;
}

std::vector<etherdream_point> LaserCircle::endPoints() const
{
	std::vector<etherdream_point> ps;

	for (int i = LASERWRAPPER_CIRCLE_POINTS; i < LASERWRAPPER_CIRCLE_POINTS + 10; i++)
	{
		etherdream_point p;

		p.x = m_p.x() + m_radius * cos(2*M_PI*i/LASERWRAPPER_CIRCLE_POINTS);
		p.y = m_p.y() + m_radius * sin(2*M_PI*i/LASERWRAPPER_CIRCLE_POINTS);
		p.r = 0;
		p.g = 0;
		p.b = 0;
		ps.push_back(p);
	}

	return ps;
}

void LaserCircle::rotate(double rad)
{
}

void LaserCircle::move(int x, int y)
{
	m_p.setX(m_p.x() + x);
	m_p.setY(m_p.y() + y);
}

