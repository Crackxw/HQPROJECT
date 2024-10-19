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
#include <charinterface.h>
#include <CharOrg.h>

#include "..\EnumData\Enum_AttackAtb.h"		// AttackAtb

extern	_Char				CR[];								// 캐릭터의 포인터 
extern	_MainData			GD;									// 게임의 주데이터 
extern	_KindInfo			KI[];								// 캐릭터 종류별 데이터 
extern	_Map 				Map;
extern	cltCharInterface	clifChar;							// 캐릭터 관련 인터페이스 


// id를 지진으로 공격할 수 있는가?
// id자체의 정보만을 가지고 확인한다. 
BOOL CanEarthQuakeGlobal(_NewID id)
{
	
	// 죽은 캐릭터는 지진 공격할 수 없다.
	if(IsSee(id.ID)==FALSE)return FALSE;

	// 비행 캐릭터는 지진 공격할 수 없다. 
	if(CR[id.ID].IsAtb(ATB_FLY))return FALSE;

	return TRUE;
}


// 자신이 지진공격이 가능한가?
BOOL _Char::CanEarthQuake()
{

	if(IsAtb3(ATB3_EARTHQUAKE) == FALSE)return FALSE;

	// 마법력이 남아 있어야 한다. 
	if(m_ParmeterBox.IP.GetMana() < MAGICPOWER_EARTHQUAKE)	return FALSE;

	return TRUE;
}

BOOL _Char::CanEarthQuake(_NewID id)
{

	if(CanEarthQuakeGlobal(id)==FALSE)return FALSE;


	if(CanEarthQuake()==FALSE)return FALSE;


	return TRUE;
}

BOOL _Char::SetEarthQuake(_NewID id)
{

	if(CanEarthQuake(id)==FALSE)return FALSE;

	WorkStep=WS_DECIDE_LENGTH_TO_DEST;

	return TRUE;
}

BOOL _Char::EarthQuakeOperation(_NewID id)
{

	SHORT smokeid;
	SI32 index;

	if(MagicCharAnimationIndex)
	{
		index = MagicCharAnimationIndex;
		m_cltCharDrawInfo.SetAnimation(ANI_MAGIC);
	}
	else
	{
		index = AttackCharAnimationIndex;
		m_cltCharDrawInfo.SetAnimation(ANI_ATTACK);
	}

	m_cltCharStatus.SetWorkDelayor(m_cltCharStatus.GetWorkDelayor() + 1);
	if(m_cltCharStatus.GetWorkDelayor() < KI[m_cltCharStatus.GetKind()].m_clCharAnimation[index].siDelay){ return FALSE;}
	m_cltCharStatus.SetWorkDelayor(0);


	if(KI[m_cltCharStatus.GetKind()].m_clCharAnimation[index].GetFrameNumber() == 0)
	{
		Step=0;
		m_cltCharDrawInfo.SetAniStep(Step);
	}
	else
	{
	   ++Step %= KI[m_cltCharStatus.GetKind()].m_clCharAnimation[index].GetFrameNumber();
	   m_cltCharDrawInfo.SetAniStep(Step);
	   m_cltCharDrawInfo.SetUpdateAnimationSwitch(TRUE);
	}

	
	 if(Step == KI[m_cltCharStatus.GetKind()].m_clCharAnimation[index].siInstant)
	 {
		 // 지진을 일으킬 수 없다면, 
	     if(CanEarthQuake(id)==FALSE)
		 {    
		     m_scBusySwitch=FALSE;
	         OrderStop(m_NewID, BY_COMPUTER);
	         return FALSE;
		 }

		 // 적에게 타격을 준다. 
		 SHORT level=m_ParmeterBox.IP.GetLevel();

		 SI32 apower = CalcDamage(SPECIAL_EARTHQUAKE, m_ParmeterBox.CalcApower(), m_scMagicApplyRate, KI[m_cltCharStatus.GetKind()].m_siKindAtb); 

		 CalculateHit(m_NewID, GetPlayerNumber(), ATTACKATB_EARTHQUAKE, 3, AREA_LAND, 
			 apower, id, CR[id.ID].m_ssX, CR[id.ID].m_ssY, 
			 m_ParmeterBox.GetHitRate(0));


	     // 마법력을 줄여준다. 
		 ReduceMana(MAGICPOWER_EARTHQUAKE);

		 smokeid=FindEmptySmoke();
		 if(smokeid)
		 {
			 PushEffect(EFFECT_ATTACK_GENERAL_J4_EXT, clifChar.GetX(id.ID), clifChar.GetY(id.ID));

			 // 화면 흔들림을 만든다. 
		     GD.EarthQuake.SetEarthQuake();

			 SetSmoke(KIND_SMOKE_EARTHQUAKE, smokeid, CR[id.ID].m_cltCharStatus.GetCenterX(), CR[id.ID].m_cltCharStatus.GetCenterY(), AREA_LAND);
		 }
	     OrderStop(m_NewID, BY_COMPUTER);
	 }

	 if(Step==0)
	 {
	   m_scBusySwitch=FALSE;
	   return TRUE;
	 }
	 else return FALSE;

}

BOOL _Char::EarthQuake()
{
	SHORT status;
	SHORT length=0;
	_NewID targetid=Para2;
	SHORT healrange = GetAttackRange() + 2;
	
   switch(WorkStep)
   {
	  case WS_DECIDE_LENGTH_TO_DEST:    // 목표까지의 거리를 결정한다. 

		 // 지진을 일으킬 수 없다면, 중단한다.
		 if(CanEarthQuake(targetid)==FALSE)
		 {   
			 m_scBusySwitch=FALSE;
		     OrderStop(m_NewID, BY_COMPUTER);
		     return FALSE;
		 }

		 // 사정거리 안에 있으면,
		 if(IsInRange(targetid.ID, healrange))
		 { 
			 WorkStep=WS_HEAL;
		 }
		 else // 적이 사거리 밖에 있으면, 
		 {  
			  WorkStep=WS_GOTO_TARGET;
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
			   if(CanEarthQuake(targetid)==FALSE)
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
		 // 타겟의 에너지가 부족하면 치료해 준다. 
		 EarthQuakeOperation(targetid);
	     break;
	}
	
	return FALSE;
}



