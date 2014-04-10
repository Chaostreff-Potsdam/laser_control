#pragma once

#include "Object.h"

namespace laser {
	class EXPORT_LASER_CONTROL Wave : public Object
	{
	public:
		Wave(Point p1, Point p2, int amplitude, int frequency);

		EtherdreamPoints points() const;
		EtherdreamPoints startPoints() const;
		EtherdreamPoints endPoints() const;

	protected:
		Point m_p1;
		Point m_s12;

		int m_amplitude;
		int m_frequency;
		int m_pointCount;
	};
}
