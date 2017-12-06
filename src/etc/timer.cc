
#include "timer.h"


starflow::etc::Timer::Timer::Timer(callback_t&& cb, std::chrono::milliseconds i, mode mode)
	: _callback(std::move(cb)), _interval(i)
{
	if (mode != mode::wait)
		operator()(mode);
}

void starflow::etc::Timer::set_callback(callback_t&& callback)
{
	_callback = std::move(callback);
}

void starflow::etc::Timer::set_interval(std::chrono::milliseconds interval)
{
	_interval = interval;
}

void starflow::etc::Timer::operator()(mode mode) throw(std::logic_error)
{
	if (_callback == nullptr)
		throw std::logic_error("Timer: no callback set");

	if (_interval == std::chrono::milliseconds(0))
		throw std::logic_error("Timer: no interval is set or interval is 0");

	if (mode == mode::wait)
		throw std::logic_error("Timer: mode wait is invalid for operator()");

	_thread = std::thread(_main, _callback, _interval);

	if (mode == mode::join)
		_thread.join();
	else if (mode == mode::detach)
		_thread.detach();
}

void starflow::etc::Timer::_main(callback_t callback, std::chrono::milliseconds interval)
{
	unsigned long long _counter = 0;
	while (callback(_counter++))
		std::this_thread::sleep_for(interval);
}
