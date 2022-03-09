#pragma once

#include "Base.h"

namespace Network
{

	class Server
	{
	public:
		Server();

		bool create(const size_t& maxClients, const uint32_t& port);
		void acceptData(std::function<void(const size_t&, ByteArray&)> callback);
		void send(const size_t& GUID, ByteArray& args);

	private:
		void acceptClients(const size_t& maxClients);
		void acceptData();

		bool everyoneDisconnected();

	private:
		std::vector<std::unique_ptr<sf::TcpSocket>> m_Clients;
		sf::TcpListener m_Listener;
		sf::SocketSelector m_Selector;

		std::function<void(const size_t&, ByteArray&)> m_Callback;

		std::jthread m_Acceptor;
	};

}