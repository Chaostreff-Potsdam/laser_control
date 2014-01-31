#pragma once

#include "EtherdreamWrapper.h"

#include <functional>

namespace laser {

	namespace Transform {

		typedef std::function<void(cv::InputArray src, cv::OutputArray dst, cv::InputArray m)>
			OpenCVTransform;

		extern void apply(EtherdreamPoints & points, OpenCVTransform opencvFunc, cv::InputArray transform);
		extern void applyInPlace(EtherdreamPoints & points, OpenCVTransform opencvFunc, cv::InputArray transform);
	}

}
