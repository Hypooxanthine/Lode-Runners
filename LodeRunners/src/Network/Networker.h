#pragma once

#include "NetworkBase.h"

#include <unordered_map>
#include <queue>

#include "Server.h"
#include "Client.h"

#define IS_SERVER Network::Networker::get()->isServer()

namespace Network
{
	using ByteArray = std::vector<std::byte>;

	enum class ReplicationMode { NotReplicated, OnServer, OnClients, Multicast };
	enum class InterfaceType { None, Server, Client };

	class Networker
	{
	public:
		Networker();
		Networker(const Networker&) = delete;
		Networker(Networker&&) = delete;
		virtual ~Networker();

		static inline Networker* get() { return s_Instance; }

		// Will return false if Networker has not been set up (neither Server nor Client has been created).
		inline bool isServer() const { return m_InterfaceType == InterfaceType::Server; }

		// WARNING : unreliable value (won't crash) if no server was set up. Please check Network::Networker::isServer() first.
		inline const bool& isSinglePlayer() const { return m_Server.isSinglePlayer(); }

		// WARNING : unreliable value (won't crash) if neither server nor client was set up.
		inline const size_t& getPlayerID() const { return (m_InterfaceType == InterfaceType::Server ? m_Server.getPlayerID() : m_Client.getPlayerID()); }

		// To be considered : ound callbacks will only be triggered on server.
		inline void bindOnPlayerLogout(const std::function<void(const size_t&)>& callback) { m_OnPlayerLogoutCallbacks.push_back(callback); }

		bool createServer(const size_t& maxClients = 1, const uint32_t& port = 80);
		bool createClient(const std::string& address = "localhost", const uint32_t& port = 80);

		void registerFunc(std::function<void(ByteArray&)> func, const size_t& GUID);

		void call(const ReplicationMode& mode, const size_t& GUID, ByteArray& args);

		void executeCallQueue();

		void reset();

	private: // Private methods
		void fillCallQueue(const size_t& GUID, ByteArray& args);

	private: // Private members
		static Networker* s_Instance;

		Client m_Client;
		Server m_Server;

		std::unordered_map<size_t, std::function<void(ByteArray&)>> m_ReplicatedFunctions;
		std::queue<std::pair<size_t, ByteArray>> m_CallQueue; // FIFO array for call stack.
		std::mutex m_CallQueueMutex;

		std::vector<std::function<void(const size_t&)>> m_OnPlayerLogoutCallbacks;

		InterfaceType m_InterfaceType = InterfaceType::None;
	};

}