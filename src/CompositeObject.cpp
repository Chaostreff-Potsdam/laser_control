#include "CompositeObject.h"


laser::CompositeObject::CompositeObject(const std::vector<ObjectPtr> & objects, const ObjectPtr & parent)
	: Object(parent)
{
	add(objects);
}

laser::EtherdreamPoints laser::CompositeObject::points() const
{
	/*EtherdreamPoints ps;

	for (const auto & obj : m_objects)
	{
		appendToVector(ps, obj->pointsToPaint());
	}
	return ps; */
}

laser::EtherdreamPoints laser::CompositeObject::startPoints() const
{
	return EtherdreamPoints();
}

laser::EtherdreamPoints laser::CompositeObject::endPoints() const
{
	return EtherdreamPoints();
}
