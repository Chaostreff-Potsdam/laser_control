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
    std::shared_ptr<EtherdreamWrapper> m_etherdream;
};


}

#endif
