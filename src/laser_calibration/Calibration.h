#ifndef CALIBRATION_H
#define CALIBRATION_H

#include <opencv/cv.h>

#include "EtherdreamWrapper.h"
#include "CalibrationRectangle.h"

class Calibration
{
public:
    Calibration();

    cv::Mat homography();

private:
    void calibrate();

    void updateRectangle();

    static void scaleChanged(int scale, void* instance);
    static void topEdgeChanged(int length, void* instance);
    static void printHomography(int, void* instance);

    int m_scale;
    int m_topEdge;

    CalibrationRectangle m_rect;
    laser::EtherdreamWrapper m_etherdream;
};

#endif
