#ifndef LASERCOMPOSITEOBJECT_H
#define LASERCOMPOSITEOBJECT_H

#include "Object.h"
#include "laser_utilities.h"

#include <vector>
#ifndef _WIN32 //MSV 11 does not suppoert initializer_list :(
#include <initializer_list>
#endif
#include <opencv2/core/core.hpp>

namespace laser {

	class EXPORT_LASER_CONTROL LaserCompositeObject;
	typedef std::shared_ptr<LaserCompositeObject> LaserCompositeObjectPtr;

	class LaserCompositeObject : public LaserObject
	{
	public:		
		LaserCompositeObject(const std::vector<LaserObjectPtr> & objects = std::vector<LaserObjectPtr>());

		void add(const LaserObjectPtr & object);
		void add(const std::vector<LaserObjectPtr> & objects);

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
		std::vector<LaserObjectPtr> m_objects;
		cv::Mat m_transform;
	};
}

#endif // LASERCOMPOSITEOBJECT_H
