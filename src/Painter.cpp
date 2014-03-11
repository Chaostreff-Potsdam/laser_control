#include "Painter.h"

#include "laser_calibration/Calibration.h"
#include "Transform.h"

#include <utility>
#include <memory>
#include <mutex>
#include <algorithm>
#include <vector>

#include <boost/date_time.hpp>

laser::Painter::Painter(bool expireObjects, bool cropObjects)
:	m_smallestFreeId(0),
	m_expireObjects(expireObjects),
	m_cropObjects(cropObjects),
	m_running(true)
{
	if (expireObjects)
	{
		m_updateLoop = std::thread(&Painter::updateLoop, this);
	}
}

laser::Painter::~Painter()
{
	m_running = false;
	if(m_expireObjects)
		m_updateLoop.join();
}

void laser::Painter::aquireEtherdreamWrapper()
{
    m_canvas = std::make_shared<EtherdreamWrapper>();
}

void laser::Painter::calibrate()
{
	if (!m_canvas) aquireEtherdreamWrapper();

    Calibration calibration(m_canvas);
    calibration.start();

    m_calibration = calibration.homography();
}

void laser::Painter::paintOn(const std::shared_ptr<EtherdreamWrapper> &e)
{
	m_canvas = e;
}

void laser::Painter::paint(const ObjectPtrMap & objects)
{
	m_objects = objects;
	updatePoints();
}

int laser::Painter::add(const ObjectPtr & object, bool update)
{	
	while (m_objects.find(m_smallestFreeId++) != m_objects.end())
		continue;
	
	m_objects[m_smallestFreeId] = object;
	if (update)
		updatePoints();
	return m_smallestFreeId;
}

int laser::Painter::add(int id, const ObjectPtr &object, bool update)
{
	m_objects[id] = object;
	if (update)
		updatePoints();
	return id;
}

void laser::Painter::deleteObject(int id, bool update)
{
	m_objects.erase(id);
	if (update)
		updatePoints();
}

void laser::Painter::deleteAll()
{
	m_objects.clear();
	updatePoints();
}

laser::ObjectPtr laser::Painter::getObject(int id)
{
	return m_objects[id];
}

void laser::Painter::removeExpiredObjects()
{
	boost::posix_time::ptime now(boost::date_time::microsec_clock<boost::posix_time::ptime>::universal_time());

	ObjectPtrMap::iterator iter = m_objects.begin();
	ObjectPtrMap::iterator end = m_objects.end();
	while(iter != end)
	{

		if ((iter->second)->permanent())
		{
			++iter;
			continue;
		}

		boost::posix_time::time_duration lifetime = now - (iter->second)->started();

		if (lifetime > boost::posix_time::seconds(10))
		{
				m_objects.erase(iter++);
		}
		else
		{
				++iter;
		}
	}
}

void laser::Painter::updatePoints()
{
	if (m_objects.empty())
		return;

	EtherdreamPoints ps;
	std::lock_guard<std::mutex> lock(m_updateMutex);

	if (m_expireObjects) {
		removeExpiredObjects();
	}

	for (auto objPair: m_objects) {
		appendToVector(ps, objPair.second->pointsToPaint());
	}

	if(m_cropObjects) {
		EtherdreamPoints transformedPoints;
		if (!m_calibration.empty()) 
			transformedPoints = Transform::applyReturning(ps, cv::perspectiveTransform, m_calibration);
		else
			transformedPoints = ps;
		m_canvas->setPoints(transformedPoints);
	} else {
		if (!m_calibration.empty()) 
			Transform::applyInPlace(ps, cv::perspectiveTransform, m_calibration);
		m_canvas->setPoints(ps);
	}
	m_canvas->writePoints();
}

void laser::Painter::setCalibration(cv::Mat homography) {
	m_calibration = homography;
}

void laser::Painter::updateLoop()
{
	std::this_thread::sleep_for(std::chrono::seconds(5));
	while (m_running) {
		std::this_thread::sleep_for(std::chrono::seconds(1));
		updatePoints();
	}
}
