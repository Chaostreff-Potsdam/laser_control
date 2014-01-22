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
	LaserPainter p(false);
    p.aquireEtherdreamWrapper();
    p.calibrate();

	//p.drawTable(1, Point(-10000, -20000), Point(256, -20000), Point(0, 0), Point(-20000, 0));

	//LaserObjectPtr l = std::make_shared<LaserLine>(0, 0, 10000, 10000);

	//p.add(l);

	//p.drawDoor(2, Point(10000, -20000), Point(10000, -30000));

//	while (true) {

//	}

//	LaserObjectPtr circle = std::make_shared<LaserCircle>(Point(0, 0), 10000);

//	p.add(circle);

	LaserServer s(p);

#if 1
//	p.drawWall(1, Point(-10000, 2000), Point(10000, 2000));

//	p.drawWall(2, Point(0, -10000), Point(0, 10000));
//	p.drawWall(3, Point(-10000, 6000), Point(10000, 6000));

#else
	LaserObjectPtr wall(new LaserLine(Point(-10000, 2000), Point(10000, 2000)));
	LaserObjectPtr wall2(new LaserLine(Point(0, -10000), Point(0, 10000)));
	LaserObjectPtr wall3(new LaserLine(Point(-10000, 6000), Point(10000, 6000)));


	LaserCompositeObjectPtr group(new LaserCompositeObject(wall, wall2, wall3));
	//group->rotate(radians(0));
	//group->move(1000, 1000);
	//group->scale(4);

	p.add(group);

	int step = 0;
	const int count = 20;
	while (true) {
		std::this_thread::sleep_for(std::chrono::milliseconds(200));

		step = (step + 1) % count;
		//group->move(0, step * 1000);
		if (step == 0)
			group->resetTransform();
		else {
			group->rotate(radians(360.0 / count));
		}

		p.updatePoints();
	}

#endif

    s.poll();


	return 0;
}
