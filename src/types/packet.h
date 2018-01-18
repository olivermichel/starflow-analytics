
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
			//TODO:	proto::packet to_proto() const;
			//TODO:	static Packet from_proto(const proto::packet);

			Packet()                         = default;
			Packet(std::chrono::microseconds ts, unsigned int len);
			Packet(const proto::packet& p);
			Packet(unsigned long ts, unsigned int len);
			Packet(const Packet&)            = default;
			Packet& operator=(const Packet&) = default;
			Packet(Packet&&)                 = default;
			Packet& operator=(Packet&&)      = default;

			bool operator==(const Packet& other) const;
			bool operator!=(const Packet& other) const;

			std::chrono::microseconds ts;
			unsigned                  len;

			Features features;

			proto::packet to_proto() const;

			std::string str_desc() const;

			virtual ~Packet() = default;
		};
	}
}

#endif
