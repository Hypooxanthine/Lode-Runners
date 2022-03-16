#pragma once

#include "../Core/Base.h"

class ColliderComponent;

class Physics
{
public:
	Physics();

	static Physics* get() { return m_Instance; }

	void resolve();

	void registerCollider(ColliderComponent* collider);
	bool unregisterCollider(ColliderComponent* collider);

private:
	static Physics* m_Instance;

	std::vector<ColliderComponent*> m_Colliders;
};

