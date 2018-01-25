
#include "pcap_file_reader.h"
#include <chrono>

starflow::modules::PCAPFileReader::PCAPFileReader(const std::string& file_name, bool outer_eth)
	throw (std::runtime_error)
	: _pcap(_open(file_name)), _end(_peek()), _outer_eth(outer_eth) { }

void starflow::modules::PCAPFileReader::next(starflow::types::Key& key, starflow::types::Packet& pkt)
	throw (std::runtime_error)
{
	_end = _peek();

	starflow::types::Packet::parse(_pl_buf, key, pkt, _outer_eth);
	pkt.ts_in = std::chrono::microseconds(_hdr->ts.tv_sec*1000000 + _hdr->ts.tv_usec);
}

pcap* starflow::modules::PCAPFileReader::_open(const std::string& file_name)
	throw (std::runtime_error)
{
	pcap* pcap;
	char errbuf[PCAP_ERRBUF_SIZE];

	if ((pcap = pcap_open_offline(file_name.c_str(), errbuf)) == nullptr) {
		std::string msg = std::string("PCAPFileReader: could not open file: ") + errbuf;
		throw std::runtime_error(msg);
	}

	return pcap;
}

bool starflow::modules::PCAPFileReader::_peek()
	throw (std::runtime_error)
{
	int status = pcap_next_ex(_pcap, &_hdr, &_pl_buf);

	if (status == 1)
		return false;

	if (status == -2)
		return true;

	throw std::runtime_error("PCAPFileReader: error reading packet");
}

void starflow::modules::PCAPFileReader::_close()
{
	pcap_close(_pcap);
}

starflow::modules::PCAPFileReader::~PCAPFileReader()
{
	_close();
}
