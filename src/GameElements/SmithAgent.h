#ifndef _GameElements_SmithAgent_H
#define _GameElements_SmithAgent_H

#include <OgreFramework/GlobalConfiguration.h>
#include <GameElements/Agent.h>
#include <Math/RoadMap.h>

namespace GameElements
{
	class SmithAgent : public Agent {
		public:

			struct MovedObjectMessage
		{
			SmithAgent * m_selected;

			MovedObjectMessage(SmithAgent * object)
				: m_selected(object)
			{}
		};

			SmithAgent(const UnitsArchetypes::Archetype * archetype, const WeaponsArchetypes::Archetype * weaponArchetype, bool computeCollisionMesh = true);
			virtual void update(const Config::Real & dt);
			void noticeMeSenpai(SmithAgent * objet = NULL);

	private:
		static DesignPattern::StaticMember<System::MessageEmitter<MovedObjectMessage> > m_movedEmitter;
	public:
		static System::MessageEmitter<MovedObjectMessage> * getMovedEmitter();
		void SmithAgent::go2(Math::Vector2<Config::Real> destination);
		Math::Vector2<Config::Real> destinationWay() const;
		void onSelect();
		void onUnselect();
		void setTarget(SmithAgent * target);

			////////////////////////////////////////////////////////////////////////////////////////////////////
			/// \fn	virtual void NullAgent::onCollision (const CollisionMessage & message);
			///
			/// \brief	Executes the collision action.
			///
			/// \author	Fabrice Lamarche, university of Rennes 1
			/// \param	message	The message.
			////////////////////////////////////////////////////////////////////////////////////////////////////
			virtual void onCollision (const CollisionMessage & message);
		protected:
			Math::Vector2<Config::Real> m_velocity;
			Math::Vector2<Config::Real> m_destination;
			SmithAgent * m_target;

		protected:
			virtual Agent::Pointer selectWeakestAgent(const std::vector<Agent::Pointer> & agents) const;
			virtual void removeFriendFromAgentsList(std::vector<Agent::Pointer> & agents) const;
			virtual std::vector<Agent::Pointer> getAgentsListFromObjectsList(const std::vector<Triggers::CollisionObject::Pointer> & objects) const;
			Math::Vector2<Config::Real> randomVelocity() const;
			Math::Vector2<Config::Real> getVelocity() const;
			bool gotTarget;
			//refresh collision
			bool m_collision;
			Math::RoadMap * m_map;
			bool m_mapInit;
	};
}
#endif