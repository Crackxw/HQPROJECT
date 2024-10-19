///////////////////////////////////////////////////////////////////////
// 기술 훔치기 관련 함수 

#include <main.h>

#include <findpath5.h>
#include "maindata.h"
#include <etc.h>
#include "music.h"

#include <CharOrg.h>
#include "text.h"

#include <Magic.h>

extern	_MainData			GD;									// 게임의 주데이터 
extern	_Char				CR[];								// 캐릭터의 포인터 
extern	_KindInfo			KI[];								// 캐릭터 종류별 데이터 
extern	SI32				UserPlayerNumber;					// 사용자 관련 
extern	_Text 				Text;								// 텍스트 


BOOL _Char::SetStealTech(_NewID id)
{

	if(CanStealTech(id)==FALSE)return FALSE;

	WorkStep=WS_DECIDE_LENGTH_TO_DEST;
    AttackGiveUpDelayor=0;

	return TRUE;
}

// id의 기술을 훔칠 수 있는가?
BOOL _Char::CanStealTech(_NewID id)
{
	// 업그레이드가 되어 있어야 한다. 
//	if(GD.Player[PlayerNumber].UpgradeSwitch[UPGRADE_STEALTECH]==FALSE)return FALSE;
	
	// 훔칠 상대가 유효해야 한다. 
	if(IsSee(id.ID)==FALSE)return FALSE;

	// 마법력이 충분해야 한다.
	if(m_ParmeterBox.IP.GetMana() < MAGICPOWER_STEALTECH)	return FALSE;

	// 건물 캐릭터만 흡수할 수 있다. 
	if(CR[id.ID].IsAtb(ATB_BUILDING)==FALSE)return FALSE;
	
	return TRUE;
}


BOOL _Char::StealTech()
{
	SHORT status;
	SHORT length=0;

	// 목표로 삼는 캐릭터  
	_NewID targetid=Para2;

   switch(WorkStep)
   {
	  case WS_DECIDE_LENGTH_TO_DEST:    // 목표까지의 거리를 결정한다. 

		 if(CanStealTech(targetid)==FALSE)
		 {   
			 BusySwitch=FALSE;
		     OrderStop(NewID, BY_COMPUTER);
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
		if(MoveDone())BusySwitch=FALSE;
		else
		{
			AttackGiveUpDelayor=0;
			BusySwitch=TRUE;
		}

		   if(BusySwitch==FALSE)
		   {
			   if(CanStealTech(targetid)==FALSE)
			   {  
				   OrderStop(NewID, BY_COMPUTER);
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
				  OrderStop(NewID, BY_COMPUTER);
		   }
		break;

	case WS_CAPTURE:
		StealTechOperation(targetid);

		 // 자신을 없앤다. 
		 return TRUE;

	   break;
	}

	return FALSE;
}

BOOL _Char::StealTechOperation(_NewID id)
{

	SHORT i;
	SHORT list[10];
	SHORT listindex=0;

	Animation=ANI_ATTACK;

	WorkDelayor++;
	if(WorkDelayor < KI[Kind].clCharAnimation[AttackCharAnimationIndex].siDelay){ return FALSE;}
	WorkDelayor=0;


	if(KI[Kind].clCharAnimation[AttackCharAnimationIndex].GetFrameNumber()==0)
	{
		Step=0;
		AniStep=Step;
	}
	else
	{
	   ++Step %= KI[Kind].clCharAnimation[AttackCharAnimationIndex].GetFrameNumber();
	   AniStep=Step;
	   UpdateAnimationSwitch=TRUE;
	}
	
	 if(Step == KI[Kind].clCharAnimation[AttackCharAnimationIndex].siInstant)
	 {
		 if(CanStealTech(id)==FALSE)
		 {   
			 BusySwitch=FALSE;
		     OrderStop(NewID, BY_COMPUTER);
		     return FALSE;
		 }



		 // 상대방이 가진 업그레이드 종류를 확인한다. 
		 listindex=0;
		 for(i=0;i<MAX_UPGRADE_NUMBER;i++)
		 {
			 // 가지고 있다면, 
			 if(KI[CR[id.ID].Kind].HaveUpgradeType(i)==TRUE )
			 {
				 // 이미 개발된 상태인지 확인한다. 
				 //if(GD.Player[ CR[id.ID].PlayerNumber ].UpgradeSwitch[i]==TRUE)
				 {

					 // 리스트에 넣는다. 
					 list[listindex]=i;
					 if(listindex<(10-1))
					     listindex++;
				 }
			 }
		 }

		 // 리스트에 들어 있는 목록중에 하나를 고른다. 
		 if(listindex>0)
		 {
			 SHORT upgradekind=list[GD.Random()%listindex];

			 // 마법력을 줄인다. 
		     m_ParmeterBox.IP.SetMana(m_ParmeterBox.IP.GetMana() - MAGICPOWER_STEALTECH);
		     if( m_ParmeterBox.IP.GetMana() < 0 )	m_ParmeterBox.IP.SetMana(0);

			 PushEffect(EFFECT_HEAL, X, Y);
	         CR[id.ID].SetHealDraw(TRUE);

		 }

		 OrderStop(NewID,BY_COMPUTER);
	 }

	 if(Step==0)
	 {
	   BusySwitch=FALSE;
	   return TRUE;
	 }
	 else return FALSE;

}
