#pragma once

#include "Component.h"

enum class CollisionType { Static, Dynamic };

enum class CollisionProfile { Tile, Runner, Man };
enum class CollisionResponse { Ignore, Overlaps, Blocks };

class ColliderComponent : public Component
{
	friend class Physics;
public:
	ColliderComponent();
	virtual ~ColliderComponent();
	virtual void update(const float& dt) override;
	virtual void render(Ref<sf::RenderWindow> window) override;

public:
	sf::Vector2f getHitbox() const;
	void setHitbox(const sf::Vector2f& size);

	sf::FloatRect getHitRect() const;

	const CollisionType& getCollisionType() const;
	void setCollisionType(const CollisionType& type);

	const CollisionProfile& getCollisionProfile() const;
	void setCollisionProfile(const CollisionProfile& profile);

	const CollisionResponse& getBehaviourWith(const CollisionProfile& profile) const;
	void setBehavioursWith(const CollisionProfile& profile, const CollisionResponse& response);

	sf::Vector2f getLastPos() const;

private: // Private members
	sf::Vector2f m_Hitbox;

	CollisionType m_ColType = CollisionType::Static;
	CollisionProfile m_ColProfile = CollisionProfile::Tile;
	
	std::map<CollisionProfile, CollisionResponse> m_BehaviourWithProfile;

	sf::Vector2f m_LastPosition;

	std::vector<ColliderComponent*> m_OverlappingColliders;
};

