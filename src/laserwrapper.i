%module laserwrappercs
%{
#include "LaserPainter.h"
%}

class LaserPainter
{
public:
	LaserPainter();

	void aquireEtherdreamWrapper();
	void updatePoints();
	void deleteObject(int id);
	void drawWall(int id, int Ax, int Ay, int Bx, int By, int Px, int Py);
};
