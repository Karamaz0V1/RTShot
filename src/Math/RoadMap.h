#ifndef _GameElements_RoadMap_H
#define _GameElements_RoadMap_H

#include <array>
#include <vector>
#include <OgreFramework/GlobalConfiguration.h>
#include <Math\Vector2.h>

namespace Math {
	class RoadMap {
		public:
			RoadMap(Math::Vector2<Config::Real> & targetPosition = Math::Vector2<Config::Real>(15.0, 15.0), std::vector<Math::Vector2<Config::Real> > & agentsPositions = std::vector<Math::Vector2<Config::Real> >());
			virtual ~RoadMap();
			Math::Vector2<Config::Real> getTargetWay(Math::Vector2<Config::Real> const & worldCoordinates) const;
			Math::Vector2<Config::Real> getPositionTowardTarget(Math::Vector2<Config::Real> & velocity, const Math::Vector2<Config::Real> const & worldCoordinates, const double & agentSpeed, const Config::Real & dt) const;

		private:
			//std::array<int, 100> _roadMap; // TODO: recreate game map space in array cf. RandomAgent
			DataStructure::Grid<double> _map;
	};
}

#endif