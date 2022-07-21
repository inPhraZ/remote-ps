/*
 * =====================================================================================
 *
 *       Filename:  remoteps_address.hpp
 *
 *    Description:  network address class
 *
 *         Author:  Farzin Monsef 
 *
 * =====================================================================================
 */

#ifndef 		__REMOTEPS_ADDRESS_HPP_
#define 		__REMOETPS_ADDRESS_HPP_		1

#include <string>

namespace remoteps {
	class Address
	{
		public:
			Address() {}

			Address(const std::string& ip, const uint16_t port)
				: ip(ip), port(port) {}

			std::string IpPort()
			{
				return (this->ip + ":" + std::to_string(this->port));
			}

			void setIP(const std::string& ip)
			{
				this->ip = ip;
			}

			void setPort(const uint16_t port)
			{
				this->port = port;
			}

			std::string getIP()
			{
				return this->ip;
			}

			uint16_t    getPort()
			{
				return this->port;
			}

		private:
			std::string ip;
			uint16_t    port;
	};
}

#endif 		/* 	__REMOTEPS_ADDRESS_HPP_ 	*/
