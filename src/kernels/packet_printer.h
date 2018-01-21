
#ifndef STARFLOW_ANALYTICS_KERNELS_PACKET_PRINTER_H
#define STARFLOW_ANALYTICS_KERNELS_PACKET_PRINTER_H

#include <raft>

namespace starflow {
	namespace kernels {
		class PacketPrinter : public raft::kernel
		{
		public:
			PacketPrinter() = delete;
			explicit PacketPrinter(bool sink = false, std::ostream& os = std::cout);
			raft::kstatus run() override;

		private:
			bool _sink;
			std::ostream& _os;
		};
	}
}

#endif
