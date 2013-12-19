#ifndef CALIBRATION_H
#define CALIBRATION_H

#include <memory>
#include <opencv/cv.h>

#include "CalibrationRectangle.h"
#include "EtherdreamWrapper.h"

namespace laser {

class Calibration
{
public:
    Calibration(std::shared_ptr<EtherdreamWrapper> wrapper);

    void start();
    cv::Mat homography();

private:

    void updateRectangle();

    void computeHomography();

    static void scaleChanged(int scale, void* instance);
    static void topEdgeChanged(int length, void* instance);
    static void printHomography(int, void* instance);

    static CalibrationRectangle maxRect();

    int m_scale;
    int m_topEdge;

    CalibrationRectangle m_rect;
    std::shared_ptr<EtherdreamWrapper> m_etherdream;
    cv::Mat m_homography;
};


}

#endif
