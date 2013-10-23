#include "FrameGrabber.h"

////////////////////////////////////////////////////////////////////////////////////////////////////
//
// FrameGrabber
//
////////////////////////////////////////////////////////////////////////////////////////////////////

FrameGrabber::FrameGrabber()
:	m_isRunning(false)
{

}

////////////////////////////////////////////////////////////////////////////////////////////////////

FrameGrabber::~FrameGrabber()
{

}

////////////////////////////////////////////////////////////////////////////////////////////////////

void FrameGrabber::start(int deviceID, int captureWidth, int captureHeight)
{
	m_isRunning = true;

	m_videoCapture = VideoCapturePtr(new cv::VideoCapture(deviceID));

	if (!m_videoCapture->isOpened())
	{
		std::cout << "Error! Could not open video capture!" << std::endl;
		exit(EXIT_FAILURE);
	}

	m_videoCapture->set(CV_CAP_PROP_FRAME_WIDTH, captureWidth);
	m_videoCapture->set(CV_CAP_PROP_FRAME_HEIGHT, captureHeight);

	m_captureWidth = m_videoCapture->get(CV_CAP_PROP_FRAME_WIDTH);
	m_captureHeight = m_videoCapture->get(CV_CAP_PROP_FRAME_HEIGHT);

	m_thread = std::thread(std::bind(&FrameGrabber::startThread, this));
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void FrameGrabber::stop()
{
	m_isRunning = false;

	m_thread.join();

	m_videoCapture.reset();
}

////////////////////////////////////////////////////////////////////////////////////////////////////

bool FrameGrabber::isCapturing() const
{
	return m_videoCapture->isOpened();
}

////////////////////////////////////////////////////////////////////////////////////////////////////

cv::Mat FrameGrabber::frame() const
{
	cv::Mat clone;

	while (clone.cols <= 0 || clone.rows <= 0)
	{
		std::lock_guard<std::mutex> lock(m_currentFrameMutex);

		clone = m_currentFrame.clone();
		m_currentFrame = cv::Mat();
	}

	return clone;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void FrameGrabber::startThread()
{
	while (true)
	{
        (*m_videoCapture) >> m_nextFrame;

		m_currentFrameMutex.lock();
        m_currentFrame = m_nextFrame;
		m_currentFrameMutex.unlock();

		std::lock_guard<std::mutex> lock(m_isRunningMutex);

		if (!m_isRunning)
			break;
	}
}
