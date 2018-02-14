
#include <raft>

#include "../etc/format_helpers.h"
#include "../kernels/benchmark_printer.h"
#include "../kernels/clfr_file_reader.h"
#include "../kernels/formatted_printer.h"

#include <cxxopts/cxxopts.h>

namespace sf = starflow;

using src_container_t = std::vector<std::unique_ptr<sf::kernels::CLFRFileReader>>;
using snk_container_t
	= std::vector<std::unique_ptr<sf::kernels::BenchmarkPrinter<starflow::types::CLFR>>>;
using snk_batch_container_t
	= std::vector<std::unique_ptr<sf::kernels::BenchmarkPrinter<std::vector<sf::types::CLFR>>>>;

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
	config.batch_size = parsed_opts.count("a") ? parsed_opts["a"].as<unsigned>() : 1;
	config.parallelization_factor = parsed_opts.count("p") ? parsed_opts["p"].as<unsigned>() : 1;

	return config;
}

void _initialize_sources(const config& config, src_container_t& sources)
{
	for (unsigned i = 0; i < config.parallelization_factor; i ++) {
		if (config.batch_size > 1)
			sources.push_back(
				std::make_unique<sf::kernels::CLFRFileReader>(config.input, config.batch_size));
		else
			sources.push_back(
				std::make_unique<sf::kernels::CLFRFileReader>(config.input));
	}
}

void _initialize_sinks(const config& config, snk_container_t& sinks)
{
	for (unsigned i = 0; i < config.parallelization_factor; i ++)
		sinks.push_back(
			std::make_unique<sf::kernels::BenchmarkPrinter<sf::types::CLFR>>(std::to_string(i)));
}

void _initialize_batch_sinks(const config& config, snk_batch_container_t& sinks)
{
	for (unsigned i = 0; i < config.parallelization_factor; i ++)
		sinks.push_back(
			std::make_unique<sf::kernels::BenchmarkPrinter<std::vector<sf::types::CLFR>>>(
				std::to_string(i)));
}

int main(int argc, char** argv)
{
	unsigned long int entries = 0;
	unsigned long long int runtime = 0;

	src_container_t clfr_file_readers;
	snk_container_t benchmark_printers;
	snk_batch_container_t batch_benchmark_printers;

	raft::map m;

	config config = _parse_config(_set_options(), argc, argv);


	_initialize_sources(config, clfr_file_readers);

	if (config.batch_size > 1)
		_initialize_batch_sinks(config, batch_benchmark_printers);
	else
		_initialize_sinks(config, benchmark_printers);

	for (unsigned i = 0; i < config.parallelization_factor; i++) {
		if (config.batch_size > 1)
			m += *clfr_file_readers[i] >> *batch_benchmark_printers[i];
		else
			m += *clfr_file_readers[i] >> *benchmark_printers[i];
	}

	m.exe();

	for (unsigned i = 0; i < config.parallelization_factor; i++) {
		if (config.batch_size > 1) {
			batch_benchmark_printers[i]->done();
			entries += batch_benchmark_printers[i]->total();
			runtime = batch_benchmark_printers[0]->runtime_ms();
		} else {
			benchmark_printers[i]->done();
			entries += benchmark_printers[i]->total();
			runtime = benchmark_printers[0]->runtime_ms();
		}
	}

	std::cout << (entries * config.batch_size) / (runtime / 1000) << std::endl;

	return 0;
}
