#ifndef LASERRECTANGLE_H
#define LASERRECTANGLE_H

#include "LaserObject.h"

class LaserRectangle : public LaserObject
{
public:
	LaserRectangle(int x, int y, int height, int width);

	etherdream_point startPoint() const;
	etherdream_point endPoint() const;
	std::vector<struct etherdream_point> points() const;

private:
	int m_x;
	int m_y;
	int m_height;
	int m_width;
};

#endif // LASERRECTANGLE_H
