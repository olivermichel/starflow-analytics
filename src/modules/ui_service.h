
#ifndef STARFLOW_ANALYTICS_RPC_SERVER_H
#define STARFLOW_ANALYTICS_RPC_SERVER_H

#include "../proto/starflow.pb.h"
#include "../proto/starflow.grpc.pb.h"

#include <grpc++/grpc++.h>


namespace starflow {
	namespace modules {
		class UIService final : public proto::UIService::Service
		{
			grpc::Status SayHello(grpc::ServerContext* c, const proto::HelloRequest* req,
								  proto::HelloReply* rep) override;
		};
	}
}

#endif
