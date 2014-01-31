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

	//p.drawButton(2, Point(0, 0));

//	while (true) {

//	}

	Server s(p);


	s.poll();


	return 0;
}
