#include "EtherdreamWrapper.h"
#include "VirtualLaser.h"

#include "Painter.h"
#include "Config.h"

#ifndef _WIN32
#  include "holodeck/Server.h"
#  include "holodeck/InstructionFactory.h"
#endif

#include "laser_calibration/Calibration.h"

#include "objects/Rectangle.h"

#include <chrono>

using namespace laser;

#ifndef _WIN32
int main(int argc, char *argv[])
{
	config::readCommandLine(argc, argv);
	Painter p(false, false, true);

	if (config::useVirtualLaser)
		p.paintOn(std::make_shared<VirtualLaser>());
	else
		p.calibrate();

#if 0
	holodeck::Server s(p);
#else
	ObjectPtr rect = std::make_shared<Rectangle>(-10000, -10000, 20000, 20000);
	p.add(rect);

	while (true) {
		rect->rotate(radians(5));
		p.updatePoints();
		std::this_thread::sleep_for(std::chrono::milliseconds(50));
	}

	//p.add(holodeck::InstructionFactory::Elevator(Point(-20000, -10000), Point(10000, -20000), Point(0, 0)));
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

