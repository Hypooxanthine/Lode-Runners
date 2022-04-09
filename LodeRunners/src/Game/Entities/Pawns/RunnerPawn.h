#pragma once

#include "Pawn.h"

#include "../Tiles/BrickTile.h"

enum class DigTarget { Left, Right };

class RunnerPawn : public Pawn
{
public:
	RunnerPawn(const size_t& ID, const std::string& name, TileMap* tileMap);

	void update(const float& dt) override;

	virtual void onBeginOverlap(Entity* other) override;
	virtual void onEndOverlap(Entity* other) override;

	void kill();
	const bool& isKilled() const { return m_IsKilled; }

private: // Private member functions
	// This function is safe, but the return value may be unsafe.
	sf::Vector2u getDigTargetPos(const DigTarget& target) const;

	// No verification will be done with the parameter "pos".
	BrickTile* getBrick(const sf::Vector2u& pos) const;

	// This function is safe.
	std::optional<BrickTile*> getBrickDigTarget(const DigTarget& target) const;

private: // Private members

	bool m_IsKilled = false;

public: // Replicated functions
	
	// This function does the verifications, then if everything's okay, it executes its code on server then calls the OnClients replicated function.
	CREATE_REPLICATED_FUNCTION
	(
		dig_OnServer,
		[this](const DigTarget& target)
		{
			if (m_IsKilled) return;

			auto tile = this->getBrickDigTarget(target);

			if (!tile) return;

			LOG_TRACE("Server digging !");

			tile.value()->dig();

			dig_OnClients(getDigTargetPos(target));
		},
		"RunnerPawn" + std::to_string(getID()), Network::ReplicationMode::OnServer,
		const DigTarget&
	);

	// This function just executes its code on all clients, without any verification.
	CREATE_REPLICATED_FUNCTION
	(
		dig_OnClients,
		[this](const sf::Vector2u& pos)
		{
			LOG_TRACE("Client digging !");

			// We're using the unsafe function because every verifications were done by the server.
			getBrick(pos)->dig();
		},
		"RunnerPawn" + std::to_string(getID()), Network::ReplicationMode::OnClients,
		const sf::Vector2u&
	);

	CREATE_REPLICATED_FUNCTION
	(
		kill_Multicast,
		[this]() {this->kill(); },
		"RunnerPawn" + std::to_string(getID()), Network::ReplicationMode::Multicast
	);
};