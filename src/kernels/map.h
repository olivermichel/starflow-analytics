
#ifndef STARFLOW_ANALYTICS_MAP_H
#define STARFLOW_ANALYTICS_MAP_H

#include <raft>

namespace starflow {
	namespace kernels {
		template<typename I, typename O>
		class Map : public raft::kernel
		{
		public:
			explicit Map(std::function<O (I)> map_function)
				: _map_function(map_function)
			{
				input.template add_port<I>("in");
				output.template add_port<O>("out");
			}

			raft::kstatus run() override
			{
				I i;
				input["in"].pop(i);
				output["out"].push(_map_function(i));
				return raft::proceed;
			}

		private:
			std::function<O (I)> _map_function;
		};
	}
}

#endif
