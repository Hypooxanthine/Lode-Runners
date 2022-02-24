#pragma once

/* Defines */
#define SHOW_DELTA_TIME // For debugging purpose
#define DELTA_TIME_LOG_STEP 1.f

/* Main STL libraries. */
#include <iostream>	
#include <memory>
#include <thread>
#include <functional>
#include <optional>
#include <string>
#include <unordered_map>
#include <map>

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
#define ASSERT(condition, msg) if(!(condition)) Application::get()->emergencyStop((std::string)"Assertion failed, file " + __FILE__ + ", line " + std::to_string(__LINE__) + ". Details : " + msg)
#else
#define ASSERT(condition, msg) if(!(condition)) Application::get()->emergencyStop((std::string)"Assertion failed. " + msg)
#endif

/* Global defines */
#define TILES_WIDTH 26
#define TILES_HEIGHT 16