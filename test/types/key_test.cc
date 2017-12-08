
#include <catch.h>
#include "../../src/types/key.h"

using namespace starflow;

TEST_CASE("Key","[Key]")
{
	SECTION("Key()")
	{
		types::Key k(6, 238923, 3439453, 23222, 80);
		CHECK(k.ip_proto == 6);
		CHECK(k.ip_src   == 238923);
		CHECK(k.ip_dst   == 3439453);
		CHECK(k.th_sport == 23222);
		CHECK(k.th_dport == 80);
	}

	SECTION("operator==()")
	{
		types::Key k1(6, 238923, 3439453, 23222, 80);
		types::Key k2(6, 238923, 3439453, 23222, 80);
		CHECK(k1 == k2);
	}

	SECTION("operator<()")
	{
		types::Key k1(6, 238923, 3439453, 23222, 80);
		types::Key k2(6, 238923, 3439453, 23222, 81);
		CHECK(k1 < k2);
	}

	SECTION("Key(proto), to_proto()")
	{
		types::Key k1(6, 238923, 3439453, 23222, 80);
		auto k_proto = k1.to_proto();
		types::Key k2(k_proto);
		CHECK(k1 == k2);
	}
}
