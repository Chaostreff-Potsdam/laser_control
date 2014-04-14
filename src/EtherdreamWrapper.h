#pragma once
#include "Canvas.h"

#include <opencv2/core/core.hpp>

namespace laser {

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
	class EtherdreamWrapper: public Canvas
	{
	public:
		static const int pps;

		/*!
		 * \details On construction, this wrapper automatically connects to the
		 * etherdream and starts to write.
		 */
        EtherdreamWrapper();
		virtual ~EtherdreamWrapper();

		/*!
		 * \brief sends new points to the DAC if needed
		 *
		 * If there are new points since the last write this function updates
		 * the DAC to show #m_points in a loop. Otherwise, it yields this
		 * time slice.
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
