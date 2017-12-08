
#include "flow_table.h"

void starflow::modules::FlowTable::add_packet(types::Key key, types::Packet packet)
	throw(std::logic_error)
{
	if (_mode == mode::callback && !_callback)
		throw std::logic_error("FlowTable: no callback function set");

	auto ts = packet.ts;

	auto flow_table_iter = _lookup_and_insert(std::move(key), std::move(packet));
	_check_evict(flow_table_iter, ts);

	if ((ts.count() - _last_to_check.count()) > _to_check_interval.count())
		_check_timeouts(ts);
}

void starflow::modules::FlowTable::add_packet(std::pair<types::Key, types::Packet> pair)
	throw(std::logic_error)
{
	add_packet(pair.first, pair.second);
}

void starflow::modules::FlowTable::set_mode(enum mode m)
{
	_mode = m;
}

void starflow::modules::FlowTable::set_callback(modules::FlowTable::export_flow_callback_t&& callback)
{
	_callback = std::move(callback);
}

unsigned long long starflow::modules::FlowTable::count_packets_processed() const
{
	return _n_packets_processed;
}

unsigned long long starflow::modules::FlowTable::count_flows_processed() const
{
	return _n_flows_processed;
}

unsigned long long starflow::modules::FlowTable::count_packets() const
{
	return _n_packets;
}

unsigned long long starflow::modules::FlowTable::count_flows() const
{
	return _active_flows.size();
}

const starflow::modules::FlowTable::exported_flows_table_t&
	starflow::modules::FlowTable::exported_flows() const
{
	return _exported_flows;
}

void starflow::modules::FlowTable::force_export_udp(bool complete)
{
	for (auto i = std::begin(_active_flows); i != std::end(_active_flows);)
		i = i->first.ip_proto == (uint8_t) _ip_proto::udp ?
			_evict_flow(i, i->second.last_packet().ts, complete) : std::next(i, 1);
}

void starflow::modules::FlowTable::force_export_tcp(bool complete)
{
	for (auto i = std::begin(_active_flows); i != std::end(_active_flows);)
		i = i->first.ip_proto == (uint8_t) _ip_proto::tcp ?
			_evict_flow(i, i->second.last_packet().ts, complete) : std::next(i, 1);
}

starflow::modules::FlowTable::flow_table_t::iterator
	starflow::modules::FlowTable::_lookup_and_insert(types::Key&& key, types::Packet&& packet)
{
	auto i = _active_flows.find(key);

	if (i == std::end(_active_flows)) {
		i = _active_flows.emplace(key, types::CLFR(key, _next_id++)).first;
		_n_flows++;
	}

	if (i != std::end(_active_flows)) {
		i->second.add_packet(std::move(packet));
		_n_packets++;
	}

	return i;
}

void starflow::modules::FlowTable::
	_check_evict(flow_table_t::iterator it, std::chrono::microseconds ts)
{
	if (it->first.ip_proto == (uint8_t) _ip_proto::tcp
		&& it->second.last_packet().features.tcp_flags.is_fin()) {
		_evict_flow(it, ts, true);
	} /* else if (_incomplete_evict_policy == incomplete_evict_policy::pkt_count
			   && i->second.n_packets() >= _incomplete_evict_pkt_count) {
		_evict_flow(i, packet.ts, false);
	} */
}

void starflow::modules::FlowTable::_check_timeouts(std::chrono::microseconds trigger_ts)
{
	for (auto i = std::begin(_active_flows); i != std::end(_active_flows);) {

		// in-place removal requires setting iterator manually
		long long int since_last_packet
			= (trigger_ts.count() - i->second.last_packet().ts.count());

		if (i->first.ip_proto == (uint8_t) _ip_proto::udp && since_last_packet >= _udp_to.count()) {
			i = _evict_flow(i, trigger_ts, true);
		} /* else if (_incomplete_evict_policy == incomplete_evict_policy::to
				   && since_last_packet >= _incomplete_evict_to.count()) {
			i = _evict_flow(i, trigger_ts, false);
		} */ else {
			i = std::next(i, 1);
		}
	}

	_last_to_check = trigger_ts;
}

starflow::modules::FlowTable::flow_table_t::iterator
	starflow::modules::FlowTable::_evict_flow(const flow_table_t::iterator& i,
											  std::chrono::microseconds evict_ts, bool complete)
{
	i->second._complete = complete;
	i->second._evict_ts = evict_ts;

	if (_mode == mode::callback)
		_callback(i->second);
	else if (_mode == mode::store)
		_exported_flows.emplace_back(i->second);

	_n_packets -= i->second.n_packets();
	_n_flows_processed += 1;

	return _active_flows.erase(i);
}
