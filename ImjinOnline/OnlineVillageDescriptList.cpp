#include <GSL.h>

#include <Main.h>
#include "OnlineVillageDescriptList.h"

//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	������.
//��������������������������������������������������������������������������������������������������������������������������������������������������������������
OnlineVillageDescriptList::OnlineVillageDescriptList()
{
	m_lpVillageDescriptData    = NULL;
	m_VillageDescriptDataCount = 0;

	m_lpVillageData    = NULL;
	m_VillageDataCount = 0;
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	�Ҹ���.
//��������������������������������������������������������������������������������������������������������������������������������������������������������������
OnlineVillageDescriptList::~OnlineVillageDescriptList()
{
	Free();
}

VOID OnlineVillageDescriptList::Initialize()
{
	ZeroMemory(m_lpVillageDescriptData, sizeof(SVillageDescriptData) * m_VillageDescriptDataCount);
	ZeroMemory(m_lpVillageData, sizeof(SVillageData) * m_VillageDataCount);
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	�ʱ�ȭ
//��������������������������������������������������������������������������������������������������������������������������������������������������������������
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

//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	����.
//��������������������������������������������������������������������������������������������������������������������������������������������������������������
VOID OnlineVillageDescriptList::Free(void)
{
	if(m_lpVillageDescriptData) delete []m_lpVillageDescriptData;

	m_lpVillageDescriptData    = NULL;
	m_VillageDescriptDataCount = 0;

	if(m_lpVillageData) delete []m_lpVillageData;

	m_lpVillageData    = NULL;
	m_VillageDataCount = 0;
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	���� ������ �߰��Ѵ�.
//��������������������������������������������������������������������������������������������������������������������������������������������������������������
BOOL OnlineVillageDescriptList::AddDescriptData(UI16 VillageCode)
{
	SI32		TempCount;
	SI32		TempEmptyCount;
	DWORD		TempLastTickCount;

	// �켱 ���� Village ID �� �ִ� ���� ã�´�.
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

	// ��ĭ�� ���� ��� �� ������ TickCount�� �˻��Ͽ� �����Ѵ�.
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

	// ��ĭ�� �����.
	ZeroMemory(&m_lpVillageDescriptData[TempEmptyCount], sizeof(SVillageDescriptData));
	m_lpVillageDescriptData[TempEmptyCount].m_uiVillageCode	= VillageCode;
	m_lpVillageDescriptData[TempEmptyCount].m_fWait			= TRUE;
	m_lpVillageDescriptData[TempEmptyCount].m_TickCount		= GetTickCount();

	return TRUE;
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	���� ������ �����Ѵ�.
//��������������������������������������������������������������������������������������������������������������������������������������������������������������
BOOL OnlineVillageDescriptList::SetDescriptData(UI16 VillageCode, char* lpText)
{
	SVillageDescriptData*		lpSVillageDescriptData;

	// �̹� ���� ID�� ������ �˻�, ������ �߰�
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

//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	���� ������ ��´�.
//��������������������������������������������������������������������������������������������������������������������������������������������������������������
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

//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	���� Data�� �߰��Ѵ�.
//��������������������������������������������������������������������������������������������������������������������������������������������������������������
BOOL OnlineVillageDescriptList::AddVillageDataData(UI16 VillageCode)
{
	SI32		TempCount;
	SI32		TempEmptyCount;
	DWORD		TempLastTickCount;

	// �켱 ���� Village ID �� �ִ� ���� ã�´�.
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

	// ��ĭ�� ���� ��� �� ������ TickCount�� �˻��Ͽ� �����Ѵ�.
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

	// ��ĭ�� �����.
	ZeroMemory(&m_lpVillageData[TempEmptyCount], sizeof(SVillageData));
	m_lpVillageData[TempEmptyCount].m_uiVillageCode = VillageCode;
	m_lpVillageData[TempEmptyCount].m_fWait			= TRUE;
	m_lpVillageData[TempEmptyCount].m_TickCount		= GetTickCount();

	return TRUE;
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	���� Data�� �����Ѵ�.
//��������������������������������������������������������������������������������������������������������������������������������������������������������������
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

//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	���� Data�� �����Ѵ�.
//��������������������������������������������������������������������������������������������������������������������������������������������������������������
BOOL OnlineVillageDescriptList::SetVillageDataData(UI16 VillageCode, SI16 VillageStateCode, char* VillageGuildName, SI32 VillageGuildFlag, VDEF vdChangedDefence, VDEF vdChangedMaxDefence, DWORD dwGuildRelation, SI32 siCurrentGarrison, SI32 siMaximumGarrison, SI16 siCounterAttackType)
{
	SVillageData*				lpVillageData;

	// �̹� ���� ID�� ������ �˻�, ������ �߰�
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
	lpVillageData->m_siCurrentGarrison   = siCurrentGarrison;		// ���� ���� ���� ��(�δ�� * 100)
	lpVillageData->m_siMaximumGarrison 	 = siMaximumGarrison;		// �ִ� ���� ���� ��(�δ�� * 100)
	lpVillageData->m_ssCounterAttackType = siCounterAttackType;		// ���� �ݰ� ���� Ÿ��

	return TRUE;
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	���� Data�� ��´�.
//��������������������������������������������������������������������������������������������������������������������������������������������������������������
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

//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	Ư�� ������ ������ ��� Data�� �ʱ�ȭ �Ѵ�.
//��������������������������������������������������������������������������������������������������������������������������������������������������������������
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

