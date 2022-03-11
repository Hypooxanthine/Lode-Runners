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
				this->fillCallQueue(GUID, args);
			}
		);

		if(!creationResult)
		{
			LOG_WARN("Couldn't create server. Aborting...");
			return false;
		}

		m_InterfaceType = InterfaceType::Server;
		m_Server.bindOnAllClientsDisconnected([this]() {this->reset(); });

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
				this->fillCallQueue(GUID, args);
			}
		);

		if(!creationResult)
		{
			LOG_WARN("Couldn't create client. Aborting...");
			return false;
		}

		m_InterfaceType = InterfaceType::Client;
		m_Client.bindOnServerDisconnected([this]() { this->reset(); });

		LOG_INFO("Networker set up.");

		return true;
	}

	void Networker::registerFunc(std::function<void(ByteArray&)> func, const size_t& GUID)
	{
		ASSERT(!m_Functions.contains(GUID), "Registered two replicated functions with the same GUID. Function details : " + func.target_type().name() + ".");	

		m_Functions[GUID] = func;
	}

	void Networker::call(const ReplicationMode& mode, const size_t& GUID, ByteArray& args)
	{
		#ifdef _DEBUG
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
		#endif

		if (m_InterfaceType == InterfaceType::None) return;

		if (mode == ReplicationMode::NotReplicated)
		{
			m_Functions.at(GUID)(args);
		}
		else if (mode == ReplicationMode::OnServer)
		{
			if (m_InterfaceType == InterfaceType::Server)
			{
				ASSERT(m_Functions.contains(GUID), "Trying to call an unregistered function. GUID : " + std::to_string(GUID) + ".");	
				m_Functions.at(GUID)(args);
			}
			else
			{
				LOG_TRACE("Trying to send data : from Client to Server. ReplicationMode : OnServer.");
				if (!m_Client.send(GUID, args))
					LOG_WARN("Couldn't send data.");
				else
					LOG_TRACE("Data sent.");
			}
		}
		else if (mode == ReplicationMode::Multicast && m_InterfaceType == InterfaceType::Server)
		{
			// Function is first called locally by the replicated function to avoid unnecessary packing/unpacking in single-player mode.
			m_Server.send(GUID, args);
		}
		else if (mode == ReplicationMode::OnClients && m_InterfaceType == InterfaceType::Server)
		{
			m_Server.send(GUID, args);
		}
	}

	void Networker::executeCallQueue()
	{
		std::lock_guard<std::mutex> lock(m_CallQueueMutex);

		while (!m_CallQueue.empty())
		{
			auto& element = m_CallQueue.front();

			call(ReplicationMode::NotReplicated, element.first, element.second);
			m_CallQueue.pop();
		}
	}

	void Networker::reset()
	{
		if (m_InterfaceType == InterfaceType::Server)
			m_Server.stop();
		else if (m_InterfaceType == InterfaceType::Client)
			m_Client.stop();

		m_InterfaceType = InterfaceType::None;
		LOG_INFO("Networker reset.");
	}

	void Networker::fillCallQueue(const size_t& GUID, ByteArray& args)
	{
		std::lock_guard<std::mutex> lock(m_CallQueueMutex);

		m_CallQueue.emplace(GUID, args);
	}

}