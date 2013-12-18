#include "Calibration.h"

#include <opencv/highgui.h>

#include <vector>
#include <cstdint>
#include <iostream>

using namespace laser;

Calibration::Calibration(std::shared_ptr<EtherdreamWrapper> wrapper)
    : m_scale(100), m_topEdge(100),
      m_rect(CalibrationRectangle(cv::Point2f(INT16_MAX, INT16_MAX),
                           cv::Point2f(INT16_MAX, -INT16_MAX),
                           cv::Point2f(-INT16_MAX, -INT16_MAX),
                           cv::Point2f(-INT16_MAX, INT16_MAX))),
      m_etherdream(wrapper)
{
    std::vector<etherdream_point> points = m_rect.points();
    m_etherdream->setPoints(points);
    m_etherdream->writePoints();

    calibrate();
}

void Calibration::calibrate()
{
    cv::namedWindow("Calibration");

    cv::createTrackbar("Scale", "Calibration", &m_scale, 100,
                       &Calibration::scaleChanged, (void*)this);
    cv::createTrackbar("Keystone", "Calibration", &m_topEdge, 100,
                       &Calibration::topEdgeChanged, (void*)this);

    cv::createButton("Print homography", &Calibration::printHomography, (void*)this);

    cv::waitKey();
}

void Calibration::scaleChanged(int scale, void * inst)
{
    Calibration *instance = (Calibration*)inst;
    instance->m_scale = scale;
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
    float keystoneFactor = m_topEdge / 100.0;

    m_rect = CalibrationRectangle(
                // top edges gets scaled,
                // bottom edges scaled and moved in x direction

                // top left
                cv::Point2f(INT16_MAX * scaleFactor, INT16_MAX * scaleFactor),

                // bottom left
                cv::Point2f(INT16_MAX * scaleFactor * keystoneFactor, -INT16_MAX * scaleFactor),

                // bottom right
                cv::Point2f(-INT16_MAX * scaleFactor * keystoneFactor, -INT16_MAX * scaleFactor),

                // top right
                cv::Point2f(-INT16_MAX * scaleFactor, INT16_MAX * scaleFactor)
                );

    m_etherdream->clear();
    std::vector<etherdream_point> points = m_rect.points();
    m_etherdream->setPoints(points);
    m_etherdream->writePoints();
}

cv::Mat Calibration::homography()
{
    std::vector<cv::Point2f> src;
    src.push_back(cv::Point2f(INT16_MAX, INT16_MAX));
    src.push_back(cv::Point2f(INT16_MAX, -INT16_MAX));
    src.push_back(cv::Point2f(-INT16_MAX, -INT16_MAX));
    src.push_back(cv::Point2f(-INT16_MAX, INT16_MAX));

    return cv::findHomography(src, m_rect.corners());
}


