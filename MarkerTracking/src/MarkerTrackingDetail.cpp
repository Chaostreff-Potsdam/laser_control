#include "MarkerTrackingDetail.h"

#include <boost/timer/timer.hpp>

#include "Marker.h"

////////////////////////////////////////////////////////////////////////////////////////////////////

namespace markerTracking
{

////////////////////////////////////////////////////////////////////////////////////////////////////

MarkerTrackingDetail::MarkerTrackingDetail()
:	m_frameGrabber(new FrameGrabber)
{
	m_resizedImage = cv::Mat(markerTracking::definitions::processingHeight, markerTracking::definitions::processingWidth, CV_8UC3);
	m_processedImage = cv::Mat(markerTracking::definitions::processingHeight, markerTracking::definitions::processingWidth, CV_8UC1);
	m_networkServer = network::NetworkServerPtr(new network::NetworkServer(3100));
}

////////////////////////////////////////////////////////////////////////////////////////////////////

MarkerTrackingDetail::~MarkerTrackingDetail()
{
	delete m_frameGrabber;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

int MarkerTrackingDetail::run(int argc, char *argv[])
{
	if (argc > 1)
		m_frameGrabber->start(atoi(argv[1]), definitions::captureWidth, definitions::captureHeight);
	else
		m_frameGrabber->start(0, definitions::captureWidth, definitions::captureHeight);

	m_bonjour.registerService(3100);

	loop();

	m_frameGrabber->stop();

	return EXIT_SUCCESS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void MarkerTrackingDetail::resize(const cv::Mat &rawImage, cv::Mat &resizedImage)
{
	cv::resize(rawImage, resizedImage, resizedImage.size());
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void MarkerTrackingDetail::convertToGrayscale(const cv::Mat &rgbImage, cv::Mat &grayScaleImage)
{
	cv::cvtColor(rgbImage, grayScaleImage, CV_BGR2GRAY);
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void MarkerTrackingDetail::loop()
{
	char key = 0;

	boost::timer::cpu_timer timer;

	int frame = 0;

	do
	{
        boost::timer::cpu_timer test;
		m_networkServer->poll();
        //std::cout << "[A] " << test.elapsed().wall / 1000000.0 << std::endl;

		// Capture a frame from the camera and make sure we got one
        test.start();
		m_rawImage = m_frameGrabber->frame();

		assert(m_rawImage.cols > 0 && m_rawImage.rows > 0);

        //std::cout << "[B] " << test.elapsed().wall / 1000000.0 << std::endl;

        test.start();
		// Process the image (resize + grayscale)
        //resize(m_rawImage, m_resizedImage);
        convertToGrayscale(m_rawImage, m_processedImage);
        //std::cout << "[C] " << test.elapsed().wall / 1000000.0 << std::endl;

		// Handle keyboard input
        key = cv::waitKey(1);
		handleKey(key);

        test.start();
		// Detect and track markers in the image
		m_alvarTracking.process(m_processedImage);
        //std::cout << "[D] " << test.elapsed().wall / 1000000.0 << std::endl;

        test.start();
		// Send position updates via network
		sendNetworkUpdates();
        //std::cout << "[E] " << test.elapsed().wall / 1000000.0 << std::endl;

		frame++;

		if (timer.elapsed().wall > 1000.0 * 1000.0 * 1000.0)
		{
            std::cout << frame << std::endl;
			frame = 0;
			timer.start();
		}
	} while (key != 27 && key != 'q'); // Esc and q exits.
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void MarkerTrackingDetail::handleKey(char key)
{
	switch (key)
	{
		case 'f':
			m_alvarTracking.freezeCalibration();
			break;

		case 'g':
			m_alvarTracking.unfreezeCalibration();
			break;

		case 's':
			m_alvarTracking.saveCalibration();
			break;

		case 'l':
			m_alvarTracking.loadCalibration();
			break;
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void MarkerTrackingDetail::sendNetworkUpdates()
{
	const AlvarTracking::Markers &markers = m_alvarTracking.markers();

	cv::Mat debug(300, 400, CV_8UC3);
	debug = debug * 0;

	for (const auto marker : markers)
	{
		if (!marker.second || !marker.second->isAlive())
			continue;

		cv::Scalar color;

        if (marker.second->id() >= definitions::referenceMarkerID && marker.second->id() < definitions::referenceMarkerID + 4)
			color = cv::Scalar(255, 0, 0);
		else
			color = cv::Scalar(0, 0, 255);

		osg::Vec3 refPos = marker.second->referencePosition() * 40 + osg::Vec3(24, 24, 0);
		cv::circle(debug, cv::Point2d(refPos.x(), refPos.y()), 5, color);
	}

	cv::imshow("2D view", debug);

	// Send out marker positions
	for (const auto marker : markers)
	{
		if (marker.second->id() >= definitions::referenceMarkerID && marker.second->id() < definitions::referenceMarkerID + 4)
			continue;

		m_networkServer->sendMarkerInformation(marker.second);
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////

}
