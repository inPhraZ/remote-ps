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

#include "remoteps.grpc.pb.h"

namespace remoteps {
	class RemotePsService final : public RemotePs::Service
	{
		public:
			RemotePsService(const std::string& ip, const uint16_t port);
			void RunServer();
			void setIP(const std::string& ip);
			void setPort(const uint16_t port);
			std::string getIP();
			uint16_t    getPort();
		private:
			std::string IpPort();
			std::string ip;
			uint16_t    port;
	};
}

#endif 		/*  __REMOTEPS_SERVICE_HPP_		*/
