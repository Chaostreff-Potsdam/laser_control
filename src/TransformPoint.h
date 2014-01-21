#pragma once

#include "etherdream.h"
#include <opencv2/core/core.hpp>

#include <vector>
#include <functional>

namespace laser {

	class TransformPoint: public cv::Point2f
	{
	public:
		TransformPoint(etherdream_point & point);
		void updateOriginal() const;

	private:
		etherdream_point & m_original;
	};

	typedef std::vector<TransformPoint> TransformPoints;
	typedef std::function<void(const TransformPoints & in, const TransformPoints & out)>
		TransformFunction;

	extern void applyTransformToEtherPoints(std::vector<etherdream_point> & points, TransformFunction func);
}
