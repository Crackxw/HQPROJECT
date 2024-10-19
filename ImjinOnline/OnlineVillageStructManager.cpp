#include <GSL.h>

#include	<Main.h>
#include	"OnlineWorld.h"
#include	"OnlineVillage.h"
#include	"OnlineVillageStructManager.h"
#include	"OnlineVillageStruct-Farm.h"
#include	"OnlineTradeBook.h"


//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	������.
//��������������������������������������������������������������������������������������������������������������������������������������������������������������
VillageStructManager::VillageStructManager()
{
	// ���� ������ �ǹ��� ���ٰ� �����Ѵ�.
	pStructure					=	NULL;		
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	�Ҹ���.
//��������������������������������������������������������������������������������������������������������������������������������������������������������������
VillageStructManager::~VillageStructManager()
{
	Free();
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	�ʱ�ȭ.
//��������������������������������������������������������������������������������������������������������������������������������������������������������������
VOID	VillageStructManager::Init( UI16 VillageCode, SI32 psiKind[ON_MAX_VIL_STRUCTNUM], cltOnlineWorld *pOnlineWorld)
{

	// � �ǹ����� �ִ��� �����Ѵ�.
	memcpy(siKinds, psiKind, sizeof(siKinds));

	pMyOnlineWorld	 =	pOnlineWorld;
	uiVillageCode	 =	VillageCode;
	siSuccess		 =	0;

	// �ʱ�ȭ.
	ZeroMemory(&RIItems, sizeof(RIItems));
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	����.
//��������������������������������������������������������������������������������������������������������������������������������������������������������������
VOID	VillageStructManager::Free()
{
	SI32	i;

	for(i = 0; i < ON_MAX_VIL_STRUCTNUM; i++)	
		siKinds[i]	=	ON_VIL_ST_KIND_UNKNOWN;	

	FreeReceiveItemList();

	// �ǹ��� ������ ������ �ǹ��� �����Ѵ�.
	if(pStructure != NULL)
	{
		delete pStructure;
		pStructure	=	NULL;
	}
}


//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	�ش� �ǹ� ������ ����.
//��������������������������������������������������������������������������������������������������������������������������������������������������������������
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
		// �ش� �ǹ��� �޸𸮸� �Ҵ� �޴´�.
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

	// ���콺 ��ǥ�� ���´�.
	pMyOnlineWorld->pIOnlineMouse->GetPos(&ptStartMousePos);	
	pStructure->SetStartMousePos(ptStartMousePos);
	
	
	pStructure->Init( uiVillageCode, pMyOnlineWorld, this );
}


//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	� �ǹ������� ������ �˾ƿ´�.
//��������������������������������������������������������������������������������������������������������������������������������������������������������������
BOOL	VillageStructManager::IsEnterStructure()
{
	if(pStructure != NULL)
		return	TRUE;
	else
		return	FALSE;
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	���� �� �ִ� ������ Kind�� ��´�.
//��������������������������������������������������������������������������������������������������������������������������������������������������������������
SI32	VillageStructManager::GetEnterStructureKind(void)
{
	return siKind;
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	������ �ִ� �������� ����Ʈ�� �޾Ҵ�.
//��������������������������������������������������������������������������������������������������������������������������������������������������������������
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

//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	������ �ִ� �������� ����Ʈ�� �ʱ�ȭ �Ѵ�.
//��������������������������������������������������������������������������������������������������������������������������������������������������������������
VOID	VillageStructManager::FreeReceiveItemList()
{
	if(RIItems.pOnlineReceivedItemInfo != NULL)
	{
		delete [] RIItems.pOnlineReceivedItemInfo;
		RIItems.pOnlineReceivedItemInfo	=	NULL;
	}

	RIItems.siItemNum	=	0;
}
