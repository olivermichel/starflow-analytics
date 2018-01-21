
#include "packet_filter.h"

starflow::kernels::PacketFilter::PacketFilter(packet_filter_t&& filter_function)
	: _filter(std::move(filter_function))
{
	input.add_port<std::pair<types::Key, types::Packet>>("in");
	output.add_port<std::pair<types::Key, types::Packet>>("out");
}

raft::kstatus starflow::kernels::PacketFilter::run()
{
	std::pair<types::Key, types::Packet> k_p_pair;
	input["in"].pop(k_p_pair);

	if (_filter(k_p_pair))
		output["out"].push(k_p_pair);

	return raft::proceed;
}
