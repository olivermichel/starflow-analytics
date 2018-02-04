
#include <raft>
#include <vector>
#include <set>

#include "../etc/format_helpers.h"
#include "../kernels/benchmark_printer.h"
#include "../kernels/clfr_file_reader.h"
#include "../kernels/formatted_printer.h"
#include "../kernels/group_by.h"
#include "../kernels/map.h"

#include <cxxopts/cxxopts.h>

template <typename key_t, typename argument_t, typename return_t>
class ConnectionsPerEpoch
{
	using _state_t = return_t;

public:
	return_t operator()(key_t k, argument_t s)
	{
		typename std::map<key_t, _state_t>::iterator i;

		if ((i = _s.find(k)) == std::end(_s))
			i = _s.emplace(k, 0).first;

		i->second++;

		return i->second;
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

cxxopts::Options _set_options()
{
	cxxopts::Options opts("active_connections", " - ");

	opts.add_options()
		("i,input", "CLFR input file (required)", cxxopts::value<std::string>(), "FILE")
		("b,benchmark", "print benchmarking information instead of app output")
		("h,help", "print this help message");

	return opts;
}

config _parse_config(cxxopts::Options opts, int argc, char** argv)
{
	config config {};

	auto parsed_opts = opts.parse(argc, argv);

	if (parsed_opts.count("h"))
		_print_help(opts);

	if (parsed_opts.count("i"))
		config.input = parsed_opts["i"].as<std::string>();
	else
		_print_help(opts, 1);

	config.benchmark = (bool) parsed_opts.count("b");

	return config;
}

int main(int argc, char** argv)
{
	namespace sf = starflow;
	using epochs_t = std::pair<sf::types::CLFR, std::set<unsigned long>>;
	using conn_per_epoch_t = std::vector<std::pair<unsigned long, unsigned long>>;

	config config = _parse_config(_set_options(), argc, argv);

	ConnectionsPerEpoch<unsigned long, unsigned long, unsigned long> cpe {};

	sf::kernels::CLFRFileReader clfr_file_reader(config.input);

	sf::kernels::Map<sf::types::CLFR, epochs_t> compute_epochs([](const sf::types::CLFR& clfr) {
		std::set<unsigned long> s;
		for (const auto& p : clfr.packets())
			s.insert(p.ts_in_us / 1000000);
		return std::make_pair(clfr, s);
	});

	sf::kernels::GroupBy<epochs_t, conn_per_epoch_t> group_epochs([&cpe](const epochs_t& e) {
		conn_per_epoch_t v;

		for (auto& i : e.second)
			v.emplace_back(i, cpe(i, 1));
		return v;
	});

	sf::kernels::FormattedPrinter<conn_per_epoch_t> printer([](std::ostream& os, const auto& c) {
		for (const auto& i : c)
			os << i.first << " -> " << i.second << std::endl;
	});

	sf::kernels::BenchmarkPrinter<conn_per_epoch_t> benchmark {};

	raft::map m;

	if (config.benchmark)
		m += clfr_file_reader >> compute_epochs >> group_epochs >> benchmark;
	else
		m += clfr_file_reader >> compute_epochs >> group_epochs >> printer;

	m.exe();

	if (config.benchmark)
		benchmark.done(), benchmark.print_results();

	return 0;
}
