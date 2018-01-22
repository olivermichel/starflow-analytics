
#include "ui_adapter.h"

starflow::modules::UIAdapter::UIAdapter(const std::string& listen_addr)
	: _ui_server(std::make_unique<UIService>())
{
	grpc::ServerBuilder builder;
	builder.AddListeningPort(listen_addr, grpc::InsecureServerCredentials());
	builder.RegisterService(_ui_server.get());

	_server = builder.BuildAndStart();
}

void starflow::modules::UIAdapter::operator()()
{
	_server->Wait();
}
