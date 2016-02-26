#include <stdafx.h>
#include <GameElements/RandomAgent.h>
#include <GameElements/BulletBase.h>
#include <Math/RoadMap.h>

using std::cout;
using std::endl;

namespace GameElements
{
	RandomAgent::RandomAgent( const UnitsArchetypes::Archetype * archetype, const WeaponsArchetypes::Archetype * weaponArchetype, bool computeCollisionMesh/*=true*/ ) : SmithAgent(archetype, weaponArchetype, computeCollisionMesh) {
		m_velocity = randomVelocity() ;
		//_map = new Math::RoadMap();
		OgreFramework::GlobalConfiguration::getCurrentMap()->show();
	}

	void RandomAgent::update( const Config::Real & dt )
	{
		SmithAgent::update(dt);
	}

	Map::GroundCellDescription RandomAgent::findEnemyCell() const
	{
		const Map::GroundCellDescription & currentCell = OgreFramework::GlobalConfiguration::getCurrentMap()->getCell(getPosition().projectZ()) ;



		return currentCell;
	}
}