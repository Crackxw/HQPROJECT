

#include <GSL.h>
#include <main.h>

#include <findpath5.h>
#include "maindata.h"
#include <etc.h>


//#include <OnlineWorld.h>
#include <map.h>
#include <mapinterface.h>
#include <charinterface.h>
#include <KindInfo.h>
#include <CharOrg.h>

#include <magic.h>

extern	_MainData			GD;									// 게임의 주데이터 
extern	_Map 				Map;
extern	_KindInfo			KI[];								// 캐릭터 종류별 데이터 
extern	cltMapInterface		clifMap;						    // 맵 관련 인터페이스 
extern	cltCharInterface	clifChar;						    // 캐릭터 관련 인터페이스 

BOOL _Char::CanMine()
{
	// 지뢰를 매설할 수 있는 속성이 있어야 한다.
	if( IsAtb3(ATB3_MINE) == FALSE )	return FALSE;
	// 마나가 충분하면
	else if(m_ParmeterBox.IP.GetMana() < MAGICPOWER_MINE)	return FALSE;

	
	return TRUE;
}

// 지뢰를 심을 수 있는가?
BOOL _Char::CanMine(SHORT x, SHORT y)
{
	
	// 지도 영역 밖이면, 
	if(Map.IsInMapArea(x, y)==FALSE)return FALSE;
	
	if(CanMine()==FALSE)return FALSE;

	if(Map.GetMineMap(x, y) >= 0)return FALSE;
	
	return TRUE;
}

BOOL _Char::SetMine(SHORT x, SHORT y)
{
	
	if(CanMine(x, y)==FALSE)return FALSE;
	
	WorkStep=WS_DECIDE_LENGTH_TO_DEST;
    AttackGiveUpDelayor=0;
	
	return TRUE;
}

BOOL _Char::Mine()
{
	SHORT status;
	SHORT length=0;
	
	// 목표로 삼는 지역
	SHORT x=m_ssX;
	SHORT y=m_ssY;
	
	
	switch(WorkStep)
	{
	case WS_DECIDE_LENGTH_TO_DEST:    // 목표까지의 거리를 결정한다. 
		
		if(CanMine(x, y)==FALSE)
		{   
			m_scBusySwitch=FALSE;
			OrderStop(m_NewID, BY_COMPUTER);
			return FALSE;
		}
		
		// 매설할 지역 위에 있으면, 
		if(m_ssX==x && m_ssY==y)
		{
			WorkStep = WS_MINE;
		}
		// 매설 가능지역에 있으면, 
		else if(abs(m_ssX - x)<=1 && abs(m_ssY - y)<=1)
		{
			Direction = DecideDirection8(m_ssX, m_ssY, x, y);
			WorkStep  = WS_MINE;
		}
		else
		{
			//WorkStep = WS_GOTO_TARGET;
			WorkStep  = WS_MINE;
			SetMove(x, y);
			
		}
		break;
		
		
	case WS_GOTO_TARGET:
		
		status=Move(BASIC_MOVE_BLOCK_COUNT, 1);
		if(MoveDone())
			m_scBusySwitch=FALSE;
		else
		{
			AttackGiveUpDelayor=0;
			m_scBusySwitch=TRUE;
		}
		
		if(m_scBusySwitch==FALSE)
		{
			if(CanMine(x, y)==FALSE)
			{  
				OrderStop(m_NewID, BY_COMPUTER);
				return FALSE;
			}
			
			// 목적지에 도착하거나 중간 지점에 도착하면, 
			if(status>0)
			{
				
				WorkStep=WS_DECIDE_LENGTH_TO_DEST;
				
				AttackGiveUpDelayor++;
				if(AttackGiveUpDelayor>AttackGiveUpDelay)
				{
					OrderStop(m_NewID, BY_COMPUTER);
				}
			}
			
		}
		
		break;
		
	case WS_MINE:
		
		if(CanMine(x, y)==FALSE)
		{
			m_scBusySwitch=FALSE;
			OrderStop(m_NewID, BY_COMPUTER);
			return FALSE;
		}
		if(m_ParmeterBox.IP.GetMana() >= MAGICPOWER_MINE)
			MineOperation(x, y);
		
		break;
	}
	
	return FALSE;
}

BOOL _Char::MineOperation(SHORT x, SHORT y)
{

	m_cltCharDrawInfo.SetAnimation(ANI_MAGIC);
	
	m_cltCharStatus.SetWorkDelayor(m_cltCharStatus.GetWorkDelayor() + 1);
	if(m_cltCharStatus.GetWorkDelayor() < KI[m_cltCharStatus.GetKind()].m_clCharAnimation[MagicCharAnimationIndex].siDelay){ return FALSE;}
	m_cltCharStatus.SetWorkDelayor(0);
	
	
	if(KI[m_cltCharStatus.GetKind()].m_clCharAnimation[MagicCharAnimationIndex].GetFrameNumber()==0)
	{
		Step=0;
		m_cltCharDrawInfo.SetAniStep(Step);
	}
	else
	{
		++Step %= KI[m_cltCharStatus.GetKind()].m_clCharAnimation[MagicCharAnimationIndex].GetFrameNumber();
		m_cltCharDrawInfo.SetAniStep(Step);
		m_cltCharDrawInfo.SetUpdateAnimationSwitch(TRUE);
	}
	
		
	
	if(Step == KI[m_cltCharStatus.GetKind()].m_clCharAnimation[MagicCharAnimationIndex].siInstant)
	{
		if(CanMine(x, y)==FALSE)
		{    
			m_scBusySwitch=FALSE;
			OrderStop(m_NewID, BY_COMPUTER);
			return FALSE;
		}
		
		// 지뢰를 심는다. 
		Map.SetMineMap(x, y, GD.Player[GetPlayerNumber()].PI.PlayerNumber);
		// 마나 소비.
		ReduceMana(MAGICPOWER_MINE);

		
		
		OrderStop(m_NewID, BY_COMPUTER);
	}
	
	if(Step==0)
	{
		m_scBusySwitch=FALSE;
		return TRUE;
	}
	else return FALSE;
	
}

// 지뢰를 매설할 수 있는 장소를 찾는다.
BOOL _Char::SearchForMineSpot(SI32 *px, SI32 *py)
{
	SI32	x=-1, y=-1;
	SI32	max_count=0;
//	SI32	count;
//	SI32	i, j, k;

/*	for(i=-2;i<=2;i++)
		for(j=-2;j<=2;j++)
		{
			if(i==0 && j==0)continue;

			if(clifMap.IsEmpty(X+j, Y+i)==TRUE)
			{
				// 적이 다닌 횟수를 구한다. 
				count=0;
				for(k=0;k<MAX_MULTI_PLAYER;k++)
				{
					if(GD.IsFriendTo(GetPlayerNumber(), k) == FALSE)
					{
						count+=GD.PlayerMoveRecordMap[k][X+j][Y+i];
					}
				}

				// 적이 가장 많이 다닌 곳을 찾는다.
				if(count>max_count)
				{
					max_count=count;
					x=X+j;
					y=Y+i;
				}
			}

		}
*/
	
	// 지뢰를 심을 만한 곳을 찾았다.
	if(x>=0 && y>=0)
	{
		*px	= x;
		*py	= y;
		return TRUE;
	}

	return FALSE;

}