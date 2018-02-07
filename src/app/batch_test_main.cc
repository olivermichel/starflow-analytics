
#include <raft>
#include <vector>
#include <set>

#include "../etc/format_helpers.h"
#include "../kernels/benchmark_printer.h"
#include "../kernels/clfr_file_reader.h"

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
	namespace sf = starflow;

	config config = _parse_config(_set_options(), argc, argv);

	sf::kernels::CLFRFileReader clfr_file_reader(config.input, config.batch_size);

	if (config.batch_size > 0) {
		sf::kernels::BenchmarkPrinter<sf::kernels::CLFRFileReader::_batch_out_t> benchmark {};

		raft::map m;
		m += clfr_file_reader >> benchmark;
		m.exe();

		if (config.benchmark)
			benchmark.done(), benchmark.print_results();
	} else {
		sf::kernels::BenchmarkPrinter<sf::kernels::CLFRFileReader::_out_t> benchmark {};

		raft::map m;
		m += clfr_file_reader >> benchmark;
		m.exe();

		if (config.benchmark)
			benchmark.done(), benchmark.print_results();
	}

	return 0;
}
