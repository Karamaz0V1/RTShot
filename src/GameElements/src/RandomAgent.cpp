#include <stdafx.h>
#include <GameElements/RandomAgent.h>
#include <GameElements/BulletBase.h>

namespace GameElements
{


	void RandomAgent::onCollision( const CollisionMessage & message )
	{

	}

	Math::Vector2<Config::Real> RandomAgent::getVelocity() const
	{
		const Map::GroundCellDescription & currentCell = OgreFramework::GlobalConfiguration::getCurrentMap()->getCell(getPosition().projectZ()) ;
		return m_velocity*(1.0-currentCell.m_speedReduction) ;
	}

	RandomAgent::RandomAgent( const UnitsArchetypes::Archetype * archetype, const WeaponsArchetypes::Archetype * weaponArchetype, bool computeCollisionMesh/*=true*/ ) : AgentAI(archetype, weaponArchetype, computeCollisionMesh)
	{
		m_velocity = randomVelocity() ;
	}

	void RandomAgent::update( const Config::Real & dt )
	{
		// Computes movements
		const Map::GroundCellDescription & currentCell = OgreFramework::GlobalConfiguration::getCurrentMap()->getCell(getPosition().projectZ()) ;
		Math::Vector2<Config::Real> newPosition = getPosition().projectZ()+m_velocity*dt*(1.0-currentCell.m_speedReduction) ;
		// If displacement is valid, the agent moves, otherwise, a new random velocity is computed
		if(OgreFramework::GlobalConfiguration::getCurrentMap()->isValid(newPosition) && OgreFramework::GlobalConfiguration::getCurrentMap()->getCell(newPosition).m_speedReduction!=1.0)
		{
			setOrientation(m_velocity) ;
			setPosition(newPosition.push(0.0)) ;
		}
		else
		{
			m_velocity = randomVelocity() ;
		}
		// Handles perception and fires on agents
		if(canFire())
		{
			::std::vector<Triggers::CollisionObject::Pointer> objects = m_perception->perceivedAgents() ;
			for(int cpt=0 ; cpt<objects.size() ; ++cpt)
			{
				if(boost::dynamic_pointer_cast<Agent>(objects[cpt])==NULL)
				{
					::std::swap(objects[cpt], objects.back()) ;
					cpt--;
					objects.pop_back() ;
				}
			}
		
			// Remove friends from target list
			for(int cpt=0 ; cpt<objects.size() ; ++cpt) {
				Agent::Pointer ptr = boost::dynamic_pointer_cast<Agent>(objects[cpt]) ;
				
				std::string sLui = ptr->getArchetype()->m_name;
				std::string sMoi = this->getArchetype()->m_name;
				char sonType = sLui.back();
				char monType = sMoi.back();

				if (sonType == monType) {
					::std::swap(objects[cpt], objects.back()) ;
					objects.pop_back() ;
					cpt--;
				}
			}

			if(objects.size()!=0) // If there is something to shoot, then think before open fire !!!! 
			{
				int minTargetLifeInRange = boost::dynamic_pointer_cast<Agent>(objects[0])->getArchetype()->m_life;
				int target = 0;
				for(int cpt=1 ; cpt<objects.size() ; ++cpt) {
					int targetLife = boost::dynamic_pointer_cast<Agent>(objects[0])->getArchetype()->m_life;
					if (targetLife < minTargetLifeInRange) {
						minTargetLifeInRange = targetLife;
						int target = cpt;
					}
				}

				Agent::Pointer ptr = boost::dynamic_pointer_cast<Agent>(objects[target]) ;
				
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
}