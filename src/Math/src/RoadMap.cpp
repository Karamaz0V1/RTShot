#include <stdafx.h>
#include <GameElements/Map.h>
#include <Math/RoadMap.h>

using namespace GameElements;
using namespace DataStructure;
using namespace std;

namespace Math
{
	RoadMap::RoadMap(Math::Vector2<Config::Real> & targetPosition, std::vector<Math::Vector2<Config::Real> > & agentsPositions)
	{
		//Map::get
		const Map * cmap = OgreFramework::GlobalConfiguration::getCurrentMap();
		_map = new Grid<int>(Interval<float>(0, cmap->width()), Interval<float>(0, cmap->height()), 1);

		cout << "[RoadMap] Create a " << _map->xSize() << "x" << _map->ySize() << " grid." << endl;
	}

	RoadMap::~RoadMap() {
		delete _map;
	}
}