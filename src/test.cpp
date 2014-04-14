#include "EtherdreamWrapper.h"

#include "Painter.h"
#include "Config.h"

#ifndef _WIN32
#  include "holodeck/Server.h"
#  include "holodeck/InstructionFactory.h"
#endif

#include "laser_calibration/Calibration.h"

#include <chrono>

using namespace laser;

#ifndef _WIN32
int main(int argc, char *argv[])
{
	config::readCommandLine(argc, argv);
	Painter p(false, false, true);
	p.calibrate();
#if 0
	holodeck::Server s(p);
#else
	p.add(holodeck::InstructionFactory::Elevator(Point(-20000, -10000), Point(10000, -20000), Point(0, 0)));	
	std::this_thread::sleep_for(std::chrono::milliseconds(100000));
#endif
	return 0;
}
#else
int main(void)
{
	Painter p(false, false, true);
	p.aquireEtherdreamWrapper();
	std::this_thread::sleep_for(std::chrono::milliseconds(100000));
	return 0;
}
#endif

