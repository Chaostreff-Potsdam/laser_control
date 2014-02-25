#ifndef LASEROBJECT_H
#define LASEROBJECT_H

#include "EtherdreamWrapper.h"
#include "DllExport.h"

#include <memory>

#include "boost/date_time/posix_time/posix_time_types.hpp"
#include "laser_utilities.h"

namespace laser {


	/*!
	 * \brief an abstract object to be painted
	 */
	class EXPORT_LASER_CONTROL Object
	{
	public:
		Object();

		/*!
		 * \brief update the point cache, so you'll not need to acces points() etc.
		 */
		void update(); // FIXME: Rename

		boost::posix_time::ptime started();

		void setPermanent(bool permanent);
		bool permanent();

		/*** Transforms ***/
		/*!
		 * \brief rotate this object by \f$\frac{180\cdot rad}{\pi}\f$ degrees
		 */
		void rotate(double rad);
		void rotate(double rad, int centerX, int centerY, double scale = 1);
		void rotate(double rad, const Point & center, double scale = 1)
		{ rotate(rad, center.x(), center.y(), scale); }

		/*!
		 * \brief move this object by x and y
		 */
		void move(int x, int y);
		void scale(double factorX, double factorY);
		void scale(double factor);

		void resetTransform();

		EtherdreamPoints pointsToPaint() const;

	protected:
		boost::posix_time::ptime m_started;
		bool m_permanent;

		/*!
		 * \brief calculate the points to be drawn with the laser projector
		 */
		virtual EtherdreamPoints points() const = 0;
		/*!
		 * \brief calculate the margin before the actual points to be drawn
		 */
		virtual EtherdreamPoints startPoints() const = 0;
		/*!
		 * \brief just like startPoints() for the end
		 */
		virtual EtherdreamPoints endPoints() const = 0;

	private:
		bool m_dirty; // TODO: Do we need that?
		EtherdreamPoints m_points, m_startPoints, m_endPoints;

		cv::Mat m_transform;
	};

	typedef std::shared_ptr<Object> ObjectPtr;
}

#endif // LASEROBJECT_H
