#pragma once

#include "../Core/Base.h"

class ColliderComponent;

class Physics
{
public:
	struct Ray
	{
		sf::Vector2f origin;
		sf::Vector2f direction;
	};
public:
	Physics();

	static Physics* get() { return m_Instance; }

	void update();

	void registerCollider(ColliderComponent* collider);
	bool unregisterCollider(ColliderComponent* collider);

	// Tests
	bool pointInCollider(const sf::Vector2f& point, const ColliderComponent* collider) const;
	bool colliderWithCollider(const ColliderComponent* a, const ColliderComponent* b) const;
	bool rayWithHitbox(const Ray& ray, const sf::FloatRect& hitbox, sf::Vector2f& contactPoint, sf::Vector2f& contactNormal, float& tHitNear) const;
	bool dynamicWithStatic(const ColliderComponent* dynColl, const ColliderComponent* statColl, const sf::Vector2f& previousPos, sf::Vector2f& contactPoint, sf::Vector2f& contactNormal, float& contactTime) const;
	bool resolveDynamicWithStatic(ColliderComponent* dynColl, ColliderComponent* statColl, const sf::Vector2f& previousPos) const;

	// Helper functions
	bool pointInRect(const sf::Vector2f& point, const sf::FloatRect& rect);
	bool rectCrossesRect(const sf::FloatRect& a, const sf::FloatRect& b);
	bool rayThroughRect(const Ray& ray, const sf::FloatRect& rect, sf::Vector2f& contactPoint, sf::Vector2f& contactNormal, float& tHitNear);
	bool dynamicRectThroughRect(const sf::FloatRect& dynRect, const sf::Vector2f& velocity, const sf::FloatRect& statRect, const float& dt, sf::Vector2f& contactPoint, sf::Vector2f& contactNormal, float& contactTime);
	bool resolveDynamicRectWithRect(ColliderComponent* dynRect, sf::Vector2f& velocity, std::array<ColliderComponent*, 4>& contacts, const float& dt, ColliderComponent* statRect);

private:
	static Physics* m_Instance;

	std::vector<ColliderComponent*> m_Colliders;
};

