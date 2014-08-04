#pragma once

#include <functional>
#include <thread>
#include <chrono>
#include <memory>

namespace laser {

	class Object;

	class Animation;
	typedef std::shared_ptr<Animation> AnimationPtr;

	class Animation
	{
	public:
		typedef std::function<void(Object *object)> Func;

		static const std::chrono::milliseconds defaultPeriod;
		static AnimationPtr construct(Object *object, const Func & func, bool running = true, std::chrono::milliseconds period = defaultPeriod);

		virtual ~Animation();

		std::chrono::milliseconds period() const
		{ return m_period; }
		void setPeriod(const std::chrono::milliseconds period);

		void start();
		void stop();

		bool isRunning()
		{ return m_running; }

	protected:
		Animation(Object *object, const Func & func, bool running, std::chrono::milliseconds period);

		void wait();

	private:
		Object *m_object;
		bool m_running;
		std::chrono::milliseconds m_period;
		std::chrono::time_point<std::chrono::system_clock> m_expiredLast;
		Func m_func;
		std::thread m_thread;
	};

}
