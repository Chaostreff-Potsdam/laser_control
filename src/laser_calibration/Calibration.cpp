#include "Calibration.h"
#include "../Config.h"

#include <opencv/highgui.h>
#include <opencv2/calib3d/calib3d.hpp>

#include <vector>
#include <cstdint>
#include <iostream>

using namespace laser;

static const double shiftScale = 10000.0 /* Shiftable Distance */ / 1000.0;

Calibration::Calibration(CanvasPtr wrapper)
	: m_scale(100),
	  m_xScale(100),
	  m_yScale(100),
	  m_keystoneFactor(100),
	  m_xFlip(0),
	  m_yFlip(0),
	  m_xShift(500),
	  m_yShift(500),
	  m_rotation(0),
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

	if (!fs["scale"].empty()) { // In case this is still an old calibration without these fields
		fs["scale"] >> m_scale;
		fs["xScale"] >> m_xScale;
		fs["yScale"] >> m_yScale;
		fs["keystoneFactor"] >> m_keystoneFactor;
		fs["m_xFlip"] >> m_xFlip;
		fs["m_yFlip"] >> m_yFlip;
	}

	if (!fs["xShift"].empty()) {
		fs["xShift"] >> m_xShift;
		fs["yShift"] >> m_yShift;
		fs["rotation"] >> m_rotation;

	}

	if (config::forceRecalibration || fs["homography"].empty())
		return false;

	fs["homography"] >> m_homography;
	std::cout << "Found calibration.yml file. Skipping calibration." << std::endl;
	return true;
}

void Calibration::saveCalibration()
{
	cv::FileStorage fs("calibration.yml", cv::FileStorage::WRITE);
	fs << "homography" << m_homography;
	fs << "scale" << m_scale;
	fs << "xScale" << m_xScale;
	fs << "yScale" << m_yScale;
	fs << "keystoneFactor" << m_keystoneFactor;
	fs << "m_xFlip" << m_xFlip;
	fs << "m_yFlip" << m_yFlip;
	fs << "xShift" << m_xShift;
	fs << "yShift" << m_yShift;
	fs << "rotation" << m_rotation;
}

void Calibration::start()
{
	if (alreadyCalibrated())
		return;

	void (*callback)(int, void *) = [](int, void *t){static_cast<Calibration *>(t)->repaint();};
	auto addTrackbar = [&](const char *name, int *target, int maxValue){
		cv::createTrackbar(name, "Calibration", target, maxValue, callback, (void*)this);
	};

	cv::namedWindow("Calibration");
	addTrackbar("Scale", &m_scale, 100);
	addTrackbar("x-Scale", &m_xScale, 100);
	addTrackbar("y-Scale", &m_yScale, 100);
	addTrackbar("Keystone", &m_keystoneFactor, 100);
	addTrackbar("x-Flip", &m_xFlip, 1);
	addTrackbar("y-Flip", &m_yFlip, 1);
	addTrackbar("x-Shift", &m_xShift, 1000);
	addTrackbar("y-Shift", &m_yShift, 1000);
	addTrackbar("rotation", &m_rotation, 3600);

	repaint();
	cv::waitKey();
	cv::destroyWindow("Calibration");

	computeHomography();
	saveCalibration();
}

void Calibration::repaint()
{
	m_rect.resetTransform();
	m_rect.scale(m_scale / 100.0);
	m_rect.scale(m_xScale / 100.0, m_yScale / 100.0);
	m_rect.setKeystoneFactor(m_keystoneFactor / 100.0);
	m_rect.move(shiftScale * (m_xShift - 50), shiftScale * (m_yShift - 50));
	if (m_xFlip)
		m_rect.flipHorizontally();
	if (m_yFlip)
		m_rect.flipVertically();
	m_rect.rotate(radians(m_rotation * 0.1));

	m_etherdream->setPoints(m_rect.pointsToPaint());
	m_etherdream->writePoints();
}

cv::Mat Calibration::homography()
{
	if (m_homography.empty())
		computeHomography();

	return m_homography;
}

cv::Mat Calibration::inverseHomography()
{
	if (m_inverseHomography.empty()) {
		m_inverseHomography = homography().inv();
	}

	return m_inverseHomography;
}

void Calibration::computeHomography()
{
	std::vector<cv::Point2f> dst = m_rect.corners();
	std::vector<cv::Point2f> src;
	for (const auto & p: m_rect.undistoredCorners())
		src.emplace_back(p.x(), p.y());

	m_homography = cv::findHomography(src, dst);
}
