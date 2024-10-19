#include <GSL.h>

#include <main.h>

#include <findpath5.h>
#include "maindata.h"
#include <etc.h>
#include "calcattack.h"


#include <Magic.h>

#include <map.h>
#include <smoke.h>
#include <CharOrg.h>

extern	_Char				CR[];								// 캐릭터의 포인터 
extern	_MainData			GD;									// 게임의 주데이터 
extern	_KindInfo			KI[];								// 캐릭터 종류별 데이터 
extern	_Map 				Map;
extern	int 				MapCenterX[][MAX_MAP_YSIZE];		// 지도의 각 좌표마다의 중심값 
extern	int 				MapCenterY[][MAX_MAP_YSIZE];


// id를 지진으로 공격할 수 있는가?
// id자체의 정보만을 가지고 확인한다. 
BOOL CanLightningGlobal(_NewID id)
{
	// 죽은 캐릭터는 번개 공격할 수 없다.
	if(IsSee(id) == FALSE)return FALSE;

	// 비행체도 번개 공격할 수 없다.
	if(CR[id.ID].IsAtb(ATB_FLY))return FALSE;

	return TRUE;
}


// 자신이 공격이 가능한가?
BOOL _Char::CanLightning()
{
	// 마법력이 남아 있어야 한다. 
	if(m_ParmeterBox.IP.GetMana() < MAGICPOWER_LIGHTNING)return FALSE;

	return TRUE;
}

BOOL _Char::CanLightning(_NewID id)
{

	if(CanLightningGlobal(id)==FALSE)return FALSE;


	if(CanLightning()==FALSE)return FALSE;


	return TRUE;
}

BOOL _Char::SetLightning(_NewID id)
{

	if(CanLightning(id)==FALSE)return FALSE;

	WorkStep=WS_DECIDE_LENGTH_TO_DEST;

	return TRUE;
}

BOOL _Char::LightningOperation(_NewID id)
{

	SHORT i, j;

	m_cltCharDrawInfo.SetAnimation(ANI_ATTACK);

	m_cltCharStatus.SetWorkDelayor(m_cltCharStatus.GetWorkDelayor() + 1);
	if(m_cltCharStatus.GetWorkDelayor() < KI[m_cltCharStatus.GetKind()].m_clCharAnimation[AttackCharAnimationIndex].siDelay){ return FALSE;}
	m_cltCharStatus.SetWorkDelayor(0);


	if(KI[m_cltCharStatus.GetKind()].m_clCharAnimation[AttackCharAnimationIndex].GetFrameNumber()== 0)
	{
		Step = 0;
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


		 // 번개를 일으킬 수 없다면, 
	     if(CanLightning(id) == FALSE)
		 {    
		     m_scBusySwitch=FALSE;
	         OrderStop(m_NewID, BY_COMPUTER);
	         return FALSE;
		 }


	     // 마법력을 줄여준다. 
		 ReduceMana(MAGICPOWER_LIGHTNING);

		 // 땅바닥에 불을 붙여준다. 
		// Map.PutFire(CR[id.ID].X, CR[id.ID].Y, GD.Random()%2+1, GROUNDFIRE_LIFETIME);


		 // 레벨에 따라 공격력을 달리준다.
		 SHORT level =  m_ParmeterBox.IP.GetLevel();
		 GD.Rain.MakeThunder(m_NewID, GetPlayerNumber(), id, CR[id.ID].m_ssX, CR[id.ID].m_ssY, CalcDamage(SPECIAL_LIGHTNING, m_ParmeterBox.CalcApower(), m_scMagicApplyRate, level));

		 // 레벨이 높으면 번개 주변에 연기도 만든다. 
			 for(i = -1;i <= 1;i++)
				 for(j = -1;j <= 1;j++)
				 {
					 if(level < 20 && (j!=0 || i!=0))continue;

					 if(Map.IsInMapArea(CR[id.ID].m_ssX+j, CR[id.ID].m_ssY+i)==TRUE)
					 {
				          SHORT smokeid=FindEmptySmoke();
				          if(smokeid)
						  {
					          SHORT centerx=MapCenterX[CR[id.ID].m_ssX+j][CR[id.ID].m_ssY+i];
					          SHORT centery=MapCenterY[CR[id.ID].m_ssX+j][CR[id.ID].m_ssY+i];
				  

				              switch(GD.Random()%3)
							  { 	
				                case 0:
		                           SetSmoke(KIND_SMOKE_LIGHTEXP1, smokeid, centerx,centery-20, AREA_LAND);
					               break;
				               case 1:
					               SetSmoke(KIND_SMOKE_LIGHTEXP2, smokeid, centerx,centery-20, AREA_LAND);
					               break;
				               case 2:
					               SetSmoke(KIND_SMOKE_LIGHTEXP3, smokeid, centerx,centery-20, AREA_LAND);
					               break;
							  }  

						  }
					 }
				 }


	     OrderStop(m_NewID, BY_COMPUTER);
	 }

	 if(Step == 0)
	 {
	   m_scBusySwitch=FALSE;
	   return TRUE;
	 }
	 else return FALSE;

}

BOOL _Char::Lightning()
{
	SHORT status;
	SHORT length=0;
	_NewID targetid=Para2;
	SHORT healrange = GetAttackRange() + 2;
	
   switch(WorkStep)
   {
	  case WS_DECIDE_LENGTH_TO_DEST:    // 목표까지의 거리를 결정한다. 

		 // 번개를 일으킬 수 없다면, 중단한다.
		 if(CanLightning(targetid)==FALSE)
		 {   
			 m_scBusySwitch=FALSE;
		     OrderStop(m_NewID, BY_COMPUTER);
		     return FALSE;
		 }

		 // 사정거리 안에 있으면,
		 if(IsInRange(targetid.ID, healrange) == TRUE )
		 { 
			 WorkStep = WS_HEAL;
		 }
		 else // 적이 사거리 밖에 있으면, 
		 {  
			  WorkStep = WS_GOTO_TARGET;

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
			   if(CanLightning(targetid)==FALSE)
			   {  
				   OrderStop(m_NewID, BY_COMPUTER);
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
				     OrderStop(m_NewID, BY_COMPUTER);
			   }

		   }

		break;

	case WS_HEAL:

		 // 번개를 친다.
		 LightningOperation(targetid);

	     break;
	}
	
	return FALSE;
}



