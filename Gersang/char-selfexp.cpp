#include <GSL.h>

#include <main.h>


#include <findpath5.h>
#include "maindata.h"
#include <etc.h>
#include "calcattack.h"
#include "music.h"


#include <map.h>
#include <CharOrg.h>
#include <smoke.h>

#include "..\EnumData\Enum_AttackAtb.h"		// AttackAtb

extern	_MainData			GD;									// 게임의 주데이터 
extern	_Map 				Map;
extern	_Char				CR[];								// 캐릭터의 포인터 
extern	int 				MapCenterX[][MAX_MAP_YSIZE];		// 지도의 각 좌표마다의 중심값 
extern	int 				MapCenterY[][MAX_MAP_YSIZE];
extern	_NewID 				ZeroID;


BOOL _Char::CanSelfExp()
{
	if(SelfExpPercent==100)return TRUE;
	
	return FALSE;
}

BOOL _Char::SetSelfExp(SHORT x, SHORT y)
{
    WorkStep=WS_DECIDE_LENGTH_TO_DEST;

	return TRUE;
}


BOOL _Char::SelfExp()
{
	SHORT id;
	SHORT i, j;
	SHORT status;
	//	_NewID targetid=Para2;
	SHORT healrange=5;
	SHORT targetx=LOWORD(Para1);
	SHORT targety=HIWORD(Para1);
	
	SHORT smokeid;
	
	if( IsAtb(ATB_SELFEXP)==FALSE ) return FALSE;
	
	switch(WorkStep)
	{
	case WS_DECIDE_LENGTH_TO_DEST:    // 목표까지의 거리를 결정한다. 
		WorkStep=WS_GOTO_TARGET;
		// 목적지로  이동한다. 
		SetMove(targetx, targety);
		
		break;
		
	case WS_GOTO_TARGET:
		
		status=Move(BASIC_MOVE_BLOCK_COUNT);
		
		// 목적지에 도착하면, 
		if(status==1)
		{
			// 컴퓨터 플레이어인 경우에 타격지점에 아무것도 없는 경우에는 
			// 공격이 취소된다. 
			if(GD.Player[GetPlayerNumber()].PI.Host==BY_COMPUTER)
			{
				// 공격을 해야하나 말아야 하나...
				SHORT validswitch=FALSE;

				for(i=-2;i<=2;i++)
					for(j=-2;j<=2;j++)
					{
						// 인근에 적이 있으므로 공격해야 한다. 
						id=Map.GetIDMap(m_ssX+j, m_ssY+i);
						if(IsSee(id) && IsFriendToByID(m_NewID.ID, id)==FALSE)
						{
							validswitch=TRUE;
						}
					}

					// 적에게 타격을 줄 수 없다면, 
					if(validswitch==FALSE)
					{
						SHORT enemyid=0;
						SHORT length, min_length=1000;

						// 주변에 적이 있는지 확인해서 있으면 공격한다. 
						for(i=-5;i<=5;i++)
						   for(j=-5;j<=5;j++)
						   {
							   // 인근에 적이 있으므로 공격해야 한다. 
						       id=Map.GetIDMap(m_ssX+j, m_ssY+i);
						       if(IsSee(id) && IsFriendToByID(m_NewID.ID, id)==FALSE)
							   {
							       
								   length=max(abs(CR[id].m_ssX-m_ssX), abs(CR[id].m_ssY-m_ssY));
								   if(length<min_length)
								   {
									   min_length=length;
									   enemyid=id;
								   }
							   }
						   }

						   // 새롭게 설정된 적이 있다면, 
						   if(enemyid)
						   {
							   OrderSelfExp(m_NewID, CR[enemyid].m_ssX, CR[enemyid].m_ssY, BY_COMPUTER);
							   return FALSE;
						   }



						OrderStop(m_NewID, BY_COMPUTER);
						return FALSE;
					}

			}

			WorkStep=WS_SELFEXP;
		}
		break;

	case WS_SELFEXP:
  
		// 화면 흔들림을 만든다. 
		GD.EarthQuake.SetEarthQuake();

		PushEffect(EFFECT_EXP, m_ssX, m_ssY);

	   smokeid=FindEmptySmoke();
	   if(smokeid)
	   {
		   SetSmoke(KIND_SMOKE_BIGEXP, smokeid, MapCenterX[m_ssX][m_ssY],MapCenterY[m_ssX][m_ssY], AREA_LAND);
	   }

		CalculateHit(m_NewID, GetPlayerNumber(), ATTACKATB_SELFEXP, 2, AREA_LAND, m_ParmeterBox.CalcApower(), ZeroID, m_ssX, m_ssY, m_ParmeterBox.GetHitRate(0));


		return TRUE;
	     break;
	}
	
	return FALSE;
}
