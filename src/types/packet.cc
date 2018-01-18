
#include "packet.h"

starflow::types::Packet::Packet(std::chrono::microseconds ts, unsigned int len)
	: ts(ts), len(len) { }

starflow::types::Packet::Packet(unsigned long ts, unsigned int len)
	: ts(ts), len(len) { }

starflow::types::Packet::Packet(const starflow::proto::packet& p)
	: ts(p.ts()), len((unsigned) p.len()), features(p.features()) { }

bool starflow::types::Packet::operator==(const Packet& other) const
{
	return ts       == other.ts
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
	p.set_ts(ts.count());
	p.set_len(len);
	p.set_allocated_features(new proto::features(features.to_proto()));
	return p;
}

std::string starflow::types::Packet::str_desc() const
{
	std::string desc = "starflow::types::Packet(ts=" + std::to_string(ts.count()) + ", len=" + std::to_string(len) + ")";
	return desc;
}
