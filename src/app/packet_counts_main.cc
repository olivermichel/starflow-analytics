
#include <raft>

#include "../etc/format_helpers.h"
#include "../kernels/benchmark_printer.h"
#include "../kernels/clfr_file_reader.h"
#include "../kernels/formatted_printer.h"
#include "../kernels/group_by.h"
#include "../modules/counter.h"

#include <cxxopts/cxxopts.h>

struct config {
	std::string input;
	bool benchmark;
};

void _print_help(cxxopts::Options& opts, int exit_code = 0)
{
	if (exit_code == 0)
		std::cout << opts.help({""}) << std::endl;
	else
		std::cerr << opts.help({""}) << std::endl;

	exit(exit_code);
}

int main(int argc, char** argv)
{
	config config { };

	cxxopts::Options opts("packet_counts", " - counts packets per source address");

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
	using pkts_per_ip_t = std::pair<sf::types::ip_addr_t, unsigned long>;

	sf::modules::Counter<sf::types::ip_addr_t> pkt_counts;

	sf::kernels::CLFRFileReader clfr_file_reader(config.input);

	sf::kernels::GroupBy<sf::types::CLFR, pkts_per_ip_t> src_ip_counter(
		[&pkt_counts](const sf::types::CLFR& clfr) {
			return pkt_counts += std::make_pair(clfr.key().ip_src, clfr.n_packets());
	});

	sf::kernels::FormattedPrinter<pkts_per_ip_t> printer(
		[](std::ostream& os, const pkts_per_ip_t& p) {
			os << sf::etc::format_helpers::uint32_ip_addr_to_str(p.first)
			   << " -> " << p.second << std::endl;
	});

	sf::kernels::BenchmarkPrinter<pkts_per_ip_t> benchmark;

	raft::map m;

	if (config.benchmark)
		m += clfr_file_reader >> src_ip_counter >> benchmark;
	else
		m += clfr_file_reader >> src_ip_counter >> printer;

	m.exe();

	if (config.benchmark) {
		benchmark.done();

		std::cout << "{" << benchmark.total() << ", " << benchmark.runtime_ms() << ", "
					<< benchmark.mean_per_interval() << ", { ";

		for (unsigned i = 0; i < benchmark.counts().size(); i++)
			std::cout << benchmark.counts()[i] << (i == benchmark.counts().size() - 1 ? "}}" : ",");

		std::cout << std::endl;
	}

	return 0;
}
