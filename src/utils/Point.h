#pragma once

#include "laser_utilities.h"

namespace laser {

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
		Point(double x, double y)
		{
			m_x = x;
			m_y = y;
		}

		double x() const {return m_x;}
		double y() const {return m_y;}

		void setX(double x) {m_x = x;}
		void setY(double y) {m_y = y;}

		Point scaled(double scale) const
		{ return scaled(scale, scale); }

		Point scaled(double scaleX, double scaleY) const
		{ return Point(m_x * scaleX, m_y * scaleY); }

		double dot(const Point & other) const
		{ return (double) other.m_x * m_x + (double) other.m_y * m_y; }

		double abs() const
		{ return std::sqrt(sqr(m_x) + sqr(m_y)); }

		Point perpendicular() const
		{ return Point(-m_y, m_x); }

		double angle() const
		{ return atan2(m_y, m_x); }

		Point norm() const
		{ return *this / abs(); }

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
		double m_x;
		double m_y;
	};

}

extern std::ostream& operator<< (std::ostream& stream, const laser::Point& point);
