#pragma once

#ifdef __GNUC__
#define LASER_DEPRECATED __attribute__((deprecated))
#elif defined(_MSC_VER)
#define LASER_DEPRECATED __declspec(deprecated)
#else
#pragma message("WARNING: You need to implement DEPRECATED for this compiler")
#define LASER_DEPRECATED
#endif

#include <cstdint>
#include <cmath>
#include <opencv2/core/core.hpp>
#include <iostream>

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

	template <typename T>
	int sgn(T val) {
		return (T(0) < val) - (val < T(0));
	}

	static inline double sqr(double a)
	{ return a * a; }

}
