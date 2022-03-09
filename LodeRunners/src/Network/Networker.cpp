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
		DEBUG_ONLY(std::cout << "Creating Networker.\n");

		if (!m_Server.create(maxClients, port))
			return false;
		DEBUG_ONLY(std::cout << "Server connected successfully to 1 client.\n");

		m_Server.acceptData([this](const size_t& GUID, ByteArray& args)
			{
				this->call(ReplicationMode::NotReplicated, GUID, args);
			});

		DEBUG_ONLY(std::cout << "Server listening to entering data.\n");

		m_InterfaceType = InterfaceType::Server;
		return true;
	}

	bool Networker::createClient(const std::string& address, const uint32_t& port)
	{
		if (m_InterfaceType != InterfaceType::None) return false;
		DEBUG_ONLY(std::cout << "Creating Networker.\n");

		if (!m_Client.create(address, port))
			return false;
		DEBUG_ONLY(std::cout << "Client connected successfully to server.\n");

		m_Client.acceptData([this](const size_t& GUID, ByteArray& args)
			{
				this->call(ReplicationMode::NotReplicated, GUID, args);
			});

		DEBUG_ONLY(std::cout << "Client listening to entering data.\n");

		m_InterfaceType = InterfaceType::Client;
		return true;
	}

	void Networker::registerFunc(std::function<void(ByteArray&)> func, const size_t& GUID)
	{
		// ASSERT(!m_Functions.contains(GUID), "Registered two replicated functions with the same GUID. Function details : " + func.target_type().name() + ".");	

		m_Functions[GUID] = func;
	}

	void Networker::call(const ReplicationMode& mode, const size_t& GUID, ByteArray& args)
	{
		#ifdef _DEBUG
		std::cout << "Networker's call function invoked. From : " << (isServer() ? "Server" : "Client") << ". Mode : ";

		switch (mode)
		{
		case ReplicationMode::NotReplicated:
			std::cout << "NotReplicated";
			break;
		case ReplicationMode::OnServer:
			std::cout << "OnServer";
			break;
		case ReplicationMode::Multicast:
			std::cout << "Multicast";
			break;
		default:
			break;
		}
		std::cout << ".\n";
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
				//ASSERT(m_Functions.contains(GUID), "Trying to call an unregistered function. Details : " + func.target_type().name() + ".");	
				m_Functions.at(GUID)(args);
			}
			else
			{
				DEBUG_ONLY(std::cout << "Trying to send data : from Client to Server. ReplicationMode : OnServer.\n");
				if (!m_Client.send(GUID, args))
					DEBUG_ONLY(std::cerr << "Couldn't send data.\n");
				else
					DEBUG_ONLY(std::cout << "Data sent.\n");
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

}