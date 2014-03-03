#include "EtherdreamWrapper.h"

#include "Painter.h"
#include "Rectangle.h"
#include "Circle.h"
#include "Line.h"
#include "CompositeObject.h"
#include "holodeck/Server.h"
#include "laser_calibration/Calibration.h"

#include <chrono>

#include <thread>
#include <cmath>

#include <vector>
#include <iostream>

#include <opencv/cv.h>

using namespace laser;

int main(void)
{
	Painter p(false);
    p.aquireEtherdreamWrapper();
#ifndef _WIN32
	p.calibrate();
#endif

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
	

	std::cout << "etherdream aquired\n";
	std::cout.flush();

	
	/*
	std::cout << "larger circle added\n";
	std::cout.flush();
	std::this_thread::sleep_for(std::chrono::milliseconds(500));*/


	
	int largerCircleID = p.add(std::make_shared<Circle>(-10000, -10000, 10000, 0, 2.0*M_PI));
	int smallerCircleID = p.add(std::make_shared<Circle>(-5000, -5000, 5000, 0, 2.0*M_PI));
	p.deleteObject(largerCircleID);

	
	std::this_thread::sleep_for(std::chrono::milliseconds(100000));

	
	return 0;
}
