#include "VirtualLaser.h"
#include "laser_utilities.h"

#include <opencv2/highgui/highgui.hpp>

namespace laser {

const char *VirtualLaser::windowName = "Virtual Laser Canvas";
const int VirtualLaser::windowSize = 720;
const float VirtualLaser::scale = VirtualLaser::windowSize / ((float)INT16_MAX - (float)INT16_MIN);


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
	static const float center = windowSize * 0.5;
	std::lock_guard<std::mutex> guard(m_pointsMutex);

	cv::Mat canvas = cv::Mat::zeros(windowSize, windowSize, CV_8UC3);

	std::vector<cv::Point> currentPoints;
	currentPoints.reserve(m_points.size());
	for (const auto & p: m_points) {
		currentPoints.emplace_back(p.x * scale + center, p.y * scale + center);
	}

	const cv::Point *pts = currentPoints.data();
	const int nums = (int) m_points.size();

	cv::polylines(canvas, &pts, &nums, 1, false, cv::Scalar(0, 255, 0));

	cv::imshow(windowName, canvas);
	if (cv::waitKey(1) == 'q') exit(0);
}

}
