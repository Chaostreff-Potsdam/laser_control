#include "CompositeObject.h"
#include "laser_utilities.h"

#include "Transform.h"

#include <opencv2/imgproc/imgproc.hpp>

laser::LaserCompositeObject::LaserCompositeObject(const std::vector<LaserObjectPtr> & objects)
	: LaserObject(),
	  m_objects(objects)
{
	resetTransform();
}

laser::EtherdreamPoints laser::LaserCompositeObject::points() const
{
	EtherdreamPoints ps;

	for (const auto & obj : m_objects)
	{
		appendToVector(ps, obj->startPoints());
		appendToVector(ps, obj->points());
		appendToVector(ps, obj->endPoints());
	}

	Transform::applyInPlace(ps, cv::transform, m_transform(cv::Rect(0, 0, 3, 2)));

	return ps;
}

laser::EtherdreamPoints laser::LaserCompositeObject::startPoints() const
{
	return EtherdreamPoints();
}

laser::EtherdreamPoints laser::LaserCompositeObject::endPoints() const
{
	return EtherdreamPoints();
}

void laser::LaserCompositeObject::add(const LaserObjectPtr & object)
{
	m_objects.emplace_back(object);
}

void laser::LaserCompositeObject::add(const std::vector<LaserObjectPtr> &objects)
{
	appendToVector(m_objects, objects);
}

void laser::LaserCompositeObject::rotate(double rad)
{
	// Counter clockwise rotation about rad
	double s = std::sin(rad);
	double c = std::cos(rad);
	double m[3][3] = {{c, -s, 0}, {s, c, 0}, {0, 0, 1}};
	m_transform = cv::Mat(3, 3, CV_64FC1, m) * m_transform;
}

void laser::LaserCompositeObject::rotate(double rad, int centerX, int centerY, double scale)
{
	// TODO: Look up right matrix
	move(-centerX, -centerY);
	rotate(rad);
	move(centerX, centerY);
	if (scale != 1) this->scale(scale);
}

void laser::LaserCompositeObject::move(int x, int y)
{
	double m[3][3] = {{1, 0, (double) x}, {0, 1, (double) y}, {0, 0, 1}};
	m_transform = cv::Mat(3, 3, CV_64FC1, m) * m_transform;
}

void laser::LaserCompositeObject::scale(double factorX, double factorY)
{
	double m[3][3] = {{factorX, 0, 0}, {0, factorY, 0}, {0, 0, 1}};
	m_transform = cv::Mat(3, 3, CV_64FC1, m) * m_transform;
}

void laser::LaserCompositeObject::scale(double factor)
{
	scale(factor, factor);
}

void laser::LaserCompositeObject::resetTransform()
{
	m_transform = cv::Mat::eye(3, 3, CV_64FC1);
}
