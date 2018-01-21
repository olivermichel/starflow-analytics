
#ifndef STARFLOW_ANALYTICS_MODULES_FILTER_H
#define STARFLOW_ANALYTICS_MODULES_FILTER_H

#include <functional>

namespace starflow {
	namespace modules {
		template<typename T>
		class Filter
		{
		public:
			explicit Filter(std::function<bool (T)> filter) : _filter(filter) { }
			bool operator()(const T& t) { return _filter(t); }

		private:
			std::function<bool (T)> _filter;
		};
	}
}

#endif
