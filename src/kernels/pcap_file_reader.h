
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

			using _out_t = std::pair<starflow::types::Key, starflow::types::Packet>;
			using _batch_out_t
				= std::vector<std::pair<starflow::types::Key, starflow::types::Packet>>;

			enum class outer_header_type { eth, ip };
			explicit PCAPFileReader(const std::string& file_name,
									outer_header_type outer_eth = outer_header_type::eth,
									unsigned batch_size = 0);
			raft::kstatus run() override;

		private:
			modules::PCAPFileReader _pcap_reader;
			unsigned _batch_size;
		};
	}
}

#endif
