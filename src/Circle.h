#ifndef LASERCIRCLE_H
#define LASERCIRCLE_H

#include "Object.h"
#include "laser_utilities.h"
#include <cmath>

namespace laser {
	class EXPORT_LASER_CONTROL LaserCircle : public LaserObject
	{
	public:
		LaserCircle(int x, int y, int radius, float startRatio = 0, float endRatio = 2*M_PI);
		LaserCircle(Point p, int radius, float startRatio = 0, float endRatio = 2*M_PI);

		EtherdreamPoints points() const;
		EtherdreamPoints startPoints() const;
		EtherdreamPoints endPoints() const;
		void rotate(double rad);
		void move(int x, int y);

	private:
		Point m_p;
		int m_radius;
		float m_endRatio;
		float m_startRatio;
	};
}

#endif // LASERCIRCLE_H
