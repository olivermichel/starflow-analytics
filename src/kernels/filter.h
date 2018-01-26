
#ifndef STARFLOW_ANALYTICS_KERNELS_FILTER_H
#define STARFLOW_ANALYTICS_KERNELS_FILTER_H

#include <raft>

namespace starflow {
	namespace kernels {
		template<typename T>
		class Filter : public raft::kernel
		{
		public:

			explicit Filter(std::function<bool(const T&)> filter_func)
				: _filter_func(filter_func)
			{
				input.template add_port<T>("in");
				output.template add_port<T>("out");
			}

			raft::kstatus run() override
			{
				T t;
				input["in"].pop(t);

				if (_filter_func(t))
					output["out"].push(t);

				return raft::proceed;
			}

		private:
			std::function<bool(const T&)> _filter_func;
		};
	}
}

#endif
