#include <stdafx.h>
#include <GameElements/SmithAgent.h>
#include <GameElements/map.h>

using std::vector;
using std::cout;
using std::endl;
//TODO
/*
	v�rifier fluidit� changement destination
	v�rifier si IA ou player
*/
namespace GameElements {
	DesignPattern::StaticMember<System::MessageEmitter<SmithAgent::MovedObjectMessage> > SmithAgent::m_movedEmitter ;
	Math::Vector2<Config::Real> newPosition;
	void SmithAgent::onCollision( const CollisionMessage & message )
	{
		
		Agent::Pointer agent1 = boost::dynamic_pointer_cast<Agent>(message.m_object1);
		Agent::Pointer agent2 = boost::dynamic_pointer_cast<Agent>(message.m_object2);

		if(agent1.get() == NULL || agent2.get() == NULL) return;
		/*
		if (m_velocity[0] * m_velocity[0] > m_velocity[1] * m_velocity[1])
			m_velocity[1] = -m_velocity[1];
		else
			m_velocity[0] = -m_velocity[0];*/

		//if collision is active
		//if(m_collision==true){
			if (this == agent1.get()){
				//m_velocity = (getPosition().projectZ() - agent2->getPosition().projectZ()).normalized() * getMaxSpeed();
				m_collision=true;
			}else{		// this == agent2
				m_collision=true;
				//m_velocity = (getPosition().projectZ() - agent1->getPosition().projectZ()).normalized() * getMaxSpeed();
			}
	}
	SmithAgent::SmithAgent( const UnitsArchetypes::Archetype * archetype, const WeaponsArchetypes::Archetype * weaponArchetype, bool computeCollisionMesh/*=true*/ ) : Agent(archetype, weaponArchetype, computeCollisionMesh), time(0.0) {
		m_collision=false;
		gotTarget = false;
		m_mapInit = false;
	}

	System::MessageEmitter<SmithAgent::MovedObjectMessage> * SmithAgent::getMovedEmitter()
	{
		return m_movedEmitter.getInstance() ;
	}

	void SmithAgent::noticeMeSenpai(SmithAgent * objet)
	{
		getMovedEmitter()->send(MovedObjectMessage(objet)) ;
	}

	void SmithAgent::setTarget(SmithAgent * target) {
		if(target != NULL) {	
			if(target->getArchetype()->m_name.back() != this->getArchetype()->m_name.back()) {
				m_target = target;
				gotTarget = true;
				::std::cout<<"Target is mine "<<this->getArchetype()->m_name<<"->"<<m_target->getArchetype()->m_name<<::std::endl;
			}
			else {
				m_target = NULL;
				gotTarget=false;
				::std::cout<<"Target is mine "<<this->getArchetype()->m_name<<"-> alli�"<<::std::endl;
			}
			m_destination = target->getPosition().projectZ();
			go2(m_destination);
		}
		else {
			::std::cout<<"Target is mine "<<this->getArchetype()->m_name<<"-> rien"<<::std::endl;
			m_target=target;
			gotTarget=false;
		}
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

	void SmithAgent::update( const Config::Real & dt )
	{
		//cout << "[RandomAgent] update" << endl;
		if (dt == 0) return;
		// Computes movements
		const Map::GroundCellDescription & currentCell = OgreFramework::GlobalConfiguration::getCurrentMap()->getCell(getPosition().projectZ()) ;
		//Math::Vector2<Config::Real> newPosition = getPosition().projectZ()+m_velocity*dt*(1.0-currentCell.m_speedReduction) ;
		//cout << "[RandomAgent] Je demande mon chemin" << endl;
		if(time/2 >= 1) {
			time=0;
			if(gotTarget) {
				go2(m_target->getPosition().projectZ());
			}
		}
		else
			time += dt;
		m_velocity = destinationWay() * m_archetype->m_speed;
		//cout << "Position : " << getPosition().projectZ() << " Velocit�: " << m_velocity << endl;

		Math::Vector2<Config::Real> newPosition;
		//Math::Vector2<Config::Real> newPosition = _map->getPositionTowardTarget(m_velocity, getPosition().projectZ(), m_archetype->m_speed, dt);

		std::vector<Triggers::CollisionObject::Pointer> objects = m_perception->perceivedAgents();
		//for (std::vector<Triggers::CollisionObject::Pointer>::const_iterator it = objects.begin(); it != objects.end(); it++) {
			//(*it)->
		//}
		
		// If displacement is valid, the agent moves, otherwise, a new random velocity is computed

		if (m_collision) m_velocity = m_velocity.rotate90();
		
		newPosition = getPosition().projectZ() + m_velocity * dt * (1.1 - currentCell.m_speedReduction);
		setOrientation(m_velocity);
		setPosition(newPosition.push(0.0));

		//if(OgreFramework::GlobalConfiguration::getCurrentMap()->isValid(newPosition) && OgreFramework::GlobalConfiguration::getCurrentMap()->getCell(newPosition).m_speedReduction!=1.0 && m_collision == false)
		//{
		//	newPosition = getPosition().projectZ()+m_velocity*dt*((1.0-currentCell.m_speedReduction) + (currentCell.m_speedReduction==1));
		//	setOrientation(m_velocity) ;
		//	setPosition(newPosition.push(0.0)) ;
		//}else if(m_collision==true){
		//	newPosition = getPosition().projectZ()+m_velocity.rotate90()*dt*((1.0-currentCell.m_speedReduction) + (currentCell.m_speedReduction==1));
		//	setPosition(newPosition.push(0.0)) ;
		//}else {
		//	//m_velocity = randomVelocity() ;
		//	newPosition = getPosition().projectZ()+m_velocity.rotate90()*dt*((1.0-currentCell.m_speedReduction) + (currentCell.m_speedReduction==1));
		//	setPosition(newPosition.push(0.0)) ;
		//}

		m_collision = false;
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

	Math::Vector2<Config::Real> SmithAgent::randomVelocity() const {
		Math::Vector2<Config::Real> velocity(rand()-RAND_MAX/2, rand()-RAND_MAX/2) ;
		velocity = velocity.normalized() * m_archetype->m_speed ;
		return velocity;
	}

	
	Math::Vector2<Config::Real> SmithAgent::getVelocity() const	{
		const Map::GroundCellDescription & currentCell = OgreFramework::GlobalConfiguration::getCurrentMap()->getCell(getPosition().projectZ()) ;
		return m_velocity*(1.0-currentCell.m_speedReduction) ;
	}

	void SmithAgent::onSelect() {
		::std::cout<<"OMG Flantier :')"<<::std::endl;
		noticeMeSenpai(this);
	}

	void SmithAgent::onUnselect() {
		::std::cout<<"jpp de ma femme Flantier, LOL"<<::std::endl;
		noticeMeSenpai();
	}

	void SmithAgent::go2(Math::Vector2<Config::Real> destination) {
		::std::cout<<"Destination..."<<destination<<::std::endl ;
		if (m_mapInit) delete m_map;
		m_map = new Math::RoadMap(destination);//, m_position);
		m_mapInit = true;
	}

	Math::Vector2<Config::Real> SmithAgent::destinationWay() const {
		if (! m_mapInit) return Math::Vector2<Config::Real>();
		return m_map->getTargetWay(m_position, m_archetype->m_speed);
	}
}