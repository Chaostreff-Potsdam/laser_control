%module laserwrappercs
%{
#include "LaserPainter.h"
#include "laser_utilities.h"
%}

namespace laser
{
	class Point
	{
	public:
		Point();
		Point(int x, int y);

		int x() const;
		int y() const;

		void setX(int x);
		void setY(int y);
	};

	class LaserPainter
	{
	public:
		LaserPainter();

		void aquireEtherdreamWrapper();
		void updatePoints();
		void deleteObject(int id);
		void drawWall(int id, int Ax, int Ay, int Bx, int By, int Px, int Py);
		void drawWall(int id, Point p1, Point p2, Point p3, Point p4);
	};
}
