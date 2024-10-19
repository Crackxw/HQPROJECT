#include <GSL.h>

#include <main.h>

#include <findpath5.h>
#include "maindata.h"
#include <etc.h>
#include "calcattack.h"
#include "music.h"

#include <Magic.h>

#include <map.h>
#include <smoke.h>
#include <CharOrg.h>
#include <charinterface.h>


extern	_Char				CR[];								// 캐릭터의 포인터 
extern	_MainData			GD;									// 게임의 주데이터 
extern	_KindInfo			KI[];								// 캐릭터 종류별 데이터 
extern	_Map 				Map;
extern	int 				MapCenterX[][MAX_MAP_YSIZE];		// 지도의 각 좌표마다의 중심값 
extern	int 				MapCenterY[][MAX_MAP_YSIZE];
extern	cltCharInterface	clifChar;


BOOL CanLongSpearGlobal(_NewID id)
{
	// 죽은 캐릭터는 번개 공격할 수 없다.
	if(IsSee(id) == FALSE)return FALSE;

	// 비행체도 번개 공격할 수 없다.
	if(CR[id.ID].IsAtb(ATB_FLY))return FALSE;

	return TRUE;
}

BOOL _Char::CanLongSpear()
{
	if(m_ParmeterBox.IP.GetMana() < MAGICPOWER_LONG_SPEAR)return FALSE;

	return TRUE;
}

BOOL _Char::CanLongSpear(_NewID id)
{
	if(CanLongSpearGlobal(id)==FALSE)return FALSE;

	if(CanLongSpear()==FALSE)return FALSE;

	return TRUE;
}

BOOL _Char::SetLongSpear(_NewID id)
{
	   
	if(CanLongSpear(id)==FALSE)return FALSE;

	WorkStep=WS_DECIDE_LENGTH_TO_DEST;

	return TRUE;
}

BOOL _Char::LongSpearOperation(_NewID id)
{
	m_cltCharDrawInfo.SetAnimation(ANI_ATTACK);

	m_cltCharStatus.SetWorkDelayor(m_cltCharStatus.GetWorkDelayor() + 1);
	if(m_cltCharStatus.GetWorkDelayor() < KI[m_cltCharStatus.GetKind()].m_clCharAnimation[AttackCharAnimationIndex].siDelay){ return FALSE;}
	m_cltCharStatus.SetWorkDelayor(0);


	if(KI[m_cltCharStatus.GetKind()].m_clCharAnimation[AttackCharAnimationIndex].GetFrameNumber()== 0)
	{
		Step = 0;
		m_cltCharDrawInfo.SetAniStep(Step);
	}
	else
	{
	   ++Step %= KI[m_cltCharStatus.GetKind()].m_clCharAnimation[AttackCharAnimationIndex].GetFrameNumber();
	   m_cltCharDrawInfo.SetAniStep(Step);
	   m_cltCharDrawInfo.SetUpdateAnimationSwitch(TRUE);
	}

	
	if(Step == KI[m_cltCharStatus.GetKind()].m_clCharAnimation[AttackCharAnimationIndex].siInstant)
	{
		// 번개를 일으킬 수 없다면, 
		if(CanLongSpear(id) == FALSE)
		{    
			m_scBusySwitch=FALSE;
			OrderStop(m_NewID, BY_COMPUTER);
			return FALSE;
		}
		
		
		// 마법력을 줄여준다. 
		ReduceMana(MAGICPOWER_LONG_SPEAR);
		
		// 땅바닥에 불을 붙여준다. 
		// Map.PutFire(CR[id.ID].X, CR[id.ID].Y, GD.Random()%2+1, GROUNDFIRE_LIFETIME);
		
		SHORT smokeid=FindEmptySmoke();
		if(smokeid)
		{
			SHORT centerx=MapCenterX[CR[id.ID].m_ssX][CR[id.ID].m_ssY];
			SHORT centery=MapCenterY[CR[id.ID].m_ssX][CR[id.ID].m_ssY];
			
			SetSmoke(KIND_SMOKE_LONG_SPEAR, smokeid, centerx,centery-620, AREA_LAND);
			SetApower(smokeid,(SI16)CalcDamage(SPECIAL_LONG_SPEAR, m_ParmeterBox.CalcApower(), m_scMagicApplyRate, m_ParmeterBox.IP.GetLevel()));
			Smoke_AttackID(smokeid, id);
			Smoke_AttackerID(smokeid, m_NewID);
		}   
		
		OrderStop(m_NewID, BY_COMPUTER);
	}
	
	if(Step == 0)
	{
		m_scBusySwitch=FALSE;
		return TRUE;
	}
	else return FALSE;
}

BOOL _Char::LongSpear()
{
	SHORT status;
	SHORT length=0;
	_NewID targetid=Para2;
	SHORT healrange = GetAttackRange() + 2;
	
   switch(WorkStep)
   {
	  case WS_DECIDE_LENGTH_TO_DEST:    // 목표까지의 거리를 결정한다. 

		 // 번개를 일으킬 수 없다면, 중단한다.
		 if(CanLongSpear(targetid)==FALSE)
		 {   
			 m_scBusySwitch=FALSE;
		     OrderStop(m_NewID, BY_COMPUTER);
		     return FALSE;
		 }

		 // 사정거리 안에 있으면,
		 if(IsInRange(targetid.ID, healrange) == TRUE )
		 { 
			 WorkStep = WS_HEAL;
		 }
		 else // 적이 사거리 밖에 있으면, 
		 {  
			  WorkStep = WS_GOTO_TARGET;

		      // 적이 있는 곳으로 이동한다. 
			  SetMove(CR[targetid.ID].GetX(), CR[targetid.ID].GetY());
		 }
		break;


	case WS_GOTO_TARGET:

		status=Move(BASIC_MOVE_BLOCK_COUNT);
		if(MoveDone())
			m_scBusySwitch=FALSE;
		else
		{
			AttackGiveUpDelayor=0;
			m_scBusySwitch=TRUE;
		}

		   if(m_scBusySwitch==FALSE)
		   {
			   if(CanLongSpear(targetid)==FALSE)
			   {  
				   OrderStop(m_NewID, BY_COMPUTER);
				   return FALSE;
			   }
			   if(IsInRange(AttackID.ID, healrange))
			   {
				  WorkStep=WS_HEAL;
				  Step=0;
			   }

			   // 목적지에 도착하거나 중간 지점에 도착하면, 
		       if(status>0)
			   {
			      WorkStep=WS_DECIDE_LENGTH_TO_DEST;
				   
			      AttackGiveUpDelayor++;
		          if(AttackGiveUpDelayor>AttackGiveUpDelay)
				     OrderStop(m_NewID, BY_COMPUTER);
			   }

		   }

		break;

	case WS_HEAL:

		 // 번개를 친다.
		 LongSpearOperation(targetid);

	     break;
	}
	
	return FALSE;
}