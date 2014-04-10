#include "EtherdreamWrapper.h"

#include "Painter.h"
#include "Rectangle.h"
#include "Circle.h"
#include "Line.h"
#include "Wave.h"
#include "CompositeObject.h"
#include "Config.h"

#ifndef _WIN32
#  include "holodeck/Server.h"
#  include "holodeck/InstructionFactory.h"
#endif

#include "laser_calibration/Calibration.h"

#include <chrono>

#include <thread>
#include <cmath>

#include <vector>
#include <iostream>

#include <opencv/cv.h>

using namespace laser;

#ifndef _WIN32
int main(int argc, char *argv[])
{
	config::readCommandLine(argc, argv);

	Painter p(false, false, true);
    p.aquireEtherdreamWrapper();

	p.calibrate();

	// p.drawTable(1, Point(-10000, -20000), Point(256, -20000), Point(0, 0), Point(-20000, 0));

	/*CompositeObjectPtr group = CompositeObject::construct();
	ObjectPtr outerRect = std::make_shared<Rectangle>(-10000, -10000, 20000, 20000);
	group->add(outerRect);
	group->add(new Rectangle(-5000, -5000, 10000, 10000));
	p.add(group);

	while (true) {
		outerRect->rotate(radians(6.0));
		group->rotate(radians(-3.0));
		p.updatePoints();
		std::this_thread::sleep_for(std::chrono::milliseconds(30));
	}

	holodeck::Server s(p);


	s.poll();
	*/
	

	
	/*
	std::cout << "larger circle added\n";
	std::cout.flush();
	std::this_thread::sleep_for(std::chrono::milliseconds(500));*/


//	p.add(holodeck::InstructionFactory::Beam(Point(10000, 10000), Point(10000, 5000)));
//	p.add(holodeck::InstructionFactory::Button(Point(0, 0)));
//	p.add(holodeck::InstructionFactory::Wall(Point(-20000, -20000), Point(-5000, -20000)));
//	p.add(holodeck::InstructionFactory::Wall(Point(30000, -5000), Point(30000, 10000)));
	p.add(holodeck::InstructionFactory::Elevator(Point(-20000, -10000), Point(10000, -20000), Point(0, 0)));
	//p.add(50, ObjectPtr(new Line(-20000,-10000, 10000, -10000)));
	

	std::this_thread::sleep_for(std::chrono::milliseconds(100000));
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

