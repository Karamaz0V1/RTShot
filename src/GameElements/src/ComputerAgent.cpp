#include <stdafx.h>
#include <GameElements/ComputerAgent.h>
#include <GameElements/BulletBase.h>

namespace GameElements
{
	ComputerAgent::ComputerAgent(const UnitsArchetypes::Archetype * archetype, const WeaponsArchetypes::Archetype * weaponArchetype, bool computeCollisionMesh) : SmithAgent(archetype, weaponArchetype, computeCollisionMesh)
	{
		m_computer = true;
	}

	void ComputerAgent::update(const Config::Real & dt) {
		SmithAgent::update(dt);
	}
	void ComputerAgent::onCollision (const CollisionMessage & message) {
		// Derp
	}
}