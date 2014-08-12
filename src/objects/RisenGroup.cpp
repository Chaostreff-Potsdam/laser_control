#include "RisenGroup.h"
#include "../laser_calibration/PointLifter.h"

namespace laser {

RisenGroup::RisenGroup() :
	CompositeObject(),
	m_z(0)
{
	;;
}

void RisenGroup::setZ(const double newZ)
{
	m_z = newZ;
	nowDirty();
}

EtherdreamPoints RisenGroup::points() const
{
	EtherdreamPoints childPoints = CompositeObject::points();
	PointLifter::sharedInstance().liftPainterPoints(childPoints, m_z);

	return childPoints;
}

EtherdreamPoints RisenGroup::startPoints() const
{
	EtherdreamPoints childPoints = CompositeObject::startPoints();
	PointLifter::sharedInstance().liftPainterPoints(childPoints, m_z);

	return childPoints;
}

EtherdreamPoints RisenGroup::endPoints() const
{
	EtherdreamPoints childPoints = CompositeObject::endPoints();
	PointLifter::sharedInstance().liftPainterPoints(childPoints, m_z);

	return childPoints;
}

}
