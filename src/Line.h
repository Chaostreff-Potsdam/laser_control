#ifndef LASERLINE_H
#define LASERLINE_H

#include "Object.h"
#include "laser_utilities.h"

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
		Point m_b;

		int m_length;
		int m_pointCount;

		bool m_visible;
		bool m_dashed;
	};
}

#endif // LASERLINE_H
