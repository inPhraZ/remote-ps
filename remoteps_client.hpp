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
#include <grpcpp/grpcpp.h>

#include "remoteps.grpc.pb.h"

using grpc::Channel;

namespace remoteps {
	class RemotePsClient {
		public:
			RemotePsClient(std::shared_ptr<Channel> channel)
				: stub_(RemotePs::NewStub(channel)) {}
		private:
			std::unique_ptr<RemotePs::Stub> stub_;
	};
}

#endif 		/*  __REMOTEPS_CLIENT_HPP	*/
