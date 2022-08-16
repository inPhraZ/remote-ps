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
#include <fstream>
#include <memory>
#include <map>
#include <string>
#include <sstream>
#include <cstdint>

#include "remoteps_client.hpp"
#include "remoteps_version.hpp"

using grpc::Status;
using grpc::ClientContext;
using grpc::ClientReader;
using remoteps::RemotePsClient;
using remoteps::Message;

static std::string readFile(const std::string& filename)
{
	std::stringstream ss;
	std::ifstream file(filename.c_str(), std::ios::in);
	if (file.is_open()) {
		ss << file.rdbuf();
		file.close();
	}
	return ss.str();
}

RemotePsClient::RemotePsClient(const std::string& ip, const uint16_t port)
{
	peer.setIP(ip);
	peer.setPort(port);
	
	std::string key;
	std::string cert;
	std::string root;

	key = readFile("cert/remotePsClient.key");
	cert = readFile("cert/remotePsClient.crt");
	root = readFile("cert/remotePsServer.crt");

	grpc::SslCredentialsOptions cred_ops;
	cred_ops.pem_private_key = key;
	cred_ops.pem_cert_chain = cert;
	cred_ops.pem_root_certs = root;

	auto channel_creds = grpc::SslCredentials(cred_ops);
	channel = grpc::CreateChannel("localhost:5000", channel_creds);
	stub_ = RemotePs::NewStub(channel);
	registerCommands();
}

void RemotePsClient::registerCommands()
{
	cmdMap.clear();
	cmdDesc.clear();
	cmdFunc.clear();

	cmdMap["help"] = HELP;
	cmdDesc[HELP] = "List of available commands and description";
	cmdFunc[HELP] = &RemotePsClient::commandHelp;

	cmdMap["list"] = LIST;
	cmdDesc[LIST] = "List of all current running processes";
	cmdFunc[LIST] = &RemotePsClient::commandList;

	cmdMap["proc"] = PROC;
	cmdDesc[PROC] = "Information of a given process";
	cmdFunc[PROC] = &RemotePsClient::commandProc;

	cmdMap["pid"] = PID;
	cmdDesc[PID] = "Information of a given process";
	cmdFunc[PID] = &RemotePsClient::commandPid;

	cmdMap["exit"] = EXIT;
	cmdDesc[EXIT] = "Exit from program";
	cmdFunc[EXIT] = &RemotePsClient::commandExit;
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
		std::string input;
		std::cout << REMOTEPS_NAME << "> ";
		std::getline(std::cin, input);
		if (std::cin.eof()) {
			std::cout << std::endl;
			return;
		}
		std::string cmd;
		std::stringstream ss(input);
		ss >> cmd;
		try {
			std::string param;
			param = input.substr(input.find(" ") + 1, input.length());
			(*this.*cmdFunc.at(cmdMap.at(cmd)))(param);
		}
		catch (std::out_of_range) {
			std::cerr << cmd << ": command not found" << std::endl;
		}
	}
}

void RemotePsClient::printProcess(const Process& proc)
{
	std::cout << proc.pid() << "\t";
	std::cout << proc.ppid() << "\t";
	std::cout << proc.user() << "\t";
	std::cout << proc.cmd() << std::endl;
}

void RemotePsClient::printCmdline(const Process& proc)
{
	for (int i = 0; i < proc.cmdline_size(); ++i)
		std::cout << proc.cmdline(i) << std::endl;
}

void RemotePsClient::commandHelp([[maybe_unused]] const std::string& param)
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

void RemotePsClient::commandList([[maybe_unused]] const std::string& param)
{
	Process process;
	Process tmp;
	ClientContext context;

	std::unique_ptr<ClientReader<Process>> reader(
			stub_->listOfProcs(&context, process));

	std::cout << "PID\tPPID\tUSR\tCMD\n";
	while(reader->Read(&tmp))
		printProcess(tmp);
}

void RemotePsClient::commandProc([[maybe_unused]] const std::string& param)
{
	ClientContext context;
	Process proc;
	Process tmp;


	proc.Clear();
	proc.set_cmd(param);

	std::unique_ptr<ClientReader<Process>> reader(
			stub_->procByCmd(&context, proc));

	std::cout << "PID\tPPID\tUSR\tCMD\n";
	while (reader->Read(&tmp))
		printProcess(tmp);
}

void RemotePsClient::commandPid([[maybe_unused]] const std::string& param)
{
	std::stringstream ss(param);
	ClientContext context;
	Process request;
	Process reply;
	uint32_t pid;
	ss >> pid;

	if (pid == 0) {
		std::cerr << "pid: Invalid argument: " << param << std::endl;
		return;
	}

	request.Clear();
	request.set_pid(pid);

	stub_->procByPid(&context, request, &reply);

	std::cout << "PID\tPPID\tUSR\tCMD\n";
	printProcess(reply);

	std::cout << "\nCmdline:\n";
	printCmdline(reply);
}

void RemotePsClient::commandExit([[maybe_unused]] const std::string& param)
{
	exit(0);
}
