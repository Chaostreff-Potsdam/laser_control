#pragma once

#include "../EtherdreamWrapper.h"
#include "../objects/Polygon.h"

namespace laser {

class EXPORT_LASER_CONTROL CalibrationRectangle: public Polygon
{
public:
	static std::vector<Point> undistoredCorners();

	CalibrationRectangle();
	void setKeystoneFactor(float keystoneFactor);

	std::vector<cv::Point2f> corners();
};

}
