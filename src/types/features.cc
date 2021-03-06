
#include "features.h"

starflow::types::Features::tcp_flags_t::tcp_flags_t()
	: _flags(0) { }

starflow::types::Features::tcp_flags_t::tcp_flags_t(unsigned char flags)
	: _flags(flags) { }

starflow::types::Features::tcp_flags_t::tcp_flags_t(const starflow::proto::tcp_flags& f)
	: _flags(static_cast<unsigned char>(f.flags())) { }

std::string starflow::types::Features::tcp_flags_t::str_desc() const
{
	std::string desc = "starflow::types::Features::tcp_flags_t(";
	desc += is_urg() ? " urg" : "";
	desc += is_ack() ? " ack" : "";
	desc += is_psh() ? " psh" : "";
	desc += is_rst() ? " rst" : "";
	desc += is_syn() ? " syn" : "";
	desc += is_fin() ? " fin" : "";
	return desc + " )";
}

bool starflow::types::Features::tcp_flags_t::operator==(
	const starflow::types::Features::tcp_flags_t& other) const
{
	return _flags == other._flags;
}

bool starflow::types::Features::tcp_flags_t::operator!=(
	const starflow::types::Features::tcp_flags_t& other) const
{
	return _flags != other._flags;
}

starflow::proto::tcp_flags starflow::types::Features::tcp_flags_t::to_proto() const
{
	proto::tcp_flags f;
	f.set_flags(static_cast<int>(_flags));
	return f;
}

starflow::types::Features::Features(const starflow::proto::features& f)
	: ip_id((std::uint16_t)f.ip_id()),
	  tcp_seq(f.tcp_seq()),
	  tcp_pl_len(f.tcp_pl_len()),
	  tcp_flags(f.tcp_flags()) { }

bool starflow::types::Features::operator==(const starflow::types::Features& other) const
{
	return ip_id      == other.ip_id
		&& tcp_seq    == other.tcp_seq
	    && tcp_pl_len == other.tcp_pl_len
	    && tcp_flags  == other.tcp_flags;
}

bool starflow::types::Features::operator!=(const starflow::types::Features& other) const
{
	return ip_id      != other.ip_id
		|| tcp_seq    != other.tcp_seq
	    || tcp_pl_len != other.tcp_pl_len
	    || tcp_flags  != other.tcp_flags;
}

starflow::proto::features starflow::types::Features::to_proto() const
{
	proto::features proto_features;
	proto_features.set_ip_id(ip_id);
	proto_features.set_tcp_seq(tcp_seq);
	proto_features.set_tcp_pl_len(tcp_pl_len);
	proto::tcp_flags* proto_flags = new proto::tcp_flags(tcp_flags.to_proto());
	proto_features.set_allocated_tcp_flags(proto_flags);
	return proto_features;
}
