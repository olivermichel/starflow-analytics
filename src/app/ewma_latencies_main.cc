
#include <raft>

#include "../etc/format_helpers.h"
#include "../kernels/benchmark_printer.h"
#include "../kernels/clfr_file_reader.h"
#include "../kernels/formatted_printer.h"
#include "../kernels/group_by.h"
#include "../modules/ewma.h"

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
	using ewma_per_flow_t = std::pair<sf::types::Key, std::vector<unsigned long long>>;

	sf::modules::EWMA<sf::types::Key> ewma;
	sf::kernels::CLFRFileReader clfr_file_reader(config.input);

	sf::kernels::GroupBy<sf::types::CLFR, ewma_per_flow_t> flow_ewma_calculator(
		[&ewma](const sf::types::CLFR& clfr){
			std::vector<unsigned long long> v;
			v.reserve(clfr.n_packets());

			for (const auto& packet : clfr.packets())
				v.push_back(ewma(clfr.key(), packet.ts_out_us - packet.ts_in_us));

			return std::make_pair(clfr.key(), v);
	});

	sf::kernels::FormattedPrinter<ewma_per_flow_t> printer(
		[](std::ostream& os, const ewma_per_flow_t& p) {
			os << p.first.str_desc() << " ->";
			for (auto v : p.second)
				os << " " << v;
			os << std::endl;
		});

	sf::kernels::BenchmarkPrinter<ewma_per_flow_t> benchmark;

	raft::map m;

	if (config.benchmark)
		m += clfr_file_reader >> flow_ewma_calculator >> benchmark;
	else
		m += clfr_file_reader >> flow_ewma_calculator >> printer;

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
