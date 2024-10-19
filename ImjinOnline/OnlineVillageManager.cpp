// だ橾貲  : OnlineVillageManager.cpp
// 氬渡濠  : 檜彌戮 
// 賅萇 勒僭擊 婦葬 и棻.
#include <GSL.h>

#include "OnlineWorld.h"
#include "OnlineVillageManager.h"


// VillageSearch擊 襬模おж晦 嬪и л熱檜棻.(檜霞 鬚儀擊 嬪п憮紫 в蹂ж棻.)
INT		qsort_CompareForVillageSearchByPos(const VOID *arg1, const VOID *arg2);


//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	儅撩濠.
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
OnlineVillageManager::OnlineVillageManager()
{
	pSearchDataByPos  = NULL;
	siTotalVillageNum = 0;

	pMyOnlineWorld = NULL;
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	模資濠.
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
OnlineVillageManager::~OnlineVillageManager()
{
	Free();
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	蟾晦��.
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
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

	// 瞪羹 葆擊曖 熱蒂 橢橫螞棻.
	siTotalVillageNum = VillageParser.GetTotalVillageNum();
	pSearchDataByPos  = new VillageSearchDataByPos[siTotalVillageNum];

	// 葆擊擊 瓊晦 嬪и 贗楚蝶曖 葆擊擊 憲溥遽棻.
	for(i = 0; i < siTotalVillageNum; i++)
	{
		pVillageHeader = VillageParser.GetVillageHeaderIndex(i);

		pSearchDataByPos[i].dwKey  = ((__int64)pVillageHeader->siX << 16) | (__int64)pVillageHeader->siY | ((__int64)pVillageHeader->siMapIndex << 32);
		pSearchDataByPos[i].lpData = pVillageHeader;
	}

	// Sort и棻.
	qsort((VOID*)pSearchDataByPos, siTotalVillageNum, sizeof(VillageSearchDataByPos), qsort_CompareForVillageSearchByPos);
	
	return TRUE;
}


//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	詭賅葬 п薯.
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
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


//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	п渡 嬪纂曖 葆擊擊 橢橫螞棻.
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
VillageHeader* OnlineVillageManager::GetVillageByPos(SI32 siX, SI32 siY, SI16 MapIndex)
{
	VillageSearchDataByPos		Data;
	VillageSearchDataByPos*		pFindData;

	Data.dwKey = ((__int64)siX << 16) | (__int64)siY | ((__int64)MapIndex << 32);					
	pFindData  = (VillageSearchDataByPos*)bsearch(&Data, pSearchDataByPos, siTotalVillageNum, sizeof(VillageSearchDataByPos), qsort_CompareForVillageSearchByPos);

	if(pFindData == NULL) return 0;
	else                  return pFindData->lpData;
}


//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	п渡 顫殮曖 葆擊 頂曖 勒僭 嬪纂菟擊 陛雖堅 氈朝 掘褻羹曖 ん檣攪蒂 橢橫螞棻.
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
VillageType* OnlineVillageManager::GetVillageStructPosType(SI16 siType, SI16 StateCode)
{
	return VillageParser.GetType(siType, StateCode);
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	Village Parser蒂 橢朝棻.
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
OnlineVillageParser* OnlineVillageManager::GetVillageParser(void)
{
	return &VillageParser;
}

VillageHeader* OnlineVillageManager::GetVillageByCode(UI16 Code)
{
	return VillageParser.GetVillageHeaderCode(Code);
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	VillageSearch擊 襬模おж晦 嬪и л熱檜棻.
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
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
