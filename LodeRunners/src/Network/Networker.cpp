#include "Networker.h"

#include <iostream>

namespace Network
{

	Networker* Networker::s_Instance = nullptr;

	Networker::Networker()
	{
		ASSERT(s_Instance == nullptr, "Network instance already exists.");
		s_Instance = this;
	}

	Networker::~Networker()
	{
		reset();
	}

	bool Networker::createServer(const size_t& maxClients, const uint32_t& port)
	{
		if (m_InterfaceType != InterfaceType::None)
		{
			const std::string interfaceTypeStr = (m_InterfaceType == InterfaceType::Server ? "server" : "client");
			LOG_WARN("Couldn't create server : an instance of " + interfaceTypeStr + " already exists.");
			return false;
		}

		const bool creationResult = m_Server.create
		(
			maxClients,
			port,
			[this](const size_t& GUID, ByteArray& args)
			{
				this->fillCallQueue(ReplicationMode::NotReplicated, GUID, args);
			}
		);

		if(!creationResult)
		{
			LOG_WARN("Couldn't create server. Aborting...");
			return false;
		}

		m_InterfaceType = InterfaceType::Server;
		m_Server.bindOnClientLogout
		(
			[this](const size_t& id) 
			{
				std::for_each
				(
					m_OnPlayerLogoutCallbacks.rbegin(), m_OnPlayerLogoutCallbacks.rend(), 
					[&](const std::function<void(const size_t&)>& cb) 
					{ 
						cb(id); 
					}
				);
			}
		);
		m_Server.bindOnAllClientsLogout([this]() {this->reset(); });

		LOG_INFO("Networker set up.");

		return true;
	}

	bool Networker::createClient(const std::string& address, const uint32_t& port)
	{
		if (m_InterfaceType != InterfaceType::None)
		{
			const std::string interfaceTypeStr = (m_InterfaceType == InterfaceType::Server ? "server" : "client");
			LOG_WARN("Couldn't create client : an instance of " + interfaceTypeStr + " already exists.");
			return false;
		}

		const bool creationResult = m_Client.create
		(
			address,
			port,
			[this](const size_t& GUID, ByteArray& args)
			{
				this->fillCallQueue(ReplicationMode::NotReplicated, GUID, args);
			}
		);

		if(!creationResult)
		{
			LOG_WARN("Couldn't create client. Aborting...");
			return false;
		}

		m_InterfaceType = InterfaceType::Client;
		m_Client.bindOnServerDisconnected
		(
			[this]()
			{
				std::for_each
				(
					m_OnServerConnexionLostCallbacks.rbegin(), m_OnServerConnexionLostCallbacks.rend(),
					[](const std::function<void(void)>& cb)
					{
						cb();
					}
				);
				this->reset();
			}
		);

		LOG_INFO("Networker set up.");

		return true;
	}

	void Networker::registerFunc(std::function<void(ByteArray&)> func, const size_t& GUID)
	{
		ASSERT(!m_ReplicatedFunctions.contains(GUID), "Registered two replicated functions with the same GUID. Function details : " + func.target_type().name() + ".");	

		m_ReplicatedFunctions[GUID] = func;
	}

	void Networker::unregisterFunc(const size_t& GUID)
	{
		ASSERT(m_ReplicatedFunctions.contains(GUID), "Couldn't find ReplicatedFunc with GUID " + std::to_string(GUID) + " to unregister.");
		m_ReplicatedFunctions.erase(GUID);
	}

