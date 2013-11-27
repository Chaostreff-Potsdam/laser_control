#ifndef LASERLINE_H
#define LASERLINE_H

#include "LaserObject.h"

class LaserLine : public LaserObject
{
public:
	LaserLine(int xa, int ya, int xb, int yb, bool visible = true);

	std::vector<etherdream_point> points() const;
	std::vector<etherdream_point> startPoints() const;
	std::vector<etherdream_point> endPoints() const;
	void rotate(double rad);
	void move(int x, int y);

private:
	int m_xa;
	int m_ya;

	int m_xb;
	int m_yb;

	bool m_visible;
};

#endif // LASERLINE_H
