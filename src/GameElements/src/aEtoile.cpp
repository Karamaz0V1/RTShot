#include <stdafx.h>
#include <GameElements/aEtoile.h>

namespace GameElements
{
	aEtoile::aEtoile(Math::Vector2<Config::Real> & actualPosition, Math::Vector2<Config::Real> & desiredPosition):actualPosition(actualPosition),desiredPosition(desiredPosition)
	{
	}

	Config::Real aEtoile::distEucli(Math::Vector2<Config::Real> a, Math::Vector2<Config::Real> b){
		Config::Real temp=(a[0]-b[0])*(a[0]-b[0]) + (a[1]-b[1])*(a[0]-b[0]);
		return sqrt(temp);
	}

	void aEtoile::myWay()
	{
		//Q.push(H(source), source)
		Math::Vector2<Config::Real>  source=actualPosition;
		std::pair<Config::Real, Math::Vector2<Config::Real>> paire(heuristic(source, desiredPosition),source);
			
		std::vector<std::pair<Config::Real,Math::Vector2<Config::Real> > > Q;
		
		Q.push_back(paire);
		//while(!Q.empty()) {
		while(!Q.empty())
		{
			//c = Q.pop()
			std::pair<Config::Real, Math::Vector2<Config::Real>> c=Q.back();
			Q.pop_back();
			//if (c==target) {
			if(c.second==desiredPosition){
				// Path is found ! 
				std::cout<<"nailed it !\n";
			}
			
			//for all	successors S of c {
			for(std::vector<std::pair<Config::Real,Math::Vector2<Config::Real> > >::iterator i = Q.begin(); i != Q.end();++i){
				//if(cost(c,S)+cost[c]<cost[S]) {
				if(cost(c.second,i->second)+c.first<i->first){
					//Q.push(cost(c,S)+cost[c]+H(S), S)
					paire.first=cost(c.second,i->second)+c.first+heuristic(i->second,desiredPosition);
					paire.second=i->second;
					Q.push_back(paire);
					//cost[S] = cost[c] + cost(c,S)
					i->first=c.first+cost(c.second,i->second);
					//pre[S]=c	
				}
			}
		}
	}

	Config::Real aEtoile::heuristic(Math::Vector2<Config::Real> & actualPosition, Math::Vector2<Config::Real> & desiredPosition)
	{
		Config::Real distHeur = distEucli(actualPosition, desiredPosition);
		return distHeur;
	}

	Config::Real aEtoile::cost(Math::Vector2<Config::Real> a, Math::Vector2<Config::Real> b)
	{
		Config::Real dCost = distEucli(a, b);
		return dCost;
	}
}