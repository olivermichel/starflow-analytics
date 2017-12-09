
#include "ui_server.h"

grpc::Status starflow::modules::UIServer::SayHello(grpc::ServerContext* c,
	const proto::HelloRequest* req, proto::HelloReply* rep)
{
	return grpc::Status::OK;
}
