// ���ϸ�  : OnlineVillageManager.cpp
// �����  : ������ 
// ��� �ǹ��� ���� �Ѵ�.
#include <GSL.h>

#include "OnlineWorld.h"
#include "OnlineVillageManager.h"


// VillageSearch�� ����Ʈ�ϱ� ���� �Լ��̴�.(���� Ž���� ���ؼ��� �ʿ��ϴ�.)
INT		qsort_CompareForVillageSearchByPos(const VOID *arg1, const VOID *arg2);


//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	������.
//��������������������������������������������������������������������������������������������������������������������������������������������������������������
OnlineVillageManager::OnlineVillageManager()
{
	pSearchDataByPos  = NULL;
	siTotalVillageNum = 0;

	pMyOnlineWorld = NULL;
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	�Ҹ���.
//��������������������������������������������������������������������������������������������������������������������������������������������������������������
OnlineVillageManager::~OnlineVillageManager()
{
	Free();
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	�ʱ�ȭ.
//��������������������������������������������������������������������������������������������������������������������������������������������������������������
BOOL OnlineVillageManager::Init(CHAR *pszVillageFileName, CHAR *pszPlacementFileName, cltOnlineWorld *pOnlineWorld)
{	
	SI32			i;
	VillageHeader	*pVillageHeader;

	pMyOnlineWorld = pOnlineWorld;	

	if(VillageParser.ReadText(pszVillageFileName, pszPlacementFileName) == FALSE)		
	{
		clGrp.Error("", pMyOnlineWorld->pOnlineText->Get(ON_TEXT_VILLAGE_PARSING_FAIL), pszVillageFileName);
		return FALSE;
	}

	// ��ü ������ ���� ���´�.
	siTotalVillageNum = VillageParser.GetTotalVillageNum();
	pSearchDataByPos  = new VillageSearchDataByPos[siTotalVillageNum];

	// ������ ã�� ���� Ŭ������ ������ �˷��ش�.
	for(i = 0; i < siTotalVillageNum; i++)
	{
		pVillageHeader = VillageParser.GetVillageHeaderIndex(i);

		pSearchDataByPos[i].dwKey  = ((__int64)pVillageHeader->siX << 16) | (__int64)pVillageHeader->siY | ((__int64)pVillageHeader->siMapIndex << 32);
		pSearchDataByPos[i].lpData = pVillageHeader;
	}

	// Sort �Ѵ�.
	qsort((VOID*)pSearchDataByPos, siTotalVillageNum, sizeof(VillageSearchDataByPos), qsort_CompareForVillageSearchByPos);
	
	return TRUE;
}


//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	�޸� ����.
//��������������������������������������������������������������������������������������������������������������������������������������������������������������
VOID OnlineVillageManager::Free()
{
	if(pSearchDataByPos)
	{
		delete[] pSearchDataByPos;
		pSearchDataByPos = NULL;
	}
	siTotalVillageNum = 0;

	VillageParser.Free();
}


//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	�ش� ��ġ�� ������ ���´�.
//��������������������������������������������������������������������������������������������������������������������������������������������������������������
VillageHeader* OnlineVillageManager::GetVillageByPos(SI32 siX, SI32 siY, SI16 MapIndex)
{
	VillageSearchDataByPos		Data;
	VillageSearchDataByPos*		pFindData;

	Data.dwKey = ((__int64)siX << 16) | (__int64)siY | ((__int64)MapIndex << 32);					
	pFindData  = (VillageSearchDataByPos*)bsearch(&Data, pSearchDataByPos, siTotalVillageNum, sizeof(VillageSearchDataByPos), qsort_CompareForVillageSearchByPos);

	if(pFindData == NULL) return 0;
	else                  return pFindData->lpData;
}


//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	�ش� Ÿ���� ���� ���� �ǹ� ��ġ���� ������ �ִ� ����ü�� �����͸� ���´�.
//��������������������������������������������������������������������������������������������������������������������������������������������������������������
VillageType* OnlineVillageManager::GetVillageStructPosType(SI16 siType, SI16 StateCode)
{
	return VillageParser.GetType(siType, StateCode);
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	Village Parser�� ��´�.
//��������������������������������������������������������������������������������������������������������������������������������������������������������������
OnlineVillageParser* OnlineVillageManager::GetVillageParser(void)
{
	return &VillageParser;
}

VillageHeader* OnlineVillageManager::GetVillageByCode(UI16 Code)
{
	return VillageParser.GetVillageHeaderCode(Code);
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	VillageSearch�� ����Ʈ�ϱ� ���� �Լ��̴�.
//��������������������������������������������������������������������������������������������������������������������������������������������������������������
INT qsort_CompareForVillageSearchByPos(const VOID *arg1, const VOID *arg2)
{
	VillageSearchDataByPos	*pVillageSearchData1;
	VillageSearchDataByPos	*pVillageSearchData2;

	pVillageSearchData1	= (VillageSearchDataByPos*)arg1;
	pVillageSearchData2	= (VillageSearchDataByPos*)arg2;
		
	if(pVillageSearchData1->dwKey < pVillageSearchData2->dwKey)
	{
		return -1;
	}
	if(pVillageSearchData1->dwKey > pVillageSearchData2->dwKey)
	{
		return 1;
	}

	return 0;
}
