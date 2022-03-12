#pragma once

#include "NetworkBase.h"

#include <iostream>

#include "Networker.h"

// func parameter is for local function name.
// id parameter (string) is to identify the function, if multiple instances could use it.
// Variadic parameter pack is for parameter types.
#define CREATE_REPLICATED_FUNCTION(funcName, func, id, mode, ...) \
Network::ReplicatedFunc<__VA_ARGS__> funcName = Network::ReplicatedFunc<__VA_ARGS__>(func, std::hash<std::string>()((std::string)#funcName + id), mode)

namespace Network
{

	template <typename... Args>
	class ReplicatedFunc
	{
	public:
		ReplicatedFunc(std::function<void(Args...)> f, const size_t& GUID, const ReplicationMode& mode) 
			: m_Function(f), m_GUID(GUID), m_Mode(mode)
		{
			Networker::get()->registerFunc([this](ByteArray& buffer) -> void
				{
					this->call(buffer);
				}, m_GUID);
		}

		void call(const ReplicationMode& mode, Args&&... args)
		{
			#ifdef _DEBUG

			std::string logStr = "ReplicatedFunc::call called. Mode : ";
			switch (mode)
			{
			case ReplicationMode::NotReplicated:
				logStr += "NotReplicated";
				break;
			case ReplicationMode::OnServer:
				logStr += "OnServer";
				break;
			case ReplicationMode::OnClients:
				logStr += "OnClient";
				break;
			case ReplicationMode::Multicast:
				logStr += "Multicast";
			default:
				break;
			}
			logStr += ". From : ";
			logStr += Networker::get()->isServer() ? "Server." : "Client.";

			LOG_INFO(logStr);
			#endif

			Networker::get()->fillCallQueue(mode, m_GUID, convertToBuffer(std::forward<Args>(args)...));
		}

		void call(Args&&... args) { this->call(m_Mode, args...); }

		ByteArray& convertToBuffer(Args&&... args)
		{
			m_Buffer.clear();
			(serializeArg(args), ...);
			
			return m_Buffer;
		}

		const size_t& getGUID() const { return m_GUID; }

		void operator() (Args&&... args) { call(std::forward<Args>(args)...); }
		void operator() (const ReplicationMode& mode, Args&&... args) { call(mode, std::forward<Args>(args)...); }

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

			/*if (std::is_same<Raw, std::string>::value)
			{
				size_t size = *reinterpret_cast<size_t*>(&buffer[cursor]);
				cursor += sizeof(size_t);

				std::string str = reinterpret_cast<const char*>(buffer.data() + cursor);

				cursor += size;
			
				return str;
			}

			Raw* object = reinterpret_cast<Raw*>(buffer.data() + cursor);
			cursor += sizeof(Raw);
			return *object;*/

			Raw object = Raw();
			deserializeArg(object, buffer, cursor);
			return object;
		}

		template<typename T>
		void deserializeArg(T& receiver, ByteArray& buffer, size_t& cursor)
		{
			receiver = *reinterpret_cast<T*>(buffer.data() + cursor);
			cursor += sizeof(T);
		}

		template<>
		void deserializeArg(std::string& receiver, ByteArray& buffer, size_t& cursor)
		{
			size_t size = *reinterpret_cast<size_t*>(buffer.data() + cursor);
			cursor += sizeof(size_t);

			receiver = reinterpret_cast<const char*>(buffer.data() + cursor);

			cursor += size;
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

		ReplicationMode m_Mode = ReplicationMode::NotReplicated;
	};

}