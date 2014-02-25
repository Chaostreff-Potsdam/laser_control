#include "Object.h"
#include "Transform.h"

#include <boost/date_time.hpp>

#include <opencv2/imgproc/imgproc.hpp>
#include <assert.h>

laser::Object::Object(const ObjectPtr & parent)
	: m_started(boost::date_time::microsec_clock<boost::posix_time::ptime>::universal_time()),
	  m_dirty(true),
	  m_parent(parent)
{
	resetTransform();
}

/**** Scene-graph ****/

laser::ObjectPtr laser::Object::parent() const
{
	return m_parent.lock();
}

void laser::Object::setParent(const ObjectPtr &newParent)
{
	if (ObjectPtr oldParent = parent()) {
		oldParent->removeChild(self());
	}
	m_parent = newParent;
}

void laser::Object::add(const ObjectPtr & object)
{
	object->setParent(self());
	m_children.emplace_back(object);
}

void laser::Object::add(const std::vector<ObjectPtr> &objects)
{
	for (ObjectPtr obj: objects)
		add(obj);
}

void laser::Object::removeChild(const ObjectPtr &object)
{
	std::remove(m_children.begin(), m_children.end(), object);
}

laser::ObjectPtr laser::Object::self()
{
#warning "Object::self not implemented!!!!"
#warning "Thus Object:add and Object::setParent are unsafe now."
	return ObjectPtr();
}

/**** Other ****/

boost::posix_time::ptime laser::Object::started() const
{
	return m_started;
}

void laser::Object::setPermanent(bool permanent)
{
	m_permanent = permanent;
}

bool laser::Object::permanent() const
{
	return m_permanent;
}

void laser::Object::update()
{
	m_untransformedPoints.clear();

	appendToVector(m_untransformedPoints, startPoints());
	appendToVector(m_untransformedPoints, points());
	appendToVector(m_untransformedPoints, endPoints());

	m_dirty = false;
	for (const ObjectPtr & child: m_children) {
		appendToVector(m_untransformedPoints, child->pointsToPaint());
	}

	if (ObjectPtr myParent = parent())
		myParent->update();
}

laser::EtherdreamPoints laser::Object::pointsToPaint() const
{
	/*if (m_dirty)
		update();*/

	// TODO: append directly to vector
	//       transform already before accessing that function.
	EtherdreamPoints ps = m_untransformedPoints;
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
