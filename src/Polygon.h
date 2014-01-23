#ifndef LASERPOLYLINE_H
#define LASERPOLYLINE_H

#include "Object.h"

#include <vector>

#include "laser_utilities.h"

namespace laser {
	class EXPORT_LASER_CONTROL LaserPolygon : public LaserObject
	{
	public:
		LaserPolygon(bool sharp = true);
		LaserPolygon(const std::vector<Point> & points, bool sharp = true);

		EtherdreamPoints points() const;
		EtherdreamPoints startPoints() const;
		EtherdreamPoints endPoints() const;
		void rotate(double rad);
		void move(int x, int y);

	protected:
		std::vector<Point> m_corners;
		bool m_sharpCorners;
	};
}

#endif // LASERPOLYLINE_H
