#include "PlanarCalibration.h"

#include <vector>
#include <opencv2/calib3d/calib3d.hpp>

namespace laser {

PlanarCalibration::PlanarCalibration(const CanvasPtr &canvas):
	AbstractCalibration(canvas),
	m_homography(0, 0, CV_8UC1, nullptr)
{
   ;;
}

EtherdreamPoints PlanarCalibration::pointsToPaint()
{
	return m_rect.pointsToPaint();
}

cv::Mat PlanarCalibration::homography()
{
	if (m_homography.empty())
		compute();

	return m_homography;
}

cv::Mat PlanarCalibration::inverseHomography()
{
	if (m_inverseHomography.empty()) {
		m_inverseHomography = homography().inv();
	}

	return m_inverseHomography;
}

void PlanarCalibration::compute()
{
	std::vector<cv::Point2f> dst = m_rect.corners();
	std::vector<cv::Point2f> src;
	for (const auto & p: m_rect.undistoredCorners())
		src.emplace_back(p.x(), p.y());

	m_homography = cv::findHomography(src, dst);
}

void PlanarCalibration::readCalibFrom(cv::FileStorage &fs)
{
	fs[configKeyName()] >> m_homography;
}

void PlanarCalibration::writeCalibTo(cv::FileStorage &fs)
{
	fs << configKeyName() << m_homography;
}

}
