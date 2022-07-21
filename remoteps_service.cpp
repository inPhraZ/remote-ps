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
	this->ip = std::string("0.0.0.0");
	this->port = 5000;
}

RemotePsService::RemotePsService(const std::string& ip, const uint16_t port)
	: RemotePs::Service()
{
	this->ip = ip;
	this->port = port;
}

void RemotePsService::RunServer()
{
	grpc::EnableDefaultHealthCheckService(true);
	grpc::reflection::InitProtoReflectionServerBuilderPlugin();

	ServerBuilder builder;
	builder.AddListeningPort(this->IpPort(), grpc::InsecureServerCredentials());
	builder.RegisterService(this);

	std::unique_ptr<Server> server(builder.BuildAndStart());
	if (server == nullptr) {
		std::cerr << "Unable to run server" << std::endl;
		return;
	}

	std::cout << "Server is listening on " << this->IpPort() << std::endl;

	server->Wait();
}

std::string RemotePsService::IpPort()
{
	return  (this->ip + ":" + std::to_string(this->port));
}

void RemotePsService::setIP(const std::string& ip)
{
	this->ip = ip;
}

void RemotePsService::setPort(const uint16_t port)
{
	this->port = port;
}

std::string RemotePsService::getIP()
{
	return this->ip;
}

uint16_t RemotePsService::getPort()
{
	return this->port;
}
