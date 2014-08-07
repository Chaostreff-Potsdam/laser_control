#include "CompositeObject.h"

laser::CompositeObject::CompositeObject()
	: Object()
{
	;;
}

void laser::CompositeObject::add(Object *object)
{
	add(ObjectPtr(object));
}

void laser::CompositeObject::add(const ObjectPtr & object)
{
	object->setParent(self.lock());
	m_children.emplace_back(object);
	nowDirty();
}

void laser::CompositeObject::add(const std::vector<ObjectPtr> &objects)
{
	for (ObjectPtr obj: objects)
		add(obj);
}

void laser::CompositeObject::removeChild(const ObjectPtr &object)
{
	m_children.erase(
		std::remove(m_children.begin(), m_children.end(), object),
		m_children.end());
	nowDirty();
}

void laser::CompositeObject::removeChild(const Object *object)
{
	m_children.erase(
		std::remove_if(m_children.begin(), m_children.end(), [=](const ObjectPtr & cur) {
			return cur.get() == object;
		}),  m_children.end());
	nowDirty();
}

laser::EtherdreamPoints laser::CompositeObject::points() const
{
	EtherdreamPoints ps;

	for (auto & child : m_children)
	{
		appendToVector(ps, child->pointsToPaint());
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