	void Networker::call(const ReplicationMode& mode, const size_t& GUID, ByteArray& args)
	{
		/*#ifdef _DEBUG
		std::string debugStr = "Networker's call function invoked. Mode : ";

		switch (mode)
		{
		case ReplicationMode::NotReplicated:
			debugStr += "NotReplicated";
			break;
		case ReplicationMode::OnServer:
			debugStr += "OnServer";
			break;
		case ReplicationMode::OnClients:
			debugStr += "OnClients";
			break;
		case ReplicationMode::Multicast:
			debugStr += "Multicast";
			break;
		default:
			break;
		}

		debugStr += ".";

		LOG_INFO(debugStr);
		#endif*/

		if (m_InterfaceType == InterfaceType::None) return;

		if (mode == ReplicationMode::NotReplicated)
		{
			m_ReplicatedFunctions.at(GUID)(args);
		}
		else if (mode == ReplicationMode::OnServer)
		{
			if (m_InterfaceType == InterfaceType::Server)
			{
				ASSERT(m_ReplicatedFunctions.contains(GUID), "Trying to call an unregistered function. GUID : " + std::to_string(GUID) + ".");	
				m_ReplicatedFunctions.at(GUID)(args);
			}
			else
			{
				/*LOG_TRACE("Trying to send data : from Client to Server. ReplicationMode : OnServer.");
				if (!m_Client.send(GUID, args))
					LOG_WARN("Couldn't send data.");
				else
					LOG_TRACE("Data sent.");*/

				m_Client.send(GUID, args);
			}
		}
		else if (mode == ReplicationMode::Multicast && m_InterfaceType == InterfaceType::Server)
		{
			m_ReplicatedFunctions.at(GUID)(args);
			m_Server.send(GUID, args);
		}
		else if (mode == ReplicationMode::OnClients && m_InterfaceType == InterfaceType::Server)
		{
			m_Server.send(GUID, args);
		}
	}

	void Networker::executeCallQueue()
	{
		while (!m_CallQueue.empty())
		{
			std::tuple<ReplicationMode, size_t, ByteArray> callCopy;

			{
				/*
				 * This is quite important.
				 * This function may, and will, call functions that will add new calls to the call queue.
				 * That being said, there is a high risk of deadlock here, because this scope could take the ownership
				 * of the lock, then call a function that adds a call to the call queue, but... The lock ownership is already
				 * taken. The problem is that we can't delay calls that add new calls, we MUST execute calls in the exact same
				 * order that they were sent. We cannot neither not use locks here, because the fastest the network is,
				 * the fastest the call queue is filled. So when we call a function, then pop the call from the call queue, if
				 * a call is added in between, all the data just goes wild because of std::queue internally dynamic memory allocation.
				 * So we copy the next call, then pop it from the queue.
				 * Then we release de the lock and perform the call. The call may fill the call queue, but there is
				 * no problem because we're working with a copy of the call.
				 * Copying is not great in general, but I couldn't find any other solution that wouldn't lead to
				 * a deadlock here.
				 */

				std::lock_guard<std::mutex> lock(m_CallQueueMutex);
				callCopy = m_CallQueue.front();
				m_CallQueue.pop();
			}

			call(std::get<0>(callCopy), std::get<1>(callCopy), std::get<2>(callCopy));
		}
	}

	void Networker::stopAcceptingClients()
	{
		if (m_InterfaceType == InterfaceType::Server)
			m_Server.stopAcceptingClients();
	}

	void Networker::acceptClients()
	{
		if (m_InterfaceType == InterfaceType::Server)
			m_Server.resumeAcceptClients();
	}

	void Networker::reset()
	{
		if (m_InterfaceType == InterfaceType::Server)
			m_Server.stop();
		else if (m_InterfaceType == InterfaceType::Client)
			m_Client.stop();

		m_OnPlayerLogoutCallbacks.clear();
		m_OnServerConnexionLostCallbacks.clear();

		m_InterfaceType = InterfaceType::None;
		LOG_INFO("Networker reset.");
	}

	void Networker::fillCallQueue(const ReplicationMode& mode, const size_t& GUID, ByteArray& args)
	{
		std::lock_guard<std::mutex> lock(m_CallQueueMutex);
		m_CallQueue.emplace(mode, GUID, args);
	}

}