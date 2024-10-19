#include <GSL.h>

#include <main.h>
#include <clGame.h>

#include "maindata.h"
#include <etc.h>
#include "calcattack.h"
#include "music.h"
#include "..\Gersang\Directx.h"
#include "mine.h"
#include <OnlineWorld.h>

#include <myfile.h>
#include <MapInterface.h>
#include <map.h>
#include <CharOrg.h>
#include <smoke.h>

#include "..\EnumData\Enum_AttackAtb.h"		// AttackAtb

// 비가오는 시간 간격의 최소 단위 
#define RAIN_INTERVAL_MIN_TIME  18000
#define RAIN_INTERVAL_MAX_TIME  23000

extern  _clGame* pGame;

extern	_MainData			GD;									// 게임의 주데이터 
extern	SHORT 				MapEditMode;						// 지도 편집 모드인가?
extern	_Map 				Map;
extern	int 				MapCenterX[][MAX_MAP_YSIZE];		// 지도의 각 좌표마다의 중심값 
extern	int 				MapCenterY[][MAX_MAP_YSIZE];
extern	_NewID 				ZeroID;
extern	cltMapInterface		clifMap;							// 맵관련 인터페이스 
extern	_Char				CR[];								// 캐릭터의 포인터 
extern	RECT				ScreenRect;							// 지도가 표시되는 영역 


void _Rain::Init()
{
	Step=0;
	RainSwitch=FALSE;
	AutoRainSwitch=TRUE;
	RainDelay=0;
	WaterLevel=0;

	RainFrame=0;

	ThunderSwitch=FALSE;
	ThunderStep=0;

}

void _Rain::SetRainSwitch(BOOL mode, SHORT delay)
{
	RainSwitch=mode;
	Step=0;

	if(mode==TRUE)
	{
		if(delay)
		   RainDelay=delay;
		else 
		   RainDelay=2000;

		RainFrame=GD.Frame;

		PushEffect(EFFECT_RAIN, 0, 0);

	}
}


