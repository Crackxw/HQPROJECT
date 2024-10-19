/************************************************************************************************
	���ϸ� : OnlineChar-Move.cpp
	����� : ������

	���� Game���� �����ϴ� Player�� ������ �����Ѵ�.
	�� �κ��� Player�� �̵��Ҷ� ó���ϴ� �κ�
************************************************************************************************/
#include <GSL.h>
#include <Main.h>
#include <OnlineWorld.h>
#include <DebugTool.h>
#include "OnlineMyData.h"

//----------------------------------------------------------------------------------------------------------------
//	����	:	ĳ������ Move
//----------------------------------------------------------------------------------------------------------------
SI32	cltOnlineChar::Move()
{
	SI32	siDirection = 0;
	SI32	siNX, siNY;
	SI32	siCenterX, siCenterY, siNX_CenterX, siNY_CenterY;	
	SI16	siValue;
	SI16	siCurrentTileMoveSpeed;
	SI32	siAdvTileMoveSpeed;

	// �̵��� ���� ���� ���·� �����Ѵ�.
	bMovingSwitch	=	FALSE;

	if(m_siX == m_siDX && m_siY == m_siDY && m_siMovedPercent == 0)
		return ON_MOVE_DONE;

	siCurrentTileMoveSpeed	=	m_siMoveSpeed;

	if(m_siX == m_siMDX && m_siY == m_siMDY && m_siMovedPercent == 0)
	{
		// ���Ӱ� ��ã�⸦ �Ѵ�. �߰� ������ ���´�.		
		siValue	=	pMyOnlineWorld->pclFindPath->FindPathL(m_siX, m_siY, m_siDX, m_siDY, &m_siMX, &m_siMY, NULL, 160, 8, 1, m_siMoveableTileInfo);			

		if(siValue == -1 || ((m_siX == m_siMX) && (m_siY == m_siMY)))
		{
			// �̵��� �Ϸ� �ߴ�.			
			return ON_MOVE_DONE;		
		}
		else
		{
			// �̵��ؾ� �� �ڸ��� �����صд�.
			pMyOnlineWorld->pOnlineMap->SetTileAtb(m_siMX, m_siMY, 1, 1, ON_TILE_ATB_CHAR_RESERVE, TRUE);
		}


//		else
//		{
//			// 4���� ��ã�⸦ �ؾ� �ϴ��� �˾ƿ´�.
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
//				// ������ �� ���� �����̴�. �����̴�.
//				return ON_MOVE_DONE;		
//			}

			SetMovingDirection(pMyOnlineWorld->GetDirection8(m_siX, m_siY, m_siMX, m_siMY));						
//		}
	}
	
	// ���� �ִ� Ÿ�Ͽ����� ���� ���Ѵ�.
	if(pMyOnlineWorld->pOnlineMap->GetRoadSpeed(m_siX, m_siY, &siAdvTileMoveSpeed) == TRUE)
	{
		siCurrentTileMoveSpeed	+=	siAdvTileMoveSpeed;
	}
	// �ּ� �ӵ��� 1�̴�.
	if(siCurrentTileMoveSpeed <= 0)
	{
		siCurrentTileMoveSpeed = 1;
	}

	// ���߿� �Ź��� ���� �߰��ӵ� ���
	//////////////////////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////////////////
	//???????????????????????????????
	//////////////////////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////////////////

	// ���� ���Ը� �ʰ��� ĳ���Ͱ� �ִٸ� �ӵ��� 1�� ����
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

	// �̵��� ���� ��ġ�� ã�´�. ( NX, NY�� MX, MY�� �������� �ڽ��� �ٷ� �� ��ġ�̴�.)
	siNX = m_siX, siNY = m_siY;  
	
	if(m_siX < m_siMX)			// ����(������)���� �����ϸ� X ��ǥ�� �����Ų��.
	{	
		siNX++;		siDirection|= ON_EAST;
	}
	else if(m_siX > m_siMX)		// ����(����)���� �����ϸ�  X ��ǥ�� ���ҽ�Ų��.
	{
		siNX--;		siDirection|= ON_WEST;
	}

	if(m_siY < m_siMY)			// ����(�Ʒ���)���� �����ϸ� Y ��ǥ�� ������Ų��.
	{
		siNY++;		siDirection|= ON_SOUTH;
	}
	else if(m_siY > m_siMY)		// ����(����)���� �����ϸ� Y ��ǥ�� ���� ��Ų��.
	{
		siNY--;		siDirection|= ON_NORTH;
	}	

	// ������ �ٲ���ٸ� ���ο� ������ �־��ش�.
	if(siDirection)
	{
		ChangeDirection(siDirection);		
	}

	if(m_siMovedPercent < 0)
	{
		m_siMovedPercent += siCurrentTileMoveSpeed;         // MoveSpeed: ���� 1 �����Ӵ� ���� % 
		if(m_siMovedPercent > 0) 
			m_siMovedPercent=0; // Over �ϸ� �ȵȴ�. �����Ѵ�.
	}
	else
		m_siMovedPercent += siCurrentTileMoveSpeed;        // �ؿ��� 50%�� �ѱ�� ���� �ϱ⶧���� if������.


	if(m_siMovedPercent >= 50)
	{
		// -%�� �����.       (-50% ~ 50% )
		m_siMovedPercent = m_siMovedPercent - 100;
		
		// �̵��� ������ �����.
		m_siMOX = m_siX;       // original x
		m_siMOY = m_siY;
		m_siMDX = siNX;      // destination x 
		m_siMDY = siNY;
		
		m_siX	= siNX;
		m_siY	= siNY;

		// ���ο� Ÿ�Ϸ� �̵��� �ߴ�.
		// ���� �����ߴ� �ڸ��� ���ش�.			
		pMyOnlineWorld->pOnlineMap->SetTileAtb(m_siX, m_siY, 1, 1, ON_TILE_ATB_CHAR_RESERVE, FALSE);
	}
	else if(m_siMovedPercent<=0)  	// ��ǥ ��ȯ�� ������ �� ������, 
	{
	}
	else
	{
		// �̵��� ������ �����.
		m_siMOX = m_siX;
		m_siMOY = m_siY;
		m_siMDX = siNX;
		m_siMDY = siNY;
	}	

	m_siNX = siNX;
	m_siNY = siNY;

	// �������� X, Y��ǥ�� �߽��� ��ǥ�� �����ش�. (dot ����)
	pMyOnlineWorld->pOnlineMap->TransformMapXYToRealDotCenterXY(m_siMOX, m_siMOY, siCenterX,	 siCenterY);
	// �̵��� ��ġ�� �߽��� ã�´�. 
	pMyOnlineWorld->pOnlineMap->TransformMapXYToRealDotCenterXY(m_siMDX, m_siMDY, siNX_CenterX, siNY_CenterY);	

	//���� ��ǥ�� ���� ��ǥ���� ������� �ش��ϴ� ��뺯���� ���Ѵ�.(RX, RY)
	if(m_siMovedPercent >= 0)
	{
		GetMoveRatio(siCenterX, siCenterY, siNX_CenterX, siNY_CenterY, m_siMovedPercent, m_siRX, m_siRY);		
	}
	else
	{
		GetMoveRatio(siNX_CenterX, siNY_CenterY, siCenterX, siCenterY, abs(m_siMovedPercent), m_siRX, m_siRY);
	}		
	
	// ���������� �̵��� �ߴ�.
	bMovingSwitch	=	TRUE;

	if(m_siX == m_siDX && m_siY == m_siDY && m_siMovedPercent == 0)
		return ON_MOVE_DONE;
	else
		return ON_MOVE_MOVING;
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	4���� ��ã�Ⱑ �ʿ��Ѱ�?
//��������������������������������������������������������������������������������������������������������������������������������������������������������������
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

