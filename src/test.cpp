#include "EtherdreamWrapper.h"

#include "LaserPainter.h"
#include "LaserRectangle.h"
#include "LaserCircle.h"
#include "LaserLine.h"

#include <chrono>

#include <thread>
#include <cmath>

#include <vector>

using namespace laser;

int main(void)
{
	LaserPainter p;

	p.aquireEtherdreamWrapper();

	p.drawWall(1, Point(-20000, -20000), Point(20000, -20000), Point(20000, 20000), Point(-20000, 20000));

	std::this_thread::sleep_for(std::chrono::seconds(3));

	p.deleteObject(1);

	std::this_thread::sleep_for(std::chrono::seconds(3));

	p.drawWall(1, Point(-20000, -20000), Point(20000, -20000), Point(20000, 20000), Point(-20000, 20000));

	std::this_thread::sleep_for(std::chrono::seconds(3));

	return 0;
}
