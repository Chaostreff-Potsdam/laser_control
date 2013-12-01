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
		void paintOn(std::shared_ptr<EtherdreamWrapper> e);
		void paint(LaserObjectPtrMap objects);
		void add(LaserObjectPtr object);
		void updatePoints();
		void deleteObject(int id);
		void drawWall(int id, int Ax, int Ay, int Bx, int By, int Px, int Py);
		void drawWall(int id, Point p1, Point p2, Point p3, Point p4);

	private:
		std::shared_ptr<EtherdreamWrapper> m_canvas;
		LaserObjectPtrMap m_objects;
		int m_smallestFreeId;
	};
}

#endif // LASERPAINTER_H
