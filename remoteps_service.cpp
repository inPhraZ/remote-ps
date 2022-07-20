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

void RemotePsService::RunServer(const std::string& address)
{
	grpc::EnableDefaultHealthCheckService(true);
	grpc::reflection::InitProtoReflectionServerBuilderPlugin();

	ServerBuilder builder;
	builder.AddListeningPort(address, grpc::InsecureServerCredentials());
	builder.RegisterService(this);

	std::unique_ptr<Server> server(builder.BuildAndStart());
	if (server == nullptr) {
		std::cerr << "Unable to run server" << std::endl;
		return;
	}

	std::cout << "Server is listening on " << address << std::endl;

	server->Wait();
}
