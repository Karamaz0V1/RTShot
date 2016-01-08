#include <stdafx.h>
#include <GameElements/AgentAI.h>
namespace GameElements {
	AgentAI::AgentAI( const UnitsArchetypes::Archetype * archetype, const WeaponsArchetypes::Archetype * weaponArchetype, bool computeCollisionMesh/*=true*/ ) : Agent(archetype, weaponArchetype, computeCollisionMesh)
	{
	}

	Agent::Pointer AgentAI::selectWeakestAgent(const std::vector<Triggers::CollisionObject::Pointer> & objects) const {
		int minTargetLifeInRange = boost::dynamic_pointer_cast<Agent>(objects[0])->getArchetype()->m_life;
		int target = 0;
		for(int cpt=1 ; cpt<objects.size() ; ++cpt) {
			int targetLife = boost::dynamic_pointer_cast<Agent>(objects[0])->getArchetype()->m_life;
			if (targetLife < minTargetLifeInRange) {
				minTargetLifeInRange = targetLife;
				int target = cpt;
			}
		}
		return boost::dynamic_pointer_cast<Agent>(objects[target]);
	}
}