
#include <catch.h>
#include "../../src/modules/pcap_file_reader.h"

using namespace starflow::modules;

TEST_CASE("PCAPFileReader", "[PCAPFileReader]")
{
	SECTION("PCAPFileReader(file_name)")
	{
		CHECK_NOTHROW(PCAPFileReader("test/data/test.pcap"));
		CHECK_THROWS_AS(PCAPFileReader("test/data/does_not_exist.pcap"), std::runtime_error);
	}

	SECTION("end()")
	{
		PCAPFileReader p1("test/data/test.pcap");
		CHECK_FALSE(p1.end());

		PCAPFileReader p2("test/data/empty.pcap");
		CHECK(p2.end());
	}

	SECTION("next(Packet)")
	{
		starflow::types::Key key;
		starflow::types::Packet pkt;

		PCAPFileReader p1("test/data/test.pcap");
		CHECK_FALSE(p1.end());

		for (unsigned i = 0; i < 36; i++) {
			CHECK_FALSE(p1.end());
			p1.next(key, pkt);
		}

		PCAPFileReader p2("test/data/test.pcap");
		unsigned pkt_count = 0;

		while (!p2.end()) {
			p2.next(key, pkt);
			pkt_count++;
		}

		CHECK(pkt_count == 36);
	}
}
