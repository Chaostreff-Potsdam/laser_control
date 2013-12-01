#ifndef LASERLINE_H
#define LASERLINE_H

#include "LaserObject.h"
#include "laser_utilities.h"

namespace laser {
	class LaserLine : public LaserObject
	{
	public:
		LaserLine(int xa, int ya, int xb, int yb, bool visible = true);
		LaserLine(Point a, Point b, bool visible = true);

		std::vector<etherdream_point> points() const;
		std::vector<etherdream_point> startPoints() const;
		std::vector<etherdream_point> endPoints() const;
		void rotate(double rad);
		void move(int x, int y);

	private:
		Point m_a;
		Point m_b;

		bool m_visible;
	};
}

#endif // LASERLINE_H
