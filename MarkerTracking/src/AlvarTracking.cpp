#include "AlvarTracking.h"

#include <fstream>

#include <opencv2/highgui/highgui.hpp>

#include "Marker.h"
#include "Common.h"

////////////////////////////////////////////////////////////////////////////////////////////////////

namespace markerTracking
{

////////////////////////////////////////////////////////////////////////////////////////////////////
//
// AlvarTracking
//
////////////////////////////////////////////////////////////////////////////////////////////////////

AlvarTracking::AlvarTracking()
:   m_hasReferencePose(false),
	m_isUpdatingReferencePose(true)
{
	if (!m_camera.SetCalib("calib.xml", definitions::processingWidth, definitions::processingHeight))
		m_camera.SetRes(definitions::processingWidth, definitions::processingHeight);

	m_camera.SetSimpleCalib(definitions::processingWidth, definitions::processingHeight);

	double glProjectionMatrix[16];
	m_camera.GetOpenglProjectionMatrix(glProjectionMatrix, definitions::processingWidth, definitions::processingHeight);
	m_projectionMatrix = osg::Matrix(glProjectionMatrix);
	m_inverseProjectionMatrix = osg::Matrix::inverse(m_projectionMatrix);

	m_deviceMatrix
		= osg::Matrix::scale(0.5, -0.5, 1.0)
		* osg::Matrix::translate(0.5, 0.5, 0.0)
		* osg::Matrix::scale(definitions::processingWidth, definitions::processingHeight, 1.0);
	m_inverseDeviceMatrix = osg::Matrix::inverse(m_deviceMatrix);

	m_detector.SetOptions(true);
	m_detector.SetMarkerSize(definitions::hatMarkerSize);

	for (int i = 0; i < 4; i++)
		m_detector.SetMarkerSizeForId(definitions::referenceMarkerID + i, definitions::referenceMarkerSize);

	cv::namedWindow("testeier");

	// Initialize multimarker consisting of corner markers
	m_referenceMarkers.resize(4);

	for (int i = 0; i < 4; i++)
		m_referenceMarkers[i].SetId(-1);

	std::vector<int> indices(4);

	for (int i = 0; i < 4; i++)
		indices[i] = definitions::referenceMarkerID + i;

	m_cornerMarkers = alvar::MultiMarker(indices);

	alvar::Pose pose;
	pose.Reset();

	pose.SetTranslation(0, 0, 0);
	m_cornerMarkers.PointCloudAdd(definitions::referenceMarkerID + 0, definitions::referenceMarkerSize, pose);

	pose.SetTranslation(definitions::courtWidth, 0, 0);
	m_cornerMarkers.PointCloudAdd(definitions::referenceMarkerID + 1, definitions::referenceMarkerSize, pose);

	pose.SetTranslation(definitions::courtWidth, definitions::courtHeight, 0);
	m_cornerMarkers.PointCloudAdd(definitions::referenceMarkerID + 2, definitions::referenceMarkerSize, pose);

	pose.SetTranslation(0, definitions::courtHeight, 0);
	m_cornerMarkers.PointCloudAdd(definitions::referenceMarkerID + 3, definitions::referenceMarkerSize, pose);
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void AlvarTracking::process(const cv::Mat &frame)
{
	IplImage image = frame;

	cv::Mat debug;
	cv::cvtColor(frame, debug, CV_GRAY2BGR);

	boost::timer::cpu_timer test;
	m_detector.Detect(&image, &m_camera, false, false, 0.16, 0.2, alvar::CVSEQ, false);
	//std::cout << "[1] " << test.elapsed().wall / 1000000.0 << std::endl;

	test.start();

	if (updateReferenceCoordinateSystem())
		m_hasReferencePose = true;

	if (!m_hasReferencePose)
	{
		cv::resize(debug, debug, cv::Size(640, 400));
		cv::imshow("testeier", debug);
		return;
	}
	//std::cout << "[2] " << test.elapsed().wall / 1000000 << std::endl;

	test.start();
	for (const auto marker : m_markers)
		marker.second->resetCurrentReliability();

	for (alvar::MarkerData &markerData : *m_detector.markers)
	{
		const int id = markerData.GetId();

		MarkerPtr &marker = m_markers[id];

		if (!marker)
			marker = MarkerPtr(new Marker(*this, id));
		else
			marker->update(markerData, m_referencePose, &m_camera);
	}
	//std::cout << "[3] " << test.elapsed().wall / 1000000.0 << std::endl;

	test.start();
	/*if (m_previousFrame.rows > 0 && m_previousFrame.cols > 0)
		for (const auto marker : m_markers)
			if (marker.second->currentReliability() == Marker::Reliability::__UNDEFINED)
				marker.second->track(m_previousFrame, frame, debug, m_referencePose, &m_camera);*/
	//std::cout << "[4] " << test.elapsed().wall / 1000000.0 << std::endl;

	test.start();
	for (const auto marker : m_markers)
		marker.second->computePositionAndOrientation(m_referencePose, m_camera);
	//std::cout << "[5] " << test.elapsed().wall / 1000000.0 << std::endl;

	test.start();
	// Debug output
	for (auto marker : m_markers)
		marker.second->debugPaint(debug, m_camera);
	//std::cout << "[6] " << test.elapsed().wall / 1000000.0 << std::endl;

	cv::resize(debug, debug, cv::Size(640, 400));

	cv::imshow("testeier", debug);

	m_previousFrame = frame.clone();
}

////////////////////////////////////////////////////////////////////////////////////////////////////

bool AlvarTracking::updateReferenceCoordinateSystem()
{
	for (alvar::MarkerData &marker : *m_detector.markers)
	{
		const int id = marker.GetId();

		if (id < definitions::referenceMarkerID || id >= definitions::referenceMarkerID + 4)
			continue;

		m_referenceMarkers[id - definitions::referenceMarkerID] = marker;
	}

	int numberOfVisibleMarkers = 0;

	for (alvar::MarkerData &marker : m_referenceMarkers)
	{
		const int id = marker.GetId();

		if (id < definitions::referenceMarkerID || id >= definitions::referenceMarkerID + 4)
			continue;

		numberOfVisibleMarkers++;
	}

	if (numberOfVisibleMarkers < 4)
		return false;

	alvar::Pose pose;
	const double error = m_cornerMarkers.Update(m_detector.markers, &m_camera, pose);

	if (error < 0.0)
		return false;

	m_referencePose = pose;

	return true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

const osg::Matrix &AlvarTracking::projectionMatrix() const
{
	return m_projectionMatrix;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

const osg::Matrix &AlvarTracking::inverseProjectionMatrix() const
{
	return m_inverseProjectionMatrix;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

const osg::Matrix &AlvarTracking::deviceMatrix() const
{
	return m_deviceMatrix;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

const osg::Matrix &AlvarTracking::inverseDeviceMatrix() const
{
	return m_inverseDeviceMatrix;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

const AlvarTracking::Markers &AlvarTracking::markers() const
{
	return m_markers;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void AlvarTracking::debugPaint(cv::Mat &image)
{
	for (const Markers::value_type &marker : m_markers)
		marker.second->debugPaint(image, m_camera);
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void AlvarTracking::freezeCalibration()
{
	std::lock_guard<std::mutex> lock(m_isUpdatingReferencePoseMutex);

	m_isUpdatingReferencePose = false;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void AlvarTracking::unfreezeCalibration()
{
	std::lock_guard<std::mutex> lock(m_isUpdatingReferencePoseMutex);

	m_isUpdatingReferencePose = true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void AlvarTracking::saveCalibration()
{
	freezeCalibration();

	double glMatrix[16];
	m_referencePose.GetMatrixGL(glMatrix);

	std::ofstream file("calibration");
	file.write((char*)glMatrix, sizeof(glMatrix));
	file.close();
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void AlvarTracking::loadCalibration()
{
	freezeCalibration();

	double glMatrix[16];

	std::ifstream file("calibration");
	file.read((char*)glMatrix, sizeof(glMatrix));
	file.close();

	m_referencePose.SetMatrixGL(glMatrix);
	m_hasReferencePose = true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

}
