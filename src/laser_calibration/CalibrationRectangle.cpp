#include "CalibrationRectangle.h"
#include "Calibration.h"
#include "../Transform.h"

#include <cstdint>

using namespace laser;

CalibrationRectangle::CalibrationRectangle() :
	Polygon(undistoredCorners())

{
	;;
}

void CalibrationRectangle::setKeystoneFactor(float keystoneFactor)
{
	int topHalfLength = keystoneFactor * (int)INT16_MAX;
	m_corners[0].setX(-topHalfLength);
	m_corners[3].setX(+topHalfLength);
	nowDirty();
}

std::vector<cv::Point2f> CalibrationRectangle::corners() const
{
	EtherdreamPoints corners;
	for (const auto & p: m_corners)
		corners.push_back(etherdreamPoint(p));

	Transform::applyInPlace(corners, cv::transform, transform()(cv::Rect(0, 0, 3, 2)));

	std::vector<cv::Point2f> result;
	for (const auto & p: corners)
		result.emplace_back(p.x, p.y);
	return result;
}

void CalibrationRectangle::setCorner(int index, const Point &p)
{
	m_corners[index] = p;
}
