#include "Pawn.h"

#include "../../Controllers/PlayerController.h"
#include "../../Controllers/AIController.h"

#include "../../Components/FlipbookComponent.h"
#include "../../Components/ColliderComponent.h"

#include "../../../Assets/Assets.h"

Pawn::Pawn(Controller* controler, const size_t& ID)
	: m_Controler(controler), m_ID(ID)
{
	m_Collider = makeComponent<ColliderComponent>("Collider");
	m_Flipbook = makeComponent<FlipbookComponent>("Flipbook");
}

std::optional<PlayerController*> Pawn::getPlayerController()
{
	auto out = dynamic_cast<PlayerController*>(m_Controler);

	return out != nullptr ? out : std::optional<PlayerController*>();
}

std::optional<AIController*> Pawn::getAIController()
{
	auto out = dynamic_cast<AIController*>(m_Controler);

	return out != nullptr ? out : std::optional<AIController*>();
}
