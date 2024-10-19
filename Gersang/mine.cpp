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
extern	_MainData			GD;									// 게임의 주데이터 
extern	SI32				UserPlayerNumber;					// 사용자 관련 
extern	_Text 				Text;								// 텍스트 
extern	int 				MapCenterX[][MAX_MAP_YSIZE];		// 지도의 각 좌표마다의 중심값 
extern	int 				MapCenterY[][MAX_MAP_YSIZE];
extern	_NewID 				ZeroID;
extern	cltCharInterface	clifChar;							// 캐릭터 관련 인터페이스 


// 적의 지뢰를 제거할 떄의 처리. 
BOOL DetectEnemyMine(SI32 x, SI32 y)
{
	PushEffect(EFFECT_EXP, x, y);

//	MineExplode(Map.GetMineMap(x, y), Map.GetMineCharMap(x,y),x, y, FALSE);

	// 밟은 지뢰는 없앤다. 
	Map.SetMineMap(x, y, -1);
	
	return TRUE;
}


// 적의 지뢰를 밟았을때의 처리. 
BOOL StepEnemyMine(SI32 x, SI32 y, SI32 playernumber)
{
	// 화면 흔들림을 만든다. 
	int a;
	GD.EarthQuake.SetEarthQuake();
	PushEffect(EFFECT_EXP, x, y);

	// 적절한 멘트를 찾아서 보여준다. 
	if(playernumber == UserPlayerNumber)
	{
		SetMessage2(Text.MineExplodeMessage.Get(), playernumber, EFFECT_MINEEXPLODEMESSAGE);
	}
	a = Map.GetIDMap(x,y);
		  
//	MineExplode(Map.GetMineMap(x, y), Map.GetMineCharMap(x,y), x, y, TRUE);

	

	// 밟은 지뢰는 없앤다. 
	Map.SetMineMap(x, y, -1);

	return TRUE;
}


// x, y, 에 지뢰가 있는가?
BOOL IsMine(SI32 x, SI32 y)
{

	if(Map.GetMineMap(x, y) >= 0)return TRUE;

	return FALSE;
}

// x, y, 에 적의 지뢰가 있는가?
BOOL IsEnemyMine(SI32 x, SI32 y, SI32 playernumber)
{

	if(Map.GetMineMap(x, y) >= 0 && IsFriendToByPlayerNumber(Map.GetMineMap(x, y), playernumber) == FALSE)return TRUE;

	return FALSE;
}

//  지뢰 폭발을 처리하는 루틴 
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

	
	
	// 큰 폭발을 일으킨다. 
	SHORT smokeid=FindEmptySmoke();
	if(smokeid)
	{
	    SetSmoke(KIND_SMOKE_BUILDINGEXP, smokeid, MapCenterX[x][y],MapCenterY[x][y], AREA_LAND);
	}

	// +-2칸 범위내의 적에게 타격을 입힌다. 
	
	// 제대로 터지면, 
//	if(validswitch == TRUE)
//	{

//	}
	// 폭파당한 경우, 
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

	   // 자폭탄을 뿌려 인근의 적을 공격한다. (제대로 터진 경우에만.)
	
	   if(validswitch == TRUE)
	   {
		   for(i=-3;i<=3;i++)
			   for(j=-3;j<=3;j++)
			   {
				   if(Map.IsInMapArea(x+j, y+i)==TRUE)
				   {
					   id=Map.GetIDMap(x+j, y+i);
					   // 적을 발견하면, 
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

