#pragma once

#include "../Entity.h"

#include "../../../Network/Network.h"

class Controller;
class PlayerController;
class AIController;

class FlipbookComponent;
class ColliderComponent;

class Pawn : public Entity
{
public:
	Pawn() = delete;
	Pawn(const size_t& ID, const std::string& name);

	void setController(Controller* controller) { m_Controller = controller; }

	const size_t& getID() const { return m_ID; }
	const std::string& getName() const { return m_Name; }

	std::optional<PlayerController*> getPlayerController();
	std::optional<AIController*> getAIController();

protected:
	ColliderComponent* m_Collider;
	FlipbookComponent* m_Flipbook;

private:
	Controller* m_Controller;

	size_t m_ID;
	std::string m_Name;

public: // Replicated functions

	CREATE_REPLICATED_FUNCTION
	(
		setPosition_Multicast,
		[this](const float& x, const float& y)
		{
			this->setPosition({ x, y });
		},
		"Pawn" + std::to_string(m_ID), Network::ReplicationMode::Multicast,
		const float&, const float&
	);

	CREATE_REPLICATED_FUNCTION
	(
		setPosition_OnServer,
		[this](const float& x, const float& y)
		{
			setPosition_Multicast(x, y);
		},
		"Pawn" + std::to_string(m_ID), Network::ReplicationMode::OnServer,
		const float&, const float&
	);

	CREATE_REPLICATED_FUNCTION
	(
		move_Multicast,
		[this](const float& x, const float& y)
		{
			this->move({ x, y });
		},
		"Pawn" + std::to_string(m_ID), Network::ReplicationMode::Multicast,
		const float&, const float&
	);

	CREATE_REPLICATED_FUNCTION
	(
		move_OnServer,
		[this](const float& x, const float& y)
		{
			move_Multicast(x, y);
		},
		"Pawn" + std::to_string(m_ID), Network::ReplicationMode::OnServer,
		const float&, const float&
	);
};

