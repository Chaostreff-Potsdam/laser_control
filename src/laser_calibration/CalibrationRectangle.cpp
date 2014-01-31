#include "CalibrationRectangle.h"

#include <iostream>
#include <cstdint>

using namespace laser;

CalibrationRectangle::CalibrationRectangle(cv::Point2f topLeft, cv::Point2f bottomLeft, cv::Point2f bottomRight, cv::Point2f topRight)
    : m_topLeft(topLeft), m_bottomLeft(bottomLeft), m_bottomRight(bottomRight), m_topRight(topRight)
{
}

laser::EtherdreamPoints CalibrationRectangle::points() const
{
    EtherdreamPoints points;

    float pointsPerLine = 50;

    // topLeft - bottomLeft
    cv::Point2f fromTo(m_bottomLeft.x - m_topLeft.x, m_bottomLeft.y - m_topLeft.y);
    for (int i = 0; i < pointsPerLine; ++i)
    {
        cv::Point2f newPoint(m_topLeft.x + (i/pointsPerLine) * fromTo.x, m_topLeft.y + (i/pointsPerLine) * fromTo.y);
		etherdream_point newEtherdreamPoint = { (int16_t) newPoint.x, (int16_t) newPoint.y, 0, UINT16_MAX, 0 }; 
		points.push_back(newEtherdreamPoint);
    }

    // bottomLeft - bottomRight
    fromTo = cv::Point2f(m_bottomRight.x - m_bottomLeft.x, m_bottomRight.y - m_bottomLeft.y);
    for (int i = 0; i < pointsPerLine; ++i)
    {
        cv::Point2f newPoint(m_bottomLeft.x + (i/pointsPerLine) * fromTo.x, m_bottomLeft.y + (i/pointsPerLine) * fromTo.y);
		etherdream_point newEtherdreamPoint = { (int16_t) newPoint.x, (int16_t) newPoint.y, 0, UINT16_MAX, 0 }; 
		points.push_back(newEtherdreamPoint);
    }

    // bottomRight - topRight
    fromTo = cv::Point2f(m_topRight.x - m_bottomRight.x, m_topRight.y - m_bottomRight.y);
    for (int i = 0; i < pointsPerLine; ++i)
    {
        cv::Point2f newPoint(m_bottomRight.x + (i/pointsPerLine) * fromTo.x, m_bottomRight.y + (i/pointsPerLine) * fromTo.y);
		etherdream_point newEtherdreamPoint = { (int16_t) newPoint.x, (int16_t) newPoint.y, 0, UINT16_MAX, 0 }; 
		points.push_back(newEtherdreamPoint);
    }

    // topRight - topLeft
    fromTo = cv::Point2f(m_topLeft.x - m_topRight.x, m_topLeft.y - m_topRight.y);
    for (int i = 0; i < pointsPerLine; ++i)
    {
        cv::Point2f newPoint(m_topRight.x + (i/pointsPerLine) * fromTo.x, m_topRight.y + (i/pointsPerLine) * fromTo.y);
		etherdream_point newEtherdreamPoint = { (int16_t) newPoint.x, (int16_t) newPoint.y, 0, UINT16_MAX, 0 }; 
		points.push_back(newEtherdreamPoint);
    }

    return points;
}

std::vector<cv::Point2f> CalibrationRectangle::corners()
{
    std::vector<cv::Point2f> corners;
    corners.push_back(m_topLeft);
    corners.push_back(m_bottomLeft);
    corners.push_back(m_bottomRight);
    corners.push_back(m_topRight);

    return corners;
}

void CalibrationRectangle::print()
{
    std::cout << "(" << m_topLeft.x << "|" << m_topLeft.y << ") \t" <<
                 "(" << m_topRight.x << "|" << m_topRight.y << ")" << std::endl <<
                 "(" << m_bottomLeft.x << "|" << m_bottomLeft.y << ") \t" <<
                 "(" << m_bottomRight.x << "|" << m_bottomRight.y << ")" << std::endl;
}
