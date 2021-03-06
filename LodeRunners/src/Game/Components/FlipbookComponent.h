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

	void setRender(const bool& val) { m_Render = val; }
	const bool& getRender() const { return m_Render; }

	const FlipbookType& getType() const;
	void setType(const FlipbookType& type);

	size_t getFramesNumber() const;
	const size_t& getCurrentFrame() const { return m_CurrentFrame; }
	void setCurrentFrame(const size_t& f) { m_CurrentFrame = f; }
	void nextFrame();
	void previousFrame();

	void freeze() { m_Frozen = true; }
	void unFreeze() { m_Frozen = false; }
	const bool& isFrozen() const { return m_Frozen; }

	void setFrameDuration(const float& duration) { m_FrameDuration = duration; }
	void setTotalDuration(const float& duration);
	const float& getFrameDuration() const { return m_FrameDuration; }
	float getTotalDuration() const;

private:
	Ref<FlipbookAsset> m_FbAsset = nullptr;

	bool m_Render = true;

	size_t m_CurrentFrame = 0;
	float m_FrameDuration = 1.f;
	float m_TimeAccumulator = 0.f;

	bool m_Frozen = false;
};

