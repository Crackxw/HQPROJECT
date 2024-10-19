#include <GSL.h>

#include <main.h>

#include "maindata.h"
#include <etc.h>


#include <map.h>
#include <CharOrg.h>

extern	_MainData			GD;									// ������ �ֵ����� 
extern	_KindInfo			KI[];								// ĳ���� ������ ������ 
extern	_Map 				Map;
extern	_Char				CR[];								// ĳ������ ������ 


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

	// ��� ȿ���� �ִ� ��쿡�� ���´�. 
	if(m_ParmeterBox.GEffectApower || m_ParmeterBox.GEffectDpower || m_ParmeterBox.GEffectHeal)
	{
	}
	else return ;

	int level = 1;

	// ����� �Ӽ��� ���� �ٸ� ��� ȿ���� ��Ÿ����. 
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
			
			// ��ȿ�ϰ�
			// ���� ���̸�, 
			if(IsSee(id) && CR[id].GetPlayerNumber()==GetPlayerNumber())
			{
				// ���ݷ� ���� ��ƾ. 
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

				// ���� ���� ��ƾ. 
				if(bDpowerSwitch == TRUE
				&& CR[id].m_siOnlineID!=0)
				{
					// ������ ���� ������ �� ���ٸ� , 
					if(CR[id].m_ParmeterBox.AssistDpowerByGEffect <= dpower)
					{
						CR[id].m_ParmeterBox.AssistDpowerByGEffect     = dpower;
						CR[id].m_ParmeterBox.AssistDpowerFrame			= GD.Frame;
					}
				}

/*				// �Ѻ��� ���� ����. 
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

					// ������ ���� ������ �� ���ٸ� , 
					if(CR[id].AssistDpower <= dpower)
					{
						CR[id].AssistDpower			=	dpower;
						CR[id].AssistDpowerFrame	=	GD.Frame;
					}
				}
*/

/*				// ü�� ȸ�� ��ƾ. 
				if(bHealthSwitch == TRUE)
				{
			
					// ü���� �ջ�� �����̸� ü���� ȸ�������ش�. 
					// ���Ǻ����� ������ ���¿��� �Ѵ�. 
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
				// ����  ��ƾ. 
/*				if(bHideSwitch == TRUE)
				{
					CR[id].bAssistHideSwitch = TRUE;
					CR[id].AssistHideFrame = GD.Frame;

				}
*/

			}
		}
}




