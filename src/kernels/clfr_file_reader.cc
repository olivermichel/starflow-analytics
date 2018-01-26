
#include "clfr_file_reader.h"

#include <iomanip>
#include "../types/clfr.h"


starflow::kernels::CLFRFileReader::CLFRFileReader(const std::string& file_name)
	: _clfr_file_reader(file_name)
{
	output.add_port<types::CLFR>("clfr_out");
}

raft::kstatus starflow::kernels::CLFRFileReader::run()
{
	if (_clfr_file_reader.end())
		return raft::stop;

	types::CLFR clfr;
	_clfr_file_reader.read_clfr(clfr);
	output["clfr_out"].push(clfr);
	return raft::proceed;
}

starflow::kernels::CLFRFileReader::~CLFRFileReader()
{
	_clfr_file_reader.close();
}
