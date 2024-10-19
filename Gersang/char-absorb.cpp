#include <GSL.h>
#include <main.h>


#include <findpath5.h>
#include "maindata.h"
#include <etc.h>
#include "music.h"
#include "calcattack.h"
#include <CharOrg.h>
#include <Magic.h>



extern	_MainData			GD;									// ������ �ֵ����� 
extern	_Char				CR[];								// ĳ������ ������ 
extern	_KindInfo			KI[];								// ĳ���� ������ ������ 


///////////////////////////////////////////////////////////////////////
//  ��� ���� �Լ� 
BOOL _Char::SetAbsorb(_NewID id)
{

	if(CanAbsorb(id)==FALSE)return FALSE;

	WorkStep=WS_DECIDE_LENGTH_TO_DEST;
    AttackGiveUpDelayor=0;

	return TRUE;
}

// id�� ü���� ����� �� �ִ°�?
BOOL _Char::CanAbsorb(_NewID id)
{

	if(IsAtb3(ATB3_ABSORBLIFE) == FALSE) return FALSE;

	// ����� ��밡 ��ȿ�ؾ� �Ѵ�. 
	if(IsSee(id.ID)==FALSE)return FALSE;

	// �������� ����ؾ� �Ѵ�.
	if(m_ParmeterBox.IP.GetMana() < MAGICPOWER_ABSORB)return FALSE;

	// ��� ĳ���͸� ����� �� �ִ�. 
	if(CR[id.ID].IsAtb(ATB_PATIENT)==FALSE)return FALSE;
	
	// �����Ը� ����� �� �ִ�. 
	if(IsFriendToByPlayerNumber(GetPlayerNumber(), CR[id.ID].GetPlayerNumber()) == TRUE)return FALSE;

	return TRUE;
}


BOOL _Char::Absorb()
{
	SHORT status;
	//SHORT length=0;

	// ��ǥ�� ��� ĳ����  
	_NewID targetid=Para2;

   switch(WorkStep)
   {
	  case WS_DECIDE_LENGTH_TO_DEST:    // ��ǥ������ �Ÿ��� �����Ѵ�. 

		 // ���� �������� ����� �� ���� ���¶��,  �ߴ��Ѵ�.
		 if(CanAbsorb(targetid)==FALSE)
		 {   
			 m_scBusySwitch=FALSE;
		     OrderStop(m_NewID, BY_COMPUTER);
		     return FALSE;
		 }

		 // ���� �����Ÿ� �ȿ� ������,
		 if(IsInRange(targetid.ID, GetAttackRange()) )
		 { 
			 WorkStep=WS_CAPTURE;
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
			   // ���� �������� ����� �� ���� ó�����, 
			   if(CanAbsorb(targetid)==FALSE)
			   {  
				   OrderStop(m_NewID, BY_COMPUTER);
				   return FALSE;
			   }
			   if(IsInRange(AttackID.ID, GetAttackRange()))
			   {
				  WorkStep=WS_CAPTURE;
			   }
		   }

		   // �������� �����ϰų� �߰� ������ �����ϸ�, 
		   if(status>0)
		   {
			   WorkStep=WS_DECIDE_LENGTH_TO_DEST;
				   
			   AttackGiveUpDelayor++;
		       if(AttackGiveUpDelayor>AttackGiveUpDelay)
				  OrderStop(m_NewID, BY_COMPUTER);
		   }
		break;

	case WS_CAPTURE:

		AbsorbOperation(targetid);
		 

		 // �ڽ��� ���ش�. 
		 return TRUE;

	   break;
	}

	return FALSE;
}

BOOL _Char::AbsorbOperation(_NewID id)
{

	m_cltCharDrawInfo.SetAnimation(ANI_ATTACK);

	m_cltCharStatus.SetWorkDelayor(m_cltCharStatus.GetWorkDelayor() + 1);
	if(m_cltCharStatus.GetWorkDelayor() < KI[m_cltCharStatus.GetKind()].m_clCharAnimation[AttackCharAnimationIndex].siDelay){ return FALSE;}
	m_cltCharStatus.SetWorkDelayor(0);


	if(KI[m_cltCharStatus.GetKind()].m_clCharAnimation[AttackCharAnimationIndex].GetFrameNumber() == 0)
	{
		Step=0;
		m_cltCharDrawInfo.SetAniStep(Step);
	}
	else
	{
	   ++Step%=KI[m_cltCharStatus.GetKind()].m_clCharAnimation[AttackCharAnimationIndex].GetFrameNumber();
	   m_cltCharDrawInfo.SetAniStep(Step);
	   m_cltCharDrawInfo.SetUpdateAnimationSwitch(TRUE);
	}
	
	 if(Step == KI[m_cltCharStatus.GetKind()].m_clCharAnimation[AttackCharAnimationIndex].siInstant)
	 {
		 if(CanAbsorb(id)==FALSE)
		 {   
			 m_scBusySwitch=FALSE;
		     OrderStop(m_NewID, BY_COMPUTER);
		     return FALSE;
		 }

		 // �������� �ٿ��ش�. 
		 if( m_ParmeterBox.IP.GetMana() > MAGICPOWER_ABSORB )
		 {
			 // �������� �ٿ��ش�. 
			 ReduceMana(MAGICPOWER_ABSORB);
		 }
		 else
		 {
			 OrderStop(m_NewID, BY_COMPUTER);
			 return FALSE;
		 }

		 // ������ ü���� ��ģ��.(ü���� 1/10)
		 SHORT absorbhealth = CalcDamage(SPECIAL_ABSORBLIFE, CR[id.ID].m_ParmeterBox.IP.GetLife(), m_scMagicApplyRate);
		 CR[id.ID].ReduceLife(absorbhealth, 70);
		 IncreaseLife(absorbhealth);
		 
		 PushEffect(EFFECT_HEAL, m_ssX, m_ssY);
	     CR[id.ID].SetHealDraw(TRUE);


		 OrderStop(m_NewID,BY_COMPUTER);
	 }

	 if(Step==0)
	 {
	   m_scBusySwitch=FALSE;
	   return TRUE;
	 }
	 else return FALSE;
}
