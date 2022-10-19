#pragma once
#include <boost/asio.hpp>
#include <boost/array.hpp>
#include <boost/bind.hpp>
#include <thread>
#include <iostream>
#include <string>


#include "LuxHelper.h"
namespace DMDUtils 
{
	using boost::asio::ip::udp;
	using boost::asio::ip::address;

	class UDPSender
	{
	public:
		UDPSender() {

		}

		~UDPSender() {
			Close();
		}

		bool Init(std::string ip, int port,std::string local_ip,int local_port) {
			if (!m_Initialized)
			{
				// Remote Endpoint
				m_Endpoint = udp::endpoint(address::from_string(ip), port);

				// Open Socket
				boost::system::error_code err;

				{
					m_Socket.open(udp::v4(), err);

					std::cout << err << std::endl;

					if (err.value() != boost::system::errc::success)
					{
						lux::LogToFile("udp open failed");

						m_Initialized = false;
						return false;
					}
					else
					{
						m_Initialized = true;
						return true;
					}
				}
			}
			else
			{

				return true; // Already Inited Succesfully.
			}

			
		}

		void Send(unsigned char* PtdData, int size) {

			//lux::LogToFile(std::string((char*)PtdData));

			boost::system::error_code err;
			auto sending_buffer = boost::asio::buffer(PtdData, size);
			auto sizee = sending_buffer.size();
			auto sent = m_Socket.send_to(boost::asio::buffer(PtdData,size), m_Endpoint, 0, err);
		}

		std::vector<unsigned char> GetReply() {
			boost::array<unsigned char, 128> recvBuf;
			size_t len = m_Socket.receive_from(boost::asio::buffer(recvBuf), m_LocalEndpoint);
			if (0)
			{
				for (size_t i = 0; i < len; i++)
				{
					std::cout << (int)recvBuf[i] << " ";
				}
			}	
			std::vector<unsigned char> ret;
			ret.insert(ret.end(), recvBuf.begin(), recvBuf.begin()+len);

			return ret;
		}

		void Close() {
			m_Socket.close();
		}

	private:

		boost::asio::io_service io_service;
		udp::socket m_Socket{ io_service };
		udp::endpoint m_Endpoint;
		udp::endpoint m_LocalEndpoint;

		bool m_Initialized = false;

		bool m_NeedValidate = true;


	};

}


