#pragma once

#include "../Core/Base.h"
#include "../Assets/Assets.h"

// x: 26
// y: 16

class Level
{
public: // Constructors

	Level();
	Level(const Level&) = delete;
	Level(const Level&&) = delete;

public: // Pubic methods

	// Loads a level from "name" level name
	bool load(const char* name);

	// Saves the currently loaded level with "name" name
	void save(const char* name);

	// Saves the currently loaded level with the same name as loaded
	void save();

private: // Private attributes

	
};

