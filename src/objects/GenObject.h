#pragma once

#include "Object.h"
#include <initializer_list>

namespace laser {

	class GenObject;
	typedef std::shared_ptr<GenObject> GenObjectPtr;

	class EXPORT_LASER_CONTROL GenObject: public Object
	{
	public:
		GenObject() : Object() {}
		GenObject(std::initializer_list<Point> ps);

		void clear();
		void addPoints(std::initializer_list<Point> ps);
		void addPoint(double x, double y);
		void addPoint(const Point & p)
		{ addPoint(p.x(), p.y()); }

		virtual EtherdreamPoints points() const
		{ return m_ps; }

		virtual EtherdreamPoints startPoints() const
		{ return EtherdreamPoints(); }

		virtual EtherdreamPoints endPoints() const
		{ return EtherdreamPoints(); }

	private:
		EtherdreamPoints m_ps;
	};

}
