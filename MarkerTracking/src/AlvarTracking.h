#pragma once

#include <map>
#include <mutex>
#include <opencv2/core/core.hpp>
#include <osg/Matrix>
#include <MarkerDetector.h>
#include <Camera.h>
#include <MultiMarker.h>

#include "ForwardDeclarations.h"

////////////////////////////////////////////////////////////////////////////////////////////////////

namespace markerTracking
{

////////////////////////////////////////////////////////////////////////////////////////////////////
//
// AlvarTracking
//
////////////////////////////////////////////////////////////////////////////////////////////////////

class AlvarTracking
{
	public:
		typedef std::map<int, MarkerPtr> Markers;

	public:
		AlvarTracking();

		void process(const cv::Mat &frame);
		bool updateReferenceCoordinateSystem();

		const osg::Matrix &projectionMatrix() const;
		const osg::Matrix &deviceMatrix() const;

		const osg::Matrix &inverseProjectionMatrix() const;
		const osg::Matrix &inverseDeviceMatrix() const;

		const Markers &markers() const;

		void debugPaint(cv::Mat &image);

		void freezeCalibration();
		void unfreezeCalibration();
		void saveCalibration();
		void loadCalibration();

	private:
		alvar::MarkerDetector<alvar::MarkerData> m_detector;
		alvar::Camera m_camera;

		osg::Matrix m_projectionMatrix;
		osg::Matrix m_deviceMatrix;

		osg::Matrix m_inverseProjectionMatrix;
		osg::Matrix m_inverseDeviceMatrix;

		cv::Mat m_previousFrame;

		Markers m_markers;

		alvar::MultiMarker m_cornerMarkers;
		std::vector<alvar::MarkerData> m_referenceMarkers;

        bool m_hasReferencePose;
		alvar::Pose m_referencePose;

		bool m_isUpdatingReferencePose;
		std::mutex m_isUpdatingReferencePoseMutex;
};

////////////////////////////////////////////////////////////////////////////////////////////////////

}
