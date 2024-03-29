#pragma once

#include "Object.h"
#include <vector>

namespace laser {
	class EXPORT_LASER_CONTROL Polygon : public Object
	{
	public:
		Polygon(const std::vector<Point> & points, bool sharp = true, bool dashed = false, bool closed = true);

		EtherdreamPoints points() const;
		EtherdreamPoints startPoints() const;
		EtherdreamPoints endPoints() const;

	protected:
		Polygon(bool sharp = true, bool dashed = false, bool closed = true);

		std::vector<Point> m_corners;
		bool m_sharpCorners;
		bool m_dashed;
		bool m_closed;
	};
}
