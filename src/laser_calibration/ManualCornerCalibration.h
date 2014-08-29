#pragma once

#include "AbstractCalibration.h"

#define LASER_MAN_CORNER_CALIB_COUNT 3

namespace laser {

/*!
 * Pick one corner point, shift it around by 3 0-31 pixel positions, since 32**3 = INT16_MAX
 */

class EXPORT_LASER_CONTROL ManualCornerCalibration: public AbstractCalibration
{
public:
	static const int levelDim;   // 32
	static const int levelCount; // LASER_MAN_CORNER_CALIB_COUNT

	struct PointStorage {
		int xVals[LASER_MAN_CORNER_CALIB_COUNT];
		int yVals[LASER_MAN_CORNER_CALIB_COUNT];

		int signX;
		int signY;

		PointStorage():
			signX(1),
			signY(1)
		{
			memset(xVals, levelDim / 2, sizeof(xVals));
			memset(xVals, levelDim / 2, sizeof(xVals));
		}

		operator Point() const;
	};

	ManualCornerCalibration(const CanvasPtr & canvas);

protected:
	virtual const char *configFileName()
	{ return "manualcornercalib.yml"; }

	virtual void showOptions();
	virtual void loadOptions(cv::FileStorage & fs) {}
	virtual void saveOptions(cv::FileStorage & fs) {}
	virtual void repaint();

private:
	int m_currentCorner;
	int m_currentXVals[LASER_MAN_CORNER_CALIB_COUNT];
	int m_currentYVals[LASER_MAN_CORNER_CALIB_COUNT];

	PointStorage points[4];

	void updateCurrentCorner();
};

}
