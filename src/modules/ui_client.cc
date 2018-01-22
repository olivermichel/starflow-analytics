
#include "ui_client.h"

starflow::modules::UIClient::UIClient(std::string addr)
	: _channel(grpc::CreateChannel(addr, grpc::InsecureChannelCredentials())),
	  _service_stub(starflow::proto::UIService::NewStub(_channel)) { }

starflow::modules::UIClient::UIClient(std::shared_ptr<grpc::Channel> channel)
	: _channel(channel),
	  _service_stub(starflow::proto::UIService::NewStub(_channel)) { }

bool starflow::modules::UIClient::say_hello()
{
	starflow::proto::HelloRequest hello_request;
	starflow::proto::HelloReply hello_reply;

	grpc::ClientContext context;
	grpc::Status status = _service_stub->SayHello(&context, hello_request, &hello_reply);

	return status.ok();
}
