#ifndef _GameElements_HumanAgent_H
#define _GameElements_HumanAgent_H

#include <GameElements/SmithAgent.h>
#include <OgreFramework/GlobalConfiguration.h>

namespace GameElements
{
	class HumanAgent : public SmithAgent {		
	public:
		HumanAgent(const UnitsArchetypes::Archetype * archetype, const WeaponsArchetypes::Archetype * weaponArchetype, bool computeCollisionMesh=true);
		virtual void update(const Config::Real & dt);
		virtual void onCollision (const CollisionMessage & message);
	};
}

#endif