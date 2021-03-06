
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
	unsigned batch_size;
};

void _print_help(cxxopts::Options& opts, int exit_code = 0)
{
	std::ostream& os = (exit_code ? std::cerr : std::cout);
	os << opts.help({""}) << std::endl;
	exit(exit_code);
}

cxxopts::Options _set_options()
{
	cxxopts::Options opts("batch_test", " - ");

	opts.add_options()
		("i,input", "CLFR input file (required)", cxxopts::value<std::string>(), "FILE")
		("b,benchmark", "print benchmarking information instead of app output")
		("a,batch", "batch size", cxxopts::value<unsigned>(), "N")
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

	if (parsed_opts.count("a"))
		config.batch_size = parsed_opts["a"].as<unsigned>();
	else
		config.batch_size = 0;

	config.benchmark = (bool) parsed_opts.count("b");

	return config;
}

int main(int argc, char** argv)
{
	config config { };

	cxxopts::Options opts("packet_counts", " - counts packets per source address");

	opts.add_options()
		("i,input", "CLFR input file (required)", cxxopts::value<std::string>(), "FILE")
		("b,benchmark", "print benchmarking information instead of app output")
		("a,batch", "batch size", cxxopts::value<unsigned>(), "N")
		("h,help", "print this help message");

	auto parsed_opts = opts.parse(argc, argv);

	if (parsed_opts.count("h"))
		_print_help(opts);

	if (parsed_opts.count("i"))
		config.input = parsed_opts["i"].as<std::string>();
	else
		_print_help(opts, 1);

	config.benchmark = (bool) parsed_opts.count("b");

	if (parsed_opts.count("a"))
		config.batch_size = parsed_opts["a"].as<unsigned>();
	else
		config.batch_size = 0;

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

	if (config.benchmark)
		benchmark.done(), benchmark.print_results();

	return 0;
}
