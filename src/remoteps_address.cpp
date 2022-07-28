/*
 * =====================================================================================
 *
 *       Filename:  remoteps_address.cpp
 *
 *    Description:  Implementation of network address class 
 *
 *         Author:  Farzin 
 *
 * =====================================================================================
 */

#include <string>
#include <cstdint>
#include <boost/asio.hpp>

#include "remoteps_address.hpp"

remoteps::Address::Address() {}

remoteps::Address::Address(const std::string& ip, const uint16_t port)
	: ip(ip), port(port) {}

std::string remoteps::Address::getIpPort()
{
	return (this->ip + ":" + std::to_string(this->port));
}

void remoteps::Address::setIP(const std::string& ip)
{
	this->ip = ip;
}

void remoteps::Address::setPort(const uint16_t port)
{
	this->port = port;
}

std::string remoteps::Address::getIP()
{
	return this->ip;
}

uint16_t remoteps::Address::getPort()
{
	return this->port;
}

bool remoteps::Address::isV4()
{
	try {
		bool valid = boost::asio::ip::make_address(ip.c_str()).is_v4();
		return valid;
	}
	catch (...) {
		return false;
	}
}

bool remoteps::Address::isV6()
{
	try {
		bool valid = boost::asio::ip::make_address(ip.c_str()).is_v6();
		return valid;
	}
	catch (...) {
		return false;
	}
}

bool remoteps::Address::isValid()
{
	if (isV4() || isV6())
		return true;
	return false;
}
