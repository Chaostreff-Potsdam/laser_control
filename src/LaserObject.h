#ifndef LASEROBJECT_H
#define LASEROBJECT_H

#include "etherdream.h"
#include <vector>
#include <memory>

namespace laser {

	/*!
	 * \brief an abstract object to be painted
	 */
	class LaserObject
	{
	public:
		LaserObject();

		/*!
		 * \brief calculate the points to be drawn with the laser projector
		 */
		virtual std::vector<etherdream_point> points() const = 0;
		/*!
		 * \brief calculate the margin before the actual points to be drawn
		 */
		virtual std::vector<etherdream_point> startPoints() const = 0;
		/*!
		 * \brief just like startPoints() for the end
		 */
		virtual std::vector<etherdream_point> endPoints() const = 0;
		/*!
		 * \brief rotate this object by \f$\frac{180\cdot rad}{\pi}\f$ degrees
		 */
		virtual void rotate(double rad) = 0;
		/*!
		 * \brief move this object by x and y
		 */
		virtual void move(int x, int y) = 0;
	};

	typedef std::shared_ptr<LaserObject> LaserObjectPtr;
}

#endif // LASEROBJECT_H
