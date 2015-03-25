#include "GenObject.h"

namespace laser {

GenObject::GenObject(std::initializer_list<Point> ps) :
	Object()
{
	addPoints(ps);
}

void GenObject::clear()
{
	m_ps.clear();
}

void GenObject::addPoints(std::initializer_list<Point> ps)
{
	m_ps.reserve(m_ps.size() + ps.size());
	for (const auto & p: ps)
		addPoint(p);
}

void GenObject::addPoint(double x, double y)
{
	m_ps.push_back(etherdreamPoint(x, y));
}

void GenObject::setColor(const Color & color)
{
	for (auto & p: m_ps) {
		p.r = color.red();
		p.g = color.green();
		p.b = color.blue();
	}
	Object::setColor(color);
}

}
