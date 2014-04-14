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
		const Point onCircle(cos(rad), sin(rad));
		const int currentRadius = m_innerRadius
				+ (m_outerRadius - m_innerRadius) * (rad / (2*M_PI*m_revolutions));
		const bool visible = m_active || (!m_active && fmod(rad, M_PI_4/2) < M_PI_4/4);
		ps.push_back(etherdreamPoint(m_p + onCircle * currentRadius, visible));
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
		const int currentRadius = m_innerRadius
				+ (m_outerRadius - m_innerRadius) * (rad / (2*M_PI*m_revolutions));
		const Point onCircle(cos(rad), sin(rad));
		ps.push_back(etherdreamPoint(m_p + onCircle * currentRadius, false));
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
		const int currentRadius = m_innerRadius
				+ (m_outerRadius - m_innerRadius) * (rad / (2*M_PI*m_revolutions));
		const Point onCircle(cos(rad), sin(rad));
		ps.push_back(etherdreamPoint(m_p + onCircle * currentRadius, false));
	}

	return ps;
}
