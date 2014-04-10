#pragma once

#include "Polygon.h"
#include "laser_utilities.h"
#include <vector>

namespace laser {
	class EXPORT_LASER_CONTROL Rectangle : public Polygon
	{
	public:
		Rectangle(int x, int y, int width, int height, bool sharp = true, double rotation = 0);
		Rectangle(Point p1, Point p2, Point p3, Point p4, bool sharp = true);
		Rectangle(int p1x, int p1y, int p2x, int p2y, int p3x, int p3y, int p4x, int p4y, bool sharp);
	};
}
