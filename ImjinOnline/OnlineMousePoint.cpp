#include <GSL.h>
//#include "Main.h"		// actdoll (2004/03/18 13:31) : ��� �������� �Ǵ� ���. ��¥ �ʿ������ ���� ��� ���� ����Ǿ� �ִ� �����. �̰� ������ ���� �߻��� �ٽ� �ѱ� �ٶ�
#include "Online.h"
#include "OnlineMegaText.h"
#include "OnlineWorld.h"
#include "DebugTool.h"
#include "OnlineMousePoint.h"
#include "OnlineResource.h"

//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	 ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������
OnlineMousePoint::OnlineMousePoint()
{
	m_fCreate = FALSE;

	m_pOnlineResource = NULL;
	m_pOnlineMap = NULL;

	ZeroMemory(m_MousePointData, sizeof(SMousePointInfo) * MAX_MOUSEPOINT);
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	 �Ҹ���.
//��������������������������������������������������������������������������������������������������������������������������������������������������������������
OnlineMousePoint::~OnlineMousePoint()
{
	Free();
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	 �ʱ�ȭ
//��������������������������������������������������������������������������������������������������������������������������������������������������������������
VOID OnlineMousePoint::Init(OnlineResource *pOnlineResource, OnlineMap *pOnlineMap)
{
	m_fCreate = TRUE;
	
	m_pOnlineResource = pOnlineResource;
	m_pOnlineMap = pOnlineMap;

	ZeroMemory(m_MousePointData, sizeof(SMousePointInfo) * MAX_MOUSEPOINT);
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	 ����.
//��������������������������������������������������������������������������������������������������������������������������������������������������������������
VOID OnlineMousePoint::Free()
{
	m_fCreate = FALSE;
	
	m_pOnlineResource = NULL;
	m_pOnlineMap = NULL;
	ZeroMemory(m_MousePointData, sizeof(SMousePointInfo) * MAX_MOUSEPOINT);
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	 Mouse Point �� �����Ѵ�.
//��������������������������������������������������������������������������������������������������������������������������������������������������������������
VOID OnlineMousePoint::SetMouse(SI32 siTileX, SI32 siTileY, SI32 siMapIndex)
{
	SI32		TempCount;
	SI32		TempEmpty;
	SI32		TempEmptyTime;

	// �� ������ ã�� �߰��Ѵ�.
	TempEmpty = -1;
	for(TempCount = 0; TempCount < MAX_MOUSEPOINT; TempCount++)
	{
		if(m_MousePointData[TempCount].sifCreate == FALSE)
		{
			TempEmpty = TempCount;
			break;
		}
	}

	// ���� ������� ������ ���� ���߰��� ����
	if(TempEmpty == -1)
	{
		TempEmptyTime = -1;
		for(TempCount = 0; TempCount < MAX_MOUSEPOINT; TempCount++)
		{
			if(m_MousePointData[TempCount].sifCreate)
			{
				if(TempEmptyTime <= m_MousePointData[TempCount].siCount)
				{
					TempEmpty     = TempCount;
					TempEmptyTime = m_MousePointData[TempCount].siCount;
				}
			}
		}
	}

	// ã�� ĭ�� ����
	m_MousePointData[TempEmpty].sifCreate  = TRUE;
	m_MousePointData[TempEmpty].siTileX    = siTileX;
	m_MousePointData[TempEmpty].siTileY    = siTileY;
	m_MousePointData[TempEmpty].siMapIndex = siMapIndex;
	m_MousePointData[TempEmpty].siCount    = 0;
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	 Mouse Point ��� �ʱ�ȭ
//��������������������������������������������������������������������������������������������������������������������������������������������������������������
VOID OnlineMousePoint::Clear(void)
{
	ZeroMemory(m_MousePointData, sizeof(SMousePointInfo) * MAX_MOUSEPOINT);
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	 Mouse Point �� �׼�.
//��������������������������������������������������������������������������������������������������������������������������������������������������������������
VOID OnlineMousePoint::Action(void)
{
	SI32		TempCount;
	XSPR*		TemplpXSpr;

	TemplpXSpr = m_pOnlineResource->GetxsprGame(ON_GAMESPR_MOVEPOINT);

	for(TempCount = 0; TempCount < MAX_MOUSEPOINT; TempCount++)
	{
		if(m_MousePointData[TempCount].sifCreate)
		{
			// Frame ����
			m_MousePointData[TempCount].siCount++;

			// ���� Frame �̻��̸� ����
			if(m_MousePointData[TempCount].siCount >= (TemplpXSpr->Header.font_number * 3))
			{
				m_MousePointData[TempCount].sifCreate = FALSE;
			}
		}
	}
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	 Mouse Point �� ȭ�鿡 �׸���.
//��������������������������������������������������������������������������������������������������������������������������������������������������������������
VOID OnlineMousePoint::Draw(void)
{
	SI32		TempCount;
	SI32		siDotDrawBaseX;
	SI32		siDotDrawBaseY;
	XSPR*		TemplpXSpr;

	TemplpXSpr = m_pOnlineResource->GetxsprGame(ON_GAMESPR_MOVEPOINT);

	// ���
	for(TempCount = 0; TempCount < MAX_MOUSEPOINT; TempCount++)
	{
		if(m_MousePointData[TempCount].sifCreate)
		{
			if(m_MousePointData[TempCount].siMapIndex == m_pOnlineMap->m_MapIndex)
			{
				if(m_pOnlineMap->TransformMapXYToScreenDotCenterXY(m_MousePointData[TempCount].siTileX, m_MousePointData[TempCount].siTileY, siDotDrawBaseX, siDotDrawBaseY))
				{		
					siDotDrawBaseX = siDotDrawBaseX - (TemplpXSpr->Header.Xsize / 2) - m_pOnlineMap->GetRX();
					siDotDrawBaseY = siDotDrawBaseY - (TemplpXSpr->Header.Ysize / 2) - m_pOnlineMap->GetRY();

					clGrp.PutSpriteLightImjinCT(siDotDrawBaseX, siDotDrawBaseY, TemplpXSpr->Header.Xsize, TemplpXSpr->Header.Ysize, &TemplpXSpr->Image[TemplpXSpr->Header.Start[m_MousePointData[TempCount].siCount / 3]]);
				}		
			}
		}
	}
}

