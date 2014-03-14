#ifndef SPIRAL_H
#define SPIRAL_H
#include "Object.h"

namespace laser {
	class EXPORT_LASER_CONTROL Spiral : public Object
	{
	public:
		Spiral(int x, int y, int innerRadius, int outerRadius, float revolutions = 3, bool active = false);
		Spiral(Point p, int innerRadius, int outerRadius, float revolutions = 3, bool active = false);

		EtherdreamPoints points() const;
		EtherdreamPoints startPoints() const;
		EtherdreamPoints endPoints() const;

	private:
		Point m_p;
		int m_innerRadius;
		int m_outerRadius;
		float m_revolutions;
		bool m_active;
	};
}

#endif // SPIRAL_H
