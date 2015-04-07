#include "Canvas.h"

#include "utils.h"

namespace laser {

bool Canvas::empty()
{
	std::lock_guard<std::mutex> guard(m_pointsMutex);

	return m_points.empty();
}

void Canvas::clear()
{
	std::lock_guard<std::mutex> guard(m_pointsMutex);

	m_points.clear();
}

void Canvas::setPoints(const EtherdreamPoints &p)
{
	std::lock_guard<std::mutex> guard(m_pointsMutex);

	m_points = p;
}

void Canvas::addPoints(const EtherdreamPoints &p)
{
	std::lock_guard<std::mutex> guard(m_pointsMutex);

	appendToVector(m_points, p);
}

}
