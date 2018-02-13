
#include <raft>

#include "../etc/format_helpers.h"
#include "../kernels/benchmark_printer.h"
#include "../kernels/clfr_file_reader.h"
#include "../kernels/formatted_printer.h"

#include <cxxopts/cxxopts.h>

struct config {
	std::string input;
	bool benchmark;
	unsigned batch_size;
	unsigned parallelization_factor;
	bool verbose;
};

void _print_help(cxxopts::Options& opts, int exit_code = 0)
{
	std::ostream& os = (exit_code ? std::cerr : std::cout);
	os << opts.help({""}) << std::endl;
	exit(exit_code);
}

cxxopts::Options _set_options()
{
	cxxopts::Options opts("parallel_test", " - ");

	opts.add_options()
		("i,input",     "CLFR input file (required)", cxxopts::value<std::string>(), "FILE")
		("b,benchmark", "print benchmarking information instead of app output")
		("a,batch",     "batch size", cxxopts::value<unsigned>(), "N")
		("p,parallel",  "parallelization factor", cxxopts::value<unsigned>(), "N")
		("v,verbose",   "print intermediate output")
		("h,help",      "print this help message");

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
	config.verbose = (bool) parsed_opts.count("v");
	config.batch_size = parsed_opts.count("a") ? parsed_opts["a"].as<unsigned>() : 0;
	config.parallelization_factor = parsed_opts.count("p") ? parsed_opts["p"].as<unsigned>() : 1;

	return config;
}

int main(int argc, char** argv)
{
	namespace sf = starflow;

	config config = _parse_config(_set_options(), argc, argv);

	std::vector<std::unique_ptr<sf::kernels::CLFRFileReader>> clfr_file_readers;
	std::vector<std::unique_ptr<sf::kernels::BenchmarkPrinter<sf::types::CLFR>>> benchmark_printers;


	for (unsigned i = 0; i < config.parallelization_factor; i ++) {

		clfr_file_readers.push_back(
			std::make_unique<sf::kernels::CLFRFileReader>(config.input)
		);

		benchmark_printers.push_back(
			std::make_unique<sf::kernels::BenchmarkPrinter<sf::types::CLFR>>(std::to_string(i))
		);
	}

	raft::map m;

	for (unsigned i = 0; i < config.parallelization_factor; i++)
		m += *clfr_file_readers[i] >> *benchmark_printers[i];

	m.exe();

	unsigned long int entries = 0;


	for (unsigned i = 0; i < config.parallelization_factor; i++) {
		benchmark_printers[i]->done();
		entries += benchmark_printers[i]->total();
	}

	std::cout << entries / (benchmark_printers[0]->runtime_ms() / 1000) << std::endl;

	return 0;
}
