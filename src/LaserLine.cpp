#include "LaserLine.h"

#include <vector>
#include <cmath>

#ifndef LASERWRAPPER_LINE_POINTS
#define LASERWRAPPER_LINE_POINTS (50)
#endif

LaserLine::LaserLine(int xa, int ya, int xb, int yb, bool visible) : LaserObject()
{
	m_xa = xa;
	m_ya = ya;

	m_xb = xb;
	m_yb = yb;

	m_visible = visible;
}

std::vector<etherdream_point> LaserLine::points() const
{
	std::vector<etherdream_point> ps;
	void rotate(double rad);
	void move(int x, int y);
	if (m_visible)
	{
		for (int i = 0; i < LASERWRAPPER_LINE_POINTS; i++)
		{
			etherdream_point p;

			p.x = m_xa + ((float)i)/LASERWRAPPER_LINE_POINTS * (m_xb - m_xa);
			p.y = m_ya + ((float)i)/LASERWRAPPER_LINE_POINTS * (m_yb - m_ya);
			p.r = 0;
			p.g = 65535;
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

			p.x = m_xa + ((float)i)/LASERWRAPPER_LINE_POINTS * (m_xb - m_xa);
			p.y = m_ya + ((float)i)/LASERWRAPPER_LINE_POINTS * (m_yb - m_ya);
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

			p.x = m_xa + ((float)i)/LASERWRAPPER_LINE_POINTS * (m_xb - m_xa);
			p.y = m_ya + ((float)i)/LASERWRAPPER_LINE_POINTS * (m_yb - m_ya);
			p.r = 0;
			p.g = 0;
			p.b = 0;
			ps.push_back(p);
		}
	}

	return ps;
}

void LaserLine::rotate(double rad)
{
	int oldXa = m_xa;
	int oldYa = m_ya;
	int oldXb = m_xb;
	int oldYb = m_yb;

	m_xa = round(oldXa * cos(rad) - oldYa * sin(rad));
	m_ya = round(oldXa * sin(rad) + oldYa * cos(rad));

	m_xb = round(oldXb * cos(rad) - oldYb * sin(rad));
	m_yb = round(oldXb * sin(rad) + oldYb * cos(rad));
}

void LaserLine::move(int x, int y)
{
	m_xa += x;
	m_xb += x;

	m_ya += y;
	m_yb += y;
}
