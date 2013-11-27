#ifndef LASERCIRCLE_H
#define LASERCIRCLE_H

#include "LaserObject.h"

class LaserCircle : public LaserObject
{
public:
	LaserCircle(int x, int y, int radius);

	std::vector<etherdream_point> points() const;
	std::vector<etherdream_point> startPoints() const;
	std::vector<etherdream_point> endPoints() const;
	void rotate(double rad);
	void move(int x, int y);

private:
	int m_y;
	int m_x;
	int m_radius;
};

#endif // LASERCIRCLE_H
