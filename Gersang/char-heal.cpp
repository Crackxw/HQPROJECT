//----------------------------------------------------------------
// ���°�
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

extern	_Char				CR[];								// ĳ������ ������ 
extern	_MainData			GD;									// ������ �ֵ����� 
extern	_Map 				Map;
extern	_KindInfo			KI[];								// ĳ���� ������ ������ 
extern	cltMapInterface		clifMap;							// �ʰ��� �������̽� 
extern	cltCharInterface	clifChar;


// id�� ġ���� �� �ִ°�?
// id��ü�� �������� ������ Ȯ���Ѵ�. 
BOOL CanHealGlobal(_NewID id)
{
	// ���� ĳ���ʹ� ġ���� �� ����. 
	if(IsSee(id.ID)==FALSE)return FALSE;

	// �������� �ִ� ���Ͽ��� �Ѵ�. 
	if(CR[id.ID].m_ParmeterBox.IP.GetLife() == CR[id.ID].m_ParmeterBox.GetMaxLife())return FALSE;

	// ġ�ᰡ�� �Ӽ��� ���ϰ� �־�� �Ѵ�. 
	if(CR[id.ID].IsAtb(ATB_PATIENT)==FALSE)return FALSE;

	// ���Ѻ� ������ �����̸� ġ�� �Ұ�. 
	if(CR[id.ID].GetTimerDelay())return FALSE;

	if(CR[id.ID].m_cltCharDrawInfo.GetTransparentSwitch() == TRUE)
		return FALSE;

	return TRUE;
}


// �ڽ��� ġ�ᰡ���Ѱ�?
BOOL _Char::CanHeal()
{

	// ġ��Ӽ��� ������ �־�� �Ѵ�. 
	if(IsAtb2(ATB2_HEAL)==FALSE)return FALSE;

	if(m_ParmeterBox.IP.GetMana() == 0) return FALSE; 

	return TRUE;
}

BOOL _Char::CanHeal(_NewID id)
{

	if(CanHealGlobal(id)==FALSE)return FALSE;

	if(CanHeal()==FALSE)return FALSE;

	// ������ �ƴϸ� ġ���� �� ����. 
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
	  case WS_DECIDE_LENGTH_TO_DEST:    // ��ǥ������ �Ÿ��� �����Ѵ�. 

		 // ġ���� �� ���ٸ� �ߴ��Ѵ�.
		 if(CanHeal(targetid)==FALSE)
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
		 HealOperation(targetid);
	     break;
	}
	
	return FALSE;
}



BOOL _Char::HealOperation(_NewID id)
{
	SI32 healability = 0 ;

	m_cltCharDrawInfo.SetAnimation(ANI_HEAL);

	// ������ �ִϸ��̼� 
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
		 // ġ���� �� ������, 
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

		 // �¶��� ��忡���� ġ�� �ɷ�. 
		 healability = 3;

		 // ������ ġ���ϴµ� �ʿ��� ������ 
		 needmagicpower = (maxhealth - health) / healability;
		 
		 // �ʿ��� ġ��º��� ���ڶ��ٸ�, 
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

	     // �������� ȸ�������ش�. 
		 CR[id.ID].IncreaseLife(recoveramount); 

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



// �ֺ��� ��� �λ���� ĳ���͸� ġ���� �ش�. 
void _Char::MassHeal()
{
 
	SHORT i, j;
	SHORT id;

	m_cltCharDrawInfo.SetAnimation(ANI_HEAL);

	// ������ �ִϸ��̼� 
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
		 // ġ���� �� ������, 
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


					// ġ��º��� ���� ���ƴٸ�, 
		            if(maxhealth-health > m_ParmeterBox.IP.GetMana())
					{
			            recoveramount=m_ParmeterBox.IP.GetMana();
					}
		            else
					{
			            recoveramount=maxhealth-health;
					}

					CR[id].SetHealDraw(TRUE);

	                // �������� ȸ�������ش�. 
					clifChar.SetLife(id, m_ParmeterBox.IP.GetLife() + recoveramount);
		            // ġ����� �ٿ��ش�. 
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

// �ΰ��������� ġ���Ѵ�. 
BOOL _Char::HealAI()
{

	SHORT id;
	
	SHORT i, j;

	// ġ���� ĳ���͸� ã�´�.
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
			  //��ȯ
			  if(GD.CharInfo[id] == KIND_ON_PANDER || GD.CharInfo[id] == KIND_ON_GOLDDRAGON)
			  {
				  // robypark 2004/8/9 11:20
				  // �������� �Ҵ��� ��ȯ�Ǿ��� ���, Ȳ���� ���� ��� �������� �Ϻ� �뺴�� �ڵ�ġ�Ḧ ���� �ʴ�
				  // ���� ����
				  continue;
			//	  return FALSE;
			  }
			 	
			  if(CanHeal(CR[id].m_NewID)==TRUE)
			  {
				  // ����� �۾��� ����
				  if(OldOrder.Order == ORDER_WAIT || OldOrder.Order ==ORDER_ATTACK )
				  {
				     // ���� �۾��� �����̾��...
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
				 // ���� �۾��� �����̾��...
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

		// ���� ĥ �� ���� �����̸�, 
		if(CanShield()==FALSE)return FALSE;


	   for(i=-5;i<=5;i++)
		   for(j=-5;j<=5;j++)
		   {
			  id=clifMap.GetIDMap(m_ssX+j, m_ssY+i);
//			  if(id==0)
//				  id=clifMap.GetSkyMap(X+j, Y+i);
			
			  // ��ȿ�� �����̸� ���� ���̰� 
			  // �����������̰ų� �ǰ����̸�, 
			  // ���� ĥ �� �ִ� �����̸�, 
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

