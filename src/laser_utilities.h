#ifndef LASER_UTILITIES_H
#define LASER_UTILITIES_H

#include <cstdint>
#include <cmath>

namespace laser {
	/*!
	 * \brief ensures that \f$ l \leq a \leq u \f$
	 * \param a value to be clamped
	 * \param l lower limit
	 * \param u upper limit
	 */
	static inline int clamp(int a, int l, int u)
	{
		a = a > u ? u : a;
		return a < l ? l : a;
	}

	template<typename T>
	static inline void appendToVector(std::vector<T> & vec, const std::vector<T> & other)
	{
		vec.insert(vec.end(), other.begin(), other.end());
	}

	template<typename T>
	static inline double degrees(T radians)
	{
		return 180.0 * (radians / M_PI);
	}

	template<typename T>
	static inline double radians(T degrees)
	{
		return M_PI * (degrees / 180.0);
	}

#ifdef _WIN32
	template<typename T>
	static int round(T number)
	{
		return number < 0.0 ? ceil(number - 0.5) : floor(number + 0.5);
	}
#endif

	static inline int_least64_t sqr(int a)
	{
		return ((int_least64_t)a)*a;
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
			m_x = x;
			m_y = y;
		}

		int x() const {return m_x;}
		int y() const {return m_y;}

		void setX(int x) {m_x = x;}
		void setY(int y) {m_y = y;}

		float abs() const
		{ return std::sqrt(sqr(m_x) + sqr(m_y)); }

		Point operator+(const Point & other) const
		{ return Point(m_x + other.m_x, m_y + other.m_y); }

		Point operator-(const Point & other) const
		{ return Point(m_x - other.m_x, m_y - other.m_y); }

		Point operator-() const
		{ return Point(-m_x, -m_y); }

		template <typename T>
		Point operator*(const T factor) const
		{ return Point(m_x * factor, m_y * factor); }

		template <typename T>
		Point operator/(const T factor) const
		{ return Point(m_x / factor, m_y / factor); }

		Point & operator+=(const Point & other)
		{
			m_x += other.m_x;
			m_y += other.m_y;
			return *this;
		}

	protected:
		int m_x;
		int m_y;
	};
}

#endif // LASER_UTILITIES_H
