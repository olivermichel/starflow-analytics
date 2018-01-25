
#ifndef STARFLOW_ANALYTICS_PACKET_H
#define STARFLOW_ANALYTICS_PACKET_H

#include <chrono>
#include <string>

#include "features.h"

namespace starflow {
	namespace types {
		class Packet
		{
		public:
			Packet()                         = default;
			Packet(std::chrono::microseconds ts_in, std::chrono::microseconds ts_out,
				   unsigned int len);
			Packet(const proto::packet& p);
			Packet(unsigned long ts_in, unsigned long ts_out, unsigned int len);
			Packet(const Packet&)            = default;
			Packet& operator=(const Packet&) = default;
			Packet(Packet&&)                 = default;
			Packet& operator=(Packet&&)      = default;

			bool operator==(const Packet& other) const;
			bool operator!=(const Packet& other) const;

			std::chrono::microseconds ts_in;
			std::chrono::microseconds ts_out;
			unsigned                  len;
			unsigned                  qid  = 0;
			unsigned                  qlen = 0;

			Features features;

			proto::packet to_proto() const;

			std::string str_desc() const;

			virtual ~Packet() = default;
		};
	}
}

#endif
