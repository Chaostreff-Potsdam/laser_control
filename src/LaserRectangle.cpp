#include "LaserRectangle.h"

#include <vector>
#include <utility>
#include <cmath>

#include "LaserLine.h"
#include "laser_utilities.h"

using namespace laser;

LaserRectangle::LaserRectangle(int x, int y, int width, int height) : LaserObject()
{
	m_corners.push_back(Point(x, y));
	m_corners.push_back(Point(x + width, y));
	m_corners.push_back(Point(x + width, y + height));
	m_corners.push_back(Point(x, y + height));
}

LaserRectangle::LaserRectangle(Point p1, Point p2, Point p3, Point p4)
{
	m_corners.push_back(p1);
	m_corners.push_back(p2);
	m_corners.push_back(p3);
	m_corners.push_back(p4);
}

std::vector<etherdream_point> LaserRectangle::points() const
{
	std::vector<etherdream_point> ps;

	// TODO refine this

	std::vector<etherdream_point> bottom = LaserLine(m_corners[0], m_corners[1]).points();
	ps.insert(ps.end(), bottom.begin(), bottom.end());
	std::vector<etherdream_point> right = LaserLine(m_corners[1], m_corners[2]).points();
	ps.insert(ps.end(), right.begin(), right.end());
	std::vector<etherdream_point> top = LaserLine(m_corners[2], m_corners[3]).points();
	ps.insert(ps.end(), top.begin(), top.end());
	std::vector<etherdream_point> left = LaserLine(m_corners[3], m_corners[0]).points();
	ps.insert(ps.end(), left.begin(), left.end());

	return ps;
}

std::vector<etherdream_point> LaserRectangle::startPoints() const
{
	std::vector<etherdream_point> ps = LaserLine(m_corners[3], m_corners[0]).points();

	for (auto it = ps.begin(); it < ps.end(); it++)
	{
		it->g = 0;
	}

	return ps;
}

std::vector<etherdream_point> LaserRectangle::endPoints() const
{
	std::vector<etherdream_point> ps = LaserLine(m_corners[0], m_corners[1]).points();

	for (auto it = ps.begin(); it < ps.end(); it++)
	{
		it->g = 0;
	}

	return ps;
}

void LaserRectangle::rotate(double rad)
{
	for (auto it = m_corners.begin(); it < m_corners.end(); it++)
	{
		int oldX = it->x();
		int oldY = it->y();

		it->setX(round(cos(rad) * oldX - sin(rad) * oldY));
		it->setY(round(sin(rad) * oldX + cos(rad) * oldY));
	}
}

void LaserRectangle::move(int x, int y)
{
	for (auto it = m_corners.begin(); it < m_corners.end(); it++)
	{
		it->setX(it->x() + x);
		it->setY(it->y() + y);
	}
}
