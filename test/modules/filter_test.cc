
#include <catch.h>
#include "../../src/modules/filter.h"

using namespace starflow;

TEST_CASE("Filter", "[modules::Filter]")
{
	modules::Filter<int> f([](int i) { return i > 5; });

	REQUIRE(f(7));
	REQUIRE_FALSE(f(5));
	REQUIRE_FALSE(f(2));
}
