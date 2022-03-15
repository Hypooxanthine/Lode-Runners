#pragma once
#include "Component.h"

class FlipbookAsset;
enum class FlipbookType;

class FlipbookComponent : public Component
{
public:
	FlipbookComponent();
	FlipbookComponent(const FlipbookType& type);

	virtual void render(Ref<sf::RenderWindow> window) override;
	virtual void update(const float& dt) override;

	void setType(const FlipbookType& type);

private:
	Ref<FlipbookAsset> m_FbAsset = nullptr;
};

