#include "LaserCircle.h"

#include <vector>
#include <cmath>
#include <cstdint>

#ifndef LASERWRAPPER_CIRCLE_POINTS
#define LASERWRAPPER_CIRCLE_POINTS (50)
#endif

using namespace laser;

LaserCircle::LaserCircle(int x, int y, int radius, float startRatio, float endRatio) : LaserObject()
{
	m_p = Point(x, y);
	m_radius = radius;
	m_startRatio = startRatio;
	m_endRatio = endRatio;
}

LaserCircle::LaserCircle(Point p, int radius, float startRatio, float endRatio)
{
	m_p = p;
	m_radius = radius;
	m_startRatio = startRatio;
	m_endRatio = endRatio;
}

std::vector<etherdream_point> LaserCircle::points() const
{
	std::vector<etherdream_point> ps;

	for (int i = m_startRatio/M_PI*LASERWRAPPER_CIRCLE_POINTS; i < m_endRatio/M_PI*LASERWRAPPER_CIRCLE_POINTS + 1; i++)
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

	for (int i =m_startRatio/M_PI*LASERWRAPPER_CIRCLE_POINTS - 10; i < m_startRatio/M_PI*LASERWRAPPER_CIRCLE_POINTS; i++)
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

	for (int i = m_endRatio/M_PI*LASERWRAPPER_CIRCLE_POINTS; i < m_endRatio/M_PI*LASERWRAPPER_CIRCLE_POINTS + 10; i++)
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
	m_startRatio += rad;
	m_endRatio += rad;
}

void LaserCircle::move(int x, int y)
{
	m_p.setX(m_p.x() + x);
	m_p.setY(m_p.y() + y);
}

