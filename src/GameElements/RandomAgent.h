#ifndef _GameElements_RandomAgent_H
#define _GameElements_RandomAgent_H

#include <GameElements/SmithAgent.h>
#include <Math/RoadMap.h>

namespace GameElements
{
	class RandomAgent : public SmithAgent
	{
		
	public:
		RandomAgent(const UnitsArchetypes::Archetype * archetype, const WeaponsArchetypes::Archetype * weaponArchetype, bool computeCollisionMesh=true);

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// \fn	virtual void NullAgent::update(const Config::Real & dt);
		///
		/// \brief	Updates the given dt.
		///
		/// \author	Fabrice Lamarche, university of Rennes 1
		/// \param	dt	The dt.
		////////////////////////////////////////////////////////////////////////////////////////////////////
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

		Map::GroundCellDescription findEnemyCell() const;
	private:
		Math::RoadMap * _map;

	};
}

#endif