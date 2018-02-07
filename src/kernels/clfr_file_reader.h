
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
			using _out_t = types::CLFR;
			using _batch_out_t = std::vector<types::CLFR>;

			explicit CLFRFileReader(const std::string& file_name, unsigned batch_size = 0);
			raft::kstatus run() override;
			~CLFRFileReader() override;
		private:
			modules::CLFRFileReader _clfr_file_reader;
			unsigned _batch_size;
		};
	}
}

#endif
