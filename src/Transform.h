#pragma once

#include "EtherdreamWrapper.h"
#include <opencv2/core/core.hpp>

#include <functional>

namespace laser {

	namespace Transform {

		struct DistortionInfo {
			double k1;
			double h1;

			DistortionInfo() :
				k1(0.0), h1(0.0)
			{}

			DistortionInfo(double _k1, double _h1) :
				k1(_k1), h1(_h1)
			{}

			cv::Mat distCoeff() const;
			operator bool() const;
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
		extern EtherdreamPoints undistort(EtherdreamPoints &points, const DistortionInfo & distInfo);
	}

}
