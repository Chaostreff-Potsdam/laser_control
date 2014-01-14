#include "Calibration.h"

#include <opencv/highgui.h>
#include <opencv2/core/core.hpp>

#include <vector>
#include <cstdint>
#include <iostream>

using namespace laser;

Calibration::Calibration(std::shared_ptr<EtherdreamWrapper> wrapper)
    : m_scale(100), m_topEdge(100),
      m_rect(maxRect()),
      m_etherdream(wrapper)
{
}

void Calibration::start()
{
	cv::FileStorage fs("calibration.yml", cv::FileStorage::READ);

	if (fs.isOpened())
	{
		fs["homography"] >> m_homography;

		fs.release();
		return;
	}

	fs.release();

    std::vector<etherdream_point> points = m_rect.points();
    m_etherdream->setPoints(points);
    m_etherdream->writePoints();

    cv::namedWindow("Calibration");

    cv::createTrackbar("Scale", "Calibration", &m_scale, 100,
                       &Calibration::scaleChanged, (void*)this);
    cv::createTrackbar("y-Scale", "Calibration", &m_yScale, 100,
                       &Calibration::yScaleChanged, (void*)this);
    cv::createTrackbar("Keystone", "Calibration", &m_topEdge, 100,
                       &Calibration::topEdgeChanged, (void*)this);
    cv::createButton("Print homography", &Calibration::printHomography, (void*)this);

    cv::waitKey();

    computeHomography();

	cv::FileStorage fs1("calibration.yml", cv::FileStorage::WRITE);

	fs1 << "homography" << m_homography;

	fs1.release();

    cv::destroyWindow("Calibration");
}

void Calibration::scaleChanged(int scale, void * inst)
{
    Calibration *instance = (Calibration*)inst;
    instance->m_scale = scale;
    instance->updateRectangle();
}

void Calibration::yScaleChanged(int scale, void * inst)
{
    Calibration *instance = (Calibration*)inst;
    instance->m_yScale = scale;
    instance->updateRectangle();
}

void Calibration::topEdgeChanged(int length, void * inst)
{
    Calibration *instance = (Calibration*)inst;
    instance->m_topEdge = length;
    instance->updateRectangle();
}

void Calibration::printHomography(int /*unused*/, void *inst)
{
    Calibration *instance = (Calibration*)inst;
    cv::Mat h = instance->homography();
    std::cout << h << std::endl;
}

void Calibration::updateRectangle()
{
    float scaleFactor = m_scale / 100.0;
    float yScaleFactor = m_yScale / 100.0;
    float keystoneFactor = m_topEdge / 100.0;

    m_rect = CalibrationRectangle(
                // top edges gets scaled,
                // bottom edges scaled and moved in x direction

                // top left
                cv::Point2f(INT16_MAX * scaleFactor, INT16_MAX * scaleFactor * yScaleFactor),

                // bottom left
                cv::Point2f(INT16_MAX * scaleFactor * keystoneFactor, -INT16_MAX * scaleFactor * yScaleFactor),

                // bottom right
                cv::Point2f(-INT16_MAX * scaleFactor * keystoneFactor, -INT16_MAX * scaleFactor * yScaleFactor),

                // top right
                cv::Point2f(-INT16_MAX * scaleFactor, INT16_MAX * scaleFactor * yScaleFactor)
                );

    m_etherdream->clear();
    std::vector<etherdream_point> points = m_rect.points();
    m_etherdream->setPoints(points);
    m_etherdream->writePoints();
}

cv::Mat Calibration::homography()
{
    // TODO check if m_homography is valid!
    // computeHomography() otherwise
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
    return CalibrationRectangle(cv::Point2f(INT16_MAX, INT16_MAX),
                                cv::Point2f(INT16_MAX, -INT16_MAX),
                                cv::Point2f(-INT16_MAX, -INT16_MAX),
                                cv::Point2f(-INT16_MAX, INT16_MAX));
}
