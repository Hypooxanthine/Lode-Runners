#pragma once

#include "../Entity.h"

class Controller;
class PlayerController;
class AIController;

class FlipbookComponent;

class Pawn : public Entity
{
public:
	Pawn() = delete;
	Pawn(Controller* controler);

	std::optional<PlayerController*> getPlayerController();
	std::optional<AIController*> getAIController();

private:
	Controller* m_Controler;

	FlipbookComponent* m_Flipbook;
};

