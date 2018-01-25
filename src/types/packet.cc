
#include "packet.h"

starflow::types::Packet::Packet(std::chrono::microseconds ts_in, std::chrono::microseconds ts_out,
								unsigned int len)
	: ts_in(ts_in), ts_out(ts_out), len(len) { }

starflow::types::Packet::Packet(unsigned long ts_in, unsigned long ts_out, unsigned int len)
	: ts_in(ts_in), ts_out(ts_out), len(len) { }

starflow::types::Packet::Packet(const starflow::proto::packet& p)
	: ts_in(p.ts_in()),
	  ts_out(p.ts_out()),
	  len((unsigned) p.len()),
	  qid((unsigned) p.qid()),
	  qlen((unsigned) p.qlen()),
	  features(p.features()) { }

bool starflow::types::Packet::operator==(const Packet& other) const
{
	return ts_in    == other.ts_in
		&& ts_out   == other.ts_out
		&& len      == other.len
		&& qid      == other.qid
		&& qlen     == other.qlen
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
	p.set_ts_out(ts_out.count());
	p.set_len(len);
	p.set_qid(qid);
	p.set_qlen(qlen);
	p.set_allocated_features(new proto::features(features.to_proto()));
	return p;
}

std::string starflow::types::Packet::str_desc() const
{
	std::string desc = "starflow::types::Packet(ts_in=" + std::to_string(ts_in.count())
					   + ", ts_out=" + std::to_string(ts_out.count())
					   + ", len=" + std::to_string(len) + ")";
	return desc;
}
