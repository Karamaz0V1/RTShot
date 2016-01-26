#include <stdafx.h>
#include <GameElements/dijkstra.h>

namespace GameElements
{
	struct path
	{
		Config::Real pathCost;
		Math::Vector2<Config::Real> maPos;
		path * monPred;		
	};
	dijkstra::dijkstra(Math::Vector2<Config::Real> & actualPosition, Math::Vector2<Config::Real> & desiredPosition):actualPosition(actualPosition),desiredPosition(desiredPosition)
	{
	}

	Config::Real dijkstra::distEucli(Math::Vector2<Config::Real> a, Math::Vector2<Config::Real> b){
		Config::Real temp=(a[0]-b[0])*(a[0]-b[0]) + (a[1]-b[1])*(a[0]-b[0]);
		return sqrt(temp);
	}

	void dijkstra::myWay()
	{
		//Q.push(0, source)
		struct path source={0.0 , actualPosition, NULL};
		std::vector<path> Q;
		Q.push_back(source);

		//while(!Q.empty()) {
		while(!Q.empty()){
		//	c = Q.pop()
			struct path c=Q.back();
		//	for all	successors S of c {
			Math::Vector2<Config::Real> suc(actualPosition);
			for (Config::Real i=-1; i<1; i++){
				for(Config::Real j=-1; j<1; j++){
					if(i==0&&j==0){
						suc[0]=actualPosition[0]+i;
						suc[1]=actualPosition[1]+j;
						struct path S={0.0,suc,NULL};
						//		if(cost(c,S)+cost(c)<cost(S)) {
						if(1+c.pathCost<S.pathCost){
							//			Q.push(cost(c,S)+cost[c], S)
							Q.push_back(S);
							//			cost[S] = cost[c] + cost(c,S)
							S.pathCost=c.pathCost+1;
							//			pre[S]=c
							S.monPred=&c;
						}
					}
				}
			}
		
		} 
	}

	Config::Real dijkstra::cost(Math::Vector2<Config::Real> a, Math::Vector2<Config::Real> b)
	{
		Config::Real dCost = distEucli(a, b);
		return dCost;
	}
}