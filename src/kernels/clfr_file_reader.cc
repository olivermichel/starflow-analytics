
#include "clfr_file_reader.h"

#include <iomanip>
#include "../types/clfr.h"


starflow::kernels::CLFRFileReader::CLFRFileReader(const std::string& file_name, unsigned batch_size)
	: _clfr_file_reader(file_name), _batch_size(batch_size)
{
	if (_batch_size > 0)
		output.add_port<_batch_out_t>("out");
	else
		output.add_port<_out_t>("out");
}

raft::kstatus starflow::kernels::CLFRFileReader::run()
{
	_batch_out_t v {};

	if (_clfr_file_reader.end())
		return raft::stop;

	types::CLFR clfr;

	if (_batch_size > 0) {
		for (unsigned i = 0; i < _batch_size; i++) {
			_clfr_file_reader.read_clfr(clfr);
			v.push_back(clfr);
		}
		output["out"].push(v);
	} else {
		_clfr_file_reader.read_clfr(clfr);
		output["out"].push(clfr);
	}

	return raft::proceed;
}

starflow::kernels::CLFRFileReader::~CLFRFileReader()
{
	_clfr_file_reader.close();
}
