#include "Server.h"

#include <iostream>

#pragma warning(disable : 26812) // "Enum instead of enum class" warning from SFML...

namespace Network
{

	Server::Server()
	{
	}

	Server::~Server()
	{
		stop();
	}

	bool Server::create(const size_t& maxClients, const uint32_t& port, std::function<void(const size_t&, ByteArray&)> callback)
	{
		LOG_TRACE("Creating Server...");

		if (m_Listener.listen(port) != sf::Socket::Done)
		{
			LOG_WARN("Can't listen on port " + std::to_string(port) + ".");
			return false;
		}

		m_IsAcceptingClients = true;
		m_RequestedStop = false;
		m_MaxClients = maxClients;
		m_Callback = callback;

		m_ClientsAcceptor = std::jthread([this]() { this->acceptClients(); });
		m_DataAcceptor = std::jthread([this]() {this->acceptData(); });	

		LOG_INFO("Server created.");

		return true;
	}

	void Server::send(const size_t& GUID, ByteArray& args)
	{
		LOG_INFO("Sending from server to all clients.");
		sf::Packet packet;

		packet << GUID;
		packet.append(args.data(), args.size());

		for (auto& c : m_Clients)
			c->send(packet);
	}

	void Server::stop()
	{
		m_IsAcceptingClients = false;
		m_RequestedStop = true;

		std::lock_guard<std::mutex> lock(m_ClientsLock);
		m_Callback = nullptr;
		m_Selector.clear();

		for (auto& c : m_Clients)
			c->disconnect();

		m_Clients.clear();
		m_Listener.close();
		m_MaxClients = 0;

		LOG_INFO("Server closed.");
	}

	void Server::acceptClients()
	{
		// If the listner is blocking, it won't release this thread even if m_IsAcceptingClients is set to false, but we need this thread to check if it should acept clients or not !
		m_Listener.setBlocking(false);

		while(m_IsAcceptingClients && !m_RequestedStop)
		{
			std::lock_guard<std::mutex> lock(m_ClientsLock);

			if (m_Clients.size() < m_MaxClients)
			{
				auto socket = std::make_unique<sf::TcpSocket>();

				if(m_Listener.accept(*socket) == sf::Socket::Done)
				{
					m_Selector.add(*socket);
					m_Clients.push_back(std::move(socket));

					LOG_INFO("Client connected. Connected clients : " + std::to_string(m_Clients.size()) + "/" + std::to_string(m_MaxClients));
				}
			}
		}

		std::lock_guard<std::mutex> lock(m_ClientsLock);
		m_Listener.close();
	}

	void Server::acceptData()
	{
		while ((m_Clients.size() > 0 || m_IsAcceptingClients) && !m_RequestedStop)
		{
			std::lock_guard<std::mutex> lock(m_ClientsLock);

			if(m_Selector.wait(sf::microseconds(50)))
			{
				LOG_INFO("Data received.");

				for (size_t i = m_Clients.size(); i > 0 ; i--)
				{
					auto& c = m_Clients[i-1];

					if (m_Selector.isReady(*c))
					{
						LOG_TRACE("Server received data.");

						sf::Packet packet;
						sf::Socket::Status status = c->receive(packet);

						if (status == sf::Socket::Status::Disconnected || status == sf::Socket::Status::Error)
						{
							m_Selector.remove(*c);
							m_Clients.erase(m_Clients.begin() + i - 1);
							LOG_INFO("Client disconnected. Connected clients : " + std::to_string(m_Clients.size()) + "/" + std::to_string(m_MaxClients));
						}
						else if(status == sf::Socket::Status::Done)
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
			m_OnAllClientsDisconnected();
		}
	}

}