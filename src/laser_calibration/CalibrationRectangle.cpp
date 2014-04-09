#include "CalibrationRectangle.h"

#include <iostream>
#include <cstdint>

using namespace laser;

static cv::Point2f toCV(const Point & p)
{ return cv::Point2f(p.x(), p.y()); }

CalibrationRectangle::CalibrationRectangle(Point topLeft, Point bottomLeft, Point bottomRight, Point topRight)
	: Object(),
	  m_topLeft(topLeft), m_bottomLeft(bottomLeft), m_bottomRight(bottomRight), m_topRight(topRight)
{
	;;
}

laser::EtherdreamPoints CalibrationRectangle::points() const
{
	const float pointsPerLine = 50;
	EtherdreamPoints points;

	auto drawLineFromTo = [&](const Point & from, const Point & to) {
		Point fromTo = from - to;
		for (int i = 0; i < pointsPerLine; ++i)
		{
			Point newPoint = to + fromTo * (i/pointsPerLine);
			points.push_back(etherdreamPoint(newPoint));
		}
	};

	drawLineFromTo(m_topLeft, m_topRight);
	drawLineFromTo(m_bottomLeft, m_topLeft);
	drawLineFromTo(m_bottomRight, m_bottomLeft);
	drawLineFromTo(m_topRight, m_bottomRight);

	return points;
}

std::vector<cv::Point2f> CalibrationRectangle::corners()
{
	std::vector<cv::Point2f> corners;
	corners.push_back(toCV(m_topLeft));
	corners.push_back(toCV(m_bottomLeft));
	corners.push_back(toCV(m_bottomRight));
	corners.push_back(toCV(m_topRight));

	return corners;
}
