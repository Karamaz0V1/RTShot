#include <stdafx.h>
#include <GameElements/SmithAgent.h>
#include <GameElements/map.h>

using std::vector;
using std::cout;
using std::endl;

namespace GameElements {
	SmithAgent::SmithAgent( const UnitsArchetypes::Archetype * archetype, const WeaponsArchetypes::Archetype * weaponArchetype, bool computeCollisionMesh/*=true*/ ) : Agent(archetype, weaponArchetype, computeCollisionMesh) {
	}

	Agent::Pointer SmithAgent::selectWeakestAgent(const vector<Agent::Pointer> & agents) const {
		Agent::Pointer weakest = agents.front();

		for(vector<Agent::Pointer>::const_iterator it = agents.begin() + 1; it != agents.end(); it++)
			if ((*it)->getLifePoints() < weakest->getLifePoints())
				weakest = *it;

		return weakest;
	}

	void SmithAgent::removeFriendFromAgentsList(vector<Agent::Pointer> & agents) const {
		std::string sMoi = this->getArchetype()->m_name;
		char monType = sMoi.back();

		for(unsigned int cpt=0 ; cpt < agents.size() ; ++cpt) {
			Agent::Pointer ptr = agents[cpt];
				
			std::string sLui = ptr->getArchetype()->m_name;
			char sonType = sLui.back();

			if (sonType == monType) {
				::std::swap(agents[cpt], agents.back()) ;
				agents.pop_back() ;
				cpt--;
			}
		}
	}

	vector<Agent::Pointer> SmithAgent::getAgentsListFromObjectsList(const vector<Triggers::CollisionObject::Pointer> & objects) const {
		vector<Agent::Pointer> agentsList;

		for (vector<Triggers::CollisionObject::Pointer>::const_iterator it = objects.begin(); it != objects.end(); it++)
			if(boost::dynamic_pointer_cast<Agent>(*it) != NULL)
				agentsList.push_back(boost::dynamic_pointer_cast<Agent>(*it));

		return agentsList;
	}

	void SmithAgent::update( const Config::Real & dt ) {
		cout << "Mister Anderson?" << endl;
		return;

		// Computes movements
		const Map::GroundCellDescription & currentCell = OgreFramework::GlobalConfiguration::getCurrentMap()->getCell(getPosition().projectZ()) ;
		Math::Vector2<Config::Real> newPosition = getPosition().projectZ()+m_velocity*dt*(1.0-currentCell.m_speedReduction) ;

		std::vector<Triggers::CollisionObject::Pointer> objects = m_perception->perceivedAgents();
		//for (std::vector<Triggers::CollisionObject::Pointer>::const_iterator it = objects.begin(); it != objects.end(); it++) {
			//(*it)->
		//}

		// If displacement is valid, the agent moves, otherwise, a new random velocity is computed
		if(OgreFramework::GlobalConfiguration::getCurrentMap()->isValid(newPosition) && OgreFramework::GlobalConfiguration::getCurrentMap()->getCell(newPosition).m_speedReduction!=1.0) {
			setOrientation(m_velocity) ;
			setPosition(newPosition.push(0.0)) ;
		} else {
			m_velocity = randomVelocity() ;
		}

		// Handles perception and fires on agents
		if(canFire()) {
			std::vector<Agent::Pointer> agents = getAgentsListFromObjectsList(objects);

			removeFriendFromAgentsList(agents);

			if(agents.size() != 0) { // If there is something to shoot, then think before open fire !!!! 
				Agent::Pointer ptr = selectWeakestAgent(agents);
				
				Math::Vector2<Config::Real> otherPosition = ptr->getPosition().projectZ() ;
				Math::Vector2<Config::Real> otherVelocity = ptr->getVelocity() ;
				Config::Real bulletSpeed = m_weapon.getArchetype()->m_speed ;
				Config::Real distanceToTarget = (getPosition().projectZ()-otherPosition).norm() ;
				Config::Real timeToTarget = distanceToTarget/bulletSpeed ;
				fire(otherPosition+otherVelocity*timeToTarget) ;
				//fire(ptr->getPosition().projectZ()) ;
				
			}
		}
		m_perception->reset() ;
	}

	Math::Vector2<Config::Real> SmithAgent::randomVelocity() const {
		Math::Vector2<Config::Real> velocity(rand()-RAND_MAX/2, rand()-RAND_MAX/2) ;
		velocity = velocity.normalized() * m_archetype->m_speed ;
		return velocity;
	}

	
	Math::Vector2<Config::Real> SmithAgent::getVelocity() const	{
		const Map::GroundCellDescription & currentCell = OgreFramework::GlobalConfiguration::getCurrentMap()->getCell(getPosition().projectZ()) ;
		return m_velocity*(1.0-currentCell.m_speedReduction) ;
	}
}