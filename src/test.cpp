#include "EtherdreamWrapper.h"

#include "LaserPainter.h"
#include "LaserRectangle.h"
#include "LaserCircle.h"
#include "LaserLine.h"
#include "LaserCompositeObject.h"
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
	LaserPainter p(true);
    p.aquireEtherdreamWrapper();
    p.calibrate();

	//p.drawTable(1, Point(-10000, -20000), Point(256, -20000), Point(0, 0), Point(-20000, 0));

	//LaserObjectPtr l = std::make_shared<LaserLine>(0, 0, 10000, 10000);

	//p.add(l);

	//p.drawDoor(2, Point(10000, -20000), Point(10000, -30000));

//	while (true) {

//	}

	LaserServer s(p);

//	p.drawWall(1, Point(-10000, 2000), Point(10000, 2000));
//	p.drawWall(3, Point(0, -10000), Point(0, 10000));

//	p.drawWall(4, Point(-10000, 6000), Point(10000, 6000));

//	p.drawWall(2, Point(30000, 0), Point(-30000, 0));
//	p.drawWall(3, Point(0, -30000), Point(0, 30000));


	LaserObjectPtr wall(new LaserLine(Point(-10000, 2000), Point(10000, 2000)));
	LaserObjectPtr wall2(new LaserLine(Point(0, -10000), Point(0, 10000)));

	LaserCompositeObjectPtr group(new LaserCompositeObject(wall, wall2));
	group->rotate(radians(180));

	p.add(group);

    s.poll();


	return 0;
}
