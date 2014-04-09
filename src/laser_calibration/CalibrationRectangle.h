#pragma once

#include "../EtherdreamWrapper.h"
#include "../Object.h"

namespace laser {

class EXPORT_LASER_CONTROL CalibrationRectangle: public Object
{
public:
	CalibrationRectangle(Point topLeft, Point bottomLeft, Point bottomRight, Point topRight);

	EtherdreamPoints points() const;
	std::vector<cv::Point2f> corners();

	EtherdreamPoints startPoints() const
	{ return EtherdreamPoints(); }

	EtherdreamPoints endPoints() const
	{ return EtherdreamPoints(); }

private:
	Point m_topLeft;
	Point m_bottomLeft;
	Point m_bottomRight;
	Point m_topRight;
};

}
