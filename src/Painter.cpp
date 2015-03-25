#include "Painter.h"
#include "Config.h"

#include "laser_calibration/Calibration.h"
#include "laser_calibration/ManualCornerCalibration.h"
#include "laser_calibration/DistortionCalibration.h"
#include "laser_calibration/PointLifter.h"
#include "Transform.h"
#include "ObjectSorting.h"

#include <utility>
#include <algorithm>

#include <boost/date_time.hpp>

laser::Painter::Painter(bool expireObjects, bool cropObjects, bool runUpdateLoop)
:	m_smallestFreeId(0),
	m_expireObjects(expireObjects),
	m_running(true),
	m_cropObjects(cropObjects)
{
	m_calibration = (cv::Mat_<double>(3,3) << 1, 0, 0, 0, 1, 0, 0, 0, 1);

	if (runUpdateLoop || expireObjects)
	{
		m_updateLoop = std::thread(&Painter::updateLoop, this);
	}
}

laser::Painter::~Painter()
{
	m_running = false;
	if (m_expireObjects)
		m_updateLoop.join();
}

void laser::Painter::aquireEtherdreamWrapper()
{
	paintOn(std::make_shared<EtherdreamWrapper>());
}

void laser::Painter::calibrate()
{
	auto transparentCalib = [&](PlanarCalibration *calibration){
		calibration->start();
		m_calibration = calibration->homography();
		PointLifter::s_sharedInstance = PointLifter(*calibration);
	};

	if (config::oldCalib) {
		Calibration calibration(canvas());
		transparentCalib(&calibration);
	} else {
		ManualCornerCalibration calibration(canvas());
		transparentCalib(&calibration);
	}

	// TODO: Apply to painter
	DistorionCalibration dcalib(canvas(), m_calibration);
	dcalib.start();
	m_distortion = dcalib.distortion();
}

laser::CanvasPtr laser::Painter::canvas()
{
	if (!m_canvas) aquireEtherdreamWrapper();

	return m_canvas;
}

void laser::Painter::paintOn(const CanvasPtr &e)
{
	m_canvas = e;
}

void laser::Painter::paint(const ObjectPtrMap & objects)
{
	{
		std::lock_guard<std::mutex> lock(m_objectsMutex);
		m_objects = objects;
	}
	updatePoints();
}

int laser::Painter::add(const ObjectPtr & object, bool update)
{	
	int result = -1;
	{
		std::lock_guard<std::mutex> lock(m_objectsMutex);
		while (m_objects.find(m_smallestFreeId++) != m_objects.end())
			continue;
	
		m_objects[m_smallestFreeId] = object;
		result = m_smallestFreeId;
	}

	if (update)
		updatePoints();
	return result;
}

int laser::Painter::add(int id, const ObjectPtr &object, bool update)
{
	{
		std::lock_guard<std::mutex> lock(m_objectsMutex);
		m_objects[id] = object;
	}
	if (update)
		updatePoints();
	return id;
}

void laser::Painter::deleteObject(int id, bool update)
{
	{
		std::lock_guard<std::mutex> lock(m_objectsMutex);
		m_objects.erase(id);
	}
	if (update)
		updatePoints();
}

void laser::Painter::deleteAll(bool update)
{
	{
		std::lock_guard<std::mutex> lock(m_objectsMutex);
		m_objects.clear();
	}
	if (update)
		updatePoints();
}

laser::ObjectPtr laser::Painter::getObject(int id)
{
	std::lock_guard<std::mutex> lock(m_objectsMutex);

	return m_objects[id];
}

laser::EtherdreamPoints laser::Painter::invisibleLine(const etherdream_point &start, const laser::ObjectPtr & end) const
{
	if (end->empty())
		return EtherdreamPoints();

	return invisibleLine(start, end->pointsToPaint().front());
}

laser::EtherdreamPoints laser::Painter::invisibleLine(const etherdream_point &start, const etherdream_point &end) const
{
	static const auto make_point = [](double x, double y) {
		etherdream_point p;
		memset(&p, 0, sizeof(p));
		p.x = clamp(x, INT16_MIN, INT16_MAX);
		p.y = clamp(y, INT16_MIN, INT16_MAX);
		return p;
	};

	const int numPoints = 5;

	const double dx = ((double) end.x - start.x) / numPoints;
	const double dy = ((double) end.y - start.y) / numPoints;

	EtherdreamPoints ps;
	for (int i = 0; i < numPoints; i++) {
		ps.push_back(make_point(dx * i + start.x, dy * i + start.y));
	}
	return ps;
}

void laser::Painter::removeExpiredObjects()
{
	boost::posix_time::ptime now(boost::date_time::microsec_clock<boost::posix_time::ptime>::universal_time());

	ObjectPtrMap::iterator iter = m_objects.begin();
	ObjectPtrMap::iterator end = m_objects.end();
	while (iter != end)
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
	EtherdreamPoints ps;

	{
		std::lock_guard<std::mutex> lock(m_objectsMutex);


		if (m_expireObjects) {
			removeExpiredObjects();
		}

		std::vector<ObjectPtr> objects;

		for (auto &objPair : m_objects)
		{
			objects.push_back(objPair.second);
		}

		if (!objects.empty()) {
			sortObjects(objects);

			for (auto & obj: objects) {
				// obj->tick();
				if (!ps.empty())
					appendToVector(ps, invisibleLine(ps.back(), obj));

				appendToVector(ps, obj->pointsToPaint());
			}

			if (!ps.empty()) {
				appendToVector(ps, invisibleLine(ps.back(), ps.front()));
			}
		}
	}

	auto undistortAndSet = [&](EtherdreamPoints & ps){
		Transform::undistortInPlace(ps, m_distortion);
		canvas()->setPoints(ps);
	};

	if (m_cropObjects) {
		EtherdreamPoints transformedPoints = Transform::applyReturning(ps, cv::perspectiveTransform, m_calibration);
		undistortAndSet(transformedPoints);
	} else {
		Transform::applyInPlace(ps, cv::perspectiveTransform, m_calibration);
		undistortAndSet(ps);
	}
	canvas()->writePoints();
}

void laser::Painter::setCalibration(cv::Mat homography)
{
	m_calibration = homography;
}

void laser::Painter::updateAllObjects()
{
	std::lock_guard<std::mutex> lock(m_objectsMutex);
	for (auto & objPair: m_objects) {
		objPair.second->update();
	}
}

void laser::Painter::updateLoop()
{
	std::this_thread::sleep_for(std::chrono::seconds(5));
	while (m_running) {
		std::this_thread::sleep_for(std::chrono::milliseconds(10));
		updatePoints();
	}
}
