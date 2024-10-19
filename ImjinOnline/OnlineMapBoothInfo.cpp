/********************************************************************************************
	だ橾貲	:	OnlineMapBoothInfo.cpp
	濛撩濠	:	檜 彌 戮
	氬渡濠	:	檜 遽 戮
	濛撩橾	:	2002.**.**
********************************************************************************************/

//#include "Main.h"	// actdoll (2004/03/18 13:31) : 橈橫紫 闡だ橾檜 腎朝 ④渦. 霞瞼 в蹂橈湍陛 鼻嬪 ④渦 頂縑 摹樹腎橫 氈朝 唳辦歜. 檜匙 陽僥縑 僥薯 嫦儅衛 棻衛 麵晦 夥塋
#include <GSL.h>
#include "Online.h"
#include "OnlineMegaText.h"
#include "DebugTool.h"
#include "OnlineMapBoothInfo.h"


//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	 儅撩濠
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
OnlineMapBoothInfo::OnlineMapBoothInfo()
{
	m_fCreate = FALSE;

	ZeroMemory(m_MapBoothData, sizeof(SMapBoothData) * MAX_BOOTHDATA);
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	 模資濠.
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
OnlineMapBoothInfo::~OnlineMapBoothInfo()
{
	Free();
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	 蟾晦��
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
VOID OnlineMapBoothInfo::Init()
{
	m_fCreate = TRUE;

	ZeroMemory(m_MapBoothData, sizeof(SMapBoothData) * MAX_BOOTHDATA);
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	 п薯.
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
VOID OnlineMapBoothInfo::Free()
{
	m_fCreate = FALSE;

	ZeroMemory(m_MapBoothData, sizeof(SMapBoothData) * MAX_BOOTHDATA);
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	 Prepare
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
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

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	 Booth Data 賅舒 蟾晦��
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
VOID OnlineMapBoothInfo::Clear(void)
{
	ZeroMemory(m_MapBoothData, sizeof(SMapBoothData) * MAX_BOOTHDATA);
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	 Booth Data 蒂 撲薑и棻.
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
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

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	 Booth Data 蒂 雖遴棻.
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
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

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	 Booth Data 蒂 撲薑и棻.
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
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

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	 Booth Data 蒂 撲薑и棻.
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
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

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	 Booth Data 縑 Descript 撲薑
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
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

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	 Booth Data 縑 Descript 餉薯и棻.
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
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

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	 Booth Data 縑 Descript 撲薑
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
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

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	 Booth Data 縑 Descript 撲薑
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
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

