#include "Server.h"

#include <iostream>

#pragma warning(disable : 26812) // "Enum instead of enum class" warning from SFML...

namespace Network
{

	Server::Server()
	{
	}

	bool Server::create(const size_t& maxClients, const uint32_t& port)
	{
		if (m_Listener.listen(port) != sf::Socket::Done) return false;

		acceptClients(maxClients);

		return true;
	}

	void Server::acceptData(std::function<void(const size_t&, ByteArray&)> callback)
	{
		m_Callback = callback;

		m_Acceptor = std::jthread([this]() {this->acceptData(); });		
	}

	void Server::send(const size_t& GUID, ByteArray& args)
	{
		DEBUG_ONLY(std::cout << "Sending from server to all clients." << std::endl);
		sf::Packet packet;

		packet << GUID;
		packet.append(args.data(), args.size());

		for (auto& c : m_Clients)
			c->send(packet);
	}

	void Server::acceptClients(const size_t& maxClients)
	{
		while(m_Clients.size() < maxClients)
		{
			auto socket = std::make_unique<sf::TcpSocket>();
			m_Listener.accept(*socket);
			m_Selector.add(*socket);
			m_Clients.push_back(std::move(socket));
		}

	}

	void Server::acceptData()
	{
		while (true)
		{
			if(m_Selector.wait(sf::milliseconds(50)))
			{
				DEBUG_ONLY(std::cout << "Data received.\n");

				for (auto& c : m_Clients)
				{
					if (m_Selector.isReady(*c))
					{
						DEBUG_ONLY(std::cout << "Server received data.\n");

						sf::Packet packet;
						c->receive(packet);

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

		DEBUG_ONLY(std::cout << "Server stopped accepting data.\n");
	}

}