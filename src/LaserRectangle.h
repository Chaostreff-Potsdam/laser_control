#ifndef LASERRECTANGLE_H
#define LASERRECTANGLE_H

#include "LaserObject.h"
#include "laser_utilities.h"
#include <vector>
#include <utility>

namespace laser {
	class LaserRectangle : public LaserObject
	{
	public:
		LaserRectangle(int x, int y, int width, int height);
		LaserRectangle(Point p1, Point p2, Point p3, Point p4);

		std::vector<etherdream_point> points() const;
		std::vector<etherdream_point> startPoints() const;
		std::vector<etherdream_point> endPoints() const;
		void rotate(double rad);
		void move(int x, int y);

	private:
		std::vector<Point> m_corners;
	};
}

#endif // LASERRECTANGLE_H
