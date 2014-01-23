#include "Polygon.h"

#include <vector>

#include "Line.h"

laser::LaserPolygon::LaserPolygon(bool sharp)
:	LaserObject(),
	m_sharpCorners(sharp)
{

}

laser::LaserPolygon::LaserPolygon(const std::vector<laser::Point> & points, bool sharp)
:	LaserObject(),
	m_sharpCorners(sharp)
{
	m_corners = points;
}

laser::EtherdreamPoints laser::LaserPolygon::points() const
{
	EtherdreamPoints ps;

	// TODO refine this

	std::vector<Point>::const_iterator start = m_corners.begin();
	std::vector<Point>::const_iterator end = m_corners.end();

	LaserLine startLine = LaserLine(*start, *(start + 1));
	EtherdreamPoints points = startLine.points();
	EtherdreamPoints endPoints = startLine.endPoints();
	ps.insert(ps.end(), points.begin(), points.end());
	if (m_sharpCorners)
		ps.insert(ps.end(), endPoints.begin(), endPoints.end());

	for (auto it = start + 1; it < end - 1; it++)
	{
		LaserLine con = LaserLine(*it, *(it + 1));
		points = con.points();
		endPoints = con.endPoints();
		EtherdreamPoints startPoints = con.startPoints();
		if (m_sharpCorners)
			ps.insert(ps.end(), startPoints.begin(), startPoints.end());
		ps.insert(ps.end(), points.begin(), points.end());
		if (m_sharpCorners)
			ps.insert(ps.end(), endPoints.begin(), endPoints.end());
	}

	LaserLine top = LaserLine(*(end - 1), *start);
	points = top.points();
	EtherdreamPoints startPoints = top.startPoints();
	if (m_sharpCorners)
		ps.insert(ps.end(), startPoints.begin(), startPoints.end());
	ps.insert(ps.end(), points.begin(), points.end());

	return ps;
}

laser::EtherdreamPoints laser::LaserPolygon::startPoints() const
{
	return LaserLine(m_corners[0], m_corners[1]).startPoints();
}

laser::EtherdreamPoints laser::LaserPolygon::endPoints() const
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
