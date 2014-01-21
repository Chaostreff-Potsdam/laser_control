#ifndef LASERPAINTER_H
#define LASERPAINTER_H

#include "EtherdreamWrapper.h"
#include "LaserObject.h"
#include "laser_utilities.h"

#include <utility>
#include <memory>
#include <thread>
#include <mutex>
#include <map>
#include <opencv/cv.h>

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
        LaserPainter(bool expireObjects = false);

		//LaserPainter& operator=(const LaserPainter&) = delete;

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
         * \brief calibrates the LaserPainter to scale and anti-keystone the image
         */
        void calibrate();

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

		/*!
		 * \brief deletes an object from #m_objects with ID \a id
		 */
		void deleteObject(int id);

		/*!
		 * \brief draw a rectangle with ID \a id and corners \a p1 to \a p4
		 */
        void drawWall(int id, Point p1, Point p2);

		void drawDoor(int id, Point p1, Point p2);

		void drawTable(int id, Point p1, Point p2, Point p3, Point p4);

	protected:
		std::shared_ptr<std::thread> m_updateLoop;
		void updateLoop();

		std::mutex m_updateMutex;

		/*!
		 * \brief the EtherdreamWrapper the objects will be painted on
		 */
		std::shared_ptr<EtherdreamWrapper> m_canvas;

		/*!
		 * \brief multiple LaserObjects that are uniquely idetified by an id
		 */
		LaserObjectPtrMap m_objects;

		/*!
		 * \brief the smallest number that is free no matter what
		 */
		int m_smallestFreeId;
        bool m_expireObjects;

        cv::Mat m_calibration;
        /*!
         * \brief apply the perspective transform from m_calibration to the given vector of etherdream_points
         */
		void applyCalibration(std::vector<etherdream_point> &);

	};
}

#endif // LASERPAINTER_H
