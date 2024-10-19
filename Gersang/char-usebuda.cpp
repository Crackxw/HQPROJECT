#include <GSL.h>

#include <main.h>


#include "maindata.h"
#include <etc.h>
#include "music.h"


#include <map.h>
#include <CharOrg.h>

extern	_MainData			GD;									// ������ �ֵ����� 
extern	_KindInfo			KI[];								// ĳ���� ������ ������ 
extern	_Map 				Map;
extern	_Char				CR[];								// ĳ������ ������ 


// ���� ������ ���� �� �ִ��� ���θ� �˷��ش�. 
BOOL _Char::CanUseBuda()
{

	// ���� �ܰ迡���� ���� ���´�. 
	return FALSE;

	// �Ҹ�� �������� �־�� �Ѵ�. 
	if(m_ParmeterBox.GetMaxMana() == m_ParmeterBox.IP.GetMana())
		return FALSE;

	// ������ �����ؾ� �Ѵ�. 
	if(m_ParmeterBox.GetMaxMana() == 0)
		return FALSE;

	return TRUE;
}

// �ֺ��� ���� ĳ���ͷκ��� �������� ���´�.
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
				// �������� �������� ������ , 
				if(m_ParmeterBox.IP.GetMana() >= m_ParmeterBox.GetMaxMana())continue;

			    id=Map.GetIDMap(m_ssX+j, m_ssY+i);
			
				// ��ȿ�ϸ� �Ʊ��̰� �������� ���� ���̶��, 
			    if(IsSee(id) && CR[id].GetPlayerNumber() == GetPlayerNumber()
				&& CR[id].m_ParmeterBox.IP.GetMana()
				&& KI[CR[id].m_cltCharStatus.GetKind()].IsKindAtb(KINDATB_GENERAL) == FALSE)
				{
					// ���� ������ �������� ���Ѵ�. 
					SHORT needmagicpower = m_ParmeterBox.GetMaxMana() - m_ParmeterBox.IP.GetMana();

					// ����� �������� ������ 
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

