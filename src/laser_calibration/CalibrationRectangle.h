#ifndef LASERRECTANGLE_H
#define LASERRECTANGLE_H

#include <vector>
#include <opencv/cv.h>

#include "etherdream.h"

class CalibrationRectangle
{
public:

    CalibrationRectangle(cv::Point2f topLeft, cv::Point2f bottomLeft, cv::Point2f bottomRight, cv::Point2f topRight);

    std::vector<etherdream_point> points();
    std::vector<cv::Point2f> corners();

    void print();


private:
    cv::Point2f m_topLeft;
    cv::Point2f m_bottomLeft;
    cv::Point2f m_bottomRight;
    cv::Point2f m_topRight;
};

#endif // LASERRECTANGLE_H
