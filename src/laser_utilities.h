#ifndef LASER_UTILITIES_H
#define LASER_UTILITIES_H

#include <cstdint>

namespace laser {
	/*!
	 * \brief ensures that \f$ l \leq a \leq u \f$
	 * \param a value to be clamped
	 * \param l lower limit
	 * \param u upper limit
	 */
	inline int clamp(int a, int l, int u)
	{
		a = a > u ? u : a;
		return a < l ? l : a;
	}

	/*!
	 * \brief a 2D point with valid laser coordinates
	 *
	 * This class ensures that the coordinates are valid for the laser.
	 * Any "overflow" will be \link clamp() clamped\endlink to the nearest
	 * valid value.
	 *
	 * Valid values are between INT16_MIN and INT16_MAX.
	 */
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
