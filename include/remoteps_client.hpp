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

			/*  Commands */
			void commandHelp([[maybe_unused]] const std::string& cmd);
			void commandList([[maybe_unused]] const std::string& cmd);
			void commandExit([[maybe_unused]] const std::string& cmd);
	};
}

#endif 		/*  __REMOTEPS_CLIENT_HPP	*/
