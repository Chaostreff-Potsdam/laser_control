#include "Animation.h"
#include "objects/Object.h"

#include <iostream>

namespace laser {

const Animation::msecs Animation::defaultPeriod(40);
const Animation::msecs Animation::noDelay(0);

const Animation::Func Animation::nop;

///////////////////////////////////////////////////////////

AnimationPtr Animation::construct(Object *object, const Func &func, bool running, Animation::msecs period, msecs initialDelay)
{
	return AnimationPtr(new Animation(object, func, running, period, initialDelay));
}

///////////////////////////////////////////////////////////

Animation::Animation(Object *object, const Func &func, bool running, Animation::msecs period, msecs initialDelay) :
	m_object(object),
	m_running(false),
	m_period(period),
	m_initialDelay(initialDelay),
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
		if (m_initialDelay != noDelay)
			std::this_thread::sleep_for(m_initialDelay);

		m_expiredLast = std::chrono::system_clock::now();
		while (m_running) {
			tick();
			wait();
		}
	});
}

void Animation::tick()
{
	m_func(m_object);
}

void Animation::stop()
{
	if (!m_running)
		return;

	m_running = false;
	m_thread.join();
}

void Animation::wait()
{
	m_expiredLast += m_period;
	std::this_thread::sleep_until(m_expiredLast);
}

}
