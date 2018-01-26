
#ifndef STARFLOW_ANALYTICS_KERNELS_FORMATTED_PRINTER_H
#define STARFLOW_ANALYTICS_KERNELS_FORMATTED_PRINTER_H

#include <raft>
#include <iostream>

namespace starflow {
	namespace kernels {
		template<typename T>
		class FormattedPrinter : public raft::kernel
		{
		public:
			explicit FormattedPrinter(std::function<void (std::ostream&, const T&)>&& format_func,
			std::ostream& os = std::cout, bool forward = false)
				: _format_func(format_func), _os(os), _forward(forward)
			{
				input.template add_port<T>("in");
				if (_forward)
					output.template add_port<T>("out");
			}

			raft::kstatus run() override
			{
				T t;
				input["in"].pop(t);

				_format_func(_os, t);

				if (_forward)
					output["out"].push(t);

				return raft::proceed;
			}

		private:
			std::function<void (std::ostream&, const T&)> _format_func;
			std::ostream& _os;
			bool _forward;
		};
	}
}

#endif
