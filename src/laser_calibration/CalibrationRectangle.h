#ifndef CALIBRATIONRECTANGLE_H
#define CALIBRATIONRECTANGLE_H

#include <vector>
#include <opencv/cv.h>

#include "etherdream.h"

namespace laser {

class CalibrationRectangle
{
public:

    CalibrationRectangle(cv::Point2f topLeft, cv::Point2f bottomLeft, cv::Point2f bottomRight, cv::Point2f topRight);

    std::vector<etherdream_point> points() const;
    std::vector<cv::Point2f> corners();

    void print();


private:
    cv::Point2f m_topLeft;
    cv::Point2f m_bottomLeft;
    cv::Point2f m_bottomRight;
    cv::Point2f m_topRight;
};

}

#endif // CALIBRATIONRECTANGLE_H
