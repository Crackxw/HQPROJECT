// Point.cpp: implementation of the Point class.
//
//////////////////////////////////////////////////////////////////////

#include "Point.h"
#include <stdlib.h>


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

Point::Point()
{

}

Point::~Point()
{

}

PlayersPoint Point::Calc(PlayersPoint stPlayers)
{	
	if(stPlayers.siPlayer1 == -1 || stPlayers.siPlayer2 == -1) 
		return stPlayers;
	else
		return Formal(stPlayers);
}

PlayersPoint Point::AddPoint(PlayersPoint stPlayersPoint,double dPer)
{
	int siPlusPoint;
	if((stPlayersPoint.siPlayer1 - stPlayersPoint.siPlayer2) >= 0)
	{
		 siPlusPoint = (int)((50 * (dPer*0.01))+0.5);
	}
	else
	{
		siPlusPoint  = (int)((50 * ((100 - dPer)*0.01))+0.5);
	}

	stPlayersPoint.siPlayer1 += siPlusPoint;
	stPlayersPoint.siPlayer2 -= siPlusPoint;
	stPlayersPoint.siPlayerPoint  = siPlusPoint;


	if(stPlayersPoint.siPlayer1 <= 0)
		stPlayersPoint.siPlayer1 = 0;
	if(stPlayersPoint.siPlayer2 <= 0)
		stPlayersPoint.siPlayer2 = 0;

	return stPlayersPoint;

}


PlayersPoint Point::Formal(PlayersPoint stPlayersPoint)
{
	double	 dPer;
	int		 siComparePoint = abs(stPlayersPoint.siPlayer1 - stPlayersPoint.siPlayer2);

	if(siComparePoint >= 700) dPer = 1;
	else if((siComparePoint >= 650) && (siComparePoint <= 699)) dPer = 2;
	else if((siComparePoint >= 600) && (siComparePoint <= 649)) dPer = 3;
	else if((siComparePoint >= 550) && (siComparePoint <= 599)) dPer = 4;
	else if((siComparePoint >= 500) && (siComparePoint <= 549)) dPer = 5;
	else if((siComparePoint >= 450) && (siComparePoint <= 499)) dPer = 7;
	else if((siComparePoint >= 400) && (siComparePoint <= 449)) dPer = 9.1;
	else if((siComparePoint >= 350) && (siComparePoint <= 399)) dPer = 11.8;
	else if((siComparePoint >= 300) && (siComparePoint <= 349)) dPer = 15.1;
	else if((siComparePoint >= 250) && (siComparePoint <= 299)) dPer = 19.2;
	else if((siComparePoint >= 200) && (siComparePoint <= 249)) dPer = 24;
	else if((siComparePoint >= 150) && (siComparePoint <= 199)) dPer = 29.7;
	else if((siComparePoint >= 100) && (siComparePoint <= 149)) dPer = 36;
	else if((siComparePoint >=  50) && (siComparePoint <=  99)) dPer = 42.9;
	else if((siComparePoint >=  0) &&  (siComparePoint <=  49)) dPer = 50;

	return AddPoint(stPlayersPoint,dPer);
}







