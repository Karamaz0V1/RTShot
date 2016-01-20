#ifndef _GameElements_dijkstra_H
#define _GameElements_dijkstra_H

#include <OgreFramework/GlobalConfiguration.h>
#include <Math\Vector2.h>

namespace GameElements
{
	class dijkstra
	{
	protected:
		Math::Vector2<Config::Real> actualPosition ;
		Math::Vector2<Config::Real> desiredPosition;
	public:
		dijkstra(Math::Vector2<Config::Real> & actualPosition, Math::Vector2<Config::Real> & desiredPosition);
		Config::Real distEucli(Math::Vector2<Config::Real> a, Math::Vector2<Config::Real> b);
		void myWay();
		Config::Real cost(Math::Vector2<Config::Real> a, Math::Vector2<Config::Real> b);
	};
}

#endif