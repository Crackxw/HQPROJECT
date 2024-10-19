// Point.h: interface for the Point class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_POINT_H__DBE7093F_4738_4639_AB8A_F805223331D8__INCLUDED_)
#define AFX_POINT_H__DBE7093F_4738_4639_AB8A_F805223331D8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

struct PlayersPoint
{
	int siPlayer1;
	int siPlayer2;
	int siPlayerPoint;

	PlayersPoint::PlayersPoint(int p1, int p2) : siPlayer1(p1), siPlayer2(p2)	{}
	PlayersPoint::PlayersPoint()												{}
};

class Point  
{
public:
	Point();
	virtual ~Point();

	PlayersPoint Calc(PlayersPoint siPlayers);
	PlayersPoint AddPoint(PlayersPoint stPlayersPoint,double dPer);
	PlayersPoint Formal(PlayersPoint stPlayersPoint);

};

#endif // !defined(AFX_POINT_H__DBE7093F_4738_4639_AB8A_F805223331D8__INCLUDED_)
