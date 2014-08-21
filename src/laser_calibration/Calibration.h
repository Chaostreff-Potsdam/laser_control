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
	cv::Mat inverseHomography();

	const CalibrationRectangle & rect() const
	{ return m_rect; }

private:
	bool alreadyCalibrated();
	void saveCalibration();
	void repaint();
	void computeHomography();

	int m_scale;
	int m_xScale;
	int m_yScale;
	int m_keystoneFactor;
	int m_xFlip;
	int m_yFlip;
	int m_xShift;
	int m_yShift;

	CalibrationRectangle m_rect;
	CanvasPtr m_etherdream;
	cv::Mat m_homography;
	cv::Mat m_inverseHomography;
};

}
