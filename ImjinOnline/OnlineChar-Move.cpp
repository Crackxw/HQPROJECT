/************************************************************************************************
	파일명 : OnlineChar-Move.cpp
	담당자 : 이윤석

	현재 Game에서 존재하는 Player의 정보를 관리한다.
	이 부분은 Player가 이동할때 처리하는 부분
************************************************************************************************/
#include <GSL.h>
#include <Main.h>
#include <OnlineWorld.h>
#include <DebugTool.h>
#include "OnlineMyData.h"

//----------------------------------------------------------------------------------------------------------------
//	설명	:	캐릭터의 Move
//----------------------------------------------------------------------------------------------------------------
SI32	cltOnlineChar::Move()
{
	SI32	siDirection = 0;
	SI32	siNX, siNY;
	SI32	siCenterX, siCenterY, siNX_CenterX, siNY_CenterY;	
	SI16	siValue;
	SI16	siCurrentTileMoveSpeed;
	SI32	siAdvTileMoveSpeed;

	// 이동을 하지 않은 상태로 설정한다.
	bMovingSwitch	=	FALSE;

	if(m_siX == m_siDX && m_siY == m_siDY && m_siMovedPercent == 0)
		return ON_MOVE_DONE;

	siCurrentTileMoveSpeed	=	m_siMoveSpeed;

	if(m_siX == m_siMDX && m_siY == m_siMDY && m_siMovedPercent == 0)
	{
		// 새롭게 길찾기를 한다. 중간 지점을 얻어온다.		
		siValue	=	pMyOnlineWorld->pclFindPath->FindPathL(m_siX, m_siY, m_siDX, m_siDY, &m_siMX, &m_siMY, NULL, 160, 8, 1, m_siMoveableTileInfo);			

		if(siValue == -1 || ((m_siX == m_siMX) && (m_siY == m_siMY)))
		{
			// 이동을 완료 했다.			
			return ON_MOVE_DONE;		
		}
		else
		{
			// 이동해야 할 자리를 예약해둔다.
			pMyOnlineWorld->pOnlineMap->SetTileAtb(m_siMX, m_siMY, 1, 1, ON_TILE_ATB_CHAR_RESERVE, TRUE);
		}


//		else
//		{
//			// 4방향 길찾기를 해야 하는지 알아온다.
///			switch(IsNeed4DirectPathFind(m_siX, m_siY, m_siMX, m_siMY))
//			{
//			case ON_CHAR_4D_PF_NEED:
//				siValue	=	pMyOnlineWorld->pclFindPath->FindPathL(m_siX, m_siY, m_siDX, m_siDY, &m_siMX, &m_siMY, NULL, 160, 4, 1);			
//				
//				if(siValue == -1 || ((m_siX == m_siMX) && (m_siY == m_siMY)))
//					return ON_MOVE_DONE;		
//
//				break;			
//			case ON_CHAR_4D_PF_STOP:
//				// 진행할 수 없는 상태이다. 정지이다.
//				return ON_MOVE_DONE;		
//			}

			SetMovingDirection(pMyOnlineWorld->GetDirection8(m_siX, m_siY, m_siMX, m_siMY));						
//		}
	}
	
	// 현재 있는 타일에서의 값을 구한다.
	if(pMyOnlineWorld->pOnlineMap->GetRoadSpeed(m_siX, m_siY, &siAdvTileMoveSpeed) == TRUE)
	{
		siCurrentTileMoveSpeed	+=	siAdvTileMoveSpeed;
	}
	// 최소 속도는 1이다.
	if(siCurrentTileMoveSpeed <= 0)
	{
		siCurrentTileMoveSpeed = 1;
	}

	// 나중에 신발의 의한 추가속도 계산
	//////////////////////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////////////////
	//???????????????????????????????
	//////////////////////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////////////////

	// 만약 무게를 초과한 캐릭터가 있다면 속도를 1로 줄임
	if(uiUniqueID == pMyOnlineWorld->pMyData->GetMyCharUniqueID())
	{
		if(pMyOnlineWorld->pMyData->GetFollowersInterface()->IsItemWeightOverflowed())
		{
			siCurrentTileMoveSpeed = 1;
		}
	}
	else
	{
		if(MoveFlag)
		{
			siCurrentTileMoveSpeed = 1;
		}
	}

	// 이동할 다음 위치를 찾는다. ( NX, NY는 MX, MY를 가기위한 자신의 바로 옆 위치이다.)
	siNX = m_siX, siNY = m_siY;  
	
	if(m_siX < m_siMX)			// 동쪽(오른쪽)으로 가야하면 X 좌표를 증기시킨다.
	{	
		siNX++;		siDirection|= ON_EAST;
	}
	else if(m_siX > m_siMX)		// 서쪽(왼쪽)으로 가야하면  X 좌표를 감소시킨다.
	{
		siNX--;		siDirection|= ON_WEST;
	}

	if(m_siY < m_siMY)			// 남쪽(아래쪽)으로 가야하면 Y 좌표를 증가시킨다.
	{
		siNY++;		siDirection|= ON_SOUTH;
	}
	else if(m_siY > m_siMY)		// 북쪽(위쪽)으로 가야하면 Y 좌표를 감소 시킨다.
	{
		siNY--;		siDirection|= ON_NORTH;
	}	

	// 방향이 바뀌었다면 새로운 방향을 넣어준다.
	if(siDirection)
	{
		ChangeDirection(siDirection);		
	}

	if(m_siMovedPercent < 0)
	{
		m_siMovedPercent += siCurrentTileMoveSpeed;         // MoveSpeed: 나의 1 프레임당 증가 % 
		if(m_siMovedPercent > 0) 
			m_siMovedPercent=0; // Over 하면 안된다. 서야한다.
	}
	else
		m_siMovedPercent += siCurrentTileMoveSpeed;        // 밑에서 50%를 넘길수 없게 하기때문에 if가없다.


	if(m_siMovedPercent >= 50)
	{
		// -%를 만든다.       (-50% ~ 50% )
		m_siMovedPercent = m_siMovedPercent - 100;
		
		// 이동의 기준을 만든다.
		m_siMOX = m_siX;       // original x
		m_siMOY = m_siY;
		m_siMDX = siNX;      // destination x 
		m_siMDY = siNY;
		
		m_siX	= siNX;
		m_siY	= siNY;

		// 새로운 타일로 이동을 했다.
		// 현재 예약했던 자리를 없앤다.			
		pMyOnlineWorld->pOnlineMap->SetTileAtb(m_siX, m_siY, 1, 1, ON_TILE_ATB_CHAR_RESERVE, FALSE);
	}
	else if(m_siMovedPercent<=0)  	// 좌표 전환이 있은지 얼마 동안은, 
	{
	}
	else
	{
		// 이동의 기준을 만든다.
		m_siMOX = m_siX;
		m_siMOY = m_siY;
		m_siMDX = siNX;
		m_siMDY = siNY;
	}	

	m_siNX = siNX;
	m_siNY = siNY;

	// 지도상의 X, Y좌표의 중심의 좌표를 돌려준다. (dot 단위)
	pMyOnlineWorld->pOnlineMap->TransformMapXYToRealDotCenterXY(m_siMOX, m_siMOY, siCenterX,	 siCenterY);
	// 이동할 위치의 중심을 찾는다. 
	pMyOnlineWorld->pOnlineMap->TransformMapXYToRealDotCenterXY(m_siMDX, m_siMDY, siNX_CenterX, siNY_CenterY);	

	//지금 좌표와 다음 좌표간의 백분율에 해당하는 상대변위를 구한다.(RX, RY)
	if(m_siMovedPercent >= 0)
	{
		GetMoveRatio(siCenterX, siCenterY, siNX_CenterX, siNY_CenterY, m_siMovedPercent, m_siRX, m_siRY);		
	}
	else
	{
		GetMoveRatio(siNX_CenterX, siNY_CenterY, siCenterX, siCenterY, abs(m_siMovedPercent), m_siRX, m_siRY);
	}		
	
	// 실질적으로 이동을 했다.
	bMovingSwitch	=	TRUE;

	if(m_siX == m_siDX && m_siY == m_siDY && m_siMovedPercent == 0)
		return ON_MOVE_DONE;
	else
		return ON_MOVE_MOVING;
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명	:	4방향 길찾기가 필요한가?
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
BOOL	cltOnlineChar::IsNeed4DirectPathFind(SI32 siX1, SI32 siY1, SI32 siX2, SI32 siY2)
{
	BOOL	bTileBlocking1, bTileBlocking2;

	if(siX1 == siX2 || siY1 == siY2)
		return ON_CHAR_4D_PF_NO_NEED;

	if(pMyOnlineWorld->pOnlineMap->GetObjectID(siX1, siY2) != 0)
	{
		bTileBlocking1	=	TRUE;
	}
	else
	{
		bTileBlocking1	=	FALSE;		
	}

	if(pMyOnlineWorld->pOnlineMap->GetObjectID(siX2, siY1) != 0)
	{
		bTileBlocking2	=	TRUE;
	}
	else																bTileBlocking2	=	FALSE;

	if(bTileBlocking1 == TRUE && bTileBlocking2 == TRUE)
		return ON_CHAR_4D_PF_STOP;
	
	if(bTileBlocking1 == TRUE || bTileBlocking2 == TRUE)
		return ON_CHAR_4D_PF_NEED;

	return ON_CHAR_4D_PF_NO_NEED;
}

