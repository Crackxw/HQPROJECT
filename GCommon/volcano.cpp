//--------------------------------------------------------------------
//  담당 : 김태곤
//--------------------------------------------------------------------
#include <GSL.h>

#include <main.h>

#include "..\Gersang\maindata.h"
#include <etc.h>
#include <map.h>
#include <bullet.h>
#include <volcano.h>


extern	_MainData			GD;									// 게임의 주데이터 
extern	_Map 				Map;
extern	int 				MapCenterX[][MAX_MAP_YSIZE];		// 지도의 각 좌표마다의 중심값 
extern	int 				MapCenterY[][MAX_MAP_YSIZE];
extern	_NewID 				ZeroID;


BOOL _Volcano::Action()
{
	SI32 id = 0;
	SI32 firex, firey;
	SI32 attackx, attacky;
	SI32 vary;
	SI32 FireBulletRandomValue ;
	
	if(GD.Frame == (UI32)FireStartFrame) 
	{
		FireSwitch = TRUE;
		FireStartFrame +=FireInterval;
		FireDelay     = 700;
	}
	
	if(FireDelay )
	{
		FireDelay --;
		
		if(FireDelay > 300)FireBulletRandomValue = 30;
		else FireBulletRandomValue = 7;
		
		if(FireDelay == 0)
			FireSwitch = FALSE;
	}
	

	if( FireSwitch == TRUE )
	{
		if(GD.Random() % FireBulletRandomValue == 0)
		{
			FireBulletSwitch = TRUE;
		}

		if(FireBulletSwitch == TRUE)
		{
			FireBulletSwitch = FALSE;
			
			firex = siX-3;
			firey = siY-3;
			
			vary = 3 + GD.Random()% 5;
			if(GD.Random()%2) vary *= (-1);
			
			attackx = firex + vary;
			
			vary = 3 + GD.Random()% 5;
			if(GD.Random()%2)vary *=(-1);
			
			attacky = firey + vary;


			if(Map.IsInMapArea(attackx, attacky)==TRUE)
			{
				id = FindEmptyBullet();
				if(id)
				{
					SetBullet(KIND_BULLET_VOLCANO, id, firex, firey, attackx, attacky, 
						MapCenterX[firex][firey], MapCenterY[firex][firey], 
						MapCenterX[attackx][attacky], MapCenterY[attackx][attacky], ZeroID, ZeroID, 500, 100, 0, AREA_LAND);
				}
			}
		}
	}


	return TRUE;
}

_Volcano Volcano[MAX_VOLCANO_NUMBER];

void InitVolcano()
{
	SI32 i;
	for(i = 0;i<MAX_VOLCANO_NUMBER;i++)
	{
		Volcano[i].Init();
	}
}


SI32 FindEmptyVolcano()
{
	SI32 i; 

	for(i = 1;i < MAX_VOLCANO_NUMBER; i++)
	{
		if(Volcano[i].bValidSwitch == FALSE)
		{
			return i;
		}
	}
	return 0;
}

BOOL SetVolcano(SI32 x, SI32 y)
{
	SI32 id;

	id = FindEmptyVolcano();
	if(id)
	{
		Volcano[id].bValidSwitch = TRUE;
		Volcano[id].siX = x;
		Volcano[id].siY = y;

		Volcano[id].FireStartFrame = GD.Random() % 1000;
		Volcano[id].FireInterval   = 1000 + GD.Random() % 500;

		return TRUE;
	}
	return FALSE;

}

void VolcanoAction()
{
	SI32 i;

	for(i = 0;i<MAX_VOLCANO_NUMBER;i++)
	{
		if(Volcano[i].bValidSwitch == TRUE)
		{
			Volcano[i].Action();
		}
	}
}


void VolcanoBlackCheck()
{
	SI32 i;

	for(i =0;i < MAX_VOLCANO_NUMBER;i++)
	{
		if(Volcano[i].bValidSwitch == TRUE)
		{
			GlobalBlackCheck(Volcano[i].siX-2, Volcano[i].siY-2, 6, TRUE);
		}
	}
}


void SaveVolcano(FILE* fp)
{
	fwrite(Volcano,		sizeof(_Volcano), MAX_VOLCANO_NUMBER, fp);
}

void LoadVolcano(FILE* fp)
{
	fread(Volcano,		sizeof(_Volcano), MAX_VOLCANO_NUMBER, fp);
}