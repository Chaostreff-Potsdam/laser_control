#pragma once

#include <vector>
#include <mutex>

#include <opencv2/core/core.hpp>

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

	/*!
	 * \brief provides access to the
	 * [etherdream](http://www.ether-dream.com/) DAC.
	 *
	 * To connect to the DAC this wrapper uses the low-level C library that is
	 * provided by the etherdream guys.
	 *
	 * This class opens a new thread to stay responsive. All member variables
	 * are either atomic and thus thread-safe by themselves or protected by
	 * mutexes.
	 *
	 * \attention Use this class as if it was a Singleton. Otherwise,
	 * unexpected behaviour might occur.
	 */
	class EtherdreamWrapper
	{
	public:
		static const int pps;

		/*!
		 * \details On construction, this wrapper automatically connects to the
		 * etherdream and starts to write.
		 */
        EtherdreamWrapper();
        EtherdreamWrapper(cv::Mat calibration);
		~EtherdreamWrapper();

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
		 * \brief sends new points to the DAC if needed
		 *
		 * If there are new points since the last write this function updates
		 * the DAC to show #m_points in a loop. Otherwise, it yields this
		 * time slice.
         * This also applies the perspective transform (see setPoints).
		 */
		void writePoints();


	protected:

		/*!
		 * \brief establishes connection to the DAC
		 *
		 * Initialises the etherdream library and connects to the first
		 * available DAC. This process takes atleast 1.2 seconds, because
		 * the etherdream DAC advertises itself only once per second.
		 */
		void connect();

		/*!
		 * \brief protects #m_points
		 */
		std::mutex m_pointsMutex;

		/*!
		 * \brief all points that should be drawn by the laser
		 */
		EtherdreamPoints m_points;

#ifdef _WIN32
		/*!
		 * \brief id of EtherDreamCard
		 */
		int m_cardNum;
#else
		/*!
		 * \brief handle for the DAC
		 */
        struct etherdream *m_etherdream;
#endif
	};
}
