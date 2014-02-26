#ifndef LASERPOLYLINE_H
#define LASERPOLYLINE_H

#include "Object.h"

#include <vector>

#include "laser_utilities.h"

namespace laser {
	class EXPORT_LASER_CONTROL Polygon : public Object
	{
	public:
		Polygon(const std::vector<Point> & points, bool sharp = true, bool dashed = false);

		EtherdreamPoints points() const;
		EtherdreamPoints startPoints() const;
		EtherdreamPoints endPoints() const;

	protected:
		Polygon(bool sharp = true, bool dashed = false);

		std::vector<Point> m_corners;
		bool m_sharpCorners;
		bool m_dashed;
	};
}

#endif // LASERPOLYLINE_H
