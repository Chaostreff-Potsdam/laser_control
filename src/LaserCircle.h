#ifndef LASERCIRCLE_H
#define LASERCIRCLE_H

#include "LaserObject.h"
#include "laser_utilities.h"

namespace laser {
	class LaserCircle : public LaserObject
	{
	public:
		LaserCircle(int x, int y, int radius);
		LaserCircle(Point p, int radius);

		std::vector<etherdream_point> points() const;
		std::vector<etherdream_point> startPoints() const;
		std::vector<etherdream_point> endPoints() const;
		void rotate(double rad);
		void move(int x, int y);

	private:
		Point m_p;
		int m_radius;
	};
}

#endif // LASERCIRCLE_H
