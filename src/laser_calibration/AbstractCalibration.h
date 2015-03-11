#pragma once

#include <memory>
#include "CalibrationRectangle.h"

namespace laser {

//! Baseclass for all kinds of image correction (planar, lens distortion, etc)
class EXPORT_LASER_CONTROL AbstractCalibration
{
public:
	typedef void (*UpdateCallback)(int, void *);

	AbstractCalibration(const CanvasPtr & canvas);

	void start();

protected:
	virtual void repaint();

	virtual const char *configFileName() = 0;
	virtual const char *configKeyName() = 0;
	virtual const char *windowName()
	{ return "Calibration"; }

	// fs << configKeyName() << my_storage;
	virtual void writeCalibTo(cv::FileStorage & fs) = 0;
	// fs[configKeyName()] >> my_storage;
	virtual void readCalibFrom(cv::FileStorage & fs) = 0;

	virtual void showOptions() = 0;
	virtual void loadOptions(cv::FileStorage & fs) = 0;
	virtual void saveOptions(cv::FileStorage & fs) = 0;

	void addTrackbar(const char *name, int *target, int maxValue, UpdateCallback callback = repaintCallback);
	void setTrackbarPos(const char *name, int pos);

	//!< compute homography or distortion coefficients
	virtual void compute() = 0;

	virtual EtherdreamPoints pointsToPaint() = 0;

private:
	static const UpdateCallback repaintCallback;

	bool alreadyCalibrated();
	void saveCalibration();

	CanvasPtr m_canvas;

};

}
