#include "Application.h"

#include <opencv2/highgui/highgui.hpp>

#include "InstructionFactory.h"
#include "InstructionOptions.h"
#include "Server.h"
#include "../laser_calibration/Calibration.h"

#include "../VirtualLaser.h"
#include "../Config.h"

#include "../objects/Circle.h"
#include "../objects/Line.h"
#include "../objects/Rectangle.h"
#include "../objects/RisenGroup.h"
#include "../objects/GenObject.h"

#define USE_ILDA_PARSER 0

#if USE_ILDA_PARSER
#include "../../ilda_parser/FileObject.h"
#endif

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
	addOption("pixel per point",  &Object::s_pixelsPerPointDefault, 2000);
	addOption("margin %",  &Object::s_marginPointFractionDefault, 200);
	addOption("color offset", &EtherdreamWrapper::offset, 30);
	addOption("black line length", &Painter::blackLinePoints, 15);
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
#if USE_ILDA_PARSER
	ObjectPtr obj(new FileObject());
	m_painter.add(obj);
	loop([]{});
#else
	ObjectPtr circ = std::make_shared<Circle>(-10000, -10000, 20000);
	m_painter.add(circ);

	m_painter.add(InstructionFactory::Player(Json::Value(), Point(-10000, -10000)));

#if 1
	int total_p = 0;
	const double width = 3000.0;
	const double left = - 10 * width / 2;
	for (int d = 0; d <= 9; d++) {
		CompositeObjectPtr digit = opts::Digit::get(d);
		std::cerr << digit->pointsToPaint().size() << " points for digit " << d << std::endl;
		total_p += digit->pointsToPaint().size();
		digit->setColor(Color::LIGHTBLUE);
		digit->move(left + width * d, 0);
		m_painter.add(digit);
	}

	std::cerr << "# A total of " << total_p << " visible points painted" << std::endl;
#endif

	loop([&]{
		circ->rotate(radians(5));
	});
#endif
}

void Application::loop(std::function<void()> onLoop, unsigned int wait)
{
	while (true) {
		onLoop();
		cv::waitKey(5);
		m_painter.updatePoints();
		std::this_thread::sleep_for(std::chrono::milliseconds(wait));
	}
}

}}
