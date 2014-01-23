#ifndef LASERRECTANGLE_H
#define LASERRECTANGLE_H

#include "Polygon.h"
#include "laser_utilities.h"
#include <vector>

namespace laser {
	class EXPORT_LASER_CONTROL LaserRectangle : public LaserPolygon
	{
	public:
		LaserRectangle(int x, int y, int width, int height, bool sharp = true);
		LaserRectangle(Point p1, Point p2, Point p3, Point p4, bool sharp = true);
	};
}

#endif // LASERRECTANGLE_H
