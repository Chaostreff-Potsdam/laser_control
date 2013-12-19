#ifndef LASERCOMPOSITEOBJECT_H
#define LASERCOMPOSITEOBJECT_H

#include "LaserObject.h"

#include <vector>


namespace laser {
	class LaserCompositeObject : public LaserObject
	{
	public:
		LaserCompositeObject(std::vector<LaserObjectPtr> objects);

		std::vector<etherdream_point> points() const;
		std::vector<etherdream_point> startPoints() const;
		std::vector<etherdream_point> endPoints() const;
        void rotate(double) {}
        void move(int, int) {}

	protected:
		std::vector<LaserObjectPtr> m_objects;
	};
}

#endif // LASERCOMPOSITEOBJECT_H
