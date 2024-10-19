#include <GSL.h>

#include <main.h>

#include <findpath5.h>
#include "maindata.h"
#include <etc.h>
#include "music.h"

#include <Magic.h>
#include <CharOrg.h>
#include <map.h>
#include <MapInterface.h>
#include <CharInterface.h>

extern	_Char				CR[];								// 캐릭터의 포인터 
extern	_MainData			GD;									// 게임의 주데이터 
extern	_Map 				Map;
extern	_KindInfo			KI[];								// 캐릭터 종류별 데이터 
extern	cltMapInterface		clifMap;							// 맵관련 인터페이스 
extern	cltCharInterface	clifChar;

BOOL CanManaGlobal(_NewID id)
{
	// 죽은 캐릭터는 치료할 수 없다. 
	if(IsSee(id.ID)==FALSE)return FALSE;

	// 마나 에너지가 최대 이하여야 한다. 
	if(CR[id.ID].m_ParmeterBox.IP.GetMana() == CR[id.ID].m_ParmeterBox.GetMaxMana())return FALSE;

//	if( (KIND_ON_GENERAL6_C == CR[id.ID].m_cltCharStatus.GetKind()) || (KIND_ON_MAGIC_C == CR[id.ID].m_cltCharStatus.GetKind()))
//		return FALSE;

	// robypark 2004/9/1 13:15
	// 마나 회복술을 가진 유닛의 마나회복을 할 수 없다.
	if (KI[CR[id.ID].m_cltCharStatus.GetKind()].m_uiAtb2 & ATB2_MANA)
		return FALSE;

	// 치료가능 속성을 지니고 있어야 한다. 
	if(CR[id.ID].IsAtb(ATB_PATIENT)==FALSE)return FALSE;

	// 시한부 생명의 유닛이면 치료 불가. 
	if(CR[id.ID].GetTimerDelay())return FALSE;


	if(CR[id.ID].m_cltCharDrawInfo.GetTransparentSwitch() == TRUE)
		return FALSE;

	return TRUE;
}

BOOL _Char::CanMana()
{
	// 치료속성을 가지고 있어야 한다. 
	if(IsAtb2(ATB2_MANA)==FALSE)return FALSE;

	if(m_ParmeterBox.IP.GetMana() == 0) return FALSE; 

	return TRUE;
}


BOOL _Char::CanMana(_NewID id)
{
	if(CanManaGlobal(id)==FALSE)return FALSE;

	if(CanMana()==FALSE)return FALSE;

	// 동맹이 아니면 치료할 수 없다. 
	if(GetPlayerNumber() != CR[id.ID].GetPlayerNumber()) return FALSE;

	return TRUE;
}

BOOL _Char::SetMana(_NewID id)
{

	if(CanMana(id)==FALSE)return FALSE;

	WorkStep=WS_DECIDE_LENGTH_TO_DEST;
 
	return TRUE;
}

BOOL _Char::Mana()
{
	SHORT status;
	SHORT length=0;
	_NewID targetid=Para2;
	SHORT healrange=5;
	
   switch(WorkStep)
   {
	  case WS_DECIDE_LENGTH_TO_DEST:    // 목표까지의 거리를 결정한다. 

		 // 치료할 수 없다면 중단한다.
		 if(CanMana(targetid)==FALSE)
		 {   
			 m_scBusySwitch=FALSE;
		     OrderStop(m_NewID, BY_COMPUTER);
		     return FALSE;
		 }

		 // 사정거리 안에 있으면,
		 if(IsInRange(targetid.ID, healrange))
		 { 
			 Direction = DecideDirection8(m_ssX, m_ssY, CR[targetid.ID].m_ssX, CR[targetid.ID].m_ssY);
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
			if(CanMana(targetid)==FALSE)
			{  
				OrderStop(m_NewID, BY_COMPUTER);
				return FALSE;
			}
			if(IsInRange(targetid.ID, healrange))
			{
				Direction = DecideDirection8(m_ssX, m_ssY, CR[targetid.ID].m_ssX, CR[targetid.ID].m_ssY);
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
		 ManaOperation(targetid);
	     break;
	}
	
	return FALSE;
}

BOOL _Char::ManaOperation(_NewID id)
{
	float manaability = 0 ;

	m_cltCharDrawInfo.SetAnimation(ANI_MANA);

	// 움직임 애니메이션 
	m_cltCharStatus.SetWorkDelayor(m_cltCharStatus.GetWorkDelayor() + 1);
	if(m_cltCharStatus.GetWorkDelayor() >= KI[m_cltCharStatus.GetKind()].m_clCharAnimation[ManaCharAnimationIndex].siDelay)
	{
		m_cltCharStatus.SetWorkDelayor(0);
		Step++;
		Step %= KI[m_cltCharStatus.GetKind()].m_clCharAnimation[ManaCharAnimationIndex].GetFrameNumber();
		
		m_cltCharDrawInfo.SetAniStep(Step);
		m_cltCharDrawInfo.SetUpdateAnimationSwitch(TRUE);
	}
	else
	{
		return FALSE;
	}

	
	 if(Step == KI[m_cltCharStatus.GetKind()].m_clCharAnimation[ManaCharAnimationIndex].siInstant)
	 {
		 // 치료할 수 없으면, 
	     if(CanMana(id)==FALSE)
		 {    
		     m_scBusySwitch=FALSE;
	         OrderStop(m_NewID, BY_COMPUTER);
	         return FALSE;
		 }

	     float mana = (float)CR[id.ID].m_ParmeterBox.IP.GetMana();
	     float maxmana = (float)CR[id.ID].m_ParmeterBox.GetMaxMana();

		 if(CR[m_NewID.ID].IsGeneral() == TRUE)
			manaability = 2.0f;
		 else if(CR[m_NewID.ID].IsGeneral() == FALSE)
			manaability = 1.2f;

		 SHORT recoveramount=0;
		 SHORT reducemagicmount=0;
		 SHORT needmagicpower=0;

		 // 온라인 모드에서의 치료 능력. 
		 

		 // 완전히 치료하는데 필요한 마법력 
		 needmagicpower = (SHORT)(maxmana - mana) / manaability;
		 
		 // 필요한 치료력보다 모자란다면, 
		 if(needmagicpower > m_ParmeterBox.IP.GetMana())
		 {
			 recoveramount		= m_ParmeterBox.IP.GetMana() * manaability;
			 reducemagicmount	= m_ParmeterBox.IP.GetMana();
		 }
		 else
		 {
			 recoveramount		=	maxmana-mana;
			 reducemagicmount	=	needmagicpower;
		 }

		 PushEffect(EFFECT_HEAL, m_ssX, m_ssY);

	     CR[id.ID].SetHealDraw(TRUE);

	     // 에너지를 회복시켜준다. 
		 CR[id.ID].IncreaseMana(recoveramount); 

		 // 치료력을 줄여준다. 
		 ReduceMana(reducemagicmount);

	     OrderStop(m_NewID, BY_COMPUTER);
	 }

	 if(Step==0)
	 {
	   m_scBusySwitch=FALSE;
	   return TRUE;
	 }
	 else return FALSE;

}