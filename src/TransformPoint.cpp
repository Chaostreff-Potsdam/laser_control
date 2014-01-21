#include "TransformPoint.h"

laser::TransformPoint::TransformPoint(etherdream_point &point) :
	cv::Point2f(point.x, point.y),
	m_original(point)
{
	;;
}

void laser::TransformPoint::updateOriginal() const
{
	m_original.x = (int16_t) x;
	m_original.y = (int16_t) y;
}

void laser::applyTransformToEtherPoints(std::vector<etherdream_point> &points, laser::TransformFunction func)
{
	if (points.empty())
		return;

	TransformPoints aux_in, aux_out;
	aux_in.reserve(points.size());
	aux_out.reserve(points.size());

	for (auto & p: points)
	{
		aux_in.emplace_back(p);
	}

	func(aux_in, aux_out);

	for (const auto & p: aux_out)
	{
		p.updateOriginal();
	}
}
