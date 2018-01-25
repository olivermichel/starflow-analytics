
#include "packet.h"

#include <net/ethernet.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <netinet/tcp.h>
#include <netinet/udp.h>
#include <arpa/inet.h>

bool starflow::types::Packet::parse(const unsigned char* buf, Key& key, Packet& pkt, bool outer_eth)
{
	std::size_t pkt_offset = 0;

	struct ether_header* eth = nullptr;
	struct ip* ip = nullptr;
	struct tcphdr* tcp = nullptr;
	struct udphdr* udp = nullptr;

	if (outer_eth) {
		eth = (ether_header*) buf;
		pkt_offset += sizeof(struct ether_header);

		if (ntohs(eth->ether_type) != ETHERTYPE_IP)
			return false;
	}

	ip = (struct ip*) (buf + pkt_offset);
	pkt_offset += sizeof(struct ip);

	if (ip->ip_p == IPPROTO_TCP) {
		tcp = (struct tcphdr*) (buf + pkt_offset);
		key = {ip->ip_p, ip->ip_src, ip->ip_dst, ntohs(tcp->th_sport), ntohs(tcp->th_dport)};
		pkt.features.tcp_flags = starflow::types::Features::tcp_flags_t(tcp->th_flags);
		pkt.features.tcp_seq   = ntohl(tcp->th_seq);
	} else if (ip->ip_p == IPPROTO_UDP) {
		udp = (struct udphdr*) (buf + pkt_offset);
		key = {ip->ip_p, ip->ip_src, ip->ip_dst, ntohs(udp->uh_sport), ntohs(udp->uh_dport)};
	} else return false;

	pkt.len = ntohs(ip->ip_len)+(outer_eth ? sizeof(eth) : 0);

	pkt.ts_out = std::chrono::microseconds(0);
	pkt.qid  = 0;
	pkt.qlen = 0;

	return true;
}

starflow::types::Packet::Packet(std::chrono::microseconds ts_in, std::chrono::microseconds ts_out,
								unsigned int len)
	: ts_in(ts_in), ts_out(ts_out), len(len) { }

starflow::types::Packet::Packet(unsigned long ts_in, unsigned long ts_out, unsigned int len)
	: ts_in(ts_in), ts_out(ts_out), len(len) { }

starflow::types::Packet::Packet(const starflow::proto::packet& p)
	: ts_in(p.ts_in()),
	  ts_out(p.ts_out()),
	  len((unsigned) p.len()),
	  qid((unsigned) p.qid()),
	  qlen((unsigned) p.qlen()),
	  features(p.features()) { }

bool starflow::types::Packet::operator==(const Packet& other) const
{
	return ts_in    == other.ts_in
		&& ts_out   == other.ts_out
		&& len      == other.len
		&& qid      == other.qid
		&& qlen     == other.qlen
		&& features == other.features;
}

bool starflow::types::Packet::operator!=(const Packet& other) const
{
	return !operator==(other);
}

starflow::proto::packet starflow::types::Packet::to_proto() const
{
	starflow::proto::packet p;
	p.set_ts_in(ts_in.count());
	p.set_ts_out(ts_out.count());
	p.set_len(len);
	p.set_qid(qid);
	p.set_qlen(qlen);
	p.set_allocated_features(new proto::features(features.to_proto()));
	return p;
}

std::string starflow::types::Packet::str_desc() const
{
	std::string desc = "starflow::types::Packet(ts_in=" + std::to_string(ts_in.count())
					   + ", ts_out=" + std::to_string(ts_out.count())
					   + ", len=" + std::to_string(len) + ")";
	return desc;
}
