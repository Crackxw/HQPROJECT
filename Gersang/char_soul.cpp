#include <GSL.h>

#include <main.h>

#include "music.h"
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


BOOL _Char::CanSoul()
{
	if( IsAtb3(ATB3_SOUL) == FALSE ) return FALSE;

	if(m_ParmeterBox.IP.GetMana() < MAGICPOWER_SOUL)return FALSE;

	if(m_bSoulUse)			return FALSE;


	return TRUE;
}

BOOL _Char::SetSoul(SI32 siSoulDelay)
{	
	m_siSoulDelay = siSoulDelay;
	m_bSoulUse	  = true;
	return TRUE;
}

BOOL _Char::Soul()
{
	_NewID		targetnewid;		// 만들 분신의 newid
	SI32		aniindex;

	
	
	if(MagicCharAnimationIndex)
	{
		m_cltCharDrawInfo.SetAnimation(ANI_MAGIC);
		aniindex		= MagicCharAnimationIndex;
	}
	else
	{
		m_cltCharDrawInfo.SetAnimation(ANI_ATTACK);
		aniindex		= AttackCharAnimationIndex;
	}
	
	m_scBusySwitch		= TRUE;

	targetnewid		= Para2;


	m_cltCharStatus.SetWorkDelayor(m_cltCharStatus.GetWorkDelayor() + 1);
	if( m_cltCharStatus.GetWorkDelayor() < KI[m_cltCharStatus.GetKind()].m_clCharAnimation[aniindex].siDelay )	 
		return TRUE;
	m_cltCharStatus.SetWorkDelayor(0);
	
	if( KI[m_cltCharStatus.GetKind()].m_clCharAnimation[aniindex].GetFrameNumber() == 0 )
	{
		Step=0;
		m_cltCharDrawInfo.SetAniStep(Step);
	}
	else
	{
		++Step %= KI[m_cltCharStatus.GetKind()].m_clCharAnimation[aniindex].GetFrameNumber();
		m_cltCharDrawInfo.SetAniStep(Step);
		m_cltCharDrawInfo.SetUpdateAnimationSwitch(TRUE);
	}
	
	
	if( Step == KI[m_cltCharStatus.GetKind()].m_clCharAnimation[aniindex].siInstant )
	{
		if(CanSoul() == FALSE)
		{
			m_scBusySwitch=FALSE;
			OrderStop(m_NewID, BY_COMPUTER);
			return TRUE;
		}

		m_ParmeterBox.IP.SetStr(m_ParmeterBox.IP.GetStr() * 3);
		m_ParmeterBox.IP.SetDex(m_ParmeterBox.IP.GetDex() * 2);

		ReduceMana(MAGICPOWER_SOUL);  //마나 감소.

		PushEffect(EFFECT_AIRMAGIC,0,0);

		OrderStop(m_NewID, BY_COMPUTER);
		return TRUE;
			
	}
	if(Step==0)
	{
    	 SetSoul(100);
		 m_scBusySwitch=FALSE;
		 return TRUE;
	}
	else
		return FALSE;
	
}