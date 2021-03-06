#pragma once

#include "Component.h"

enum class CollisionType { Static, Dynamic };

enum class CollisionProfile { TileSolid, TileTransparent, NoCollision, Runner, Ennemy, LadderTop };
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

	CollisionProfile getCollisionProfile() const;
	void setCollisionProfile(const CollisionProfile& profile);

	CollisionResponse getBehaviourWith(const CollisionProfile& profile) const;
	void setBehavioursWith(const CollisionProfile& profile, const CollisionResponse& response);

	void enableCollisions();
	void disableCollisions();

	bool ignores(const ColliderComponent* other) const;
	bool overlaps(const ColliderComponent* other) const;
	bool blocks(const ColliderComponent* other) const;

	void notifyOverlaps(ColliderComponent* other);

	sf::Vector2f getLastPosition() const;
	void setLastPosition(const sf::Vector2f& position);

private: // Private members
	sf::Vector2f m_Hitbox;

	CollisionType m_ColType = CollisionType::Static;
	CollisionProfile m_ColProfile = CollisionProfile::TileSolid;
	
	std::map<CollisionProfile, CollisionResponse> m_BehaviourWithProfile;
	bool m_Disabled = false;

	sf::Vector2f m_LastPosition;

	std::vector<ColliderComponent*> m_OverlappingColliders;
};

