/*
 * =====================================================================================
 *
 *       Filename:  remoteps_client.cpp
 *
 *    Description:  Implementation of the remoteps client class
 *
 *         Author:  Farzin Monsef 
 *
 * =====================================================================================
 */

#include <iostream>
#include <memory>
#include <string>
#include <map>
#include <cstdint>

#include "remoteps_client.hpp"

using grpc::Status;
using grpc::ClientContext;
using remoteps::RemotePsClient;
using remoteps::Message;

RemotePsClient::RemotePsClient(const std::string& ip, const uint16_t port)
{
	peer.setIP(ip);
	peer.setPort(port);
	channel = grpc::CreateChannel(peer.IpPort(), grpc::InsecureChannelCredentials());
	stub_ = RemotePs::NewStub(channel);
	GenerateCommands();
}

void RemotePsClient::GenerateCommands()
{
	cmdMap.clear();
	cmdMap["help"] = HELP;
	cmdMap["exit"] = EXIT;
}

int RemotePsClient::ConnectionTest()
{
	Message request;
	Message reply;

	ClientContext context;
	Status status = stub_->ConnectionTest(&context, request, &reply);

	if (status.ok())
		return 0;
	return 1;
}

void RemotePsClient::CommandLoop()
{
	if (ConnectionTest()) {
		std::cerr << "Connection failed to " << peer.IpPort() << std::endl;
		return;
	}

	while (true) {
		std::string cmd;
		std::cout << "Enter command: ";
		std::getline(std::cin, cmd);
		if (std::cin.eof()) {
			std::cout << std::endl;
			return;
		}

		if (ExecuteCommand(cmd))
			return;
	}
}

int RemotePsClient::ExecuteCommand(const std::string& cmd)
{
	switch (cmdMap[cmd]) {
		case HELP:
			break;
		case EXIT:
			return 1;
			break;
		default:
			std::cout << "Invalid command: " << cmd << std::endl;
			break;
	}
	return 0;
}
