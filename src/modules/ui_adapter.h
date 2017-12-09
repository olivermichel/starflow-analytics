
#ifndef STARFLOW_ANALYTICS_UI_ADAPTER_H
#define STARFLOW_ANALYTICS_UI_ADAPTER_H

#include <string>
#include <memory>

#include <grpc++/grpc++.h>

#include "ui_server.h"

namespace starflow {
	namespace modules {

		class UIAdapter
		{
		public:
			explicit UIAdapter(const std::string& listen_addr);
			void operator()();

		private:
			std::unique_ptr<UIServer> _ui_server  = nullptr;
			std::unique_ptr<grpc::Server> _server = nullptr;
		};
	}
}

#endif