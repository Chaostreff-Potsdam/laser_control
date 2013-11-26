#ifndef LASERCIRCLE_H
#define LASERCIRCLE_H

#include "LaserObject.h"

class LaserCircle : public LaserObject
{
public:
	LaserCircle(int x, int y, int radius);

	std::vector<etherdream_point> points() const;
	etherdream_point startPoint() const;
	etherdream_point endPoint() const;

private:
	int m_y;
	int m_x;
	int m_radius;
};

#endif // LASERCIRCLE_H
