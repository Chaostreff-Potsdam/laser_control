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

	LaserLine bottom = LaserLine(m_corners[0], m_corners[1]);
	std::vector<etherdream_point> points = bottom.points();
	std::vector<etherdream_point> endPoints = bottom.endPoints();
	std::vector<etherdream_point> startPoints = bottom.startPoints();
	ps.insert(ps.end(), points.begin(), points.end());
	ps.insert(ps.end(), endPoints.begin(), endPoints.end());

	LaserLine right = LaserLine(m_corners[1], m_corners[2]);
	points = right.points();
	endPoints = right.endPoints();
	startPoints = right.startPoints();
	ps.insert(ps.end(), startPoints.begin(), startPoints.end());
	ps.insert(ps.end(), points.begin(), points.end());
	ps.insert(ps.end(), endPoints.begin(), endPoints.end());

	LaserLine top = LaserLine(m_corners[2], m_corners[3]);
	points = top.points();
	endPoints = top.endPoints();
	startPoints = top.startPoints();
	ps.insert(ps.end(), startPoints.begin(), startPoints.end());
	ps.insert(ps.end(), points.begin(), points.end());
	ps.insert(ps.end(), endPoints.begin(), endPoints.end());

	LaserLine left = LaserLine(m_corners[3], m_corners[0]);
	points = left.points();
	endPoints = left.endPoints();
	startPoints = left.startPoints();
	ps.insert(ps.end(), startPoints.begin(), startPoints.end());
	ps.insert(ps.end(), points.begin(), points.end());

	return ps;
}

std::vector<etherdream_point> LaserRectangle::startPoints() const
{
	return LaserLine(m_corners[0], m_corners[1]).startPoints();

//	for (auto it = ps.begin(); it < ps.end(); it++)
//	{
//		it->g = 0;
//	}

//	return ps;
}

std::vector<etherdream_point> LaserRectangle::endPoints() const
{
	return LaserLine(m_corners[3], m_corners[0]).endPoints();

//	for (auto it = ps.begin(); it < ps.end(); it++)
//	{
//		it->g = 0;
//	}

//	return ps;
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
