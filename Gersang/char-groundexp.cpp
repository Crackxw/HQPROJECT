#include <GSL.h>

#include <main.h>

#include "music.h"

#include <findpath5.h>
#include "maindata.h"
#include <KindInfo.h>

#include <Magic.h>

#include <map.h>
#include <bullet.h>
#include <CharOrg.h>
#include "calcattack.h"

extern	_Char				CR[];								// 캐릭터의 포인터 
extern	_MainData			GD;									// 게임의 주데이터 
extern	_KindInfo			KI[];								// 캐릭터 종류별 데이터 
extern	_Map 				Map;
extern	int 				MapCenterX[][MAX_MAP_YSIZE];		// 지도의 각 좌표마다의 중심값 
extern	int 				MapCenterY[][MAX_MAP_YSIZE];
extern	_NewID 				ZeroID;

BOOL _Char::GroundExpOperation(_NewID id,SI16 siAttackX,SI16 siAttackY)
{
	 SHORT bulletid;
	 SHORT siCount = 0;
	 SI16  siPosX;
	 SI16  siPosY;

	 while(siCount < 1)
	 {
		 if(Map.IsInMapArea(siAttackX , siAttackY ) == TRUE)
		 {
			 bulletid = FindEmptyBullet();
			 if(bulletid)
			 {
				PushEffect(EFFECT_GUARDIAN_DRAGON_ATTACK, 0, 0);

				if(id.ID == 0)
				{
					siPosX  = m_ssX;
					siPosY  = m_ssY;

					SetBullet(KIND_BULLET_GROUNDEXP, bulletid, m_ssX , m_ssY, siPosX , siPosY , 
						 m_cltCharStatus.GetCenterX(), m_cltCharStatus.GetCenterY(), 
						 MapCenterX[siPosX][siPosY], MapCenterY[siPosX][siPosY], 
						 m_NewID, ZeroID, CalcDamage(SPECIAL_GROUNDEXP, m_ParmeterBox.CalcApower(),m_scMagicApplyRate), m_ParmeterBox.GetHitRate(0), 0, AttackArea);
				}
				else
				{
					SetBullet(KIND_BULLET_GROUNDEXP, bulletid, m_ssX , m_ssY, CR[id.ID].GetX() , CR[id.ID].GetY() , 
						 m_cltCharStatus.GetCenterX(), m_cltCharStatus.GetCenterY(), 
						 MapCenterX[siAttackX][siAttackY], MapCenterY[siAttackX][siAttackY], 
						 m_NewID, ZeroID, CalcDamage(SPECIAL_GROUNDEXP, m_ParmeterBox.CalcApower(),m_scMagicApplyRate), m_ParmeterBox.GetHitRate(0), 0, AttackArea);
				}
			 }
				 
			 // hojae_append
			 Bullet_DrawSwitch(bulletid, FALSE);
			 Bullet_CurrentOrder(bulletid, ORDER_BULLET_WAITEXP4);
			 
		 }
		 siCount++;
	 }

	
	 OrderStop(m_NewID, BY_COMPUTER);

	 //수호부를 사용한 기술이 종료 되었다.
	 m_siGuardianNoTimeOutSkillUse = true;

	 if(Step==0)
	 {
	   m_scBusySwitch=FALSE;
	   return TRUE;
	 }
	 else return FALSE;

}
		
BOOL _Char::GroundExp()
{
	SHORT status;
	SHORT length=0;
	SHORT healrange = GetAttackRange() + 2;
	
   switch(WorkStep)
   {
	  case WS_DECIDE_LENGTH_TO_DEST:    // 목표까지의 거리를 결정한다. 
		WorkStep=WS_HEAL;
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
//		GroundExpOperation(targetid);
	    break;
	}
	return FALSE;
}



