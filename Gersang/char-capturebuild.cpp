#include <GSL.h>

#include <main.h>
#include <findpath5.h>
#include "maindata.h"
#include <etc.h>
#include "music.h"
#include <CharOrg.h>

#include <Magic.h>

#include "text.h"

extern	_MainData			GD;									// ������ �ֵ����� 
extern	_Char				CR[];								// ĳ������ ������ 
extern	SI32				UserPlayerNumber;					// ����� ���� 
extern	_Text 				Text;								// �ؽ�Ʈ 
extern	_KindInfo			KI[];								// ĳ���� ������ ������ 


// �÷��̾� ��ȣ�� �ٲ��ش�. 
BOOL _Char::ChangePlayer(SHORT destpnumber)
{
	
	// ���õǾ� �ִ� �����̸� ������ �����Ѵ�.
	if(m_scSelectedSwitch==TRUE)
		SetSelect(FALSE, FALSE);
	
	
	// ���� �Ҽ��� �Ǿ� �ִٸ� ������ �����Ѵ�. 
	//GD.Player[GetPlayerNumber()].Team.DeleteTeam(id);
	

	// ���� �÷��̾�� �����Ѵ�. 
	GD.DecreaseCharNumber(GetPlayerNumber(), m_cltCharStatus.GetKind());
	
	
	// ����� �÷��̾� ��ȣ�� �����Ѵ�.
	m_cltCharStatus.SetPlayerNumber(destpnumber);
	m_cltCharStatus.SetOriginalPlayerNumber(destpnumber);

	//CR[id.ID].m_cltCharStatus.SetPlayerNumber(destpnumber);
	//CR[id.ID].m_cltCharStatus.SetOriginalPlayerNumber(destpnumber);
	
	// ���ο� �÷��̾ �߰��Ѵ�. 
    GD.IncreaseCharNumber(destpnumber, m_cltCharStatus.GetKind());
	
	// ���� ���� �� �������� �����Ѵ�. 
	m_cltCharStatus.SetChangedPlayerFrame(GD.Frame);

	return TRUE;
}

BOOL _Char::ChangePlayerMagic(SHORT destpnumber,_NewID id )
{
	// ���õǾ� �ִ� �����̸� ������ �����Ѵ�.
	if(m_scSelectedSwitch==TRUE)
		SetSelect(FALSE, FALSE);
	
	
	// ���� �Ҽ��� �Ǿ� �ִٸ� ������ �����Ѵ�. 
	//GD.Player[GetPlayerNumber()].Team.DeleteTeam(id);
	

	// ���� �÷��̾�� �����Ѵ�. 
	//GD.DecreaseCharNumber(GetPlayerNumber(), m_cltCharStatus.GetKind());
	
	
	// ����� �÷��̾� ��ȣ�� �����Ѵ�.
	//m_cltCharStatus.SetPlayerNumber(destpnumber);
	//m_cltCharStatus.SetOriginalPlayerNumber(destpnumber);

	CR[id.ID].m_cltCharStatus.SetPlayerNumber(destpnumber);
	CR[id.ID].m_cltCharStatus.SetOriginalPlayerNumber(destpnumber);

	CR[id.ID].m_siOnlineID = -1;
	
	// ���ο� �÷��̾ �߰��Ѵ�. 
    //GD.IncreaseCharNumber(destpnumber, m_cltCharStatus.GetKind());
	
	// ���� ���� �� �������� �����Ѵ�. 
	//m_cltCharStatus.SetChangedPlayerFrame(GD.Frame);

	return TRUE;

}

///////////////////////////////////////////////////////////////////////
//  ��Ȥ ���� �Լ� 

BOOL _Char::SetBetray(_NewID id, int* preturnval)
{
	if(CanBetray(id, preturnval)==FALSE)
	{
		return FALSE;
	}

	WorkStep=WS_DECIDE_LENGTH_TO_DEST;
    AttackGiveUpDelayor=0;

	return TRUE;
}

