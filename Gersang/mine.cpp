#include <GSL.h>
#include <main.h>

#include "maindata.h"
#include <etc.h>
#include "calcattack.h"
#include "music.h"
#include "mine.h"

#include <map.h>
#include <charinterface.h>
#include <bullet.h>
#include <smoke.h>
#include "text.h"

extern	_Map 				Map;
extern	_MainData			GD;									// ������ �ֵ����� 
extern	SI32				UserPlayerNumber;					// ����� ���� 
extern	_Text 				Text;								// �ؽ�Ʈ 
extern	int 				MapCenterX[][MAX_MAP_YSIZE];		// ������ �� ��ǥ������ �߽ɰ� 
extern	int 				MapCenterY[][MAX_MAP_YSIZE];
extern	_NewID 				ZeroID;
extern	cltCharInterface	clifChar;							// ĳ���� ���� �������̽� 


// ���� ���ڸ� ������ ���� ó��. 
BOOL DetectEnemyMine(SI32 x, SI32 y)
{
	PushEffect(EFFECT_EXP, x, y);

//	MineExplode(Map.GetMineMap(x, y), Map.GetMineCharMap(x,y),x, y, FALSE);

	// ���� ���ڴ� ���ش�. 
	Map.SetMineMap(x, y, -1);
	
	return TRUE;
}


// ���� ���ڸ� ��������� ó��. 
BOOL StepEnemyMine(SI32 x, SI32 y, SI32 playernumber)
{
	// ȭ�� ��鸲�� �����. 
	int a;
	GD.EarthQuake.SetEarthQuake();
	PushEffect(EFFECT_EXP, x, y);

	// ������ ��Ʈ�� ã�Ƽ� �����ش�. 
	if(playernumber == UserPlayerNumber)
	{
		SetMessage2(Text.MineExplodeMessage.Get(), playernumber, EFFECT_MINEEXPLODEMESSAGE);
	}
	a = Map.GetIDMap(x,y);
		  
//	MineExplode(Map.GetMineMap(x, y), Map.GetMineCharMap(x,y), x, y, TRUE);

	

	// ���� ���ڴ� ���ش�. 
	Map.SetMineMap(x, y, -1);

	return TRUE;
}


// x, y, �� ���ڰ� �ִ°�?
BOOL IsMine(SI32 x, SI32 y)
{

	if(Map.GetMineMap(x, y) >= 0)return TRUE;

	return FALSE;
}

// x, y, �� ���� ���ڰ� �ִ°�?
BOOL IsEnemyMine(SI32 x, SI32 y, SI32 playernumber)
{

	if(Map.GetMineMap(x, y) >= 0 && IsFriendToByPlayerNumber(Map.GetMineMap(x, y), playernumber) == FALSE)return TRUE;

	return FALSE;
}

//  ���� ������ ó���ϴ� ��ƾ 
BOOL MineExplode(SHORT minepnumber, SHORT myid,SHORT x, SHORT y, BOOL validswitch)
{

	if(Map.IsInMapArea(x, y)==FALSE)return FALSE;

	SHORT i, j;
	SHORT id;
	SHORT bulletid=0;
    SHORT centerx=MapCenterX[x][y];
	SHORT centery=MapCenterY[x][y]; 
	SI32 apower = 0;
	SI32 childbombpower = 0; 
	_NewID newid;//enemyid;

	newid.ID = myid;

	
	
	// ū ������ ����Ų��. 
	SHORT smokeid=FindEmptySmoke();
	if(smokeid)
	{
	    SetSmoke(KIND_SMOKE_BUILDINGEXP, smokeid, MapCenterX[x][y],MapCenterY[x][y], AREA_LAND);
	}

	// +-2ĭ �������� ������ Ÿ���� ������. 
	
	// ����� ������, 
//	if(validswitch == TRUE)
//	{

//	}
	// ���Ĵ��� ���, 
//	else apower = 2;
	
	childbombpower = 50;
	
	/*
	for(i=-2;i<=2;i++)
	   for(j=-2;j<=2;j++)
	   {
	      if(Map.IsInMapArea(x+j, y+i)==TRUE && Map.GetMineMap(x,y) >= 0 )
		  {

			 id=Map.GetIDMap(x+j, y+i);
			 _NewID nid; 
			 nid = clifChar.GetID(id);
			 if(id > 0 )
			 {
				 enemyid.ID = id;
				 CalculateHit(newid, minepnumber, ATTACKATB_CANNON, 2, AREA_LAND, apower, enemyid, x+j, y+i, 100);
			 }
		  }
	   }
		*/

	   // ����ź�� �ѷ� �α��� ���� �����Ѵ�. (����� ���� ��쿡��.)
	
	   if(validswitch == TRUE)
	   {
		   for(i=-3;i<=3;i++)
			   for(j=-3;j<=3;j++)
			   {
				   if(Map.IsInMapArea(x+j, y+i)==TRUE)
				   {
					   id=Map.GetIDMap(x+j, y+i);
					   // ���� �߰��ϸ�, 
					   if(IsSee(id) && IsFriendToByPlayerNumber(minepnumber, clifChar.GetPlayerNumber(id))==FALSE)
					   {
						   bulletid=FindEmptyBullet();
						   if(id > 0)
								int b = 0;
						   if(bulletid)
						   {
//							   _NewID nid; 
//								nid = clifChar.GetID(Map.GetMineCharMap(x,y));
							   
//							   SetBullet(KIND_BULLET_SHIPCANNON, bulletid, x, y, clifChar.GetX(id), clifChar.GetY(id), centerx, centery, 
//								   clifChar.GetCenterX(id), clifChar.GetCenterY(id), nid, clifChar.GetID(id), childbombpower , 100, 0, AREA_LAND);
						   }
					   }
				   }
			   }
	   }
	
	
	   return TRUE;
}

