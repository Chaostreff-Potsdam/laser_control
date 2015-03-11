#include "AbstractCalibration.h"
#include "../Config.h"

#include <opencv/highgui.h>

namespace laser {

const AbstractCalibration::UpdateCallback AbstractCalibration::repaintCallback
	= [](int, void *t){static_cast<AbstractCalibration *>(t)->repaint();};

AbstractCalibration::AbstractCalibration(const CanvasPtr &canvas) :
	m_canvas(canvas)
{
	;;
}

void AbstractCalibration::repaint()
{
	m_canvas->setPoints(pointsToPaint());
	m_canvas->writePoints();
}

bool AbstractCalibration::alreadyCalibrated()
{
	cv::FileStorage fs(configFileName(), cv::FileStorage::READ);

	if (!fs.isOpened())
		return false;

	loadOptions(fs);

	if (config::forceRecalibration || fs[configKeyName()].empty())
		return false;

	readCalibFrom(fs);
	std::cout << "Found " << configFileName() << " file. Skipping calibration." << std::endl;
	return true;
}

void AbstractCalibration::saveCalibration()
{
	cv::FileStorage fs(configFileName(), cv::FileStorage::WRITE);
	writeCalibTo(fs);
	saveOptions(fs);
}

void AbstractCalibration::start()
{
	if (alreadyCalibrated())
		return;

	cv::namedWindow(windowName());
	showOptions();
	repaint();
	cv::waitKey();
	cv::destroyWindow(windowName());

	compute();
	saveCalibration();
}

void AbstractCalibration::addTrackbar(const char *name, int *target, int maxValue, UpdateCallback callback)
{
	cv::createTrackbar(name, windowName(), target, maxValue, callback, (void*)this);
}

void AbstractCalibration::setTrackbarPos(const char *name, int pos)
{
	cv::setTrackbarPos(name, windowName(), pos);
}

}