// id�� ��Ȥ�� �� �ִ°�?
BOOL _Char::CanBetray(_NewID id, int* preturnval)
{
	
	// ��Ȥ�� ���� ��ȿ�ؾ� �Ѵ�. 
	if(IsSee(id)==FALSE)return FALSE;

	// �������� ����ؾ� �Ѵ�.
	if(m_ParmeterBox.IP.GetMana() < MAGICPOWER_BETRAY)	return FALSE;

	// ����� ��Ȥ�� �� ����. 
	if(KI[ CR[id.ID].m_cltCharStatus.GetKind() ].IsKindAtb(KINDATB_GENERAL))return FALSE;

	// ���ͱ��� ��Ȥ���� ���Ѵ�.
	if(IsFriendToByPlayerNumber(GetPlayerNumber(), CR[id.ID].m_cltCharStatus.GetPlayerNumber())==TRUE)return FALSE;

	// ��Ȥ�ϰ��� �ϴ� ĳ������  ü���� 2/3 ���Ϸ� ������ ���¿��� �Ѵ�. 
/*	if(CR[id.ID].m_ParmeterBox.IP.GetLife() >= CR[id.ID].m_ParmeterBox.GetMaxLife()*2/3 )
	{
		*preturnval = ERROR_BETRAY_HEALTH;
		return FALSE;
	}
*/
	return TRUE;
}


BOOL _Char::Betray()
{
	SI32 returnval = 0;
	SHORT status;
	SHORT length=0;

	// ��ǥ�� ��� ĳ����  
	_NewID targetid=Para2;

   switch(WorkStep)
   {
	  case WS_DECIDE_LENGTH_TO_DEST:    // ��ǥ������ �Ÿ��� �����Ѵ�. 

		 // ���� ��Ȥ�� �� ���� ���¶��,  �ߴ��Ѵ�.
		 if(CanBetray(targetid, &returnval)==FALSE)
		 {   
			 m_scBusySwitch = FALSE;
		     OrderStop(m_NewID, BY_COMPUTER);
		     return FALSE;
		 }

		 // ���� �����Ÿ� �ȿ� ������,
		 if(IsInRange(targetid.ID, GetAttackRange()))
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
			   // ���� ��Ȥ�� ������  ó�����, 
			   if(CanBetray(targetid, &returnval)==FALSE)
			   {  
				   OrderStop(m_NewID, BY_COMPUTER);
				   return FALSE;
			   }
			   if(IsInRange(AttackID.ID, GetAttackRange()))
			   {
				  WorkStep=WS_CAPTURE;
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

	case WS_CAPTURE:

		BetrayOperation(targetid);
		 

		 // �ڽ��� ���ش�. 
		 return TRUE;

	   break;
	}

	return FALSE;
}

BOOL _Char::BetrayOperation(_NewID id)
{

	SHORT i;
	SHORT temp_id;
	SI32 returnval = 0;

	UI16 uiRand = GD.Random() % 75 + 1;


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
	   ++Step %= KI[m_cltCharStatus.GetKind()].m_clCharAnimation[AttackCharAnimationIndex].GetFrameNumber();
	   m_cltCharDrawInfo.SetAniStep(Step);
	   m_cltCharDrawInfo.SetUpdateAnimationSwitch(TRUE);
	}

	
	 if(Step == KI[m_cltCharStatus.GetKind()].m_clCharAnimation[AttackCharAnimationIndex].siInstant)
	 {
		// ���� ��Ȥ�� ������  ó�����, 
		 if(CanBetray(id, &returnval)==FALSE)
		 {   
			 m_scBusySwitch=FALSE;
		     OrderStop(m_NewID, BY_COMPUTER);
		     return FALSE;
		 }

		 // �������� ���δ�. 
		 m_ParmeterBox.IP.SetMana(m_ParmeterBox.IP.GetMana() - MAGICPOWER_BETRAY);
		 if( m_ParmeterBox.IP.GetMana() < 0 )	m_ParmeterBox.IP.SetMana(0);

		 PushEffect(EFFECT_HEAL, m_ssX, m_ssY);
	     CR[id.ID].SetHealDraw(TRUE);

		 // ������ �÷��̾� ��ȣ�� �ٲ۴�. 
		 if( (CR[id.ID].m_ParmeterBox.GetTotalMagicResist() < 25) || (CR[id.ID].m_ParmeterBox.GetTotalMagicResist() < uiRand))
		 {
			 CR[id.ID].HostID = m_NewID;
			 CR[id.ID].m_bChangePlayer = TRUE;
			 CR[id.ID].ChangePlayerMagic(GetPlayerNumber(),id);
		 }
		// ���̻� �����ؼ��� �ȵȴ�. 
		 OrderStop(m_NewID, BY_COMPUTER);
		 
		 // ��Ȥ�� ����� �������� ������.
		 if(CR[id.ID].CurrentOrder == ORDER_ATTACK)
		 {
			 OrderStop(id,   BY_PLAYER);
		 }
		 
		 CR[id.ID].ClearReserveOrder();
		 
		 // ��Ȥ�� ĳ���͸� �����ϴ� ĳ���͵� ������ �����. 
		 for(i=0;i<GD.CharOrderIndex;i++)
		 {

			 temp_id=GD.CharOrder[i];

			 if(IsSee(temp_id) && CR[temp_id].CurrentOrder==ORDER_ATTACK 
			 && CR[temp_id].AttackID.IsSame(id) )
			 {
				 OrderStop(CR[temp_id].m_NewID, BY_COMPUTER);
			 }

		 }
	 }

	 if(Step==0)
	 {
	   m_scBusySwitch=FALSE;
	   return TRUE;
	 }
	 else return FALSE;

}

