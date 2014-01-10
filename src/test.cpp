#include "EtherdreamWrapper.h"

#include "LaserPainter.h"
#include "LaserRectangle.h"
#include "LaserCircle.h"
#include "LaserLine.h"
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
    LaserPainter p(false);
    p.aquireEtherdreamWrapper();
    p.calibrate();

	//p.drawWall(1, Point(INT16_MIN, INT16_MIN), Point(INT16_MAX, INT16_MIN), Point(INT16_MAX, INT16_MAX), Point(INT16_MIN, INT16_MAX));
	//p.drawWall(1, Point(-10000, -5000), Point(256, -5000), Point(0, 0), Point(-20000, 0));

	//LaserObjectPtr l = std::make_shared<LaserLine>(0, 0, 10000, 10000);

	//p.add(l);

//	LaserObjectPtr c = std::make_shared<LaserCircle>(0, 0, 20000/*, M_PI_4, M_PI_2*/);
//	p.add(c);

//	p.drawDoor(2, Point(2000, 2000), Point(10000, 8000));

//	while (true) {

//	}

	std::this_thread::sleep_for(std::chrono::seconds(10));

	return 0;
}
