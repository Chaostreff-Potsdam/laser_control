#ifndef LASERPOLYLINE_H
#define LASERPOLYLINE_H

#include "LaserObject.h"

#include <vector>

#include "laser_utilities.h"

namespace laser {
	class EXPORT_LASER_CONTROL LaserPolygon : public LaserObject
	{
	public:
		LaserPolygon();
		LaserPolygon(std::vector<Point> points);

		std::vector<etherdream_point> points() const;
		std::vector<etherdream_point> startPoints() const;
		std::vector<etherdream_point> endPoints() const;
		void rotate(double rad);
		void move(int x, int y);

	protected:
		std::vector<Point> m_corners;
	};
}

#endif // LASERPOLYLINE_H
