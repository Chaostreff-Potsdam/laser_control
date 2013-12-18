#ifndef ETHERDREAMWRAPPER_H
#define ETHERDREAMWRAPPER_H

#include <vector>
#include <thread>
#include <mutex>
#include <atomic>

#include <opencv/cv.h>

#include "etherdream.h"

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
	class EtherdreamWrapper
	{
	public:
		/*!
		 * \details On construction, this wrapper automatically connects to the
		 * etherdream and starts to write.
		 */
        EtherdreamWrapper();
        EtherdreamWrapper(cv::Mat calibration);
		~EtherdreamWrapper();

        /*!
         * \brief sets the homography to be applied to all painted points
         */
        void setCalibration(cv::Mat calibration);

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
		void setPoints(std::vector<etherdream_point> &p);
		/*!
		 * \brief appends the contents of \a p to #m_points
         *
         * This function also applies the perspective transform (using m_calibration)
         * to each given point.
         * During this  the i, u1 and u2 value of the etherdream_point are omitted.
		 */
		void addPoints(std::vector<etherdream_point> const&p);
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
        std::shared_ptr<std::mutex> m_pointsMutex;

		/*!
		 * \brief all points that should be drawn by the laser
		 */
		std::vector<etherdream_point> m_points;

		/*!
		 * \brief handle for the DAC
		 */
		struct etherdream *m_etherdream;

        /*!
         * \brief transformation matrix to eliminate keystone and scale the image
         */
        cv::Mat m_calibration;
	};
}
#endif // ETHERDREAMWRAPPER_H
