
#include <iomanip>

#include "clfr_file_exporter.h"
#include "../types/clfr.h"


starflow::kernels::CLFRFileExporter::CLFRFileExporter(const std::string& file_name, bool verbose)
	: _ofs(file_name),
	  _proto_flows(),
	  _verbose(verbose),
	  _start1(std::chrono::steady_clock::now()),
	  _start2(std::chrono::steady_clock::now())
{
	input.add_port<types::CLFR>("clfr_in");
}

raft::kstatus starflow::kernels::CLFRFileExporter::run()
{
	types::CLFR clfr;
	input["clfr_in"].pop(clfr);

	proto::clfr* proto_clfr = _proto_flows.add_flows();
	*proto_clfr = clfr.to_proto();

	int flows_exported = _proto_flows.flows_size();

	if (_verbose && flows_exported % 1000 == 0) {

		auto now = std::chrono::steady_clock::now();
		auto dur = std::chrono::duration_cast<std::chrono::milliseconds>(now - _start2);

		std::cout << std::setw(9) << flows_exported << " flows, " << std::setw(7) << std::fixed
				  << std::setprecision(2) << (double) _proto_flows.ByteSize() / 1048576 << " MB, "
				  << std::setw(4) << dur.count() << " ms" << std::endl;

		_start2 = now;
	}

	return raft::proceed;
}

starflow::kernels::CLFRFileExporter::~CLFRFileExporter()
{
	_proto_flows.SerializePartialToOstream(&_ofs);
	_ofs.close();

	if (_verbose) {
		auto now = std::chrono::steady_clock::now();
		auto dur = std::chrono::duration_cast<std::chrono::seconds>(now - _start1);
		std::cout << "exported " << _proto_flows.flows_size() << " flows in "
				  << dur.count() << "s" << std::endl;
	}
}
