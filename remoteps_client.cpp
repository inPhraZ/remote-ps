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
	cmdDesc.clear();

	cmdMap["help"] = HELP;
	cmdDesc[HELP] = "List of available commands and description";

	cmdMap["exit"] = EXIT;
	cmdDesc[EXIT] = "Exit from program";
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
	try {
		switch (cmdMap.at(cmd)) {
			case HELP:
				CommandHelp();
				break;
			case EXIT:
				return 1;
				break;
			default:
				break;
		}
	}
	catch (std::out_of_range) {
		std::cout << cmd << ": command not found" << std::endl;
	}

	return 0;
}

void RemotePsClient::CommandHelp()
{
	std::cout << "Available commands:" << std::endl;
	for (const auto &[cmd, i] : cmdMap) {
		try {
			std::cout << cmd << "\t" << cmdDesc.at(i) << std::endl;
		}
		catch (...) {}
	}
}
