#pragma once

#include "Base.h"

namespace Network
{

	class Client
	{
	public:
		Client();

		bool create(const std::string& address, const uint32_t& port);

		bool send(const size_t& GUID, ByteArray& args);
		void acceptData(std::function<void(const size_t&, ByteArray&)> callback);

	private:
		void acceptData();

	private:
		std::unique_ptr<sf::TcpSocket> m_Server;
		std::function<void(const size_t&, ByteArray&)> m_Callback;

		std::jthread m_Acceptor;
	};

}