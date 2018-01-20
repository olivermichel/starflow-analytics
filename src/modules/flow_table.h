
#ifndef STARFLOW_ANALYTICS_MODULES_FLOW_TABLE_H
#define STARFLOW_ANALYTICS_MODULES_FLOW_TABLE_H

#include "../types/key.h"
#include "../types/clfr.h"

#include <map>
#include <stdexcept>

namespace starflow {
	namespace modules {

		class FlowTable
		{
			using flow_table_t           = std::map<types::Key, types::CLFR>;
			using exported_flows_table_t = std::list<types::CLFR>;
			using export_flow_callback_t = std::function<void (types::CLFR)>;
			enum class _ip_proto : uint8_t { icmp = 1, tcp = 6, udp = 17 };

		public:

//			enum class incomplete_evict_policy { none, to, pkt_count };
			enum class mode { callback, store };

			FlowTable() = default;
			explicit FlowTable(unsigned id);

			FlowTable(const FlowTable&)            = delete;
			FlowTable& operator=(const FlowTable&) = delete;

			FlowTable(FlowTable&&)                 = default;
			FlowTable& operator=(FlowTable&&)      = default;

			unsigned id() const;

			void add_packet(types::Key key, types::Packet packet)
				throw(std::logic_error);

			void add_packet(std::pair<types::Key, types::Packet> pair)
				throw(std::logic_error);

			void set_mode(mode m);

			void set_callback(export_flow_callback_t&& callback);

			unsigned long long count_packets_processed() const;
			unsigned long long count_flows_processed() const;
			unsigned long long count_packets() const;
			unsigned long long count_flows() const;

			const flow_table_t& flows() const;

			const exported_flows_table_t& exported_flows() const;

			// for unit tests:
			void _force_export_tcp(bool complete = false);
			void _force_export_udp(bool complete = false);
			void _force_check_last_ack();

		private:

			unsigned _id                                       = 0;
			flow_table_t _active_flows                         = {};
			exported_flows_table_t _exported_flows             = {};

			std::chrono::microseconds _to_check_interval       = std::chrono::seconds(10);

			std::chrono::microseconds _udp_to                  = std::chrono::seconds(30);
			std::chrono::microseconds _last_to_check           = std::chrono::seconds(0);

			FlowTable::mode _mode                              = mode::callback;
/*
			incomplete_evict_policy _incomplete_evict_policy = incomplete_evict_policy::none;
			std::chrono::microseconds _incomplete_evict_to   = std::chrono::seconds(10);
			unsigned long _incomplete_evict_pkt_count        = 50;
*/
			unsigned long long _next_id                        = 1;

			unsigned long long _n_packets_processed            = 0;
			unsigned long long _n_flows_processed              = 0;
			unsigned long long _n_packets                      = 0;
			unsigned long long _n_flows                        = 0;

			export_flow_callback_t _callback = nullptr;

			flow_table_t::iterator _lookup_and_insert(types::Key&& key, types::Packet&& packet);

			void _check_evict(flow_table_t::iterator, std::chrono::microseconds ts);

			void _check_timeouts(std::chrono::microseconds trigger_ts);

			void _check_last_ack();

			flow_table_t::iterator _evict_flow(const flow_table_t::iterator& i,
											   std::chrono::microseconds evict_ts, bool complete);

			flow_table_t::iterator _delete_flow(const flow_table_t::iterator& i);
		};
	}
}

#endif
