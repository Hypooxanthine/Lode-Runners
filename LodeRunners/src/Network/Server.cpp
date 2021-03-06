#include "Server.h"

#include <iostream>

namespace Network
{

	Server::Server()
	{
	}

	bool Server::create(const size_t& maxClients, const uint32_t& port, std::function<void(const size_t&, ByteArray&)> callback)
	{
		LOG_TRACE("Creating Server...");

		m_RequestedStop = false;
		m_MaxClients = maxClients;
		m_ClientsAcceptorRunning = false;
		m_Port = port;

		if (maxClients > 0)
		{
			if (!tryListen())
				return false;
			m_IsAcceptingClients = true;
			m_SinglePlayer = false;

			// Only for maxClients > 0. We're not supposed to need any callback in solo mode (maxClients = 0)
			m_Callback = callback;

			m_ClientsAcceptor = std::jthread([this]() { this->acceptClients(); });

			// Only for maxClients > 0. No clients => no data to accept => no callback to call.
			m_DataAcceptor = std::jthread([this]() {this->acceptData(); });

			LOG_INFO("Server created (multi-player network). Max clients : " + std::to_string(m_MaxClients));
		}
		else
		{
			m_IsAcceptingClients = false;
			m_SinglePlayer = true;
			LOG_INFO("Server created (single-player local).");
		}


		return true;
	}

	void Server::send(const size_t& GUID, ByteArray& args)
	{
		sf::Packet packet;

		packet << GUID;
		packet.append(args.data(), args.size());

		for (auto& c : m_Clients)
			c.second->send(packet);
	}

	void Server::stop()
	{
		m_RequestedStop = true;
		m_IsAcceptingClients = false;
		m_SinglePlayer = false;

		std::lock_guard<std::mutex> lock(m_ClientsLock);
		m_Selector.clear();

		for (auto& c : m_Clients)
			c.second->disconnect();

		m_Clients.clear();
		m_Listener.close();
		m_MaxClients = 0;
		m_Port = 0;

		LOG_INFO("Server closed.");
	}

	void Server::stopAcceptingClients()
	{
		m_IsAcceptingClients = false;
	}

	void Server::resumeAcceptClients()
	{
		m_IsAcceptingClients = true;
		
		if(!m_ClientsAcceptorRunning && m_MaxClients > 0)
		{
			ASSERT(tryListen(), "Server couldn't listen back to its asked port.");
			m_ClientsAcceptor = std::jthread([this]() {this->acceptClients(); });
		}
	}

	bool Server::tryListen()
	{
		if (m_Listener.listen(m_Port) != sf::Socket::Done)
		{
			LOG_WARN("Can't listen on port " + std::to_string(m_Port) + ".");
			return false;
		}
		else
		{
			LOG_INFO("Server listening on port " + std::to_string(m_Port) + ".");
			return true;
		}
	}

	void Server::acceptClients()
	{
		m_ClientsAcceptorRunning = true;

		// If the listner is blocking, it won't release this thread even if m_IsAcceptingClients is set to false, but we need this thread to check if it should acept clients or not !
		m_Listener.setBlocking(false);

		while(m_IsAcceptingClients && !m_RequestedStop && m_Clients.size() < m_MaxClients)
		{
			sf::sleep(sf::seconds(FIXED_NETWORK_DELTA_TIME_SECONDS));

			std::lock_guard<std::mutex> lock(m_ClientsLock);

			auto socket = std::make_unique<sf::TcpSocket>();
			socket->setBlocking(false);

			if(m_Listener.accept(*socket) == sf::Socket::Done)
			{
				size_t playerID = m_Clients.size() > 0 ?
					m_Clients.back().first + 1
					: 1;

				m_Clients.emplace_back(playerID, std::move(socket));
				m_Selector.add(*m_Clients.back().second);
				m_Listener.setBlocking(false);
				initClient(m_Clients.back());

				LOG_INFO("Client connected. Connected clients : " + std::to_string(m_Clients.size()) + "/" + std::to_string(m_MaxClients));
			}
		}

		std::lock_guard<std::mutex> lock(m_ClientsLock);
		m_Listener.close();
		LOG_INFO("Server stopped listening on port " + std::to_string(m_Port) + ".");

		m_ClientsAcceptorRunning = false;
	}

	void Server::initClient(std::pair<size_t, std::unique_ptr<sf::TcpSocket>>& client)
	{
		LOG_INFO("Initializing client " + std::to_string(client.first) + ".");
		sf::Packet packet;

		packet << size_t(0);
		packet << client.first;

		client.second->send(packet);
	}

	void Server::acceptData()
	{
		while (/*(m_Clients.size() > 0 || m_IsAcceptingClients || m_MaxClients == 0) && */!m_RequestedStop)
		{
			sf::sleep(sf::seconds(FIXED_NETWORK_DELTA_TIME_SECONDS));
			
			std::lock_guard<std::mutex> lock(m_ClientsLock);

			if(m_Selector.wait(sf::milliseconds(1)))
			{
				for (size_t i = m_Clients.size(); i > 0 ; i--)
				{
					auto& c = m_Clients[i-1];

					if (m_Selector.isReady(*c.second))
					{
						sf::Packet packet;
						sf::Socket::Status status = c.second->receive(packet);

						if (status != sf::Socket::Status::Done)
						{
							size_t logoutPlayerID = c.first;
							m_Selector.remove(*c.second);
							c.second->disconnect();
							m_Clients.erase(m_Clients.begin() + i - 1);
							m_OnClientLogout(logoutPlayerID);

							LOG_INFO("Client with ID " + std::to_string(logoutPlayerID) + " disconnected. Connected clients : " + std::to_string(m_Clients.size()) + " / " + std::to_string(m_MaxClients));

							if (m_IsAcceptingClients && !m_ClientsAcceptorRunning )
							{
								ASSERT(tryListen(), "Server couldn't listen back to its asked port.");
								m_ClientsAcceptor = std::jthread([this]() {this->acceptClients(); });
							}

						}
						else
						{
							if (packet.getDataSize() < sizeof(size_t)) // Invalid data
								continue;

							size_t GUID;
							ByteArray args;
							args.reserve(packet.getDataSize() - sizeof(size_t));

							packet >> GUID;
							std::byte* argsPtr = (std::byte*)packet.getData() + sizeof(size_t);
							std::byte* argsEnd = (std::byte*)packet.getData() + packet.getDataSize();
							for (argsPtr; argsPtr < argsEnd; argsPtr++)
								args.push_back(*argsPtr);

							m_Callback(GUID, args);
						}
					}
				}
			}
		}

		LOG_INFO("Server stopped accepting data.");

		// If we're here, it's not because stop were requested. So it's because all clients disconnected.
		if (!m_RequestedStop)
		{
			LOG_INFO("All clients disconnected.");
			m_OnAllClientsLogout();
		}
	}

}