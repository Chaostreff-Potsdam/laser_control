#include "CompositeObject.h"


laser::CompositeObject::CompositeObject(const std::vector<ObjectPtr> & objects)
	: Object(),
	  m_objects(objects)
{
	;;
}

laser::EtherdreamPoints laser::CompositeObject::points() const
{
	EtherdreamPoints ps;

	for (const auto & obj : m_objects)
	{
		/*
		appendToVector(ps, obj->startPoints());
		appendToVector(ps, obj->points());
		appendToVector(ps, obj->endPoints());
		*/
		appendToVector(ps, obj->pointsToPaint());
	}
	return ps;
}

laser::EtherdreamPoints laser::CompositeObject::startPoints() const
{
	return EtherdreamPoints();
}

laser::EtherdreamPoints laser::CompositeObject::endPoints() const
{
	return EtherdreamPoints();
}

void laser::CompositeObject::add(const ObjectPtr & object)
{
	m_objects.emplace_back(object);
}

void laser::CompositeObject::add(const std::vector<ObjectPtr> &objects)
{
	appendToVector(m_objects, objects);
}
