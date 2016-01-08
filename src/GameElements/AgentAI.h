#ifndef _GameElements_AgentAI_H
#define _GameElements_AgentAI_H

#include <GameElements/Agent.h>
namespace GameElements
{
	class AgentAI : public Agent {
		public:
			AgentAI(const UnitsArchetypes::Archetype * archetype, const WeaponsArchetypes::Archetype * weaponArchetype, bool computeCollisionMesh = true);
		protected:
			virtual Agent::Pointer selectWeakestAgent(const std::vector<Agent::Pointer> & agents) const;
			virtual void removeFriendFromAgentsList(std::vector<Triggers::CollisionObject::Pointer> & agents) const;
			virtual std::vector<Agent::Pointer> getAgentsListFromObjectsList(const std::vector<Triggers::CollisionObject::Pointer> & objects) const;
	};
}
#endif