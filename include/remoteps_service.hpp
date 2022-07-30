/*
 * =====================================================================================
 *
 *       Filename:  remoteps_service.hpp
 *
 *    Description:  Definiton of RemotePsService class.
 *                  Logic and data behind the server's behavior.
 *
 *         Author:  Farzin Monsef 
 *
 * =====================================================================================
 */

#ifndef 		__REMOTEPS_SERVICE_HPP_
#define 		__REMOTEPS_SERVICE_HPP_		1

#include <string>
#include <memory>
#include <cstdint>

#include <grpcpp/grpcpp.h>
#include "remoteps.grpc.pb.h"
#include "remoteps_procinfo.hpp"
#include "remoteps_address.hpp"

using grpc::Status;
using grpc::Server;
using grpc::ServerContext;
using grpc::ServerWriter;

namespace remoteps {
	class RemotePsService final : public RemotePs::Service
	{
		public:
			RemotePsService();
			RemotePsService(const std::string& ip, const uint16_t port);
			void runServer();
			void stopServer();
			Status connectionTest(ServerContext* context, const Message *request,
					Message *reply);
			Address addr;
		private:
			ProcInfo procinfo;
			std::unique_ptr<Server> server;

			Status listOfProcs(ServerContext* context, const Process* process,
					ServerWriter<Process>* writer);
	};
}

#endif 		/*  __REMOTEPS_SERVICE_HPP_		*/
