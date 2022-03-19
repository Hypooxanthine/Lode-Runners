#pragma once

#include "../Entity.h"

#include "../../../Assets/Assets.h"
#include "../../../Network/Network.h"

class Controller;
class PlayerController;
class AIController;

class FlipbookComponent;
class ColliderComponent;

class Pawn : public Entity
{
public:
	enum class MoveDir { Left, Right, Up, Down };
public:
	Pawn() = delete;
	Pawn(const size_t& ID, const std::string& name);

	virtual void update(const float& dt) override;

	void setController(Controller* controller) { m_Controller = controller; }

	virtual void setPosition(const sf::Vector2f& position) override;
	virtual void move(const sf::Vector2f& delta) override;

	void setPositionLocal(const sf::Vector2f& position);

	const size_t& getID() const { return m_ID; }
	const std::string& getName() const { return m_Name; }

	std::optional<PlayerController*> getPlayerController();
	std::optional<AIController*> getAIController();

protected:
	ColliderComponent* m_Collider;
	FlipbookComponent* m_Flipbook;

	bool m_IsMovingLeft = false, m_IsMovingRight = false, m_IsMovingUp = false, m_IsMovingDown = false;

private:
	Controller* m_Controller;

	std::string m_Name;

	// For networking
	size_t m_ID = 0;

public: // Replicated functions

	CREATE_REPLICATED_FUNCTION
	(
		setPosition_OnClients,
		[this](const float& x, const float& y)
		{
			setPositionLocal({ x, y });
		},
		"Pawn" + std::to_string(m_ID), Network::ReplicationMode::OnClients,
		const float&, const float&
	);

	CREATE_REPLICATED_FUNCTION
	(
		setPosition_OnServer,
		[this](const float& x, const float& y)
		{
			m_WorldPosition.x = x * SPACE_UNIT;
			m_WorldPosition.y = y * SPACE_UNIT;

			Physics::get()->fixEntityPosition(this);

			setPosition_OnClients(getPosition().x, getPosition().y);
		},
		"Pawn" + std::to_string(m_ID), Network::ReplicationMode::OnServer,
		const float&, const float&
	);

	CREATE_REPLICATED_FUNCTION
	(
		move_OnClients,
		[this](const float& x, const float& y)
		{
			m_WorldPosition.x += x * SPACE_UNIT;
			m_WorldPosition.y += y * SPACE_UNIT;
		},
		"Pawn" + std::to_string(m_ID), Network::ReplicationMode::OnClients,
		const float&, const float&
	);

	CREATE_REPLICATED_FUNCTION
	(
		move_OnServer,
		[this](const float& x, const float& y)
		{
			m_WorldPosition.x += x * SPACE_UNIT;
			m_WorldPosition.y += y * SPACE_UNIT;

			Physics::get()->fixEntityPosition(this);

			setPosition_OnClients(getPosition().x, getPosition().y);
		},
		"Pawn" + std::to_string(m_ID), Network::ReplicationMode::OnServer,
		const float&, const float&
	);

	CREATE_REPLICATED_FUNCTION
	(
		setMoving_Multicast,
		[this](const MoveDir& dir, const bool& val)
		{
			switch (dir)
			{
			case MoveDir::Left:
				this->m_IsMovingLeft = val;
				break;
			case MoveDir::Right:
				this->m_IsMovingRight = val;
				break;
			case MoveDir::Up:
				this->m_IsMovingUp = val;
				break;
			case MoveDir::Down:
				this->m_IsMovingDown = val;
				break;
			default:
				break;
			}
		},
		"Pawn" + std::to_string(m_ID), Network::ReplicationMode::Multicast,
		const MoveDir&, const bool&
	);

	CREATE_REPLICATED_FUNCTION
	(
		setMoving_OnServer,
		[this](const MoveDir& dir, const bool& val)
		{
			setMoving_Multicast(dir, val);
		},
		"Pawn" + std::to_string(m_ID), Network::ReplicationMode::OnServer,
		const MoveDir&, const bool&
	);
};

