#include <stdafx.h>
#include <GameElements/Map.h>
#include <Math/RoadMap.h>
#include <queue>
#include <limits>

using namespace GameElements;
using namespace DataStructure;
using namespace Math;
using namespace Config;
using namespace std;

namespace Math
{
	RoadMap::RoadMap(Math::Vector2<Config::Real> & targetPosition, std::vector<Math::Vector2<Config::Real> > & agentsPositions) :
		_map(Interval<float>(0, OgreFramework::GlobalConfiguration::getCurrentMap()->width()), Interval<float>(0, OgreFramework::GlobalConfiguration::getCurrentMap()->height()), 1)
	{
		//Map::get
		const Map * cmap = OgreFramework::GlobalConfiguration::getCurrentMap();
		//_map = new Grid<int>(Interval<float>(0, cmap->width()), Interval<float>(0, cmap->height()), 1);
		Vector2<int> tposition = cmap->toGridCoordinates(targetPosition);
		queue<Vector2<int> > cellStack;
		
		_map[tposition] = 0;
		cellStack.push(tposition);

		cout << "Map creation : " << cmap->width() << "x" << cmap->height() << "..." << endl;
		while (! cellStack.empty()) {
			Vector2<int> cell = cellStack.front();
			cellStack.pop();

			for (int i = -1; i <= 1; i++)
				for (int j = -1; j <=1; j++) {
					if (j == 0 && i == 0) continue;
					Vector2<int> neighbour(cell[0] + i, cell[1] + j);
					if (! _map.isValid(neighbour)) continue;
					if (_map[neighbour] != 0) continue;
					if (cmap->getCell(cmap->toWorldCoordinates(neighbour)).m_speedReduction == 1) { // impassable
						// QUICK FIX
						for (int ii = -3; ii <= 3; ii++)
							for (int jj = -3; jj <= 3; jj++) {
								Vector2<int> neighbourMountain(neighbour[0] + ii, neighbour[1] + jj);
								_map[neighbour] = numeric_limits<unsigned int>::max();
							}
						continue;
					}
					_map[neighbour] = _map[cell] + cmap->getCell(cmap->toWorldCoordinates(neighbour)).m_speedReduction * 10;
					//cout << _map[cell] + 1 << " " ;
					cellStack.push(neighbour);
				}
		}
		cout << "Done." << endl;
	}

	RoadMap::~RoadMap() {
		//delete _map;
	}
	
	Vector2<Real> RoadMap::getTargetWay(Vector2<Real> const & worldCoordinates) const {
		const Map * cmap = OgreFramework::GlobalConfiguration::getCurrentMap();
		Vector2<int> gridCoordinates = cmap->toGridCoordinates(worldCoordinates);
		if (_map[gridCoordinates] == 0) return Vector2<Real>();

		Vector2<Real> bestWay;
		unsigned int bestWayScore = _map[gridCoordinates];

		for (int i = -1; i <= 1; i++)
			for (int j = -1; j <= 1; j++) {
				if (j == 0 && i == 0) continue;
				Vector2<int> neighbour(gridCoordinates[0] + i, gridCoordinates[1] + j);
				if (! _map.isValid(neighbour)) continue;
				if (_map[neighbour] >= bestWayScore) continue;
				bestWayScore = _map[neighbour];
				// Todo: set bestWay direction refactor
				bestWay[0] = i ;//- j * 1.0f / 2;
				bestWay[1] = j ;//- i * 1.0f / 2;
			}

		return bestWay.normalized();

	}

}