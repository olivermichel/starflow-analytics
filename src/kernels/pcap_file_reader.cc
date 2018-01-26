
#include "pcap_file_reader.h"

#include "../types/raw_packet.h"

starflow::kernels::PCAPFileReader::PCAPFileReader(const std::string& file_name,
												  outer_header_type outer_hdr)
	: _pcap_reader(file_name, outer_hdr == outer_header_type::eth)
{
	output.add_port<std::pair<starflow::types::Key, starflow::types::Packet>>("out");
}

raft::kstatus starflow::kernels::PCAPFileReader::run()
{
	starflow::types::Key k;
	starflow::types::Packet p;

	if (!_pcap_reader.end()) {
		_pcap_reader.next(k, p);
		auto pair = std::make_pair(k, p);
		output["out"].push(pair);
		return raft::proceed;
	} else return raft::stop;
}
