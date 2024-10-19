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

BOOL _Char::MagicGuardianHorse()
{
	m_scBusySwitch=TRUE;

	PushEffect(EFFECT_GUARDIAN_HORSE_ATTACK, 0, 0);

	for(SI16 id = 0; id < MAX_CHAR_NUMBER ; id++)
	{
		if(IsAlive(id) && (CR[id].GetPlayerNumber() == GetPlayerNumber()))
		{
			//100%확률.
			CR[id].m_bHorseAttack = true;
			CR[id].m_MoveDelay	   = 2.0f;
			CR[id].m_AttackDelay   = 0.5f;
		}
	}

	m_scBusySwitch=FALSE;
	OrderStop(m_NewID, BY_COMPUTER);

	return FALSE;
}

BOOL _Char::DeleteGuardianHorse()
{
	PushEffect(EFFECT_GUARDIAN_HORSE_END, 0, 0);

	for(SI16 id = 0; id < MAX_CHAR_NUMBER ; id++)
	{
		if(CR[id].m_bHorseAttack == true)
		{
			if(CR[id].GetPlayerNumber() == GetPlayerNumber())
			{
				CR[id].m_bHorseAttack = false;
				CR[id].m_MoveDelay	= 1.0f;
				CR[id].m_AttackDelay = 1.0f;
			}
		}
	}
	return TRUE;
}