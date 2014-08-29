#include "AbstractCalibration.h"
#include "../Config.h"

#include <vector>
#include <opencv2/calib3d/calib3d.hpp>
#include <opencv/highgui.h>

namespace laser {

const AbstractCalibration::RepaintCallback AbstractCalibration::repaintCallback
	= [](int, void *t){static_cast<AbstractCalibration *>(t)->repaint();};

AbstractCalibration::AbstractCalibration(const CanvasPtr &canvas) :
	m_canvas(canvas),
	m_homography(0, 0, CV_8UC1, nullptr)
{
	;;
}

cv::Mat AbstractCalibration::homography()
{
	if (m_homography.empty())
		computeHomography();

	return m_homography;
}

cv::Mat AbstractCalibration::inverseHomography()
{
	if (m_inverseHomography.empty()) {
		m_inverseHomography = homography().inv();
	}

	return m_inverseHomography;
}

void AbstractCalibration::computeHomography()
{
	std::vector<cv::Point2f> dst = m_rect.corners();
	std::vector<cv::Point2f> src;
	for (const auto & p: m_rect.undistoredCorners())
		src.emplace_back(p.x(), p.y());

	m_homography = cv::findHomography(src, dst);
}

void AbstractCalibration::repaint()
{
	m_canvas->setPoints(m_rect.pointsToPaint());
	m_canvas->writePoints();
}

bool AbstractCalibration::alreadyCalibrated()
{
	cv::FileStorage fs(configFileName(), cv::FileStorage::READ);

	if (!fs.isOpened())
		return false;

	loadOptions(fs);

	if (config::forceRecalibration || fs["homography"].empty())
		return false;

	fs["homography"] >> m_homography;
	std::cout << "Found " << configFileName() << " file. Skipping calibration." << std::endl;
	return true;
}

void AbstractCalibration::saveCalibration()
{
	cv::FileStorage fs(configFileName(), cv::FileStorage::WRITE);
	fs << "homography" << m_homography;
	saveOptions(fs);
}

void AbstractCalibration::start()
{
	if (alreadyCalibrated())
		return;

	cv::namedWindow("Calibration");
	showOptions();
	repaint();
	cv::waitKey();
	cv::destroyWindow("Calibration");

	computeHomography();
	saveCalibration();
}

void AbstractCalibration::addTrackbar(const char *name, int *target, int maxValue)
{
	cv::createTrackbar(name, "Calibration", target, maxValue, repaintCallback, (void*)this);
}

}
