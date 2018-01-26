
#include <raft>

#include "../etc/format_helpers.h"
#include "../kernels/clfr_file_reader.h"
#include "../kernels/formatted_printer.h"
#include "../kernels/group_by.h"
#include "../modules/counter.h"

int main(int argc, char** argv)
{
	namespace sf = starflow;
	using pkts_per_ip_t = std::pair<sf::types::ip_addr_t, unsigned long>;

	sf::modules::Counter<sf::types::ip_addr_t> pkt_counts;

	sf::kernels::CLFRFileReader clfr_file_reader("caida15_2a-01.clfr");

	sf::kernels::GroupBy<pkts_per_ip_t> src_ip_counter([&pkt_counts](const sf::types::CLFR& clfr) {
		return pkt_counts += std::make_pair(clfr.key().ip_src, clfr.n_packets());
	});

	sf::kernels::FormattedPrinter<pkts_per_ip_t> printer(
		[](std::ostream& os, const pkts_per_ip_t& p) {
			os << sf::etc::format_helpers::uint32_ip_addr_to_str(p.first)
			   << " -> " << p.second << std::endl;
	});

	raft::map m;
	m += clfr_file_reader >> src_ip_counter >> printer;
	m.exe();

	return 0;
}
