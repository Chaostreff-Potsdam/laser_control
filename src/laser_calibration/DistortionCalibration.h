#pragma once

#include "AbstractCalibration.h"
#include "Transform.h"

namespace laser {

//!	Removing lense-like distortions
class EXPORT_LASER_CONTROL DistorionCalibration: public AbstractCalibration
{
	enum { VERTICAL = 0, HORIZONTAL = 1 };
	enum { LINE_COUNT = 6 };

public:
	DistorionCalibration(const CanvasPtr & canvas);

	Transform::DistortionInfo distortion();

protected:
	CompositeObjectPtr m_vlines;
	CompositeObjectPtr m_hlines;

	virtual const char *configFileName()
	{ return "distortion.yml"; }

	virtual EtherdreamPoints pointsToPaint();
	virtual void readCalibFrom(cv::FileStorage &fs);
	virtual void writeCalibTo(cv::FileStorage &fs);

	virtual void showOptions();
	virtual void loadOptions(cv::FileStorage & fs);
	virtual void saveOptions(cv::FileStorage & fs);

	virtual const char *configKeyName()
	{ return "distortion"; }

	CompositeObjectPtr currentLines() const;

private:
	Transform::DistortionInfo m_distortion;

	int m_currentDirection;
	int m_k1;
	int m_h1;

	virtual void compute();
};

}
