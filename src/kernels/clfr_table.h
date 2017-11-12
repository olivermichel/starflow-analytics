
#ifndef STARFLOW_ANALYTICS_FLOW_TABLE_H
#define STARFLOW_ANALYTICS_FLOW_TABLE_H

#include <raft>
#include <pcap.h>
#include "../types/key.h"
#include "../types/clfr.h"

namespace starflow {
	namespace kernels {
		class CLFRTable : public raft::kernel
		{
		public:
			CLFRTable();
			raft::kstatus run() override;

		private:
			std::map<types::Key, types::CLFR> _flows;

			std::chrono::microseconds _to_check_interval = std::chrono::seconds(10);
			std::chrono::microseconds _udp_to            = std::chrono::seconds(30);
			std::chrono::microseconds _last_to_check     = std::chrono::seconds(0);

			unsigned long long _n_packets = 0;

			void _check_timeouts(std::chrono::microseconds trigger_ts);
			void _evict_flow(const types::Key& k, std::chrono::microseconds evict_ts,
							 bool complete = true);
		};
	}
}

#endif
