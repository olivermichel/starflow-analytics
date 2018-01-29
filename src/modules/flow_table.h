
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

			const std::uint32_t _E6                            = 1000000;

			uint16_t _id                                       = 0;
			flow_table_t _active_flows                         = {};
			exported_flows_table_t _exported_flows             = {};

			std::uint32_t _to_check_interval_s                 = 5;
			std::uint32_t _ack_check_interval_s                = 1;
			std::uint32_t _udp_to_s                            = 10;
			std::uint32_t _tcp_to_s                            = 30;
			std::uint32_t _last_to_check_s                     = 0;
			std::uint32_t _last_ack_check_s                    = 0;
			std::uint32_t _incomplete_evict_pkt_count          = 50;

			FlowTable::mode _mode                              = mode::callback;

			unsigned long long _next_id                        = 1;

			unsigned long long _n_packets_processed            = 0;
			unsigned long long _n_flows_processed              = 0;
			unsigned long long _n_packets                      = 0;
			unsigned long long _n_flows                        = 0;

			export_flow_callback_t _callback = nullptr;

			flow_table_t::iterator _lookup_and_insert(types::Key&& key, types::Packet&& packet);

			void _check_evict(flow_table_t::iterator, std::uint32_t trigger_ts_s);

			void _check_timeouts(std::uint32_t trigger_ts_s);

			void _check_last_ack(std::uint32_t trigger_ts_s);

			flow_table_t::iterator _evict_flow(const flow_table_t::iterator& i,
											   std::uint32_t evict_ts_s, bool complete);

			flow_table_t::iterator _delete_flow(const flow_table_t::iterator& i);
		};
	}
}

#endif
