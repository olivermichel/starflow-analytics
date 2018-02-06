
#include "pcap_file_reader.h"

#include "../types/raw_packet.h"

starflow::kernels::PCAPFileReader::PCAPFileReader(const std::string& file_name,
												  outer_header_type outer_hdr,
												  unsigned batch_size)
	: _pcap_reader(file_name, outer_hdr == outer_header_type::eth),
	  _batch_size(batch_size)
{
	if (_batch_size)
		output.add_port<_batch_out_t>("out");
	else
		output.add_port<_out_t>("out");
}

raft::kstatus starflow::kernels::PCAPFileReader::run()
{
	_batch_out_t v;
	unsigned batch_counter = 0;

	starflow::types::Key k;
	starflow::types::Packet p;

	if (!_pcap_reader.end()) {
		_pcap_reader.next(k, p);
		auto pair = std::make_pair(k, p);

		if (_batch_size) {
			if (batch_counter <= _batch_size)
				v.push_back(pair);
			else
				output["out"].push(v);`
		} else {
			output["out"].push(pair);
		}
		return raft::proceed;
	} else return raft::stop;
}
