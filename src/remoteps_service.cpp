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
#include <cstdint>
#include <syslog.h>

#include <grpcpp/grpcpp.h>
#include <grpcpp/health_check_service_interface.h>
#include <grpcpp/ext/proto_server_reflection_plugin.h>

#include "remoteps_service.hpp"
#include "remoteps_version.hpp"

using grpc::Server;
using grpc::ServerBuilder;
using grpc::Status;
using grpc::ServerContext;
using remoteps::RemotePsService;
using remoteps::Message;
using remoteps::Process;

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

void RemotePsService::runServer()
{
	openlog(REMOTEPS_NAME, LOG_PID, LOG_DAEMON);

	if (addr.isValid() == false) {
		syslog(LOG_ERR, "Invalid address to bind: %s", addr.getIP().c_str());
		std::cerr << "Invalid address to bind: " << addr.getIP() << std::endl;
		closelog();
		return;
	}

	ServerBuilder builder;
	builder.AddListeningPort(addr.getIpPort(), grpc::InsecureServerCredentials());
	builder.RegisterService(this);

	server = builder.BuildAndStart();
	if (server == nullptr) {
		syslog(LOG_ERR, "Unable to run server");
		std::cerr << "Unable to run server" << std::endl;
		closelog();
		return;
	}

	syslog(LOG_INFO, "Server is listening on %s", addr.getIpPort().c_str());
	std::cout << "Server is listening on " << addr.getIpPort() << std::endl;

	server->Wait();
}

void RemotePsService::stopServer()
{
	server->Shutdown();
	syslog(LOG_INFO, "Server stopper");
	std::cout << "Server stopped" << std::endl;
	closelog();
}

Status RemotePsService::connectionTest(ServerContext* context, const Message* request,
		Message* reply)
{
	return Status::OK;
}

Status RemotePsService::listOfProcs(ServerContext* context, const Process* process,
		ServerWriter<Process>* writer)
{
	procinfo.readProcs();
	const std::vector<Process> procs = procinfo.getProcs();
	for (const auto p : procs)
		writer->Write(p);
	return Status::OK;
}
