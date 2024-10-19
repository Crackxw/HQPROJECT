//----------------------------------------------------------------
// 김태곤
//----------------------------------------------------------------

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


// id를 치료할 수 있는가?
// id자체의 정보만을 가지고 확인한다. 
BOOL CanHealGlobal(_NewID id)
{
	// 죽은 캐릭터는 치료할 수 없다. 
	if(IsSee(id.ID)==FALSE)return FALSE;

	// 에너지가 최대 이하여야 한다. 
	if(CR[id.ID].m_ParmeterBox.IP.GetLife() == CR[id.ID].m_ParmeterBox.GetMaxLife())return FALSE;

	// 치료가능 속성을 지니고 있어야 한다. 
	if(CR[id.ID].IsAtb(ATB_PATIENT)==FALSE)return FALSE;

	// 시한부 생명의 유닛이면 치료 불가. 
	if(CR[id.ID].GetTimerDelay())return FALSE;

	if(CR[id.ID].m_cltCharDrawInfo.GetTransparentSwitch() == TRUE)
		return FALSE;

	return TRUE;
}


// 자신이 치료가능한가?
BOOL _Char::CanHeal()
{

	// 치료속성을 가지고 있어야 한다. 
	if(IsAtb2(ATB2_HEAL)==FALSE)return FALSE;

	if(m_ParmeterBox.IP.GetMana() == 0) return FALSE; 

	return TRUE;
}

BOOL _Char::CanHeal(_NewID id)
{

	if(CanHealGlobal(id)==FALSE)return FALSE;

	if(CanHeal()==FALSE)return FALSE;

	// 동맹이 아니면 치료할 수 없다. 
	if(IsFriendToByPlayerNumber(GetPlayerNumber(), CR[id.ID].GetPlayerNumber())==FALSE)return FALSE;

	return TRUE;
}

BOOL _Char::SetHeal(_NewID id)
{

	if(CanHeal(id)==FALSE)return FALSE;

	WorkStep=WS_DECIDE_LENGTH_TO_DEST;

	return TRUE;
}

BOOL _Char::Heal()
{
	SHORT status;
	SHORT length=0;
	_NewID targetid=Para2;
	SHORT healrange=5;
	
   switch(WorkStep)
   {
	  case WS_DECIDE_LENGTH_TO_DEST:    // 목표까지의 거리를 결정한다. 

		 // 치료할 수 없다면 중단한다.
		 if(CanHeal(targetid)==FALSE)
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
			if(CanHeal(targetid)==FALSE)
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
		 HealOperation(targetid);
	     break;
	}
	
	return FALSE;
}



