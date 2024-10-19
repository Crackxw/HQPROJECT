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

extern	_MainData			GD;									// ������ �ֵ����� 
extern	_Map 				Map;
extern	_Char				CR[];								// ĳ������ ������ 
extern	int 				MapCenterX[][MAX_MAP_YSIZE];		// ������ �� ��ǥ������ �߽ɰ� 
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
	case WS_DECIDE_LENGTH_TO_DEST:    // ��ǥ������ �Ÿ��� �����Ѵ�. 
		WorkStep=WS_GOTO_TARGET;
		// ��������  �̵��Ѵ�. 
		SetMove(targetx, targety);
		
		break;
		
	case WS_GOTO_TARGET:
		
		status=Move(BASIC_MOVE_BLOCK_COUNT);
		
		// �������� �����ϸ�, 
		if(status==1)
		{
			// ��ǻ�� �÷��̾��� ��쿡 Ÿ�������� �ƹ��͵� ���� ��쿡�� 
			// ������ ��ҵȴ�. 
			if(GD.Player[GetPlayerNumber()].PI.Host==BY_COMPUTER)
			{
				// ������ �ؾ��ϳ� ���ƾ� �ϳ�...
				SHORT validswitch=FALSE;

				for(i=-2;i<=2;i++)
					for(j=-2;j<=2;j++)
					{
						// �αٿ� ���� �����Ƿ� �����ؾ� �Ѵ�. 
						id=Map.GetIDMap(m_ssX+j, m_ssY+i);
						if(IsSee(id) && IsFriendToByID(m_NewID.ID, id)==FALSE)
						{
							validswitch=TRUE;
						}
					}

					// ������ Ÿ���� �� �� ���ٸ�, 
					if(validswitch==FALSE)
					{
						SHORT enemyid=0;
						SHORT length, min_length=1000;

						// �ֺ��� ���� �ִ��� Ȯ���ؼ� ������ �����Ѵ�. 
						for(i=-5;i<=5;i++)
						   for(j=-5;j<=5;j++)
						   {
							   // �αٿ� ���� �����Ƿ� �����ؾ� �Ѵ�. 
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

						   // ���Ӱ� ������ ���� �ִٸ�, 
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
  
		// ȭ�� ��鸲�� �����. 
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
