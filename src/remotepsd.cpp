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
#include <exception>
#include <cstdint>
#include <cerrno>
#include <boost/program_options.hpp>

#include "remoteps_service.hpp"
#include "remoteps_version.hpp"

namespace po = boost::program_options;

void parseCommandLine(int argc, char *argv[], std::string& ip, uint16_t& port);

int main(int argc, char *argv[])
{
	std::string ip;
	uint16_t    port;

	parseCommandLine(argc, argv, ip, port);

	remoteps::RemotePsService server(ip, port);
	server.runServer();

	return 0;
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
