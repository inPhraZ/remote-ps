/*
 * =====================================================================================
 *
 *       Filename:  remoteps.cpp
 *
 *    Description:  
 *
 *         Author:  Farzin Monsef 
 *
 * =====================================================================================
 */

#include <iostream>
#include <string>

#include "remoteps_client.hpp"


int main()
{
	std::string target_str("0.0.0.0:5000");
	remoteps::RemotePsClient client(
			grpc::CreateChannel(target_str, grpc::InsecureChannelCredentials()));
	std::cout << "Hello from client\n";
	return 0;
}
