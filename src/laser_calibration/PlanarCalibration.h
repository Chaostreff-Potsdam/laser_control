#pragma once

#include "AbstractCalibration.h"

namespace laser {

//! Calibration that produces a homography (removing planar distortion)
class EXPORT_LASER_CONTROL PlanarCalibration: public AbstractCalibration
{
public:
	PlanarCalibration(const CanvasPtr & canvas);

	cv::Mat homography();
	cv::Mat inverseHomography();
	const CalibrationRectangle & rect() const
	{ return m_rect; }

protected:
	CalibrationRectangle m_rect;

	virtual EtherdreamPoints pointsToPaint();
	virtual void readCalibFrom(cv::FileStorage &fs);
	virtual void writeCalibTo(cv::FileStorage &fs);

	virtual const char *configKeyName()
	{ return "homography"; }

private:
	cv::Mat m_homography;
	cv::Mat m_inverseHomography;

	virtual void compute();
};

}
