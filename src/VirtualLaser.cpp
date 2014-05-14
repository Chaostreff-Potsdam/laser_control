#include "VirtualLaser.h"
#include "laser_utilities.h"

#include <opencv2/highgui/highgui.hpp>

namespace laser {

static const char *windowName = "Virtual Laser Canvas";
static const int windowSize = 720;
static const float windowScale = windowSize / ((float)INT16_MAX - (float)INT16_MIN);
static const float windowCenter = windowSize * 0.5;

static std::pair<cv::Point, cv::Scalar> parsePoint(const etherdream_point & p)
{
	return std::make_pair(
					cv::Point(p.x * windowScale + windowCenter, p.y * windowScale + windowCenter),
					cv::Scalar(p.b, p.g, p.r)
				);
}

VirtualLaser::VirtualLaser() :
	Canvas()
{
	cv::namedWindow(windowName);
}

VirtualLaser::~VirtualLaser()
{
	cv::destroyWindow(windowName);
}

void VirtualLaser::writePoints()
{
	std::lock_guard<std::mutex> guard(m_pointsMutex);

	cv::Mat canvas = cv::Mat::zeros(windowSize, windowSize, CV_16UC3);

	for (size_t currentIndex = 0; currentIndex < m_points.size(); )
		currentIndex = drawNextPolyline(currentIndex, canvas);

	cv::imshow(windowName, canvas);
	if (cv::waitKey(1) == 'q') exit(0);
}

size_t VirtualLaser::drawNextPolyline(size_t currentIndex, cv::Mat &canvas)
{
	std::vector<cv::Point> currentPoints;

	auto firstPoint = parsePoint(m_points[currentIndex]);
	currentPoints.push_back(firstPoint.first);
	const cv::Scalar currentColor = firstPoint.second;

	for (currentIndex++; currentIndex < m_points.size(); currentIndex++) {
		auto nextPoint = parsePoint(m_points[currentIndex]);
		if (nextPoint.second != currentColor)
			break;
		currentPoints.push_back(nextPoint.first);
	}

	const cv::Point *pts = currentPoints.data();
	const int nums = (int) currentPoints.size();
	if (currentColor != cv::Scalar(0,0,0))
		cv::polylines(canvas, &pts, &nums, 1, false, currentColor);

	return currentIndex;
}

}
