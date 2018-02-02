
#ifndef STARFLOW_ANALYTICS_KERNELS_BENCHMARK_PRINTER_H
#define STARFLOW_ANALYTICS_KERNELS_BENCHMARK_PRINTER_H

#include <raft>

#include "../etc/timer.h"

namespace starflow {
	namespace kernels {

		template<typename T>
		class BenchmarkPrinter : public raft::kernel
		{
		public:
			explicit BenchmarkPrinter()
				: _timer([this](unsigned long long i) { return _tick(); }, std::chrono::seconds(1))
			{
				input.template add_port<T>("in");
				_timer();
			}

			raft::kstatus run() override
			{
				T t;
				input["in"].pop(t);
				_count++;
				return raft::proceed;
			}

			bool _tick()
			{
				std::cout << _count << std::endl;
				_count = 0;
				return true;
			}

		private:
			etc::Timer _timer;
			unsigned long _count = 0;
		};

	}
}


#endif
