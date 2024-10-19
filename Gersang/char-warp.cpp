///////////////////////////////////////////////////////////////////////
//  공간이동 관련 함수 

#include <GSL.h>

#include <main.h>

#include "maindata.h"
#include <etc.h>
#include "music.h"
#include <Magic.h>

#include <map.h>
#include <CharOrg.h>

extern	_MainData			GD;									// 게임의 주데이터 
extern	_KindInfo			KI[];								// 캐릭터 종류별 데이터 
extern	_Char				CR[];								// 캐릭터의 포인터 
extern	_Map 				Map;

#define WARP_POSITION_BUFFER 50

BOOL _Char::SetWarp()
{
	if(CanWarp() == FALSE)return FALSE;

	return TRUE;
}

// 이 캐릭터의 능력이 공간이동 가능한가?
BOOL _Char::CanWarp()
{

	if(IsAtb2(ATB2_WARP)==FALSE)return FALSE;

	// 마법력이 충분해야 한다.
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

	// 위치를 바꾼다. 

	
	if(CanWarp() == TRUE)
	{
		// 이동할 만한 공간을 찾는다. 
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
				
				// 마법력을 줄인다. 
				m_ParmeterBox.IP.SetMana(m_ParmeterBox.IP.GetMana() - MAGICPOWER_WARP);
				if( m_ParmeterBox.IP.GetMana() < 0 )	m_ParmeterBox.IP.SetMana(0);
				
				// 완전히 어두운 곳에 있지 않아야 소리가 난다. 
				if(GD.IsInBlackFog(m_ssX, m_ssY)==FALSE)
					PushEffect(EFFECT_WARP, m_ssX, m_ssY);
				
				SetSeeSwitch(TRUE);
				
				SetID();
			}


	}

	OrderStop(m_NewID, BY_COMPUTER);

	return FALSE;
}

