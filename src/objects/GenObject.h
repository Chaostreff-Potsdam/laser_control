#pragma once

#include "Object.h"
#include <initializer_list>

namespace laser {

	class GenObject;
	typedef std::shared_ptr<GenObject> GenObjectPtr;

	class EXPORT_LASER_CONTROL GenObject: public Object
	{
	public:
		enum { default_sharpness = 9 };
		GenObject(int _sharpness = default_sharpness) : Object(), sharpness(_sharpness) {}
		GenObject(std::initializer_list<Point> ps, int _sharpness = default_sharpness);

		int sharpness;

		void clear();
		void addPoints(std::initializer_list<Point> ps);
		void addPoint(double x, double y);
		void addPoint(const Point & p)
		{ addPoint(p.x(), p.y()); }

		virtual EtherdreamPoints points() const
		{ return m_ps; }

		virtual EtherdreamPoints startPoints() const;
		virtual EtherdreamPoints endPoints() const;

		virtual void setColor(const Color & color);

	private:
		EtherdreamPoints m_ps;

		void duplicatePointAsBlack(etherdream_point p, EtherdreamPoints & dst) const;
	};

}
