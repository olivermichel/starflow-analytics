
#ifndef STARFLOW_ANALYTICS_MODULES_CLFR_FILE_WRITER_H
#define STARFLOW_ANALYTICS_MODULES_CLFR_FILE_WRITER_H

#include <fstream>
#include <google/protobuf/io/zero_copy_stream.h>
#include <google/protobuf/io/zero_copy_stream_impl.h>
#include <iostream>
#include <string>

#include "../types/clfr.h"

namespace starflow {
	namespace modules {
		class CLFRFileWriter
		{
		public:
			CLFRFileWriter() = delete;
			explicit CLFRFileWriter(const std::string& file_name, bool debug = false,
									std::ostream& debug_os = std::cout);
			void write_clfr(const types::CLFR& clfr);
			void close();
			virtual ~CLFRFileWriter();

		private:
			std::string _buf;
			std::fstream _ofs;
			unsigned _obj_len = 0;
			bool _closed = false;
			bool _debug = false;
			std::ostream& _debug_os = std::cout;
			google::protobuf::io::OstreamOutputStream* _raw_out = nullptr;
			google::protobuf::io::CodedOutputStream* _coded_out = nullptr;
			unsigned long long _total_bytes = 0;
			unsigned long long _total_clfrs = 0;
		};
	}
}

#endif
