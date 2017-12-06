
#ifndef STARFLOW_ANALYTICS_ETC_TIMER_H
#define STARFLOW_ANALYTICS_ETC_TIMER_H

#include <functional>
#include <chrono>
#include <stdexcept>
#include <thread>

namespace starflow {
	namespace etc {
		class Timer
		{
			using callback_t = std::function<bool (unsigned long long)>;

		public:
			enum class mode { wait, join, detach };

			Timer()                        = default;
			Timer(callback_t&& cb, std::chrono::milliseconds i, mode mode = mode::wait);
			Timer(const Timer&)            = delete;
			Timer& operator=(const Timer&) = delete;

			Timer(Timer&&)                 = default;
			Timer& operator=(Timer&&)      = default;

			void set_callback(callback_t&& callback);
			void set_interval(std::chrono::milliseconds interval);

			void operator()(mode mode = mode::detach) throw(std::logic_error);

			~Timer() = default;

		private:
			static void _main(callback_t callback, std::chrono::milliseconds interval);
			std::thread _thread;
			callback_t _callback                = nullptr;
			std::chrono::milliseconds _interval = std::chrono::milliseconds(0);
		};
	}
}

#endif
