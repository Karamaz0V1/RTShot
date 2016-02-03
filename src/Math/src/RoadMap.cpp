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

		cout << "[RoadMap] Map creation : " << cmap->width() << "x" << cmap->height() << "..." << endl;
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

		cout << "[RoadMap] Done." << endl;
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

	Math::Vector2<Config::Real> RoadMap::getPositionTowardTarget(Math::Vector2<Config::Real> & velocity, const Math::Vector2<Config::Real> const & worldCoordinates, const double & agentSpeed, const Config::Real & dt) const {		
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