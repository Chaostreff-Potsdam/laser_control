#include "Application.h"

#include <opencv2/highgui/highgui.hpp>

#include "InstructionFactory.h"
#include "Server.h"
#include "../laser_calibration/Calibration.h"

#include "../VirtualLaser.h"
#include "../Config.h"

#include "../objects/Circle.h"
#include "../objects/Line.h"
#include "../objects/Rectangle.h"
#include "../objects/RisenGroup.h"

namespace laser { namespace holodeck {

bool Application::forceReadOptions(int argc, char *argv[])
{
	config::readCommandLine(argc, argv);
	return true;
}

Application::Application(int argc, char *argv[]) :
	m_dummy(forceReadOptions(argc, argv)),
	m_painter(false, false, true)
{
	showLaserOptions();
	startCanvas();
}

int Application::run()
{
	if (config::displayTests) {
		displayTests();
	} else {
		holodeck::Server s(m_painter);
		loop();
	}
	return 0;
}

void Application::showLaserOptions()
{
	void (*laserOptionsChanged)(int, void *) = [](int, void *t){
		static_cast<Painter *>(t)->updateAllObjects();
	};

	auto addOption = [&](const char *name, int * target, int maxValue){
		cv::createTrackbar(name, "Laser options", target, maxValue, laserOptionsChanged, &m_painter);
	};

	cv::namedWindow("Laser options");
	addOption("fps",  &EtherdreamWrapper::framesPerSecond, 60);
	addOption("pixel per second",  &EtherdreamWrapper::pps, 90000);
	addOption("pixel per point",  &Object::s_pixelsPerPointDefault, 1000);
	addOption("margin %",  &Object::s_marginPointFractionDefault, 200);
}

void Application::startCanvas()
{
	if (config::useVirtualLaser)
		m_painter.paintOn(std::make_shared<VirtualLaser>());
	else
		m_painter.calibrate();
}

void Application::displayTests()
{
	ObjectPtr rect = std::make_shared<Circle>(-10000, -10000, 20000);
	//m_painter.add(rect);
	m_painter.add(holodeck::InstructionFactory::Corpse(Json::Value(), Point(0, 0), Point(0, 10000)));

	loop([&]{
		rect->rotate(radians(5));
	});
}

void Application::loop(std::function<void()> onLoop, unsigned int wait)
{
	while (true) {
		onLoop();
		m_painter.updatePoints();
		std::this_thread::sleep_for(std::chrono::milliseconds(wait));
	}
}

}}
