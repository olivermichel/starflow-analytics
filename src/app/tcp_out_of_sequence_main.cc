
#include <raft>

#include "../etc/format_helpers.h"
#include "../kernels/benchmark_printer.h"
#include "../kernels/clfr_file_reader.h"
#include "../kernels/filter.h"
#include "../kernels/formatted_printer.h"
#include "../kernels/group_by.h"

#include <cxxopts/cxxopts.h>

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

struct config {
	std::string input;
	bool benchmark;
};

void _print_help(cxxopts::Options& opts, int exit_code = 0)
{
	std::ostream& os = (exit_code ? std::cerr : std::cout);
	os << opts.help({""}) << std::endl;
	exit(exit_code);
}

int main(int argc, char** argv)
{
	config config { };

	cxxopts::Options opts("tcp_out_of_sequence", " - ");

	opts.add_options()
		("i,input", "CLFR input file (required)", cxxopts::value<std::string>(), "FILE")
		("b,benchmark", "print benchmarking information instead of app output")
		("h,help", "print this help message");

	auto parsed_opts = opts.parse(argc, argv);

	if (parsed_opts.count("h"))
		_print_help(opts);

	if (parsed_opts.count("i"))
		config.input = parsed_opts["i"].as<std::string>();
	else
		_print_help(opts, 1);

	config.benchmark = (bool) parsed_opts.count("b");

	namespace sf = starflow;
	using oos_per_flow_t = std::pair<sf::types::Key, unsigned int>;

	using key_t = starflow::types::Key;
	using arg_t = starflow::types::Packet;
	using res_t = unsigned int;

	OOS<key_t, arg_t, res_t> oos;

	sf::kernels::CLFRFileReader clfr_file_reader(config.input);

	sf::kernels::Filter<sf::types::CLFR> tcp_filter(
		[](const sf::types::CLFR& clfr) {
			return clfr.key().ip_proto == 6;
	});

	sf::kernels::GroupBy<sf::types::CLFR, oos_per_flow_t> flow_tcp_oos(
		[&oos](const sf::types::CLFR& clfr) {
			unsigned int oos_packets = 0;
			for (auto& packet : clfr.packets())
				oos_packets = oos(clfr.key(), packet);
			return std::make_pair(clfr.key(), oos_packets);
	});

	sf::kernels::FormattedPrinter<oos_per_flow_t> printer(
		[](std::ostream& os, const oos_per_flow_t& p) {
			os << p.first.str_desc() << " -> " << p.second << std::endl;
	});

	sf::kernels::BenchmarkPrinter<oos_per_flow_t> benchmark;

	raft::map m;

	if (config.benchmark)
		m += clfr_file_reader >> tcp_filter >> flow_tcp_oos >> benchmark;
	else
		m += clfr_file_reader >> tcp_filter >> flow_tcp_oos >> printer;

	m.exe();

	if (config.benchmark)
		benchmark.done(), benchmark.print_results();

	return 0;
}