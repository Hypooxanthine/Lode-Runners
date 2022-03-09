#pragma once

#include "Base.h"

#include <iostream>

#include "Networker.h"

// func parameter is for local function name.
// id parameter (string) is to identify the function, if multiple instances could use it.
// Variadic parameter pack is for parameter types.
#define CREATE_REPLICATED_FUNCTION(funcName, func, id, ...) \
auto funcName##Replicated = Network::ReplicatedFunc<__VA_ARGS__>(func, std::hash<std::string>()((std::string)#funcName + id))

#define GET_REPLICATED_FUNCTION(funcName) funcName##Replicated

namespace Network
{

	template <typename... Args>
	class ReplicatedFunc
	{
	public:
		ReplicatedFunc(std::function<void(Args...)> f, const size_t& GUID) : m_Function(f), m_GUID(GUID)
		{
			Networker::get()->registerFunc([this](ByteArray& buffer) -> void
				{
					this->call(buffer);
				}, m_GUID);
		}

		void call(const ReplicationMode& mode, Args&&... args)
		{
			LOG_INFO("ReplicatedFunc::call called. From : ", (Networker::get()->isServer() ? "Server" : "Client"), ". Mode : ");

			#ifdef _DEBUG
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
			default:
				break;
			}
			std::cout << ".\n";
			#endif


			switch (mode)
			{
			case ReplicationMode::NotReplicated:
				m_Function(std::forward<Args>(args)...);
				break;
			case ReplicationMode::OnServer:
			case ReplicationMode::Multicast:
				Networker::get()->call(mode, m_GUID, convertToBuffer(std::forward<Args>(args)...));
				break;
			default:
				break;
			}
		}

		ByteArray& convertToBuffer(Args&&... args)
		{
			m_Buffer.clear();
			(serializeArg(args), ...);

			return m_Buffer;
		}

		const size_t& getGUID() const { return m_GUID; }

		void operator() (Args&&... args) { call(std::forward<Args>(args)...); }
		void operator() (char* pack) { call(pack); }

	private:
		void call(ByteArray& buffer)
		{
			size_t cursor = 0;

			m_Function(deserializeArgs<Args>(buffer, cursor)...);
		}

		template<typename T>
		std::remove_reference<T>::type deserializeArgs(ByteArray& buffer, size_t& cursor)
		{
			using Raw = std::remove_const<std::remove_reference<T>::type>::type;

			if (std::is_same<Raw, std::string>::value)
			{
				size_t size = *reinterpret_cast<size_t*>(&buffer[cursor]);
				cursor += sizeof(size_t);

				std::string str = reinterpret_cast<const char*>(buffer.data() + cursor);

				cursor += size;
			
				return str;
			}

			Raw* object = reinterpret_cast<Raw*>(buffer.data() + cursor);
			cursor += sizeof(Raw);
			return *object;
		}

		template<typename T>
		void serializeArg(const T& arg)
		{
			using Raw = std::remove_reference<T>::type;

			std::byte* asByte = (std::byte*)&arg;
			ByteArray asArrayByte;

			for (size_t i = 0; i < sizeof(Raw); i++)
				asArrayByte.push_back(*(asByte + i));

			m_Buffer.insert(m_Buffer.begin(), asArrayByte.begin(), asArrayByte.end());
		}

		template<>
		void serializeArg(const std::string& arg)
		{
			ByteArray serialized;
			size_t size = arg.size() + 1; // +1 for '\0'
			const std::byte* sizeAsBytePtr = reinterpret_cast<const std::byte*>(&size);
			
			// Filling with size_t as bytes
			for (size_t i = 0; i < sizeof(size_t); i++)
				serialized.push_back(sizeAsBytePtr[i]);
		
			const std::byte* dataAsBytePtr = reinterpret_cast<const std::byte*>(arg.data());
		
			for (size_t i = 0; i < size; i++)
				serialized.push_back(dataAsBytePtr[i]);
		
			m_Buffer.insert(m_Buffer.begin(), serialized.begin(), serialized.end());
		}

	private:
		std::function<void(Args...)> m_Function;

		ByteArray m_Buffer;

		size_t m_GUID = "";
	};

}