#include <GSL.h>

#include <main.h>
#include <clGame.h>
#include <setchar.h>
#include "music.h"

#include <findpath5.h>
#include "maindata.h"
#include <KindInfo.h>

#include <Magic.h>
#include <Smoke.h>

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


BOOL	_Char::AttackMagicGuardianChicken()
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

							//이곳에서 확률 계산을 한다.위에서 해도 되고.
//							SI32 hitrate = 100;//CR[MagicID.ID].m_ParmeterBox.GetHitRate(enemydr) + 20;
//							if(hitrate>100)		hitrate = 100;

							SetBullet(KIND_BULLET_GUARDIAN_CHICK, bulletid, 
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

	return TRUE;
}

BOOL _Char::MagicGuardianChicken(_NewID attackID)
{
	_NewID	tempID;

	tempID.Init();

	SI16 siKind;
	siKind = TransKindImjinToOnline(CR[attackID.ID].m_cltCharStatus.GetKind());

	int siMonsterType = pGame->pOnlineWorld->pOnlineCharParser->GetCharHeaderID(siKind)->siMonsterType;

	int siHitrate = (GD.Random()%100);
	
	if(siMonsterType == ON_MONSTER_TYPE1)
	{
		if(siHitrate < m_siGuardianNHitrate)
		{
			tempID	=	attackID;		
		}
	}
	else if(siMonsterType == ON_MONSTER_TYPE2)
	{
		if(siHitrate < m_siGuardianBHitrate)
		{
			tempID	=	attackID;
		}
	}
	else if(siMonsterType == ON_MONSTER_TYPE3)
	{
		if(siHitrate < m_siGuardianSHitrate)
		{
			tempID	= attackID;
		}
	}
	else
	{
		if(siHitrate < m_siGuardianNoHaltHitrate)
		{
			tempID = attackID;
		}
	}

	if(tempID.IsEmpty())		return FALSE;

	
	SI32 smoke_id=FindEmptySmoke();
	if(smoke_id)
	{
		SetSmoke(KIND_SMOKE_GUARDIAN_CHICK, smoke_id, CR[tempID.ID].m_cltCharStatus.GetCenterX(), CR[tempID.ID].m_cltCharStatus.GetCenterY(), AREA_LAND);
	}

	_ImportantParameta ip;
	ZeroMemory(&ip, sizeof(_ImportantParameta));
	UI16 uiKind = TransKindImjinToOnline(KIND_ON_GUARDIAN_CHICK);
		
	ip.SetStr(pGame->pOnlineWorld->pOnlineCharParser->GetCharHeaderID(uiKind)->Parameta.GetStr());
	ip.SetDex(pGame->pOnlineWorld->pOnlineCharParser->GetCharHeaderID(uiKind)->Parameta.GetDex());
	ip.SetVit(pGame->pOnlineWorld->pOnlineCharParser->GetCharHeaderID(uiKind)->Parameta.GetVit());
	ip.SetInt(pGame->pOnlineWorld->pOnlineCharParser->GetCharHeaderID(uiKind)->Parameta.GetInt());
	ip.SetLife(pGame->pOnlineWorld->pOnlineCharParser->GetCharHeaderID(uiKind)->Parameta.GetLife());
	ip.SetMana(pGame->pOnlineWorld->pOnlineCharParser->GetCharHeaderID(uiKind)->Parameta.GetMana());
	ip.SetAC(pGame->pOnlineWorld->pOnlineCharParser->GetCharHeaderID(uiKind)->Parameta.GetAC());
	ip.SetLevel(1);

	SI32 siPrevKind = CR[tempID.ID].m_cltCharStatus.GetKind();
	SetChar(KIND_ON_GUARDIAN_CHICK, tempID.ID, CR[tempID.ID].GetX(), CR[tempID.ID].GetY(), CR[tempID.ID].Direction, 
		100, CR[tempID.ID].GetPlayerNumber(), &ip, NULL, CR[tempID.ID].m_siOnlineID);
	// 병아리로 변하기 전 Kind를 기억해둔다.(경험치 계산시 반영한다)
	CR[tempID.ID].m_siChickPrevKind = siPrevKind;

	// 다른 사용자에게 알린다.
	pGame->pBattle->PushUpdateCharInfo(tempID.ID);



	return FALSE;
}	