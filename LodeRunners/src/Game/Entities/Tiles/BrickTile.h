#pragma once

#include "Tile.h"

class ColliderComponent;

class Pawn;

class BrickTile : public Tile
{
public:
	BrickTile(const size_t& ID);

	void update(const float& dt) override;

	void dig();
	void notifyPawnOverlap(Pawn* p);
	void notifyPawnEndOverlap(Pawn* p);

private: // Private member functions
	void hide();
	void show();
	void refill();

protected:
	ColliderComponent* m_Collider;

	std::vector<Pawn*> m_OverlappingPawns;

	bool m_IsDug = false;
	float m_TimeAccumulator = 0.f;

	// For networking.
	size_t m_ID = 0;

private: // Replicated functions

	CREATE_REPLICATED_FUNCTION
	(
		refill_Multicast,
		[this]() { this->refill(); },
		"BrickTile" + std::to_string(m_ID), Network::ReplicationMode::Multicast
	);
};

