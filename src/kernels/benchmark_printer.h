
#ifndef STARFLOW_ANALYTICS_KERNELS_BENCHMARK_PRINTER_H
#define STARFLOW_ANALYTICS_KERNELS_BENCHMARK_PRINTER_H

#include <raft>
#include <numeric>
#include <stdexcept>

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
				if (_show_status)
					std::cout << std::endl;

				_end = std::chrono::steady_clock::now();
				_counts.push_back(_current);
				_done = true;
			}

			inline const std::vector<unsigned long>& counts() const throw (std::logic_error)
			{
				if (!_done)
					throw std::logic_error("BenchmarkPrinter: collection not yet finalized");

				return _counts;
			}

			unsigned long long runtime_ms() const throw (std::logic_error)
			{
				if (!_done)
					throw std::logic_error("BenchmarkPrinter: collection not yet finalized");

				return (unsigned)
					std::chrono::duration_cast<std::chrono::milliseconds>(_end - _start).count();
			}

			inline const unsigned long total() const throw (std::logic_error)
			{
				if (!_done)
					throw std::logic_error("BenchmarkPrinter: collection not yet finalized");

				return std::accumulate(std::begin(_counts), std::end(_counts), (unsigned) 0);
			}

			inline const double mean_per_interval() const throw (std::logic_error)
			{
				if (!_done)
					throw std::logic_error("BenchmarkPrinter: collection not yet finalized");

				return total() / _counts.size();
			}

			void print_results(std::ostream& os = std::cout) const throw (std::logic_error)
			{
				if (!_done)
					throw std::logic_error("BenchmarkPrinter: collection not yet finalized");

				os  << _counts.front() << " " << total() << " " << runtime_ms() << " "
					<< mean_per_interval() << std::endl;

				for (unsigned i = 1; i < _counts.size(); i++)
					os << _counts[i] << std::endl;
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
