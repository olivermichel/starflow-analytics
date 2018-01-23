
#include "clfr.h"

#include <numeric>

starflow::types::CLFR::CLFR()
	: _key(), _id(0), _table_id(0) { }

starflow::types::CLFR::CLFR(const Key& k, unsigned long long flow_id, unsigned table_id)
	: _key(k), _id(flow_id), _table_id(table_id) { }

starflow::types::CLFR::CLFR(const proto::clfr& proto_clfr)
	: _key(proto_clfr.key()),
	  _id(static_cast<unsigned long long>(proto_clfr.id())),
	  _table_id(static_cast<unsigned>(proto_clfr.table_id())),
	  _evict_ts(std::chrono::microseconds(proto_clfr.evict_ts())),
	  _complete(proto_clfr.complete())
{
	for (int i = 0; i < proto_clfr.packets_size(); i++)
		_packets.emplace_back(proto_clfr.packets(i));
}

starflow::proto::clfr starflow::types::CLFR::to_proto() const
{
	proto::clfr proto_clfr;
	proto_clfr.set_allocated_key(new proto::key(_key.to_proto()));
	proto_clfr.set_id(_id);
	proto_clfr.set_complete(_complete);
	proto_clfr.set_evict_ts(_evict_ts.count());
	proto_clfr.set_table_id(_table_id);

	for (auto& p : _packets) {
		proto::packet* new_packet = proto_clfr.add_packets();
		*new_packet = p.to_proto();
	}

	return proto_clfr;
}

const starflow::types::Key& starflow::types::CLFR::key() const
{
	return _key;
}

unsigned long long starflow::types::CLFR::id() const
{
	return _id;
}

unsigned starflow::types::CLFR::table_id() const
{
	return _table_id;
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

bool starflow::types::CLFR::operator==(const starflow::types::CLFR& other) const
{
	return _key == other._key
		&& _id  == other._id
		&& _evict_ts == other._evict_ts
		&& _complete == other._complete
		&& _table_id == other._table_id
		&& _packets == other._packets;
}

bool starflow::types::CLFR::operator!=(const starflow::types::CLFR& other) const
{
	return !operator==(other);
}
