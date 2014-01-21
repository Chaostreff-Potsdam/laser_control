#include "LaserCompositeObject.h"
#include "laser_utilities.h"

#include "TransformPoint.h"

#include <opencv2/imgproc/imgproc.hpp>

laser::LaserCompositeObject::LaserCompositeObject(const std::vector<LaserObjectPtr> & objects)
	: LaserObject(),
	  m_transform(cv::Mat::eye(3, 3, CV_64FC1))
{
	m_objects = objects;
}

std::vector<etherdream_point> laser::LaserCompositeObject::points() const
{
	std::vector<etherdream_point> ps;

	for (const auto & obj : m_objects)
	{
		appendToVector(ps, obj->startPoints());
		appendToVector(ps, obj->points());
		appendToVector(ps, obj->endPoints());
	}

	cv::Mat openCVTrans = m_transform(cv::Rect(0, 0, 3, 2));

	applyTransformToEtherPoints(ps, [&](const TransformPoints & ins, const TransformPoints & outs) {
		cv::transform(ins, outs, openCVTrans);
	});

	return ps;
}

std::vector<etherdream_point> laser::LaserCompositeObject::startPoints() const
{
	return std::vector<etherdream_point>();
}

std::vector<etherdream_point> laser::LaserCompositeObject::endPoints() const
{
	return std::vector<etherdream_point>();
}

void laser::LaserCompositeObject::rotate(double rad)
{
	double s = std::sin(rad);
	double c = std::cos(rad);

	// Counter clockwise rotation about rad
	double m[3][3] = {{c, -s, 0}, {s, c, 0}, {0, 0, 1}};
	m_transform = cv::Mat(3, 3, CV_64FC1, m) * m_transform;
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

void laser::LaserCompositeObject::addObject(const laser::LaserObjectPtr &object)
{
	m_objects.push_back(object);
}
