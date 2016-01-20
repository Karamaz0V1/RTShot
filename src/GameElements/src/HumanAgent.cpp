#include <stdafx.h>
#include <GameElements/HumanAgent.h>
#include <GameElements/BulletBase.h>

namespace GameElements
{
	HumanAgent::HumanAgent(const UnitsArchetypes::Archetype * archetype, const WeaponsArchetypes::Archetype * weaponArchetype, bool computeCollisionMesh) : SmithAgent(archetype, weaponArchetype, computeCollisionMesh)
	{
		m_velocity = randomVelocity();
	}

	void HumanAgent::update(const Config::Real & dt) {
		// Derp
	}
	void HumanAgent::onCollision (const CollisionMessage & message) {
		// Derp
	}
}