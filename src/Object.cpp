#include "Object.h"
#include "Transform.h"

#include "boost/date_time.hpp"

#include <opencv2/imgproc/imgproc.hpp>

laser::Object::Object()
:	m_started(boost::date_time::microsec_clock<boost::posix_time::ptime>::universal_time()),
	m_dirty(false)
{
	resetTransform();
}

boost::posix_time::ptime laser::Object::started()
{
	return m_started;
}

void laser::Object::setPermanent(bool permanent)
{
	m_permanent = permanent;
}

bool laser::Object::permanent()
{
	return m_permanent;
}

void laser::Object::update()
{
	// TODO: notify parent, that we've updated
	m_points = points();
	m_startPoints = startPoints();
	m_endPoints = endPoints();
	m_dirty = false;
}

laser::EtherdreamPoints laser::Object::pointsToPaint() const
{
	// TODO: append directly to vector
	//       transform already before accessing that function.
	EtherdreamPoints ps;

	appendToVector(ps, m_startPoints);
	appendToVector(ps, m_points);
	appendToVector(ps, m_endPoints);

	Transform::applyInPlace(ps, cv::transform, m_transform(cv::Rect(0, 0, 3, 2)));
	return ps;
}


/**** Transform ***/

void laser::Object::rotate(double rad)
{
	// Counter clockwise rotation about rad
	double s = std::sin(rad);
	double c = std::cos(rad);
	double m[3][3] = {{c, -s, 0}, {s, c, 0}, {0, 0, 1}};
	m_transform = cv::Mat(3, 3, CV_64FC1, m) * m_transform;
}

void laser::Object::rotate(double rad, int centerX, int centerY, double scale)
{
	// TODO: Look up right matrix
	move(-centerX, -centerY);
	rotate(rad);
	move(centerX, centerY);
	if (scale != 1) this->scale(scale);
}

void laser::Object::move(int x, int y)
{
	double m[3][3] = {{1, 0, (double) x}, {0, 1, (double) y}, {0, 0, 1}};
	m_transform = cv::Mat(3, 3, CV_64FC1, m) * m_transform;
}

void laser::Object::scale(double factorX, double factorY)
{
	double m[3][3] = {{factorX, 0, 0}, {0, factorY, 0}, {0, 0, 1}};
	m_transform = cv::Mat(3, 3, CV_64FC1, m) * m_transform;
}

void laser::Object::scale(double factor)
{
	scale(factor, factor);
}

void laser::Object::resetTransform()
{
	m_transform = cv::Mat::eye(3, 3, CV_64FC1);
}
