#pragma once

#include <memory>
#include "CalibrationRectangle.h"

namespace laser {

class EXPORT_LASER_CONTROL AbstractCalibration
{
public:
	typedef void (*RepaintCallback)(int, void *);

	AbstractCalibration(const CanvasPtr & canvas);

	void start();
	cv::Mat homography();
	cv::Mat inverseHomography();

	const CalibrationRectangle & rect() const
	{ return m_rect; }

protected:
	virtual const char *configFileName() = 0;
	virtual void showOptions() = 0;
	virtual void loadOptions(cv::FileStorage & fs) = 0;
	virtual void saveOptions(cv::FileStorage & fs) = 0;
	virtual void repaint();


	void addTrackbar(const char *name, int *target, int maxValue);

	CalibrationRectangle m_rect;

private:
	static const RepaintCallback repaintCallback;

	bool alreadyCalibrated();
	void saveCalibration();
	void computeHomography();

	CanvasPtr m_canvas;
	cv::Mat m_homography;
	cv::Mat m_inverseHomography;
};

}
