#ifndef LASEROBJECT_H
#define LASEROBJECT_H

#include "EtherdreamWrapper.h"
#include "DllExport.h"

#include <memory>

#include "boost/date_time/posix_time/posix_time_types.hpp"


#ifdef _WIN32
static int round(double number)
	{
		return number < 0.0 ? ceil(number-0.5) : floor(number + 0.5);
	}
#endif

namespace laser {


	/*!
	 * \brief an abstract object to be painted
	 */
	class EXPORT_LASER_CONTROL LaserObject
	{
	public:
		LaserObject();

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
		/*!
		 * \brief rotate this object by \f$\frac{180\cdot rad}{\pi}\f$ degrees
		 */
		virtual void rotate(double rad) = 0;
		/*!
		 * \brief move this object by x and y
		 */
		virtual void move(int x, int y) = 0;

		boost::posix_time::ptime started();

		void setPermanent(bool permanent);
		bool permanent();

	protected:
		boost::posix_time::ptime m_started;
		bool m_permanent;
	};

	typedef std::shared_ptr<LaserObject> LaserObjectPtr;
}

#endif // LASEROBJECT_H
