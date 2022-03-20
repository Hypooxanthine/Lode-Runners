#pragma once

#include "Pawn.h"

class GoldTile;

class EnnemyPawn : public Pawn
{
public:
	EnnemyPawn(const size_t& ID, const std::string& name);

	void update(const float& dt) override;

	virtual void onBeginOverlap(Entity* other) override;
	virtual void onEndOverlap(Entity* other) override;

private:
	GoldTile* m_CarriedGold = nullptr;
};
