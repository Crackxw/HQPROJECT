///////////////////////////////////////////////////////////////////////
// ��� ��ġ�� ���� �Լ� 

#include <main.h>

#include <findpath5.h>
#include "maindata.h"
#include <etc.h>
#include "music.h"

#include <CharOrg.h>
#include "text.h"

#include <Magic.h>

extern	_MainData			GD;									// ������ �ֵ����� 
extern	_Char				CR[];								// ĳ������ ������ 
extern	_KindInfo			KI[];								// ĳ���� ������ ������ 
extern	SI32				UserPlayerNumber;					// ����� ���� 
extern	_Text 				Text;								// �ؽ�Ʈ 


BOOL _Char::SetStealTech(_NewID id)
{

	if(CanStealTech(id)==FALSE)return FALSE;

	WorkStep=WS_DECIDE_LENGTH_TO_DEST;
    AttackGiveUpDelayor=0;

	return TRUE;
}

// id�� ����� ��ĥ �� �ִ°�?
BOOL _Char::CanStealTech(_NewID id)
{
	// ���׷��̵尡 �Ǿ� �־�� �Ѵ�. 
//	if(GD.Player[PlayerNumber].UpgradeSwitch[UPGRADE_STEALTECH]==FALSE)return FALSE;
	
	// ��ĥ ��밡 ��ȿ�ؾ� �Ѵ�. 
	if(IsSee(id.ID)==FALSE)return FALSE;

	// �������� ����ؾ� �Ѵ�.
	if(m_ParmeterBox.IP.GetMana() < MAGICPOWER_STEALTECH)	return FALSE;

	// �ǹ� ĳ���͸� ����� �� �ִ�. 
	if(CR[id.ID].IsAtb(ATB_BUILDING)==FALSE)return FALSE;
	
	return TRUE;
}


BOOL _Char::StealTech()
{
	SHORT status;
	SHORT length=0;

	// ��ǥ�� ��� ĳ����  
	_NewID targetid=Para2;

   switch(WorkStep)
   {
	  case WS_DECIDE_LENGTH_TO_DEST:    // ��ǥ������ �Ÿ��� �����Ѵ�. 

		 if(CanStealTech(targetid)==FALSE)
		 {   
			 BusySwitch=FALSE;
		     OrderStop(NewID, BY_COMPUTER);
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

		   // �������� �����ϰų� �߰� ������ �����ϸ�, 
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

		 // �ڽ��� ���ش�. 
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



		 // ������ ���� ���׷��̵� ������ Ȯ���Ѵ�. 
		 listindex=0;
		 for(i=0;i<MAX_UPGRADE_NUMBER;i++)
		 {
			 // ������ �ִٸ�, 
			 if(KI[CR[id.ID].Kind].HaveUpgradeType(i)==TRUE )
			 {
				 // �̹� ���ߵ� �������� Ȯ���Ѵ�. 
				 //if(GD.Player[ CR[id.ID].PlayerNumber ].UpgradeSwitch[i]==TRUE)
				 {

					 // ����Ʈ�� �ִ´�. 
					 list[listindex]=i;
					 if(listindex<(10-1))
					     listindex++;
				 }
			 }
		 }

		 // ����Ʈ�� ��� �ִ� ����߿� �ϳ��� ����. 
		 if(listindex>0)
		 {
			 SHORT upgradekind=list[GD.Random()%listindex];

			 // �������� ���δ�. 
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
