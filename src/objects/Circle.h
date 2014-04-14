#pragma once

#include "Object.h"
#include <cmath>

namespace laser {
	class EXPORT_LASER_CONTROL Circle : public Object
	{
	public:
		Circle(int x, int y, int radius, float startRatio = 0, float endRatio = 2*M_PI);
		Circle(Point p, int radius, float startRatio = 0, float endRatio = 2*M_PI);

		EtherdreamPoints points() const;
		EtherdreamPoints startPoints() const;
		EtherdreamPoints endPoints() const;

	private:
		Point m_p;
		int m_radius;
		float m_endRatio;
		float m_startRatio;
	};
}
