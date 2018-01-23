
#ifndef STARFLOW_ANALYTICS_MODULES_CLFR_FILE_WRITER_H
#define STARFLOW_ANALYTICS_MODULES_CLFR_FILE_WRITER_H

#include <fstream>
#include <google/protobuf/io/zero_copy_stream.h>
#include <google/protobuf/io/zero_copy_stream_impl.h>
#include <iostream>
#include <stdexcept>
#include <string>

#include "../types/clfr.h"

namespace starflow {
	namespace modules {
		class CLFRFileWriter
		{
		public:
			CLFRFileWriter() = delete;
			explicit CLFRFileWriter(const std::string& file_name, bool debug = false,
									std::ostream& debug_os = std::cout) throw (std::runtime_error);
			void write_clfr(const types::CLFR& clfr) throw (std::logic_error);
			inline unsigned long long total_bytes() const { return _total_bytes; }
			inline unsigned long long total_clfrs() const { return _total_clfrs; }
			void close();
			virtual ~CLFRFileWriter();

		private:
			std::string _buf;
			std::ofstream _ofs;
			google::protobuf::io::OstreamOutputStream* _raw_out;
			google::protobuf::io::CodedOutputStream* _coded_out;
			unsigned _obj_len = 0;
			bool _closed = false;
			bool _debug = false;
			std::ostream& _debug_os = std::cout;
			unsigned long long _total_bytes = 0;
			unsigned long long _total_clfrs = 0;

			void _write_debug_out() const;
		};
	}
}

#endif
