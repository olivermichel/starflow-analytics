
#ifndef STARFLOW_ANALYTICS_MODULES_CLFR_FILE_READER_H
#define STARFLOW_ANALYTICS_MODULES_CLFR_FILE_READER_H

#include <fstream>
#include <google/protobuf/io/zero_copy_stream.h>
#include <google/protobuf/io/zero_copy_stream_impl.h>
#include <iostream>
#include <stdexcept>
#include <string>

#include "../types/clfr.h"

namespace starflow {
	namespace modules {
		class CLFRFileReader
		{
		public:
			CLFRFileReader() = delete;
			explicit CLFRFileReader(const std::string& file_name, bool debug = false,
									std::ostream& debug_os = std::cout) throw (std::runtime_error);
			inline bool end() const { return _end; }
			void read_clfr(types::CLFR& clfr);
			inline unsigned long long total_clfrs() const { return _total_clfrs; }
			void close();
			virtual ~CLFRFileReader();

		private:
			std::string _buf;
			std::fstream _ifs;
			bool _end = false;
			unsigned _obj_len = 0;
			google::protobuf::io::IstreamInputStream* _raw_in = nullptr;
			google::protobuf::io::CodedInputStream* _coded_in = nullptr;
			bool _closed = false;
			bool _debug = false;
			std::ostream& _debug_os = std::cout;
			unsigned long long _total_clfrs = 0;
			void _peek(unsigned& next_len);
		};
	}
}

#endif
