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
		const Map * cmap = OgreFramework::GlobalConfiguration::getCurrentMap();
		Vector2<int> tposition = cmap->toGridCoordinates(targetPosition);
		queue<Vector2<int> > cellStack;
		
		_map[tposition] = 1;
		cellStack.push(tposition);

		cout << "Map creation : " << cmap->width() << "x" << cmap->height() << "..." << endl;
		while (! cellStack.empty()) {
			Vector2<int> cell = cellStack.front();
			cellStack.pop();

			for (int i = -1; i <= 1; i++) {
				for (int j = -1; j <= 1; j++) {
					if (j == 0 && i == 0) continue;
					Vector2<int> neighbour(cell[0] + i, cell[1] + j);
					if (! _map.isValid(neighbour)) continue;

					if (cmap->getCell(cmap->toWorldCoordinates(neighbour)).m_speedReduction == 1) {
						_map[neighbour] = numeric_limits<double>::max();
						continue;
					}
					
					double neighbourvalue = _map[cell];
					if (abs(i) == abs(j)) neighbourvalue += 1.414214;
					else neighbourvalue += 1;

					if (_map[neighbour] == 0 || _map[neighbour] > neighbourvalue) {
						_map[neighbour] = neighbourvalue;
						cellStack.push(neighbour);
					}
				}
			}
		}

		cout << "Done." << endl;
	}

	RoadMap::~RoadMap() {
		//delete _map;
	}
	
	Vector2<Real> RoadMap::getTargetWay(Vector2<Real> const & worldCoordinates) const {
		cout << "[RoadMap] On me demande le chemin." << endl;
		const Map * cmap = OgreFramework::GlobalConfiguration::getCurrentMap();
		Vector2<int> gridCoordinates = cmap->toGridCoordinates(worldCoordinates);
		if (_map[gridCoordinates] == 1) {
			cout << "[RoadMap] Normalement t'es arrivé gros." << endl;
			return Vector2<Real>();
		}

		Vector2<Real> bestWay;
		double bestWayScore = _map[gridCoordinates];

		// Shit happens
		if (bestWayScore == 0) bestWayScore = numeric_limits<double>::max();

		for (int i = -1; i <= 1; i++)
			for (int j = -1; j <= 1; j++) {
				if (j == 0 && i == 0) continue;
				Vector2<int> neighbour(gridCoordinates[0] + i, gridCoordinates[1] + j);
				if (! _map.isValid(neighbour)) continue;
				if (_map[neighbour] > bestWayScore) continue;
				if (_map[neighbour] == bestWayScore && abs(i) == abs(j)) continue;
				bestWayScore = _map[neighbour];
				// Todo: set bestWay direction refactor
				bestWay[0] = i ;//- j * 1.0f / 2;
				bestWay[1] = j ;//- i * 1.0f / 2;
			}

		/*if (bestWay[0] == 0 && bestWay[1] == 0) {
			cout << "arrivé ? -------------------------------- " << endl;
			cout << "case actuelle : " << _map[gridCoordinates] << endl;
			for (int i = -1; i <= 1; i++)
				for (int j = -1; j <= 1; j++) {
					Vector2<int> neighbour(gridCoordinates[0] + i, gridCoordinates[1] + j);
					cout << "case " << i << " " << j << " : " << _map[neighbour] << endl;
				}
			cout << "----------------------------------------- " << endl;
		} else {
			cout << "On bouge" << endl;
		}*/

		return bestWay.normalized();

	}

}