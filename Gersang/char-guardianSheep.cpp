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

BOOL _Char::MagicGuardianLifeUp()
{
	m_scBusySwitch=TRUE;

	PushEffect(EFFECT_GUARDIAN_SHEEP_ATTACK, 0, 0);

	for(SI16 id = 0; id < MAX_CHAR_NUMBER ; id++)
	{
		if(IsAlive(id) && (CR[id].GetPlayerNumber() == GetPlayerNumber()))
		{
			CR[id].m_bGuardianLifeUp	= true;

			//SI16 siLife = CR[id].m_ParmeterBox.IP.GetLife();

			CR[id].m_siGuardianLife		= CR[id].m_ParmeterBox.IP.GetLife();
			CR[id].m_siGuardianMaxLife	= CR[id].m_ParmeterBox.GetMaxLife();

//			CR[id].m_ParmeterBox.IP.SetVit(CR[id].m_ParmeterBox.IP.GetVit()*2);
//			CR[id].m_ParmeterBox.SetMaxLife(CR[id].m_ParmeterBox.GetMaxLife()*2);
//			CR[id].IncreaseLife(siLife);
		}
	}

	m_scBusySwitch=FALSE;
	OrderStop(m_NewID, BY_COMPUTER);

	return FALSE;
}

BOOL _Char::DeleteGuardianLifeUp()
{
	for(SI16 id = 0; id < MAX_CHAR_NUMBER ; id++)
	{
		if(CR[id].m_bGuardianLifeUp == true)
		{
			if(IsAlive(id) && (CR[id].GetPlayerNumber() == GetPlayerNumber()))
			{
				CR[id].m_bGuardianLifeUp = false;

				SI16 siLife = CR[id].m_ParmeterBox.IP.GetLife()/2;

				CR[id].m_siGuardianLife		= 0;
				CR[id].m_siGuardianMaxLife	= 0;
			}
		}
	}	
	return TRUE;
}