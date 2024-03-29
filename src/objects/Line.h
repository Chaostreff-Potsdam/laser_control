#pragma once

#include "Object.h"

namespace laser {
	class EXPORT_LASER_CONTROL Line : public Object
	{
	public:
		Line(int xa, int ya, int xb, int yb, bool visible = true, bool dashed = false);
		Line(Point a, Point b, bool visible = true, bool dashed = false);

		EtherdreamPoints points() const;
		EtherdreamPoints startPoints() const;
		EtherdreamPoints endPoints() const;

	private:
		Point m_a;
		Point m_sAB;

		int m_pointCount;

		bool m_visible;
		bool m_dashed;
	};
}
