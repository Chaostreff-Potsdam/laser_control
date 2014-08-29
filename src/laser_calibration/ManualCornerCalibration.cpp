#include "ManualCornerCalibration.h"

namespace laser {

const int ManualCornerCalibration::levelDim = 32;
const int ManualCornerCalibration::levelMax = ManualCornerCalibration::levelDim - 1;
const int ManualCornerCalibration::levelCount = LASER_MAN_CORNER_CALIB_COUNT;

ManualCornerCalibration::PointStorage::operator Point() const
{
	double x = 0, y = 0;
	for (int i = 0; i < levelCount; i++) {
		x = x * levelDim + xVals[i];
		y = y * levelDim + yVals[i];
	}
	return Point(x * signX, y * signY);
}

////////////////////////////////////

void ManualCornerCalibration::levelsSet(Levels levels, int value)
{
	for (int i = 0; i < levelCount; i++) {
		levels[i] = value;
	}
}

ManualCornerCalibration::ManualCornerCalibration(const CanvasPtr &canvas) :
	AbstractCalibration(canvas),
	m_currentCorner(0)
{
	m_corners[0].signX = -1;
	m_corners[0].signY = -1;

	m_corners[1].signX = -1;

	m_corners[3].signY = -1;

	levelsSet(m_currentXVals);
	levelsSet(m_currentYVals);
}

void ManualCornerCalibration::showOptions()
{
	UpdateCallback cornerChangedClbk = [](int, void *t){
		static_cast<ManualCornerCalibration *>(t)->cornerChanged();
	};

	addTrackbar("corner", &m_currentCorner, 3, cornerChangedClbk);

	addTrackbar("xLvl0", &m_currentXVals[0], levelMax);
	addTrackbar("xLvl1", &m_currentXVals[1], levelMax);
	addTrackbar("xLvl2", &m_currentXVals[2], levelMax);

	addTrackbar("yLvl0", &m_currentYVals[0], levelMax);
	addTrackbar("yLvl1", &m_currentYVals[1], levelMax);
	addTrackbar("yLvl2", &m_currentYVals[2], levelMax);
}

void ManualCornerCalibration::loadOptions(cv::FileStorage &fs)
{
	for (int corner = 0; corner < 4; corner++) {
		for (int level = 0; level < levelCount; level++) {
			char idX[80], idY[80];
			snprintf(idX, 80, "c%dxLvl%d", corner, level);
			snprintf(idY, 80, "c%dyLvl%d", corner, level);

			if (!fs[idX].empty())
				fs[idX] >> m_corners[corner].xVals[level];

			if (!fs[idY].empty())
				fs[idY] >> m_corners[corner].yVals[level];
		}
		m_rect.setCorner(corner, m_corners[corner]);
	}
	memcpy(m_currentXVals, m_corners[0].xVals, sizeof(m_currentXVals));
	memcpy(m_currentYVals, m_corners[0].yVals, sizeof(m_currentYVals));
}

void ManualCornerCalibration::saveOptions(cv::FileStorage &fs)
{
	for (int corner = 0; corner < 4; corner++) {
		for (int level = 0; level < levelCount; level++) {
			char idX[80], idY[80];
			snprintf(idX, 80, "c%dxLvl%d", corner, level);
			snprintf(idY, 80, "c%dyLvl%d", corner, level);

			fs << idX << m_corners[corner].xVals[level];
			fs << idY << m_corners[corner].yVals[level];
		}
	}
}

void ManualCornerCalibration::repaint()
{
	memcpy(m_corners[m_currentCorner].xVals, m_currentXVals, sizeof(m_currentXVals));
	memcpy(m_corners[m_currentCorner].yVals, m_currentYVals, sizeof(m_currentYVals));

	m_rect.setCorner(m_currentCorner, m_corners[m_currentCorner]);

	AbstractCalibration::repaint();
}

void ManualCornerCalibration::cornerChanged()
{
	memcpy(m_currentXVals, m_corners[m_currentCorner].xVals, sizeof(m_currentXVals));
	memcpy(m_currentYVals, m_corners[m_currentCorner].yVals, sizeof(m_currentYVals));

	setTrackbarPos("xLvl0", m_currentXVals[0]);
	setTrackbarPos("xLvl1", m_currentXVals[1]);
	setTrackbarPos("xLvl2", m_currentXVals[2]);

	setTrackbarPos("yLvl0", m_currentYVals[0]);
	setTrackbarPos("yLvl1", m_currentYVals[1]);
	setTrackbarPos("yLvl2", m_currentYVals[2]);
}

}
