#include "Transform.h"
#include "laser_utilities.h"

#include <iostream>

namespace laser { namespace Transform {

typedef std::vector<cv::Point2f> TransformPoints;

static void opaqueApply(EtherdreamPoints & points, TransformPoints & aux_in, TransformPoints & aux_out, OpenCVTransform opencvFunc, cv::InputArray transform)
{
	for (auto & p: points)	{
		aux_in.emplace_back(p.x, p.y);
	}

	opencvFunc(aux_in, aux_out, transform);

	for (unsigned int i = 0; i < points.size(); i++)
	{
		aux_out[i].x = clamp(aux_out[i].x, INT16_MIN, INT16_MAX);
		aux_out[i].y = clamp(aux_out[i].y, INT16_MIN, INT16_MAX);
		if(aux_out[i].x == INT16_MAX || aux_out[i].x == INT16_MIN || aux_out[i].y == INT16_MAX || aux_out[i].y == INT16_MIN) {
			points[i].r = 0;
			points[i].g = 0;
			points[i].b = 0;
		}

		points[i].x = (int16_t) aux_out[i].x;
		points[i].y = (int16_t) aux_out[i].y;
	}
}

void apply(EtherdreamPoints & points, OpenCVTransform opencvFunc, cv::InputArray transform)
{
	if (points.empty())
		return;

	TransformPoints aux_in, aux_out;
	aux_in.reserve(points.size());
	aux_out.reserve(points.size());

	opaqueApply(points, aux_in, aux_out, opencvFunc, transform);
}

void applyInPlace(EtherdreamPoints & points, OpenCVTransform opencvFunc, cv::InputArray transform)
{
	if (points.empty())
		return;

	TransformPoints aux_in;
	aux_in.reserve(points.size());

	opaqueApply(points, aux_in, aux_in, opencvFunc, transform);
}

}} // namespace Laser::Transform
