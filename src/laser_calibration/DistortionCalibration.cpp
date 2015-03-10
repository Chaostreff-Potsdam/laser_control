#include "DistortionCalibration.h"

#include "objects/Line.h"
#include "objects/CompositeObject.h"
#include "Transform.h"

#include <cmath>

namespace laser {

static const int trackLen = 50;
static const int trackZero = trackLen / 2;

template <typename T> int sgn(T val) {
	return (T(0) < val) - (val < T(0));
}

double valFrom(int value)
{
	value -= trackZero;
	return 0.000000000001 * value;
}

DistorionCalibration::DistorionCalibration(const CanvasPtr &canvas) :
	AbstractCalibration(canvas),
	m_distCoeffs(0, 0, CV_64FC1, nullptr),
	m_currentDirection(HORIZONTAL),
	m_k1(trackZero),
	m_h1(trackZero)
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

cv::Mat DistorionCalibration::distCoeffs()
{
	if (m_distCoeffs.empty())
		compute();

	return m_distCoeffs;
}

void DistorionCalibration::readCalibFrom(cv::FileStorage &fs)
{
	fs[configKeyName()] >> m_distCoeffs;
}

void DistorionCalibration::writeCalibTo(cv::FileStorage &fs)
{
	fs << configKeyName() << m_distCoeffs;
}

void DistorionCalibration::loadOptions(cv::FileStorage &fs)
{
	fs["k1"] >> m_k1;
	fs["h1"] >> m_h1;
}

void DistorionCalibration::saveOptions(cv::FileStorage &fs)
{
	fs << "k1" << m_k1;
	fs << "h1" << m_h1;
}

void DistorionCalibration::showOptions()
{
	addTrackbar("direction", &m_currentDirection, 1);

	addTrackbar("k1", &m_k1, trackLen);
	addTrackbar("h1", &m_h1, trackLen);
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
	return Transform::undistort(points, m_distCoeffs);
}

void DistorionCalibration::compute()
{
	// Filled dummy, making sure we loaded something
	m_distCoeffs = cv::Mat::zeros(5, 1, CV_64FC1);
	m_distCoeffs.at<double>(0, 0) = valFrom(m_k1);
}

}
