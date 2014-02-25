#ifndef LASERCOMPOSITEOBJECT_H
#define LASERCOMPOSITEOBJECT_H

#include "Object.h"

namespace laser {

	class EXPORT_LASER_CONTROL CompositeObject;
	typedef std::shared_ptr<CompositeObject> CompositeObjectPtr;

	class CompositeObject : public Object
	{
	public:		
		CompositeObject(const std::vector<ObjectPtr> & objects = std::vector<ObjectPtr>());

		void add(const ObjectPtr & object);
		void add(const std::vector<ObjectPtr> & objects);

		EtherdreamPoints points() const;
		EtherdreamPoints startPoints() const;
		EtherdreamPoints endPoints() const;

	protected:
		std::vector<ObjectPtr> m_objects;
	};
}

#endif // LASERCOMPOSITEOBJECT_H
