#pragma once

#include <memory>

#include "CalibrationRectangle.h"

namespace laser {

class EXPORT_LASER_CONTROL Calibration
{
public:
    Calibration(std::shared_ptr<EtherdreamWrapper> wrapper);

    void start();
    cv::Mat homography();

private:
	bool alreadyCalibrated();
    void repaint();

    void computeHomography();

    static CalibrationRectangle maxRect();

    int m_scale;
    int m_yScale;
    int m_keystoneFactor;

    CalibrationRectangle m_rect;
    std::shared_ptr<EtherdreamWrapper> m_etherdream;
    cv::Mat m_homography;
};


}
