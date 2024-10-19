#include <GSL.h>

#include <main.h>


#include "maindata.h"
#include <etc.h>
#include "music.h"

#include <map.h>
#include <CharOrg.h>

#define STEALEXP_MIN_EXP   50    // ����ġ ���ѱ��� �ּ� ���� 

extern	_Order 				Order[];
extern	_KindInfo			KI[];								// ĳ���� ������ ������ 
extern	_Map 				Map;
extern	_Char				CR[];								// ĳ������ ������ 
extern	_MainData			GD;									// ������ �ֵ����� 


BOOL _Char::CanStealExp()
{

	// �������� �����ϸ� , 
	if(m_ParmeterBox.IP.GetMana() < Order[ORDER_STEALEXP].Para1)return FALSE;

	// ������ �����ϴ� ĳ���;� �Ѵ�. 
	if(IsAtb2(ATB2_STEALEXP) == FALSE)
	{
		return FALSE;
	}

	return TRUE;

}

/*
BOOL _Char::CanStealExp(_NewID newid)
{

	if(CanStealEp()==FALSE)return FALSE;

	// ����ġ�� ��ġ���� ����� ���� ������� �Ѵ�. 
	if(IsSee(newid) && KI[CR[newid.ID].Kind].IsKindAtb(KINDATB_GENERAL)
	&& IsFriendByPlayerNumber(CR[newid.ID].PlayerNumber, PlayerNumber)==FALSE)
	{
		return TRUE;
	}

	return FALSE;

}
*/


void _Char::StealExp()
{
	SHORT i, j;
	SHORT id;
	SHORT min_enemy_length, min_our_length;
	SHORT min_enemy_id=0, min_our_id=0;
	SHORT length;
	SHORT needmagicpower=Order[CurrentOrder].Para1;

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

		 min_enemy_length=1000;
		 min_enemy_id=0;
		 min_our_length=1000;
		 min_our_id=0;

		 for(i=-5;i<=5;i++)
		    for(j=-5;j<=5;j++)
			{
				// �������� �����ϸ� , 
				if( m_ParmeterBox.IP.GetMana() < needmagicpower )	continue;

			    id=Map.GetIDMap(m_ssX+j, m_ssY+i);
			
				// ��ȿ�ϸ� ����� �Ӽ��� ����ٸ�, 
			    if(IsSee(id) && KI[CR[id].m_cltCharStatus.GetKind()].IsKindAtb(KINDATB_GENERAL)==TRUE)
				{ 
					
					// �� ����� �ִ��� Ȯ���Ѵ�.
					if(IsFriendToByPlayerNumber(GetPlayerNumber(), CR[id].GetPlayerNumber())==FALSE)
					{
/*						// ����ġ�� �������� �̻� �־�� �Ѵ�.
						if(GD.Player[ CR[id].PlayerNumber ].GeneralExp[CR[id].Kind]>=STEALEXP_MIN_EXP)
						{
							// �Ÿ��� ���� ����� ����� ã�´�. 
							length=max( abs(CR[id].X-X), abs(CR[id].Y-Y));
							if(length<min_enemy_length)
							{
								min_enemy_length=length;
								min_enemy_id=id;
							}
						}
						*/
 					}
					// ���� ���̸�, 
					else if(CR[id].GetPlayerNumber()==GetPlayerNumber())
					{
						// �Ÿ��� ���� ����� ����� ã�´�. 
						// �ִ� ������ �ƴϾ�� �Ѵ�.
						length=max( abs(CR[id].m_ssX-m_ssX), abs(CR[id].m_ssY-m_ssY));
						if(length<min_enemy_length)
						{
							min_our_length=length;
							min_our_id=id;
						}
					}

				}

			}

			// ������ �� ����� �ִٸ� ����ġ�� ���Ѵ´�. 
			if(min_enemy_id)
			{
				//GD.Player[ CR[min_enemy_id].PlayerNumber ].DecreaseExp(CR[min_enemy_id].Kind, CR[min_enemy_id].NewID, STEALEXP_MIN_EXP);
				CR[id].SetStealExpDraw(TRUE);

				// ������ �Ʊ� ����� �ִٸ� ������ ����ġ�� �ش�. 
			    if(min_our_id)
				{
			       //GD.Player[ PlayerNumber ].IncreaseExp(CR[min_our_id].Kind, CR[min_our_id].NewID, STEALEXP_MIN_EXP);
				   CR[id].SetStealExpDraw(TRUE);
				}

				// �������� �ٿ��ش�. 
			    m_ParmeterBox.IP.SetMana(m_ParmeterBox.IP.GetMana() - needmagicpower);
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

