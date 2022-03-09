#pragma once

#include <vector>
#include <functional>
#include <cinttypes>
#include <string>
#include <memory>
#include <thread>

#include <SFML/Network.hpp>

namespace Network
{
	using ByteArray = std::vector<std::byte>;
}

#ifdef _DEBUG
#define DEBUG_ONLY(x) x
#else
#define DEBUG_ONLY(x)	
#endif