void _Rain::Action()
{
	SI32 i, j;
	SHORT diffframe;
	SI32 id;

	SI32 WindDir[]= {SOUTH, SOUTH|WEST, WEST, NORTH|WEST,  NORTH, NORTH|EAST, EAST, SOUTH|EAST};


	// 지도 편집 모드가 설정되어 있으면 비를 내리게 하지 않는다. 
	if(MapEditMode==TRUE)return ;



	// 비가 오는 시간을 줄여간다. 
	if(RainDelay)
	{
	
		diffframe=abs(GD.Frame-RainFrame);

		// 번개를 보여준다. 
		if(GD.Random()%200==0)
		{
			if(ThunderSwitch==FALSE)
			{

				MakeThunder(ZeroID, -1, ZeroID, GD.Random()%Map.MH.MapXsize, GD.Random()%Map.MH.MapYsize, 1000);

			}
		}
    
		RainDelay--;
		
		// 수위를 높인다.
		if(GD.Frame % 100 == 0)
		{
			if( WaterLevel < ( MAX_WATER_LEVEL_STEP-1 )  )
			{
			   WaterLevel++;

			   // 최고 수위가 되면, 
			   if(WaterLevel == ( MAX_WATER_LEVEL_STEP - 1 ))
			   {
				   if( Map.IsValidShallowMap() )
				   {
					   // 얕은 물 영역을 모두 깊은 물로 바꾼다. 
					   for(i=0;i<Map.MH.MapYsize;i++)
						   for(j=0;j<Map.MH.MapXsize;j++)
						   {
							   if( Map.GetShallowMap(j,i) )
							   {
								   Map.DeleteCollMap(j, i, BLOCK_SHALLOWWATER);
								   Map.SetCollMap(j, i,    BLOCK_DEEPWATER);

								   // 만약 지뢰를 이곳에 심었다면  모두 폭파 시킨다. 
								   if(IsMine(j, i))
								   {
									   DetectEnemyMine(j, i);
								   }

							   }


						   }
				   }
			   }
			}

		}

		// 비가 다오면 , 
		if(RainDelay==0)
		{
		     SetRainSwitch(FALSE);
		}
	}

	// 번개를 처리한다. 
	if(ThunderSwitch==TRUE)
	{
		// 밝게 만든다. 
		if(ThunderStep==0)
		{
			// 번개가 치는 소리를 낸다. 
			PushEffect(EFFECT_THUNDER, 0, 0);
			SetGamePalette(pGame->ThunderPalette);
		}
		else if(ThunderStep==5)
		{
			SetGamePalette(clGrp.Palette);
		}
		else if(ThunderStep==50)
		{
			ThunderSwitch=FALSE;
		}

		ThunderStep++;
	}


	// 비가 오는 중이라면, 
	if(RainSwitch==TRUE)
	{
		
    	if(GD.Frame % 3 == 0)
		{
	       Step++;
	       Step %= MAX_RAIN_STEP;
		}
	}
	// 비가 안오면, 
	else
	{

		// 수위를 낮춘다. 
		if(GD.Frame%100==0)
		{
			if(WaterLevel)
			{
			   WaterLevel--;

			   if(WaterLevel==0)
			   {
				   if( Map.IsValidShallowMap() )
				   {
					   // 얕은 물 영역을 모두 얕은 물로 바꾼다. 
					   for(i=0;i<Map.MH.MapYsize;i++)
						   for(j=0;j<Map.MH.MapXsize;j++)
						   {
							   if( Map.GetShallowMap(j, i) )
							   {
								   Map.SetCollMap(j, i, BLOCK_SHALLOWWATER);
								   Map.DeleteCollMap(j, i, BLOCK_DEEPWATER);
							   }
						   }
				   }
			   }

			}
		}

		// 비오는 시간 사이의 최소 간격보다 크고 
		if(abs(GD.Frame-RainFrame)>RAIN_INTERVAL_MIN_TIME)
		{
		
			// 자연적으로 비가 오는 설정이면, 
			if(AutoRainSwitch==TRUE)
			{

				// 비오는 시간 사이의 간격이 최대 값보다 크면, 
			    if(abs(GD.Frame-RainFrame)>RAIN_INTERVAL_MAX_TIME)
				{
		            SetRainSwitch(TRUE);
				}
			    // 최대값범위 내에 있으면, 
			    else
				{
			       // 비를 오게 만든다. 
		           if(GD.Rand%400==0)
				   {
			         // 비가 오기 시작하는 프레임을 기억한다. 
 			         RainFrame=GD.Frame;
 
		             PushEffect(EFFECT_RAIN, 0, 0);
		             SetRainSwitch(TRUE);
				   }
				}
			} 
		}
	}


	// 불의 행동 
	if(GD.Frame % 60 == 0)
	{

		for( i = 0; i< clifMap.GetYsize();i++)
			for( j = 0;j < clifMap.GetXsize();j++)
			{
				if(Map.GetFireMap(j, i))
				{

					SI32 life = 0;
					
					// 비가 오면 쉽게 꺼진다.
					if(RainSwitch == TRUE)
					{
						life = 2;
					}
					else
					{
						life = 1;
					}
					
					// robypark 2004/8/10 15:5
					// [기존]바닥 불 수명 감소 함수 호출 위치를 데미지 연산 이후에 호출하도록 수정
					//Map.DecreaseFireLifeTime(j, i, life);
		
					// 불이 붙어 있는 지형의 상태를 살핀다. 
					
					// 유닛이 배치되어 있다면 체력을 줄인다. 
					id = clifMap.GetIDMap(j, i);
					
					if(IsAlive(id))
					{
						CR[id].FireOperation(j,i);
					}

					// robypark 2004/8/10 15:5
					// [수정]바닥 불 수명 감소 함수 호출 위치를 데미지 연산 이후에 호출하도록 수정
					Map.DecreaseFireLifeTime(j, i, life);
				}
					
			}
			
	}

}

void _Rain::MakeThunder(_NewID myid, SI32 myplayernumber, _NewID attackid, SHORT x, SHORT y, SHORT apower)
{

    SHORT smokeid = FindEmptySmoke();


	if(smokeid)
	{
	    // 번개  
        SetSmoke(KIND_SMOKE_LIGHTNING, smokeid, MapCenterX[x][y], MapCenterY[x][y], AREA_AIR);
	}

	CalculateHit(myid, myplayernumber, ATTACKATB_LIGHTNING, 1, AREA_LAND, apower, attackid, x, y, 100);
	
	ThunderSwitch=TRUE;
	ThunderStep=0;
	
}


void _Rain::Draw()
{
	SHORT i, j;
	SHORT file, font;


	// 비가 오는 상태가 아니라면 비를 그리지 않는다. 
	if(RainSwitch==FALSE)return ;

	if(Step<(MAX_RAIN_STEP-1))
	{
		file=FILE_RAIN0;
		font=Step;
	}
	else
	{
		file=FILE_RAIN1;
		font=0;
	}

	SHORT xcount=(ScreenRect.right-ScreenRect.left+1)/GetFileXsize(file);	// hojae_repair
	if((ScreenRect.right-ScreenRect.left+1)%GetFileXsize(file))xcount++;	// hojae_repair

	SHORT ycount=(ScreenRect.bottom-ScreenRect.top+1)/GetFileYsize(file);	// hojae_repair
	if((ScreenRect.bottom-ScreenRect.top+1)%GetFileYsize(file))ycount++;	// hojae_repair


		for(i=0;i<ycount;i++)
			for(j=0;j<xcount;j++)
			{
				Draw_PutSpriteT(j*GetFileXsize(file), i*GetFileYsize(file), file, font);	// hojae_append
			}


}

