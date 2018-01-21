
#ifndef STARFLOW_ANALYTICS_KERNELS_CLFR_FILE_EXPORTER_H
#define STARFLOW_ANALYTICS_KERNELS_CLFR_FILE_EXPORTER_H

#include <raft>
#include <string>
#include <fstream>

#include "../proto/starflow.pb.h"

namespace starflow {
	namespace kernels {
		class CLFRFileExporter : public raft::kernel
		{
		public:
			explicit CLFRFileExporter(const std::string& file_name, bool verbose = false);
			raft::kstatus run() override;
			~CLFRFileExporter() override;
		private:
			std::ofstream _ofs;
			proto::flow_list _proto_flows;
			bool _verbose;
			std::chrono::time_point<std::chrono::steady_clock> _start1;
			std::chrono::time_point<std::chrono::steady_clock> _start2;
		};
	}
}

#endif
