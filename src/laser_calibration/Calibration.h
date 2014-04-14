#pragma once

#include <memory>

#include "CalibrationRectangle.h"

namespace laser {

class EXPORT_LASER_CONTROL Calibration
{
public:
	Calibration(CanvasPtr wrapper);

    void start();
    cv::Mat homography();

private:
	bool alreadyCalibrated();
	void repaint();
	void computeHomography();

	int m_scale;
	int m_yScale;
	int m_keystoneFactor;

	CalibrationRectangle m_rect;
	CanvasPtr m_etherdream;
	cv::Mat m_homography;
};

}
