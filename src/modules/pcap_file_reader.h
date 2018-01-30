
#ifndef STARFLOW_ANALYTICS_MODULES_PCAP_FILE_READER_H
#define STARFLOW_ANALYTICS_MODULES_PCAP_FILE_READER_H

#include <stdexcept>
#include <string>
#include <vector>
#include <pcap.h>
#include <random>
#include "../types/packet.h"

namespace starflow {
	namespace modules {

		/*
		 * 	PCAPFileReader pfr("test.pcap")
		 *
		 * 	Packet p;
		 *
		 *	while(!pfr.end()) {
		 *		p = pfr.next();
		 *	}
		 */

		class PCAPFileReader
		{
		public:
			PCAPFileReader()                            = delete;
			PCAPFileReader(const std::string& file_name, bool outer_eth = true)
				throw (std::runtime_error);
			PCAPFileReader(const PCAPFileReader&)       = delete;
			PCAPFileReader operator=(PCAPFileReader&)   = delete;
			PCAPFileReader(PCAPFileReader&&)            = default;
			PCAPFileReader& operator=(PCAPFileReader&&) = default;

			void next(types::Key&, types::Packet&) throw (std::runtime_error);

			inline bool end() const { return _end; }

			~PCAPFileReader();

		private:
			struct pcap_pkthdr* _hdr = {};
			const u_char* _pl_buf = {};
			pcap* _pcap = nullptr;
			bool _end;
			bool _outer_eth;

			std::random_device _random_device;
			std::mt19937 _random_generator;

			pcap* _open(const std::string& file_name) throw (std::runtime_error);
			bool _peek() throw (std::runtime_error);
			void _close();
		};
	}
}

#endif
