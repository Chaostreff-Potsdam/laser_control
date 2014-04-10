#include "Polygon.h"

#include <vector>

#include "Line.h"
#include "laser_utilities.h"

laser::Polygon::Polygon(bool sharp, bool dashed, bool closed)
:	Object(),
	m_sharpCorners(sharp),
	m_dashed(dashed),
	m_closed(closed)
{
	;;
}

laser::Polygon::Polygon(const std::vector<laser::Point> & points, bool sharp, bool dashed, bool closed)
:	Object(),
	m_sharpCorners(sharp),
	m_dashed(dashed),
	m_closed(closed)
{
	m_corners = points;
}

laser::EtherdreamPoints laser::Polygon::points() const
{
	EtherdreamPoints ps;

	auto append = [&ps](const EtherdreamPoints & points) {
		appendToVector(ps, points);
	};

	auto appendIfSharp = [&ps, this](const EtherdreamPoints & points) {
		if (m_sharpCorners)
			appendToVector(ps, points);
	};

	std::vector<Point>::const_iterator start = m_corners.begin();
	std::vector<Point>::const_iterator end = m_corners.end();

	Line startLine = Line(*start, *(start + 1), true, m_dashed);
	append       (startLine.points());
	appendIfSharp(startLine.endPoints());

	for (auto it = start + 1; it < end - 1; it++)
	{
		Line con = Line(*it, *(it + 1), true, m_dashed);
		appendIfSharp(con.startPoints());
		append       (con.points());
		appendIfSharp(con.endPoints());
	}

	if (m_closed)
	{
		Line endLine = Line(*(end - 1), *start, true, m_dashed);
		appendIfSharp(endLine.startPoints());
		append       (endLine.points());
	}

	return ps;
}

laser::EtherdreamPoints laser::Polygon::startPoints() const
{
	return Line(m_corners[0], m_corners[1]).startPoints();
}

laser::EtherdreamPoints laser::Polygon::endPoints() const
{
	if (m_closed)
		return Line(m_corners.back(), m_corners.front()).endPoints();
	else
		return Line(*(m_corners.end() - 2), *(m_corners.end() - 1)).endPoints();
}
