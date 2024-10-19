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

//static SI32	targetid[50];
//static SI32	targetidCount;

BOOL _Char::AttackMagicGuardianTiger()
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

	for(i = -10;i <= 10;i++)
	{
		for(j = -10;j <= 10;j++)
		{
			if(i==0 && j ==0)continue;

			if(count < 3)
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
							SI16 siHitrate;

							//이곳에서 확률 계산을 한다.위에서 해도 되고.
//							SI32 hitrate = 100;//CR[MagicID.ID].m_ParmeterBox.GetHitRate(enemydr) + 20;
//							if(hitrate>100)		hitrate = 100;
							//CR[tempid.ID].get

							SI16 siKind;
							siKind = TransKindImjinToOnline(CR[tempid.ID].m_cltCharStatus.GetKind());

							int siMonsterType = pGame->pOnlineWorld->pOnlineCharParser->GetCharHeaderID(siKind)->siMonsterType;

						
							if(siMonsterType == ON_MONSTER_TYPE1)
							{
								siHitrate = m_siGuardianNHitrate;
							}						
							else if(siMonsterType == ON_MONSTER_TYPE2)
							{
								siHitrate = m_siGuardianBHitrate;
							}
							else if(siMonsterType == ON_MONSTER_TYPE3)
							{
								siHitrate = m_siGuardianSHitrate;
							}
							else 
							{
								siHitrate = m_siGuardianNoHaltHitrate;
							}
							PushEffect(EFFECT_GUARDIAN_TIGER_ATTACK, 0, 0);
							//tempid 뒤 인자 데미지.CalcDamage(에서 따로 계산 하던지..뒤에서 하던지.
							SetBullet(KIND_BULLET_GUARDIAN_TIGER, bulletid, 
								CR[m_NewID.ID].GetX(), CR[m_NewID.ID].GetY(), 
								firetilex, firetiley, 
								MapCenterX[CR[m_NewID.ID].GetX()][CR[m_NewID.ID].GetY()], MapCenterY[CR[m_NewID.ID].GetX()][CR[m_NewID.ID].GetY()], 
								MapCenterX[firetilex][firetiley], MapCenterY[firetilex][firetiley],
								m_NewID, tempid, 0, siHitrate, 0, AREA_LAND);
							count++;
						}
					}
				}
			}
			
		}
	}
	m_siGuardianNoTimeOutSkillUse = true;

	m_scBusySwitch=FALSE;
	OrderStop(m_NewID, BY_COMPUTER);

	return TRUE;
}

_NewID _Char::FindNearEnemy(SI16 siX,SI16 siY)
{
	SI32	k;
	SI32	id;
	SI32	tilex, tiley;
	_NewID  attack_newid,tempid;
//	SI32	length;
	SI32	min_length = 1000;
	SI32	min_attack_length = 1000;
	BOOL	bExist;

	targetidCount = 0;

	attack_newid.Init();

	tilex = siX;
	tiley = siY;
	
	if(Map.IsInMapArea(tilex, tiley)==TRUE)
	{
		id = Map.GetIDMap(tilex, tiley);
		if(IsAlive(id) && IsFriendToByPlayerNumber(GetPlayerNumber(),CR[id].GetPlayerNumber()) == FALSE)
		{
			tempid = clifChar.GetID(id);
			
			bExist = FALSE;

			for( k=0; k<50; k++ )
			{
				if( tempid.ID == targetid[k] )
					bExist = TRUE;
			}
			
			if( bExist == FALSE )
			{
				targetid[targetidCount] = tempid.ID;
				targetidCount++;
				if( targetidCount >= 50 ) targetidCount = 0;
				
				attack_newid = tempid;
				return attack_newid;
			}
		}
	}
	return ZeroID;

}