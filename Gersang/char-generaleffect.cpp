#include <GSL.h>

#include <main.h>

#include "maindata.h"
#include <etc.h>


#include <map.h>
#include <CharOrg.h>

extern	_MainData			GD;									// 게임의 주데이터 
extern	_KindInfo			KI[];								// 캐릭터 종류별 데이터 
extern	_Map 				Map;
extern	_Char				CR[];								// 캐릭터의 포인터 


void _Char::GeneralEffect()
{

	SHORT id;
	SHORT i, j;
	int apower, dpower;

	BOOL bApowerSwitch		= FALSE;
	BOOL bDpowerSwitch		= FALSE;
	BOOL bHealthSwitch		= FALSE;
	BOOL bFireResistSwitch	= FALSE;
	BOOL bHideSwitch		= FALSE;
	bool bGunSwitch			= FALSE;

	// 장수 효과가 있는 경우에만 들어온다. 
	if(m_ParmeterBox.GEffectApower || m_ParmeterBox.GEffectDpower || m_ParmeterBox.GEffectHeal)
	{
	}
	else return ;

	int level = 1;

	// 장수의 속성에 따라 다른 장수 효과를 나타낸다. 
	if(m_ParmeterBox.GEffectApower)
	{
		bApowerSwitch = TRUE;
		apower = m_ParmeterBox.GEffectApower;
	}
	if(m_ParmeterBox.GEffectDpower)
	{
		bDpowerSwitch = TRUE;
		dpower = m_ParmeterBox.GEffectDpower;
	}

	int range = 10;

	if(GD.Frame % 10)return ;

	for(i=-range;i<=range;i++)
		for(j=-range;j<=range;j++)
		{
			id=Map.GetIDMap(m_ssX+j, m_ssY+i);
			 if(id==0)
				  id=Map.GetSkyMap(m_ssX+j, m_ssY+i);
			
			// 유효하고
			// 같은 편이며, 
			if(IsSee(id) && CR[id].GetPlayerNumber()==GetPlayerNumber())
			{
				// 공격력 증대 루틴. 
				if(bApowerSwitch == TRUE 
				&& CR[id].IsAtb(ATB_ATTACK|ATB_ATTACKAIR)
				&& CR[id].m_siOnlineID!=0)
				{
					if(CR[id].m_ParmeterBox.AssistApowerByGEffect <= apower)
					{
						CR[id].m_ParmeterBox.AssistApowerByGEffect     = apower ;
						CR[id].m_ParmeterBox.AssistApowerFrame			= GD.Frame;
					}
				}

				// 방어력 증대 루틴. 
				if(bDpowerSwitch == TRUE
				&& CR[id].m_siOnlineID!=0)
				{
					// 기존의 보조 방어력이 더 세다면 , 
					if(CR[id].m_ParmeterBox.AssistDpowerByGEffect <= dpower)
					{
						CR[id].m_ParmeterBox.AssistDpowerByGEffect     = dpower;
						CR[id].m_ParmeterBox.AssistDpowerFrame			= GD.Frame;
					}
				}

/*				// 총병의 성능 개선. 
				if(bGunSwitch == TRUE
				&& CR[id].IsAtb(ATB_GENERAL)==FALSE
				&& (CR[id].IsAttackAtb(ATTACKATB_GUN) || CR[id].IsAttackAtb(ATTACKATB_STRONGGUN)) )
				{
					apower = CR[id].m_ParmeterBox.GetMaxApower() * apower_percent / 100;
					dpower = CR[id].m_ParmeterBox.GetTotalDpower()		* dpower_percent / 100;

					if(CR[id].AssistApower <= apower)
					{
						CR[id].AssistApower			=	apower ;
						CR[id].AssistApowerFrame	=	GD.Frame;
					}

					// 기존의 보조 방어력이 더 세다면 , 
					if(CR[id].AssistDpower <= dpower)
					{
						CR[id].AssistDpower			=	dpower;
						CR[id].AssistDpowerFrame	=	GD.Frame;
					}
				}
*/

/*				// 체력 회복 루틴. 
				if(bHealthSwitch == TRUE)
				{
			
					// 체력이 손상된 유닛이면 체력을 회복시켜준다. 
					// 동의보감이 편찬된 상태여야 한다. 
					if((GD.Frame % 2 == 0) 
					&& CR[id].IsAtb(ATB_PATIENT) == TRUE)
					{
						if(CR[id].m_ParmeterBox.IP.GetLife() < CR[id].m_ParmeterBox.GetMaxLife())
						{
							CR[id].IncreaseLife(3);
						}
					}
				}

*/
				// 은신  루틴. 
/*				if(bHideSwitch == TRUE)
				{
					CR[id].bAssistHideSwitch = TRUE;
					CR[id].AssistHideFrame = GD.Frame;

				}
*/

			}
		}
}




