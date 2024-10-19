///////////////////////////////////////////////////////////////////////
//  �����̵� ���� �Լ� 

#include <GSL.h>

#include <main.h>

#include "maindata.h"
#include <etc.h>
#include "music.h"
#include <Magic.h>

#include <map.h>
#include <CharOrg.h>

extern	_MainData			GD;									// ������ �ֵ����� 
extern	_KindInfo			KI[];								// ĳ���� ������ ������ 
extern	_Char				CR[];								// ĳ������ ������ 
extern	_Map 				Map;

#define WARP_POSITION_BUFFER 50

BOOL _Char::SetWarp()
{
	if(CanWarp() == FALSE)return FALSE;

	return TRUE;
}

// �� ĳ������ �ɷ��� �����̵� �����Ѱ�?
BOOL _Char::CanWarp()
{

	if(IsAtb2(ATB2_WARP)==FALSE)return FALSE;

	// �������� ����ؾ� �Ѵ�.
	if(m_ParmeterBox.IP.GetMana() < MAGICPOWER_WARP)return FALSE;

	return  TRUE;
}

BOOL _Char::Warp()
{

	SI32 i, j;
	SHORT x;
	SHORT y;
	SI32 length;
	SI32 listindex = 0;
	POINT list[WARP_POSITION_BUFFER];
	POINT dest;

	// ��ġ�� �ٲ۴�. 

	
	if(CanWarp() == TRUE)
	{
		// �̵��� ���� ������ ã�´�. 
		for(i = -6; i <= 6;i++)
			for(j = -6;j <= 6;j++)
			{
				
				length = max(abs(i), abs(j));

				x = m_ssX + j;
				y = m_ssY + i;

				if(Map.GetLevel(x,y) == Map.GetLevel(m_ssX,m_ssY))
				{
					if(length >=5)
					{
						if(IsColl(x, y) == FALSE)
						{
							list[listindex].x = x;
							list[listindex].y = y;

							if(listindex < (WARP_POSITION_BUFFER - 1))
								listindex++;
						}
					}
				}
			}

			if(listindex)
			{
				dest = list[GD.Random() % listindex];

				m_ssX	=	(short)dest.x;
				m_ssY	=	(short)dest.y;
				
				// �������� ���δ�. 
				m_ParmeterBox.IP.SetMana(m_ParmeterBox.IP.GetMana() - MAGICPOWER_WARP);
				if( m_ParmeterBox.IP.GetMana() < 0 )	m_ParmeterBox.IP.SetMana(0);
				
				// ������ ��ο� ���� ���� �ʾƾ� �Ҹ��� ����. 
				if(GD.IsInBlackFog(m_ssX, m_ssY)==FALSE)
					PushEffect(EFFECT_WARP, m_ssX, m_ssY);
				
				SetSeeSwitch(TRUE);
				
				SetID();
			}


	}

	OrderStop(m_NewID, BY_COMPUTER);

	return FALSE;
}

