#include "GenObject.h"

namespace laser {

GenObject::GenObject(std::initializer_list<Point> ps, int _sharpness) :
	Object(),
	sharpness(_sharpness)
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

void GenObject::duplicatePointAsBlack(etherdream_point p, EtherdreamPoints &dst) const
{
	dst.reserve(sharpness);
	p.r = p.g = p.b = 0;
	for (int i = 0; i < sharpness; i++)
		dst.push_back(p);
}

EtherdreamPoints GenObject::startPoints() const
{
	EtherdreamPoints ps;
	if (!m_ps.empty() && sharpness) {
		duplicatePointAsBlack(m_ps.front(), ps);
	}
	return ps;
}

EtherdreamPoints GenObject::endPoints() const
{
	EtherdreamPoints ps;
	if (!m_ps.empty() && sharpness) {
		duplicatePointAsBlack(m_ps.back(), ps);
	}
	return ps;
}

}
