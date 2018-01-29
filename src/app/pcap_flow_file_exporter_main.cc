
#include <iostream>
#include <iomanip>
#include <fstream>

#include "../proto/starflow.pb.h"
#include "../kernels/flow_table.h"
#include "../kernels/raw_packet_parser.h"
#include "../kernels/pcap_file_reader.h"
#include "../kernels/clfr_file_writer.h"

#include <cxxopts/cxxopts.h>

struct options {
	std::string input;
	std::string output;
	std::string encapsulation;
	bool verbose;
};

void _print_help(cxxopts::Options& opts, int exit_code = 0)
{
	if (exit_code == 0)
		std::cout << opts.help({""}) << std::endl;
	else
		std::cerr << opts.help({""}) << std::endl;

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

	unsigned long long pkt_count = 0, clfr_count = 0;
	auto eth = (options.encapsulation == "eth");

	starflow::modules::PCAPFileReader pcap_reader(options.input, eth);
	starflow::modules::FlowTable flow_table;
	starflow::modules::CLFRFileWriter clfr_file_writer(options.output, false);
	flow_table.set_callback([&clfr_file_writer, &clfr_count](starflow::types::CLFR clfr) {
		clfr_count++;
		clfr_file_writer.write_clfr(clfr);
	});

	auto start = std::chrono::steady_clock::now();

	while (!pcap_reader.end()) {
		starflow::types::Key k;
		starflow::types::Packet p;
		pcap_reader.next(k, p);
		flow_table.add_packet(k, p);
		pkt_count++;

		if (options.verbose && pkt_count % 100000 == 0)
			std::cout << '.' << std::flush;
	}

	auto end = std::chrono::steady_clock::now();
	auto dur = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

	if (options.verbose)
		std::cout << std::endl << pkt_count << " pkts, " << clfr_count << " CLFRs in "
				  << ((double) dur.count()) / 1000  << "s" << std::endl;

	clfr_file_writer.close();
	return 0;
}
