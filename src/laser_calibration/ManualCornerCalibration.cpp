#include "ManualCornerCalibration.h"

namespace laser {

const int ManualCornerCalibration::levelDim = 32;
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

ManualCornerCalibration::ManualCornerCalibration(const CanvasPtr &canvas) :
	AbstractCalibration(canvas),
	m_currentCorner(0)
{
	;;
}

void ManualCornerCalibration::showOptions()
{
	addTrackbar("corner", &m_currentCorner, 4);
}

void ManualCornerCalibration::updateCurrentCorner()
{
	memcpy(points[m_currentCorner].xVals, m_currentXVals, sizeof(m_currentXVals));
	memcpy(points[m_currentCorner].yVals, m_currentYVals, sizeof(m_currentYVals));
}

void ManualCornerCalibration::repaint()
{
	updateCurrentCorner();
	m_rect.setCorner(m_currentCorner, points[m_currentCorner]);

	AbstractCalibration::repaint();
}

}
