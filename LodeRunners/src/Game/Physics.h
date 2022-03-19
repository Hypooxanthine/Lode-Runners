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

private:
	static Physics* m_Instance;

	std::vector<ColliderComponent*> m_Colliders;
};

