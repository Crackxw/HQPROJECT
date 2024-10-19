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

extern	_Char				CR[];								// ĳ������ ������ 
extern	_MainData			GD;									// ������ �ֵ����� 
extern	_Map 				Map;
extern	_KindInfo			KI[];								// ĳ���� ������ ������ 
extern	cltMapInterface		clifMap;							// �ʰ��� �������̽� 
extern	cltCharInterface	clifChar;

BOOL CanManaGlobal(_NewID id)
{
	// ���� ĳ���ʹ� ġ���� �� ����. 
	if(IsSee(id.ID)==FALSE)return FALSE;

	// ���� �������� �ִ� ���Ͽ��� �Ѵ�. 
	if(CR[id.ID].m_ParmeterBox.IP.GetMana() == CR[id.ID].m_ParmeterBox.GetMaxMana())return FALSE;

//	if( (KIND_ON_GENERAL6_C == CR[id.ID].m_cltCharStatus.GetKind()) || (KIND_ON_MAGIC_C == CR[id.ID].m_cltCharStatus.GetKind()))
//		return FALSE;

	// robypark 2004/9/1 13:15
	// ���� ȸ������ ���� ������ ����ȸ���� �� �� ����.
	if (KI[CR[id.ID].m_cltCharStatus.GetKind()].m_uiAtb2 & ATB2_MANA)
		return FALSE;

	// ġ�ᰡ�� �Ӽ��� ���ϰ� �־�� �Ѵ�. 
	if(CR[id.ID].IsAtb(ATB_PATIENT)==FALSE)return FALSE;

	// ���Ѻ� ������ �����̸� ġ�� �Ұ�. 
	if(CR[id.ID].GetTimerDelay())return FALSE;


	if(CR[id.ID].m_cltCharDrawInfo.GetTransparentSwitch() == TRUE)
		return FALSE;

	return TRUE;
}

BOOL _Char::CanMana()
{
	// ġ��Ӽ��� ������ �־�� �Ѵ�. 
	if(IsAtb2(ATB2_MANA)==FALSE)return FALSE;

	if(m_ParmeterBox.IP.GetMana() == 0) return FALSE; 

	return TRUE;
}


BOOL _Char::CanMana(_NewID id)
{
	if(CanManaGlobal(id)==FALSE)return FALSE;

	if(CanMana()==FALSE)return FALSE;

	// ������ �ƴϸ� ġ���� �� ����. 
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
	  case WS_DECIDE_LENGTH_TO_DEST:    // ��ǥ������ �Ÿ��� �����Ѵ�. 

		 // ġ���� �� ���ٸ� �ߴ��Ѵ�.
		 if(CanMana(targetid)==FALSE)
		 {   
			 m_scBusySwitch=FALSE;
		     OrderStop(m_NewID, BY_COMPUTER);
		     return FALSE;
		 }

		 // �����Ÿ� �ȿ� ������,
		 if(IsInRange(targetid.ID, healrange))
		 { 
			 Direction = DecideDirection8(m_ssX, m_ssY, CR[targetid.ID].m_ssX, CR[targetid.ID].m_ssY);
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
		 ManaOperation(targetid);
	     break;
	}
	
	return FALSE;
}

BOOL _Char::ManaOperation(_NewID id)
{
	float manaability = 0 ;

	m_cltCharDrawInfo.SetAnimation(ANI_MANA);

	// ������ �ִϸ��̼� 
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
		 // ġ���� �� ������, 
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

		 // �¶��� ��忡���� ġ�� �ɷ�. 
		 

		 // ������ ġ���ϴµ� �ʿ��� ������ 
		 needmagicpower = (SHORT)(maxmana - mana) / manaability;
		 
		 // �ʿ��� ġ��º��� ���ڶ��ٸ�, 
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

	     // �������� ȸ�������ش�. 
		 CR[id.ID].IncreaseMana(recoveramount); 

		 // ġ����� �ٿ��ش�. 
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