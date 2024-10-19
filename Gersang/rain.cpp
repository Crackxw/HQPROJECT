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

// �񰡿��� �ð� ������ �ּ� ���� 
#define RAIN_INTERVAL_MIN_TIME  18000
#define RAIN_INTERVAL_MAX_TIME  23000

extern  _clGame* pGame;

extern	_MainData			GD;									// ������ �ֵ����� 
extern	SHORT 				MapEditMode;						// ���� ���� ����ΰ�?
extern	_Map 				Map;
extern	int 				MapCenterX[][MAX_MAP_YSIZE];		// ������ �� ��ǥ������ �߽ɰ� 
extern	int 				MapCenterY[][MAX_MAP_YSIZE];
extern	_NewID 				ZeroID;
extern	cltMapInterface		clifMap;							// �ʰ��� �������̽� 
extern	_Char				CR[];								// ĳ������ ������ 
extern	RECT				ScreenRect;							// ������ ǥ�õǴ� ���� 


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


	// ���� ���� ��尡 �����Ǿ� ������ �� ������ ���� �ʴ´�. 
	if(MapEditMode==TRUE)return ;



	// �� ���� �ð��� �ٿ�����. 
	if(RainDelay)
	{
	
		diffframe=abs(GD.Frame-RainFrame);

		// ������ �����ش�. 
		if(GD.Random()%200==0)
		{
			if(ThunderSwitch==FALSE)
			{

				MakeThunder(ZeroID, -1, ZeroID, GD.Random()%Map.MH.MapXsize, GD.Random()%Map.MH.MapYsize, 1000);

			}
		}
    
		RainDelay--;
		
		// ������ ���δ�.
		if(GD.Frame % 100 == 0)
		{
			if( WaterLevel < ( MAX_WATER_LEVEL_STEP-1 )  )
			{
			   WaterLevel++;

			   // �ְ� ������ �Ǹ�, 
			   if(WaterLevel == ( MAX_WATER_LEVEL_STEP - 1 ))
			   {
				   if( Map.IsValidShallowMap() )
				   {
					   // ���� �� ������ ��� ���� ���� �ٲ۴�. 
					   for(i=0;i<Map.MH.MapYsize;i++)
						   for(j=0;j<Map.MH.MapXsize;j++)
						   {
							   if( Map.GetShallowMap(j,i) )
							   {
								   Map.DeleteCollMap(j, i, BLOCK_SHALLOWWATER);
								   Map.SetCollMap(j, i,    BLOCK_DEEPWATER);

								   // ���� ���ڸ� �̰��� �ɾ��ٸ�  ��� ���� ��Ų��. 
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

		// �� �ٿ��� , 
		if(RainDelay==0)
		{
		     SetRainSwitch(FALSE);
		}
	}

	// ������ ó���Ѵ�. 
	if(ThunderSwitch==TRUE)
	{
		// ��� �����. 
		if(ThunderStep==0)
		{
			// ������ ġ�� �Ҹ��� ����. 
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


	// �� ���� ���̶��, 
	if(RainSwitch==TRUE)
	{
		
    	if(GD.Frame % 3 == 0)
		{
	       Step++;
	       Step %= MAX_RAIN_STEP;
		}
	}
	// �� �ȿ���, 
	else
	{

		// ������ �����. 
		if(GD.Frame%100==0)
		{
			if(WaterLevel)
			{
			   WaterLevel--;

			   if(WaterLevel==0)
			   {
				   if( Map.IsValidShallowMap() )
				   {
					   // ���� �� ������ ��� ���� ���� �ٲ۴�. 
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

		// ����� �ð� ������ �ּ� ���ݺ��� ũ�� 
		if(abs(GD.Frame-RainFrame)>RAIN_INTERVAL_MIN_TIME)
		{
		
			// �ڿ������� �� ���� �����̸�, 
			if(AutoRainSwitch==TRUE)
			{

				// ����� �ð� ������ ������ �ִ� ������ ũ��, 
			    if(abs(GD.Frame-RainFrame)>RAIN_INTERVAL_MAX_TIME)
				{
		            SetRainSwitch(TRUE);
				}
			    // �ִ밪���� ���� ������, 
			    else
				{
			       // �� ���� �����. 
		           if(GD.Rand%400==0)
				   {
			         // �� ���� �����ϴ� �������� ����Ѵ�. 
 			         RainFrame=GD.Frame;
 
		             PushEffect(EFFECT_RAIN, 0, 0);
		             SetRainSwitch(TRUE);
				   }
				}
			} 
		}
	}


	// ���� �ൿ 
	if(GD.Frame % 60 == 0)
	{

		for( i = 0; i< clifMap.GetYsize();i++)
			for( j = 0;j < clifMap.GetXsize();j++)
			{
				if(Map.GetFireMap(j, i))
				{

					SI32 life = 0;
					
					// �� ���� ���� ������.
					if(RainSwitch == TRUE)
					{
						life = 2;
					}
					else
					{
						life = 1;
					}
					
					// robypark 2004/8/10 15:5
					// [����]�ٴ� �� ���� ���� �Լ� ȣ�� ��ġ�� ������ ���� ���Ŀ� ȣ���ϵ��� ����
					//Map.DecreaseFireLifeTime(j, i, life);
		
					// ���� �پ� �ִ� ������ ���¸� ���ɴ�. 
					
					// ������ ��ġ�Ǿ� �ִٸ� ü���� ���δ�. 
					id = clifMap.GetIDMap(j, i);
					
					if(IsAlive(id))
					{
						CR[id].FireOperation(j,i);
					}

					// robypark 2004/8/10 15:5
					// [����]�ٴ� �� ���� ���� �Լ� ȣ�� ��ġ�� ������ ���� ���Ŀ� ȣ���ϵ��� ����
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
	    // ����  
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


	// �� ���� ���°� �ƴ϶�� �� �׸��� �ʴ´�. 
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

