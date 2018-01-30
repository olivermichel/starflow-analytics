
#include <raft>

#include "../etc/format_helpers.h"
#include "../kernels/clfr_file_reader.h"
#include "../kernels/formatted_printer.h"
#include "../kernels/group_by.h"
#include "../modules/ewma.h"

int main(int argc, char** argv)
{
	if (argc != 2) {
		std::cerr << "Usage: " << argv[0] << " <data.clfr>" << std::endl;
		return 1;
	}

	namespace sf = starflow;
	using ewma_per_flow_t = std::pair<sf::types::Key, unsigned long long>;

	sf::modules::EWMA<sf::types::Key> ewma;
	sf::kernels::CLFRFileReader clfr_file_reader(argv[1]);

	sf::kernels::GroupBy<ewma_per_flow_t> flow_ewma_calculator([&ewma](const sf::types::CLFR& clfr){
		unsigned long long last_ewma = 0;
		for (const auto& packet : clfr.packets())
			last_ewma = ewma(clfr.key(), packet.ts_out_us - packet.ts_in_us);
		return std::make_pair(clfr.key(), last_ewma);
	});

	sf::kernels::FormattedPrinter<ewma_per_flow_t> printer(
		[](std::ostream& os, const ewma_per_flow_t& p) {
			os << p.first.str_desc() << " -> " << p.second << std::endl;
		});

	raft::map m;
	m += clfr_file_reader >> flow_ewma_calculator >> printer;
	m.exe();

	return 0;
}
