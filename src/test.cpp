#include "EtherdreamWrapper.h"

#include "etherdream.h"

#include <chrono>

#include <thread>

#include <vector>

int main(int argc, char **argv)
{
	std::vector<struct etherdream_point> points;

	for (int i = -50; i < 50; ++i)
	{
		struct etherdream_point p;

		p.x = i*100;
		p.y = -i*100;
		p.r = (i+50)*1000;
		p.g = (-i+50)*1000;
		p.b = 0;

		points.push_back(p);
	}

	EtherdreamWrapper e;

	e.setPoints(points);

	std::this_thread::sleep_for(std::chrono::milliseconds(5000));
}
