#pragma once

#include "Object.h"

namespace laser {

	#define LASER_OBJECT_GROUP(GroupT) \
	public: \
		static CompositeObjectPtr construct(const std::vector<ObjectPtr> & objects = std::vector<ObjectPtr>()) \
		{ return wrap_(new GroupT, objects); } \
		\
		static CompositeObjectPtr construct(const ObjectPtr & obj) \
		{ return wrap_(new GroupT, obj); } \
		\
		static std::shared_ptr<GroupT> cast(const CompositeObjectPtr & p) \
		{ return std::static_pointer_cast<GroupT>(p); }\
	private:


	class EXPORT_LASER_CONTROL CompositeObject : public Object
	{
		LASER_OBJECT_GROUP(CompositeObject)

	public:
		virtual ~CompositeObject();

		//!< Add this object and take overship
		void add(Object *object);
		void add(const ObjectPtr & object);
		void add(const std::vector<ObjectPtr> & objects);

		void removeChild(const ObjectPtr & object);

		CompositeObjectPtr yourself() const
		{ return self.lock(); }

		virtual void setColor(const Color & color);
		virtual void update();
		const std::vector<ObjectPtr> children() const
		{ return m_children; }

	protected:
		template<typename AddableT>
		static CompositeObjectPtr wrap_(CompositeObject* newGroup, AddableT toAdd)
		{
			CompositeObjectPtr group(newGroup);
			group->self = group;
			group->add(toAdd);
			return group;
		}

		CompositeObject();

		EtherdreamPoints points() const;
		EtherdreamPoints startPoints() const;
		EtherdreamPoints endPoints() const;
		std::vector<ObjectPtr> m_children;
		std::weak_ptr<CompositeObject> self;
		virtual void tick();

	private:
		void removeChild(const Object *object);

		friend void Object::setParent(const parent_t &);
	};
}
