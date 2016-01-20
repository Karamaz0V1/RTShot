#include <stdafx.h>
#include <GameElements/ComputerAgent.h>
#include <GameElements/BulletBase.h>

namespace GameElements
{
	ComputerAgent::ComputerAgent(const UnitsArchetypes::Archetype * archetype, const WeaponsArchetypes::Archetype * weaponArchetype, bool computeCollisionMesh) : SmithAgent(archetype, weaponArchetype, computeCollisionMesh)
	{
		m_velocity = randomVelocity();
	}

	void ComputerAgent::update(const Config::Real & dt) {
		// Derp
	}
	void ComputerAgent::onCollision (const CollisionMessage & message) {
		// Derp
	}
}