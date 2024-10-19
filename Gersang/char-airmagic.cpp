#include <GSL.h>

#include <main.h>

#include "maindata.h"
#include "music.h"
#include <KindInfo.h>
#include <CharOrg.h>

#include <MapInterface.h>
#include <bullet.h>

extern	_MainData			GD;									// ������ �ֵ����� 
extern	_KindInfo			KI[];								// ĳ���� ������ ������ 
extern	cltMapInterface		clifMap;							// �ʰ��� �������̽� 
extern	_NewID 				ZeroID;


BOOL _Char::CanAirMagic()
{

	// ������ �����ϴ� ��� ĳ���;� �Ѵ�. 
	if(KI[m_cltCharStatus.GetKind()].IsKindAtb(KINDATB_AIRMAGIC) == FALSE)	return FALSE;


	return TRUE;

}

void _Char::AirMagic()
{
	
	SI32 siFrameNumber;
	SI32 siInstant;
	SI32 sidelay;

	SHORT i, j;

	if( MagicCharAnimationIndex )
	{
		m_cltCharDrawInfo.SetAnimation(ANI_MAGIC);
		siFrameNumber	= KI[m_cltCharStatus.GetKind()].m_clCharAnimation[MagicCharAnimationIndex].GetFrameNumber();
		siInstant		= KI[m_cltCharStatus.GetKind()].m_clCharAnimation[MagicCharAnimationIndex].siInstant;
		sidelay			= KI[m_cltCharStatus.GetKind()].m_clCharAnimation[MagicCharAnimationIndex].siDelay;
	}
	else
	{
		m_cltCharDrawInfo.SetAnimation(ANI_ATTACK);
		siFrameNumber	= KI[m_cltCharStatus.GetKind()].m_clCharAnimation[AttackCharAnimationIndex].GetFrameNumber();
		siInstant		= KI[m_cltCharStatus.GetKind()].m_clCharAnimation[AttackCharAnimationIndex].siInstant;
		sidelay			= KI[m_cltCharStatus.GetKind()].m_clCharAnimation[AttackCharAnimationIndex].siDelay;
	}

	m_scBusySwitch=TRUE;

	m_cltCharStatus.SetWorkDelayor(m_cltCharStatus.GetWorkDelayor() + 1);
	if(m_cltCharStatus.GetWorkDelayor() < sidelay){ return ;}
	m_cltCharStatus.SetWorkDelayor(0);

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

	
	if(Step==siInstant)
	{
		
		
		if( m_ParmeterBox.IP.GetMana() < 0 )	m_ParmeterBox.IP.SetMana(0);
		
		// �ֺ� 8�������� �̻����� �߻��Ѵ�. 
		for(i=-3;i<=3;i+=3)
			for(j=-3;j<=3;j+=3)
			{
				if(j==0 && i==0)continue;
				if(clifMap.IsInMapArea(m_ssX+j, m_ssY+i)==FALSE)continue;
				
				SHORT bullet_id = FindEmptyBullet();
				if(bullet_id)
				{
					int centerx, centery;
					
					if(clifMap.GetRealDotCenterXY(m_ssX+j, m_ssY+i, centerx, centery))
					{
						SHORT apower=m_ParmeterBox.CalcApower() * 3 ;//+(40 * GD.Player[PlayerNumber].GeneralLevel[Kind]);
						
						// ������ ��ο� ���� ���� �ʾƾ� �Ҹ��� ����. 
						if(GD.IsInBlackFog(m_ssX, m_ssY)==FALSE)
							PushEffect(EFFECT_AIRMAGIC, m_ssX, m_ssY);
						
						SetBullet(KIND_BULLET_AIRMAGIC, bullet_id, m_ssX, m_ssY, m_ssX+j, m_ssY+i, 
							m_cltCharStatus.GetCenterX(), 
							m_cltCharStatus.GetCenterY(), 
							centerx, centery, m_NewID, ZeroID, apower, m_ParmeterBox.GetHitRate(0), 0, AREA_AIR);
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

