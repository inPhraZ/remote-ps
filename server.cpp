/*
 * =====================================================================================
 *
 *       Filename:  server.cpp
 *
 *    Description:  server application of remote-ps
 *
 *         Author:  Farzin Monsef 
 *
 * =====================================================================================
 */

#include "remoteps_service.hpp"

int main()
{
	RemotePsService server;
	server.RunServer("0.0.0.0:2048");

	return 0;
}
