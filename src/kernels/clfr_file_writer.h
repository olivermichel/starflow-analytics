
#ifndef STARFLOW_ANALYTICS_KERNELS_CLFR_FILE_EXPORTER_H
#define STARFLOW_ANALYTICS_KERNELS_CLFR_FILE_EXPORTER_H

#include <raft>
#include <string>
#include <fstream>

#include "../proto/starflow.pb.h"
#include "../modules/clfr_file_writer.h"

namespace starflow {
	namespace kernels {
		class CLFRFileWriter : public raft::kernel
		{
		public:
			explicit CLFRFileWriter(const std::string& file_name, bool verbose = false);
			raft::kstatus run() override;
			~CLFRFileWriter() override;
		private:
			modules::CLFRFileWriter _clfr_file_writer;
			bool _verbose;
			std::chrono::time_point<std::chrono::steady_clock> _start1;
			std::chrono::time_point<std::chrono::steady_clock> _start2;
		};
	}
}

#endif
