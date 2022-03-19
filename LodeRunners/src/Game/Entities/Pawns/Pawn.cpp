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
	m_Flipbook = makeComponent<FlipbookComponent>("Flipbook");
}

void Pawn::update(const float& dt)
{
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
