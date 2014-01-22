#ifndef LASERCOMPOSITEOBJECT_H
#define LASERCOMPOSITEOBJECT_H

#include "LaserObject.h"

#include <vector>
#include <initializer_list>
#include <opencv2/core/core.hpp>

namespace laser {

	class LaserCompositeObject;
	typedef std::shared_ptr<LaserCompositeObject> LaserCompositeObjectPtr;

	class LaserCompositeObject : public LaserObject
	{
	public:
		template<typename... LaserObjectPtrTs>
		LaserCompositeObject(LaserObjectPtrTs... objects) :
			LaserObject()
		{
			resetTransform();
			add(objects...);
		}

		LaserCompositeObject(const std::vector<LaserObjectPtr> & objects);
		LaserCompositeObject(const std::initializer_list<LaserObjectPtr> && objects);

		std::vector<etherdream_point> points() const;
		std::vector<etherdream_point> startPoints() const;
		std::vector<etherdream_point> endPoints() const;
		void rotate(double rad);
		void move(int x, int y);
		void scale(double factorX, double factorY);
		void scale(double factor);

		void resetTransform();

		template<typename LaserObjectPtrT, typename... LaserObjectPtrTs>
		void add(LaserObjectPtrT object, LaserObjectPtrTs... others)
		{
			m_objects.emplace_back(object);
			add(others...);
		}

	protected:
		std::vector<LaserObjectPtr> m_objects;
		cv::Mat m_transform;

	private:
		void add() {}

	};
}

#endif // LASERCOMPOSITEOBJECT_H
