#pragma once

#include "NetworkBase.h"

#include <unordered_map>

#include "Server.h"
#include "Client.h"

namespace Network
{
	using ByteArray = std::vector<std::byte>;

	enum class ReplicationMode { NotReplicated, OnServer, Multicast };
	enum class InterfaceType { None, Server, Client };

	class Networker
	{
	public:
		Networker();
		Networker(const Networker&) = delete;
		Networker(Networker&&) = delete;
		virtual ~Networker();

		static inline Networker* get() { return s_Instance; }

		inline bool isServer() const { return m_InterfaceType == InterfaceType::Server; }

		// WARNING : unreliable value (won't crash) if no server was set up. Please check Network::Networker::isServer() first.
		inline const bool& isSinglePlayer() const { return m_Server.isSinglePlayer(); }

		bool createServer(const size_t& maxClients = 1, const uint32_t& port = 80);
		bool createClient(const std::string& address = "localhost", const uint32_t& port = 80);

		void registerFunc(std::function<void(ByteArray&)> func, const size_t& GUID);

		void call(const ReplicationMode& mode, const size_t& GUID, ByteArray& args);

		void reset();

	private:
		static Networker* s_Instance;

		Client m_Client;
		Server m_Server;

		std::unordered_map<size_t, std::function<void(ByteArray&)>> m_Functions;

		InterfaceType m_InterfaceType = InterfaceType::None;
	};

}