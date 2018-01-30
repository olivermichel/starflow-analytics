
#include "pcap_file_reader.h"
#include <chrono>

starflow::modules::PCAPFileReader::PCAPFileReader(const std::string& file_name, bool outer_eth)
	throw (std::runtime_error)
	: _pcap(_open(file_name)),
	  _end(_peek()),
	  _outer_eth(outer_eth),
	  _random_device(),
	  _random_generator(_random_device()) { }

void starflow::modules::PCAPFileReader::next(starflow::types::Key& key, starflow::types::Packet& pkt)
	throw (std::runtime_error)
{
	_end = _peek();

	starflow::types::Packet::parse(_pl_buf, key, pkt, _outer_eth);

	std::normal_distribution<> delay_dist(500, 100);
	std::normal_distribution<> qlen_dist(10, 3);

	pkt.ts_in_us  = ((std::uint64_t)_hdr->ts.tv_sec * 1000000 + (std::uint64_t)_hdr->ts.tv_usec);
	pkt.ts_out_us = pkt.ts_in_us + (std::uint64_t) delay_dist(_random_generator);
	pkt.qid = 1;
	pkt.qlen = (std::uint16_t) qlen_dist(_random_generator);
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
