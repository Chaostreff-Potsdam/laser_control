#ifndef LASERRECTANGLE_H
#define LASERRECTANGLE_H

#include "Polygon.h"
#include "laser_utilities.h"
#include <vector>

namespace laser {
	class EXPORT_LASER_CONTROL Rectangle : public Polygon
	{
	public:
		Rectangle(int x, int y, int width, int height, bool sharp = true, double rotation = 0);
		Rectangle(Point p1, Point p2, Point p3, Point p4, bool sharp = true);
	};
}

#endif // LASERRECTANGLE_H
