#pragma once

#include "Pawn.h"

class GoldTile;

class EnnemyPawn : public Pawn
{
public:
	EnnemyPawn(const size_t& ID, const std::string& name, TileMap* tileMap);

	void update(const float& dt) override;

	virtual void onBeginOverlap(Entity* other) override;
	virtual void onEndOverlap(Entity* other) override;

	void respawn();

private:
	GoldTile* m_CarriedGold = nullptr;

public: // Replicated functions

	CREATE_REPLICATED_FUNCTION
	(
		respawn_Multicast,
		[this]() {this->respawn(); },
		"EnnemyPawn" + std::to_string(getID()), Network::ReplicationMode::Multicast
	);
};
