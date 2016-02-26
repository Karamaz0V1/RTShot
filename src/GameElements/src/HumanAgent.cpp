#include <stdafx.h>
#include <GameElements/HumanAgent.h>
#include <GameElements/BulletBase.h>

namespace GameElements
{
	HumanAgent::HumanAgent(const UnitsArchetypes::Archetype * archetype, const WeaponsArchetypes::Archetype * weaponArchetype, bool computeCollisionMesh) : SmithAgent(archetype, weaponArchetype, computeCollisionMesh)
	{
		m_computer = false;
	}

	void HumanAgent::update(const Config::Real & dt) {
		SmithAgent::update(dt);
	}
	void HumanAgent::onCollision (const CollisionMessage & message) {
		// Derp
	}
}