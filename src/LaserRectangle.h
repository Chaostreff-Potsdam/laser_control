#ifndef LASERRECTANGLE_H
#define LASERRECTANGLE_H

#include "LaserObject.h"
#include <vector>
#include <utility>

class LaserRectangle : public LaserObject
{
public:
	LaserRectangle(int x, int y, int width, int height);

	std::vector<etherdream_point> points() const;
	std::vector<etherdream_point> startPoints() const;
	std::vector<etherdream_point> endPoints() const;
	void rotate(double rad);
	void move(int x, int y);

private:
	std::vector<std::pair<int, int>> m_corners;
};

#endif // LASERRECTANGLE_H
