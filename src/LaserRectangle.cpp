#include "LaserRectangle.h"

#include <vector>

#include "LaserLine.h"


LaserRectangle::LaserRectangle(int x, int y, int height, int width) : LaserObject()
{
	m_x = x;
	m_y = y;
	m_height = height;
	m_width = width;
}

std::vector<etherdream_point> LaserRectangle::points() const
{
	std::vector<etherdream_point> ps;

	// TODO refine this

	std::vector<etherdream_point> bottom = LaserLine(m_x, m_y, m_x + m_width, m_y).points();
	ps.insert(ps.end(), bottom.begin(), bottom.end());
	std::vector<etherdream_point> right = LaserLine(m_x + m_width, m_y, m_x + m_width, m_y + m_height).points();
	ps.insert(ps.end(), right.begin(), right.end());
	std::vector<etherdream_point> top = LaserLine(m_x + m_width, m_y + m_height, m_x, m_y + m_height).points();
	ps.insert(ps.end(), top.begin(), top.end());
	std::vector<etherdream_point> left = LaserLine(m_x, m_y + m_height, m_x, m_y).points();
	ps.insert(ps.end(), left.begin(), left.end());

	return ps;
}

etherdream_point LaserRectangle::startPoint() const
{
	etherdream_point p;

	p.x = m_x;
	p.y = m_y;

	return p;
}

etherdream_point LaserRectangle::endPoint() const
{
	etherdream_point p;

	p.x = m_x;
	p.y = m_y;

	return p;
}
