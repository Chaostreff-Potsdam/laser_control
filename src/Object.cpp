#include "Object.h"

#include "boost/date_time.hpp"

laser::Object::Object()
:	m_started(boost::date_time::microsec_clock<boost::posix_time::ptime>::universal_time())
{
}

boost::posix_time::ptime laser::Object::started()
{
	return m_started;
}

void laser::Object::setPermanent(bool permanent)
{
	m_permanent = permanent;
}

bool laser::Object::permanent()
{
	return m_permanent;
}
