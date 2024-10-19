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

extern	_Char				CR[];								// 캐릭터의 포인터 
extern	_MainData			GD;									// 게임의 주데이터 
extern	_KindInfo			KI[];								// 캐릭터 종류별 데이터 
extern	cltCharInterface	clifChar;							// 캐릭터 관련 인터페이스 
extern	int 				MapCenterX[][MAX_MAP_YSIZE];		// 지도의 각 좌표마다의 중심값 
extern	int 				MapCenterY[][MAX_MAP_YSIZE];



// id를 풍백술로 공격할 수 있는가?
// id자체의 정보만을 가지고 확인한다. 
BOOL CanTornadoGlobal(_NewID id)
{
	// 죽은 캐릭터는 공격할 수 없다.
	if(IsSee(id.ID)==FALSE)return FALSE;

	// 비행 캐릭터는 공격할 수 없다. 
	if(CR[id.ID].IsAtb(ATB_FLY))return FALSE;

	return TRUE;
}


// 자신이 공격이 가능한가?
BOOL _Char::CanTornado()
{
	// 마법력이 남아 있어야 한다. 
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
		 // 풍백술을 일으킬 수 없다면, 
	     if(CanTornado(id)==FALSE)
		 {    
		     BusySwitch=FALSE;
	         OrderStop(NewID, BY_COMPUTER);
	         return FALSE;
		 }

		 // 적에게 타격을 준다. 
		 SI32 level = m_ParmeterBox.IP.GetLevel();

	     // 마법력을 줄여준다. 
		 if(ReduceMana(MAGICPOWER_TORNADO) == FALSE)return FALSE;

		 // 풍백술을 일으켜 적을 공격한다. 
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
	  case WS_DECIDE_LENGTH_TO_DEST:    // 목표까지의 거리를 결정한다. 

		 // 풍백술을 일으킬 수 없다면, 중단한다.
		 if(CanTornado(targetid)==FALSE)
		 {   
			 BusySwitch=FALSE;
		     OrderStop(NewID, BY_COMPUTER);
		     return FALSE;
		 }

		 // 사정거리 안에 있으면,
		 if(IsInRange(targetid.ID, healrange))
		 { 
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

 		       // 목적지에 도착하거나 중간 지점에 도착하면, 
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
		 // 타겟의 에너지가 부족하면 치료해 준다. 
		 TornadoOperation(targetid);
	     break;
	}
	
	return FALSE;
}



