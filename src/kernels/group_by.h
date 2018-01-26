
#ifndef STARFLOW_ANALYTICS_KERNELS_GROUP_BY_H
#define STARFLOW_ANALYTICS_KERNELS_GROUP_BY_H

#include <raft>
#include "../types/clfr.h"

namespace starflow {
	namespace kernels {
		template<typename O>
		class GroupBy : public raft::kernel
		{
		public:
			explicit GroupBy(std::function<O (const starflow::types::CLFR&)>&& aggregator)
				: _aggregator(aggregator)
			{
				input.add_port<starflow::types::CLFR>("clfr_in");
				output.template add_port<O>("out");
			}

			raft::kstatus run() override
			{
				starflow::types::CLFR clfr;
				input["clfr_in"].pop(clfr);
				auto pair = _aggregator(clfr);
				output["out"].push(pair);
				return raft::proceed;
			}

		private:
			std::function<O (const starflow::types::CLFR&)> _aggregator;
		};
	}
}

#endif
