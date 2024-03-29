#include "Object.h"
#include "../utils/Transform.h"
#include "CompositeObject.h"

#include <boost/date_time.hpp>

#include <opencv2/imgproc/imgproc.hpp>
#include <assert.h>

int laser::Object::s_pixelsPerPointDefault = 1000;
int laser::Object::s_marginPointFractionDefault = 8;

const int laser::Object::useDefaultValue = -1;

laser::Object::Object()
	: m_started(boost::date_time::microsec_clock<boost::posix_time::ptime>::universal_time()),
	  m_pixelsPerPoint(useDefaultValue),
	  m_marginPointFraction(useDefaultValue),
	  m_isUpdating(false),
	  m_flags(none),
	  m_isVisible(true)
{
	resetTransform();
}

/**** For scene graph structure with CompositeObject ****/

laser::Object::parent_t laser::Object::parent() const
{
	return m_parent.lock();
}

void laser::Object::setParent(const parent_t & newParent)
{
	if (parent_t oldParent = parent()) {
		oldParent->removeChild(this);
	}
	m_parent = newParent;
}

void laser::Object::rebuildCache()
{
	if (m_isUpdating)
		return;

	m_isUpdating = true;

	m_cache.clear();

	appendToVector(m_cache, startPoints());
	appendToVector(m_cache, points());
	appendToVector(m_cache, endPoints());

	Transform::applyInPlace(m_cache, cv::transform, m_transform(cv::Rect(0, 0, 3, 2)));

	// Now I'm rebuild and I prevent my parent from calling
	// this function
	m_dirty = false;
	if (parent_t myParent = parent()) {
		myParent->rebuildCache();
	}
	m_isUpdating = false;
}

bool laser::Object::empty() const
{
	return !m_isVisible || (startPoints().empty() && endPoints().empty() && points().empty());
}

void laser::Object::setFlags(const Flags flags)
{
	m_flags = flags;
	nowDirty();
}

laser::EtherdreamPoints laser::Object::pointsToPaint()
{
	if (!m_isVisible)
		return EtherdreamPoints();

	//if (m_dirty)
		rebuildCache();

	return m_cache;
}

void laser::Object::nowDirty()
{
	m_dirty = true;
	if (parent_t p = parent()) {
		p->nowDirty();
	}
}

const etherdream_point laser::Object::etherdreamPoint(int x, int y, bool visible) const
{
	etherdream_point p;
	memset(&p, 0, sizeof(p));

	//flo: remove all clamping, except directly before sending them to the dac? after all transformations etc.
	p.x = clamp(x, INT16_MIN, INT16_MAX);
	p.y = clamp(y, INT16_MIN, INT16_MAX);

	p.r = visible ? m_color.red() : 0;
	p.g = visible ? m_color.green() : 0;
	p.b = visible ? m_color.blue() : 0;
	return p;
}

/**** Transform ***/

void laser::Object::rotate(double rad)
{
	// Counter clockwise rotation about rad
	double s = std::sin(rad);
	double c = std::cos(rad);
	double m[3][3] = {{c, -s, 0}, {s, c, 0}, {0, 0, 1}};
	m_transform = cv::Mat(3, 3, CV_64FC1, m) * m_transform;
	nowDirty();
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
	nowDirty();
}

void laser::Object::scale(double factorX, double factorY)
{
	double m[3][3] = {{factorX, 0, 0}, {0, factorY, 0}, {0, 0, 1}};
	m_transform = cv::Mat(3, 3, CV_64FC1, m) * m_transform;
	nowDirty();
}

void laser::Object::scale(double factor)
{
	scale(factor, factor);
}

void laser::Object::flipHorizontally()
{
	double m[3][3] = {{-1, 0, 0}, {0, 1, 0}, {0, 0, 1}};
	m_transform = cv::Mat(3, 3, CV_64FC1, m) * m_transform;
	nowDirty();
}

void laser::Object::flipVertically()
{
	double m[3][3] = {{1, 0, 0}, {0, -1, 0}, {0, 0, 1}};
	m_transform = cv::Mat(3, 3, CV_64FC1, m) * m_transform;
	nowDirty();
}

void laser::Object::resetTransform()
{
	m_transform = cv::Mat::eye(3, 3, CV_64FC1);
	nowDirty();
}

void laser::Object::addAnimation(const Animation::Func &func, const std::chrono::milliseconds & period)
{
	m_animations.emplace_back(Animation::construct(this, func, true, period));
}

void laser::Object::removeAllAnimations()
{
	m_animations.clear();
}

void laser::Object::setVisible(bool visible)
{
	m_isVisible = visible;
	nowDirty();
}

bool laser::Object::visible()
{
	return m_isVisible;
}

void laser::Object::setPixelsPerPoint(int pixelsPerPoint)
{
	m_pixelsPerPoint = pixelsPerPoint;
	nowDirty();
}

void laser::Object::setMarginPointFraction(int marginPointFraction)
{
	m_marginPointFraction = marginPointFraction;
	nowDirty();
}

void laser::Object::setColor(const Color & color)
{
	m_color = color;
	nowDirty();
}

laser::Color laser::Object::color()
{
	return m_color;
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

int laser::Object::pixelsPerPoint() const
{
	return m_pixelsPerPoint == useDefaultValue ? s_pixelsPerPointDefault : m_pixelsPerPoint;
}

int laser::Object::marginPointFraction() const
{
	return m_marginPointFraction == useDefaultValue ? s_marginPointFractionDefault : m_marginPointFraction;
}
