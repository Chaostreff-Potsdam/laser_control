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
#include "objects/Line.h"

#include <chrono>

using namespace laser;

#ifndef _WIN32

void run(Painter & p, std::function<void()> onLoop = []{}, unsigned int wait=50)
{
	while (true) {
		onLoop();
		p.updatePoints();
		std::this_thread::sleep_for(std::chrono::milliseconds(wait));
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

		run(p, [&]{
			rect->rotate(radians(5));
		});
	} else {
		holodeck::Server s(p);
		run(p);
	}

	return 0;
}
#else

int main(void)
{
	Painter p(false, false, false);
	p.aquireEtherdreamWrapper();
	std::cout << "aquire Etherdream done\n";


	ObjectPtr line = std::make_shared<Line>(-10000, -10000, 20000, 10000);
	line->setColor(Color(200,0,0));
	p.add(line);
	std::this_thread::sleep_for(std::chrono::milliseconds(100000));
	return 0;
}
#endif
