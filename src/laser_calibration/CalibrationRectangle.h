#ifndef CALIBRATIONRECTANGLE_H
#define CALIBRATIONRECTANGLE_H

#include "../EtherdreamWrapper.h"
#include "DllExport.h"

namespace laser {

class EXPORT_LASER_CONTROL CalibrationRectangle
{
public:

    CalibrationRectangle(cv::Point2f topLeft, cv::Point2f bottomLeft, cv::Point2f bottomRight, cv::Point2f topRight);

    EtherdreamPoints points() const;
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
