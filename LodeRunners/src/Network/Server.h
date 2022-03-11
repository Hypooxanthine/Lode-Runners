#pragma once

#include "NetworkBase.h"

namespace Network
{

	class Server
	{
	public:
		Server();

		inline const bool& isSinglePlayer() const { return m_SinglePlayer; }

		bool create(const size_t& maxClients, const uint32_t& port, std::function<void(const size_t&, ByteArray&)> callback);
		void send(const size_t& GUID, ByteArray& args);

		void stop();
		void stopAcceptingClients();

		inline void bindOnAllClientsDisconnected(const std::function<void(void)>& callback) { m_OnAllClientsDisconnected = callback; }

	private: // Private methods
		bool tryListen(const uint32_t& port);
		void acceptClients();
		void acceptData();

	private: // Private members
		std::vector<std::unique_ptr<sf::TcpSocket>> m_Clients;
		std::mutex m_ClientsLock;
		std::mutex m_MaxClientsLock;

		bool m_IsAcceptingClients = false;
		size_t m_MaxClients = 0;

		bool m_SinglePlayer = false;

		sf::TcpListener m_Listener;
		sf::SocketSelector m_Selector;

		bool m_RequestedStop = false;

		std::function<void(const size_t&, ByteArray&)> m_Callback;

		std::function<void(void)> m_OnAllClientsDisconnected;

		std::jthread m_DataAcceptor;
		std::jthread m_ClientsAcceptor;
	};

}