#ifndef LASERCOMPOSITEOBJECT_H
#define LASERCOMPOSITEOBJECT_H

#include "Object.h"
#include "laser_utilities.h"

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

		void rotate(double rad);
		void rotate(double rad, int centerX, int centerY, double scale = 1);
		void rotate(double rad, const Point & center, double scale = 1)
		{ rotate(rad, center.x(), center.y(), scale); }

		void move(int x, int y);
		void scale(double factorX, double factorY);
		void scale(double factor);

		void resetTransform();

	protected:
		std::vector<ObjectPtr> m_objects;
		cv::Mat m_transform;
	};
}

#endif // LASERCOMPOSITEOBJECT_H