///////////////////////////////////////////////////////////////////////
//////// ��� ������ �������� �������� �Ϳ� ���õ� ��ƾ��   ///////////
///////////////////////////////////////////////////////////////////////


BOOL _Char::SetGoIntoHQ(_NewID id)
{

	if(CanGoIntoHQ(id)==FALSE)return FALSE;

	WorkStep=WS_DECIDE_LENGTH_TO_DEST;
    AttackGiveUpDelayor=0;

	return TRUE;
}



BOOL _Char::CanGoIntoHQ(_NewID id)
{


	// ���� �츮���� ������ �ȵȴ�. 
	if(m_cltCharStatus.GetOriginalPlayerNumber() == GetPlayerNumber()) return FALSE;

	// ��ȿ�� ĳ���Ͷ��, 
	if(IsSee(id.ID)==FALSE)return FALSE;
	
	// �Ʊ���  �ƴϸ�, 
	if(GetPlayerNumber() != CR[id.ID].GetPlayerNumber())return FALSE;

	// ������°� �ƴϸ�, 
	if(CR[id.ID].CanWork() != TRUE)return FALSE;

	return TRUE;
}


BOOL _Char::GoIntoHQ()
{
	SHORT status;

	// ��ǥ�� ��� �ǹ� 
	_NewID targetid=Para2;

   switch(WorkStep)
   {
	  case WS_DECIDE_LENGTH_TO_DEST:    // ��ǥ������ �Ÿ��� �����Ѵ�. 

		 if(CanGoIntoHQ(targetid)==FALSE)
		 {   
			 m_scBusySwitch=FALSE;
		     OrderStop(m_NewID, BY_COMPUTER);
		     return FALSE;
		 }

		 if(IsInRange(targetid.ID, 1))
		 { 
			 WorkStep=WS_CAPTURE;
		 }
		 else 
		 {  
			  WorkStep=WS_GOTO_TARGET;
		      
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
			   if(CanGoIntoHQ(targetid)==FALSE)
			   {  
				   OrderStop(m_NewID, BY_COMPUTER);
				   return FALSE;
			   }
			   if(IsInRange(AttackID.ID, 1))
			   {
				  WorkStep=WS_CAPTURE;
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

	case WS_CAPTURE:
		 if(CanGoIntoHQ(targetid)==FALSE)
		 {   
			 m_scBusySwitch=FALSE;
		     OrderStop(m_NewID, BY_COMPUTER);
		     return FALSE;
		 }

		 // �ڽ��� ���ش�. 
		 return TRUE;


	   break;
	}

	return FALSE;
}
