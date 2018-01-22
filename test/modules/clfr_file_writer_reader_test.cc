
#include <catch.h>
#include "../../src/modules/clfr_file_writer.h"
#include "../../src/modules/clfr_file_reader.h"

using namespace starflow;

TEST_CASE("CLFRFileWriter/CLFRFileReader", "[modules::CLFRFileWriter][modules::CLFRFileReader]")
{
	types::Key key1(0x6, 4393423, 4324893, 2392, 24933);
	types::Key key2(0x6, 4304042, 3483999, 3503, 12882);
	types::CLFR clfr1(key1, 1, 5);
	types::CLFR clfr2(key2, 2, 10);
	clfr1.add_packet(types::Packet(32323, 16));
	clfr1.add_packet(types::Packet(24222, 32));
	clfr2.add_packet(types::Packet(9723, 64));

	SECTION("write a clfr file")
	{
		modules::CLFRFileWriter writer("test_clfrs.clfr");

		CHECK(writer.total_bytes() == 0);
		CHECK(writer.total_clfrs() == 0);

		CHECK_NOTHROW(writer.write_clfr(clfr1));
		CHECK(writer.total_bytes() == 50);
		CHECK(writer.total_clfrs() == 1);

		CHECK_NOTHROW(writer.write_clfr(clfr2));
		CHECK(writer.total_bytes() == 86);
		CHECK(writer.total_clfrs() == 2);

		writer.close();
	}
}