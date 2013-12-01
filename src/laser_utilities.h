#ifndef LASER_UTILITIES_H
#define LASER_UTILITIES_H

#include <cstdint>

namespace laser {
	inline int clamp(int a, int bottom, int top)
	{
		a = a > top ? top : a;
		return a < bottom ? bottom : a;
	}

	class Point
	{
	public:
		Point() {}
		Point(int x, int y)
		{
			m_x = clamp(x, INT16_MIN, INT16_MAX);
			m_y = clamp(y, INT16_MIN, INT16_MAX);
		}

		int x() const {return m_x;}
		int y() const {return m_y;}

		void setX(int x) {m_x = clamp(x, INT16_MIN, INT16_MAX);}
		void setY(int y) {m_y = clamp(y, INT16_MIN, INT16_MAX);}

	protected:
		int m_x;
		int m_y;
	};
}

#endif // LASER_UTILITIES_H
