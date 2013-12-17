#include "EtherdreamWrapper.h"

#include "LaserPainter.h"
#include "LaserRectangle.h"
#include "LaserCircle.h"
#include "LaserLine.h"
#include "LaserServer.h"

#include <chrono>

#include <thread>
#include <cmath>

#include <vector>
#include <iostream>

using namespace laser;

int main(void)
{
	LaserPainter p(true);

	p.aquireEtherdreamWrapper();

	p.drawWall(1, Point(INT16_MIN, INT16_MIN), Point(INT16_MAX, INT16_MIN), Point(INT16_MAX, INT16_MAX), Point(INT16_MIN, INT16_MAX));
	//p.drawWall(1, Point(-5000, -5000), Point(0, -5000), Point(0, 0), Point(-5000, 0));

//	LaserObjectPtr c = std::make_shared<LaserCircle>(0, 0, 20000/*, M_PI_4, M_PI_2*/);
//	p.add(c);

//	p.drawDoor(2, Point(2000, 2000), Point(10000, 8000));

//	while (true) {

//	}

	LaserServer s(p);

	s.poll();

	return 0;
}
