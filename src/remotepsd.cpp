/*
 * =====================================================================================
 *
 *       Filename:  remotepsd.cpp
 *
 *    Description:  remoteps daemon (Server application of remote-ps)
 *
 *         Author:  Farzin Monsef 
 *
 * =====================================================================================
 */

#include <iostream>
#include <string>
#include <memory>
#include <thread>
#include <chrono>
#include <exception>
#include <cstdint>
#include <cerrno>
#include <signal.h>
#include <boost/program_options.hpp>
#include <prometheus/family.h>
#include <prometheus/gauge.h>
#include <prometheus/exposer.h>
#include <prometheus/registry.h>

#include "remoteps_service.hpp"
#include "remoteps_procinfo.hpp"
#include "remoteps_version.hpp"

namespace po = boost::program_options;

using remoteps::RemotePsService;
using namespace prometheus;

static bool shutdown = false;
static std::unique_ptr<RemotePsService> server;

void signalHandler(int sig);
void checkShutdown();
void parseCommandLine(int argc, char *argv[], std::string& ip, uint16_t& port);
void monitor_procs(const std::string ip, const uint16_t port);

int main(int argc, char *argv[])
{
	std::string ip;
	uint16_t    port;

	signal(SIGINT, signalHandler);

	parseCommandLine(argc, argv, ip, port);

	std::thread monitor_thread(monitor_procs, ip, port+1);
	std::thread t(checkShutdown);

	server = std::unique_ptr<RemotePsService>(new RemotePsService(ip, port));
	server->runServer();

	t.detach();
	t.detach();

	return 0;
}

void signalHandler(int sig)
{
	shutdown = true;
}

void checkShutdown()
{
	while(!shutdown) sleep(1);
	server->stopServer();
}

void parseCommandLine(int argc, char *argv[], std::string& ip, uint16_t& port)
{
	try {
		po::options_description description("[OPTIONS]");
		description.add_options()
			("ip,i",
			 po::value<std::string>()->default_value("0.0.0.0"),
			 "specify IP address to bind")
			("port,p",
			 po::value<uint16_t>()->default_value(5000),
			 "specify PORT number to use")
			("help,h", "display this help and exit")
			("version,v", "output version information and exit");
	
		po::variables_map vm;
		po::store(po::parse_command_line(argc, argv, description), vm);
		po::notify(vm);

		if (vm.count("help")) {
			std::cout << "Usage: " << program_invocation_name << " [OPTIONS]" << std::endl;
			std::cout << "Report information of the current "
				"processes on a remote linux server\n" << std::endl;
			std::cout << description;
			exit(EXIT_SUCCESS);
		}

		if (vm.count("version")) {
			std::cout << REMOTEPS_NAME << " " << REMOTEPS_VERSION << std::endl;
			std::cout << "Written by " << REMOTEPS_AUTHOR << std::endl;
			exit(EXIT_SUCCESS);
		}

		if (vm.count("port")) {
			if (vm["port"].as<uint16_t>() <= 0) {
				std::cerr << "Invalid port number: " << vm["port"].as<uint16_t>() << std::endl;
				exit(EXIT_FAILURE);
			}
		}
		ip = vm["ip"].as<std::string>();
		port = vm["port"].as<uint16_t>();
	}
	catch (std::exception& e) {
		std::cerr << "Error: " << e.what() << std::endl;
		exit(EXIT_FAILURE);
	}
	catch (...) {
		std::cerr << "Unknown exception" << std::endl;
		exit(EXIT_FAILURE);
	}
}

void monitor_procs(const std::string ip, const uint16_t port)
{
	remoteps::Address addr(ip, port);
	Exposer exposer(addr.getIpPort());
	
	auto registry = std::make_shared<Registry>();

	auto& proc_gauge_family = BuildGauge()
		.Name("number_of_procs")
		.Help("Number of processes")
		.Register(*registry);
	auto& proc_gauge = proc_gauge_family.Add({});

	exposer.RegisterCollectable(registry);

	remoteps::ProcInfo procinfo;
	for (;;) {
		procinfo.readProcs();
		proc_gauge.Set(procinfo.getProcs().size());
		std::this_thread::sleep_for(std::chrono::seconds(3));
	}
}
