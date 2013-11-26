#ifndef LASERLINE_H
#define LASERLINE_H

#include "LaserObject.h"

class LaserLine : public LaserObject
{
public:
	LaserLine(int xa, int ya, int xb, int yb, bool visible = true);
	LaserLine(LaserObjectPtr start, LaserObjectPtr end, bool visible = false);

	std::vector<etherdream_point> points() const;
	etherdream_point startPoint() const;
	etherdream_point endPoint() const;

private:
	int m_xa;
	int m_ya;

	int m_xb;
	int m_yb;

	bool m_visible;
};

#endif // LASERLINE_H
