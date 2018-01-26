
#include <iomanip>

#include "clfr_file_writer.h"
#include "../types/clfr.h"


starflow::kernels::CLFRFileWriter::CLFRFileWriter(const std::string& file_name, bool verbose)
	: _clfr_file_writer(file_name),
	  _verbose(verbose),
	  _start1(std::chrono::steady_clock::now()),
	  _start2(std::chrono::steady_clock::now())
{
	input.add_port<types::CLFR>("clfr_in");
}

raft::kstatus starflow::kernels::CLFRFileWriter::run()
{
	types::CLFR clfr;
	input["clfr_in"].pop(clfr);

	_clfr_file_writer.write_clfr(clfr);

	if (_verbose && _clfr_file_writer.total_clfrs() % 1000 == 0) {

		auto now = std::chrono::steady_clock::now();
		auto dur = std::chrono::duration_cast<std::chrono::milliseconds>(now - _start2);

		std::cout << std::setw(9) << _clfr_file_writer.total_clfrs() << " flows, " << std::setw(7)
				  << std::fixed << std::setprecision(2)
				  << (double) _clfr_file_writer.total_bytes() / 1048576 << " MB, " << std::setw(4)
				  << dur.count() << " ms" << std::endl;

		_start2 = now;
	}

	return raft::proceed;
}

starflow::kernels::CLFRFileWriter::~CLFRFileWriter()
{
	_clfr_file_writer.close();

	if (_verbose) {
		auto now = std::chrono::steady_clock::now();
		auto dur = std::chrono::duration_cast<std::chrono::seconds>(now - _start1);
		std::cout << "exported " << _clfr_file_writer.total_clfrs() << " flows in "
				  << dur.count() << "s" << std::endl;
	}
}
