#include <GSL.h>

#include <main.h>


#include "maindata.h"
#include <etc.h>
#include "music.h"

#include <map.h>
#include <CharOrg.h>

#define STEALEXP_MIN_EXP   50    // 경험치 빼앗기의 최소 단위 

extern	_Order 				Order[];
extern	_KindInfo			KI[];								// 캐릭터 종류별 데이터 
extern	_Map 				Map;
extern	_Char				CR[];								// 캐릭터의 포인터 
extern	_MainData			GD;									// 게임의 주데이터 


BOOL _Char::CanStealExp()
{

	// 마법력이 부족하면 , 
	if(m_ParmeterBox.IP.GetMana() < Order[ORDER_STEALEXP].Para1)return FALSE;

	// 마법을 구사하는 캐릭터야 한다. 
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

	// 경험치를 훔치려는 대상이 적의 장수여야 한다. 
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
				// 마법력이 부족하면 , 
				if( m_ParmeterBox.IP.GetMana() < needmagicpower )	continue;

			    id=Map.GetIDMap(m_ssX+j, m_ssY+i);
			
				// 유효하며 장수의 속성을 지녔다면, 
			    if(IsSee(id) && KI[CR[id].m_cltCharStatus.GetKind()].IsKindAtb(KINDATB_GENERAL)==TRUE)
				{ 
					
					// 적 장수가 있는지 확인한다.
					if(IsFriendToByPlayerNumber(GetPlayerNumber(), CR[id].GetPlayerNumber())==FALSE)
					{
/*						// 경험치가 일정수준 이상 있어야 한다.
						if(GD.Player[ CR[id].PlayerNumber ].GeneralExp[CR[id].Kind]>=STEALEXP_MIN_EXP)
						{
							// 거리가 가장 가까운 장수를 찾는다. 
							length=max( abs(CR[id].X-X), abs(CR[id].Y-Y));
							if(length<min_enemy_length)
							{
								min_enemy_length=length;
								min_enemy_id=id;
							}
						}
						*/
 					}
					// 같은 편이면, 
					else if(CR[id].GetPlayerNumber()==GetPlayerNumber())
					{
						// 거리가 가장 가까운 장수를 찾는다. 
						// 최대 레벨이 아니어야 한다.
						length=max( abs(CR[id].m_ssX-m_ssX), abs(CR[id].m_ssY-m_ssY));
						if(length<min_enemy_length)
						{
							min_our_length=length;
							min_our_id=id;
						}
					}

				}

			}

			// 적절한 적 장수가 있다면 경험치를 빼앗는다. 
			if(min_enemy_id)
			{
				//GD.Player[ CR[min_enemy_id].PlayerNumber ].DecreaseExp(CR[min_enemy_id].Kind, CR[min_enemy_id].NewID, STEALEXP_MIN_EXP);
				CR[id].SetStealExpDraw(TRUE);

				// 적절한 아군 장수가 있다면 빼앗은 경험치를 준다. 
			    if(min_our_id)
				{
			       //GD.Player[ PlayerNumber ].IncreaseExp(CR[min_our_id].Kind, CR[min_our_id].NewID, STEALEXP_MIN_EXP);
				   CR[id].SetStealExpDraw(TRUE);
				}

				// 마법력을 줄여준다. 
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

