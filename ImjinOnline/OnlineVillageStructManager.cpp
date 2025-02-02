#include <GSL.h>

#include	<Main.h>
#include	"OnlineWorld.h"
#include	"OnlineVillage.h"
#include	"OnlineVillageStructManager.h"
#include	"OnlineVillageStruct-Farm.h"
#include	"OnlineTradeBook.h"


//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	儅撩濠.
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
VillageStructManager::VillageStructManager()
{
	// ⑷營 摹鷗и 勒僭檜 橈棻堅 撲薑и棻.
	pStructure					=	NULL;		
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	模資濠.
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
VillageStructManager::~VillageStructManager()
{
	Free();
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	蟾晦��.
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
VOID	VillageStructManager::Init( UI16 VillageCode, SI32 psiKind[ON_MAX_VIL_STRUCTNUM], cltOnlineWorld *pOnlineWorld)
{

	// 橫雯 勒僭菟檜 氈朝雖 盪濰и棻.
	memcpy(siKinds, psiKind, sizeof(siKinds));

	pMyOnlineWorld	 =	pOnlineWorld;
	uiVillageCode	 =	VillageCode;
	siSuccess		 =	0;

	// 蟾晦��.
	ZeroMemory(&RIItems, sizeof(RIItems));
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	п薯.
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
VOID	VillageStructManager::Free()
{
	SI32	i;

	for(i = 0; i < ON_MAX_VIL_STRUCTNUM; i++)	
		siKinds[i]	=	ON_VIL_ST_KIND_UNKNOWN;	

	FreeReceiveItemList();

	// 勒僭擊 陛雖堅 氈戲賊 勒僭擊 п薯и棻.
	if(pStructure != NULL)
	{
		delete pStructure;
		pStructure	=	NULL;
	}
}


//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	п渡 勒僭 寰戲煎 菟橫除棻.
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
VOID	VillageStructManager::EnterStructure(SI32 siStructureIndex, BOOL bField)
{
	POINT		ptStartMousePos;

	if(IsEnterStructure() == TRUE)
	{
		clGrp.Error(pMyOnlineWorld->pOnlineText->Get(ON_TEXT_BE_IN_ANOTHER_STRUCTURE), "");
		return;
	}

	if( !bField )
	{
		// п渡 勒僭曖 詭賅葬蒂 й渡 嫡朝棻.
		switch(siKinds[siStructureIndex])
		{
		case ON_VIL_ST_KIND_BARRACK:
			pStructure			=	new OnlineVillageStructureBarrack;		
			break;
		case ON_VIL_ST_KIND_MARKET:
			pStructure			=	new OnlineVillageStructureMarket;		
			break;
		case ON_VIL_ST_KIND_BANK:
			pStructure			=	new OnlineVillageStructureBank;		
			break;
		case ON_VIL_ST_KIND_HOSPITAL:
			pStructure			=	new OnlineVillageStructureHospital;		
			break;
		case ON_VIL_ST_KIND_INN:
			pStructure			=	new OnlineVillageStructureInn;		
			break;
		case ON_VIL_ST_KIND_WHARF:
			pStructure			=	new OnlineVillageStructureWharf;		
			break;
		case ON_VIL_ST_KIND_FARM:
			pStructure			=	new OnlineVillageStructFarm;
			break;
		case ON_VIL_ST_KIND_GOVERNMENT:
			pStructure			=	new OnlineGovernMgr;
			break;
		case ON_VIL_ST_KIND_WSTORE:
			pStructure			=	new OnlineVillageStructureMarket;		
			break;
		case ON_VIL_ST_KIND_DSTORE:
			pStructure			=	new OnlineVillageStructureMarket;		
			break;
		case ON_VIL_ST_KIND_FSTORE:
			pStructure			=	new OnlineVillageStructureMarket;		
			break;
		case ON_VIL_ST_KIND_RING:
			pStructure			=	new OnlineVillageStructureMarket;		
			break;
		case ON_VIL_ST_KIND_WEAPON:
			pStructure			=	new OnlineVillageStructFarm;
			break;
		case ON_VIL_ST_KIND_ARMOR:
			pStructure			=	new OnlineVillageStructFarm;
			break;
		case ON_VIL_ST_KIND_PASTURE:
			pStructure			=	new OnlineVillageStructFarm;
			break;
		case ON_VIL_ST_KIND_FACTORY:
			pStructure			=	new OnlineVillageStructFarm;
			break;
		case ON_VIL_ST_KIND_GBARRACK:
			pStructure			=	new OnlineVillageStructureGBarrack;		
			break;
		case ON_VIL_ST_KIND_MBARRACK:
			pStructure			=	new OnlineVillageStructureMBarrack;		
			break;
		default:
			clGrp.Error("VillageStructManager::Init", "%s. [%d][%d]", pMyOnlineWorld->pOnlineText->Get(ON_TEXT_ERROR_NOTKNOWSTRUCT), siStructureIndex, siKinds[siStructureIndex]);
			return;
		}	

		siKind	=	siKinds[siStructureIndex];
	}
	else
	{
		switch( siStructureIndex )
		{
		case ON_VIL_ST_KIND_BARRACK:
			pStructure			=	new OnlineVillageStructureBarrack;		
			break;
		case ON_VIL_ST_KIND_MARKET:
			pStructure			=	new OnlineVillageStructureMarket;		
			break;
		case ON_VIL_ST_KIND_BANK:
			pStructure			=	new OnlineVillageStructureBank;		
			break;
		case ON_VIL_ST_KIND_HOSPITAL:
			pStructure			=	new OnlineVillageStructureHospital;		
			break;
		case ON_VIL_ST_KIND_INN:
			pStructure			=	new OnlineVillageStructureInn;		
			break;
		case ON_VIL_ST_KIND_WHARF:
			pStructure			=	new OnlineVillageStructureWharf;		
			break;
		case ON_VIL_ST_KIND_FARM:
			pStructure			=	new OnlineVillageStructFarm;
			break;
//		case ON_VIL_ST_KIND_GOVERNMENT:
//			pStructure			=	new OnlineVillageStructureGovernment;
//			break;
		case ON_VIL_ST_KIND_WSTORE:
			pStructure			=	new OnlineVillageStructureMarket;		
			break;
		case ON_VIL_ST_KIND_DSTORE:
			pStructure			=	new OnlineVillageStructureMarket;		
			break;
		case ON_VIL_ST_KIND_FSTORE:
			pStructure			=	new OnlineVillageStructureMarket;		
			break;
		case ON_VIL_ST_KIND_RING:
			pStructure			=	new OnlineVillageStructureMarket;		
			break;
		case ON_VIL_ST_KIND_WEAPON:
			pStructure			=	new OnlineVillageStructFarm;
			break;
		case ON_VIL_ST_KIND_ARMOR:
			pStructure			=	new OnlineVillageStructFarm;
			break;
		case ON_VIL_ST_KIND_PASTURE:
			pStructure			=	new OnlineVillageStructFarm;
			break;
		case ON_VIL_ST_KIND_FACTORY:
			pStructure			=	new OnlineVillageStructFarm;
			break;
		case ON_VIL_ST_KIND_GBARRACK:
			pStructure			=	new OnlineVillageStructureGBarrack;		
			break;
		case ON_VIL_ST_KIND_MBARRACK:
			pStructure			=	new OnlineVillageStructureMBarrack;		
			break;
		default:
			clGrp.Error("VillageStructManager::Init", "%s. [%d]", pMyOnlineWorld->pOnlineText->Get(ON_TEXT_ERROR_NOTKNOWSTRUCT), siKinds[siStructureIndex]);
			return;
		}

		siKind	=	siStructureIndex;
	}

	// 葆辦蝶 謝ル蒂 橢橫螞棻.
	pMyOnlineWorld->pIOnlineMouse->GetPos(&ptStartMousePos);	
	pStructure->SetStartMousePos(ptStartMousePos);
	
	
	pStructure->Init( uiVillageCode, pMyOnlineWorld, this );
}


//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	橫雯 勒僭寰戲煎 菟橫鬲朝雖 憲嬴螞棻.
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
BOOL	VillageStructManager::IsEnterStructure()
{
	if(pStructure != NULL)
		return	TRUE;
	else
		return	FALSE;
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	⑷營 菟橫陛 氈朝 摹僭曖 Kind蒂 橢朝棻.
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
SI32	VillageStructManager::GetEnterStructureKind(void)
{
	return siKind;
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	憮幗縑 氈朝 嬴檜蠱曖 葬蝶お蒂 嫡懊棻.
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
VOID	VillageStructManager::ReceiveItemList(SI32 siItemNum, OnlineReceivedItemInfo *pOnlineReceivedItemInfo)
{	
	if(siItemNum != 0)
	{
		FreeReceiveItemList();

		RIItems.siItemNum					=	siItemNum;
		RIItems.pOnlineReceivedItemInfo		=	new OnlineReceivedItemInfo[siItemNum];
		memcpy(RIItems.pOnlineReceivedItemInfo, pOnlineReceivedItemInfo, sizeof(OnlineReceivedItemInfo) * siItemNum);
	}
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	憮幗縑 氈朝 嬴檜蠱曖 葬蝶お蒂 蟾晦�� и棻.
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
VOID	VillageStructManager::FreeReceiveItemList()
{
	if(RIItems.pOnlineReceivedItemInfo != NULL)
	{
		delete [] RIItems.pOnlineReceivedItemInfo;
		RIItems.pOnlineReceivedItemInfo	=	NULL;
	}

	RIItems.siItemNum	=	0;
}
