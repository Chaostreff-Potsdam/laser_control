#include "LaserRectangle.h"

#include <vector>
#include <utility>
#include <cmath>

#include "LaserLine.h"


LaserRectangle::LaserRectangle(int x, int y, int width, int height) : LaserObject()
{
	m_corners.push_back(std::make_pair(x, y));
	m_corners.push_back(std::make_pair(x + width, y));
	m_corners.push_back(std::make_pair(x + width, y + height));
	m_corners.push_back(std::make_pair(x, y + height));
}

std::vector<etherdream_point> LaserRectangle::points() const
{
	std::vector<etherdream_point> ps;

	// TODO refine this

	std::vector<etherdream_point> bottom = LaserLine(m_corners[0].first, m_corners[0].second, m_corners[1].first, m_corners[1].second).points();
	ps.insert(ps.end(), bottom.begin(), bottom.end());
	std::vector<etherdream_point> right = LaserLine(m_corners[1].first, m_corners[1].second, m_corners[2].first, m_corners[2].second).points();
	ps.insert(ps.end(), right.begin(), right.end());
	std::vector<etherdream_point> top = LaserLine(m_corners[2].first, m_corners[2].second, m_corners[3].first, m_corners[3].second).points();
	ps.insert(ps.end(), top.begin(), top.end());
	std::vector<etherdream_point> left = LaserLine(m_corners[3].first, m_corners[3].second, m_corners[0].first, m_corners[0].second).points();
	ps.insert(ps.end(), left.begin(), left.end());

	return ps;
}

std::vector<etherdream_point> LaserRectangle::startPoints() const
{
	std::vector<etherdream_point> ps = LaserLine(m_corners[3].first, m_corners[3].second, m_corners[0].first, m_corners[0].second).points();

	for (auto it = ps.begin(); it < ps.end(); it++)
	{
		it->g = 0;
	}

	return ps;
}

std::vector<etherdream_point> LaserRectangle::endPoints() const
{
	std::vector<etherdream_point> ps = LaserLine(m_corners[0].first, m_corners[0].second, m_corners[1].first, m_corners[1].second).points();

	for (auto it = ps.begin(); it < ps.end(); it++)
	{
		it->g = 0;
	}

	return ps;
}

void LaserRectangle::rotate(double rad)
{
	for (auto it = m_corners.begin(); it < m_corners.end(); it++)
	{
		int oldX = it->first;
		int oldY = it->second;

		it->first = round(cos(rad) * oldX - sin(rad) * oldY);
		it->second = round(sin(rad) * oldX + cos(rad) * oldY);
	}
}

void LaserRectangle::move(int x, int y)
{
	for (auto it = m_corners.begin(); it < m_corners.end(); it++)
	{
		it->first += x;
		it->second += y;
	}
}
