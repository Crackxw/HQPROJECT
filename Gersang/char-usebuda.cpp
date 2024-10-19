#include <GSL.h>

#include <main.h>


#include "maindata.h"
#include <etc.h>
#include "music.h"


#include <map.h>
#include <CharOrg.h>

extern	_MainData			GD;									// 게임의 주데이터 
extern	_KindInfo			KI[];								// 캐릭터 종류별 데이터 
extern	_Map 				Map;
extern	_Char				CR[];								// 캐릭터의 포인터 


// 마법 보조를 행할 수 있는지 여부를 알려준다. 
BOOL _Char::CanUseBuda()
{

	// 현재 단계에서는 막아 놓는다. 
	return FALSE;

	// 소모된 마법력이 있어야 한다. 
	if(m_ParmeterBox.GetMaxMana() == m_ParmeterBox.IP.GetMana())
		return FALSE;

	// 마법을 구사해야 한다. 
	if(m_ParmeterBox.GetMaxMana() == 0)
		return FALSE;

	return TRUE;
}

// 주변의 마법 캐릭터로부터 마법력을 얻어온다.
void _Char::UseBuda()
{
	SHORT i, j;
	SHORT id;

	m_cltCharDrawInfo.SetAnimation(ANI_ATTACK);

	m_cltCharStatus.SetWorkDelayor(m_cltCharStatus.GetWorkDelayor() + 1);
	if(m_cltCharStatus.GetWorkDelayor() < KI[m_cltCharStatus.GetKind()].m_clCharAnimation[AttackCharAnimationIndex].siDelay){ return ;}
	m_cltCharStatus.SetWorkDelayor(0);

	if(KI[m_cltCharStatus.GetKind()].m_clCharAnimation[AttackCharAnimationIndex].GetFrameNumber()==0)
	{
		Step=0;
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

		 PushEffect(EFFECT_HEAL, m_ssX, m_ssY);

		 for(i = -5;i <= 5;i++)
		    for(j = -5;j <= 5;j++)
			{
				// 마법력이 부족하지 않으면 , 
				if(m_ParmeterBox.IP.GetMana() >= m_ParmeterBox.GetMaxMana())continue;

			    id=Map.GetIDMap(m_ssX+j, m_ssY+i);
			
				// 유효하며 아군이고 마법력을 지닌 것이라면, 
			    if(IsSee(id) && CR[id].GetPlayerNumber() == GetPlayerNumber()
				&& CR[id].m_ParmeterBox.IP.GetMana()
				&& KI[CR[id].m_cltCharStatus.GetKind()].IsKindAtb(KINDATB_GENERAL) == FALSE)
				{
					// 나의 부족한 마법력을 구한다. 
					SHORT needmagicpower = m_ParmeterBox.GetMaxMana() - m_ParmeterBox.IP.GetMana();

					// 상대의 마법력이 적으면 
					if(needmagicpower > CR[id].m_ParmeterBox.IP.GetMana())
					{
						m_ParmeterBox.IP.SetMana(m_ParmeterBox.IP.GetMana() + CR[id].m_ParmeterBox.IP.GetMana());
						CR[id].m_ParmeterBox.IP.SetMana(0);
						CR[id].SetHealDraw(TRUE);
					}
					else
					{
						CR[id].m_ParmeterBox.IP.SetMana( CR[id].m_ParmeterBox.IP.GetMana()-(m_ParmeterBox.GetMaxMana()-m_ParmeterBox.IP.GetMana()) );
						m_ParmeterBox.IP.SetMana(m_ParmeterBox.GetMaxMana());

						CR[id].SetHealDraw(TRUE);
					}

				}
			}
	 }

	 if(Step==0)
	 {
	   m_scBusySwitch=FALSE;
	   OrderStop(m_NewID, BY_COMPUTER);
	   return ;
	 }
	 else return ;
}

