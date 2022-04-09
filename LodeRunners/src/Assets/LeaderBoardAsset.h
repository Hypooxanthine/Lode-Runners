#pragma once

#include "../Core/Base.h"

class LeaderBoardAsset
{
public:
	LeaderBoardAsset() = default;
	LeaderBoardAsset(const LeaderBoardAsset& other) = default;
	LeaderBoardAsset& operator=(const LeaderBoardAsset& other) = default;

	void clear();

	const uint64_t& get(const std::string& playerName) const;
	uint64_t getOrDefault(const std::string& playerName, const uint64_t& defaultValue = 0);

	const std::map<std::string, uint64_t>& getData() const;

	void set(const std::string& playerName, const uint64_t& score);
	void addScore(const std::string& playerName, const uint64_t& score);

private:
	// std::map because we need to sort the leader board.
	std::map<std::string, uint64_t> m_Data;
};

