#pragma once

#include "../Painter.h"

namespace laser { namespace holodeck {

class Application
{
	static bool forceReadOptions(int argc, char *argv[]);

public:
	Application(int argc, char *argv[]);
	int run();

protected:
	void showLaserOptions();
	void startCanvas();
	void displayTests();
	void loop(std::function<void()> onLoop = []{}, unsigned int wait=50);

private:
	bool m_dummy;
	Painter m_painter;

};

}}
