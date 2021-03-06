
#ifndef STARFLOW_ANALYTICS_KEY_H
#define STARFLOW_ANALYTICS_KEY_H

#include <cstdint>
#include <netinet/in.h>
#include <string>

#include "../proto/starflow.pb.h"

namespace starflow {
	namespace types {

		using ip_proto_t = std::uint8_t;
		using ip_addr_t  = std::uint32_t;
		using tp_port_t  = std::uint16_t;

		class Key
		{
		public:
			Key()                      = default;
			Key(const Key&)            = default;
			Key& operator=(const Key&) = default;
			Key(Key&&)                 = default;
			Key& operator=(Key&&)      = default;

			explicit Key(const proto::key&);

			proto::key to_proto() const;

			Key(u_char ip_p, in_addr ip_src, in_addr ip_dst, unsigned short th_sport,
				unsigned short th_dport);

			Key(std::uint8_t ip_p, std::uint32_t ip_src, std::uint32_t ip_dst,
				std::uint16_t th_sport, std::uint16_t th_dport);

			std::string str_desc() const;

			bool operator==(const Key& b) const;
			bool operator<(const Key& b) const;

			std::uint8_t  ip_proto = 0;
			std::uint32_t ip_src   = 0;
			std::uint32_t ip_dst   = 0;
			std::uint16_t th_sport = 0;
			std::uint16_t th_dport = 0;

			virtual ~Key() = default;
		};
	}
}

#endif
