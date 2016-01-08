#include <stdafx.h>
#include <GameElements/AgentAI.h>
namespace GameElements {
	AgentAI::AgentAI( const UnitsArchetypes::Archetype * archetype, const WeaponsArchetypes::Archetype * weaponArchetype, bool computeCollisionMesh/*=true*/ ) : Agent(archetype, weaponArchetype, computeCollisionMesh)
	{
	}

	Agent::Pointer AgentAI::selectWeakestAgent(const std::vector<Triggers::CollisionObject::Pointer> & agents) const {
		int minTargetLifeInRange = boost::dynamic_pointer_cast<Agent>(agents[0])->getArchetype()->m_life;
		int target = 0;
		for(int cpt=1 ; cpt<agents.size() ; ++cpt) {
			int targetLife = boost::dynamic_pointer_cast<Agent>(agents[0])->getArchetype()->m_life;
			if (targetLife < minTargetLifeInRange) {
				minTargetLifeInRange = targetLife;
				int target = cpt;
			}
		}
		return boost::dynamic_pointer_cast<Agent>(agents[target]);
	}

	void AgentAI::removeFriendFromAgentsList(std::vector<Triggers::CollisionObject::Pointer> & agents) const {
		for(int cpt=0 ; cpt<agents.size() ; ++cpt) {
			Agent::Pointer ptr = boost::dynamic_pointer_cast<Agent>(agents[cpt]) ;
				
			std::string sLui = ptr->getArchetype()->m_name;
			std::string sMoi = this->getArchetype()->m_name;
			char sonType = sLui.back();
			char monType = sMoi.back();

			if (sonType == monType) {
				::std::swap(agents[cpt], agents.back()) ;
				agents.pop_back() ;
				cpt--;
			}
		}
	}
}