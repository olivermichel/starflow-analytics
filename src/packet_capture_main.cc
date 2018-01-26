
#include <raft>
#include <utility>

#include "kernels/live_capture.h"
#include "kernels/pcap_file_reader.h"
#include "kernels/packet_printer.h"
#include "kernels/raw_packet_parser.h"
#include "types/clfr.h"
#include "kernels/packet_filter.h"

#include "kernels/tzsp_receiver.h"
#include "kernels/flow_table.h"
#include "kernels/clfr_printer.h"
#include "kernels/map.h"

#include "modules/ui_adapter.h"
#include "kernels/printer.h"

int main(int argc, char** argv)
{
	namespace sf = starflow;
	using kp_pair = std::pair<starflow::types::Key, starflow::types::Packet>;

	sf::modules::UIAdapter ui_adapter("127.0.0.1:23202");
	std::thread ui_server_thread([&ui_adapter](){ ui_adapter(); });
	ui_server_thread.detach();


	auto hdr_type = sf::kernels::RawPacketParser::outer_header_type::eth;
	auto cap_len  = sf::kernels::RawPacketParser::capture_length::trunc;

	sf::kernels::LiveCapture live_capture("en10");
//	starflow::kernels::TZSPReceiver tzsp_receiver(37008);
	sf::kernels::RawPacketParser raw_packet_parser(hdr_type, cap_len);
	sf::kernels::PacketFilter packet_filter([](kp_pair p) { return p.first.ip_proto == 17; });

	starflow::kernels::FlowTable flow_table;

	sf::kernels::Map<kp_pair, unsigned> map([](kp_pair p) -> unsigned { return p.second.len; });

	sf::kernels::PacketPrinter packet_printer(true);
	sf::kernels::CLFRPrinter clfr_printer(true);
	sf::kernels::Printer<unsigned> p;

	raft::map m;
//	m += tzsp_receiver >> raw_packet_parser >> flow_table >> clfr_printer;
	m += live_capture >> raw_packet_parser >> flow_table >> clfr_printer;
//	m += live_capture >> raw_packet_parser >> packet_filter >> map >> p;
	m.exe();

	return 0;
}