BOOL _Char::HealOperation(_NewID id)
{
	SI32 healability = 0 ;

	m_cltCharDrawInfo.SetAnimation(ANI_HEAL);

	// 움직임 애니메이션 
	m_cltCharStatus.SetWorkDelayor(m_cltCharStatus.GetWorkDelayor() + 1);
	if(m_cltCharStatus.GetWorkDelayor() >= KI[m_cltCharStatus.GetKind()].m_clCharAnimation[HealCharAnimationIndex].siDelay)
	{
		m_cltCharStatus.SetWorkDelayor(0);
		Step++;
		Step %= KI[m_cltCharStatus.GetKind()].m_clCharAnimation[HealCharAnimationIndex].GetFrameNumber();
		
		m_cltCharDrawInfo.SetAniStep(Step);
		m_cltCharDrawInfo.SetUpdateAnimationSwitch(TRUE);
	}
	else
	{
		return FALSE;
	}

	
	 if(Step == KI[m_cltCharStatus.GetKind()].m_clCharAnimation[HealCharAnimationIndex].siInstant)
	 {
		 // 치료할 수 없으면, 
	     if(CanHeal(id)==FALSE)
		 {    
		     m_scBusySwitch=FALSE;
	         OrderStop(m_NewID, BY_COMPUTER);
	         return FALSE;
		 }

	     SHORT health=CR[id.ID].m_ParmeterBox.IP.GetLife();
	     SHORT maxhealth=CR[id.ID].m_ParmeterBox.GetMaxLife();

		 SHORT recoveramount=0;
		 SHORT reducemagicmount=0;
		 SHORT needmagicpower=0;

		 // 온라인 모드에서의 치료 능력. 
		 healability = 3;

		 // 완전히 치료하는데 필요한 마법력 
		 needmagicpower = (maxhealth - health) / healability;
		 
		 // 필요한 치료력보다 모자란다면, 
		 if(needmagicpower > m_ParmeterBox.IP.GetMana())
		 {
			 recoveramount		= m_ParmeterBox.IP.GetMana() * healability;
			 reducemagicmount	= m_ParmeterBox.IP.GetMana();
		 }
		 else
		 {
			 recoveramount		=	maxhealth-health;
			 reducemagicmount	=	needmagicpower;
		 }

		 PushEffect(EFFECT_HEAL, m_ssX, m_ssY);

	     CR[id.ID].SetHealDraw(TRUE);

	     // 에너지를 회복시켜준다. 
		 CR[id.ID].IncreaseLife(recoveramount); 

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



// 주변의 모든 부상당한 캐릭터를 치료해 준다. 
void _Char::MassHeal()
{
 
	SHORT i, j;
	SHORT id;

	m_cltCharDrawInfo.SetAnimation(ANI_HEAL);

	// 움직임 애니메이션 
	m_cltCharStatus.SetWorkDelayor(m_cltCharStatus.GetWorkDelayor() + 1);
	if(m_cltCharStatus.GetWorkDelayor() >= KI[m_cltCharStatus.GetKind()].m_clCharAnimation[HealCharAnimationIndex].siDelay)
	{
		m_cltCharStatus.SetWorkDelayor(0);
		Step++;
		Step %= KI[m_cltCharStatus.GetKind()].m_clCharAnimation[HealCharAnimationIndex].GetFrameNumber();
		
		m_cltCharDrawInfo.SetAniStep(Step);
		m_cltCharDrawInfo.SetUpdateAnimationSwitch(TRUE);
	}
	else
	{
		return ;
	}

	
	 if(Step == KI[m_cltCharStatus.GetKind()].m_clCharAnimation[HealCharAnimationIndex].siInstant)
	 {
		 // 치료할 수 없으면, 
	     if(CanHeal()==FALSE)
		 {    
		     m_scBusySwitch=FALSE;
	         OrderStop(m_NewID, BY_COMPUTER);
	         return ;
		 }

		 PushEffect(EFFECT_HEAL, m_ssX, m_ssY);

		 for(i=-5;i<=5;i++)
		    for(j=-5;j<=5;j++)
			{
			    id=Map.GetIDMap(m_ssX+j, m_ssY+i);
			
			    if(CanHeal(CR[id].m_NewID)==TRUE)
				{

					SHORT health=CR[id].m_ParmeterBox.IP.GetLife();
	                SHORT maxhealth=CR[id].m_ParmeterBox.GetMaxLife();

					SHORT recoveramount;


					// 치료력보다 많이 다쳤다면, 
		            if(maxhealth-health > m_ParmeterBox.IP.GetMana())
					{
			            recoveramount=m_ParmeterBox.IP.GetMana();
					}
		            else
					{
			            recoveramount=maxhealth-health;
					}

					CR[id].SetHealDraw(TRUE);

	                // 에너지를 회복시켜준다. 
					clifChar.SetLife(id, m_ParmeterBox.IP.GetLife() + recoveramount);
		            // 치료력을 줄여준다. 
		            ReduceMana(recoveramount);
				}
			}
	 }

	 if( Step==0 )
	 {
		 m_scBusySwitch=FALSE;
		 OrderStop(m_NewID, BY_COMPUTER);
		 return;
	 }
	 else return;
}

// 인공지능으로 치료한다. 
BOOL _Char::HealAI()
{

	SHORT id;
	
	SHORT i, j;

	// 치료할 캐릭터를 찾는다.
	switch((GD.Frame / 2) % 3)
	{

	case 0:
	   if(IsAtb2(ATB2_HEAL)==FALSE)return FALSE;

	   for(i=-5;i<=5;i++)
		   for(j=-5;j<=5;j++)
		   {
			  id=Map.GetIDMap(m_ssX+j, m_ssY+i);
//			  if(id == 0)
//				id = clifMap.GetSkyMap(X+j, Y+i);
			  //소환
			  if(GD.CharInfo[id] == KIND_ON_PANDER || GD.CharInfo[id] == KIND_ON_GOLDDRAGON)
			  {
				  // robypark 2004/8/9 11:20
				  // 조세림의 팬더가 소환되었을 경우, 황룡이 있을 경우 힐러들이 일부 용병의 자동치료를 하지 않는
				  // 문제 수정
				  continue;
			//	  return FALSE;
			  }
			 	
			  if(CanHeal(CR[id].m_NewID)==TRUE)
			  {
				  // 저장된 작업이 없고
				  if(OldOrder.Order == ORDER_WAIT || OldOrder.Order ==ORDER_ATTACK )
				  {
				     // 현재 작업이 공격이어야...
			         if(CurrentOrder==ORDER_ATTACK)
					 {
				        SetOldOrder(ORDER_ATTACK, HostMode, FALSE, m_ssX|(m_ssY<<16), AttackID);
					 } 

				  }

			      OrderHeal(m_NewID, CR[id].m_NewID, BY_COMPUTER);
				  return TRUE;
			  }

		   } 
		   if(CanHeal(m_NewID)==TRUE)
		   {
			   if(OldOrder.Order == ORDER_WAIT || OldOrder.Order ==ORDER_ATTACK )
			   {
				 // 현재 작업이 공격이어야...
				 if(CurrentOrder==ORDER_ATTACK)
				 {
					SetOldOrder(ORDER_ATTACK, HostMode, FALSE, m_ssX|(m_ssY<<16), AttackID);
				 } 

			   }
			      OrderHeal(m_NewID, m_NewID, BY_COMPUTER);
		   }

		   break;


	case 1:

		if(CanUseBuda()==FALSE)return FALSE;

		if(OldOrder.Order==ORDER_WAIT )
		{
	         if(CurrentOrder==ORDER_ATTACK)
			 {
			    SetOldOrder(ORDER_ATTACK, HostMode, FALSE, m_ssX|(m_ssY<<16), AttackID);
			 }
		}

	     OrderUseBuda(m_NewID, BY_COMPUTER);
		return TRUE;
		break;

	case 2:

		// 방어막을 칠 수 없는 유닛이면, 
		if(CanShield()==FALSE)return FALSE;


	   for(i=-5;i<=5;i++)
		   for(j=-5;j<=5;j++)
		   {
			  id=clifMap.GetIDMap(m_ssX+j, m_ssY+i);
//			  if(id==0)
//				  id=clifMap.GetSkyMap(X+j, Y+i);
			
			  // 유효한 유닛이며 같은 편이고 
			  // 자폭수행중이거나 피격중이면, 
			  // 방어막을 칠 수 있는 유닛이면, 
			  if(IsSee(id) && CR[id].GetPlayerNumber() == GetPlayerNumber() 
			  && (CR[id].CurrentOrder==ORDER_SELFEXP || CR[id].BeAttackedSwitch==TRUE)
			  && CanShield(CR[id].m_NewID)==TRUE)
			  {
				  
				  if(OldOrder.Order==ORDER_WAIT )
				  {
	                 if(CurrentOrder==ORDER_ATTACK)
					 {
			             SetOldOrder(ORDER_ATTACK, HostMode, FALSE, m_ssX|(m_ssY<<16), AttackID);
					 }
				  }

	            // OrderShield(NewID, CR[id].NewID, BY_COMPUTER);


				  return TRUE;
			  }
		   } 


	}

	return FALSE;
}

