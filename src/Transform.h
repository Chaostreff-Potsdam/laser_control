#pragma once

#include "etherdream.h"
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
