#include "PointLifter.h"
#include "Transform.h"

#include <opencv2/calib3d/calib3d.hpp>

namespace laser {

static double projectorMatrixData[3][3] = {{1 /*fx*/, 0       , 1 /*cx*/},
										   {0       , 1 /*fy*/, 1 /*cy*/},
										   {0       , 0       , 1       }};

const cv::Mat PointLifter::projectorMatrix(3, 3, CV_64FC1, projectorMatrixData);

PointLifter PointLifter::s_sharedInstance;

template<typename PointT>
static std::vector<cv::Point3f> raiseToHeight(const std::vector<PointT> & groundPoints, const double zInMeters)
{
	std::vector<cv::Point3f> points;
	points.reserve(points.size());

	for (const auto & p: groundPoints) {
		points.emplace_back(p.x, p.y, zInMeters);
	}
	return points;
}

std::vector<cv::Point2f> PointLifter::realWorldCornersOnGround()
{
	static const double fieldExtendsInM = 5;
	std::vector<cv::Point2f> points;

	points.emplace_back(0,               0);
	points.emplace_back(0,               fieldExtendsInM);
	points.emplace_back(fieldExtendsInM, fieldExtendsInM);
	points.emplace_back(fieldExtendsInM, 0);
	return points;
}

/////////////////////////////////////////////

PointLifter::PointLifter(Calibration & calibration) :
	m_painterToCanvas(calibration.homography()),
	m_canvasRectCorners(calibration.rect().corners())
{
	compute();
}

PointLifter::PointLifter(const cv::Mat &painterToCanvas, const std::vector<cv::Point2f> canvasRectCorners) :
	m_painterToCanvas(painterToCanvas),
	m_canvasRectCorners(canvasRectCorners)
{
	compute();
}

void PointLifter::compute()
{
	cv::solvePnP(raiseToHeight(realWorldCornersOnGround(), 0), m_canvasRectCorners,
				 projectorMatrix, cv::Mat(),
				 m_rotationVec, m_translationVec);

	m_canvasToRealWorld = cv::findHomography(realWorldCornersOnGround(), m_canvasRectCorners);
	m_painterToRealWorld = m_painterToCanvas * m_canvasToRealWorld;
	m_canvasToPainter = m_painterToCanvas.inv();
}

void PointLifter::realWorldToCanvas(cv::InputArray pointsIn3D, cv::OutputArray pointsIn2D)
{
	cv::projectPoints(pointsIn3D, m_rotationVec, m_translationVec,
					  projectorMatrix, cv::Mat(),
					  pointsIn2D);
}

void PointLifter::canvasToRealWorld(cv::InputArray pointsIn2D, std::vector<cv::Point3f> & points3DOnGround)
{
	std::vector<cv::Point2f> rwCords;

	cv::perspectiveTransform(pointsIn2D, rwCords, m_canvasToRealWorld);
	points3DOnGround = raiseToHeight(rwCords, 0);
}

void PointLifter::liftPainterPoints(EtherdreamPoints & points, const double heightInMeters)
{
	std::vector<cv::Point2f> aux;
	aux.reserve(points.size());

	for (const auto & p: points) {
		aux.emplace_back(p.x, p.y);
	}

	cv::perspectiveTransform(aux, aux, m_painterToRealWorld);
	std::vector<cv::Point3f> liftedPoints = raiseToHeight(aux, heightInMeters);

	realWorldToCanvas(liftedPoints, aux);
	cv::perspectiveTransform(aux, aux, m_canvasToPainter);

	for (unsigned int i = 0; i < points.size(); i++) {
		points[i].x = (int16_t) clamp(aux[i].x, INT16_MIN, INT16_MAX);
		points[i].y = (int16_t) clamp(aux[i].y, INT16_MIN, INT16_MAX);
	}
}

}
