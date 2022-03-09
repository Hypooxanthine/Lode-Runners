#include "Client.h"

#include <iostream>

#pragma warning(disable : 26812) // "Enum instead of enum class" warning from SFML...

namespace Network
{

	Client::Client()
	{
	}

	bool Client::create(const std::string& address, const uint32_t& port)
	{
		m_Server = std::make_unique<sf::TcpSocket>();
		
		if (m_Server->connect(address, port) == sf::Socket::Error) return false;
		else return true;
	}

	bool Client::send(const size_t& GUID, ByteArray& args)
	{
		sf::Packet packet;

		packet << GUID;
		packet.append(args.data(), args.size());
		if (m_Server->send(packet) != sf::Socket::Done)
			return false;
		else
			return true;
	}

	void Client::acceptData(std::function<void(const size_t&, ByteArray&)> callback)
	{
		m_Callback = callback;

		m_Acceptor = std::jthread([this]() {this->acceptData(); });
	}

	void Client::acceptData()
	{
		sf::Socket::Status status = sf::Socket::Status::Done;
		
		while (status != sf::Socket::Status::Disconnected)
		{
			sf::Packet packet;
			status = m_Server->receive(packet);


			if (status == sf::Socket::Status::Done)
			{
				if (packet.getDataSize() < sizeof(size_t)) // When pinged
				{
					packet.clear();
					continue;
				}

				LOG_TRACE("Packet received.");

				size_t GUID;
				ByteArray args;
				args.reserve(packet.getDataSize() - sizeof(size_t));

				packet >> GUID;
				std::byte* argsPtr = (std::byte*)packet.getData() + sizeof(size_t);
				std::byte* argsEnd = (std::byte*)packet.getData() + packet.getDataSize();
				for (argsPtr; argsPtr < argsEnd; argsPtr++)
					args.push_back(*argsPtr);

				m_Callback(GUID, args);
				packet.clear();
			}
		}

		LOG_INFO("Server disconnected.");
	}

}