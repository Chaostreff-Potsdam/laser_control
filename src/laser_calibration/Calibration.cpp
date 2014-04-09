#include "Calibration.h"

#include <opencv/highgui.h>
#include <opencv2/calib3d/calib3d.hpp>

#include <vector>
#include <cstdint>
#include <iostream>

using namespace laser;

Calibration::Calibration(std::shared_ptr<EtherdreamWrapper> wrapper)
	: m_scale(100),
	  m_yScale(100),
	  m_keystoneFactor(100),
      m_rect(maxRect()),
	  m_etherdream(wrapper),
	  m_homography(0, 0, CV_8UC1, nullptr)
{
	;;
}

bool Calibration::alreadyCalibrated()
{
	cv::FileStorage fs("calibration.yml", cv::FileStorage::READ);

	if (!fs.isOpened())
		return false;

	fs["homography"] >> m_homography;
	std::cout << "Found calibration.yml file. Skipping calibration." << std::endl;
	return true;
}

void Calibration::start()
{
	if (alreadyCalibrated())
		return;

	void (*callback)(int, void *) = [](int, void *t){static_cast<Calibration *>(t)->repaint();};

	cv::namedWindow("Calibration");
	cv::createTrackbar("Scale", "Calibration", &m_scale, 100, callback, (void*)this);
	cv::createTrackbar("y-Scale", "Calibration", &m_yScale, 100, callback, (void*)this);
	cv::createTrackbar("Keystone", "Calibration", &m_keystoneFactor, 100, callback, (void*)this);
	//cv::createButton("Print homography", &Calibration::printHomography, (void*)this);

	repaint();

	cv::waitKey();

	cv::destroyWindow("Calibration");

	computeHomography();
	cv::FileStorage fs1("calibration.yml", cv::FileStorage::WRITE);
	fs1 << "homography" << m_homography;
}

void Calibration::repaint()
{
	float scaleFactor = m_scale / 100.0;
	float yScaleFactor = m_yScale / 100.0;
	float keystoneFactor = m_keystoneFactor / 100.0;

	m_rect = CalibrationRectangle(
			// top edges gets scaled,
			// bottom edges scaled and moved in x direction

			// top left
			Point(INT16_MAX * scaleFactor, INT16_MAX * scaleFactor * yScaleFactor),

			// bottom left
			Point(INT16_MAX * scaleFactor * keystoneFactor, -INT16_MAX * scaleFactor * yScaleFactor),

			// bottom right
			Point(-INT16_MAX * scaleFactor * keystoneFactor, -INT16_MAX * scaleFactor * yScaleFactor),

			// top right
			Point(-INT16_MAX * scaleFactor, INT16_MAX * scaleFactor * yScaleFactor)
			);

	m_etherdream->setPoints(m_rect.points());
	m_etherdream->writePoints();
}

cv::Mat Calibration::homography()
{
	if (m_homography.empty())
		computeHomography();

	return m_homography;
}

void Calibration::computeHomography()
{
	std::vector<cv::Point2f> src = maxRect().corners();
	std::vector<cv::Point2f> dst = m_rect.corners();
	m_homography = cv::findHomography(src, dst);
}

CalibrationRectangle Calibration::maxRect()
{
	return CalibrationRectangle(Point(INT16_MAX, INT16_MAX),
								Point(INT16_MAX, -INT16_MAX),
								Point(-INT16_MAX, -INT16_MAX),
								Point(-INT16_MAX, INT16_MAX));
}
