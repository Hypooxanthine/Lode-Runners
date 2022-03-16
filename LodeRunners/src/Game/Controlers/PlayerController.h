#pragma once

#include "Controller.h"

class PlayerController : public Controller
{
public:
	PlayerController() = default;

	virtual void update(const float& dt) override;
};

