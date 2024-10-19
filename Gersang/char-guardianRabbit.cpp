#include <GSL.h>

#include <main.h>
#include <clGame.h>
#include "music.h"

#include <findpath5.h>
#include "maindata.h"
#include <KindInfo.h>

#include <Magic.h>

#include <etc.h>
#include <map.h>
#include <bullet.h>
#include <CharOrg.h>
#include <CharInterface.h>
#include "calcattack.h"

extern _clGame* pGame;

extern	_Char				CR[];								// 캐릭터의 포인터 
extern	_MainData			GD;									// 게임의 주데이터 
extern	_KindInfo			KI[];								// 캐릭터 종류별 데이터 
extern	_Map 				Map;
extern	int 				MapCenterX[][MAX_MAP_YSIZE];		// 지도의 각 좌표마다의 중심값 
extern	int 				MapCenterY[][MAX_MAP_YSIZE];
extern	_NewID 				ZeroID;
extern	cltCharInterface		clifChar;							// 캐릭터 관련 인터페이스


BOOL _Char::AttackMagicGuardianRabbit()
{
	SI32	i, j,k;
	SI32	bulletid;
	SI32	firetilex, firetiley, desttilex, desttiley;
	_NewID	tempid;
	SI16    count = 0;

	for( k=0; k<50; k++ )
	{
		targetid[k] = -1;
	}

	for(i = -12;i <= 12;i++)
	{
		for(j = -12;j <= 12;j++)
		{
			if(i==0 && j ==0)continue;

			if(count < 20)
			{
				bulletid = FindEmptyBullet();
				if(bulletid)
				{
					firetilex = m_ssX + j;
					firetiley = m_ssY + i;
					if(Map.IsInMapArea(firetilex, firetiley) == TRUE)
					{
						//CR[NewID.ID].GetX();
						//CR[NewID.ID].GetY();

						tempid = FindNearEnemy(firetilex, firetiley);

						if(tempid.ID != 0)
						{
							//if(tempid.IsEmpty()==FALSE)
							{
								desttilex = m_ssAttackX;
								desttiley = m_ssAttackY;
							}
							/*else
							{
								desttilex = firetilex;
								desttiley = firetiley;
							}*/
							SI32 enemydr = CR[tempid.ID].m_ParmeterBox.GetDR();

							//이곳에서 확률 계산을 한다.위에서 해도 되고.
//							SI32 hitrate = 100;//CR[MagicID.ID].m_ParmeterBox.GetHitRate(enemydr) + 20;
//							if(hitrate>100)		hitrate = 100;

							SetBullet(KIND_BULLET_GUARDIAN_RABBIT, bulletid, 
								CR[m_NewID.ID].GetX(), CR[m_NewID.ID].GetY(), 
								firetilex, firetiley, 
								MapCenterX[CR[m_NewID.ID].GetX()][CR[m_NewID.ID].GetY()], MapCenterY[CR[m_NewID.ID].GetX()][CR[m_NewID.ID].GetY()], 
								MapCenterX[firetilex][firetiley], MapCenterY[firetilex][firetiley],
								m_NewID, tempid, 0, 100, 0, AREA_LAND);

							count++;
						}
					}
				}
			}
			
		}
	}
	m_scBusySwitch=FALSE;
	OrderStop(m_NewID, BY_COMPUTER);

	return TRUE;
}

BOOL _Char::MagicGuardianRabbit(_NewID attackID)
{
	SI16 siKind;
	siKind = TransKindImjinToOnline(CR[attackID.ID].m_cltCharStatus.GetKind());

	int siMonsterType = pGame->pOnlineWorld->pOnlineCharParser->GetCharHeaderID(siKind)->siMonsterType;

	int siHitrate = (GD.Random()%100);
	
	PushEffect(EFFECT_GUARDIAN_RABBIT_ATTACK, 0, 0);
	
	if(siMonsterType == ON_MONSTER_TYPE1)
	{
		if(siHitrate < m_siGuardianNHitrate)
		{
			CR[attackID.ID].m_RabbitShooterId = m_NewID.ID;
			CR[attackID.ID].m_bRabbitAttack = true;
			CR[attackID.ID].m_MoveDelay	    = 0.4f;
			CR[attackID.ID].m_AttackDelay   = 2.5f;
		}
	}
	else if(siMonsterType == ON_MONSTER_TYPE2)
	{
		if(siHitrate < m_siGuardianBHitrate)
		{
			CR[attackID.ID].m_RabbitShooterId = m_NewID.ID;
			CR[attackID.ID].m_bRabbitAttack = true;
			CR[attackID.ID].m_MoveDelay	   = 0.4f;
			CR[attackID.ID].m_AttackDelay  = 2.5f;
		}
	}
	else if(siMonsterType == ON_MONSTER_TYPE3)
	{
		if(siHitrate < m_siGuardianSHitrate)
		{
			CR[attackID.ID].m_RabbitShooterId = m_NewID.ID;
			CR[attackID.ID].m_bRabbitAttack = true;
			CR[attackID.ID].m_MoveDelay	   = 0.4f;
			CR[attackID.ID].m_AttackDelay  = 2.5f;
		}
	}
	else
	{
		if(siHitrate < m_siGuardianNoHaltHitrate)
		{
			CR[attackID.ID].m_RabbitShooterId = m_NewID.ID;
			CR[attackID.ID].m_bRabbitAttack = true;
			CR[attackID.ID].m_MoveDelay	   = 0.4f;
			CR[attackID.ID].m_AttackDelay  = 2.5f;
		}
	}

	return FALSE;
}

BOOL _Char::DeleteGuardianRabbit()
{
	PushEffect(EFFECT_GUARDIAN_RABBIT_END, 0, 0);

	for(SI16 id = 0; id < MAX_CHAR_NUMBER ; id++)
	{
		if(CR[id].m_bRabbitAttack == true)
		{
			if(CR[id].m_RabbitShooterId == m_NewID.ID)
			{
				CR[id].m_bRabbitAttack = false;
				CR[id].m_MoveDelay	 = 1.0f;
				CR[id].m_AttackDelay = 1.0f;
			}
		}
	}
	return TRUE;
}