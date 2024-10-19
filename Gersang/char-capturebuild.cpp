#include <GSL.h>

#include <main.h>
#include <findpath5.h>
#include "maindata.h"
#include <etc.h>
#include "music.h"
#include <CharOrg.h>

#include <Magic.h>

#include "text.h"

extern	_MainData			GD;									// 게임의 주데이터 
extern	_Char				CR[];								// 캐릭터의 포인터 
extern	SI32				UserPlayerNumber;					// 사용자 관련 
extern	_Text 				Text;								// 텍스트 
extern	_KindInfo			KI[];								// 캐릭터 종류별 데이터 


// 플레이어 번호를 바꿔준다. 
BOOL _Char::ChangePlayer(SHORT destpnumber)
{
	
	// 선택되어 있는 유닛이면 선택을 해제한다.
	if(m_scSelectedSwitch==TRUE)
		SetSelect(FALSE, FALSE);
	
	
	// 팀에 소속이 되어 있다면 팀에서 제거한다. 
	//GD.Player[GetPlayerNumber()].Team.DeleteTeam(id);
	

	// 기존 플레이어에서 삭제한다. 
	GD.DecreaseCharNumber(GetPlayerNumber(), m_cltCharStatus.GetKind());
	
	
	// 변경된 플레이어 번호를 설정한다.
	m_cltCharStatus.SetPlayerNumber(destpnumber);
	m_cltCharStatus.SetOriginalPlayerNumber(destpnumber);

	//CR[id.ID].m_cltCharStatus.SetPlayerNumber(destpnumber);
	//CR[id.ID].m_cltCharStatus.SetOriginalPlayerNumber(destpnumber);
	
	// 새로운 플레이어에 추가한다. 
    GD.IncreaseCharNumber(destpnumber, m_cltCharStatus.GetKind());
	
	// 같은 편이 된 프레임을 저장한다. 
	m_cltCharStatus.SetChangedPlayerFrame(GD.Frame);

	return TRUE;
}

BOOL _Char::ChangePlayerMagic(SHORT destpnumber,_NewID id )
{
	// 선택되어 있는 유닛이면 선택을 해제한다.
	if(m_scSelectedSwitch==TRUE)
		SetSelect(FALSE, FALSE);
	
	
	// 팀에 소속이 되어 있다면 팀에서 제거한다. 
	//GD.Player[GetPlayerNumber()].Team.DeleteTeam(id);
	

	// 기존 플레이어에서 삭제한다. 
	//GD.DecreaseCharNumber(GetPlayerNumber(), m_cltCharStatus.GetKind());
	
	
	// 변경된 플레이어 번호를 설정한다.
	//m_cltCharStatus.SetPlayerNumber(destpnumber);
	//m_cltCharStatus.SetOriginalPlayerNumber(destpnumber);

	CR[id.ID].m_cltCharStatus.SetPlayerNumber(destpnumber);
	CR[id.ID].m_cltCharStatus.SetOriginalPlayerNumber(destpnumber);

	CR[id.ID].m_siOnlineID = -1;
	
	// 새로운 플레이어에 추가한다. 
    //GD.IncreaseCharNumber(destpnumber, m_cltCharStatus.GetKind());
	
	// 같은 편이 된 프레임을 저장한다. 
	//m_cltCharStatus.SetChangedPlayerFrame(GD.Frame);

	return TRUE;

}

///////////////////////////////////////////////////////////////////////
//  현혹 관련 함수 

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

