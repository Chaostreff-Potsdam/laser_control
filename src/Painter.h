#ifndef LASERPAINTER_H
#define LASERPAINTER_H

#include "EtherdreamWrapper.h"
#include "Object.h"
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
	class EXPORT_LASER_CONTROL Painter
	{
		typedef std::pair<int, ObjectPtr> ObjectPtrPair;
		typedef std::map<int, ObjectPtr> ObjectPtrMap;

	public:
		Painter(bool expireObjects = false);
		~Painter();

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
		void paintOn(const std::shared_ptr<EtherdreamWrapper> & e);

		/*!
		 * \brief overrides #m_objects with \a objects and calls updatePoints()
		 */
		void paint(const ObjectPtrMap &objects);

		/*!
		 * \brief adds \a object to #m_objects and calls updatePoints()
		 */
		int add(const ObjectPtr & object);

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
		 * \brief Clear the whole scene
		 */
		void deleteAll();

		/*!
		 * \brief draw a rectangle with ID \a id and corners \a p1 to \a p4
		 */
		ObjectPtr drawWall(int id, Point p1, Point p2);

		ObjectPtr drawDoor(int id, Point p1, Point p2);

		ObjectPtr drawTable(int id, Point p1, Point p2, Point p3, Point p4);

		ObjectPtr drawPlayer(int id, Point p1);

		ObjectPtr drawButton(int id, Point p);

	protected:
		std::thread m_updateLoop;
		void updateLoop();

		std::mutex m_updateMutex;

		/*!
		 * \brief the EtherdreamWrapper the objects will be painted on
		 */
		std::shared_ptr<EtherdreamWrapper> m_canvas;

		/*!
		 * \brief multiple LaserObjects that are uniquely idetified by an id
		 */
		ObjectPtrMap m_objects;

		/*!
		 * \brief the smallest number that is free no matter what
		 */
		int m_smallestFreeId;
        bool m_expireObjects;

		cv::Mat m_calibration;

	private:
		void removeExpiredObjects();
		bool m_running;

	};
}

#endif // LASERPAINTER_H
