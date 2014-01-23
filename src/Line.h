#ifndef LASERLINE_H
#define LASERLINE_H

#include "Object.h"
#include "laser_utilities.h"

namespace laser {
	class EXPORT_LASER_CONTROL LaserLine : public LaserObject
	{
	public:
		LaserLine(int xa, int ya, int xb, int yb, bool visible = true);
		LaserLine(Point a, Point b, bool visible = true);

		EtherdreamPoints points() const;
		EtherdreamPoints startPoints() const;
		EtherdreamPoints endPoints() const;
		void rotate(double rad);
		void move(int x, int y);

	private:
		Point m_a;
		Point m_b;

		int m_length;
		int m_pointCount;

		bool m_visible;
	};
}

#endif // LASERLINE_H
