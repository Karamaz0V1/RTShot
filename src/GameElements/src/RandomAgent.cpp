#include <stdafx.h>
#include <GameElements/RandomAgent.h>
#include <GameElements/BulletBase.h>

namespace GameElements
{

	void RandomAgent::onCollision( const CollisionMessage & message )
	{
		
		Agent::Pointer agent1 = boost::dynamic_pointer_cast<Agent>(message.m_object1);
		Agent::Pointer agent2 = boost::dynamic_pointer_cast<Agent>(message.m_object2);

		if(agent1.get() == NULL || agent2.get() == NULL) return;
		/*
		if (m_velocity[0] * m_velocity[0] > m_velocity[1] * m_velocity[1])
			m_velocity[1] = -m_velocity[1];
		else
			m_velocity[0] = -m_velocity[0];*/
		if(microsoftRecrute==true){
			if (this == agent1.get())
				m_velocity = (agent1->getPosition().projectZ() - agent2->getPosition().projectZ()).normalized() * getMaxSpeed();
			else		// this == agent2
				m_velocity = (agent2->getPosition().projectZ() - agent1->getPosition().projectZ()).normalized() * getMaxSpeed();
		}
	}


	RandomAgent::RandomAgent( const UnitsArchetypes::Archetype * archetype, const WeaponsArchetypes::Archetype * weaponArchetype, bool computeCollisionMesh/*=true*/ ) : SmithAgent(archetype, weaponArchetype, computeCollisionMesh) {
		m_velocity = randomVelocity() ;
		microsoftRecrute=false;
	}

	void RandomAgent::update( const Config::Real & dt )
	{
		int testCollision = 1;
		int dtMagie = dt;
		//Check the collision at each frame
		if(dtMagie%testCollision==0){
			microsoftRecrute=true;
		}else{
			microsoftRecrute=false;
		}


		// Computes movements
		const Map::GroundCellDescription & currentCell = OgreFramework::GlobalConfiguration::getCurrentMap()->getCell(getPosition().projectZ()) ;
		Math::Vector2<Config::Real> newPosition = getPosition().projectZ()+m_velocity*dt*(1.0-currentCell.m_speedReduction) ;
		std::vector<Triggers::CollisionObject::Pointer> objects = m_perception->perceivedAgents();
		//for (std::vector<Triggers::CollisionObject::Pointer>::const_iterator it = objects.begin(); it != objects.end(); it++) {
			//(*it)->
		//}
		
		// If displacement is valid, the agent moves, otherwise, a new random velocity is computed
		if(OgreFramework::GlobalConfiguration::getCurrentMap()->isValid(newPosition) && OgreFramework::GlobalConfiguration::getCurrentMap()->getCell(newPosition).m_speedReduction!=1.0)
		{
			setOrientation(m_velocity) ;
			setPosition(newPosition.push(0.0)) ;
		} else {
			m_velocity = randomVelocity() ;
		}
		// Handles perception and fires on agents
		if(canFire())
		{
			std::vector<Agent::Pointer> agents = getAgentsListFromObjectsList(objects);

			removeFriendFromAgentsList(agents);

			if(agents.size() != 0) // If there is something to shoot, then think before open fire !!!! 
			{
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

	Map::GroundCellDescription RandomAgent::findEnemyCell() const
	{
		const Map::GroundCellDescription & currentCell = OgreFramework::GlobalConfiguration::getCurrentMap()->getCell(getPosition().projectZ()) ;



		return currentCell;
	}
}