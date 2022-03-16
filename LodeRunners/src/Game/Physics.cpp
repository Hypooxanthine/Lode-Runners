#include "Physics.h"

#include "Components/ColliderComponent.h"

Physics* Physics::m_Instance = nullptr;

Physics::Physics()
{
	ASSERT(m_Instance == nullptr, "Physics singleton already exists.");

	m_Instance = this;
}

void Physics::resolve()
{
	
}

void Physics::registerCollider(ColliderComponent* collider)
{
	m_Colliders.push_back(collider);
}

bool Physics::unregisterCollider(ColliderComponent* collider)
{
	auto it = std::find(m_Colliders.begin(), m_Colliders.end(), collider);

	if (it != m_Colliders.end())
	{
		m_Colliders.erase(it);
		return true;
	}

	return false;
}
