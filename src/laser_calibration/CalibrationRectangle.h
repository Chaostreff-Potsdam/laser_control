#pragma once

#include "../EtherdreamWrapper.h"
#include "../objects/Polygon.h"

namespace laser {

class EXPORT_LASER_CONTROL CalibrationRectangle: public Polygon
{
public:
	template<typename PointT>
	static std::vector<PointT> undistoredCorners_()
	{
		std::vector<PointT> points;

		points.emplace_back(INT16_MIN, INT16_MIN);
		points.emplace_back(INT16_MIN, INT16_MAX);
		points.emplace_back(INT16_MAX, INT16_MAX);
		points.emplace_back(INT16_MAX, INT16_MIN);
		return points;
	}
	static std::vector<Point> undistoredCorners()
	{ return undistoredCorners_<Point>(); }


	CalibrationRectangle();
	void setKeystoneFactor(float keystoneFactor);

	std::vector<cv::Point2f> corners() const;
};

}
