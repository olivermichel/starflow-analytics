
#include <catch.h>
#include "../../src/types/packet.h"

using namespace starflow;

TEST_CASE("Packet","[types::Packet]")
{
	types::Packet p1(123, 23);
	types::Packet p2(495, 3439);
	types::Packet p3(123, 23);

	p1.features = types::Features { };
	p2.features = types::Features { };
	p3.features = types::Features { };

	SECTION("Packet(ts_in, len)")
	{
		CHECK(p1.ts_in.count() == 123);
		CHECK(p1.len == 23);
		CHECK(p2.ts_in.count() == 495);
		CHECK(p2.len == 3439);
	}

	SECTION("operator==")
	{
		CHECK(p1 == p3);
		CHECK(p2 != p1);
		CHECK(p2 != p3);
	}

	SECTION("Packet(proto), to_proto()")
	{
		proto::packet proto_packet = p1.to_proto();
		types::Packet packet_restore(proto_packet);
		CHECK(p1 == packet_restore);
	}
}