#include "Animation.h"
#include "objects/Object.h"

#include <iostream>

namespace laser {

const std::chrono::milliseconds Animation::defaultPeriod(40);

///////////////////////////////////////////////////////////

AnimationPtr Animation::construct(Object *object, const Func &func, bool running, std::chrono::milliseconds period)
{
	return AnimationPtr(new Animation(object, func, running, period));
}

///////////////////////////////////////////////////////////

Animation::Animation(Object *object, const Func &func, bool running, std::chrono::milliseconds period) :
	m_object(object),
	m_running(false),
	m_period(period),
	m_func(func)
{
	if (running)
		start();
}

Animation::~Animation()
{
	stop();
}

void Animation::start()
{
	if (m_running)
		return;

	m_running = true;
	m_thread = std::thread([&](){
		m_expiredLast = std::chrono::system_clock::now();
		while (m_running) {
			m_func(m_object);
			std::cerr << "Wait now" << std::endl;
			wait();
			std::cerr << "Wait finished" << std::endl;
		}
	});
}

void Animation::stop()
{
	std::cerr << "Stop animation" << std::endl;
	m_running = false;
	m_thread.join();
}

void Animation::wait()
{
	m_expiredLast += m_period;
	std::this_thread::sleep_until(m_expiredLast);
}

}
