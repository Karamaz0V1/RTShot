#include <stdafx.h>
#include <GameElements/AgentAI.h>
namespace GameElements {
	AgentAI::AgentAI( const UnitsArchetypes::Archetype * archetype, const WeaponsArchetypes::Archetype * weaponArchetype, bool computeCollisionMesh/*=true*/ ) : Agent(archetype, weaponArchetype, computeCollisionMesh)
	{
	}
}