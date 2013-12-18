#ifndef LASERRECTANGLE_H
#define LASERRECTANGLE_H

#include "LaserPolygon.h"
#include "laser_utilities.h"
#include <vector>

namespace laser {
	class LaserRectangle : public LaserPolygon
	{
	public:
		LaserRectangle(int x, int y, int width, int height);
		LaserRectangle(Point p1, Point p2, Point p3, Point p4);
	};
}

#endif // LASERRECTANGLE_H
