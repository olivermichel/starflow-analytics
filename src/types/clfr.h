
#ifndef STARFLOW_ANALYTICS_CLFR_H
#define STARFLOW_ANALYTICS_CLFR_H

#include <list>
#include <cstdint>

#include "packet.h"
#include "key.h"


namespace starflow {

	// forward declare kernels::CLFRTable to allow friend
	// specification w/o circular dependency
	namespace kernels { class CLFRTable; }
	namespace modules { class FlowTable; }

	namespace types {
		class CLFR
		{
			friend class starflow::modules::FlowTable;

		public:
			CLFR();
			CLFR(const Key& k, uint64_t flow_id, uint16_t table_id = 0);
			CLFR(const CLFR&)            = default;
			CLFR& operator=(const CLFR&) = default;
			CLFR(CLFR&&)                 = default;
			CLFR& operator=(CLFR&&)      = default;

			explicit CLFR(const proto::clfr&);

			proto::clfr to_proto() const;

			const Key& key() const;

			std::uint64_t id() const;
			std::uint16_t table_id() const;
			std::uint32_t evict_ts_s() const;

			void add_packet(Packet p);
			const Packet& last_packet() const;

			const std::list<Packet>& packets() const;

			std::string str_desc() const;

			bool complete() const;

			unsigned long n_packets() const;
			unsigned long n_bytes() const;

			bool operator==(const CLFR& other) const;
			bool operator!=(const CLFR& other) const;

			void _set_evict_ts_s(std::uint32_t evict_ts_s);

			virtual ~CLFR() = default;

		private:
			Key _key;
			bool _complete = false;

			std::uint64_t _id;
			std::uint16_t _table_id = 0;
			std::uint32_t _evict_ts_s;

			std::list<types::Packet> _packets = {};
		};
	}
}

#endif
