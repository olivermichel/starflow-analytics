
#include <catch.h>

#include "../../src/etc/timer.h"

#include <iostream>

using namespace starflow;

TEST_CASE("Timer", "[etc::Timer]")
{
	SECTION("Timer(callback, interval, mode): initializes callback and interval")
	{
		auto callback = [](unsigned long long i) -> bool { return true; };
		etc::Timer t(callback, std::chrono::milliseconds(50));
		CHECK_NOTHROW(t());
	}

	SECTION("Timer(callback, interval, mode): starts the timer if mode != wait")
	{
		unsigned long long counter = 0;
		auto callback = [&counter](unsigned long long i){ counter = i; return i < 5; };
		CHECK_NOTHROW(etc::Timer(callback, std::chrono::milliseconds(50), etc::Timer::mode::join));
		CHECK(counter == 5);
	}

	SECTION("operator(): throws an exception if no callback is set")
	{
		etc::Timer t;
		t.set_interval(std::chrono::seconds(1));
		CHECK_THROWS_AS(t(), std::logic_error);
	}

	SECTION("operator(): throws an exception if interval is set to 0")
	{
		etc::Timer t;
		t.set_callback([](unsigned long long i) -> bool { return true; });
		CHECK_THROWS_AS(t(), std::logic_error);
	}

	SECTION("operator(): runs until callback returns false")
	{
		etc::Timer t;
		unsigned long long counter = 0;
		t.set_callback([&counter](unsigned long long i){ counter = i; return i < 5; });
		t.set_interval(std::chrono::milliseconds(10));
		CHECK_NOTHROW(t(etc::Timer::mode::join));
		CHECK(counter == 5);
	}
}
