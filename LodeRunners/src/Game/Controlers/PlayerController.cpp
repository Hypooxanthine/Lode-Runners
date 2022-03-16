#include "PlayerController.h"

#include "../Entities/Pawns/RunnerPawn.h"
#include "../../Assets/Assets.h"

#define EVENT(eventType) Application::get()->getEvent(eventType)

void PlayerController::update(const float& dt)
{
	if (EVENT(EventType::MoveLeft))
	{
		getPawn()->move({ -dt * 2 * Assets::getElementSize(), 0.f});
		LOG_TRACE("Moving left.");
	}
	if (EVENT(EventType::MoveRight))
	{
		getPawn()->move({ dt * 2 * Assets::getElementSize(), 0.f });
		LOG_TRACE("Moving right.");
	}
	if (EVENT(EventType::MoveUp))
	{
		getPawn()->move({ 0.f, -dt * 2 * Assets::getElementSize() });
		LOG_TRACE("Moving up.");
	}
	if (EVENT(EventType::MoveDown))
	{
		getPawn()->move({ 0.f, dt * 2 * Assets::getElementSize() });
		LOG_TRACE("Moving down.");
	}
}
