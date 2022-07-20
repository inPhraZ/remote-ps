/*
 * =====================================================================================
 *
 *       Filename:  server.cpp
 *
 *    Description:  server application of remote-ps
 *
 *         Author:  Farzin Monsef 
 *
 * =====================================================================================
 */

#include <iostream>
#include <iterator>
#include <exception>
#include <cerrno>
#include <boost/program_options.hpp>

#include "remoteps_service.hpp"
#include "version.hpp"

namespace po = boost::program_options;

int main(int argc, char *argv[])
{
	try {
		po::options_description description("[OPTIONS]");
		description.add_options()
			("ip,i", "specify IP address to bind")
			("port,p", "specify PORT number to use")
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
			return 0;
		}

		if (vm.count("version")) {
			std::cout << REMOTEPS_NAME << " " << REMOTEPS_VERSION << std::endl;
			std::cout << "Written by " << REMOTEPS_AUTHOR << std::endl;
			return 0;
		}
	}
	catch (std::exception& e) {
		std::cerr << "Error: " << e.what() << std::endl;
		return 1;
	}
	catch (...) {
		std::cerr << "Unknown exception" << std::endl;
		return 1;
	}

	RemotePsService server;
	server.RunServer("0.0.0.0:2048");

	return 0;
}
