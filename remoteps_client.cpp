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

#include <string>
#include <memory>
#include <cstdint>

#include "remoteps_client.hpp"

remoteps::RemotePsClient::RemotePsClient(const std::string& ip, const uint16_t port)
{
	peer.setIP(ip);
	peer.setPort(port);
	auto channel_creds = grpc::SslCredentials(grpc::SslCredentialsOptions());
	channel = grpc::CreateChannel(peer.IpPort(), channel_creds);
	stub_ = RemotePs::NewStub(channel);
}
