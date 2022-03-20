#pragma once

#define GOLD_POINTS 150

#include "Tile.h"

#include "../../../Network/Network.h"

class SpriteComponent;
class ColliderComponent;

class GoldTile : public Tile
{
public:
	GoldTile(const size_t& ID);

	void pickUp();
	void hide();
	void show();

	const bool& wasTaken() const;

private:
	SpriteComponent* m_Sprite;
	ColliderComponent* m_Collider;

	// For networking
	size_t m_ID = 0;

	bool m_PickedUp = false;
	bool m_Hidden = false;

public: // Replicated functions

	CREATE_REPLICATED_FUNCTION
	(
		hide_OnClients,
		[this]() { this->hide(); },
		"GoldTile" + std::to_string(m_ID), Network::ReplicationMode::OnClients
	);

	CREATE_REPLICATED_FUNCTION
	(
		hide_OnServer,
		[this]()
		{
			if (!m_Hidden)
			{
				this->hide();
				hide_OnClients();
			}
		},
		"GoldTile" + std::to_string(m_ID), Network::ReplicationMode::OnServer
	);

	CREATE_REPLICATED_FUNCTION
	(
		show_OnClients,
		[this]() { this->show(); },
		"GoldTile" + std::to_string(m_ID), Network::ReplicationMode::OnClients
	);

	CREATE_REPLICATED_FUNCTION
	(
		show_OnServer,
		[this]()
		{
			if (m_Hidden)
			{
				this->show();
				show_OnClients();
			}
		},
		"GoldTile" + std::to_string(m_ID), Network::ReplicationMode::OnServer
	);

	CREATE_REPLICATED_FUNCTION
	(
		pickup_OnClients,
		[this]() { this->pickUp(); },
		"GoldTile" + std::to_string(m_ID), Network::ReplicationMode::OnClients
	);

	CREATE_REPLICATED_FUNCTION
	(
		pickup_OnServer,
		[this]()
		{
			if (!m_PickedUp)
			{
				this->pickUp();
				pickup_OnClients();
			}
		},
		"GoldTile" + std::to_string(m_ID), Network::ReplicationMode::OnServer
	);
};

