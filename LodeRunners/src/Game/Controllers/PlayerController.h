#pragma once

#include "Controller.h"

class PlayerController : public Controller
{
public:
	PlayerController() = default;

	virtual void update(const float& dt) override;

private:
	float m_Speed = 3.f;

	bool m_MoveLeftOld = false, m_MoveRightOld = false, m_MoveUpOld = false, m_MoveDownOld = false;
};

