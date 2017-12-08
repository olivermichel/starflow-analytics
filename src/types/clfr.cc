
#include "clfr.h"

#include <numeric>

starflow::types::CLFR::CLFR()
	: _key() { }

starflow::types::CLFR::CLFR(const Key& k)
	: _key(k) { }

//TODO: deserialize packet list
starflow::types::CLFR::CLFR(const proto::clfr& proto_clfr)
	: _key(proto_clfr.key()),
	  _evict_ts(std::chrono::microseconds(proto_clfr.evict_ts())),
	  _complete(proto_clfr.complete()) { }

//TODO: serialize packet list
starflow::proto::clfr starflow::types::CLFR::to_proto() const
{
	proto::clfr proto_clfr;
	proto_clfr.set_allocated_key(new proto::key(_key.to_proto()));
	proto_clfr.set_complete(_complete);
	proto_clfr.set_evict_ts(_evict_ts.count());
	return proto_clfr;
}

const starflow::types::Key& starflow::types::CLFR::key() const
{
	return _key;
}

void starflow::types::CLFR::add_packet(Packet p)
{
	_packets.push_back(p);
}

const starflow::types::Packet& starflow::types::CLFR::last_packet() const
{
	return _packets.back();
}

const std::list<starflow::types::Packet>& starflow::types::CLFR::packets() const
{
	return _packets;
}

std::string starflow::types::CLFR::str_desc() const
{
	std::string desc = "starflow::types::CLFR(status="
					   + (_complete ? std::string("complete") : std::string("incomplete"))
					   + ", n_packets=" + std::to_string(n_packets())
					   + ", n_bytes=" + std::to_string(n_bytes())
					   + ", evict_ts=" + std::to_string(_evict_ts.count() / 1000000) + ")";
	return desc;
}

bool starflow::types::CLFR::complete() const
{
	return _complete;
}

std::chrono::microseconds starflow::types::CLFR::evict_ts() const
{
	return _evict_ts;
}

unsigned long starflow::types::CLFR::n_packets() const
{
	return _packets.size();
}

unsigned long starflow::types::CLFR::n_bytes() const
{
	return (unsigned long) std::accumulate(std::begin(_packets), std::end(_packets), 0,
		[](int s, const types::Packet& p) -> long { return s += p.len; });
}
