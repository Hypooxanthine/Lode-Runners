#include "LeaderBoardAsset.h"

void LeaderBoardAsset::clear()
{
	m_Data.clear();
}

const uint64_t& LeaderBoardAsset::get(const std::string& playerName) const
{
	return m_Data.at(playerName);
}

uint64_t LeaderBoardAsset::getOrDefault(const std::string& playerName, const uint64_t& defaultValue)
{
	return m_Data.contains(playerName) ? m_Data.at(playerName) : defaultValue;
}

const std::map<std::string, uint64_t>& LeaderBoardAsset::getData() const
{
	return m_Data;
}

void LeaderBoardAsset::set(const std::string& playerName, const uint64_t& score)
{
	m_Data[playerName] = score;
}

void LeaderBoardAsset::addScore(const std::string& playerName, const uint64_t& score)
{
	set(playerName, getOrDefault(playerName) + score);
}
