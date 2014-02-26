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
	Painter p(true);
    p.aquireEtherdreamWrapper();
#ifndef _WIN32
	p.calibrate();
#endif

	// p.drawTable(1, Point(-10000, -20000), Point(256, -20000), Point(0, 0), Point(-20000, 0));


	CompositeObjectPtr group = CompositeObject::construct();
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

	Server s(p);


	s.poll();


	return 0;
}
