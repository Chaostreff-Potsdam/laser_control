%module laserwrappercs
%{
#include "Painter.h"
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

	class Painter
	{
	public:
		Painter();

		void aquireEtherdreamWrapper();
		void updatePoints();
		void deleteObject(int id);
		void drawWall(int id, Point p1, Point p2);
	};
}
