#ifndef _GameElements_RoadMap_H
#define _GameElements_RoadMap_H

#include <array>
#include <vector>
#include <OgreFramework/GlobalConfiguration.h>
#include <Math\Vector2.h>

namespace Math {
	class RoadMap {
		public:
			RoadMap(Math::Vector2<Config::Real> & targetPosition, std::vector<Math::Vector2<Config::Real> > & agentsPositions);

		private:
			std::array<int, 100> _roadMap;
	};
}

#endif