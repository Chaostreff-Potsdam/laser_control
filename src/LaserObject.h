#ifndef LASEROBJECT_H
#define LASEROBJECT_H

#ifdef _WIN32
	#include "dac.h"

	#undef DELETE

	#ifndef _CPPUNWIND
	#define _CPPUNWIND //To prevent it from looking for nonextistant throw_exception
	#endif
#else // _WIN32
	#include "etherdream.h"
#endif // _WIN32

#include "DllExport.h"

#include <vector>
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

		boost::posix_time::ptime started();

	protected:
		boost::posix_time::ptime m_started;
	};

	typedef std::shared_ptr<LaserObject> LaserObjectPtr;
}

#endif // LASEROBJECT_H
