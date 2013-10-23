#pragma once

#include "NetworkServer.h"
#include "Common.h"
#include "AlvarTracking.h"
#include "BonjourRegistration.h"
#include "AlvarTracking.h"
#include "FrameGrabber.h"
#include "ForwardDeclarations.h"

#include <opencv2/opencv.hpp>

////////////////////////////////////////////////////////////////////////////////////////////////////

namespace markerTracking
{

////////////////////////////////////////////////////////////////////////////////////////////////////
//
// MarkerTrackingDetail
//
////////////////////////////////////////////////////////////////////////////////////////////////////

class MarkerTrackingDetail
{
	public:
		MarkerTrackingDetail();
		virtual ~MarkerTrackingDetail();

		int run(int argc, char *argv[]);

	private:
		void loop();

		static void resize(const cv::Mat &rawImage, cv::Mat &resizedImage);
		static void convertToGrayscale(const cv::Mat &rgbImage, cv::Mat &grayScaleImage);

		void handleKey(char key);

		void sendNetworkUpdates();

		cv::Mat m_rawImage;
		cv::Mat m_resizedImage;
		cv::Mat m_processedImage;

		FrameGrabber *m_frameGrabber;

		AlvarTracking m_alvarTracking;

		network::NetworkServerPtr m_networkServer;

		BonjourRegistration m_bonjour;
};

////////////////////////////////////////////////////////////////////////////////////////////////////

}
