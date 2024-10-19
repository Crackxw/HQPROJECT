///////////////////////////////////////////////////////////////////////
//  방어막 관련 함수 

#include <GSL.h>

#include <main.h>


#include <findpath5.h>
#include <etc.h>
#include "music.h"


#include <CharOrg.h>
#include <Magic.h>

extern	_Char				CR[];								// 캐릭터의 포인터 
extern	_KindInfo			KI[];								// 캐릭터 종류별 데이터 


BOOL _Char::SetShield(_NewID id)
{

	if(CanShield(id)==FALSE)return FALSE;

	WorkStep=WS_DECIDE_LENGTH_TO_DEST;
    AttackGiveUpDelayor=0;

	return TRUE;
}

#include "Message.h"

BOOL _Char::CanShield()
{

	if(IsAtb3(ATB3_SHIELDMAGIC)==FALSE)return FALSE;

	// 마법력이 충분해야 한다.
	if(m_ParmeterBox.IP.GetMana() < MAGICPOWER_SHIELD)	return FALSE;

	return TRUE;
}

// id에게 방어막을 줄 수 있는가?
BOOL _Char::CanShield(_NewID id)
{

	if(CanShield()==FALSE)return FALSE;

	// 방어막을 줄 상대가 유효해야 한다. 
	if(IsSee(id)==FALSE)return FALSE;

	// 치료 가능한 유닛만 보호막을 해줄 수 있다.
	if(CR[id.ID].IsAtb(ATB_PATIENT)==FALSE)return FALSE;

	return TRUE;
}


BOOL _Char::Shield()
{
	SHORT status;
	SHORT length=0;

	// 목표로 삼는 캐릭터  
	_NewID targetid=Para2;
	SI32   shield_range = 6;

   switch(WorkStep)
   {
	  case WS_DECIDE_LENGTH_TO_DEST:    // 목표까지의 거리를 결정한다. 

		 // 방어막을 칠수 없는 상태라면,   중단한다.
		 if(CanShield(targetid)==FALSE)
		 {   
			 m_scBusySwitch=FALSE;
		     OrderStop(m_NewID, BY_COMPUTER);
		     return FALSE;
		 }

		 // 사정거리 안에 있으면,
		 if(IsInRange(targetid.ID, shield_range ))
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
			   if(CanShield(targetid)==FALSE)
			   {  
				   OrderStop(m_NewID, BY_COMPUTER);
				   return FALSE;
			   }
			   if(IsInRange(AttackID.ID, shield_range))
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

		ShieldOperation(targetid);
		 

		 // 자신을 없앤다. 
		 return TRUE;

	   break;
	}

	return FALSE;
}

BOOL _Char::ShieldOperation(_NewID id)
{

	m_cltCharDrawInfo.SetAnimation(ANI_ATTACK);

	m_cltCharStatus.SetWorkDelayor(m_cltCharStatus.GetWorkDelayor() + 1);
	if(m_cltCharStatus.GetWorkDelayor() < KI[m_cltCharStatus.GetKind()].m_clCharAnimation[AttackCharAnimationIndex].siDelay){ return FALSE;}
	m_cltCharStatus.SetWorkDelayor(0);


	if(KI[m_cltCharStatus.GetKind()].m_clCharAnimation[AttackCharAnimationIndex].GetFrameNumber()==0)
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
	
	 if(Step==KI[m_cltCharStatus.GetKind()].m_clCharAnimation[AttackCharAnimationIndex].siInstant)
	 {
		 if(CanShield(id)==FALSE)
		 {   
			 m_scBusySwitch=FALSE;
		     OrderStop(m_NewID, BY_COMPUTER);
		     return FALSE;
		 }

		 // 마법력을 줄인다. 
		 m_ParmeterBox.IP.SetMana(m_ParmeterBox.IP.GetMana() - MAGICPOWER_SHIELD);
		 if( m_ParmeterBox.IP.GetMana() < 0 )	m_ParmeterBox.IP.SetMana(0);

		 if(CR[m_NewID.ID].IsGeneral() == TRUE)
			CR[id.ID].ShieldDelay=MAX_SHIELD_DELAY;

		 else if(CR[m_NewID.ID].IsGeneral() == FALSE)
			CR[id.ID].ShieldDelay=MIN_SHIELD_DELAY;

		 PushEffect(EFFECT_HEAL, m_ssX, m_ssY);
	     
		 OrderStop(m_NewID,BY_COMPUTER);
	 }

	 if(Step==0)
	 {
	   m_scBusySwitch=FALSE;
	   return TRUE;
	 }
	 else return FALSE;

}