// id를 현혹할 수 있는가?
BOOL _Char::CanBetray(_NewID id, int* preturnval)
{
	
	// 현혹할 적이 유효해야 한다. 
	if(IsSee(id)==FALSE)return FALSE;

	// 마법력이 충분해야 한다.
	if(m_ParmeterBox.IP.GetMana() < MAGICPOWER_BETRAY)	return FALSE;

	// 장수는 현혹할 수 없다. 
	if(KI[ CR[id.ID].m_cltCharStatus.GetKind() ].IsKindAtb(KINDATB_GENERAL))return FALSE;

	// 동맹군은 현혹하지 못한다.
	if(IsFriendToByPlayerNumber(GetPlayerNumber(), CR[id.ID].m_cltCharStatus.GetPlayerNumber())==TRUE)return FALSE;

	// 현혹하고자 하는 캐릭터의  체력이 2/3 이하로 떨어진 상태여야 한다. 
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

	// 목표로 삼는 캐릭터  
	_NewID targetid=Para2;

   switch(WorkStep)
   {
	  case WS_DECIDE_LENGTH_TO_DEST:    // 목표까지의 거리를 결정한다. 

		 // 적을 현혹할 수 없는 상태라면,  중단한다.
		 if(CanBetray(targetid, &returnval)==FALSE)
		 {   
			 m_scBusySwitch = FALSE;
		     OrderStop(m_NewID, BY_COMPUTER);
		     return FALSE;
		 }

		 // 적이 사정거리 안에 있으면,
		 if(IsInRange(targetid.ID, GetAttackRange()))
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
			   // 적을 현혹할 수없는  처지라면, 
			   if(CanBetray(targetid, &returnval)==FALSE)
			   {  
				   OrderStop(m_NewID, BY_COMPUTER);
				   return FALSE;
			   }
			   if(IsInRange(AttackID.ID, GetAttackRange()))
			   {
				  WorkStep=WS_CAPTURE;
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

	case WS_CAPTURE:

		BetrayOperation(targetid);
		 

		 // 자신을 없앤다. 
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
		// 적을 현혹할 수없는  처지라면, 
		 if(CanBetray(id, &returnval)==FALSE)
		 {   
			 m_scBusySwitch=FALSE;
		     OrderStop(m_NewID, BY_COMPUTER);
		     return FALSE;
		 }

		 // 마법력을 줄인다. 
		 m_ParmeterBox.IP.SetMana(m_ParmeterBox.IP.GetMana() - MAGICPOWER_BETRAY);
		 if( m_ParmeterBox.IP.GetMana() < 0 )	m_ParmeterBox.IP.SetMana(0);

		 PushEffect(EFFECT_HEAL, m_ssX, m_ssY);
	     CR[id.ID].SetHealDraw(TRUE);

		 // 상대방의 플레이어 번호를 바꾼다. 
		 if( (CR[id.ID].m_ParmeterBox.GetTotalMagicResist() < 25) || (CR[id.ID].m_ParmeterBox.GetTotalMagicResist() < uiRand))
		 {
			 CR[id.ID].HostID = m_NewID;
			 CR[id.ID].m_bChangePlayer = TRUE;
			 CR[id.ID].ChangePlayerMagic(GetPlayerNumber(),id);
		 }
		// 더이상 공격해서는 안된다. 
		 OrderStop(m_NewID, BY_COMPUTER);
		 
		 // 현혹된 대상이 공격중일 때에만.
		 if(CR[id.ID].CurrentOrder == ORDER_ATTACK)
		 {
			 OrderStop(id,   BY_PLAYER);
		 }
		 
		 CR[id.ID].ClearReserveOrder();
		 
		 // 현혹된 캐릭터를 공격하던 캐릭터도 공격을 멈춘다. 
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
//////// 깃발 차량을 본영으로 가져가는 것에 관련된 루틴들   ///////////
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


	// 원래 우리편인 차량은 안된다. 
	if(m_cltCharStatus.GetOriginalPlayerNumber() == GetPlayerNumber()) return FALSE;

	// 무효한 캐릭터라면, 
	if(IsSee(id.ID)==FALSE)return FALSE;
	
	// 아군이  아니면, 
	if(GetPlayerNumber() != CR[id.ID].GetPlayerNumber())return FALSE;

	// 정상상태가 아니면, 
	if(CR[id.ID].CanWork() != TRUE)return FALSE;

	return TRUE;
}


BOOL _Char::GoIntoHQ()
{
	SHORT status;

	// 목표로 삼는 건물 
	_NewID targetid=Para2;

   switch(WorkStep)
   {
	  case WS_DECIDE_LENGTH_TO_DEST:    // 목표까지의 거리를 결정한다. 

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

	case WS_CAPTURE:
		 if(CanGoIntoHQ(targetid)==FALSE)
		 {   
			 m_scBusySwitch=FALSE;
		     OrderStop(m_NewID, BY_COMPUTER);
		     return FALSE;
		 }

		 // 자신을 없앤다. 
		 return TRUE;


	   break;
	}

	return FALSE;
}
