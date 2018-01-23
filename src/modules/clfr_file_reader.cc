
#include "clfr_file_reader.h"

starflow::modules::CLFRFileReader::CLFRFileReader(const std::string& file_name, bool debug,
	std::ostream& debug_os) throw (std::runtime_error) : _buf(), _debug(debug), _debug_os(debug_os)
{
	_ifs.open(file_name, std::ios::in | std::ios::binary);

	if (!_ifs.is_open())
		throw std::runtime_error("CLFRFileReader: could not open file");

	_raw_in = new google::protobuf::io::IstreamInputStream(&_ifs);
	_coded_in = new google::protobuf::io::CodedInputStream(_raw_in);

	_peek(_obj_len);

	if (!_obj_len)
		throw std::runtime_error("CLFRFileReader: file is empty or could not be read");
}

void starflow::modules::CLFRFileReader::read_clfr(types::CLFR& clfr)
{
	_coded_in->ReadString(&_buf, _obj_len);
	proto::clfr proto_clfr;
	proto_clfr.ParseFromString(_buf);

	clfr = types::CLFR(proto_clfr);
	_total_clfrs++;

	_peek(_obj_len);

	if (_obj_len == 0)
		_end = true;
}

void starflow::modules::CLFRFileReader::close()
{
	_ifs.close();
	_closed = true;
}

starflow::modules::CLFRFileReader::~CLFRFileReader()
{
	if (!_closed)
		close();

	delete _coded_in;
	delete _raw_in;
}

void starflow::modules::CLFRFileReader::_peek(unsigned& next_len)
{
	_coded_in->ReadVarint32(&next_len);
}
