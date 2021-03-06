#pragma once

#include "NetworkBase.h"

namespace Network
{
	
	class Client
	{
	public:
		Client();

		bool create(const std::string& address, const uint32_t& port, std::function<void(const size_t&, ByteArray&)> callback);

		inline void bindOnServerDisconnected(const std::function<void(void)>& callback) { m_OnServerDisconnected = callback; }

		bool send(const size_t& GUID, ByteArray& args);

		void stop();

		inline const size_t& getPlayerID() const { return m_playerID; }

	private:
		void acceptData();

	private:
		size_t m_playerID = 0; // 0 only for initialization.

		std::unique_ptr<sf::TcpSocket> m_Server;
		std::mutex m_ServerMutex;

		bool m_RequestedStop = false;

		std::function<void(const size_t&, ByteArray&)> m_Callback;
		std::function<void(void)> m_OnServerDisconnected;

		std::jthread m_DataAcceptor;
	};

}