/********************************************************************************************
	���ϸ�	:	OnlineMapBoothInfo.cpp
	�ۼ���	:	�� �� ��
	�����	:	�� �� ��
	�ۼ���	:	2002.**.**
********************************************************************************************/

//#include "Main.h"	// actdoll (2004/03/18 13:31) : ��� �������� �Ǵ� ���. ��¥ �ʿ������ ���� ��� ���� ����Ǿ� �ִ� �����. �̰� ������ ���� �߻��� �ٽ� �ѱ� �ٶ�
#include <GSL.h>
#include "Online.h"
#include "OnlineMegaText.h"
#include "DebugTool.h"
#include "OnlineMapBoothInfo.h"


//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	 ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������
OnlineMapBoothInfo::OnlineMapBoothInfo()
{
	m_fCreate = FALSE;

	ZeroMemory(m_MapBoothData, sizeof(SMapBoothData) * MAX_BOOTHDATA);
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	 �Ҹ���.
//��������������������������������������������������������������������������������������������������������������������������������������������������������������
OnlineMapBoothInfo::~OnlineMapBoothInfo()
{
	Free();
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	 �ʱ�ȭ
//��������������������������������������������������������������������������������������������������������������������������������������������������������������
VOID OnlineMapBoothInfo::Init()
{
	m_fCreate = TRUE;

	ZeroMemory(m_MapBoothData, sizeof(SMapBoothData) * MAX_BOOTHDATA);
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	 ����.
//��������������������������������������������������������������������������������������������������������������������������������������������������������������
VOID OnlineMapBoothInfo::Free()
{
	m_fCreate = FALSE;

	ZeroMemory(m_MapBoothData, sizeof(SMapBoothData) * MAX_BOOTHDATA);
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	 Prepare
//��������������������������������������������������������������������������������������������������������������������������������������������������������������
VOID OnlineMapBoothInfo::Prepare(SI32 siTileX, SI32 siTileY, SI32 siMapIndex, SI32 Range)
{
	SI32		TempCount;

	for(TempCount = 0; TempCount < MAX_BOOTHDATA; TempCount++)
	{
		if(m_MapBoothData[TempCount].sifCreate)
		{
			if(m_MapBoothData[TempCount].siMapIndex != siMapIndex)
			{
				m_MapBoothData[TempCount].sifCreate = FALSE;
			}
			else if(m_MapBoothData[TempCount].siTileX < (siTileX - Range))
			{
				m_MapBoothData[TempCount].sifCreate = FALSE;
			}
			else if(m_MapBoothData[TempCount].siTileX > (siTileX + Range))
			{
				m_MapBoothData[TempCount].sifCreate = FALSE;
			}
			else if(m_MapBoothData[TempCount].siTileY < (siTileY - Range))
			{
				m_MapBoothData[TempCount].sifCreate = FALSE;
			}
			else if(m_MapBoothData[TempCount].siTileY > (siTileY + Range))
			{
				m_MapBoothData[TempCount].sifCreate = FALSE;
			}
		}
	}
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	 Booth Data ��� �ʱ�ȭ
//��������������������������������������������������������������������������������������������������������������������������������������������������������������
VOID OnlineMapBoothInfo::Clear(void)
{
	ZeroMemory(m_MapBoothData, sizeof(SMapBoothData) * MAX_BOOTHDATA);
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	 Booth Data �� �����Ѵ�.
//��������������������������������������������������������������������������������������������������������������������������������������������������������������
VOID OnlineMapBoothInfo::AddBoothData(UI16 uiBoothID, SI32 siTileX, SI32 siTileY, SI32 siMapIndex)
{
	SI32		TempCount;

	for(TempCount = 0; TempCount < MAX_BOOTHDATA; TempCount++)
	{
		if(m_MapBoothData[TempCount].sifCreate == FALSE)
		{
			ZeroMemory(&m_MapBoothData[TempCount], sizeof(SMapBoothDescriptData));
			m_MapBoothData[TempCount].sifCreate  = TRUE;
			m_MapBoothData[TempCount].uiBoothID  = uiBoothID;
			m_MapBoothData[TempCount].siTileX    = siTileX;
			m_MapBoothData[TempCount].siTileY    = siTileY;
			m_MapBoothData[TempCount].siMapIndex = siMapIndex;
			return;
		}
	}
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	 Booth Data �� �����.
//��������������������������������������������������������������������������������������������������������������������������������������������������������������
VOID OnlineMapBoothInfo::DelBoothData(UI16 uiBoothID)
{
	SI32		TempCount;

	for(TempCount = 0; TempCount < MAX_BOOTHDATA; TempCount++)
	{
		if(m_MapBoothData[TempCount].sifCreate == TRUE)
		{
			if(m_MapBoothData[TempCount].uiBoothID == uiBoothID)
			{
				ZeroMemory(&m_MapBoothData[TempCount], sizeof(SMapBoothDescriptData));
				return;
			}
		}
	}
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	 Booth Data �� �����Ѵ�.
//��������������������������������������������������������������������������������������������������������������������������������������������������������������
VOID OnlineMapBoothInfo::SetBoothData(UI16 uiBoothID, SI32 siTileX, SI32 siTileY, SI32 siMapIndex, OnBaseBoothItem* pOnBaseBoothItem, UI08 uiBoothItemNum)
{
	SI32		TempCount, TempCount1;

	for(TempCount = 0; TempCount < MAX_BOOTHDATA; TempCount++)
	{
		if(m_MapBoothData[TempCount].sifCreate == TRUE)
		{
			if(m_MapBoothData[TempCount].uiBoothID == uiBoothID)
			{
				m_MapBoothData[TempCount].sifData = TRUE;

				ZeroMemory(m_MapBoothData[TempCount].uiItemID, sizeof(UI16) * ON_MAX_BOOTH_ITEM);
				for(TempCount1 = 0; TempCount1 < uiBoothItemNum; TempCount1++)
				{
					if(pOnBaseBoothItem[TempCount1].siType == LIST_ITEM)
						m_MapBoothData[TempCount].uiItemID[pOnBaseBoothItem[TempCount1].uiBoothSlot] = pOnBaseBoothItem[TempCount1].uiProductID;
				}
				return;
			}
		}
	}

	for(TempCount = 0; TempCount < MAX_BOOTHDATA; TempCount++)
	{
		if(m_MapBoothData[TempCount].sifCreate == FALSE)
		{
			ZeroMemory(&m_MapBoothData[TempCount], sizeof(SMapBoothDescriptData));
			m_MapBoothData[TempCount].sifCreate  = TRUE;
			m_MapBoothData[TempCount].uiBoothID  = uiBoothID;
			m_MapBoothData[TempCount].siTileX    = siTileX;
			m_MapBoothData[TempCount].siTileY    = siTileY;
			m_MapBoothData[TempCount].siMapIndex = siMapIndex;

			m_MapBoothData[TempCount].sifData = TRUE;

			ZeroMemory(m_MapBoothData[TempCount].uiItemID, sizeof(UI16) * ON_MAX_BOOTH_ITEM);
			for(TempCount1 = 0; TempCount1 < uiBoothItemNum; TempCount1++)
			{
				if(pOnBaseBoothItem[TempCount1].siType == LIST_ITEM)
					m_MapBoothData[TempCount].uiItemID[pOnBaseBoothItem[TempCount1].uiBoothSlot] = pOnBaseBoothItem[TempCount1].uiProductID;
			}
			return;
		}
	}
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	 Booth Data �� �����Ѵ�.
//��������������������������������������������������������������������������������������������������������������������������������������������������������������
SMapBoothData* OnlineMapBoothInfo::GetBoothData(UI16 uiBoothID)
{
	SI32		TempCount;

	for(TempCount = 0; TempCount < MAX_BOOTHDATA; TempCount++)
	{
		if(m_MapBoothData[TempCount].sifCreate == TRUE)
		{
			if(m_MapBoothData[TempCount].uiBoothID == uiBoothID)
			{
				return &m_MapBoothData[TempCount];
			}
		}
	}

	return NULL;
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	 Booth Data �� Descript ����
//��������������������������������������������������������������������������������������������������������������������������������������������������������������
VOID OnlineMapBoothInfo::AddBoothDataDescript(UI16 uiBoothID, UI08 SlotNum)
{
	SI32		TempCount;

	for(TempCount = 0; TempCount < MAX_BOOTHDATA; TempCount++)
	{
		if(m_MapBoothData[TempCount].sifCreate == TRUE)
		{
			if(m_MapBoothData[TempCount].uiBoothID == uiBoothID)
			{
				ZeroMemory(&m_MapBoothData[TempCount].stItemDescript[SlotNum], sizeof(SMapBoothDescriptData));

				m_MapBoothData[TempCount].stItemDescript[SlotNum].sifCreate = TRUE;
				m_MapBoothData[TempCount].stItemDescript[SlotNum].sifData   = FALSE;
				return;
			}
		}
	}
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	 Booth Data �� Descript �����Ѵ�.
//��������������������������������������������������������������������������������������������������������������������������������������������������������������
VOID OnlineMapBoothInfo::DelBoothDataDescript(UI16 uiBoothID, UI08 SlotNum)
{
	SI32		TempCount;

	for(TempCount = 0; TempCount < MAX_BOOTHDATA; TempCount++)
	{
		if(m_MapBoothData[TempCount].sifCreate == TRUE)
		{
			if(m_MapBoothData[TempCount].uiBoothID == uiBoothID)
			{
				ZeroMemory(&m_MapBoothData[TempCount].stItemDescript[SlotNum], sizeof(SMapBoothDescriptData));
				return;
			}
		}
	}
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	 Booth Data �� Descript ����
//��������������������������������������������������������������������������������������������������������������������������������������������������������������
VOID OnlineMapBoothInfo::SetBoothDataDescript(UI16 uiBoothID, UI08 SlotNum, char* lpDescript)
{
	SI32		TempCount;

	for(TempCount = 0; TempCount < MAX_BOOTHDATA; TempCount++)
	{
		if(m_MapBoothData[TempCount].sifCreate == TRUE)
		{
			if(m_MapBoothData[TempCount].uiBoothID == uiBoothID)
			{
				ZeroMemory(&m_MapBoothData[TempCount].stItemDescript[SlotNum], sizeof(SMapBoothDescriptData));

				m_MapBoothData[TempCount].stItemDescript[SlotNum].sifCreate = TRUE;
				m_MapBoothData[TempCount].stItemDescript[SlotNum].sifData   = TRUE;
				strncpy((char*)m_MapBoothData[TempCount].stItemDescript[SlotNum].szItemDescript, lpDescript, ON_MAX_BOOTH_NOTE);
				return;
			}
		}
	}
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	 Booth Data �� Descript ����
//��������������������������������������������������������������������������������������������������������������������������������������������������������������
SMapBoothDescriptData* OnlineMapBoothInfo::GetBoothDataDescript(UI16 uiBoothID, UI08 SlotNum)
{
	SI32		TempCount;

	for(TempCount = 0; TempCount < MAX_BOOTHDATA; TempCount++)
	{
		if(m_MapBoothData[TempCount].sifCreate == TRUE)
		{
			if(m_MapBoothData[TempCount].uiBoothID == uiBoothID)
			{
				return &m_MapBoothData[TempCount].stItemDescript[SlotNum];
			}
		}
	}

	return NULL;
}

