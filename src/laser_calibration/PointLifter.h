#pragma once

#include "Calibration.h"

namespace laser {

class Painter;

/*! Compensates for parallax effects with lifted objects by reconstrucion the laser projectors 3D pose
 *
 * It assumes the CalibrationRectangle to depict a 5x5m square with the projector two floors up.
 *
 * This is based on an object pose from 3D-2D point correspondences (PnP).
 * see http://docs.opencv.org/modules/calib3d/doc/camera_calibration_and_3d_reconstruction.html
 *
 * We use following terminologies for different coordinate systems:
 *		* RealWorld (3D) - Position in meters on the playfield
 *		* Canvas    (2D) - The projectors output, as send to the etherdream
 *		* Painter   (2D) - Where you place and arrange objects in
 */

class EXPORT_LASER_CONTROL PointLifter
{
public:
	static const cv::Mat projectorMatrix;
	static std::vector<cv::Point2f> realWorldCornersOnGround();

	static PointLifter & sharedInstance()
	{ return s_sharedInstance; }

	PointLifter(Calibration & calibration);
	PointLifter(const cv::Mat & painterToCanvas = cv::Mat::eye(3, 3, CV_64FC1),
				const std::vector<cv::Point2f> canvasRectCorners = CalibrationRectangle::undistoredCorners_<cv::Point2f>());

	void liftPainterPoints(EtherdreamPoints & points, const double heightInMeters);

	void realWorldToCanvas(const std::vector<cv::Point3f> & pointsIn3D, std::vector<cv::Point2f> & pointsIn2D);
	std::vector<cv::Point3f> painterToRealWorld(const std::vector<cv::Point2f> &pointsIn2D, const double height);

private:
	static PointLifter s_sharedInstance;

	cv::Mat m_painterToCanvas;
	std::vector<cv::Point2f> m_canvasRectCorners;

	cv::Mat m_rotationVec;
	cv::Mat m_translationVec;

	//! Homography from canvas to real world coordinates on the ground
	cv::Mat m_canvasToRealWorld;

	//! Homography from painter to real world coordinates on the ground
	cv::Mat m_painterToRealWorld;
	cv::Mat m_canvasToPainter;

	void compute();

	friend class Painter;
};

}
