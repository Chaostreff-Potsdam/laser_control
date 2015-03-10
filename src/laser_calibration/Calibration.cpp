#include "Calibration.h"

using namespace laser;

static const double shiftScale = 10000.0 /* Shiftable Distance */ / 1000.0;

Calibration::Calibration(const CanvasPtr & canvas) :
	PlanarCalibration(canvas),
	m_scale(100),
	m_xScale(100),
	m_yScale(100),
	m_keystoneFactor(100),
	m_xFlip(0),
	m_yFlip(0),
	m_xShift(500),
	m_yShift(500),
	m_rotation(0)
{
	;;
}

void Calibration::loadOptions(cv::FileStorage & fs)
{
	if (!fs["scale"].empty()) { // In case this is still an old calibration without these fields
		fs["scale"] >> m_scale;
		fs["xScale"] >> m_xScale;
		fs["yScale"] >> m_yScale;
		fs["keystoneFactor"] >> m_keystoneFactor;
		fs["m_xFlip"] >> m_xFlip;
		fs["m_yFlip"] >> m_yFlip;
	}

	if (!fs["xShift"].empty()) {
		fs["xShift"] >> m_xShift;
		fs["yShift"] >> m_yShift;
		fs["rotation"] >> m_rotation;

	}
}

void Calibration::saveOptions(cv::FileStorage & fs)
{
	fs << "scale" << m_scale;
	fs << "xScale" << m_xScale;
	fs << "yScale" << m_yScale;
	fs << "keystoneFactor" << m_keystoneFactor;
	fs << "m_xFlip" << m_xFlip;
	fs << "m_yFlip" << m_yFlip;
	fs << "xShift" << m_xShift;
	fs << "yShift" << m_yShift;
	fs << "rotation" << m_rotation;
}

void Calibration::showOptions()
{
	addTrackbar("Scale", &m_scale, 100);
	addTrackbar("x-Scale", &m_xScale, 100);
	addTrackbar("y-Scale", &m_yScale, 100);
	addTrackbar("Keystone", &m_keystoneFactor, 100);
	addTrackbar("x-Flip", &m_xFlip, 1);
	addTrackbar("y-Flip", &m_yFlip, 1);
	addTrackbar("x-Shift", &m_xShift, 1000);
	addTrackbar("y-Shift", &m_yShift, 1000);
	addTrackbar("rotation", &m_rotation, 3600);	
}

void Calibration::repaint()
{
	m_rect.resetTransform();
	m_rect.scale(m_scale / 100.0);
	m_rect.scale(m_xScale / 100.0, m_yScale / 100.0);
	m_rect.setKeystoneFactor(m_keystoneFactor / 100.0);
	m_rect.move(shiftScale * (m_xShift - 50), shiftScale * (m_yShift - 50));
	if (m_xFlip)
		m_rect.flipHorizontally();
	if (m_yFlip)
		m_rect.flipVertically();
	m_rect.rotate(radians(m_rotation * 0.1));

	PlanarCalibration::repaint();
}


