#include "Rectangle.h"

#include <vector>

#include "laser_utilities.h"
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
