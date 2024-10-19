#include <GSL.h>

#include <Main.h>
#include "OnlineVillageDescriptList.h"

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	儅撩濠.
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
OnlineVillageDescriptList::OnlineVillageDescriptList()
{
	m_lpVillageDescriptData    = NULL;
	m_VillageDescriptDataCount = 0;

	m_lpVillageData    = NULL;
	m_VillageDataCount = 0;
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	模資濠.
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
OnlineVillageDescriptList::~OnlineVillageDescriptList()
{
	Free();
}

VOID OnlineVillageDescriptList::Initialize()
{
	ZeroMemory(m_lpVillageDescriptData, sizeof(SVillageDescriptData) * m_VillageDescriptDataCount);
	ZeroMemory(m_lpVillageData, sizeof(SVillageData) * m_VillageDataCount);
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	蟾晦��
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
BOOL OnlineVillageDescriptList::Init(SI32 MaxDescriptCount, SI32 MaxVillageDataCount)
{
	SI32		TempCount;

	m_lpVillageDescriptData    = new SVillageDescriptData[MaxDescriptCount];
	m_VillageDescriptDataCount = MaxDescriptCount;

	for(TempCount = 0; TempCount < m_VillageDescriptDataCount; TempCount++)
	{
		m_lpVillageDescriptData[TempCount].m_uiVillageCode = 0;
	}

	m_lpVillageData    = new SVillageData[MaxVillageDataCount];
	m_VillageDataCount = MaxVillageDataCount;

	for(TempCount = 0; TempCount < m_VillageDataCount; TempCount++)
	{
		m_lpVillageData[TempCount].m_uiVillageCode = 0;
	}

	return TRUE;
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	п薯.
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
VOID OnlineVillageDescriptList::Free(void)
{
	if(m_lpVillageDescriptData) delete []m_lpVillageDescriptData;

	m_lpVillageDescriptData    = NULL;
	m_VillageDescriptDataCount = 0;

	if(m_lpVillageData) delete []m_lpVillageData;

	m_lpVillageData    = NULL;
	m_VillageDataCount = 0;
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	葆擊 撲貲擊 蹺陛и棻.
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
BOOL OnlineVillageDescriptList::AddDescriptData(UI16 VillageCode)
{
	SI32		TempCount;
	SI32		TempEmptyCount;
	DWORD		TempLastTickCount;

	// 辦摹 ⑷營 Village ID 陛 氈朝 夠擊 瓊朝棻.
	TempEmptyCount = -1;
	for(TempCount = 0; TempCount < m_VillageDescriptDataCount; TempCount++)
	{
		if(m_lpVillageDescriptData[TempCount].m_uiVillageCode == 0)
		{
			TempEmptyCount = TempCount;
		}
		else
		{
			if(m_lpVillageDescriptData[TempCount].m_uiVillageCode == VillageCode)
			{
				m_lpVillageDescriptData[TempCount].m_TickCount = GetTickCount();
				return TRUE;
			}
		}
	}

	// 綴蘊檜 橈擊 唳辦 薨 螃楚脹 TickCount蒂 匐儀ж罹 餉薯и棻.
	if(TempEmptyCount == -1)
	{
		TempLastTickCount = 0xffffffff;
		for(TempCount = 0; TempCount < m_VillageDescriptDataCount; TempCount++)
		{
			if(m_lpVillageDescriptData[TempCount].m_uiVillageCode == 0)
			{
				TempEmptyCount = TempCount;
				break;
			}
			else
			{
				if(m_lpVillageDescriptData[TempCount].m_TickCount < TempLastTickCount)
				{
					TempEmptyCount    = TempCount;
					TempLastTickCount = m_lpVillageDescriptData[TempCount].m_TickCount;
				}
			}
		}
	}

	// 綴蘊擊 虜萇棻.
	ZeroMemory(&m_lpVillageDescriptData[TempEmptyCount], sizeof(SVillageDescriptData));
	m_lpVillageDescriptData[TempEmptyCount].m_uiVillageCode	= VillageCode;
	m_lpVillageDescriptData[TempEmptyCount].m_fWait			= TRUE;
	m_lpVillageDescriptData[TempEmptyCount].m_TickCount		= GetTickCount();

	return TRUE;
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	葆擊 撲貲擊 雖薑и棻.
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
BOOL OnlineVillageDescriptList::SetDescriptData(UI16 VillageCode, char* lpText)
{
	SVillageDescriptData*		lpSVillageDescriptData;

	// 檜嘐 葆擊 ID陛 氈戲賊 匐儀, 橈戲賊 蹺陛
	lpSVillageDescriptData = GetDescriptData(VillageCode);
	if(lpSVillageDescriptData == NULL)
	{
		AddDescriptData(VillageCode);
		lpSVillageDescriptData = GetDescriptData(VillageCode);
	}

	if(lpSVillageDescriptData == NULL) return FALSE;

	lpSVillageDescriptData->m_TickCount = GetTickCount();
	lpSVillageDescriptData->m_fWait     = FALSE;
	strcpy((char*)lpSVillageDescriptData->m_DescriptText, lpText);

	return TRUE;
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	葆擊 撲貲擊 橢朝棻.
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
SVillageDescriptData* OnlineVillageDescriptList::GetDescriptData(UI16 VillageCode)
{
	SI32		TempCount;

	for(TempCount = 0; TempCount < m_VillageDescriptDataCount; TempCount++)
	{
		if(m_lpVillageDescriptData[TempCount].m_uiVillageCode == VillageCode)
		{
			return &m_lpVillageDescriptData[TempCount];
		}
	}

	return NULL;
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	葆擊 Data擊 蹺陛и棻.
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
BOOL OnlineVillageDescriptList::AddVillageDataData(UI16 VillageCode)
{
	SI32		TempCount;
	SI32		TempEmptyCount;
	DWORD		TempLastTickCount;

	// 辦摹 ⑷營 Village ID 陛 氈朝 夠擊 瓊朝棻.
	TempEmptyCount = -1;
	for(TempCount = 0; TempCount < m_VillageDataCount; TempCount++)
	{
		if(m_lpVillageData[TempCount].m_uiVillageCode == 0)
		{
			TempEmptyCount = TempCount;
		}
		else
		{
			if(m_lpVillageData[TempCount].m_uiVillageCode == VillageCode)
			{
				m_lpVillageData[TempCount].m_TickCount = GetTickCount();
				return TRUE;
			}
		}
	}

	// 綴蘊檜 橈擊 唳辦 薨 螃楚脹 TickCount蒂 匐儀ж罹 餉薯и棻.
	if(TempEmptyCount == -1)
	{
		TempLastTickCount = 0xffffffff;
		for(TempCount = 0; TempCount < m_VillageDataCount; TempCount++)
		{
			if(m_lpVillageData[TempCount].m_uiVillageCode == 0)
			{
				TempEmptyCount = TempCount;
				break;
			}
			else
			{
				if(m_lpVillageData[TempCount].m_TickCount < TempLastTickCount)
				{
					TempEmptyCount    = TempCount;
					TempLastTickCount = m_lpVillageData[TempCount].m_TickCount;
				}
			}
		}
	}

	// 綴蘊擊 虜萇棻.
	ZeroMemory(&m_lpVillageData[TempEmptyCount], sizeof(SVillageData));
	m_lpVillageData[TempEmptyCount].m_uiVillageCode = VillageCode;
	m_lpVillageData[TempEmptyCount].m_fWait			= TRUE;
	m_lpVillageData[TempEmptyCount].m_TickCount		= GetTickCount();

	return TRUE;
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	葆擊 Data擊 餉薯и棻.
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
BOOL OnlineVillageDescriptList::DelVillageDataData(UI16 VillageCode)
{
	SI32		TempCount;

	for(TempCount = 0; TempCount < m_VillageDataCount; TempCount++)
	{
		if(m_lpVillageData[TempCount].m_uiVillageCode == VillageCode)
		{
			ZeroMemory(&m_lpVillageData[TempCount], sizeof(SVillageData));
			return TRUE;
		}
	}

	return FALSE;
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	葆擊 Data擊 雖薑и棻.
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
BOOL OnlineVillageDescriptList::SetVillageDataData(UI16 VillageCode, SI16 VillageStateCode, char* VillageGuildName, SI32 VillageGuildFlag, VDEF vdChangedDefence, VDEF vdChangedMaxDefence, DWORD dwGuildRelation, SI32 siCurrentGarrison, SI32 siMaximumGarrison, SI16 siCounterAttackType)
{
	SVillageData*				lpVillageData;

	// 檜嘐 葆擊 ID陛 氈戲賊 匐儀, 橈戲賊 蹺陛
	lpVillageData = GetVillageDataData(VillageCode);
	if(lpVillageData == NULL)
	{
		AddVillageDataData(VillageCode);
		lpVillageData = GetVillageDataData(VillageCode);
	}

	if(lpVillageData == NULL) return FALSE;

	lpVillageData->m_TickCount = GetTickCount();
	lpVillageData->m_fWait     = FALSE;

	lpVillageData->m_VillageStateCode = VillageStateCode;
	ZeroMemory(lpVillageData->m_VillageGuildName, sizeof(lpVillageData->m_VillageGuildName));
	if(VillageGuildName)
	{
		strncpy((char*)lpVillageData->m_VillageGuildName, (char*)VillageGuildName, ON_GUILDNAME_LENGTH);
	}
	lpVillageData->m_VillageGuildFlag    = VillageGuildFlag;
	lpVillageData->m_vdChangedDefence    = vdChangedDefence;
	lpVillageData->m_vdChangedMaxDefence = vdChangedMaxDefence;
	lpVillageData->m_dwGuildRelation     = dwGuildRelation;

	// robypark 2004/11/22 11:48
	lpVillageData->m_siCurrentGarrison   = siCurrentGarrison;		// ⑷營 葆擊 熱綠煽 熱(睡渠熱 * 100)
	lpVillageData->m_siMaximumGarrison 	 = siMaximumGarrison;		// 譆渠 葆擊 熱綠煽 熱(睡渠熱 * 100)
	lpVillageData->m_ssCounterAttackType = siCounterAttackType;		// 葆擊 奩問 奢問 顫殮

	return TRUE;
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	葆擊 Data擊 橢朝棻.
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
SVillageData* OnlineVillageDescriptList::GetVillageDataData(UI16 VillageCode)
{
	SI32		TempCount;

	for(TempCount = 0; TempCount < m_VillageDataCount; TempCount++)
	{
		if(m_lpVillageData[TempCount].m_uiVillageCode == VillageCode)
		{
			return &m_lpVillageData[TempCount];
		}
	}

	return NULL;
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	か薑 葆擊擊 薯諼и 賅萇 Data蒂 蟾晦�� и棻.
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
BOOL OnlineVillageDescriptList::ClearVillageDataData(UI16 VillageCode)
{
	SVillageData		TempData;
	BOOL				TempfFlag;
	SI32				TempCount;

	TempfFlag = FALSE;
	for(TempCount = 0; TempCount < m_VillageDataCount; TempCount++)
	{
		if(m_lpVillageData[TempCount].m_uiVillageCode == VillageCode)
		{
			TempData  = m_lpVillageData[TempCount];
			TempfFlag = TRUE;
			break;
		}
	}
	ZeroMemory(m_lpVillageData, sizeof(SVillageData) * m_VillageDataCount);
	if(TempfFlag)
	{
		m_lpVillageData[0] = TempData;
	}

	return TRUE;
}

