
#include "packet.h"

starflow::types::Packet::Packet(std::chrono::microseconds ts_in, unsigned int len)
	: ts_in(ts_in), len(len) { }

starflow::types::Packet::Packet(unsigned long ts_in, unsigned int len)
	: ts_in(ts_in), len(len) { }

starflow::types::Packet::Packet(const starflow::proto::packet& p)
	: ts_in(p.ts_in()), len((unsigned) p.len()), features(p.features()) { }

bool starflow::types::Packet::operator==(const Packet& other) const
{
	return ts_in    == other.ts_in
		&& len      == other.len
		&& features == other.features;
}

bool starflow::types::Packet::operator!=(const Packet& other) const
{
	return !operator==(other);
}

starflow::proto::packet starflow::types::Packet::to_proto() const
{
	starflow::proto::packet p;
	p.set_ts_in(ts_in.count());
	p.set_len(len);
	p.set_allocated_features(new proto::features(features.to_proto()));
	return p;
}

std::string starflow::types::Packet::str_desc() const
{
	std::string desc = "starflow::types::Packet(ts_in=" + std::to_string(ts_in.count()) + ", len=" + std::to_string(len) + ")";
	return desc;
}
