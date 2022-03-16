#pragma once

#include "../Entity.h"

class Controller;
class PlayerController;
class AIController;

class FlipbookComponent;
class ColliderComponent;

class Pawn : public Entity
{
public:
	Pawn() = delete;
	Pawn(Controller* controler);

	std::optional<PlayerController*> getPlayerController();
	std::optional<AIController*> getAIController();

protected:
	ColliderComponent* m_Collider;
	FlipbookComponent* m_Flipbook;

private:
	Controller* m_Controler;
};

