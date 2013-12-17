#include "LaserObject.h"

#include "boost/date_time.hpp"

using namespace laser;

LaserObject::LaserObject() : m_started(boost::date_time::microsec_clock<boost::posix_time::ptime>::universal_time())
{
}

boost::posix_time::ptime LaserObject::started()
{
	return m_started;
}
