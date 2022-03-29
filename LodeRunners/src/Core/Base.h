#pragma once

// "Enum instead of enum class" warning from libraries...
#pragma warning(disable : 26812)

/* Defines */
#define TILES_WIDTH 26
#define TILES_HEIGHT 16

#define FIXED_FPS 120
#define FIXED_DELTA_TIME_SECONDS 1.f / float(FIXED_FPS)
#define DELTA_TIME_LOG_STEP 1.f

#define FIXED_NETWORK_DELTA_TIME_SECONDS FIXED_DELTA_TIME_SECONDS / 10.f

/* Main STL libraries. */
#include <iostream>	
#include <memory>
#include <functional>
#include <optional>
#include <string>
#include <unordered_map>
#include <map>
#include <vector>
#include <cinttypes>
#include <source_location> // C++20 is wonderful

/* SFML Graphic component */
#include <SFML/Graphics.hpp>

// We will be using shared pointers a lot. Shared pointers are not this expensive for a single referenced
// pointer compared to unique pointers, so we can use it by default to make the code easier to read.
// Thanks to TheCherno's "Hazel" game engine for this little code : https://github.com/TheCherno/Hazel
template<typename T>
using Ref = std::shared_ptr<T>;
template<typename T, typename ... Args>
constexpr Ref<T> MakeRef(Args&& ... args)
{
	return std::make_shared<T>(std::forward<Args>(args)...);
}

/* Logging. */
#include "Log.h"

/* Do not include Application.h in Application.h. */
#ifndef APPLICATION_CLASS_FILE
#include "Application.h"
#endif

/* Assertion system. */
#if defined(_DEBUG)
#define ASSERT(condition, msg) if(!(condition)) Application::get()->emergencyStop((std::string)"Assertion failed, file " + std::source_location::current().file_name() + ", line " + std::to_string(std::source_location::current().line()) + ". Details : " + msg)
#else
#define ASSERT(condition, msg) if(!(condition)) Application::get()->emergencyStop((std::string)"Assertion failed. Details : " + msg)
#endif

/* Callbacks */
using Callback = std::function<void(void)>;
#define BIND_FN(f) [this]() -> void { this->f(); }