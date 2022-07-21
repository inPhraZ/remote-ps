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
#include <cstdint>

#include "remoteps.grpc.pb.h"
#include "remoteps_address.hpp"

namespace remoteps {
	class RemotePsService final : public RemotePs::Service
	{
		public:
			RemotePsService();
			RemotePsService(const std::string& ip, const uint16_t port);
			void RunServer();
			Address addr;
	};
}

#endif 		/*  __REMOTEPS_SERVICE_HPP_		*/
