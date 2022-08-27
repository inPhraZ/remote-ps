/*
 * =====================================================================================
 *
 *       Filename:  remoteps_client.hpp
 *
 *    Description:  Definition of the client class for remoteps
 *
 *         Author:  Farzin Monsef 
 *
 * =====================================================================================
 */

#ifndef 		__REMOTEPS_CLIENT_HPP_
#define 		__REMOTEPS_CLIENT_HPP_	1

#include <memory>
#include <string>
#include <map>
#include <cstdint>
#include <grpcpp/grpcpp.h>

#include "remoteps.grpc.pb.h"
#include "remoteps_address.hpp"

namespace remoteps {

	enum CommandIndex {
		HELP = 1,
		LIST,
		PROC,
		PID,
		EXIT
	};

	class RemotePsClient
	{
		public:
			RemotePsClient(const std::string& ip, const uint16_t port);
			void commandLoop();
		private:
			Address peer;
			std::map<std::string, int> cmdMap;
			std::map<int, std::string> cmdDesc;
			std::map<int, void (RemotePsClient::*)
				([[maybe_unused]] const std::string&)> cmdFunc;
			std::shared_ptr<grpc::Channel> channel;
			std::unique_ptr<RemotePs::Stub> stub_;
			int connectionTest();
			void registerCommands();

			/*  Utils */
			void printProcess(const Process& proc);
			void printProcDetail(const Process& proc);
			void printCmdline(const Process& proc);

			/*  Commands */
			void commandHelp([[maybe_unused]] const std::string& param);
			void commandList([[maybe_unused]] const std::string& param);
			void commandProc([[maybe_unused]] const std::string& param);
			void commandPid([[maybe_unused]] const std::string& param);
			void commandExit([[maybe_unused]] const std::string& param);
	};
}

#endif 		/*  __REMOTEPS_CLIENT_HPP	*/
