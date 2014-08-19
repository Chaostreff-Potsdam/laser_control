#include "EtherdreamWrapper.h"

#include "Painter.h"
#include "Config.h"

#ifndef _WIN32

#include "Application.h"

int main(int argc, char *argv[])
{
	laser::holodeck::Application app(argc, argv);
	return app.run();
}

#else
using namespace laser;

int main(void)
{
	Painter p(false, false, false);
	p.aquireEtherdreamWrapper();
	std::cout << "aquire Etherdream done\n";


	ObjectPtr line = std::make_shared<Line>(-10000, -10000, 20000, 10000);
	line->setColor(Color(200,0,0));
	p.add(line);
	std::this_thread::sleep_for(std::chrono::milliseconds(100000));
	return 0;
}
#endif
