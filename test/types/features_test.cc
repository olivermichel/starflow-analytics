
#include <catch.h>
#include "../../src/types/clfr.h"

using namespace starflow;

TEST_CASE("Features", "[types::Features]")
{
	// flags definition, copied from src/types/features.h
	// cf. RFC 793 Sec. 3.1 (https://tools.ietf.org/html/rfc793#section-3.1)
	unsigned char urg = 32;
	unsigned char ack = 16;
	unsigned char psh =  8;
	unsigned char rst =  4;
	unsigned char syn =  2;
	unsigned char fin =  1;

	types::Features::tcp_flags_t f1(ack | syn);
	types::Features::tcp_flags_t f2(fin | ack);
	types::Features::tcp_flags_t f3(rst | psh | urg);
	types::Features::tcp_flags_t f4;

	SECTION("Features::tcp_flags_t()")
	{
		CHECK(f1.is_ack());
		CHECK(f1.is_syn());
		CHECK(!f1.is_urg());
		CHECK(!f1.is_psh());
		CHECK(!f1.is_rst());
		CHECK(!f1.is_fin());
		CHECK(f2.is_fin());
		CHECK(f2.is_ack());
		CHECK(!f2.is_syn());
		CHECK(f3.is_rst());
		CHECK(f3.is_psh());
		CHECK(f3.is_urg());
		CHECK(!f3.is_syn());
		CHECK(!f4.is_ack());
		CHECK(!f4.is_syn());
	}

	SECTION("Features::tcp_flags_t::operator==()")
	{
		types::Features::tcp_flags_t f5(fin | ack);

		CHECK(f1 != f2);
		CHECK(f1 == f1);
		CHECK(f2 != f3);
		CHECK(f2 == f5);
	}

	SECTION("Features::tcp_flags_t(proto), to_proto()")
	{
		proto::tcp_flags proto_tcp1 = f1.to_proto();
		types::Features::tcp_flags_t f1_restore(proto_tcp1);
		CHECK(f1_restore == f1);
	}

	SECTION("Features::operator==()")
	{
		types::Features feat1;
		types::Features feat2;
		types::Features feat3;

		feat1.ip_ttl    = 12;
		feat1.tcp_seq   = 23482;
		feat1.tcp_flags = f1;

		feat2.ip_ttl    = 30;
		feat2.tcp_seq   = 28888;
		feat2.tcp_flags = f2;

		feat3.ip_ttl    = 12;
		feat3.tcp_seq   = 23482;
		feat3.tcp_flags = f1;

		CHECK(feat1 == feat3);
		CHECK(feat1 != feat2);
	}

	SECTION("Features::Features(proto), to_proto()")
	{
		types::Features features;
		features.tcp_flags = f1;
		features.ip_ttl = 12;
		features.tcp_seq = 23929228;
		proto::features proto_features = features.to_proto();
		types::Features features_restore(proto_features);
		CHECK(features_restore == features);
	}
}
