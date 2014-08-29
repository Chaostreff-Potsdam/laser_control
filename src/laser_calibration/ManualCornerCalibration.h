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
	typedef int Levels[LASER_MAN_CORNER_CALIB_COUNT];
	static const int levelDim;   // 32
	static const int levelMax;   // levelDim - 1
	static const int levelCount; // LASER_MAN_CORNER_CALIB_COUNT

	static void levelsSet(Levels levels, int value = levelMax);

	struct PointStorage {
		Levels xVals;
		Levels yVals;

		int signX;
		int signY;

		PointStorage() :
			signX(1),
			signY(1)
		{
			levelsSet(xVals);
			levelsSet(yVals);
		}

		operator Point() const;
	};

	ManualCornerCalibration(const CanvasPtr & canvas);

protected:
	virtual const char *configFileName()
	{ return "manualcornercalib.yml"; }

	virtual void showOptions();
	virtual void loadOptions(cv::FileStorage & fs);
	virtual void saveOptions(cv::FileStorage & fs);
	virtual void repaint();

private:
	int m_currentCorner;
	Levels m_currentXVals;
	Levels m_currentYVals;

	PointStorage m_corners[4];

	void cornerChanged();
};

}
