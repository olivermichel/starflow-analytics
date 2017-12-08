
#ifndef STARFLOW_ANALYTICS_CLFR_AGGREGATOR_H
#define STARFLOW_ANALYTICS_CLFR_AGGREGATOR_H

#include <raft>

namespace starflow {
	namespace kernels {

		class CLFRAggregator : public raft::kernel
		{
			CLFRAggregator(std::chrono::seconds bin_size = std::chrono::seconds(1));
			raft::kstatus run() override;
		};
	}
}


#endif
