
#include <raft>

#include "../etc/format_helpers.h"
#include "../kernels/clfr_file_reader.h"
#include "../kernels/filter.h"
#include "../kernels/formatted_printer.h"
#include "../kernels/group_by.h"

template <typename key_t, typename argument_t, typename return_t>
class OOS
{
	using _state_t = std::pair<std::uint32_t, return_t>;

public:
	return_t operator()(key_t k, argument_t s) {

		typename std::map<key_t, _state_t>::iterator i;
		auto& f = s.features;

		if ((i = _s.find(k)) == std::end(_s))
			i = _s.emplace(k, std::make_pair(f.tcp_seq, 0)).first;

		if (i->second.first != f.tcp_seq)
			i->second.second++;

		i->second.first += (f.tcp_flags.is_syn() || f.tcp_flags.is_fin() ? 1 : f.tcp_pl_len);
		return i->second.second;
	}

private:
	std::map<key_t, _state_t> _s;
};

int main(int argc, char** argv)
{
	if (argc != 2) {
		std::cerr << "Usage: " << argv[0] << " <data.clfr>" << std::endl;
		return 1;
	}

	namespace sf = starflow;
	using oos_per_flow_t = std::pair<sf::types::Key, unsigned int>;

	using key_t = starflow::types::Key;
	using arg_t = starflow::types::Packet;
	using res_t = unsigned int;

	OOS<key_t, arg_t, res_t> oos;

	sf::kernels::CLFRFileReader clfr_file_reader(argv[1]);

	sf::kernels::Filter<sf::types::CLFR> tcp_filter([](const sf::types::CLFR& clfr) {
		return clfr.key().ip_proto == 6;
	});

	sf::kernels::GroupBy<oos_per_flow_t> flow_tcp_oos([&oos](const sf::types::CLFR& clfr) {
		unsigned int oos_packets = 0;
		for (auto& packet : clfr.packets())
			oos_packets = oos(clfr.key(), packet);
		return std::make_pair(clfr.key(), oos_packets);
	});

	sf::kernels::FormattedPrinter<oos_per_flow_t> printer(
		[](std::ostream& os, const oos_per_flow_t& p) {
			os << p.first.str_desc() << " -> " << p.second << std::endl;
	});

	raft::map m;
	m += clfr_file_reader >> tcp_filter >> flow_tcp_oos >> printer;
	m.exe();

	return 0;
}