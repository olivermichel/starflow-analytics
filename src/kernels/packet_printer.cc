
#include "packet_printer.h"

#include "../types/key.h"
#include "../types/packet.h"

starflow::kernels::PacketPrinter::PacketPrinter(bool sink, std::ostream& os)
	: _sink(sink), _os(os)
{
	input.add_port<std::pair<types::Key, types::Packet>>("in");
	if (!_sink)
		output.add_port<std::pair<types::Key, types::Packet>>("out");
}

raft::kstatus starflow::kernels::PacketPrinter::run()
{
	std::pair<types::Key, types::Packet> k_p_pair;
	input["in"].pop(k_p_pair);

	_os << k_p_pair.first.str_desc() << " / " << k_p_pair.second.str_desc() << std::endl;

	if(!_sink)
		output["out"].push(k_p_pair);

	return raft::proceed;
}
