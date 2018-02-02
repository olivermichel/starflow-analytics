
#ifndef STARFLOW_ANALYTICS_KERNELS_BENCHMARK_PRINTER_H
#define STARFLOW_ANALYTICS_KERNELS_BENCHMARK_PRINTER_H

#include <raft>
#include <numeric>

#include "../etc/timer.h"

namespace starflow {
	namespace kernels {

		template<typename T>
		class BenchmarkPrinter : public raft::kernel
		{
		public:
			explicit BenchmarkPrinter(std::chrono::seconds interval = std::chrono::seconds(1),
									  bool show_status = false, bool use_timer = false)
				: _interval(interval),
				  _show_status(show_status),
				  _use_timer(use_timer),
				  _timer([this](unsigned long long i) { return _tick(); }, interval)
			{
				input.template add_port<T>("in");

				if (_use_timer)
					_timer();
			}

			raft::kstatus run() override
			{
				if (_counts.empty() && _current == 0)
					_start = _int_start = std::chrono::steady_clock::now();

				auto now = std::chrono::steady_clock::now();
				auto dur = std::chrono::duration_cast<std::chrono::milliseconds>(now - _int_start);

				if (dur > _interval) {
					if (_show_status)
						std::cout << '.' << std::flush;
					_counts.push_back(_current);
					_current = 0;
					_int_start = now;
				}

				T t;
				input["in"].pop(t);
				_current++;

				return raft::proceed;
			}

			bool _tick()
			{
				if (_show_status)
					std::cout << '.' << std::flush;

				_current = 0;
				return true;
			}

			void done()
			{
				std::cout << std::endl;
				_end = std::chrono::steady_clock::now();
				_counts.push_back(_current);
				_done = true;
			}

			inline const std::vector<unsigned long>& counts() const
			{
				return _counts;
			}

			unsigned long long runtime_ms() const
			{
				return (unsigned)
					std::chrono::duration_cast<std::chrono::milliseconds>(_end - _start).count();
			}

			inline const unsigned long total() const
			{
				return std::accumulate(std::begin(_counts), std::end(_counts), (unsigned) 0);
			}

			inline const double mean_per_interval() const
			{
				return total() / _counts.size();
			}

		private:
			std::chrono::milliseconds _interval;
			bool _show_status;
			bool _done = false;
			bool _use_timer;
			etc::Timer _timer;
			unsigned long _current = 0;
			std::vector<unsigned long> _counts = {};
			std::chrono::steady_clock::time_point _start;
			std::chrono::steady_clock::time_point _end;
			std::chrono::steady_clock::time_point _int_start;
		};
	}
}

#endif
