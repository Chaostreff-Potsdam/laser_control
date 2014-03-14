#include "Spiral.h"

#include <cmath>

#ifndef LASERWRAPPER_SPIRAL_POINTS_PER_REVOLUTION
#define LASERWRAPPER_SPIRAL_POINTS_PER_REVOLUTION (30)
#endif

laser::Spiral::Spiral(int x, int y, int innerRadius, int outerRadius, float revolutions, bool active)
:	Object(),
  m_p(x, y),
  m_innerRadius(innerRadius),
  m_outerRadius(outerRadius),
  m_revolutions(revolutions),
  m_active(active)
{

}

laser::Spiral::Spiral(laser::Point p, int innerRadius, int outerRadius, float revolutions, bool active)
:	Object(),
  m_p(p),
  m_innerRadius(innerRadius),
  m_outerRadius(outerRadius),
  m_revolutions(revolutions),
  m_active(active)
{

}

laser::EtherdreamPoints laser::Spiral::points() const
{
	EtherdreamPoints ps;

	for (float rad = 0;
		 rad <= 2*M_PI*m_revolutions;
		 rad += 2*M_PI/LASERWRAPPER_SPIRAL_POINTS_PER_REVOLUTION)
	{
		etherdream_point p;

		int currentRadius = m_innerRadius
				+ (m_outerRadius - m_innerRadius) * (rad / (2*M_PI*m_revolutions));

		//std::cout << rad << "; " << currentRadius << std::endl;

		p.x = clamp(m_p.x() + currentRadius * cos(rad), INT16_MIN, INT16_MAX);
		p.y = clamp(m_p.y() + currentRadius * sin(rad), INT16_MIN, INT16_MAX);

		bool pointVisible = true;
		if(p.x == INT16_MIN || p.x == INT16_MAX || p.y == INT16_MIN || p.y == INT16_MAX)
			pointVisible = false;

		p.r = 0;
		p.g = pointVisible ? (m_active ? UINT16_MAX : (fmod(rad, M_PI_4/2) < M_PI_4/4 ? UINT16_MAX : 0)) : 0;
		p.b = 0;
		ps.push_back(p);
	}

	return ps;
}

laser::EtherdreamPoints laser::Spiral::startPoints() const
{
	EtherdreamPoints ps;

	for (float rad = -M_PI_2;
		 rad < 0;
		 rad += M_PI_2/20)
	{
		etherdream_point p;

		int currentRadius = m_innerRadius
				+ (m_outerRadius - m_innerRadius) * (rad / (2*M_PI*m_revolutions));

		p.x = clamp(m_p.x() + currentRadius * cos(rad), INT16_MIN, INT16_MAX);
		p.y = clamp(m_p.y() + currentRadius * sin(rad), INT16_MIN, INT16_MAX);
		p.r = 0;
		p.g = 0;
		p.b = 0;
		ps.push_back(p);
	}

	return ps;
}

laser::EtherdreamPoints laser::Spiral::endPoints() const
{
	EtherdreamPoints ps;

	for (float rad = 2*M_PI*m_revolutions;
		 rad < 2*M_PI*m_revolutions + M_PI_2;
		 rad += M_PI_2/20)
	{
		etherdream_point p;

		int currentRadius = m_innerRadius
				+ (m_outerRadius - m_innerRadius) * (rad / (2*M_PI*m_revolutions));

		p.x = clamp(m_p.x() + currentRadius * cos(rad), INT16_MIN, INT16_MAX);
		p.y = clamp(m_p.y() + currentRadius * sin(rad), INT16_MIN, INT16_MAX);
		p.r = 0;
		p.g = 0;
		p.b = 0;
		ps.push_back(p);
	}

	return ps;
}
