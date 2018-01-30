
#ifndef STARFLOW_ANALYTICS_MODULES_EWMA_H
#define STARFLOW_ANALYTICS_MODULES_EWMA_H

#include <map>

namespace starflow {
	namespace modules {
		template<typename aggregate_by_t>
		class EWMA
		{
		public:
			unsigned long long operator()(aggregate_by_t k, unsigned long long v) {
				double alpha = 0.1;
				auto i = _ewmas.find(k);

				if (i == std::end(_ewmas))
					return (_ewmas.insert(std::make_pair(k, v)).first)->second;
				else {
					i->second = (1 - alpha) * i->second + alpha * v;
					return i->second;
				}
			}

		private:
			std::map<aggregate_by_t, unsigned long long> _ewmas;
		};
	}
}

#endif
