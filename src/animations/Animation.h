#pragma once

#include "DllExport.h"
#include <functional>
#include <thread>
#include <chrono>
#include <memory>

namespace laser {

	class Object;

	class Animation;
	typedef std::shared_ptr<Animation> AnimationPtr;

	class EXPORT_LASER_CONTROL Animation
	{
	public:
		typedef std::function<void(Object *object)> Func;
		typedef std::chrono::milliseconds msecs;

		static const msecs defaultPeriod;
		static const msecs noDelay;
		static AnimationPtr construct(Object *object, const Func & func, bool running = true, msecs period = defaultPeriod, msecs initialDelay = noDelay);

		virtual ~Animation();

		msecs period() const
		{ return m_period; }
		void setPeriod(const msecs period);

		void start();
		void stop();

		bool isRunning()
		{ return m_running; }

		Object *object() const
		{ return m_object; }

	protected:
		static const Func nop;

		Animation(Object *object, const Func & func, bool running, msecs period, msecs initialDelay);

		virtual void tick();
		void wait();

	private:
		Object *m_object;
		bool m_running;
		msecs m_period;
		msecs m_initialDelay;
		std::chrono::time_point<std::chrono::system_clock> m_expiredLast;
		Func m_func;
		std::thread m_thread;
	};

}
