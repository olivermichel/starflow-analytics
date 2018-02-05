
#ifndef STARFLOW_ANALYTICS_PACKET_H
#define STARFLOW_ANALYTICS_PACKET_H

#include <chrono>
#include <string>

#include "features.h"
#include "key.h"

namespace starflow {
	namespace types {
		class Packet
		{
		public:

			static bool parse(const unsigned char* buf, std::size_t len, Key& key, Packet& pkt,
							  bool outer_eth = true);

			Packet()                         = default;
			Packet(const proto::packet& p);
			Packet(std::uint64_t ts_in_us, std::uint64_t ts_out_us, std::uint16_t len);
			Packet(const Packet&)            = default;
			Packet& operator=(const Packet&) = default;
			Packet(Packet&&)                 = default;
			Packet& operator=(Packet&&)      = default;

			bool operator==(const Packet& other) const;
			bool operator!=(const Packet& other) const;

			std::uint64_t id(const Key& key) const;

			std::uint64_t ts_in_us;
			std::uint64_t ts_out_us;

			std::uint16_t len;
			std::uint16_t qid;
			std::uint16_t qlen;

			Features features;

			proto::packet to_proto() const;

			std::string str_desc() const;

			virtual ~Packet() = default;
		};
	}
}

#endif
