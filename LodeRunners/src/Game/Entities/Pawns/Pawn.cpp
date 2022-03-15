#include "Pawn.h"

#include "../../Controlers/PlayerController.h"
#include "../../Controlers/AIController.h"

#include "../../Components/FlipbookComponent.h"

Pawn::Pawn(Controller* controler)
	: m_Controler(controler)
{
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
