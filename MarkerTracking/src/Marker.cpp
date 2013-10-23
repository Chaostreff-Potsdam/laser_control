#include "Marker.h"

#include <assert.h>
#include <opencv2/highgui/highgui.hpp>
#include <osg/io_utils>

#include <Marker.h>

#include "AlvarTracking.h"
#include "Common.h"

namespace markerTracking
{

////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Marker
//
////////////////////////////////////////////////////////////////////////////////////////////////////

Marker::Marker(const AlvarTracking &alvarTracking, int id)
:	m_alvarTracking(alvarTracking),
	m_id(id),
	m_currentReliability(Reliability::__UNDEFINED),
	m_numberOfVisibleFrames(0)
{

}

////////////////////////////////////////////////////////////////////////////////////////////////////

int Marker::id() const
{
	return m_id;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

const osg::Vec2 &Marker::pixelPosition() const
{
	return m_pixelPosition;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

const osg::Vec3 Marker::referencePosition()
{
	return m_referencePosition;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

const osg::Vec2 Marker::referenceOrientation()
{
	return m_referenceOrientation;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void Marker::resetCurrentReliability()
{
	m_currentReliability = Reliability::__UNDEFINED;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

Marker::Reliability Marker::currentReliability() const
{
	return m_currentReliability;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void Marker::update(alvar::MarkerData &markerData, alvar::Pose &referencePose, alvar::Camera *camera)
{
	m_numberOfVisibleFrames++;

	m_currentReliability = Marker::Reliability::DETECTED;

	osg::Matrix referenceSpaceToCameraSpace, cameraSpaceToReferenceSpace;

	matrixFromPose(markerData.pose, m_markerSpaceToCameraSpace, m_cameraSpaceToMarkerSpace, camera);
	matrixFromPose(referencePose, referenceSpaceToCameraSpace, cameraSpaceToReferenceSpace, camera);

	m_markerSpaceToReferenceSpace = m_markerSpaceToCameraSpace * cameraSpaceToReferenceSpace;
	m_referenceSpaceToMarkerSpace = referenceSpaceToCameraSpace * m_cameraSpaceToMarkerSpace;

	m_pixelPosition = projectToCameraImage(osg::Vec3());

	m_timer.start();
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void Marker::track(const cv::Mat &previousFrame, const cv::Mat &currentFrame, cv::Mat &debugImage,
				   alvar::Pose &referencePose, alvar::Camera *camera)
{
	if (!isAlive())
	{
		m_numberOfVisibleFrames = 0;

		return;
	}

	assert(previousFrame.rows > 0 && previousFrame.cols > 0);

	m_currentReliability = Reliability::TRACKED;

	std::vector<uchar> status;
	std::vector<float> err;
	cv::TermCriteria termcrit(CV_TERMCRIT_ITER | CV_TERMCRIT_EPS, 20, 0.03);
    static const cv::Size kernelSize(10, 10);
    static const cv::Size windowSize(30, 30);

    const int startX = m_pixelPosition.x() - windowSize.width / 2;
    const int endX = m_pixelPosition.x() + windowSize.width / 2;
    const int startY = m_pixelPosition.y() - windowSize.height / 2;
    const int endY = m_pixelPosition.y() + windowSize.height / 2;

	std::vector<cv::Point2f> points;
	std::vector<cv::Point2f> trackedPoints;

    for (int x = startX; x < endX; x += 4)
        for (int y = startY; y < endY; y += 4)
			points.push_back(cv::Point2f(x, y));

	cv::calcOpticalFlowPyrLK(previousFrame, currentFrame,
							 points, trackedPoints,
							 status, err,
							 kernelSize, 3, termcrit, 0);

	assert(points.size() == trackedPoints.size());

	cv::Vec2d meanMovement(0.0, 0.0);

	for (int i = 0; i < points.size(); i++)
	{
		const cv::Vec2d movement = (cv::Vec2f)(trackedPoints[i] - points[i]);
		meanMovement += movement;
	}

	meanMovement *= 1.0 / trackedPoints.size();

	m_pixelPosition = m_pixelPosition + osg::Vec2(meanMovement[0], meanMovement[1]);

	const osg::Vec3 newPositionInMarkerSpace
		= projectToMarkerSpace(m_pixelPosition);

	osg::Matrix translateInMarkerSpace = osg::Matrix::translate(newPositionInMarkerSpace);

	m_markerSpaceToCameraSpace = translateInMarkerSpace * m_markerSpaceToCameraSpace;

	m_cameraSpaceToMarkerSpace = osg::Matrix::inverse(m_markerSpaceToCameraSpace);

	osg::Matrix referenceSpaceToCameraSpace, cameraSpaceToReferenceSpace;

	matrixFromPose(referencePose, referenceSpaceToCameraSpace, cameraSpaceToReferenceSpace, camera);

	m_markerSpaceToReferenceSpace = m_markerSpaceToCameraSpace * cameraSpaceToReferenceSpace;
	m_referenceSpaceToMarkerSpace = osg::Matrix::inverse(m_markerSpaceToReferenceSpace);

	for (int i = 0; i < points.size(); i++)
		cv::line(debugImage, points[i], trackedPoints[i], cv::Scalar(255, 0, 255));
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void Marker::computePositionAndOrientation(alvar::Pose &referencePose, alvar::Camera &camera)
{
	osg::Vec3 relPos = osg::Vec3() * m_markerSpaceToReferenceSpace;

	// Reference markers reside in the same plane
	if (m_id >= definitions::referenceMarkerID && m_id < definitions::referenceMarkerID + 4)
		relPos.z() = 0;

	const osg::Vec2 centerInCameraImage = projectToCameraImage(osg::Vec3());
	m_referencePosition = projectToReferenceSpace(centerInCameraImage, referencePose, -relPos.z(), camera);

	const osg::Vec2 cI = projectToCameraImage(osg::Vec3());
	const osg::Vec2 dI = projectToCameraImage(osg::X_AXIS);

	const osg::Vec3 cR = projectToReferenceSpace(cI, referencePose, -relPos.z(), camera);
	const osg::Vec3 dR = projectToReferenceSpace(dI, referencePose, -relPos.z(), camera);

	const osg::Vec3 orientation3D = dR - cR;

	m_referenceOrientation = osg::Vec2(orientation3D.x(), -orientation3D.y());
	m_referenceOrientation.normalize();
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void Marker::debugPaint(cv::Mat &image, alvar::Camera &camera)
{
	if (!isAlive())
		return;

	cv::Scalar color;

	switch (m_currentReliability)
	{
		case Reliability::DETECTED:
			color = cv::Scalar(0, 192, 32);
			break;

		case Reliability::TRACKED:
			color = cv::Scalar(0, 128, 255);
			break;

		case Reliability::EXTRAPOLATED:
			color = cv::Scalar(0, 0, 192);
			break;

		default:
			return;
	}

	if (m_id == definitions::referenceMarkerID)
		color = cv::Scalar(192, 64, 0);

	const osg::Vec2 originInCameraImage = projectToCameraImage(osg::Vec3());

	const osg::Vec2 orientationPointInCameraSpace = originInCameraImage + referenceOrientation() * 25.0;

	const cv::Point2d cvOrigin(originInCameraImage.x(), originInCameraImage.y());
	const cv::Point2d cvOrientationPoint(orientationPointInCameraSpace.x(), orientationPointInCameraSpace.y());

	cv::ellipse(image, cvOrigin, cv::Size(5, 5), 0, 0, 360, color, -1);
	cv::line(image, cvOrigin, cvOrientationPoint, cv::Scalar(0, 0, 255), 2);

	osg::Vec3 corners[10];
	corners[0] = osg::Vec3(-1, -1, 0);
	corners[1] = osg::Vec3(1, -1, 0);
	corners[2] = osg::Vec3(1, 1, 0);
	corners[3] = osg::Vec3(-1, 1, 0);
	corners[4] = osg::Vec3(-1, -1, 2);
	corners[5] = osg::Vec3(1, -1, 2);
	corners[6] = osg::Vec3(1, 1, 2);
	corners[7] = osg::Vec3(-1, 1, 2);

	corners[8] = osg::Vec3();
	corners[9] = osg::Vec3(1, 0, 0);

	for (int i = 0; i < 10; i++)
		corners[i] = corners[i] * definitions::referenceMarkerSize / 2;

	auto drawLine = [&](int a, int b, cv::Scalar color = cv::Scalar(192, 0, 0))
	{
		const osg::Vec2 ap = projectToCameraImage(corners[a]);
		const osg::Vec2 bp = projectToCameraImage(corners[b]);

		cv::line(image, cv::Point2d(ap.x(), ap.y()), cv::Point2d(bp.x(), bp.y()), color, 2);

		return;

		std::stringstream text;
		text << corners[a] - corners[b];

		cv::putText(image, text.str(), cv::Point2d(ap.x(), ap.y()), CV_FONT_HERSHEY_COMPLEX, 0.5, cv::Scalar(255, 255, 255));
	};

	drawLine(0, 1, cv::Scalar(255, 0, 192));
	drawLine(1, 2, cv::Scalar(255, 0, 192));
	drawLine(2, 3, color);
	drawLine(3, 0, color);

	/*drawLine(4, 5, cv::Scalar(255, 0, 192));
	drawLine(5, 6, cv::Scalar(255, 0, 192));
	drawLine(6, 7, color);
	drawLine(7, 4, color);

	drawLine(0, 4, color);
	drawLine(1, 5, color);
	drawLine(2, 6, color);
	drawLine(3, 7, color);*/

	//drawLine(8, 9, cv::Scalar(0, 0, 192));
}

////////////////////////////////////////////////////////////////////////////////////////////////////

osg::Vec2 Marker::projectToCameraImage(const osg::Vec3 &point) const
{
	const osg::Vec3 projectedPoint = point
		* m_markerSpaceToCameraSpace
		* m_alvarTracking.projectionMatrix()
		* m_alvarTracking.deviceMatrix();

	return osg::Vec2(projectedPoint.x(), projectedPoint.y());
}

////////////////////////////////////////////////////////////////////////////////////////////////////

osg::Vec3 Marker::projectToMarkerSpace(const osg::Vec2 &point, double zOffset) const
{
	const osg::Matrix cameraImageToMarkerSpace
		= m_alvarTracking.inverseDeviceMatrix()
		* m_alvarTracking.inverseProjectionMatrix()
		* m_cameraSpaceToMarkerSpace
		* osg::Matrix::translate(0.0, 0.0, zOffset);

	// p1 and p2 form the line (in reference space) in which the 2D camera image point could
	// plausible be
	const osg::Vec3 p1 = osg::Vec3(point, 0) * cameraImageToMarkerSpace;
	const osg::Vec3 p2 = osg::Vec3(point, 100) * cameraImageToMarkerSpace;

	const osg::Vec3 d = (p2 - p1);

	// Compute the intersection, where z = 0
	const osg::Vec3 intersection = p1 - d * (p1.z() / d.z());

	return intersection;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

osg::Vec3 Marker::projectToReferenceSpace(const osg::Vec2 &point, alvar::Pose &referencePose, double zOffset, alvar::Camera &camera)
{
	osg::Matrix referenceSpaceToCameraSpace;
	osg::Matrix cameraSpaceToReferenceSpace;

	matrixFromPose(referencePose, referenceSpaceToCameraSpace, cameraSpaceToReferenceSpace, &camera);

	const osg::Matrix cameraImageToMarkerSpace
		= m_alvarTracking.inverseDeviceMatrix()
		* m_alvarTracking.inverseProjectionMatrix()
		* cameraSpaceToReferenceSpace
		* osg::Matrix::translate(0.0, 0.0, zOffset);

	// p1 and p2 form the line (in reference space) in which the 2D camera image point could
	// plausible be
	const osg::Vec3 p1 = osg::Vec3(point, 0) * cameraImageToMarkerSpace;
	const osg::Vec3 p2 = osg::Vec3(point, 100) * cameraImageToMarkerSpace;

	const osg::Vec3 d = (p2 - p1);

	// Compute the intersection, where z = 0
	const osg::Vec3 intersection = p1 - d * (p1.z() / d.z());

	return intersection;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void Marker::matrixFromPose(alvar::Pose &pose, osg::Matrix &markerSpaceToCameraSpace, osg::Matrix &cameraSpaceToMarkerSpace,
	alvar::Camera *camera)
{
	auto computeMatrix = [&](osg::Matrix &mat, osg::Matrix &inv)
	{
		double glMatrix[16];

		pose.GetMatrixGL(glMatrix);

		mat = osg::Matrix(glMatrix);
		inv = osg::Matrix::inverse(mat);
	};

	osg::Matrix m, i;

	computeMatrix(m, i);

	markerSpaceToCameraSpace = m;
	cameraSpaceToMarkerSpace = i;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

bool Marker::isAlive() const
{
	return m_numberOfVisibleFrames > 5 && m_timer.elapsed().wall <= 1000.0 * 1000.0 * 1000.0;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

}
