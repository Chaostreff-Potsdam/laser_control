#pragma once

#include "AbstractCalibration.h"

namespace laser {

class EXPORT_LASER_CONTROL ManualCornerCalibration: public AbstractCalibration
{
public:
	ManualCornerCalibration(const CanvasPtr & canvas);

protected:
	virtual const char *configFileName()
	{ return "manualcornercalib.yml"; }

	virtual void showOptions() {}
	virtual void loadOptions(cv::FileStorage & fs) {}
	virtual void saveOptions(cv::FileStorage & fs) {}
	virtual void repaint() {}


};

}
