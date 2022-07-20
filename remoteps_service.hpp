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

using remoteps::RemotePs;

class RemotePsService final : public RemotePs::Service
{
	public:
		void RunServer(const std::string& address);
};

#endif 		/*  __REMOTEPS_SERVICE_HPP_		*/
