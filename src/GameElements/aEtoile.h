#ifndef _GameElements_aEtoile_H
#define _GameElements_aEtoile_H

#include <OgreFramework/GlobalConfiguration.h>
#include <Math\Vector2.h>

namespace GameElements
{
	class aEtoile
	{
	protected:
		Math::Vector2<Config::Real> actualPosition ;
		Math::Vector2<Config::Real> desiredPosition;
	public:
		aEtoile(Math::Vector2<Config::Real> & actualPosition, Math::Vector2<Config::Real> & desiredPosition);
		Config::Real distEucli(Math::Vector2<Config::Real> a, Math::Vector2<Config::Real> b);
		void myWay();
		Config::Real heuristic(Math::Vector2<Config::Real> & actualPosition, Math::Vector2<Config::Real> & desiredPosition);
		Config::Real cost(Math::Vector2<Config::Real> a, Math::Vector2<Config::Real> b);
	};
}

#endif