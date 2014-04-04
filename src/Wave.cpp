#include "Wave.h"

#ifndef LASERWRAPPER_WAVE_POINTS
#define LASERWRAPPER_WAVE_POINTS (100)
#endif

#include <cmath>

#include "laser_utilities.h"

laser::Wave::Wave(Point p1, Point p2, int amplitude, int frequency)
	: Object(),
	  m_amplitude(amplitude),
	  m_frequency(frequency)
{
	m_p1 = p1;
	m_p2 = p2;

	float length = std::sqrt(sqr(m_p1.x() - m_p2.x()) + sqr(m_p1.y() - m_p2.y()));
	m_pointCount = (int)(length/LASERWRAPPER_WAVE_POINTS);
}

laser::EtherdreamPoints laser::Wave::points() const
{
	EtherdreamPoints ps;
	float angle = atan2(m_p2.y() - m_p1.y(), m_p2.x() - m_p1.x());

	for (int i = 0; i < m_pointCount; ++i)
	{
		etherdream_point p;


		int lineX = m_p1.x() + ((float)i)/m_pointCount * (m_p2.x() - m_p1.x());
		int lineY = m_p1.y() + ((float)i)/m_pointCount * (m_p2.y() - m_p1.y());

		int rawSineY = (float)m_amplitude * sin((float)i*LASERWRAPPER_WAVE_POINTS/m_frequency * 2*M_PI);

		std::cout << rawSineY << std::endl;

		int sineX = - sin(angle)*rawSineY;
		int sineY = cos(angle)*rawSineY;


		p.x = clamp(lineX + sineX, INT16_MIN, INT16_MAX);
		p.y = clamp(lineY + sineY, INT16_MIN, INT16_MAX);

		bool pointVisible = true;
		if(p.x == INT16_MIN || p.x == INT16_MAX || p.y == INT16_MIN || p.y == INT16_MAX)
			pointVisible = false;

		p.r = 0;
		p.g = pointVisible ? UINT16_MAX : 0;
		p.b = 0;
		ps.push_back(p);
	}

	return ps;
}

laser::EtherdreamPoints laser::Wave::startPoints() const
{
	EtherdreamPoints ps;

	for (int i = - m_pointCount/5; i < 0; i++)
	{
		etherdream_point p;

		p.x = clamp(m_p1.x() + ((float)i)/LASERWRAPPER_WAVE_POINTS * (m_p2.x() - m_p1.x()), INT16_MIN, INT16_MAX);
		p.y = clamp(m_p1.y() + ((float)i)/LASERWRAPPER_WAVE_POINTS * (m_p2.y() - m_p1.y()), INT16_MIN, INT16_MAX);
		p.r = 0;
		p.g = 0;
		p.b = 0;
		ps.push_back(p);
	}

	return ps;
}

laser::EtherdreamPoints laser::Wave::endPoints() const
{
	EtherdreamPoints ps;

	for (int i = LASERWRAPPER_WAVE_POINTS; i < LASERWRAPPER_WAVE_POINTS + m_pointCount/5; i++)
	{
		etherdream_point p;

		p.x = clamp(m_p1.x() + ((float)i)/LASERWRAPPER_WAVE_POINTS * (m_p2.x() - m_p1.x()), INT16_MIN, INT16_MAX);
		p.y = clamp(m_p1.y() + ((float)i)/LASERWRAPPER_WAVE_POINTS * (m_p2.y() - m_p1.y()), INT16_MIN, INT16_MAX);
		p.r = 0;
		p.g = 0;
		p.b = 0;
		ps.push_back(p);
	}

	return ps;
}
