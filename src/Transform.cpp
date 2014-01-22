#include "Transform.h"

namespace laser { namespace Transform {

class TransformPoint: public cv::Point2f
{
public:
	TransformPoint(etherdream_point & point) :
		cv::Point2f(point.x, point.y),
		m_original(point)
	{
		;;
	}

	void updateOriginal() const
	{
		m_original.x = (int16_t) x;
		m_original.y = (int16_t) y;
	}

private:
	etherdream_point & m_original;
};

typedef std::vector<TransformPoint> TransformPoints;

//////////////

static void opaqueApply(std::vector<etherdream_point> & points, TransformPoints & aux_in, TransformPoints & aux_out, OpenCVTransform opencvFunc, cv::InputArray transform)
{
	for (auto & p: points)	{
		aux_in.emplace_back(p);
	}

	opencvFunc(aux_in, aux_out, transform);

	for (const auto & p: aux_out) {
		p.updateOriginal();
	}
}

void apply(std::vector<etherdream_point> & points, OpenCVTransform opencvFunc, cv::InputArray transform)
{
	if (points.empty())
		return;

	std::vector<TransformPoint> aux_in, aux_out;
	aux_in.reserve(points.size());
	aux_out.reserve(points.size());

	opaqueApply(points, aux_in, aux_out, opencvFunc, transform);
}

void applyInPlace(std::vector<etherdream_point> & points, OpenCVTransform opencvFunc, cv::InputArray transform)
{
	if (points.empty())
		return;

	std::vector<TransformPoint> aux_in;
	aux_in.reserve(points.size());

	opaqueApply(points, aux_in, aux_in, opencvFunc, transform);
}

}} // namespace Laser::Transform
