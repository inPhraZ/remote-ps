/*
 * =====================================================================================
 *
 *       Filename:  remoteps_client.cpp
 *
 *    Description:  Implementation of the remoteps client class
 *
 *         Author:  Farzin Monsef 
 *
 * =====================================================================================
 */

#include <iostream>
#include <string>
#include <memory>
#include <cstdint>

#include "remoteps_client.hpp"

using grpc::Status;
using grpc::ClientContext;

remoteps::RemotePsClient::RemotePsClient(const std::string& ip, const uint16_t port)
{
	peer.setIP(ip);
	peer.setPort(port);
	channel = grpc::CreateChannel(peer.IpPort(), grpc::InsecureChannelCredentials());
	stub_ = RemotePs::NewStub(channel);
}

int remoteps::RemotePsClient::ConnectionTest()
{
	remoteps::Message request;
	remoteps::Message reply;

	ClientContext context;
	Status status = stub_->ConnectionTest(&context, request, &reply);

	if (status.ok())
		return 0;
	return 1;
}
