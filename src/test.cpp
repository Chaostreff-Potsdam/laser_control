#include "EtherdreamWrapper.h"

#include "Painter.h"
#include "Rectangle.h"
#include "Circle.h"
#include "Line.h"
#include "CompositeObject.h"
#include "Server.h"
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

	//p.drawTable(1, Point(-10000, -20000), Point(256, -20000), Point(0, 0), Point(-20000, 0));

	ObjectPtr l = std::make_shared<Line>(0, 0, 10000, 10000);

	p.add(l);

	//p.drawDoor(2, Point(10000, -20000), Point(10000, -30000));

//	while (true) {

//	}

//	LaserObjectPtr circle = std::make_shared<LaserCircle>(Point(0, 0), 10000);

//	p.add(circle);

	Server s(p);


    s.poll();

	CompositeObjectPtr group(new CompositeObject());

	group->add(std::make_shared<Line>(Point(-10000, 2000), Point(10000, 2000)));
	group->add(std::make_shared<Line>(Point(0, -10000), Point(0, 10000)));
	group->add(std::make_shared<Line>(Point(-10000, 6000), Point(10000, 6000)));

	group->rotate(radians(90));



	return 0;
}
