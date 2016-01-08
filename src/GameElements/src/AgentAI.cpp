#include <stdafx.h>
#include <GameElements/AgentAI.h>

using std::vector;

namespace GameElements {
	AgentAI::AgentAI( const UnitsArchetypes::Archetype * archetype, const WeaponsArchetypes::Archetype * weaponArchetype, bool computeCollisionMesh/*=true*/ ) : Agent(archetype, weaponArchetype, computeCollisionMesh)
	{
	}

	Agent::Pointer AgentAI::selectWeakestAgent(const vector<Agent::Pointer> & agents) const {
		Agent::Pointer weakest = agents.front();

		for(vector<Agent::Pointer>::const_iterator it = agents.begin() + 1; it != agents.end(); it++)
			if ((*it)->getLifePoints() < weakest->getLifePoints())
				weakest = *it;

		return weakest;
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

	vector<Agent::Pointer> AgentAI::getAgentsListFromObjectsList(const vector<Triggers::CollisionObject::Pointer> & objects) const {
		vector<Agent::Pointer> agentsList;
		for (vector<Triggers::CollisionObject::Pointer>::const_iterator it = objects.begin(); it != objects.end(); it++)
			if(boost::dynamic_pointer_cast<Agent>(*it)==NULL)
				agentsList.push_back(boost::dynamic_pointer_cast<Agent>(*it));

		return agentsList;
	}
}