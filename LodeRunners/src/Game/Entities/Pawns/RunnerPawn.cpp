#include "RunnerPawn.h"

#include "../../../Assets/Assets.h"

#include "../../Components/FlipbookComponent.h"
#include "../../Components/ColliderComponent.h"
#include "../../Components/TextComponent.h"

#include "../Tiles/Gold.h"
#include "../Tiles/LadderTile.h"

#include "../TileMap.h"

#include "../../../States/GameState.h"

RunnerPawn::RunnerPawn(const size_t& ID, const std::string& name, TileMap* tileMap)
	: Pawn(ID, name, tileMap)
{
	m_Flipbook->setType(FlipbookType::PlayerLeft);
	m_Flipbook->setTotalDuration(.8f);

	m_Collider->setCollisionProfile(CollisionProfile::Runner);
	m_Collider->setRelativePosition({ .25f, 0.f });
	m_Collider->setHitbox({ .5f, 1.f });

	m_Speed = 3.f;
}

void RunnerPawn::update(const float& dt)
{
	if (m_IsKilled) return;

	Pawn::update(dt);

	if (m_IsMovingRight && !m_IsMovingLeft)
	{
		if (m_Flipbook->getType() != FlipbookType::PlayerRight || m_Flipbook->isFrozen())
		{
			m_Flipbook->setType(FlipbookType::PlayerRight);
			m_Flipbook->setCurrentFrame(0);
			m_Flipbook->unFreeze();
		}
	}
	else if (m_IsMovingLeft && !m_IsMovingRight)
	{
		if (m_Flipbook->getType() != FlipbookType::PlayerLeft || m_Flipbook->isFrozen())
		{
			m_Flipbook->setType(FlipbookType::PlayerLeft);
			m_Flipbook->setCurrentFrame(0);
			m_Flipbook->unFreeze();
		}
	}
	else
	{
		if (!m_Flipbook->isFrozen())
		{
			m_Flipbook->freeze();
		}
	}
}

// Only called on server
void RunnerPawn::onBeginOverlap(Entity* other)
{
	Pawn::onBeginOverlap(other);

	GoldTile* asGold = dynamic_cast<GoldTile*>(other);

	if (asGold)
	{
		asGold->pickup_OnServer();
		setScore_Multicast(m_Score + GOLD_POINTS);

		GameState* gs = dynamic_cast<GameState*>(Application::get()->getCurrentState());

		if (gs) gs->notifyGoldPicked();

		return;
	}

	LadderTile* asLadder = dynamic_cast<LadderTile*>(other);

	if (asLadder && asLadder->isExit())
	{
		GameState* gs = dynamic_cast<GameState*>(Application::get()->getCurrentState());

		if (gs) gs->endGame_Multicast(PawnType::Runner);

		return;
	}
}

// Only called on server
void RunnerPawn::onEndOverlap(Entity* other)
{
	Pawn::onEndOverlap(other);
}

void RunnerPawn::kill()
{
	m_IsKilled = true;
	m_Flipbook->setRender(false);
	m_NameText->setRender(false);
	m_Collider->disableCollisions();

	if (IS_SERVER)
	{
		GameState* gs = dynamic_cast<GameState*>(Application::get()->getCurrentState());

		if (gs)
			gs->notifyRunnerDeath();
	}
}

sf::Vector2u RunnerPawn::getDigTargetPos(const DigTarget& target) const
{
	auto out = (sf::Vector2u)m_Collider->getWorldPosition();

	out.y++;

	if (target == DigTarget::Left)
		out.x--;
	else
		out.x++;

	return out;
}

BrickTile* RunnerPawn::getBrick(const sf::Vector2u& pos) const
{
	return dynamic_cast<BrickTile*>(m_TileMap->getTile(size_t(pos.x + pos.y * TILES_WIDTH)));
}

std::optional<BrickTile*> RunnerPawn::getBrickDigTarget(const DigTarget& target) const
{
	std::optional<BrickTile*> out;
	auto tileTarget = getDigTargetPos(target);

	if ( tileTarget.x >= TILES_WIDTH || tileTarget.y >= TILES_HEIGHT) return out;

	out = getBrick(tileTarget);

	if (out.value() != nullptr) return out;
	else
	{
		out.reset();
		return out;
	}
}

void RunnerPawn::setScore(const size_t& score)
{
	m_Score = score;
	m_GoldsTaken++;

	LOG_INFO("Player {} picked up a gold. New score : {}.", getName(), m_Score);

	if(getID() == PLAYER_ID)
	{
		auto state = dynamic_cast<GameState*>(Application::get()->getCurrentState());
		if (state) state->setScore(m_Score);
	}

}
