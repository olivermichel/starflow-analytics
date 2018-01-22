
#ifndef STARFLOW_ANALYTICS_UI_CLIENT_H
#define STARFLOW_ANALYTICS_UI_CLIENT_H

#include <grpc++/grpc++.h>
#include "../proto/starflow.grpc.pb.h"

namespace starflow {
	namespace modules {
		class UIClient
		{
		public:
			explicit UIClient(std::string addr);
			explicit UIClient(std::shared_ptr<grpc::Channel> channel);
			bool say_hello();

		private:
			std::shared_ptr<grpc::Channel> _channel;
			std::unique_ptr<proto::UIService::Stub> _service_stub;
		};
	}
}

#endif
