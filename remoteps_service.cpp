/*
 * =====================================================================================
 *
 *       Filename:  remoteps_service.cpp
 *
 *    Description:  Implementation of RemotePsService.
 *                  Logic and data behind the server's behavior.
 *
 *         Author:  Farzin Monsef 
 *
 * =====================================================================================
 */

#include <iostream>
#include <string>
#include <memory>

#include <grpcpp/grpcpp.h>
#include <grpcpp/health_check_service_interface.h>
#include <grpcpp/ext/proto_server_reflection_plugin.h>

#include "remoteps_service.hpp"

using grpc::Server;
using grpc::ServerBuilder;
using remoteps::RemotePsService;

RemotePsService::RemotePsService()
	: RemotePs::Service()
{
	addr.setIP("0.0.0.0");
	addr.setPort(5000);
}

RemotePsService::RemotePsService(const std::string& ip, const uint16_t port)
	: RemotePs::Service()
{
	addr.setIP(ip);
	addr.setPort(port);
}

void RemotePsService::RunServer()
{
	if (addr.is_valid() == false) {
		std::cerr << "Invalid address to bind: " << addr.getIP() << std::endl;
		return;
	}

	grpc::EnableDefaultHealthCheckService(true);
	grpc::reflection::InitProtoReflectionServerBuilderPlugin();

	ServerBuilder builder;
	builder.AddListeningPort(addr.IpPort(), grpc::InsecureServerCredentials());
	builder.RegisterService(this);

	std::unique_ptr<Server> server(builder.BuildAndStart());
	if (server == nullptr) {
		std::cerr << "Unable to run server" << std::endl;
		return;
	}

	std::cout << "Server is listening on " << addr.IpPort() << std::endl;

	server->Wait();
}
