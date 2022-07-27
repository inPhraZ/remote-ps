/*
 * =====================================================================================
 *
 *       Filename:  remoteps_address.hpp
 *
 *    Description:  Definition of network address class
 *
 *         Author:  Farzin Monsef 
 *
 * =====================================================================================
 */

#ifndef 		__REMOTEPS_ADDRESS_HPP_
#define 		__REMOETPS_ADDRESS_HPP_		1

#include <string>
#include <cstdint>

namespace remoteps {
	class Address
	{
		public:
			Address();
			Address(const std::string& ip, const uint16_t port);
			std::string IpPort();
			void setIP(const std::string& ip);
			void setPort(const uint16_t port);
			std::string getIP();
			uint16_t getPort();
			bool isV4();
			bool isV6();
			bool isValid();

		private:
			std::string ip;
			uint16_t    port;
	};
}

#endif 		/* 	__REMOTEPS_ADDRESS_HPP_ 	*/
