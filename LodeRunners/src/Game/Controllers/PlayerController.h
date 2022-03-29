#pragma once

#include "Controller.h"

class PlayerController : public Controller
{
public:
	PlayerController();

	virtual void update(const float& dt) override;
};

