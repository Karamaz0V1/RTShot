#ifndef _GameElements_AgentAI_H
#define _GameElements_AgentAI_H

#include <GameElements/Agent.h>
namespace GameElements
{
	class AgentAI : public Agent {
		public:
			AgentAI(const UnitsArchetypes::Archetype * archetype, const WeaponsArchetypes::Archetype * weaponArchetype, bool computeCollisionMesh = true);
		protected:
			virtual Agent::Pointer selectWeakestAgent(const std::vector<Triggers::CollisionObject::Pointer> & objects) const;
	};
}
#endif