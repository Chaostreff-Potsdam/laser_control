#ifndef LASERCIRCLE_H
#define LASERCIRCLE_H

#include "LaserObject.h"
#include "laser_utilities.h"
#include <cmath>

namespace laser {
	class LaserCircle : public LaserObject
	{
	public:
		LaserCircle(int x, int y, int radius, float startRatio = 0, float endRatio = M_PI);
		LaserCircle(Point p, int radius, float startRatio = 0, float endRatio = M_PI);

		std::vector<etherdream_point> points() const;
		std::vector<etherdream_point> startPoints() const;
		std::vector<etherdream_point> endPoints() const;
		void rotate(double rad);
		void move(int x, int y);

	private:
		Point m_p;
		int m_radius;
		float m_endRatio;
		float m_startRatio;
	};
}

#endif // LASERCIRCLE_H
