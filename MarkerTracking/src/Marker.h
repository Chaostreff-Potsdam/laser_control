#pragma once

#include <osg/Vec2>
#include <osg/Vec3>
#include <osg/Quat>
#include <osg/Matrix>

#include <opencv2/core/core.hpp>

#include <boost/timer/timer.hpp>

#include <Camera.h>

#include "ForwardDeclarations.h"

////////////////////////////////////////////////////////////////////////////////////////////////////

namespace markerTracking
{

// Forward declarations
class AlvarTracking;

////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Marker
//
////////////////////////////////////////////////////////////////////////////////////////////////////

class Marker
{
	public:
		enum class Reliability
		{
			__UNDEFINED,
			DETECTED,
			TRACKED,
			EXTRAPOLATED
		};

	public:
		Marker(const AlvarTracking &alvarTracking, int id);

		int id() const;
		const osg::Vec2 &pixelPosition() const;

		const osg::Vec3 referencePosition();
		const osg::Vec2 referenceOrientation();

		void resetCurrentReliability();
		Reliability currentReliability() const;

		void update(alvar::MarkerData &markerData, alvar::Pose &referencePose, alvar::Camera *camera);
		void track(const cv::Mat &previousFrame, const cv::Mat &currentFrame, cv::Mat &debugImage,
				   alvar::Pose &referencePose, alvar::Camera *camera);
		void computePositionAndOrientation(alvar::Pose &referencePose, alvar::Camera &camera);

		void debugPaint(cv::Mat &image, alvar::Camera &camera);

		bool isAlive() const;

		osg::Vec2 projectToCameraImage(const osg::Vec3 &point) const;
		osg::Vec3 projectToMarkerSpace(const osg::Vec2 &point, double zOffset = 0.0) const;
		osg::Vec3 projectToReferenceSpace(const osg::Vec2 &point, alvar::Pose &referencePose, double zOffset, alvar::Camera &camera);

	private:
		void matrixFromPose(alvar::Pose &pose, osg::Matrix &markerSpaceToCameraSpace, osg::Matrix &cameraSpaceToMarkerSpace, alvar::Camera *camera);

		const AlvarTracking &m_alvarTracking;

		const int m_id;

		osg::Vec3 m_referencePosition;
		osg::Vec2 m_referenceOrientation;

		osg::Vec2 m_pixelPosition;

		osg::Matrix m_markerSpaceToCameraSpace;
		osg::Matrix m_cameraSpaceToMarkerSpace;

		osg::Matrix m_markerSpaceToReferenceSpace;
		osg::Matrix m_referenceSpaceToMarkerSpace;

		boost::timer::cpu_timer m_timer;

		Reliability m_currentReliability;

		int m_numberOfVisibleFrames;
};

////////////////////////////////////////////////////////////////////////////////////////////////////

}
