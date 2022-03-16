#include "Client.h"

#include <iostream>

namespace Network
{

	Client::Client()
	{
	}

	bool Client::create(const std::string& address, const uint32_t& port, std::function<void(const size_t&, ByteArray&)> callback)
	{
		LOG_TRACE("Creating Client...");

		m_Server = std::make_unique<sf::TcpSocket>();
		
		if (m_Server->connect(address, port) == sf::Socket::Error)
		{
			LOG_WARN("Couldn't connect to " + address + ":" + std::to_string(port) + ".");
			return false;
		}
		
		m_RequestedStop = false;
		m_Callback = callback;

		// Waiting for initialization by the server.
		sf::Packet packet;
		
		if(m_Server->receive(packet) == sf::Socket::Status::Done) // Blocking at first, client NEEDS to get its player ID.
		{
			size_t GUID;
			packet >> GUID;

			if (GUID == 0) // 0 GUID is for client initialization.
			{
				packet >> m_playerID;
				LOG_INFO("Initialized with player ID " + std::to_string(m_playerID) + ".");
			}
			else
			{
				LOG_WARN("Couldn't initialize : first packet received doesn't fit the player ID packet formatting requirements.");
				return false;
			}
		}
		else
		{
			LOG_WARN("Couldn't initialize : no player ID sent.");
			return false;
		}

		// Now we set socket to not blocking, otherwise the lock won't be freed until client receives anything.
		m_Server->setBlocking(false);
		m_DataAcceptor = std::jthread([this]() {this->acceptData(); });

		LOG_INFO("Client created.");
		return true;
	}

	bool Client::send(const size_t& GUID, ByteArray& args)
	{
		std::lock_guard<std::mutex> lock(m_ServerMutex);
		sf::Packet packet;

		packet << GUID;
		packet.append(args.data(), args.size());
		if (m_Server->send(packet) != sf::Socket::Done)
			return false;
		else
			return true;
	}

	void Client::stop()
	{
		m_RequestedStop = true;

		// We don't want to disconnect our client while it's still receiving/sending data.
		std::lock_guard<std::mutex> lock(m_ServerMutex);
		m_Server->disconnect();

		LOG_INFO("Client closed.");
	}

	void Client::acceptData()
	{
		sf::Socket::Status status = sf::Socket::Status::Done;
		
		while (status != sf::Socket::Status::Disconnected
			&& status != sf::Socket::Status::Error
			&& !m_RequestedStop)
		{
			sf::sleep(sf::seconds(FIXED_NETWORK_DELTA_TIME_SECONDS));

			std::lock_guard<std::mutex> lock(m_ServerMutex);
			
			sf::Packet packet;
			status = m_Server->receive(packet);

			if (status == sf::Socket::Status::Done)
			{
				if (packet.getDataSize() < sizeof(size_t)) // Invalid data
					continue;

				size_t GUID;
				packet >> GUID;

				ByteArray args;
				args.reserve(packet.getDataSize() - sizeof(size_t));

				std::byte* argsPtr = (std::byte*)packet.getData() + sizeof(size_t);
				std::byte* argsEnd = (std::byte*)packet.getData() + packet.getDataSize();
				for (argsPtr; argsPtr < argsEnd; argsPtr++)
					args.push_back(*argsPtr);

				m_Callback(GUID, args);
				packet.clear();
			}
		}

		LOG_INFO("Client stopped accepting data.");

		// If we're here, it's not because stop were requested. So it's because the server has disconnected.
		if (!m_RequestedStop)
		{
			LOG_INFO("Server disconnected.");
			m_OnServerDisconnected();
		}

	}

}