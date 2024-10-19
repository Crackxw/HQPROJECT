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

extern	_Char				CR[];								// ĳ������ ������ 
extern	_MainData			GD;									// ������ �ֵ����� 
extern	_KindInfo			KI[];								// ĳ���� ������ ������ 
extern	_Map 				Map;
extern	cltCharInterface	clifChar;							// ĳ���� ���� �������̽� 

BOOL CanFireAttackGlobal(_NewID id)
{
	
	// ���� ĳ���ʹ� ���� ������ �� ����.
	if(IsSee(id.ID)==FALSE)return FALSE;

	// ���� ĳ���ʹ� ���� ������ �� ����. 
	if(CR[id.ID].IsAtb(ATB_FLY))return FALSE;

	return TRUE;
}

BOOL _Char::CanFireAttack()
{

	if(IsAtb4(ATB4_FIREATTACK) == FALSE)return FALSE;

	// �������� ���� �־�� �Ѵ�. 
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
		 // ������ ����ų �� ���ٸ�, 
	     if(CanFireAttack(id)==FALSE)
		 {    
		     m_scBusySwitch=FALSE;
	         OrderStop(m_NewID, BY_COMPUTER);
	         return FALSE;
		 }

		 // ������ Ÿ���� �ش�. 
		 SHORT level=m_ParmeterBox.IP.GetLevel();

		 SI32 apower = CalcDamage(SPECIAL_FIREATTACK, m_ParmeterBox.CalcApower(), KI[m_cltCharStatus.GetKind()].m_siKindAtb, m_scMagicApplyRate); 

		 CalculateHit(m_NewID, GetPlayerNumber(), ATTACKATB_FIREATTACK, 3, AREA_LAND, 
			 apower, id, CR[id.ID].m_ssX, CR[id.ID].m_ssY, 
			 m_ParmeterBox.GetHitRate(0));


	     // �������� �ٿ��ش�. 
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
	  case WS_DECIDE_LENGTH_TO_DEST:    // ��ǥ������ �Ÿ��� �����Ѵ�. 

		 // ������ ����ų �� ���ٸ�, �ߴ��Ѵ�.
		 if(CanFireAttack(targetid)==FALSE)
		 {   
			 m_scBusySwitch=FALSE;
		     OrderStop(m_NewID, BY_COMPUTER);
		     return FALSE;
		 }

		 // �����Ÿ� �ȿ� ������,
		 if(IsInRange(targetid.ID, healrange))
		 { 
			 WorkStep=WS_HEAL;
		 }
		 else // ���� ��Ÿ� �ۿ� ������, 
		 {  
			  WorkStep=WS_GOTO_TARGET;
		      // ���� �ִ� ������ �̵��Ѵ�. 
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

 		       // �������� �����ϰų� �߰� ������ �����ϸ�, 
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
		 // Ÿ���� �������� �����ϸ� ġ���� �ش�. 
		 FireAttackOperation(targetid);
	     break;
	}
	
	return FALSE;
}














