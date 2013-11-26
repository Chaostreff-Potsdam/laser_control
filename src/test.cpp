#include "EtherdreamWrapper.h"

#include "LaserPainter.h"
#include "LaserRectangle.h"
#include "LaserCircle.h"
#include "LaserLine.h"

#include <chrono>

#include <thread>

#include <vector>

int main(void)
{
	LaserPainter p;

	EtherdreamWrapper e;

	p.paintOn(&e);

	LaserObjectPtr c(new LaserLine(0, -2500, 10000, 5000));
	//LaserObjectPtr d(new LaserCircle(2500, 2500, 10000));
	LaserObjectPtr f(new LaserLine(-10000, 10000, 10000, -10000));
	p.add(c);
	//p.add(d);
	//p.add(f);

	std::this_thread::sleep_for(std::chrono::seconds(20));

	return 0;
}
