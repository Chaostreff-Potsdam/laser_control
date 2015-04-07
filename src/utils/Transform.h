#pragma once

#include "EtherdreamWrapper.h"
#include <opencv2/core/core.hpp>

#include <functional>

namespace laser {

	namespace Transform {

		class DistortionInfo {

		public:
			DistortionInfo() :
				m_h(0.0), m_v(0.0)
			{ }

			DistortionInfo(double horizonal, double vertical) :
				m_h(horizonal), m_v(vertical)
			{ compute(); }

			cv::Mat verticalDistCoeff() const
			{ return m_vdC; }

			cv::Mat horizontalDistCoeff() const
			{ return m_hdC; }

			operator bool() const;

		private:
			double m_h;
			double m_v;
			cv::Mat m_vdC, m_hdC;

			cv::Mat makeDistCoeff(double val) const;
			void compute();
		};

		typedef std::function<void(cv::InputArray src, cv::OutputArray dst, cv::InputArray m)>
			OpenCVTransform;

		extern void apply(EtherdreamPoints & points, OpenCVTransform opencvFunc, cv::InputArray transform);
		
		/*!
		 * \brief apply transformation on points. moves all points that afterwards are outside of the canvas to its border and makes them black
		 */
		extern void applyInPlace(EtherdreamPoints & points, OpenCVTransform opencvFunc, cv::InputArray transform);

		/*!
		 * \brief apply transformation on points. moves all points that afterwards are outside of the canvas to its border and makes them black. points that are far away from the canvas are deleted
		 */
		extern EtherdreamPoints applyReturning(EtherdreamPoints & points, OpenCVTransform opencvFunc, cv::InputArray transform);

		/*!
		 * \brief undistorts a laser distortion (horizontal pincussion, vertical linear expansion from center)
		 */
		extern void undistortInPlace(EtherdreamPoints &points, const DistortionInfo & distInfo);
	}

}
