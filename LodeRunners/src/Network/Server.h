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

		inline void stopAcceptingClients() { m_IsAcceptingClients = false; }

		inline void bindOnAllClientsDisconnected(const std::function<void(void)>& callback) { m_OnAllClientsDisconnected = callback; }

	private:
		void acceptClients();
		void acceptData();

	private:
		std::vector<std::unique_ptr<sf::TcpSocket>> m_Clients;
		std::mutex m_ClientsLock;
		bool m_IsAcceptingClients = false;
		size_t m_MaxClients = 0;
		sf::TcpListener m_Listener;
		sf::SocketSelector m_Selector;

		std::function<void(const size_t&, ByteArray&)> m_Callback;

		std::function<void(void)> m_OnAllClientsDisconnected;

		std::jthread m_DataAcceptor;
		std::jthread m_ClientsAcceptor;
	};

}