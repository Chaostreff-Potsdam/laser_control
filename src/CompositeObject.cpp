#include "CompositeObject.h"
#include "laser_utilities.h"

#include "Transform.h"

#include <opencv2/imgproc/imgproc.hpp>

laser::CompositeObject::CompositeObject(const std::vector<ObjectPtr> & objects)
	: Object(),
	  m_objects(objects)
{
	resetTransform();
}

laser::EtherdreamPoints laser::CompositeObject::points() const
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

laser::EtherdreamPoints laser::CompositeObject::startPoints() const
{
	return EtherdreamPoints();
}

laser::EtherdreamPoints laser::CompositeObject::endPoints() const
{
	return EtherdreamPoints();
}

void laser::CompositeObject::add(const ObjectPtr & object)
{
	m_objects.emplace_back(object);
}

void laser::CompositeObject::add(const std::vector<ObjectPtr> &objects)
{
	appendToVector(m_objects, objects);
}

void laser::CompositeObject::rotate(double rad)
{
	// Counter clockwise rotation about rad
	double s = std::sin(rad);
	double c = std::cos(rad);
	double m[3][3] = {{c, -s, 0}, {s, c, 0}, {0, 0, 1}};
	m_transform = cv::Mat(3, 3, CV_64FC1, m) * m_transform;
}

void laser::CompositeObject::rotate(double rad, int centerX, int centerY, double scale)
{
	// TODO: Look up right matrix
	move(-centerX, -centerY);
	rotate(rad);
	move(centerX, centerY);
	if (scale != 1) this->scale(scale);
}

void laser::CompositeObject::move(int x, int y)
{
	double m[3][3] = {{1, 0, (double) x}, {0, 1, (double) y}, {0, 0, 1}};
	m_transform = cv::Mat(3, 3, CV_64FC1, m) * m_transform;
}

void laser::CompositeObject::scale(double factorX, double factorY)
{
	double m[3][3] = {{factorX, 0, 0}, {0, factorY, 0}, {0, 0, 1}};
	m_transform = cv::Mat(3, 3, CV_64FC1, m) * m_transform;
}

void laser::CompositeObject::scale(double factor)
{
	scale(factor, factor);
}

void laser::CompositeObject::resetTransform()
{
	m_transform = cv::Mat::eye(3, 3, CV_64FC1);
}
