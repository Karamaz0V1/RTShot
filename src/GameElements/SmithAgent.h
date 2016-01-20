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

		protected:
			Math::Vector2<Config::Real> m_velocity;

		protected:
			virtual Agent::Pointer selectWeakestAgent(const std::vector<Agent::Pointer> & agents) const;
			virtual void removeFriendFromAgentsList(std::vector<Agent::Pointer> & agents) const;
			virtual std::vector<Agent::Pointer> getAgentsListFromObjectsList(const std::vector<Triggers::CollisionObject::Pointer> & objects) const;
			Math::Vector2<Config::Real> randomVelocity() const;
			Math::Vector2<Config::Real> getVelocity() const;
	};
}
#endif