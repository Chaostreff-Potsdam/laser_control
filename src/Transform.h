#pragma once

#ifdef _WIN32
	#include "dac.h"
#else // _WIN32
	#include "etherdream.h"
#endif // _WIN32

#include <opencv2/core/core.hpp>

#include <vector>
#include <functional>

namespace laser {

	namespace Transform {

		typedef std::function<void(cv::InputArray src, cv::OutputArray dst, cv::InputArray m)>
			OpenCVTransform;

		extern void apply(std::vector<etherdream_point> & points, OpenCVTransform opencvFunc, cv::InputArray transform);
		extern void applyInPlace(std::vector<etherdream_point> & points, OpenCVTransform opencvFunc, cv::InputArray transform);
	}

}
