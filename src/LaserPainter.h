#ifndef LASERPAINTER_H
#define LASERPAINTER_H

#include "EtherdreamWrapper.h"
#include "LaserObject.h"
#include "laser_utilities.h"

#include <utility>
#include <memory>
#include <map>

namespace laser {

	/*!
	 * \brief provides higher level abstraction for the laser
	 *
	 * This class manages drawing of LaserObject to the laser.
	 */
	class LaserPainter
	{
		typedef std::pair<int, LaserObjectPtr> LaserObjectPtrPair;
		typedef std::map<int, LaserObjectPtr> LaserObjectPtrMap;

	public:
		LaserPainter();

		/*!
		 * \brief gets a new EtherdreamWrapper
		 *
		 * \attention Be aware that this always builds a new EtherdreamWrapper
		 * and discards the old one. This \em should work fine as the new one
		 * needs atleast a second to initiliase while the old one gets
		 * destroyed. This hasn't been tested yet, though.
		 */
		void aquireEtherdreamWrapper();
		/*!
		 * \brief set a new EtherdreamWrapper
		 *
		 * This overrides the old #m_canvas.
		 */
		void paintOn(std::shared_ptr<EtherdreamWrapper> e);
		/*!
		 * \brief overrides #m_objects with \a objects and calls updatePoints()
		 */
		void paint(LaserObjectPtrMap objects);
		/*!
		 * \brief adds \a object to #m_objects and calls updatePoints()
		 */
		void add(LaserObjectPtr object);
		/*!
		 * \brief send a new array to #m_canvas
		 *
		 * This constructs a new array out of #m_objects. This takes
		 * LaserObject::points() as well as LaserObject::startPoints()
		 * and LaserObject::endPoints() into account. The later ones provide
		 * certain margins for the laser. If they are empty objects will be
		 * connected with ugly lines.
		 */
		void updatePoints();
		void deleteObject(int id);
		void drawWall(int id, Point p1, Point p2, Point p3, Point p4);

	private:
		std::shared_ptr<EtherdreamWrapper> m_canvas;
		LaserObjectPtrMap m_objects;
		int m_smallestFreeId;
	};
}

#endif // LASERPAINTER_H
