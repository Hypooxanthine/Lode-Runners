#pragma once

#include "../Core/Base.h"

class FontAsset
{
public:
	FontAsset() = default;
	FontAsset(const FontAsset&) = delete;

	const sf::Font& get() const { return m_Data; }
	operator const sf::Font& () const { return m_Data; }

	bool load(const std::string& path) { return m_Data.loadFromFile(path); }

private:
	sf::Font m_Data;
};

