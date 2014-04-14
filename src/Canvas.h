#pragma once
#include <vector>
#include <memory>
#include <mutex>

#ifdef _WIN32
#	include "dac.h"
#	undef DELETE

#	ifndef _CPPUNWIND
#		define _CPPUNWIND //To prevent it from looking for nonextistant throw_exception
#	endif
#else // _WIN32
#	include "etherdream.h"
#endif // _WIN32

namespace laser {

typedef std::vector<etherdream_point> EtherdreamPoints;

	class Canvas
	{
	public:
		Canvas() {}
		virtual ~Canvas() {}

		/*!
		 * \brief clears #m_points
		 */
		void clear();
		/*!
		 * \brief checks whether #m_points is empty
		 * \return #m_points.empty()
		 */
		bool empty();
		/*!
		 * \brief overwrites the old #m_points and sets it anew
		 * \param p new #m_points
		 */
		void setPoints(const EtherdreamPoints &p);
		/*!
		 * \brief appends the contents of \a p to #m_points
		 */
		void addPoints(const EtherdreamPoints &p);

		/*!
		 * \brief sends the current #m_points to the output device
		 */
		virtual void writePoints() = 0;

	protected:
		/*!
		 * \brief protects #m_points
		 */
		std::mutex m_pointsMutex;

		/*!
		 * \brief all points that should be drawn by the laser
		 */
		EtherdreamPoints m_points;
	};

	typedef std::shared_ptr<Canvas> CanvasPtr;

}
