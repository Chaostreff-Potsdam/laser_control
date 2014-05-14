#include "EtherdreamWrapper.h"
#include "VirtualLaser.h"

#include "Painter.h"
#include "Config.h"

#ifndef _WIN32
#  include "holodeck/Server.h"
#  include "holodeck/InstructionFactory.h"
#endif

#include "laser_calibration/Calibration.h"

#include "objects/Circle.h"

#include <chrono>

using namespace laser;

#ifndef _WIN32

void runServer(Painter & p)
{
	holodeck::Server s(p);

	std::thread pollThread([&](){
		s.poll();
	});

	while (true) {
		p.updatePoints();
		std::this_thread::sleep_for(std::chrono::milliseconds(50));
	}
}

int main(int argc, char *argv[])
{
	config::readCommandLine(argc, argv);
	Painter p(false, false, true);

	if (config::useVirtualLaser)
		p.paintOn(std::make_shared<VirtualLaser>());
	else
		p.calibrate();

	if (config::displayTests) {
		ObjectPtr rect = std::make_shared<Circle>(-10000, -10000, 20000);
		p.add(rect);
	} else {
		runServer(p);
	}

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

