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

#include "../Core/Log.h"

#include "../Core/Base.h"