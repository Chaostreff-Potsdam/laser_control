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

	LaserObjectPtr c(new LaserRectangle(-5000, -2500, 10000, 5000));
	LaserObjectPtr d(new LaserCircle(0, 0, 10000));
	LaserObjectPtr f(new LaserLine(-10000, 10000, 10000, -10000));
	p.add(c);
	p.add(d);
	//p.add(f);

	while (true)
	{
		c->rotate(M_PI/100);
		p.updatePoints();
		std::this_thread::sleep_for(std::chrono::milliseconds(5));
	}

	return 0;
}
