#include "EtherdreamWrapper.h"

#include "etherdream.h"

#ifdef HAVE_BOOST_CHRONO
#include <boost/chrono.hpp>
#endif

#ifndef HAVE_BOOST_CHRONO
#include <boost/date_time/posix_time/posix_time_types.hpp>
#endif

#include <boost/thread/thread.hpp>

#include <vector>

int main(int argc, char **argv)
{
	std::vector<struct etherdream_point> points;

	for (int i = -50; i < 50; ++i)
	{
		struct etherdream_point p;

		p.x = i*100;
		p.y = -i*100;
		p.r = (i+50)*1000;
		p.g = (-i+50)*1000;
		p.b = 0;

		points.push_back(p);
	}

	EtherdreamWrapper e;

	boost::thread loop(boost::bind(&EtherdreamWrapper::connect, &e));

	e.setPoints(points);

#ifdef HAVE_BOOST_CHRONO
	boost::this_thread::sleep_for(boost::chrono::milliseconds(5000));
#endif
#ifndef HAVE_BOOST_CHRONO
	boost::this_thread::sleep(boost::posix_time::milliseconds(5000));
#endif

	loop.interrupt();
	loop.join();
}
