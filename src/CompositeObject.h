#pragma once

#include "Object.h"

namespace laser {

	class EXPORT_LASER_CONTROL CompositeObject : public Object
	{
	public:		
		static CompositeObjectPtr construct(const std::vector<ObjectPtr> & objects = std::vector<ObjectPtr>());

		//!< Add this object and take overship
		void add(Object *object);
		void add(const ObjectPtr & object);
		void add(const std::vector<ObjectPtr> & objects);

		void removeChild(const ObjectPtr & object);

	protected:
		CompositeObject();

		EtherdreamPoints points() const;
		EtherdreamPoints startPoints() const;
		EtherdreamPoints endPoints() const;

	private:
		std::vector<ObjectPtr> m_children;
		std::weak_ptr<CompositeObject> self;

		void removeChild(const Object *object);
		friend void Object::setParent(const parent_t &);
	};
}
