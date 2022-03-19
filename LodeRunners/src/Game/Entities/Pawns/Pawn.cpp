#include "Pawn.h"

#include "../../Controllers/PlayerController.h"
#include "../../Controllers/AIController.h"

#include "../../Components/FlipbookComponent.h"
#include "../../Components/ColliderComponent.h"

#include "../../../Assets/Assets.h"

Pawn::Pawn(const size_t& ID, const std::string& name)
	: m_ID(ID), m_Name(name), m_Controller(nullptr)
{
	m_Collider = makeComponent<ColliderComponent>("Collider");
	m_Collider->setCollisionType(CollisionType::Dynamic);
	m_Collider->setBehavioursWith(CollisionProfile::Man, CollisionResponse::Overlaps);
	m_Collider->setBehavioursWith(CollisionProfile::Runner, CollisionResponse::Overlaps);
	m_Collider->setBehavioursWith(CollisionProfile::TileSolid, CollisionResponse::Blocks);
	m_Collider->setBehavioursWith(CollisionProfile::TileTransparent, CollisionResponse::Overlaps);

	m_Flipbook = makeComponent<FlipbookComponent>("Flipbook");
}

void Pawn::update(const float& dt)
{
}

void Pawn::setPosition(const sf::Vector2f& position)
{
	setPosition_OnServer(position.x, position.y);
}

void Pawn::move(const sf::Vector2f& delta)
{
	move_OnServer(delta.x, delta.y);
}

void Pawn::setPositionLocal(const sf::Vector2f& position)
{
	m_WorldPosition = position * SPACE_UNIT;
	m_Collider->setLastPosition(m_Collider->getWorldPosition());
}

std::optional<PlayerController*> Pawn::getPlayerController()
{
	auto out = dynamic_cast<PlayerController*>(m_Controller);

	return out != nullptr ? out : std::optional<PlayerController*>();
}

std::optional<AIController*> Pawn::getAIController()
{
	auto out = dynamic_cast<AIController*>(m_Controller);

	return out != nullptr ? out : std::optional<AIController*>();
}
