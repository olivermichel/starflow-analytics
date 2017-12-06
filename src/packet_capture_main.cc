
#include <raft>
#include <utility>

#include "kernels/live_capture.h"
#include "kernels/pcap_file_reader.h"
#include "kernels/printer.h"
#include "kernels/raw_packet_parser.h"
#include "types/clfr.h"

#include "kernels/tzsp_receiver.h"
#include "kernels/flow_table.h"

int main(int argc, char** argv)
{
	raft::lambdak<std::pair<starflow::types::Key, starflow::types::Packet>>
		packet_printer(1, 0, [&] (Port& in, Port& out) {
		std::pair<starflow::types::Key, starflow::types::Packet> p {};
		in["0"].pop(p);
		std::cout << p.first.str_desc() << " " << p.second.str_desc() << std::endl;
		std::cout << p.second.features.tcp_flags.str_desc() << std::endl;
		return raft::proceed;
	});

	raft::lambdak<starflow::types::CLFR>
		clfr_printer(1, 0, [&] (Port& in, Port& out) {
		starflow::types::CLFR c {};
		in["0"].pop(c);
		std::cout << c.key().str_desc() << " " << c.str_desc() << std::endl;
		return raft::proceed;
	});

	auto hdr_type = starflow::kernels::RawPacketParser::outer_header_type::eth;
	auto cap_len  = starflow::kernels::RawPacketParser::capture_length::trunc;

	starflow::kernels::TZSPReceiver tzsp_receiver(37008);
	starflow::kernels::RawPacketParser raw_packet_parser(hdr_type, cap_len);
	starflow::kernels::FlowTable flow_table;

	raft::map m;
	m += tzsp_receiver >> raw_packet_parser >> flow_table >> clfr_printer;
	m.exe();

	return 0;
}
