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
		_map(Interval<float>(0, OgreFramework::GlobalConfiguration::getCurrentMap()->width()), Interval<float>(0, OgreFramework::GlobalConfiguration::getCurrentMap()->height()), 1),
		_cmap(OgreFramework::GlobalConfiguration::getCurrentMap())
	{
		Vector2<int> tposition = _cmap->toGridCoordinates(targetPosition);
		queue<Vector2<int> > cellStack;
		
		_map[tposition] = 1;
		cellStack.push(tposition);

		cout << "[RoadMap] Map creation : " << _cmap->width() << "x" << _cmap->height() << "..." << endl;
		while (! cellStack.empty()) {
			Vector2<int> cell = cellStack.front();
			cellStack.pop();

			for (int i = -1; i <= 1; i++) {
				for (int j = -1; j <= 1; j++) {
					if (j == 0 && i == 0) continue;
					Vector2<int> neighbour(cell[0] + i, cell[1] + j);
					if (! _map.isValid(neighbour)) continue;

					if (_cmap->getCell(_cmap->toWorldCoordinates(neighbour)).m_speedReduction == 1) {
						if (_map[neighbour] == 0) {
						//_map[neighbour] = numeric_limits<double>::max();
						_map[neighbour] = _map[cell] + 10;
						cellStack.push(neighbour);
						}
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

		cout << "[RoadMap] Done." << endl;
	}

	RoadMap::~RoadMap() {
		//delete _map;
	}
	
	Vector2<Real> RoadMap::getTargetWay(const Vector2<Real> & worldCoordinates, const Config::Real & distanceAllowed) const {
		//cout << "[RoadMap] On me demande le chemin." << endl;
		Vector2<int> gridCoordinates = _cmap->toGridCoordinates(worldCoordinates);
		double bestWayScore = _map[gridCoordinates];

		if (bestWayScore < 1.1) {
			//cout << "[RoadMap] Normalement t'es arrivé gros." << endl;
			return Vector2<Real>();
		}

		Vector2<int> actualCoordinates = gridCoordinates;
		Vector2<int> bestWayCoordinates = gridCoordinates;

		while ( (bestWayScore + 0 > _map[gridCoordinates] - distanceAllowed) && (bestWayScore > 1.1)) {
		
			//cout << "[RoadMap] Je suis en " << actualCoordinates << endl;
			Vector2<int> neighbour;
			for (int i = -1; i <= 1; i++)
				for (int j = -1; j <= 1; j++) {
					if (j == 0 && i == 0) continue;
					neighbour= Vector2<int> (actualCoordinates[0] + i, actualCoordinates[1] + j);
					if (! _map.isValid(neighbour)) continue;
					if (_map[neighbour] > bestWayScore) continue;
					//if (_map[neighbour] == bestWayScore && abs(i) == abs(j)) continue;
					bestWayScore = _map[neighbour];
					bestWayCoordinates = neighbour;
				}
			//cout << "[RoadMap] Je vais en " << neighbour << endl;
			actualCoordinates = bestWayCoordinates;	
			//break; // TODO: refactor
		}

		Vector2<Real> bestWay(actualCoordinates - gridCoordinates);
		//cout << "[RoadMap] J'ai donné le chemin." << endl;

		return bestWay.normalized();
	}

	Math::Vector2<Config::Real> RoadMap::getPositionTowardTarget(Math::Vector2<Config::Real> & velocity, const Math::Vector2<Config::Real> & worldCoordinates, const double & agentSpeed, const Config::Real & dt) const {		
		double maxdist = agentSpeed * dt * 10;
		cout << "[RoadMap] [getPosition] Distance autorisée : " << maxdist << endl;
		const Map * cmap = OgreFramework::GlobalConfiguration::getCurrentMap();
		Vector2<int> gridCoordinates = cmap->toGridCoordinates(worldCoordinates);
		

		// TODO: return target when possible
		// TODO: while dist > 0 : find next best cell; if dist - cellCost > 0 : select cell then iter;

		double dist = 0;
		Vector2<int> newGridCoordinates = gridCoordinates;
		bool pathfind = false;

		while (pathfind == false) {

			if (_map[newGridCoordinates] == 1) {
				cout << "[RoadMap] Normalement t'es arrivé gros." << endl;
				//Vector2<int> ivelocity(newGridCoordinates - gridCoordinates);
				//velocity[0] = ivelocity[0];
				//velocity[1] = ivelocity[1];
				//velocity = velocity.normalized();
				return cmap->toWorldCoordinates(newGridCoordinates);
			}

			Vector2<int> bestNeighbour; // \o~~ 
			double bestWayScore = _map[gridCoordinates];

			//// Shit happens
			//if (bestWayScore == 0) bestWayScore = numeric_limits<double>::max();

			for (int i = -1; i <= 1; i++)
				for (int j = -1; j <= 1; j++) {
					if (j == 0 && i == 0) continue;
					Vector2<int> neighbour(newGridCoordinates[0] + i, newGridCoordinates[1] + j);
					if (! _map.isValid(neighbour)) continue;
					if (_map[neighbour] > bestWayScore) continue;
					if (_map[neighbour] == bestWayScore && abs(i) == abs(j)) continue;
					bestWayScore = _map[neighbour];
					bestNeighbour = neighbour;
				}

			cout << "[RoadMap] [getPosition] Distance actuelle : " << dist << " voisin selectionné : " << bestNeighbour << endl;
			if (dist + _map[newGridCoordinates] - _map[bestNeighbour] <= maxdist) {
				dist += _map[newGridCoordinates] - _map[bestNeighbour];
				newGridCoordinates = bestNeighbour;
			} else {
				pathfind = true;
			}

		}

		cout << "[RoadMap] [getPosition] ----- Recap -----  " << endl;
		cout << "Distance uutorisée : " << maxdist << endl;
		cout << "Distance parcourue : " << dist << endl;
		cout << "Case de départ : " <<  gridCoordinates << endl;
		cout << "Case d'arrivée : " << newGridCoordinates << endl;
		cout << "------------------------------------------ " << endl;
		//cout << "[RoadMap] Normalement t'es arrivé gros." << endl;
		//Vector2<int> ivelocity(newGridCoordinates - gridCoordinates);
		//velocity[0] = ivelocity[0];
		//velocity[1] = ivelocity[1];
		//velocity = velocity.normalized();
		return cmap->toWorldCoordinates(newGridCoordinates);
	}

}