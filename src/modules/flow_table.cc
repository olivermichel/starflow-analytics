
#include "flow_table.h"

starflow::modules::FlowTable::FlowTable(std::uint16_t id)
	: _id(id) { }

unsigned starflow::modules::FlowTable::id() const
{
	return _id;
}

void starflow::modules::FlowTable::add_packet(types::Key key, types::Packet packet)
	throw(std::logic_error)
{
	if (!_callback)
		throw std::logic_error("FlowTable: no callback function set");


	auto ts_s = (std::uint32_t)(packet.ts_in_us / _E6);
	auto flow_table_iter = _lookup_and_insert(std::move(key), std::move(packet));
	_check_evict(flow_table_iter, ts_s);

	if ((ts_s - _last_to_check_s) >= _to_check_interval_s)
		_check_timeouts(ts_s);

	if ((ts_s - _last_ack_check_s) >= _ack_check_interval_s)
		_check_last_ack(ts_s);
}

void starflow::modules::FlowTable::set_callback(
	modules::FlowTable::export_flow_callback_t&& callback)
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

const starflow::modules::FlowTable::flow_table_t& starflow::modules::FlowTable::flows() const
{
	return _active_flows;
}

void starflow::modules::FlowTable::_force_export_udp(bool complete)
{
	for (auto i = std::begin(_active_flows); i != std::end(_active_flows);) {
		i = i->first.ip_proto == (uint8_t) _ip_proto::udp
			? _evict_flow(i, (std::uint32_t)(i->second.last_packet().ts_in_us / _E6), complete)
			: std::next(i, 1);
	}
}

void starflow::modules::FlowTable::_force_export_tcp(bool complete)
{
	for (auto i = std::begin(_active_flows); i != std::end(_active_flows);)
		i = i->first.ip_proto == (uint8_t) _ip_proto::tcp ?
			_evict_flow(i, (std::uint32_t)(i->second.last_packet().ts_in_us / _E6), complete) : std::next(i, 1);
}

void starflow::modules::FlowTable::_force_check_last_ack()
{
	_check_last_ack(0);
}

starflow::modules::FlowTable::flow_table_t::iterator
	starflow::modules::FlowTable::_lookup_and_insert(types::Key&& key, types::Packet&& packet)
{
	auto i = _active_flows.find(key);

	if (i == std::end(_active_flows)) {
		i = _active_flows.emplace(key, types::CLFR(key, _next_id++, _id)).first;
		_n_flows++;
	}

	if (i != std::end(_active_flows)) {
		i->second.add_packet(std::move(packet));
		_n_packets++;
	}

	return i;
}

void starflow::modules::FlowTable::
	_check_evict(flow_table_t::iterator it, std::uint32_t trigger_ts_s)
{
	if (it->first.ip_proto == (uint8_t) _ip_proto::tcp
		&& it->second.last_packet().features.tcp_flags.is_fin()) {
		_evict_flow(it, trigger_ts_s, true);
	} /* else if (_incomplete_evict_policy == incomplete_evict_policy::pkt_count
			   && i->second.n_packets() >= _incomplete_evict_pkt_count) {
		_evict_flow(i, packet.ts_in, false);
	} */
}

void starflow::modules::FlowTable::_check_timeouts(std::uint32_t trigger_ts_s)
{
	for (auto i = std::begin(_active_flows); i != std::end(_active_flows);) {

		// in-place removal requires setting iterator manually
		std::uint32_t since_last_packet_s
			= (trigger_ts_s - (std::uint32_t)(i->second.last_packet().ts_in_us/_E6));

		if (i->first.ip_proto == (uint8_t) _ip_proto::udp && since_last_packet_s >= _udp_to_s) {
				i = _evict_flow(i, trigger_ts_s, true);
		} else if (i->first.ip_proto == (uint8_t) _ip_proto::tcp
				&& ((since_last_packet_s >= _tcp_to_s)
					|| i->second.n_packets() >= _incomplete_evict_pkt_count)) {
			i = _evict_flow(i, trigger_ts_s, false);
		} else {
			i = std::next(i, 1);
		}
	}

	_last_to_check_s = trigger_ts_s;
}

void starflow::modules::FlowTable::_check_last_ack(std::uint32_t trigger_ts_s)
{
	auto is_last_ack = [](types::CLFR& flow) -> bool {
		return flow.key().ip_proto == (uint8_t) _ip_proto::tcp
			   && flow.n_packets() == 1
			   && flow.packets().front().features.tcp_flags.is_ack()
			   && !flow.packets().front().features.tcp_flags.is_syn();
	};

	for (auto i = std::begin(_active_flows); i != std::end(_active_flows);) {
		if (is_last_ack(i->second)) {
			i = _delete_flow(i);
		} else {
			i = std::next(i, 1);
		}
	}
//		i = is_last_ack(i->second) ? _delete_flow(i) : std::next(i, 1);

	_last_ack_check_s = trigger_ts_s;
}

starflow::modules::FlowTable::flow_table_t::iterator
	starflow::modules::FlowTable::_evict_flow(const flow_table_t::iterator& i,
											  std::uint32_t evict_ts_s, bool complete)
{
	i->second._complete = complete;
	i->second._evict_ts_s = evict_ts_s;

	_callback(i->second);

	_n_packets -= i->second.n_packets();
	_n_flows_processed += 1;
	_n_packets_processed += i->second.n_packets();

	return _active_flows.erase(i);
}

starflow::modules::FlowTable::flow_table_t::iterator
	starflow::modules::FlowTable::_delete_flow(const flow_table_t::iterator& i)
{
	_n_packets -= i->second.n_packets();
	return _active_flows.erase(i);
}