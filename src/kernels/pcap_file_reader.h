
#ifndef STARFLOW_ANALYTICS_PCAP_FILE_READER_H
#define STARFLOW_ANALYTICS_PCAP_FILE_READER_H

#include <raft>
#include <pcap.h>

#include "../modules/pcap_file_reader.h"

namespace starflow {
	namespace kernels {
		class PCAPFileReader : public raft::kernel
		{
		public:
			enum class outer_header_type { eth, ip };
			explicit PCAPFileReader(const std::string& file_name,
									outer_header_type outer_eth = outer_header_type::eth);
			raft::kstatus run() override;

		private:
			modules::PCAPFileReader _pcap_reader;
		};
	}
}

#endif
