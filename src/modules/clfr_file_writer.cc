
#include "clfr_file_writer.h"

starflow::modules::CLFRFileWriter::CLFRFileWriter(const std::string& file_name, bool debug,
	std::ostream& debug_os) throw (std::runtime_error) : _buf(), _debug(debug), _debug_os(debug_os)
{
	_ofs.open(file_name, std::ios::out | std::ios::binary | std::ios::trunc);

	if (!_ofs.is_open())
		throw std::runtime_error("CLFRFileWriter: could not open file.");

	_raw_out = new google::protobuf::io::OstreamOutputStream(&_ofs);
	_coded_out = new google::protobuf::io::CodedOutputStream(_raw_out);
}

void starflow::modules::CLFRFileWriter::write_clfr(const starflow::types::CLFR& clfr)
	throw (std::logic_error)
{
	proto::clfr proto_clfr = clfr.to_proto();
	proto_clfr.SerializeToString(&_buf);
	_obj_len = (unsigned int) _buf.size();

	if (_obj_len == 0)
		throw std::logic_error("CLFRFileWriter: write_clfr: zero object size");

	_coded_out->WriteVarint32(_obj_len);
	_coded_out->WriteString(_buf);

	_total_bytes += (_coded_out->VarintSize32(_obj_len) + _obj_len);
	_total_clfrs++;

	if (_debug && _total_clfrs % 10000 == 0)
		_write_debug_out();
}

void starflow::modules::CLFRFileWriter::close()
{
	_coded_out->WriteVarint32(0);

	_ofs.close();
	_closed = true;

	if (_debug)
		_write_debug_out();
}

starflow::modules::CLFRFileWriter::~CLFRFileWriter()
{
	if (!_closed)
		close();

	delete _coded_out;
	delete _raw_out;
}

void starflow::modules::CLFRFileWriter::_write_debug_out() const
{
	_debug_os << _total_clfrs << " CLFRS, " << _total_bytes << " Bytes" << std::endl;
}
