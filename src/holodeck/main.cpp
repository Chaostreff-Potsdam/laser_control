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
#include "objects/Rectangle.h"
#include "objects/RisenGroup.h"

#include <chrono>
#include <opencv2/highgui/highgui.hpp>

using namespace laser;

#ifndef _WIN32

static void run(Painter & p, std::function<void()> onLoop = []{}, unsigned int wait=50)
{
	while (true) {
		onLoop();
		cv::waitKey(10);
		p.updatePoints();
		std::this_thread::sleep_for(std::chrono::milliseconds(wait));
	}
}


static void displayTests(Painter & p)
{
#if 0
	ObjectPtr rect = std::make_shared<Circle>(-10000, -10000, 20000);
	p.add(rect);

	run(p, [&]{
		rect->rotate(radians(5));
	});
#endif

	CompositeObjectPtr risen = RisenGroup::construct();

	ObjectPtr normal(new Rectangle( 10000, -10000, 10000, 20000));
	risen->add(new Rectangle(-20000, -10000, 10000, 20000));
	RisenGroup::cast(risen)->setZ(0.75);

	normal->setColor(Color::GREEN);
	risen->setColor(Color::GREEN);

	p.add(normal);
	p.add(risen);

	run(p);

	//0.476190476
}

static void showLaserOptions(Painter & p)
{
	void (*laserOptionsChanged)(int, void *) = [](int, void *t){
		static_cast<Painter *>(t)->updateAllObjects();
	};

	auto addOption = [&](const char *name, int * target, int maxValue){
		cv::createTrackbar(name, "Laser options", target, maxValue, laserOptionsChanged, &p);
	};

	cv::namedWindow("Laser options");
	addOption("fps",  &EtherdreamWrapper::framesPerSecond, 60);
	addOption("pixel per second",  &EtherdreamWrapper::pps, 90000);
	addOption("pixel per point",  &Object::s_pixelsPerPointDefault, 1000);
	addOption("margin %",  &Object::s_marginPointFractionDefault, 200);
}

int main(int argc, char *argv[])
{
	config::readCommandLine(argc, argv);
	Painter p(false, false, true);

	showLaserOptions(p);

	if (config::useVirtualLaser)
		p.paintOn(std::make_shared<VirtualLaser>());
	else
		p.calibrate();

	if (config::displayTests) {
		displayTests(p);
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
