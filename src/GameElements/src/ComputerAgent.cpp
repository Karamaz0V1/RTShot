#include <stdafx.h>
#include <GameElements/ComputerAgent.h>
#include <GameElements/BulletBase.h>
#include <OgreFramework/GlobalConfiguration.h>

using namespace OgreFramework;

namespace GameElements
{
	ComputerAgent::ComputerAgent(const UnitsArchetypes::Archetype * archetype, const WeaponsArchetypes::Archetype * weaponArchetype, bool computeCollisionMesh) : SmithAgent(archetype, weaponArchetype, computeCollisionMesh)
	{
		m_computer = true;
		go2(GlobalConfiguration::getCurrentMap()->toWorldCoordinates(GlobalConfiguration::getCurrentMap()->findFreeLocation()));
	}

	void ComputerAgent::update(const Config::Real & dt) {
		SmithAgent::update(dt);
		if (rand() % 1000 == 0) go2(GlobalConfiguration::getCurrentMap()->toWorldCoordinates(GlobalConfiguration::getCurrentMap()->findFreeLocation()));
	}
	void ComputerAgent::onCollision (const CollisionMessage & message) {
		// Derp
	}
}