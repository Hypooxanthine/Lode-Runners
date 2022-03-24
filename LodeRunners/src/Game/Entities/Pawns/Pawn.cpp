#include "Pawn.h"

#include "../../../Assets/Assets.h"

#include "../../Controllers/PlayerController.h"
#include "../../Controllers/AIController.h"

#include "../../Components/FlipbookComponent.h"
#include "../../Components/ColliderComponent.h"
#include "../../Components/TextComponent.h"

#include "../Tiles/LadderTile.h"
#include "../Tiles/BridgeTile.h"

Pawn::Pawn(const size_t& ID, const std::string& name, TileMap* tileMap)
	: m_ID(ID), m_Name(name), m_Controller(nullptr), m_TileMap(tileMap)
{
	m_Collider = makeComponent<ColliderComponent>("Collider");
	m_Collider->setCollisionType(CollisionType::Dynamic);
	m_Collider->setBehavioursWith(CollisionProfile::Man, CollisionResponse::Overlaps);
	m_Collider->setBehavioursWith(CollisionProfile::Runner, CollisionResponse::Ignore);
	m_Collider->setBehavioursWith(CollisionProfile::TileSolid, CollisionResponse::Blocks);
	m_Collider->setBehavioursWith(CollisionProfile::TileTransparent, CollisionResponse::Overlaps);

	m_Flipbook = makeComponent<FlipbookComponent>("Flipbook");

	m_NameText = makeComponent<TextComponent>("Name");

	if (name != "")
		m_NameText->setColor(ID == PLAYER_ID ? sf::Color::Red : sf::Color(230, 135, 11));

	m_NameText->setRelativePosition({ .5f, -.15f });
	m_NameText->setText(name);
	m_NameText->setCharacterSize(20);
	m_NameText->setBold();
	m_NameText->setOutlineThickness(.025f);
	m_NameText->setOutlineColor(sf::Color::Black);
	m_NameText->setHorizontalAlign(TextComponent::HorizontalAlign::Center);
	m_NameText->setVerticalAlign(TextComponent::VerticalAlign::Bottom);
}

void Pawn::onBeginOverlap(Entity* other)
{
	if (dynamic_cast<LadderTile*>(other) != nullptr)
		m_OverlappingLadders++;
	else if (dynamic_cast<BridgeTile*>(other) != nullptr)
		m_OverlappingBridges++;
}

void Pawn::onEndOverlap(Entity* other)
{
	if (dynamic_cast<LadderTile*>(other) != nullptr)
		m_OverlappingLadders--;
	else if (dynamic_cast<BridgeTile*>(other) != nullptr)
		m_OverlappingBridges--;
}

void Pawn::update(const float& dt)
{
	if (IS_SERVER)
	{
		if (m_IsMovingLeft)
			move({ -dt * m_Speed, 0.f });
		if (m_IsMovingRight)
			move({ dt * m_Speed, 0.f });

		if (m_OverlappingLadders > 0 || m_OverlappingBridges > 0)
		{
			if (m_IsMovingUp)
				move({ 0.f, -dt * m_Speed });
			if (m_IsMovingDown)
				move({ 0.f, dt * m_Speed });
		}
		else
		{
			move({ 0.f, dt * m_GravityForce });
		}
	}
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
