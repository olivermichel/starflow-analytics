
#ifndef STARFLOW_ANALYTICS_KERNELS_CLFR_FILE_IMPORTER_H
#define STARFLOW_ANALYTICS_KERNELS_CLFR_FILE_IMPORTER_H

#include <raft>
#include <string>
#include <fstream>

#include "../proto/starflow.pb.h"
#include "../modules/clfr_file_reader.h"

namespace starflow {
	namespace kernels {
		class CLFRFileReader : public raft::kernel
		{
		public:
			explicit CLFRFileReader(const std::string& file_name);
			raft::kstatus run() override;
			~CLFRFileReader() override;
		private:
			modules::CLFRFileReader _clfr_file_reader;
		};
	}
}

#endif
