#include <GSL.h>
#include <main.h>


#include <findpath5.h>
#include "maindata.h"
#include <etc.h>
#include "music.h"
#include "calcattack.h"
#include <CharOrg.h>
#include <Magic.h>



extern	_MainData			GD;									// 게임의 주데이터 
extern	_Char				CR[];								// 캐릭터의 포인터 
extern	_KindInfo			KI[];								// 캐릭터 종류별 데이터 


///////////////////////////////////////////////////////////////////////
//  흡수 관련 함수 
BOOL _Char::SetAbsorb(_NewID id)
{

	if(CanAbsorb(id)==FALSE)return FALSE;

	WorkStep=WS_DECIDE_LENGTH_TO_DEST;
    AttackGiveUpDelayor=0;

	return TRUE;
}

// id의 체력을 흡수할 수 있는가?
BOOL _Char::CanAbsorb(_NewID id)
{

	if(IsAtb3(ATB3_ABSORBLIFE) == FALSE) return FALSE;

	// 흡수할 상대가 유효해야 한다. 
	if(IsSee(id.ID)==FALSE)return FALSE;

	// 마법력이 충분해야 한다.
	if(m_ParmeterBox.IP.GetMana() < MAGICPOWER_ABSORB)return FALSE;

	// 사람 캐릭터만 흡수할 수 있다. 
	if(CR[id.ID].IsAtb(ATB_PATIENT)==FALSE)return FALSE;
	
	// 적에게만 사용할 수 있다. 
	if(IsFriendToByPlayerNumber(GetPlayerNumber(), CR[id.ID].GetPlayerNumber()) == TRUE)return FALSE;

	return TRUE;
}


BOOL _Char::Absorb()
{
	SHORT status;
	//SHORT length=0;

	// 목표로 삼는 캐릭터  
	_NewID targetid=Para2;

   switch(WorkStep)
   {
	  case WS_DECIDE_LENGTH_TO_DEST:    // 목표까지의 거리를 결정한다. 

		 // 적의 에너지를 흡수할 수 없는 상태라면,  중단한다.
		 if(CanAbsorb(targetid)==FALSE)
		 {   
			 m_scBusySwitch=FALSE;
		     OrderStop(m_NewID, BY_COMPUTER);
		     return FALSE;
		 }

		 // 적이 사정거리 안에 있으면,
		 if(IsInRange(targetid.ID, GetAttackRange()) )
		 { 
			 WorkStep=WS_CAPTURE;
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
			   // 적의 에너지를 흡수할 수 없는 처지라면, 
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

		   // 목적지에 도착하거나 중간 지점에 도착하면, 
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
		 

		 // 자신을 없앤다. 
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

		 // 마법력을 줄여준다. 
		 if( m_ParmeterBox.IP.GetMana() > MAGICPOWER_ABSORB )
		 {
			 // 마법력을 줄여준다. 
			 ReduceMana(MAGICPOWER_ABSORB);
		 }
		 else
		 {
			 OrderStop(m_NewID, BY_COMPUTER);
			 return FALSE;
		 }

		 // 상대방의 체력을 훔친다.(체력의 1/10)
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
