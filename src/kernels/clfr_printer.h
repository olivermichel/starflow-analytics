
#ifndef STARFLOW_ANALYTICS_KERNELS_CLFR_PRINTER_H
#define STARFLOW_ANALYTICS_KERNELS_CLFR_PRINTER_H

#include <raft>

namespace starflow {
	namespace kernels {
		class CLFRPrinter : public raft::kernel
		{
		public:
			CLFRPrinter() = delete;
			explicit CLFRPrinter(bool sink = false, std::ostream& os = std::cout);
			raft::kstatus run() override;

		private:
			bool _sink;
			std::ostream& _os;
		};
	}
}

#endif
