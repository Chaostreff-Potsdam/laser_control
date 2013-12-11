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
	LaserPainter p;

	p.aquireEtherdreamWrapper();

	LaserObjectPtr c = std::make_shared<LaserCircle>(0, 0, 20000/*, M_PI_4, M_PI_2*/);
	p.add(c);

	while (true) {

	}

	return 0;
}
