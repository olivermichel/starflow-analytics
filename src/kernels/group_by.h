
#ifndef STARFLOW_ANALYTICS_KERNELS_GROUP_BY_H
#define STARFLOW_ANALYTICS_KERNELS_GROUP_BY_H

#include <raft>
#include "../types/clfr.h"

namespace starflow {
	namespace kernels {
		template<typename I, typename O>
		class GroupBy : public raft::kernel
		{
		public:
			explicit GroupBy(std::function<O (const I&)>&& aggregator)
				: _aggregator(aggregator)
			{
				input.template add_port<I>("in");
				output.template add_port<O>("out");
			}

			raft::kstatus run() override
			{
				I i;
				input["in"].pop(i);
				auto pair = _aggregator(i);
				output["out"].push(pair);
				return raft::proceed;
			}

		private:
			std::function<O (const I&)> _aggregator;
		};
	}
}

#endif
