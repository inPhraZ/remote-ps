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
#include <cstdint>
#include <grpcpp/grpcpp.h>

#include "remoteps.grpc.pb.h"
#include "remoteps_address.hpp"

namespace remoteps {
	class RemotePsClient
	{
		public:
			RemotePsClient(const std::string& ip, const uint16_t port);
			int ConnectionTest();
			void CommandLoop();
		private:
			Address peer;
			std::shared_ptr<grpc::Channel> channel;
			std::unique_ptr<RemotePs::Stub> stub_;
			int ExecuteCommand(const std::string& cmd);
	};
}

#endif 		/*  __REMOTEPS_CLIENT_HPP	*/
