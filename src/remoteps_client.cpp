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
#include "remoteps_version.hpp"

using grpc::Status;
using grpc::ClientContext;
using grpc::ClientReader;
using remoteps::RemotePsClient;
using remoteps::Message;

RemotePsClient::RemotePsClient(const std::string& ip, const uint16_t port)
{
	peer.setIP(ip);
	peer.setPort(port);
	channel = grpc::CreateChannel(peer.getIpPort(), grpc::InsecureChannelCredentials());
	stub_ = RemotePs::NewStub(channel);
	registerCommands();
}

void RemotePsClient::registerCommands()
{
	cmdMap.clear();
	cmdDesc.clear();

	cmdMap["help"] = HELP;
	cmdDesc[HELP] = "List of available commands and description";
	cmdFunc[HELP] = &RemotePsClient::commandHelp;

	cmdMap["list"] = LIST;
	cmdDesc[LIST] = "List of all current running processes";
	cmdFunc[LIST] = &RemotePsClient::commandList;

	cmdMap["exit"] = EXIT;
	cmdDesc[EXIT] = "Exit from program";
}

int RemotePsClient::connectionTest()
{
	Message request;
	Message reply;

	ClientContext context;
	Status status = stub_->connectionTest(&context, request, &reply);

	if (status.ok())
		return 0;
	return 1;
}

void RemotePsClient::commandLoop()
{
	if (connectionTest()) {
		std::cerr << "Connection failed to " << peer.getIpPort() << std::endl;
		return;
	}

	while (true) {
		std::string cmd;
		std::cout << REMOTEPS_NAME << "> ";
		std::getline(std::cin, cmd);
		if (std::cin.eof()) {
			std::cout << std::endl;
			return;
		}

		try {
			(*this.*cmdFunc.at(cmdMap.at(cmd)))();
		}
		catch (...) {
			std::cout << cmd << ": command not found" << std::endl;
		}
#if 0
		if (executeCommand(cmd))
			return;
#endif
	}
}

#if 0
int RemotePsClient::executeCommand(const std::string& cmd)
{
	try {
		switch (cmdMap.at(cmd)) {
			case HELP:
				commandHelp();
				break;
			case LIST:
				commandList();
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
#endif

void RemotePsClient::commandHelp()
{
	std::cout << "\nAvailable commands:\n";
	for (const auto &[cmd, i] : cmdMap) {
		try {
			std::cout << cmd << "\t" << cmdDesc.at(i) << std::endl;
		}
		catch (...) {}
	}
	std::cout << std::endl;
}

void RemotePsClient::commandList()
{
	Process process;
	Process tmp;
	ClientContext context;

	std::unique_ptr<ClientReader<Process>> reader(
			stub_->listOfProcs(&context, process));

	std::cout << "PID\tPPID\tCMD\n";
	while (reader->Read(&tmp)) {
		std::cout << tmp.pid() << "\t";
		std::cout << tmp.ppid() << "\t";
		std::cout << tmp.cmd() << std::endl;
	}
}
