#pragma once

#include "PlanarCalibration.h"

namespace laser {

class EXPORT_LASER_CONTROL Calibration: public PlanarCalibration
{
public:
	Calibration(const CanvasPtr & canvas);

protected:
	virtual const char *configFileName()
	{ return "calibration.yml"; }

	virtual void repaint();

	void showOptions();
	void loadOptions(cv::FileStorage &fs);
	void saveOptions(cv::FileStorage &fs);

private:
	int m_scale;
	int m_xScale;
	int m_yScale;
	int m_keystoneFactor;
	int m_xFlip;
	int m_yFlip;
	int m_xShift;
	int m_yShift;
	int m_rotation;
};

}
