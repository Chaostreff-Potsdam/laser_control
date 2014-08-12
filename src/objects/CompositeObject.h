#pragma once

#include "Object.h"

namespace laser {

	#define LASER_GROUP_CONSTRUCT(GroupT) \
	static CompositeObjectPtr construct(const std::vector<ObjectPtr> & objects = std::vector<ObjectPtr>()) \
	{ return wrap_(new GroupT, objects); }

	class EXPORT_LASER_CONTROL CompositeObject : public Object
	{
	public:
		LASER_GROUP_CONSTRUCT(CompositeObject)

		//!< Add this object and take overship
		void add(Object *object);
		void add(const ObjectPtr & object);
		void add(const std::vector<ObjectPtr> & objects);

		void removeChild(const ObjectPtr & object);

	protected:
		static CompositeObjectPtr wrap_(CompositeObject* newGroup, const std::vector<ObjectPtr> &objects)
		{
			CompositeObjectPtr group(newGroup);
			group->self = group;
			group->add(objects);
			return group;
		}

		CompositeObject();

		EtherdreamPoints points() const;
		EtherdreamPoints startPoints() const;
		EtherdreamPoints endPoints() const;
		std::vector<ObjectPtr> m_children;
		std::weak_ptr<CompositeObject> self;

	private:
		void removeChild(const Object *object);

		friend void Object::setParent(const parent_t &);
	};
}
