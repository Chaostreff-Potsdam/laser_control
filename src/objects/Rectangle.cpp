#include "Rectangle.h"

#include <vector>

#include "Polygon.h"

laser::Rectangle::Rectangle(int x, int y, int width, int height, bool sharp, double rotation)
:	Polygon(sharp)
{
	m_corners.push_back(Point(x, y));
	m_corners.push_back(Point(x + width, y));
	m_corners.push_back(Point(x + width, y + height));
	m_corners.push_back(Point(x, y + height));
	rotate(rotation, x + width/2, y + height/2);
}

laser::Rectangle::Rectangle(Point p1, Point p2, Point p3, Point p4, bool sharp)
:	Polygon(sharp)
{
	m_corners.push_back(p1);
	m_corners.push_back(p2);
	m_corners.push_back(p3);
	m_corners.push_back(p4);
}

laser::Rectangle::Rectangle(int p1x, int p1y, int p2x, int p2y, int p3x, int p3y, int p4x, int p4y, bool sharp)
:	Polygon(sharp)
{
	m_corners.push_back(Point(p1x, p1y));
	m_corners.push_back(Point(p2x, p2y));
	m_corners.push_back(Point(p3x, p3y));
	m_corners.push_back(Point(p4x, p4y));
}

