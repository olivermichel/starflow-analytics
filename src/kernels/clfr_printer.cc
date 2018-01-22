
#include "clfr_printer.h"

#include "../types/clfr.h"

starflow::kernels::CLFRPrinter::CLFRPrinter(bool sink, std::ostream& os)
	: _sink(sink), _os(os)
{
	input.add_port<types::CLFR>("in");
	if (!_sink)
		output.add_port<types::CLFR>("out");
}

raft::kstatus starflow::kernels::CLFRPrinter::run()
{
	types::CLFR clfr;
	input["in"].pop(clfr);

	_os << clfr.str_desc() << std::endl;

	if(!_sink)
		output["out"].push(clfr);

	return raft::proceed;
}
