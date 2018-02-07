
#ifndef STARFLOW_ANALYTICS_MODULES_COUNTER_H
#define STARFLOW_ANALYTICS_MODULES_COUNTER_H

#include <map>
#include <vector>

namespace starflow {
	namespace modules {
		template<typename AggregationType>
		class Counter
		{
		public:
			std::pair<AggregationType, unsigned long long>
			operator+=(std::pair<AggregationType, unsigned long long> pair) {

				auto i = _counts.find(pair.first);

				if (i == std::end(_counts)) {
					return *(_counts.insert(pair).first);
				} else {
					i->second += pair.second;
					return *i;
				}
			}

			std::vector<std::pair<AggregationType, unsigned long long>>
			operator+=(std::vector<std::pair<AggregationType, unsigned long long>> pair_vector)
			{
				std::vector<std::pair<AggregationType, unsigned long long>> v;
				for (auto p : pair_vector)
					v.push_back(operator+=(p));
				return v;
			};

		private:
			std::map<AggregationType, unsigned long long> _counts;
		};
	}
}

#endif
