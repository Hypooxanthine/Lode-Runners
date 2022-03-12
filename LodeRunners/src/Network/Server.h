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

		inline void bindOnClientLogout(const std::function<void(const size_t&)>& callback) { m_OnClientLogout = callback; }
		inline void bindOnAllClientsLogout(const std::function<void(void)>& callback) { m_OnAllClientsLogout = callback; }

		inline const size_t& getPlayerID() const { return m_playerID; }

	private: // Private methods
		bool tryListen();
		void acceptClients();
		void initClient(std::pair<size_t, std::unique_ptr<sf::TcpSocket>>& client);
		void acceptData();

	private: // Private members
		size_t m_playerID = 0; // Always 0 for server.

		std::vector<std::pair<size_t, std::unique_ptr<sf::TcpSocket>>> m_Clients;
		std::mutex m_ClientsLock;

		uint32_t m_Port = 0;

		bool m_IsAcceptingClients = false;
		size_t m_MaxClients = 0;

		bool m_SinglePlayer = false;

		sf::TcpListener m_Listener;
		sf::SocketSelector m_Selector;

		bool m_RequestedStop = false;

		// Replicated function call(ByteArray&) reference
		std::function<void(const size_t&, ByteArray&)> m_Callback;

		std::function<void(const size_t&)> m_OnClientLogout;
		std::function<void(void)> m_OnAllClientsLogout;

		std::jthread m_DataAcceptor;
		std::jthread m_ClientsAcceptor;
		bool m_ClientsAcceptorRunning = false;
	};

}