
#include <catch.h>
#include "../../src/types/clfr.h"

using namespace starflow;

TEST_CASE("CLFR", "[types::CLFR]")
{
	types::Key k1(0x6, 4393423, 4324893, 2392, 24933);
	types::Key k2(0x6, 4304042, 3483999, 3503, 12882);

	SECTION("CLFR(): can be constructed without a key")
	{
		types::CLFR clfr;
		CHECK(clfr.key().ip_proto == 0);
		CHECK(clfr.key().ip_src   == 0);
		CHECK(clfr.key().ip_dst   == 0);
		CHECK(clfr.key().th_sport == 0);
		CHECK(clfr.key().th_dport == 0);
		CHECK(clfr.id()           == 0);
	}

	SECTION("CLFR(): can be constructed with a key, id and table id")
	{
		types::CLFR clfr(k1, 1, 5);
		CHECK(clfr.key() == k1);
		CHECK(clfr.id()  == 1);
		CHECK(clfr.table_id() == 5);
	}

	SECTION("CLFR(proto), to_proto()")
	{
		types::CLFR clfr1(k1, 1, 5);
		clfr1.add_packet(types::Packet(32323, 16));
		clfr1.add_packet(types::Packet(24222, 32));

		proto::clfr proto_clfr = clfr1.to_proto();
		types::CLFR clfr2(proto_clfr);

		CHECK(clfr1.key()      == clfr2.key());
		CHECK(clfr1.id()       == clfr2.id());
		CHECK(clfr1.table_id() == clfr2.table_id());
		CHECK(clfr1.complete() == clfr2.complete());
		CHECK(clfr1.evict_ts() == clfr2.evict_ts());
		CHECK(clfr1.packets().size() == clfr2.packets().size());
		CHECK(clfr1.packets().front() == clfr2.packets().front());
	}
}
