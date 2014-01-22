#include "LaserCompositeObject.h"
#include "laser_utilities.h"

laser::LaserCompositeObject::LaserCompositeObject(const std::vector<LaserObjectPtr> & objects)
	: LaserObject(),
	  m_transform(2, 3, CV_32FC1)
{
	m_objects = objects;
}

std::vector<etherdream_point> laser::LaserCompositeObject::points() const
{
	std::vector<etherdream_point> ps;

	for (const auto & obj : m_objects)
	{
		appendToVector(ps, obj->startPoints());
		appendToVector(ps, obj->points());
		appendToVector(ps, obj->endPoints());
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

void laser::LaserCompositeObject::rotate(double rad)
{

}

void laser::LaserCompositeObject::move(int x, int y)
{

}

void laser::LaserCompositeObject::addObject(const laser::LaserObjectPtr &object)
{
	m_objects.push_back(object);
}

