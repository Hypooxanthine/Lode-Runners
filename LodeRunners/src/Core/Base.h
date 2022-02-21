#pragma once

/* Defines */
#define SHOW_DELTA_TIME // For debugging purpose
#define DELTA_TIME_LOG_STEP 1.f

/* Main STL libraries. */
#include <iostream>
#include <memory>
#include <functional>
#include <optional>
#include <string>

/* SFML Graphic component */
#include <SFML/Graphics.hpp>

// We will be using shared pointers a lot. Shared pointers are not this expensive for a single referenced
// pointer compared to unique pointers, so we can use it by default to make the code easier to read.
template<typename T>
using Ref = std::shared_ptr<T>;
template<typename T, typename ... Args>
constexpr Ref<T> MakeRef(Args&& ... args)
{
	return std::make_shared<T>(std::forward<Args>(args)...);
}

/* Logging. */
#include "Log.h"

/* XML parser. */
#include "tinyxml2.h"