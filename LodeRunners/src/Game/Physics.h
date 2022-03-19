#pragma once

#include "../Core/Base.h"

class ColliderComponent;
class Entity;

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
	void unregisterCollider(ColliderComponent* collider);
	void updateColliderType(ColliderComponent* collider);

	void fixEntityPosition(Entity* entity) const;

private:

	bool pointInCollider(const sf::Vector2f& point, const ColliderComponent* collider) const;
	bool colliderWithCollider(const ColliderComponent* a, const ColliderComponent* b) const;
	bool rayWithHitbox(const Ray& ray, const sf::FloatRect& hitbox, sf::Vector2f& contactPoint, sf::Vector2f& contactNormal, float& tHitNear) const;
	bool dynamicWithStatic(const ColliderComponent* dynColl, const ColliderComponent* statColl, const sf::Vector2f& previousPos, sf::Vector2f& contactPoint, sf::Vector2f& contactNormal, float& contactTime) const;
	void resolveDynamicWithStatic(ColliderComponent* dynColl, ColliderComponent* statColl) const;

private:
	static Physics* m_Instance;

	std::vector<ColliderComponent*> m_Colliders;
	std::vector<ColliderComponent*> m_StaticColliders;
	std::vector<ColliderComponent*> m_DynamicColliders;
};

