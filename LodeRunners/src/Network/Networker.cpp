#include "Networker.h"

#include <iostream>

namespace Network
{

	Networker* Networker::s_Instance = nullptr;

	Networker::Networker()
	{
		// ASSERT(s_Instance == nullptr, "Network instance already exists.");
		s_Instance = this;
	}

	bool Networker::createServer(const size_t& maxClients, const uint32_t& port)
	{
		if (m_InterfaceType != InterfaceType::None) return false;
		LOG_TRACE("Creating Server.");

		if (!m_Server.create(maxClients, port))
			return false;
		LOG_INFO("Server connected successfully to " + std::to_string(maxClients) + " client.");

		m_Server.acceptData([this](const size_t& GUID, ByteArray& args)
			{
				this->call(ReplicationMode::NotReplicated, GUID, args);
			});

		LOG_INFO("Server listening to entering data.");

		m_InterfaceType = InterfaceType::Server;
		m_Server.bindOnAllClientsDisconnected([this]() {this->reset(); });

		return true;
	}

	bool Networker::createClient(const std::string& address, const uint32_t& port)
	{
		if (m_InterfaceType != InterfaceType::None) return false;
		LOG_TRACE("Creating Client.");

		if (!m_Client.create(address, port))
		{
			LOG_WARN("Aborting...");
			return false;
		}
		LOG_INFO("Client connected successfully to server.");

		m_Client.acceptData([this](const size_t& GUID, ByteArray& args)
			{
				this->call(ReplicationMode::NotReplicated, GUID, args);
			});

		LOG_INFO("Client listening to entering data.");

		m_InterfaceType = InterfaceType::Client;
		m_Client.bindOnServerDisconnected([this]() { this->reset(); });

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
		std::string fromStr;

		switch (mode)
		{
		case ReplicationMode::NotReplicated:
			fromStr = "NotReplicated";
			break;
		case ReplicationMode::OnServer:
			fromStr = "OnServer";
			break;
		case ReplicationMode::Multicast:
			fromStr = "Multicast";
			break;
		default:
			break;
		}

		LOG_INFO("Networker's call function invoked. From : ", (isServer() ? "Server" : "Client"), ". Mode : ", fromStr, ".");
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
				LOG_INFO("Trying to send data : from Client to Server. ReplicationMode : OnServer.");
				if (!m_Client.send(GUID, args))
					LOG_WARN("Couldn't send data.");
				else
					LOG_TRACE("Data sent.");
			}
		}
		else if (mode == ReplicationMode::Multicast)
		{
			if (m_InterfaceType == InterfaceType::Server) // Multicast asked by server
			{
				m_Functions.at(GUID)(args);
				m_Server.send(GUID, args);
			}
		}
	}

	void Networker::reset()
	{
		m_InterfaceType = InterfaceType::None;
		LOG_INFO("Networker reset.");
	}

}