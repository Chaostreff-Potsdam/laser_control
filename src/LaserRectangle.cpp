#include "LaserRectangle.h"

#include <vector>

#include "laser_utilities.h"
#include "LaserPolygon.h"

laser::LaserRectangle::LaserRectangle(int x, int y, int width, int height)
:	LaserPolygon()
{
	m_corners.push_back(Point(x, y));
	m_corners.push_back(Point(x + width, y));
	m_corners.push_back(Point(x + width, y + height));
	m_corners.push_back(Point(x, y + height));
}

laser::LaserRectangle::LaserRectangle(Point p1, Point p2, Point p3, Point p4)
:	LaserPolygon()
{
	m_corners.push_back(p1);
	m_corners.push_back(p2);
	m_corners.push_back(p3);
	m_corners.push_back(p4);
}
