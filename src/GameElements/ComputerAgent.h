#ifndef _GameElements_ComputerAgent_H
#define _GameElements_ComputerAgent_H

#include <GameElements/SmithAgent.h>
#include <OgreFramework/GlobalConfiguration.h>

namespace GameElements
{
	class ComputerAgent : public SmithAgent {		
	public:
		ComputerAgent(const UnitsArchetypes::Archetype * archetype, const WeaponsArchetypes::Archetype * weaponArchetype, bool computeCollisionMesh=true);
		virtual void update(const Config::Real & dt);
		virtual void onCollision (const CollisionMessage & message);
	};
}

#endif