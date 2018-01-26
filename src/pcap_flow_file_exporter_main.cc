
#include <iostream>
#include <iomanip>
#include <fstream>

#include "proto/starflow.pb.h"
#include "kernels/flow_table.h"
#include "kernels/raw_packet_parser.h"
#include "kernels/pcap_file_reader.h"
#include "kernels/clfr_file_exporter.h"

#include <cxxopts/cxxopts.h>

struct options {
	std::string input;
	std::string output;
	std::string encapsulation;
	bool verbose;
};

void _print_help(cxxopts::Options& opts, int exit_code = 0)
{
	(exit_code == 0 ? std::cout : std::cerr) << opts.help({""}) << std::endl;
	exit(exit_code);
}

const options _parse_cli_options(int argc, char** argv)
{
	options parse_result { };

	try {
		cxxopts::Options opts("flow_file_exporter",
							  " - Generates a file containing all CLFRs from a given PCAP file.");

		opts.add_options()
			("i,input", "PCAP input file (required)", cxxopts::value<std::string>(), "FILE")
			("o,output", "CLFR output file (required)", cxxopts::value<std::string>(), "FILE")
			("e,encapsulation", "PCAP outer header, default: eth",
			 cxxopts::value<std::string>(), "eth|ip")
			("v,verbose", "print status messages")
			("h,help", "print this help message");

		auto parsed_opts = opts.parse(argc, argv);

		if (parsed_opts.count("h"))
			_print_help(opts);

		if (parsed_opts.count("i"))
			parse_result.input = parsed_opts["i"].as<std::string>();
		else
			_print_help(opts, 1);

		if (parsed_opts.count("o"))
			parse_result.output = parsed_opts["o"].as<std::string>();
		else
			_print_help(opts, 1);

		if (parsed_opts.count("e")) {
			std::string e = parsed_opts["e"].as<std::string>();

			if (e == "eth" || e == "ip")
				parse_result.encapsulation = e;
			else
				_print_help(opts, 1);
		} else {
			parse_result.encapsulation = "eth";
		}

		parse_result.verbose = parsed_opts.count("v") >= 1;

	} catch (const cxxopts::OptionException& e) {
		std::cerr << e.what() << std::endl;
	}

	return parse_result;
}

int main(int argc, char** argv)
{
	namespace sf = starflow;
	const auto options = _parse_cli_options(argc, argv);

	auto hdr_type = options.encapsulation == "ip" ?
					sf::kernels::PCAPFileReader::outer_header_type::ip
					: sf::kernels::PCAPFileReader::outer_header_type::eth;

	sf::kernels::PCAPFileReader pcap_file_reader(options.input, hdr_type);
	sf::kernels::FlowTable flow_table {};
	sf::kernels::CLFRFileExporter clfr_file_exporter(options.output, options.verbose);

	raft::map m;
	m += pcap_file_reader >> flow_table >> clfr_file_exporter;
	m.exe();

	return 0;
}
