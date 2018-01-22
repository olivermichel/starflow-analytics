
#include "ui_service.h"

grpc::Status starflow::modules::UIService::SayHello(grpc::ServerContext* c,
	const proto::HelloRequest* req, proto::HelloReply* rep)
{
	return grpc::Status::OK;
}
