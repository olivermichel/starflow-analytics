
#include <catch.h>
#include "../../src/types/packet.h"

using namespace starflow;

TEST_CASE("Packet","[types::Packet]")
{
	types::Packet p1(123, 124, 23);
	types::Packet p2(495, 496, 3439);
	types::Packet p3(123, 124, 23);

	p1.qid = 1;
	p1.qlen = 21;
	p1.features = types::Features { };

	p2.qid = 2;
	p2.qlen = 22;
	p2.features = types::Features { };

	p3.qid = 1;
	p3.qlen = 21;
	p3.features = types::Features { };

	SECTION("Packet(ts_in, ts_out, len)")
	{
		CHECK(p1.ts_in_us == 123);
		CHECK(p1.ts_out_us == 124);
		CHECK(p1.len == 23);
		CHECK(p2.ts_in_us == 495);
		CHECK(p2.ts_out_us == 496);
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