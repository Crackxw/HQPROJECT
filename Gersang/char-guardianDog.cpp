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




BOOL _Char::MagicGuardianDamageResistUp()
{

	PushEffect(EFFECT_GUARDIAN_DOG_ATTACK, 0, 0);

	for(SI16 id = 0; id < MAX_CHAR_NUMBER ; id++)
	{
		if(IsAlive(id) && (CR[id].GetPlayerNumber() == GetPlayerNumber()))
		{
			CR[id].m_bDamageResist	= true;

			CR[id].m_ParmeterBox.SetTotalDamageResist(CR[id].m_ParmeterBox.GetTotalDamageResist()+10);
			CR[id].m_ParmeterBox.SetTotalMagicResist(CR[id].m_ParmeterBox.GetTotalMagicResist()+10);
		}
	}
	return FALSE;
}

BOOL _Char::DeleteGuardianDamageResistUp()
{
	for(SI16 id = 0; id < MAX_CHAR_NUMBER ; id++)
	{
		if(IsAlive(id) && (CR[id].GetPlayerNumber() == GetPlayerNumber()))
		{
			CR[id].m_bDamageResist	= false;

			CR[id].m_ParmeterBox.SetTotalDamageResist(CR[id].m_ParmeterBox.GetTotalDamageResist()-10);
			CR[id].m_ParmeterBox.SetTotalMagicResist(CR[id].m_ParmeterBox.GetTotalMagicResist()-10);
		}
	}
	return FALSE;
}


BOOL _Char::MagicGuardianMagicResistUp()
{
	for(SI16 id = 0; id < MAX_CHAR_NUMBER ; id++)
	{
		if(IsAlive(id) && (CR[id].GetPlayerNumber() == GetPlayerNumber()))
		{
			CR[id].m_bMagicResist	= true;

			CR[id].m_ParmeterBox.SetTotalMagicResist(CR[id].m_ParmeterBox.GetTotalMagicResist()+10);
		}
	}

	return FALSE;
}

BOOL _Char::DeleteGuardianMagicResistUp()
{

	for(SI16 id = 0; id < MAX_CHAR_NUMBER ; id++)
	{
		if(CR[id].m_bMagicResist == true)
		{
			if(IsAlive(id) && (CR[id].GetPlayerNumber() == GetPlayerNumber()))
			{
				CR[id].m_bMagicResist	= false;

				CR[id].m_ParmeterBox.SetTotalMagicResist(CR[id].m_ParmeterBox.GetTotalMagicResist()-10);
			}
		}
	}

	return FALSE;
}