#include "LaserCompositeObject.h"

laser::LaserCompositeObject::LaserCompositeObject(std::vector<LaserObjectPtr> objects)
:	LaserObject()
{
	m_objects = objects;
}

std::vector<etherdream_point> laser::LaserCompositeObject::points() const
{
	std::vector<etherdream_point> ps;

	for (auto it = m_objects.begin(); it < m_objects.end(); it++)
	{
		//if (m_objects.size() != 1)
		{
			std::vector<etherdream_point> s = (*it)->startPoints();
			ps.insert(ps.end(), s.begin(), s.end());
		}

		std::vector<etherdream_point> p = (*it)->points();
		ps.insert(ps.end(), p.begin(), p.end());

		//if (m_objects.size() != 1)
		{
			std::vector<etherdream_point> e = (*it)->endPoints();
			ps.insert(ps.end(), e.begin(), e.end());
		}
	}

	return ps;
}

std::vector<etherdream_point> laser::LaserCompositeObject::startPoints() const
{
	return std::vector<etherdream_point>();
}

std::vector<etherdream_point> laser::LaserCompositeObject::endPoints() const
{
	return std::vector<etherdream_point>();
}
