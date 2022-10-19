#pragma once
#include <boost/asio.hpp>
#include <boost/array.hpp>
#include <boost/bind.hpp>
#include <thread>
#include <iostream>
#include <string>
#include <thread>

namespace DMDUtils
{
	using boost::asio::ip::udp;
	using boost::asio::ip::address;

	class UDPReceiver
	{
	public:
		UDPReceiver() {

		}
		~UDPReceiver() {
			
		}
		
		void Init(std::string ip, int port) {
			m_Socket.open(udp::v4());
			m_Socket.bind(udp::endpoint(address::from_string(ip), port));
		}

		void Run() {
			std::thread thread_receive(&UDPReceiver::receive, this);
			thread_receive.join();
		}
		

	private:

		void handle_receive(const boost::system::error_code& error, size_t bytes_transferred) {
			if (error) {
				std::cout << "Receive failed: " << error.message() << "\n";
				return;
			}
			std::cout << "Received: '" << std::string(m_RecvBuffer.begin(), m_RecvBuffer.begin() + bytes_transferred) << "' (" << error.message() << ")\n";
		}

		void wait() {
			m_Socket.async_receive_from(boost::asio::buffer(m_RecvBuffer),
				m_Endpoint,
				boost::bind(&UDPReceiver::handle_receive, this, boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred));
		}

		void receive() {
			wait();

			std::cout << "Receiving\n";
			io_service.run();
			std::cout << "Receiver exit\n";
		}

		boost::asio::io_service io_service;
		udp::socket m_Socket{ io_service };

		boost::array<char, 1024> m_RecvBuffer;;
		udp::endpoint m_Endpoint;
	};


}
