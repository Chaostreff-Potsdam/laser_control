#include "LaserPolygon.h"

#include <vector>

#include "LaserLine.h"

laser::LaserPolygon::LaserPolygon()
:	LaserObject()
{

}

laser::LaserPolygon::LaserPolygon(std::vector<laser::Point> points)
:	LaserObject()
{
	m_corners = points;
}

std::vector<etherdream_point> laser::LaserPolygon::points() const
{
	std::vector<etherdream_point> ps;

	// TODO refine this

	std::vector<Point>::const_iterator start = m_corners.begin();
	std::vector<Point>::const_iterator end = m_corners.end();

	LaserLine startLine = LaserLine(*start, *(start + 1));
	std::vector<etherdream_point> points = startLine.points();
	std::vector<etherdream_point> endPoints = startLine.endPoints();
	ps.insert(ps.end(), points.begin(), points.end());
	ps.insert(ps.end(), endPoints.begin(), endPoints.end());

	for (auto it = start + 1; it < end - 1; it++)
	{
		LaserLine con = LaserLine(*it, *(it + 1));
		points = con.points();
		endPoints = con.endPoints();
		std::vector<etherdream_point> startPoints = con.startPoints();
		ps.insert(ps.end(), startPoints.begin(), startPoints.end());
		ps.insert(ps.end(), points.begin(), points.end());
		ps.insert(ps.end(), endPoints.begin(), endPoints.end());
	}

	LaserLine top = LaserLine(*(end - 1), *start);
	points = top.points();
	std::vector<etherdream_point> startPoints = top.startPoints();
	ps.insert(ps.end(), startPoints.begin(), startPoints.end());
	ps.insert(ps.end(), points.begin(), points.end());

	return ps;
}

std::vector<etherdream_point> laser::LaserPolygon::startPoints() const
{
	return LaserLine(m_corners[0], m_corners[1]).startPoints();
}

std::vector<etherdream_point> laser::LaserPolygon::endPoints() const
{
	return LaserLine(m_corners.back(), m_corners.front()).endPoints();
}

void laser::LaserPolygon::rotate(double rad)
{
	for (auto it = m_corners.begin(); it < m_corners.end(); it++)
	{
		int oldX = it->x();
		int oldY = it->y();

		it->setX(round(cos(rad) * oldX - sin(rad) * oldY));
		it->setY(round(sin(rad) * oldX + cos(rad) * oldY));
	}
}

void laser::LaserPolygon::move(int x, int y)
{
	for (auto it = m_corners.begin(); it < m_corners.end(); it++)
	{
		it->setX(it->x() + x);
		it->setY(it->y() + y);
	}
}
