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

	LaserServer s(p);

	s.poll();

	std::cout << "juhu" << std::endl;

	return 0;
}
