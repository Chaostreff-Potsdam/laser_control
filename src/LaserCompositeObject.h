#ifndef LASERCOMPOSITEOBJECT_H
#define LASERCOMPOSITEOBJECT_H

#include "LaserObject.h"

#include <vector>
#include <opencv2/core/core.hpp>

namespace laser {
	class LaserCompositeObject : public LaserObject
	{
	public:
		LaserCompositeObject(const std::vector<LaserObjectPtr> & objects);

		std::vector<etherdream_point> points() const;
		std::vector<etherdream_point> startPoints() const;
		std::vector<etherdream_point> endPoints() const;
		void rotate(double rad);
		void move(int x, int y);
		void scale(double factorX, double factorY);
		void scale(double factor);

		void addObject(const LaserObjectPtr & object);

	protected:
		std::vector<LaserObjectPtr> m_objects;
		cv::Mat m_transform;
	};
}

#endif // LASERCOMPOSITEOBJECT_H
