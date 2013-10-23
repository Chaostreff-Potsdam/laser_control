#pragma

#include <thread>
#include <mutex>

#include <opencv2/opencv.hpp>

////////////////////////////////////////////////////////////////////////////////////////////////////
//
// FrameGrabber
//
////////////////////////////////////////////////////////////////////////////////////////////////////

typedef std::shared_ptr<cv::VideoCapture> VideoCapturePtr;

class FrameGrabber
{
	public:
		FrameGrabber();
		virtual ~FrameGrabber();

		void start(int deviceID, int captureWidth, int captureHeight);
		void stop();

		bool isCapturing() const;

		cv::Mat frame() const;

	private:
		void startThread();

		bool m_isRunning;
		std::mutex m_isRunningMutex;

		std::thread m_thread;

		int m_captureWidth;
		int m_captureHeight;

		mutable cv::Mat m_currentFrame;
		mutable std::mutex m_currentFrameMutex;

		cv::Mat m_nextFrame;

		VideoCapturePtr m_videoCapture;
};
