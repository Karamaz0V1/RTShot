#ifndef _GameElements_SmithAgent_H
#define _GameElements_SmithAgent_H

#include <OgreFramework/GlobalConfiguration.h>
#include <GameElements/Agent.h>

namespace GameElements
{
	class SmithAgent : public Agent {
		public:
			SmithAgent(const UnitsArchetypes::Archetype * archetype, const WeaponsArchetypes::Archetype * weaponArchetype, bool computeCollisionMesh = true);
			virtual void update(const Config::Real & dt);
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

		protected:
			virtual Agent::Pointer selectWeakestAgent(const std::vector<Agent::Pointer> & agents) const;
			virtual void removeFriendFromAgentsList(std::vector<Agent::Pointer> & agents) const;
			virtual std::vector<Agent::Pointer> getAgentsListFromObjectsList(const std::vector<Triggers::CollisionObject::Pointer> & objects) const;
			Math::Vector2<Config::Real> randomVelocity() const;
			Math::Vector2<Config::Real> getVelocity() const;
			//refresh collision
			bool m_collision;
	};
}
#endif