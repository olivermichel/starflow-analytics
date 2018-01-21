
#ifndef STARFLOW_ANALYTICS_KERNELS_PACKET_FILTER_H
#define STARFLOW_ANALYTICS_KERNELS_PACKET_FILTER_H

#include <raft>
#include <functional>

#include "../types/key.h"
#include "../types/packet.h"
#include "../modules/filter.h"

namespace starflow {
	namespace kernels {

		class PacketFilter : public raft::kernel
		{
			using packet_filter_t = std::function<bool (std::pair<types::Key, types::Packet>)>;

		public:
			explicit PacketFilter(packet_filter_t&& filter_function);
			raft::kstatus run() override;

		private:
			modules::Filter<std::pair<types::Key, types::Packet>> _filter;
		};
	}
}

#endif
