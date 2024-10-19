#include <GSL.h>

#include <main.h>


#include "maindata.h"
#include "music.h"

#include <CharOrg.h>
#include <Magic.h>

extern	_MainData			GD;									// 게임의 주데이터 
extern	_KindInfo			KI[];								// 캐릭터 종류별 데이터 


BOOL _Char::CanRainMake()
{

	// 마법력이 남아 있어야 한다. 
	if(m_ParmeterBox.IP.GetMana() < MAGICPOWER_RAINMAKE)	return FALSE;


	return TRUE;
}


void _Char::RainMake()
{

	SI32 siFrameNumber;
	SI32 siInstant;
	SI32 sidelay;

	m_cltCharDrawInfo.SetAnimation(ANI_MAGIC);
	siFrameNumber	= KI[m_cltCharStatus.GetKind()].m_clCharAnimation[MagicCharAnimationIndex].GetFrameNumber();
	siInstant		= KI[m_cltCharStatus.GetKind()].m_clCharAnimation[MagicCharAnimationIndex].siInstant;
	sidelay			= KI[m_cltCharStatus.GetKind()].m_clCharAnimation[MagicCharAnimationIndex].siDelay;
	
	
	m_scBusySwitch=TRUE;

	m_cltCharStatus.SetWorkDelayor(m_cltCharStatus.GetWorkDelayor() + 1);
	if(m_cltCharStatus.GetWorkDelayor() < sidelay){ return ;}
	m_cltCharStatus.SetWorkDelayor(0);

	// 효과음을 낸다. 
	if(Step==0)
	{
		 // 완전히 어두운 곳에 있지 않아야 소리가 난다. 
		 if(GD.IsInBlackFog(m_ssX, m_ssY)==FALSE)
		    PushEffect(EFFECT_GENERAL4KMAGIC, m_ssX, m_ssY);
	}


	if(siFrameNumber==0)
	{
		Step=0;
		m_cltCharDrawInfo.SetAniStep(Step);
	}
	else
	{
	   ++Step%=siFrameNumber;
	   m_cltCharDrawInfo.SetAniStep(Step);
	   m_cltCharDrawInfo.SetUpdateAnimationSwitch(TRUE);
	}

	
	 if(Step == siInstant)
	 {
	     if(CanRainMake()==FALSE)
		 {    
		     m_scBusySwitch=FALSE;
	         OrderStop(m_NewID, BY_COMPUTER);
	         return ;
		 }



	     // 마법력을 줄여준다. 
		 m_ParmeterBox.IP.SetMana(m_ParmeterBox.IP.GetMana() - MAGICPOWER_RAINMAKE);
		 if( m_ParmeterBox.IP.GetMana() < 0 )	m_ParmeterBox.IP.SetMana(0);

		 // 비를 오게 만든다. 
		 GD.Rain.SetRainSwitch(TRUE);
		 PushEffect(EFFECT_HEAL, m_ssX, m_ssY);
	 }

	 if(Step==0)
	 {
	   m_scBusySwitch=FALSE;
	   OrderStop(m_NewID, BY_COMPUTER);
	   return ;
	 }
	 else return ;
}

