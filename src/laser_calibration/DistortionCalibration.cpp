#include "DistortionCalibration.h"

#include "objects/Line.h"
#include "objects/CompositeObject.h"
#include "utils/Transform.h"

#include <cmath>

namespace laser {

static const int trackLen = 50;
static const int trackZero = trackLen / 2;

double valFrom(int value)
{
	value -= trackZero;
	return 0.000000000001 * value;
}

DistorionCalibration::DistorionCalibration(const CanvasPtr &canvas, const cv::Mat &homography) :
	AbstractCalibration(canvas),
	m_homography(homography.clone()),
	m_currentDirection(HORIZONTAL),
	m_h(trackZero),
	m_v(trackZero)
{
	const double line_gap = (UINT16_MAX - 1) / (LINE_COUNT - 1);
	m_vlines = CompositeObject::construct();
	m_hlines = CompositeObject::construct();

	for (int i = 0; i < LINE_COUNT; ++i) {
		m_vlines->add(new Line(INT16_MIN, i * line_gap + INT16_MIN,
							   INT16_MAX, i * line_gap + INT16_MIN));
		m_hlines->add(new Line(i * line_gap + INT16_MIN, INT16_MIN,
							   i * line_gap + INT16_MIN, INT16_MAX));
	}
}

Transform::DistortionInfo DistorionCalibration::distortion()
{
	if (!m_distortion)
		compute();

	return m_distortion;
}

void DistorionCalibration::readCalibFrom(cv::FileStorage &fs)
{
	// Turns out the computed k1, h1 get rounded to zero when
	// written to file. Instead I will directly store my options here.
	cv::Mat m;
	fs[configKeyName()] >> m;
	assert(m.type() == CV_32S);
	assert(m.channels() == 1);
	assert(m.rows == 2);
	assert(m.cols == 1);

	m_h = m.at<int32_t>(0, 0);
	m_v = m.at<int32_t>(0,	1);
	compute();
}

void DistorionCalibration::writeCalibTo(cv::FileStorage &fs)
{
	int32_t data[2] = {m_h, m_v};
	cv::Mat m(2, 1, CV_32SC1, data);
	fs << configKeyName() << m;
}

void DistorionCalibration::loadOptions(cv::FileStorage &fs)
{
	fs["horizontalSlider"] >> m_h;
	fs["verticalSlider"] >> m_v;
}

void DistorionCalibration::saveOptions(cv::FileStorage &fs)
{
	fs << "horizontalSlider" << m_h;
	fs << "verticalSlider" << m_v;
}

void DistorionCalibration::showOptions()
{
	addTrackbar("line direction", &m_currentDirection, 1);

	addTrackbar("horizontal", &m_h, trackLen);
	addTrackbar("vertical", &m_v, trackLen);
}

CompositeObjectPtr DistorionCalibration::currentLines() const
{
	switch (m_currentDirection) {
		case VERTICAL:
			return m_vlines;
		case HORIZONTAL:
			return m_hlines;
		default:
			return CompositeObject::construct();
	}
}

EtherdreamPoints DistorionCalibration::pointsToPaint()
{
	auto points = currentLines()->pointsToPaint();
	compute();
	Transform::applyInPlace(points, cv::perspectiveTransform, m_homography);
	Transform::undistortInPlace(points, m_distortion);
	return points;
}

void DistorionCalibration::compute()
{
	m_distortion = Transform::DistortionInfo(valFrom(m_h), valFrom(m_v));
}

}
