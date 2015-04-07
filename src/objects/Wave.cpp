#include "Wave.h"

#ifndef LASERWRAPPER_WAVE_POINTS
#define LASERWRAPPER_WAVE_POINTS (100)
#endif

laser::Wave::Wave(Point p1, Point p2, int amplitude, int frequency)
	: Object(),
	  m_amplitude(amplitude),
	  m_frequency(frequency)
{
	m_p1 = p1;
	m_s12 = p2 - p1;
	m_pointCount = (int)(m_s12.abs()/LASERWRAPPER_WAVE_POINTS);
}

laser::EtherdreamPoints laser::Wave::points() const
{
	EtherdreamPoints ps;
	float angle = atan2(m_s12.y(), m_s12.x());

	for (int i = 0; i < m_pointCount; ++i)
	{
		Point pointOnLine = m_p1 + m_s12 * ((float)i)/m_pointCount;
		Point pointOnCircle(-sin(angle), cos(angle));

		int rawSineY = (float)m_amplitude * sin((float)i*LASERWRAPPER_WAVE_POINTS/m_frequency * 2*M_PI);

		ps.push_back(etherdreamPoint(pointOnLine + pointOnCircle * rawSineY));
	}

	return ps;
}

laser::EtherdreamPoints laser::Wave::startPoints() const
{
	EtherdreamPoints ps;

	for (int i = - m_pointCount/5; i < 0; i++)
	{
		ps.push_back(etherdreamPoint(m_p1 + m_s12 * ((float)i)/LASERWRAPPER_WAVE_POINTS, false));
	}

	return ps;
}

laser::EtherdreamPoints laser::Wave::endPoints() const
{
	EtherdreamPoints ps;

	for (int i = LASERWRAPPER_WAVE_POINTS; i < LASERWRAPPER_WAVE_POINTS + m_pointCount/5; i++)
	{
		ps.push_back(etherdreamPoint(m_p1 + m_s12 * ((float)i)/LASERWRAPPER_WAVE_POINTS, false));
	}

	return ps;
}
