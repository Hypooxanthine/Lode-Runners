#pragma once

#include "Base.h"

namespace Network
{
	
	class Client
	{
	public:
		Client();
		virtual ~Client();

		bool create(const std::string& address, const uint32_t& port);

		inline void bindOnServerDisconnected(const std::function<void(void)>& callback) { m_OnServerDisconnected = callback; }

		bool send(const size_t& GUID, ByteArray& args);
		void acceptData(std::function<void(const size_t&, ByteArray&)> callback);

		inline void stop() { m_Server->disconnect(); }

	private:
		void acceptData();

	private:
		std::unique_ptr<sf::TcpSocket> m_Server;
		std::function<void(const size_t&, ByteArray&)> m_Callback;

		std::function<void(void)> m_OnServerDisconnected;

		std::jthread m_DataAcceptor;
	};

}