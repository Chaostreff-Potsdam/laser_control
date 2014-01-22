#include "LaserObject.h"

#include "boost/date_time.hpp"

laser::LaserObject::LaserObject()
:	m_started(boost::date_time::microsec_clock<boost::posix_time::ptime>::universal_time())
{
}

boost::posix_time::ptime laser::LaserObject::started()
{
	return m_started;
}

void laser::LaserObject::setPermanent(bool permanent)
{
	m_permanent = permanent;
}

bool laser::LaserObject::permanent()
{
	return m_permanent;
}
