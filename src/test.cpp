#include "EtherdreamWrapper.h"

#include "LaserPainter.h"
#include "LaserRectangle.h"
#include "LaserCircle.h"
#include "LaserLine.h"
#include "LaserServer.h"
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

	//p.drawWall(1, Point(-10000, -5000), Point(256, -5000), Point(0, 0), Point(-20000, 0));

	//LaserObjectPtr l = std::make_shared<LaserLine>(0, 0, 10000, 10000);

	//p.add(l);

	LaserObjectPtr c = std::make_shared<LaserCircle>(0, 0, 20000, M_PI + M_PI_2, 2*M_PI);
	p.add(c);

//	p.drawDoor(2, Point(2000, 2000), Point(10000, 8000));

//	while (true) {

//	}

	LaserServer s(p);

	//p.drawWall(1, Point(-10000, 2000), Point(10000, 2000));
//	p.drawWall(3, Point(0, -10000), Point(0, 10000));

//	p.drawWall(4, Point(-10000, 6000), Point(10000, 6000));

    s.poll();

//    p.drawWall(2, Point(30000, 0), Point(-30000, 0));
//    p.drawWall(2, Point(0, -30000), Point(0, 30000));

	return 0;
}
