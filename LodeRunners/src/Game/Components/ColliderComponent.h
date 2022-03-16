#pragma once

#include "Component.h"

enum class CollisionType { Static, Dynamic };

enum class CollisionProfile { Tile, Runner, Man };
enum class CollisionResponse { Ignore, Overlaps, Blocks };

class ColliderComponent : public Component
{
public:
	ColliderComponent();
	virtual ~ColliderComponent();
	virtual void update(const float& dt) override;

public:
	const sf::Vector2f& getHitbox() const                   { return m_Hitbox; }
	void                setHitbox(const sf::Vector2f& size) { m_Hitbox = size; }

	sf::FloatRect getHitRect() const { return sf::FloatRect(getWorldPosition(), m_Hitbox); }

	const CollisionType& getCollisionType() const                    { return m_ColType; }
	void                 setCollisionType(const CollisionType& type) { m_ColType = type; }

	const CollisionProfile& getCollisionProfile() const                          { return m_ColProfile; }
	void                    setCollisionProfile(const CollisionProfile& profile) { m_ColProfile = profile; }

	const CollisionResponse& getBehaviourWith(const CollisionProfile& profile) const 
	{ 
		return m_BehaviourWithProfile.at(profile);
	}
	void setBehavioursWith(const CollisionProfile& profile, const CollisionResponse& response)
	{ 
		m_BehaviourWithProfile[profile] = response; 
	}

	// A collider A can collide a collider B while B doesn't collide A because of collision profiles and responses. 
	bool collides(const ColliderComponent* other) const;
	bool resolveCollisionWith(ColliderComponent* other);

private: // Private methods
	void clip(ColliderComponent* other);

private: // Private members
	sf::Vector2f m_Hitbox;

	CollisionType m_ColType = CollisionType::Static;
	CollisionProfile m_ColProfile = CollisionProfile::Tile;
	
	std::map<CollisionProfile, CollisionResponse> m_BehaviourWithProfile;

	sf::Vector2f m_LastPosition;

	std::vector<ColliderComponent*> m_OverlappingColliders;
};

