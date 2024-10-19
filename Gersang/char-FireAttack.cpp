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

BOOL CanFireAttackGlobal(_NewID id)
{
	
	// 죽은 캐릭터는 지진 공격할 수 없다.
	if(IsSee(id.ID)==FALSE)return FALSE;

	// 비행 캐릭터는 지진 공격할 수 없다. 
	if(CR[id.ID].IsAtb(ATB_FLY))return FALSE;

	return TRUE;
}

BOOL _Char::CanFireAttack()
{

	if(IsAtb4(ATB4_FIREATTACK) == FALSE)return FALSE;

	// 마법력이 남아 있어야 한다. 
	if(m_ParmeterBox.IP.GetMana() < MAGICPOWER_FIREATTACK)	return FALSE;

	return TRUE;
}

BOOL _Char::CanFireAttack(_NewID id)
{

	if(CanFireAttackGlobal(id)==FALSE)return FALSE;


	if(CanFireAttack()==FALSE)return FALSE;


	return TRUE;
}

BOOL _Char::SetFireAttack(_NewID id)
{

	if(CanFireAttack(id)==FALSE)return FALSE;

	WorkStep=WS_DECIDE_LENGTH_TO_DEST;

	return TRUE;
}

BOOL _Char::FireAttackOperation(_NewID id)
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
	     if(CanFireAttack(id)==FALSE)
		 {    
		     m_scBusySwitch=FALSE;
	         OrderStop(m_NewID, BY_COMPUTER);
	         return FALSE;
		 }

		 // 적에게 타격을 준다. 
		 SHORT level=m_ParmeterBox.IP.GetLevel();

		 SI32 apower = CalcDamage(SPECIAL_FIREATTACK, m_ParmeterBox.CalcApower(), KI[m_cltCharStatus.GetKind()].m_siKindAtb, m_scMagicApplyRate); 

		 CalculateHit(m_NewID, GetPlayerNumber(), ATTACKATB_FIREATTACK, 3, AREA_LAND, 
			 apower, id, CR[id.ID].m_ssX, CR[id.ID].m_ssY, 
			 m_ParmeterBox.GetHitRate(0));


	     // 마법력을 줄여준다. 
		 ReduceMana(MAGICPOWER_FIREATTACK);

		 smokeid=FindEmptySmoke();
		 if(smokeid)
		 {
//			 PushEffect(EFFECT_ATTACK_GENERAL_J4_EXT, clifChar.GetX(id.ID), clifChar.GetY(id.ID));
			 SetSmoke(KIND_SMOKE_FIREATTACK, smokeid, CR[id.ID].m_cltCharStatus.GetCenterX(), CR[id.ID].m_cltCharStatus.GetCenterY(), AREA_LAND);
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



BOOL _Char::FireAttack()
{
	SHORT status;
	SHORT length=0;
	_NewID targetid=Para2;
	SHORT healrange = GetAttackRange() + 2;
	
   switch(WorkStep)
   {
	  case WS_DECIDE_LENGTH_TO_DEST:    // 목표까지의 거리를 결정한다. 

		 // 지진을 일으킬 수 없다면, 중단한다.
		 if(CanFireAttack(targetid)==FALSE)
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
			   if(CanFireAttack(targetid)==FALSE)
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
		 FireAttackOperation(targetid);
	     break;
	}
	
	return FALSE;
}














