#include <main.h>

#include <findpath5.h>
#include "maindata.h"
#include <etc.h>
#include "music.h"
#include "calcattack.h"

#include <CharOrg.h>
#include <smoke.h>
#include <charinterface.h>

#include <Magic.h>

extern	_Char				CR[];								// ĳ������ ������ 
extern	_MainData			GD;									// ������ �ֵ����� 
extern	_KindInfo			KI[];								// ĳ���� ������ ������ 
extern	cltCharInterface	clifChar;							// ĳ���� ���� �������̽� 
extern	int 				MapCenterX[][MAX_MAP_YSIZE];		// ������ �� ��ǥ������ �߽ɰ� 
extern	int 				MapCenterY[][MAX_MAP_YSIZE];



// id�� ǳ����� ������ �� �ִ°�?
// id��ü�� �������� ������ Ȯ���Ѵ�. 
BOOL CanTornadoGlobal(_NewID id)
{
	// ���� ĳ���ʹ� ������ �� ����.
	if(IsSee(id.ID)==FALSE)return FALSE;

	// ���� ĳ���ʹ� ������ �� ����. 
	if(CR[id.ID].IsAtb(ATB_FLY))return FALSE;

	return TRUE;
}


// �ڽ��� ������ �����Ѱ�?
BOOL _Char::CanTornado()
{
	// �������� ���� �־�� �Ѵ�. 
	if(m_ParmeterBox.IP.GetMana() < MAGICPOWER_TORNADO)	return FALSE;

	return TRUE;
}

BOOL _Char::CanTornado(_NewID id)
{

	if(CanTornadoGlobal(id)==FALSE)return FALSE;


	if(CanTornado()==FALSE)return FALSE;


	return TRUE;
}

BOOL _Char::SetTornado(_NewID id)
{

	if(CanTornado(id)==FALSE)return FALSE;

	WorkStep=WS_DECIDE_LENGTH_TO_DEST;

	return TRUE;
}

BOOL _Char::TornadoOperation(_NewID id)
{

	SHORT smokeid;

	CharDrawInfo.SetAnimation(ANI_ATTACK);

	WorkDelayor++;
	if(WorkDelayor < KI[m_cltCharStatus.GetKind()].clCharAnimation[AttackCharAnimationIndex].siDelay){ return FALSE;}
	WorkDelayor=0;


	if(KI[m_cltCharStatus.GetKind()].clCharAnimation[AttackCharAnimationIndex].GetFrameNumber()==0)
	{
		Step=0;
		CharDrawInfo.SetAniStep(Step);
	}
	else
	{
	   ++Step %= KI[m_cltCharStatus.GetKind()].clCharAnimation[AttackCharAnimationIndex].GetFrameNumber();
	   CharDrawInfo.SetAniStep(Step);
	   CharDrawInfo.SetUpdateAnimationSwitch(TRUE);
	}

	
	 if(Step == KI[m_cltCharStatus.GetKind()].clCharAnimation[AttackCharAnimationIndex].siInstant)
	 {
		 // ǳ����� ����ų �� ���ٸ�, 
	     if(CanTornado(id)==FALSE)
		 {    
		     BusySwitch=FALSE;
	         OrderStop(NewID, BY_COMPUTER);
	         return FALSE;
		 }

		 // ������ Ÿ���� �ش�. 
		 SI32 level = m_ParmeterBox.IP.GetLevel();

	     // �������� �ٿ��ش�. 
		 if(ReduceMana(MAGICPOWER_TORNADO) == FALSE)return FALSE;

		 // ǳ����� ������ ���� �����Ѵ�. 
		 smokeid=FindEmptySmoke();
		 if(smokeid)
		 {
			 PushEffect(EFFECT_TORNADO, clifChar.GetX(id.ID), clifChar.GetY(id.ID));

			 SetSmoke(KIND_SMOKE_TORNADO, smokeid, MapCenterX[CR[id.ID].X][CR[id.ID].Y], MapCenterY[CR[id.ID].X][CR[id.ID].Y], AREA_LAND);

			 // hojae_repair
			 Smoke_PlayerNumber(smokeid, GetPlayerNumber());
			 Smoke_Apower(smokeid, CalcDamage(CALCDAMAGE_TORNADO, m_ParmeterBox.CalcApower()) );
			 Smoke_AttackID(smokeid, id);
			 Smoke_AttackerID(smokeid, NewID);
		 }

	     OrderStop(NewID, BY_COMPUTER);
	 }

	 if(Step==0)
	 {
	   BusySwitch=FALSE;
	   return TRUE;
	 }
	 else return FALSE;

}

BOOL _Char::Tornado()
{
	SHORT status;
	SHORT length=0;
	_NewID targetid=Para2;
	SHORT healrange = 7;
	
   switch(WorkStep)
   {
	  case WS_DECIDE_LENGTH_TO_DEST:    // ��ǥ������ �Ÿ��� �����Ѵ�. 

		 // ǳ����� ����ų �� ���ٸ�, �ߴ��Ѵ�.
		 if(CanTornado(targetid)==FALSE)
		 {   
			 BusySwitch=FALSE;
		     OrderStop(NewID, BY_COMPUTER);
		     return FALSE;
		 }

		 // �����Ÿ� �ȿ� ������,
		 if(IsInRange(targetid.ID, healrange))
		 { 
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
		if(MoveDone())BusySwitch=FALSE;
		else
		{
			AttackGiveUpDelayor=0;
			BusySwitch=TRUE;
		}

		   if(BusySwitch==FALSE)
		   {
			   if(CanTornado(targetid)==FALSE)
			   {  
				   OrderStop(NewID, BY_COMPUTER);
				   return FALSE;
			   }
			   if(IsInRange(AttackID.ID, healrange))
			   {
				  WorkStep=WS_HEAL;
				  Step=0;
			   }

 		       // �������� �����ϰų� �߰� ������ �����ϸ�, 
		       if(status>0)
			   {
			       WorkStep=WS_DECIDE_LENGTH_TO_DEST;
				   
			       AttackGiveUpDelayor++;
		           if(AttackGiveUpDelayor>AttackGiveUpDelay)
				      OrderStop(NewID, BY_COMPUTER);
			   }

		   }

		break;

	case WS_HEAL:
		 // Ÿ���� �������� �����ϸ� ġ���� �ش�. 
		 TornadoOperation(targetid);
	     break;
	}
	
	return FALSE;
}



