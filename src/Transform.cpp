#include "Transform.h"
#include "laser_utilities.h"

#include <opencv2/imgproc/imgproc.hpp>


namespace laser { namespace Transform {

typedef std::vector<cv::Point2f> TransformPoints;

/////////////////////////////////////////////////

DistortionInfo::operator bool() const
{
	return k1 != 0.0 || h1 != 0.0;
}

cv::Mat DistortionInfo::distCoeff() const
{
	cv::Mat m(5, 1, CV_64FC1);
	m = 0.;
	m.at<double>(0, 0) = k1;
	return m;
}

/////////////////////////////////////////////////

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

EtherdreamPoints applyReturning(EtherdreamPoints & points, OpenCVTransform opencvFunc, cv::InputArray transform)
{
	if (points.empty())
		return points;

	EtherdreamPoints returnPoints; //will be all transformed points within the canvas (+margin)
	
	TransformPoints transformPoints;
	transformPoints.reserve(points.size());

	for (auto & p: points)	{
		transformPoints.emplace_back(p.x, p.y);
	}

	opencvFunc(transformPoints, transformPoints, transform);

	int canvasMargin = 10000;

	for (unsigned int i = 0; i < points.size(); i++)
	{
		if(transformPoints[i].x < INT16_MAX + canvasMargin && transformPoints[i].x > INT16_MIN  - canvasMargin 
			&& transformPoints[i].y < INT16_MAX  + canvasMargin && transformPoints[i].y > INT16_MIN - canvasMargin) {
			transformPoints[i].x = clamp(transformPoints[i].x, INT16_MIN, INT16_MAX);
			transformPoints[i].y = clamp(transformPoints[i].y, INT16_MIN, INT16_MAX);
			if(transformPoints[i].x == INT16_MAX || transformPoints[i].x == INT16_MIN || transformPoints[i].y == INT16_MAX || transformPoints[i].y == INT16_MIN) {
				points[i].r = 0;
				points[i].g = 0;
				points[i].b = 0;
			}

			points[i].x = (int16_t) transformPoints[i].x;
			points[i].y = (int16_t) transformPoints[i].y;

			returnPoints.push_back(points[i]);
		}
	}
	return returnPoints;
}

EtherdreamPoints undistort(EtherdreamPoints &points, const DistortionInfo &distInfo)
{
	static const cv::Mat camMat = cv::Mat::eye(3, 3, CV_64FC1);
	if (!distInfo) // Nothing to do
		return points;

	int count = points.size();

	std::vector<cv::Point2d> in, out;
	in.reserve(count);
	out.reserve(count);

	for (const auto & p: points) {
		in.emplace_back(p.x, p.y);
	}

	std::cerr << distInfo.distCoeff() << std::endl;

	cv::undistortPoints(in, out, camMat, distInfo.distCoeff());


	for (int i = 0; i < count; i++) {
		// FIXME: Make hidden stuff black
		points[i].x = (int16_t) clamp(out[i].x, INT16_MIN, INT16_MAX);
	}

	return points;
}

}} // namespace Laser::Transform
