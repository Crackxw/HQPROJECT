#include "SOMain.h"
#include "SOWorld.h"
#include "SODatabase.h"
#include "OnlineItem.h"
#include "OnlineProductStructure.h"
#include "SOVillage_IndustrialComplexsStructure.h"

CItem			*SOVillage_IndustrialComplexs::m_pItem;
SODatabase		*SOVillage_IndustrialComplexs::m_pDataBase;
OnlineBuilding	*SOVillage_IndustrialComplexs::m_pBuilding;

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	정적 변수를 초기화 한다.
//	수정 일자	:	2002-10-04 오후 3:08:31 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
VOID	SOVillage_IndustrialComplexs::InitStaticVariable( CItem *pItem, SODatabase *pDataBase, OnlineBuilding *pBuilding )
{
	m_pItem			=	pItem;
	m_pDataBase		=	pDataBase;
	m_pBuilding		=	pBuilding;

	SOVillage_IndustrialComplex::InitStaticVariable( pItem, pDataBase, pBuilding );
	SOVillage_IndustrialComplex_Factory::InitStaticVariable( pItem, pDataBase, pBuilding );
}


//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	생성자.
//	수정 일자	:	2002-10-04 오후 3:16:17 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
SOVillage_IndustrialComplexs::SOVillage_IndustrialComplexs()
{

}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	소멸자.
//	수정 일자	:	2002-10-04 오후 3:16:31 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
SOVillage_IndustrialComplexs::~SOVillage_IndustrialComplexs()
{

}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	초기화.
//	수정 일자	:	2002-10-11 오후 12:01:18 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
BOOL	SOVillage_IndustrialComplexs::Init( UI16 uiVillageCode, CHAR cIndustrialComplexKind[ 4 ] )
{
	SI32	i;

	SOVillage_BaseStructure::Init( uiVillageCode, 1000 );

	m_uiVillageCode = uiVillageCode;

	memcpy( m_cIndustrialComplexKind, cIndustrialComplexKind, sizeof( m_cIndustrialComplexKind ) );

	if( ( m_pIndustrialComplexInfo = m_pBuilding->GetPlants( m_cIndustrialComplexKind ) ) == NULL )		return	FALSE;

	for( i = 0; i < ON_MAX_INDUSTRIAL_COMPLEX_NUM; i++ )
	{
		m_IndustrialComplex[ i ].Init( i, m_uiVillageCode );
	}
		
	

	return	TRUE;
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	메모리 해제.
//	수정 일자	:	2002-10-11 오후 12:01:23 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
VOID	SOVillage_IndustrialComplexs::Free()
{
	SI32	i;

	for( i = 0; i < ON_MAX_INDUSTRIAL_COMPLEX_NUM; i++ )
	{
		m_IndustrialComplex[ i ].Free();
	}

	SOVillage_BaseStructure::Free();
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	플레이어가 이 건물에 들어왔다.
//	수정 일자	:	2002-10-04 오후 8:57:46 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
BOOL	SOVillage_IndustrialComplexs::EnterPlayer( SOPlayer *pPlayer, SI32 siIndustrialComplexID )
{ 
	SOVillage_IndustrialComplex		*pIndustrialComplex;			

	if( SOVillage_BaseStructure::EnterPlayer( pPlayer )	== FALSE )							return	FALSE;		

	if( ( pIndustrialComplex = GetIndustrialComplex( siIndustrialComplexID ) ) == NULL )	return	FALSE;

	// 산업 단지에 들어간다.
	if( pIndustrialComplex->EnterPlayer( pPlayer ) == TRUE )
	{
		return	TRUE;
	}
	else
	{
		SOVillage_BaseStructure::LeavePlayer( pPlayer );
		return	FALSE;
	}

	

	/*
	stFarmData	*pFactoryData;

	if( uiPlantID > PLANT_CODE_NUM )										return	FALSE;			// 생산 시설물 ID가 유효한지 검사한다.
	
	if(	( pFactoryData = m_pFarmGroup->GetFarm( uiFarmID ) ) == NULL )		return	FALSE;			// 들어가려는 생산 시설물이 유효한지 검사한다.			

	pPlayer->stAccountInfo.pStructure	=	pFactoryData;

	pPlayer->SendFieldMsg( ON_RESPONSE_FARM_SLOTINFO, SO_SFM_ONLYME, LPVOID( ON_RET_FARM_OK ) );

	return	TRUE;
	
	/*
	switch( uiPlantID )
	{
	case ON_PLANT_FARM:			pPlayer->stAccountInfo.uiStatusInVillage = INVILLAGE_STATUS_FARM;			break;
	case ON_PLANT_RANCH:		pPlayer->stAccountInfo.uiStatusInVillage = INVILLAGE_STATUS_RANCH;			break;
	case ON_PLANT_FACTORY:		pPlayer->stAccountInfo.uiStatusInVillage = INVILLAGE_STATUS_FACTORY;		break;
	case ON_PLANT_ARMOR:		pPlayer->stAccountInfo.uiStatusInVillage = INVILLAGE_STATUS_ARMOR;			break;
	case ON_PLANT_WEAPON:		pPlayer->stAccountInfo.uiStatusInVillage = INVILLAGE_STATUS_WEAPON;			break;
	case ON_PLANT_MINE:			pPlayer->stAccountInfo.uiStatusInVillage = INVILLAGE_STATUS_MINE;			break;	
	}	
	*/

	/*
	// 작업장에 들어왔다.
	pPlayer->stAccountInfo.uiStatusInVillage	=	INVILLAGE_STATUS_FACTORY;

	pPlayer->stAccountInfo.pStructure	=	pFarm;

	pPlayer->SendFieldMsg(ON_RESPONSE_FARM_SLOTINFO, SO_SFM_ONLYME, LPVOID(ON_RET_FARM_OK), LPVOID(pFarm));
	*/

	return TRUE;
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	플레이어가 이 건물에서 나갔다.
//	수정 일자	:	2002-10-11 오후 1:25:51 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
BOOL	SOVillage_IndustrialComplexs::LeavePlayer( SOPlayer *pPlayer )
{

	SOVillage_IndustrialComplex		*pIndustrialComplex;

	// 산업 단지에 들어가 있다면 산업단지에서도 나갈 수 있도록 해준다.
	if( ( pIndustrialComplex = GetEnteredIndustrialComplex( pPlayer ) ) != NULL )
	{
		pIndustrialComplex->LeavePlayer( pPlayer );
	}

	return	SOVillage_BaseStructure::LeavePlayer( pPlayer );
	
}	



//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	산업 단지들의 정보를 플레이어에게 보내준다.
//	수정 일자	:	2002-10-04 오후 3:16:45 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
BOOL	SOVillage_IndustrialComplexs::SendIndustrialComplexList( SOPlayer *pPlayer )
{
	OnIndustrialComplexInfo		IndustrialComplexInfo[ ON_MAX_INDUSTRIAL_COMPLEX_NUM ]; 
	UI32						uiIndustrialComplexNum;
	DWORD						dwInfoLength;
	
	dwInfoLength	=	GetAllIndustrialComplexInfo( IndustrialComplexInfo, &uiIndustrialComplexNum );

	pPlayer->SendFieldMsg( ON_RESPONSE_FARM_INFO, SO_SFM_ONLYME, LPVOID( uiIndustrialComplexNum ), LPVOID( IndustrialComplexInfo ), LPVOID( dwInfoLength ) );

	return	TRUE;
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	이 마을에 있는 모든 생산 시설물을 얻어온다.
//	수정 일자	:	2002-10-04 오후 6:31:57 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
stFarmGroup*	SOVillage_IndustrialComplexs::GetFactories()
{
//	return	m_pFarmGroup;

	return	NULL;
}

/*
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	가동하고 있지 않는 건물은 없앤다.
//	수정 일자	:	2002-10-04 오후 7:15:30 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
VOID	SOVillage_IndustrialComplexs::DismantleFactoryNotWork( SOWorld *pWorld )
{	
	DBIKBuyFarmByForceInfo		stUpdateWorkDay;
	SI32						i;
	stFarmData					*pFactoryData;	
	MONEY						mnProductFacilityPrice;				// 생산 시설이 해체되고 받는 돈.
	SOPlayer					*pFactoryOwner;

	stUpdateWorkDay.uiVillageCode	=	m_uiVillageCode;
	stUpdateWorkDay.uiPlantID		=	m_pFarmGroup->pPlant->DBIndex;
	
	for( i = 0; i < ON_VILLAGE_FARM_NUM; i++ )
	{
		pFactoryData	=	&m_pFarmGroup->Farm[ i ];

		if( pFactoryData->siOwnerDBAcnt != 0 )
		{					
			pFactoryOwner	=	pWorld->GetPlayerByDBAccount( pFactoryData->siOwnerDBAcnt );

			pFactoryData->siWorkDay++;			

			if( pFactoryData->siWorkDay > ON_FORCE_TAKE_DAY)
			{					
				if( pFactoryOwner != NULL)		
				{
					if( SellFactoryByForce( i, &mnProductFacilityPrice) == TRUE )
						pFactoryOwner->GiveBonusMoney( mnProductFacilityPrice, ON_RET_CHNGEMONEY_PRODUCTFACILITY, FALSE );
				}
				else
				{
					SellFactoryByForce( i, NULL );	
				}
			}
			else if( ( pFactoryData->siWorkDay % 5) == 0)
			{
				stUpdateWorkDay.uiFarmID	=	i;
				stUpdateWorkDay.siWorkDay	=	pFactoryData->siWorkDay;

				m_pDataBase->writeDBQuery( SO_DB_INFOKIND_FARM_SETREMAINDAY, LPVOID( &stUpdateWorkDay ), sizeof( DBIKBuyFarmByForceInfo ) );
			}
		}
	}
}
*/
/*
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	플레이어가 소유한 생산건물을 얻어온다.
//	수정 일자	:	2002-10-04 오후 7:27:48 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
stFarmData*		SOVillage_IndustrialComplexs::GetOwnFactory( SI32 siPlayerDBAccount, UI08 uiPlayerDBSlot )
{
	SI32			i;
	stFarmData		*pFactoryData;

	for( i = 0; i < ON_VILLAGE_FARM_NUM; i++ )
	{
		pFactoryData	=	&m_pFarmGroup->Farm[ i ];

		if( ( pFactoryData->siOwnerDBAcnt == siPlayerDBAccount ) && ( pFactoryData->uiOwnerSlot	== uiPlayerDBSlot ) )
			return	pFactoryData;
	}	

	return	NULL;
}
*/

/*
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	생산 시설을 구입한다.
//	수정 일자	:	2002-10-04 오후 8:29:54 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
BOOL	SOVillage_IndustrialComplexs::BuyFactory( SOPlayer *pPlayer, UI08 uiPlantID, UI08 uiFarmID, MONEY *pmnCost )
{
	DBIKBuyFarmInfo					BuyFarmInfo;	
	stFarmData						*pFarm = NULL;
	stFarmData						*pFactoryData;	

	if( ( pFarm = m_pFarmGroup->GetFarm( uiFarmID ) ) == NULL)			return	FALSE;
	
	if( pFarm->siOwnerDBAcnt != 0 )										return	FALSE;		// 생산 시설물의 소유주가 있는지 검사한다.		
	if( pFarm->pPlant->siReqCredit > pPlayer->GetTradeGrade() )			return	FALSE;		// 생산 시설물을 구입하기위한 신용도가 되는지 검사한다. 		
	if( pFarm->pPlant->uiCost > pPlayer->siMoney)						return	FALSE;		// 생산 시설물을 구입하기위한 돈이 되는지 검사한다.		
	if( pPlayer->GetOwnFactory() !=	NULL )								return	FALSE;		// 이미 생산 시설물을 가지고 있는지 검사한다.		
	
	BuyFarmInfo.siAccount		=	pPlayer->stAccountInfo.siDBAccount;
	BuyFarmInfo.uiSlot			=	pPlayer->stAccountInfo.uiSlot;
	BuyFarmInfo.uiVillageCode	=	m_uiVillageCode;
	BuyFarmInfo.uiFarmID		=	uiFarmID;
	BuyFarmInfo.uiPlantID		=	m_pFarmGroup->pPlant->DBIndex;	
	BuyFarmInfo.uiAvail			=	m_pFarmGroup->pPlant->uiInitSlot;		
	m_pDataBase->writeDBQuery( SO_DB_INFOKIND_FARM_BUY, &BuyFarmInfo, sizeof( BuyFarmInfo ) );

	// 서버도 업데이트 한다.	
	pFactoryData			=	&m_pFarmGroup->Farm[ uiFarmID ];

	pFactoryData->Init();
	pFactoryData->siOwnerDBAcnt		=	pPlayer->stAccountInfo.siDBAccount;
	pFactoryData->uiOwnerSlot		=	pPlayer->stAccountInfo.uiSlot;

	// 생산 시설물의 주인을 설정한다.
	ZeroMemory( pFactoryData->FarmInfo.szOwner, sizeof( pFactoryData->FarmInfo.szOwner ) );
	strncpy( pFactoryData->FarmInfo.szOwner, pPlayer->szID, ON_ID_LENGTH );	

	// 플레이어의 돈을 감소시킨다.
	*pmnCost				=	m_pFarmGroup->pPlant->uiCost;	

	return TRUE;	
}
/*

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	건물이 강제로 팔렸다.
//	수정 일자	:	2002-10-04 오후 8:46:43 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
BOOL	SOVillage_IndustrialComplexs::SellFactoryByForce( UI08 uiFarmID, MONEY *pmnMoney )
{
	DBIKBuyFarmByForceInfo			SellFarmInfo;	
	stFarmData						*pFarm;
		
	if( ( pFarm = m_pFarmGroup->GetFarm( uiFarmID ) ) == NULL )				return	FALSE;	
	
	if( pmnMoney == NULL )
	{
		SellFarmInfo.bSendMsgToPlayer	=	FALSE;	
	}
	else
	{	
		*pmnMoney						=	pFarm->pPlant->uiCost;
		SellFarmInfo.bSendMsgToPlayer	=	TRUE;	
	}

	SellFarmInfo.mnPrice		=	pFarm->pPlant->uiCost;	
	SellFarmInfo.uiFarmID		=	uiFarmID;
	SellFarmInfo.uiPlantID		=	m_pFarmGroup->pPlant->DBIndex;
	SellFarmInfo.uiVillageCode	=	m_uiVillageCode;	
	m_pDataBase->writeDBQuery( SO_DB_INFOKIND_FARM_SELL_BYFORCE, &SellFarmInfo, sizeof( SellFarmInfo ) );

	// 서버도 업데이트 한다.
	pFarm->Init();	

	return TRUE;	
}
*/

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	일하려고 하는 곳이 유효한 곳인지 검사한다.
//	수정 일자	:	2002-10-07 오후 2:23:08 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
BOOL	SOVillage_IndustrialComplexs::CanWork( SOPlayer *pPlayer )
{		
	SOVillage_IndustrialComplex		*pIndustrialComplex;

	// 산업 단지에 들어가 있다면 산업단지에서도 나갈 수 있도록 해준다.
	if( ( pIndustrialComplex = GetEnteredIndustrialComplex( pPlayer ) ) == NULL )		return	FALSE;
		
	return	pIndustrialComplex->CanWork( pPlayer );
	

	/*
	if( ( pIndustrialComplex = GetIndustrialComplex( uiIndustrialComplexID ) ) == NULL )		return	FALSE;		// 산업 단지를 얻어온다.
	
	if( ( pFarmSlot	= pIndustrialComplex->GetFarmSlot( uiSlot ) ) == NULL )		return	FALSE;		// 일하려고 하는 곳이 유효한 곳인지 검사한다.

	// 생산 시설 내에서 일을 할수 있는 준비가 갖춰져 있는지 검사한다.
	pFarmSlot->bWorkable	=	FALSE;

	if( pFarmSlot->bOnSchedule() == FALSE )														return	FALSE;		// 일하려고 하는 곳의 작업량이 남아있어 작업을 할 수 있는지 검사한다.
	
	// 창고에 일을 하기 위한 도구가 있는지 검사한다.
	if( CheckStorageTool( pFarmGroup->pPlant->DBIndex, pPlayer->stAccountInfo.pStructure->nIndex, pFarmSlot->ProductItem ) == FALSE )
		return	FALSE;

	// 작업을 할 수 있다.
	pFarmSlot->bWorkable	=	TRUE;

	return TRUE;	
	*/
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	유효한 산업 단지의 ID인지 알아온다.
//	수정 일자	:	2002-10-07 오후 3:48:55 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
BOOL	SOVillage_IndustrialComplexs::IsValidIndustrialComplexID( SI32 siIndustrialComplexID )
{
	if( siIndustrialComplexID >= 0 && siIndustrialComplexID < ON_MAX_INDUSTRIAL_COMPLEX_NUM )		return	TRUE;

	return	FALSE;
}


//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	산업 단지를 얻어온다.
//	수정 일자	:	2002-10-07 오후 3:03:36 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
SOVillage_IndustrialComplex*	SOVillage_IndustrialComplexs::GetIndustrialComplex( SI32 siIndustrialComplexID )
{
	if( IsValidIndustrialComplexID( siIndustrialComplexID ) == FALSE )								return	NULL;

	return	&m_IndustrialComplex[ siIndustrialComplexID ];	
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	플레이어가 소유하고 있는 산업 단지를 얻어온다.
//	수정 일자	:	2002-10-15 오후 2:07:30 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
SOVillage_IndustrialComplex*	SOVillage_IndustrialComplexs::GetIndustrialComplexOwned( SI32 siIndustrialComplexID )
{
	SOVillage_IndustrialComplex		*pIndustrialComplex;

	if( ( pIndustrialComplex = GetIndustrialComplex( siIndustrialComplexID ) ) == NULL )		return	NULL;

	if( pIndustrialComplex->HasOwner() == FALSE )												return	NULL;
	
	return	pIndustrialComplex;
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	해당 플레이어가 소유하고 있는 산업 단지를 얻어온다.
//	수정 일자	:	2002-10-10 오후 8:53:38 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
SOVillage_IndustrialComplex*	SOVillage_IndustrialComplexs::GetOwnIndustrialComplex( SOPlayer *pPlayer )
{
	SI32	i;

	for( i = 0; i < ON_MAX_INDUSTRIAL_COMPLEX_NUM; i++ )
	{
		if( m_IndustrialComplex[ i ].IsOwner( pPlayer ) == TRUE )
			return	&m_IndustrialComplex[ i ];
	}

	return	NULL;
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	해당 플레이어가 들어가있는 산업 단지를 얻어온다.
//	수정 일자	:	2002-10-11 오후 1:27:13 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
SOVillage_IndustrialComplex*	SOVillage_IndustrialComplexs::GetEnteredIndustrialComplex( SOPlayer *pPlayer )
{
	SI32	i;

	for( i = 0; i < ON_MAX_INDUSTRIAL_COMPLEX_NUM; i++ )
	{
		if( m_IndustrialComplex[ i ].IsPlayerIn( pPlayer ) == TRUE )
		{
			return	&m_IndustrialComplex[ i ];
		}
	}

	return	NULL;
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	해당 플레이어가 들어가있는 산업 단지를 얻어온다.
//	수정 일자	:	2002-10-11 오후 1:27:13 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
SI32	SOVillage_IndustrialComplexs::GetEnteredIndustrialComplexIndex( SOPlayer *pPlayer )
{
	SI32 i;

	for( i = 0; i < ON_MAX_INDUSTRIAL_COMPLEX_NUM; i++ )
	{
		if( m_IndustrialComplex[ i ].IsPlayerIn( pPlayer ) == TRUE )
			return	i;
	}

	return	-1;
}


//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	모든 산업 단지의 정보를 얻어온다.
//	수정 일자	:	2002-10-07 오후 7:20:48 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
DWORD	SOVillage_IndustrialComplexs::GetAllIndustrialComplexInfo( OnIndustrialComplexInfo *pIndustrialComplexInfo, UI32 *puiIndustrialComplexInfoNum )
{
	SI32	i;
	DWORD	dwTempInfoLength, dwInfoLength;

	dwInfoLength					=	0;
	*puiIndustrialComplexInfoNum	=	0;
	for( i = 0; i < ON_MAX_INDUSTRIAL_COMPLEX_NUM; i++ )
	{
		dwTempInfoLength	=	m_IndustrialComplex[ i ].GetInfo( ( OnIndustrialComplexInfo* ) ( ( CHAR* ) pIndustrialComplexInfo + dwInfoLength ) );

		if( dwTempInfoLength != 0)
		{		
			dwInfoLength	+=	dwTempInfoLength;
			( *puiIndustrialComplexInfoNum )++;
		}			
	}

	return	dwInfoLength;
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	DB에서 얻어온 정보로 산업 단지를 초기화 한다.
//	수정 일자	:	2002-10-07 오후 9:24:05 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
BOOL	SOVillage_IndustrialComplexs::InitIndustrialComplexFromDB( SI32 siIndustrialComplexID, 
												SI32 siOwnerDBAccount, UI08 uiOwnerDBSlot, CHAR *pszOwnerName, 
												SI32 siMaxConstructibleFactoryNum, SI32 siDayNotWorked )
{
	SOVillage_IndustrialComplex		*pIndustrialComplex;

	if( ( pIndustrialComplex = GetIndustrialComplex( siIndustrialComplexID ) ) == NULL )	return	FALSE;
	
	pIndustrialComplex->InitFromDB( siOwnerDBAccount, uiOwnerDBSlot, pszOwnerName, siMaxConstructibleFactoryNum, siDayNotWorked );

	return	TRUE;
}


//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	DB에서 얻어온 정보로 창고를 초기화 한다.
//	수정 일자	:	2002-10-07 오후 9:24:30 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
BOOL	SOVillage_IndustrialComplexs::InitStorageFromDB( SI32 siIndustrialComplexID, SI32 siFactoryID, UI08 uiLevel, MONEY mnPay, UI32 uiAmountOfWork, 
									UI16 uiItemID0, UI16 uiItemID1, UI16 uiItemID2, UI16 uiItemID3, UI16 uiItemID4, 
									UI16 uiItemID5, UI16 uiItemID6, UI16 uiItemID7, UI16 uiItemID8, UI16 uiItemID9, 
									UI16 uiQuantity0, UI16 uiQuantity1, UI16 uiQuantity2, UI16 uiQuantity3, UI16 uiQuantity4, 
									UI16 uiQuantity5, UI16 uiQuantity6, UI16 uiQuantity7, UI16 uiQuantity8, UI16 uiQuantity9 )
{
	SOVillage_IndustrialComplex		*pIndustrialComplex;

	if( ( pIndustrialComplex = GetIndustrialComplex( siIndustrialComplexID ) ) == NULL )	return	FALSE;

	return	pIndustrialComplex->InitStorageFromDB( siFactoryID, uiLevel, mnPay, uiAmountOfWork,
											uiItemID0, uiItemID1, uiItemID2, uiItemID3, uiItemID4, 
											uiItemID5, uiItemID6, uiItemID7, uiItemID8, uiItemID9,
											uiQuantity0, uiQuantity1, uiQuantity2, uiQuantity3, uiQuantity4, 
											uiQuantity5, uiQuantity6, uiQuantity7, uiQuantity8, uiQuantity9 );	
	
}


//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	DB에서 얻어온 정보로 공장을 초기화 한다.
//	수정 일자	:	2002-10-08 오전 10:35:58 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
BOOL	SOVillage_IndustrialComplexs::InitFactoryFromDB( SI32 siIndustrialComplexID, SI32 siFactoryID, UI08 uiLevel, MONEY mnPay, UI32 uiAmountOfWork, CHAR cFactoryKind[ 4 ],
										UI16 uiProducingItemID, UI32 uiProducedItemQuantity, UI32 uiMaxProduceItemQuantity)
{
	SOVillage_IndustrialComplex		*pIndustrialComplex;

	if( ( pIndustrialComplex = GetIndustrialComplex( siIndustrialComplexID ) ) == NULL )	return	FALSE;

	return	pIndustrialComplex->InitFactoryFromDB( siFactoryID, uiLevel, mnPay, uiAmountOfWork, cFactoryKind, 
													uiProducingItemID, uiProducedItemQuantity, uiMaxProduceItemQuantity );		
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	공장을 처분한다.
//	수정 일자	:	2002-10-08 오전 11:41:43 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
BOOL	SOVillage_IndustrialComplexs::SellFactory( SOPlayer *pPlayer, SI32 siFactoryID )
{
	SI32	i;
	SI32	siOwnerDBAccount;
	UI08	uiOwnerDBSlot;

	for( i = 0; i < ON_MAX_INDUSTRIAL_COMPLEX_NUM; i++ )
	{
		if( m_IndustrialComplex[ i ].HasOwner() == TRUE )
		{
			m_IndustrialComplex[ i ].GetOwner( &siOwnerDBAccount, &uiOwnerDBSlot );

			if( siOwnerDBAccount == pPlayer->stAccountInfo.siDBAccount &&
				uiOwnerDBSlot == pPlayer->stAccountInfo.uiSlot )
			{
				return	m_IndustrialComplex[ i ].SellFactory( pPlayer, siFactoryID );
			}
		}
	}

	return	FALSE;
}


//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	일을 끝마쳤다.
//	수정 일자	:	2002-10-08 오전 11:57:56 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
BOOL	SOVillage_IndustrialComplexs::FinishWork( SOPlayer *pPlayer, OnReqFarmWorkMsg *pMsg )
{	
	SI32	i;
	
	for( i = 0; i < ON_MAX_INDUSTRIAL_COMPLEX_NUM; i++ )
	{
		if( m_IndustrialComplex[ i ].HasOwner() == TRUE )
		{
			if( m_IndustrialComplex[ i ].IsPlayerIn( pPlayer ) == TRUE )
			{
				return m_IndustrialComplex[ i ].FinishWork( pPlayer, pMsg );
			}
		}
	}

	return	FALSE;
}

/*
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	산업 단지에 있는 공장의 정보를 플레이어에게 보내준다.
//	수정 일자	:	2002-10-08 오후 2:17:55 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
BOOL	SOVillage_IndustrialComplexs::SendFactoryInfo( SOPlayer *pPlayer, SI32 siFactoryID )
{	
	SI32	i;
	
	for( i = 0; i < ON_MAX_INDUSTRIAL_COMPLEX_NUM; i++ )
	{
		if( m_IndustrialComplex[ i ].HasOwner() == TRUE )
		{
			if( m_IndustrialComplex[ i ].IsPlayerIn( pPlayer ) == TRUE )
			{
				return	m_IndustrialComplex[ i ].SendFactoryInfo( pPlayer, siFactoryID );
			}
		}
	}

	return	FALSE;	
}


//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	산업 단지에 있는 창고의 정보를 플레이어에게 보내준다.
//	수정 일자	:	2002-10-08 오후 3:16:13 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
BOOL	SOVillage_IndustrialComplexs::SendStorageInfo( SOPlayer *pPlayer, SI32 siFactoryID )
{
	SI32	i;
	
	for( i = 0; i < ON_MAX_INDUSTRIAL_COMPLEX_NUM; i++ )
	{
		if( m_IndustrialComplex[ i ].HasOwner() == TRUE )
		{
			if( m_IndustrialComplex[ i ].IsPlayerIn( pPlayer ) == TRUE )
			{
				return	m_IndustrialComplex[ i ].SendStorageInfo( pPlayer, siFactoryID );
			}
		}
	}

	return	FALSE;	
}
*/
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	산업 단지를 구입한다.
//	수정 일자	:	2002-10-08 오후 8:34:06 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
BOOL	SOVillage_IndustrialComplexs::BuyIndustrialComplex( SOPlayer *pPlayer, SI32 siIndustrialComplexID )
{
	SOVillage_IndustrialComplex		*pIndustrialComplex;	

	if( ( pIndustrialComplex = GetIndustrialComplex( siIndustrialComplexID ) ) == NULL )	return	FALSE;	

	if( m_pIndustrialComplexInfo->siReqCredit > pPlayer->GetTradeGrade() )			return	FALSE;		// 생산 시설물을 구입하기위한 신용도가 되는지 검사한다. 		
	if( m_pIndustrialComplexInfo->uiCost > pPlayer->siMoney)						return	FALSE;		// 생산 시설물을 구입하기위한 돈이 되는지 검사한다.				
	
	if( pIndustrialComplex->BuyIndustrialComplex( pPlayer, m_pIndustrialComplexInfo->uiInitSlot ) == FALSE )		return	FALSE;

	// 비용을 지불한다.
	pPlayer->DecreaseMoney( m_pIndustrialComplexInfo->uiCost , FALSE);

	





	// DB에 저장한다.

	DBIKBuyFarmInfo				BuyFarmInfo;	
	BuyFarmInfo.siAccount		=	pPlayer->stAccountInfo.siDBAccount;
	BuyFarmInfo.uiSlot			=	pPlayer->stAccountInfo.uiSlot;
	BuyFarmInfo.uiVillageCode	=	pPlayer->stAccountInfo.pVillage->uiVillageCode;
	BuyFarmInfo.uiFarmID		=	siIndustrialComplexID;
	BuyFarmInfo.moMoney			=   m_pIndustrialComplexInfo->uiCost;


	
	
	m_pDataBase->writeDBQuery( SO_DB_INFOKIND_FARM_BUY, &BuyFarmInfo, sizeof( BuyFarmInfo ) );	

	// 성공적으로 산업 단지를 구입했다.
	
	// 성공적으로 구입했다고 알려준다.
	pPlayer->SendFieldMsg( ON_RESPONSE_FARM_BUY, SO_SFM_ONLYME, LPVOID( ON_RET_OK ) );

	return	TRUE;		
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	토지를 개척한다.
//	수정 일자	:	2002-10-10 오후 5:31:43 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
BOOL	SOVillage_IndustrialComplexs::Reclamation( SOPlayer *pPlayer, SI32 siIndustrialComplexID, MONEY *pmnCapital )
{
	SOVillage_IndustrialComplex		*pIndustrialComplex;	

	if( ( pIndustrialComplex = GetIndustrialComplex( siIndustrialComplexID ) ) == NULL )				return	FALSE;	

	if( pIndustrialComplex->Reclamation( pPlayer, m_pIndustrialComplexInfo->uiMaxSlot ) == FALSE )		return	FALSE;

	// 성공적으로 토지를 개척했다.
	// 비용을 지불한다.
	pPlayer->DecreaseMoney( ON_PLANT_EXPANDSLOT_COST ,FALSE);

	// 마을의 수익을 얻어온다.
	*pmnCapital	=	MONEY( ON_PLANT_EXPANDSLOT_COST * ON_CAPITAL_RATE_PLANT );

	// 성공적으로 개척했다고 알려준다.
	pPlayer->SendFieldMsg( ON_RESPONSE_FARM_EXPANDSLOT, SO_SFM_ONLYME, LPVOID( ON_RET_OK ) );

	return	TRUE;
}


//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	공장을 건설한다.
//	수정 일자	:	2002-10-10 오후 8:51:26 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
BOOL	SOVillage_IndustrialComplexs::ConstructFactory( SOPlayer *pPlayer, SI32 siFactoryID, CHAR cFactoryCode[ 4 ], MONEY mnPay, MONEY *pmnCapital )
{


	SOVillage_IndustrialComplex		*pIndustrialComplex;	

	if( ( pIndustrialComplex = GetOwnIndustrialComplex( pPlayer ) ) == NULL )										return	FALSE;		
	
	if( pIndustrialComplex->ConstructFactory( pPlayer, siFactoryID, cFactoryCode, mnPay, pmnCapital ) == FALSE )	return	FALSE;

	// 성공적으로 공장을 건설했다.
	
		
	// 성공적으로 공장을 건설했다고 알려준다.
	pPlayer->SendFieldMsg( ON_RESPONSE_FARM_BUILDSTRUCT, SO_SFM_ONLYME, LPVOID( ON_RET_OK ) );

	return	TRUE;
}


//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	산업 단지의 공장에 들어간다.
//	수정 일자	:	2002-10-11 오후 4:36:34 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
BOOL	SOVillage_IndustrialComplexs::EnterFactory( SOPlayer *pPlayer, SI32 siFactoryID )
{
	SOVillage_IndustrialComplex		*pIndustrialComplex;

	// 이 산업 단지에 들어와 있는지 검사한다.
	if( ( pIndustrialComplex = GetEnteredIndustrialComplex( pPlayer ) ) == NULL )		return	FALSE;
	
	return	pIndustrialComplex->EnterFactory( pPlayer, siFactoryID );		
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	산업 단지의 공장에서 나간다.
//	수정 일자	:	2002-10-11 오후 4:48:28 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
BOOL	SOVillage_IndustrialComplexs::LeaveFactory( SOPlayer *pPlayer )
{
	SOVillage_IndustrialComplex		*pIndustrialComplex;

	// 이 산업 단지에 들어와 있는지 검사한다.
	if( ( pIndustrialComplex = GetEnteredIndustrialComplex( pPlayer ) ) == NULL )		return	FALSE;
	
	return	pIndustrialComplex->LeaveFactory( pPlayer );				
}


//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	일을 시작한다.
//	수정 일자	:	2002-10-11 오후 7:24:58 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
BOOL	SOVillage_IndustrialComplexs::BeginWork( SOPlayer *pPlayer )
{
	SOVillage_IndustrialComplex		*pIndustrialComplex;

	// 플레이어가 들어간 산업 단지를 얻어온다.
	if( ( pIndustrialComplex = GetEnteredIndustrialComplex( pPlayer ) ) == NULL )	
	{
		return	FALSE;
	}
	
	return	pIndustrialComplex->BeginWork( pPlayer );					
}


//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	공장의 임금을 변경한다.
//	수정 일자	:	2002-10-11 오후 8:58:48 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
BOOL	SOVillage_IndustrialComplexs::ChangeFactoryPay( SOPlayer *pPlayer, MONEY mnPay, MONEY *pmnCapital )
{
	SOVillage_IndustrialComplex		*pIndustrialComplex;

	// 플레이어가 들어간 산업 단지를 얻어온다.
	if( ( pIndustrialComplex = GetEnteredIndustrialComplex( pPlayer ) ) == NULL )		return	FALSE;

	// 산업 단지의 주인인지 검사한다.
	if( pIndustrialComplex->IsOwner( pPlayer ) == FALSE )								return	FALSE;	

	return	pIndustrialComplex->ChangeFactoryPay( pPlayer, mnPay, pmnCapital );
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	현재 작업중인 공장에서 작업중인것을 취소한다.
//	수정 일자	:	2002-10-12 오후 2:44:36 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
BOOL	SOVillage_IndustrialComplexs::CancelWork( SOPlayer *pPlayer, SI32 siFactoryID, MONEY *pmnCapital )
{
	SOVillage_IndustrialComplex		*pIndustrialComplex;

	// 플레이어가 들어간 산업 단지를 얻어온다.
	if( ( pIndustrialComplex = GetEnteredIndustrialComplex( pPlayer ) ) == NULL )		return	FALSE;

	// 산업 단지의 주인인지 검사한다.
	if( pIndustrialComplex->IsOwner( pPlayer ) == FALSE )								return	FALSE;	

	return	pIndustrialComplex->CancelWork( pPlayer, siFactoryID, pmnCapital );	
}


//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	공장을 업그레이드 시킨다.
//	수정 일자	:	2002-10-12 오후 5:49:40 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
BOOL	SOVillage_IndustrialComplexs::UpgradeFactory( SOPlayer *pPlayer, SI32 siFactoryID, MONEY mnPay, MONEY *pmnCapital )
{
	SOVillage_IndustrialComplex		*pIndustrialComplex;

	// 플레이어가 들어간 산업 단지를 얻어온다.
	if( ( pIndustrialComplex = GetEnteredIndustrialComplex( pPlayer ) ) == NULL )		return	FALSE;

	// 산업 단지의 주인인지 검사한다.
	if( pIndustrialComplex->IsOwner( pPlayer ) == FALSE )								return	FALSE;	

	return	pIndustrialComplex->UpgradeFactory( pPlayer, siFactoryID, mnPay, pmnCapital );		
}


//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	아이템을 생산한다.
//	수정 일자	:	2002-10-12 오후 7:15:39 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
DWORD	SOVillage_IndustrialComplexs::ProduceItem( SOPlayer *pPlayer, SI32 siFactoryID, UI16 uiItemID, UI32 uiItemQuantity, MONEY mnPay, CHAR cNation, MONEY *pmnCapital )
{
	SOVillage_IndustrialComplex		*pIndustrialComplex;

	// 플레이어가 들어간 산업 단지를 얻어온다.
	if( ( pIndustrialComplex = GetEnteredIndustrialComplex( pPlayer ) ) == NULL )		return	ON_RET_NO;

	// 산업 단지의 주인인지 검사한다.
	if( pIndustrialComplex->IsOwner( pPlayer ) == FALSE )								return	ON_RET_FARM_NOTOWNER;		

	return	pIndustrialComplex->ProduceItem( pPlayer, siFactoryID, uiItemID, uiItemQuantity, mnPay, cNation, pmnCapital );		
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	창고 안에 아이템을 넣는다.
//	수정 일자	:	2002-10-14 오전 11:25:37 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
BOOL	SOVillage_IndustrialComplexs::AddItem( SOPlayer *pPlayer, UI16 uiItemID, UI16 uiItemQuantity ,BOOL bSendToDB)
{
	SOVillage_IndustrialComplex		*pIndustrialComplex;

	// 플레이어가 들어간 산업 단지를 얻어온다.
	if( ( pIndustrialComplex = GetEnteredIndustrialComplex( pPlayer ) ) == NULL )		return	FALSE;

	// 산업 단지의 주인인지 검사한다.
	if( pIndustrialComplex->IsOwner( pPlayer ) == FALSE )								return	FALSE;		

	return	pIndustrialComplex->AddItem( pPlayer, uiItemID, uiItemQuantity ,bSendToDB);		
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	창고안에서 아이템을 삭제한다.
//	수정 일자	:	2002-10-14 오전 11:26:15 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
BOOL	SOVillage_IndustrialComplexs::DeleteItem( SOPlayer *pPlayer, UI16 uiItemID, UI16 uiItemQuantity ,BOOL bSendToDB)
{
	SOVillage_IndustrialComplex		*pIndustrialComplex;

	// 플레이어가 들어간 산업 단지를 얻어온다.
	if( ( pIndustrialComplex = GetEnteredIndustrialComplex( pPlayer ) ) == NULL )		return	FALSE;

	// 산업 단지의 주인인지 검사한다.
	if( pIndustrialComplex->IsOwner( pPlayer ) == FALSE )								return	FALSE;		

	return	pIndustrialComplex->DeleteItem( pPlayer, uiItemID, uiItemQuantity ,bSendToDB);		
}


//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	공장을 파괴한다.
//	수정 일자	:	2002-10-15 오전 11:12:21 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
BOOL	SOVillage_IndustrialComplexs::DestroyFactory( SOPlayer *pPlayer, SI32 siFactoryID, MONEY *pmnCapital )
{
	SOVillage_IndustrialComplex		*pIndustrialComplex;

	// 플레이어가 들어간 산업 단지를 얻어온다.
	if( ( pIndustrialComplex = GetEnteredIndustrialComplex( pPlayer ) ) == NULL )		return	FALSE;

	// 산업 단지의 주인인지 검사한다.
	if( pIndustrialComplex->IsOwner( pPlayer ) == FALSE )								return	FALSE;		

	return	pIndustrialComplex->DestroyFactory( pPlayer, siFactoryID, pmnCapital );			
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	하루가 지났다.
//	수정 일자	:	2002-10-15 오후 2:03:18 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
VOID	SOVillage_IndustrialComplexs::DayChanged( SOWorld *pWorld ,UI32 uiNPCLabor)
{
	MONEY							maxpays[ON_MAX_INDUSTRIAL_COMPLEX_NUM];
	SI32							i;
	SOVillage_IndustrialComplex		*pIndustrialComplex;
	SI32							siOwnerDBAccount;
	UI08							uiOwnerDBSlot;
	SOPlayer						*pPlayer;
	MONEY                           maxPay = 0;
	SI32                            siMaxPayComplexIndex = -1;

	
	OnIndustrialComplexInfo IndustrialComplexInfo;

	
	// 최고임금 방식
/*	for( i = 0; i < ON_MAX_INDUSTRIAL_COMPLEX_NUM; i++ )
	{
		if( ( pIndustrialComplex = GetIndustrialComplexOwned( i ) ) == NULL )		continue;		
	
		pIndustrialComplex->GetOwner( &siOwnerDBAccount, &uiOwnerDBSlot );

		pPlayer	=	pWorld->GetPlayerByDBAccount( siOwnerDBAccount, uiOwnerDBSlot );

		// 하루가 지났을때의 작업을 해준다.
		pIndustrialComplex->DayChanged( pPlayer, m_pIndustrialComplexInfo->uiCost );

		// 가장임금을 많이주는곳을 구해놓는다 (초기임금을 0으로 해놨기때문에 작업량이 없는곳은 찾지않는다.)
		if (pIndustrialComplex->GetInfo(&IndustrialComplexInfo)) {
			
			if (IndustrialComplexInfo.m_moMaxPay > maxPay)
			{
				siMaxPayComplexIndex = i;
				maxPay = IndustrialComplexInfo.m_moMaxPay;
				
			}
		}

		
	}

	// 일을한다.
	if (siMaxPayComplexIndex >= 0 && uiNPCLabor > 0)
	{
		pIndustrialComplex = GetIndustrialComplexOwned( siMaxPayComplexIndex );
		if (pIndustrialComplex != NULL)
		{
			pIndustrialComplex->DoNPCWork(uiNPCLabor);
		}

	}*/

	// 최고임금 비율 방식

	MONEY totalpay = 0;

	

	for( i = 0; i < ON_MAX_INDUSTRIAL_COMPLEX_NUM; i++ )
	{
		if( ( pIndustrialComplex = GetIndustrialComplexOwned( i ) ) == NULL )		{
			maxpays[i] = 0;
			continue;		
		}
	
		pIndustrialComplex->GetOwner( &siOwnerDBAccount, &uiOwnerDBSlot );

		pPlayer	=	pWorld->GetPlayerByDBAccount( siOwnerDBAccount, uiOwnerDBSlot );

		// 하루가 지났을때의 작업을 해준다.
		pIndustrialComplex->DayChanged( pPlayer, m_pIndustrialComplexInfo->uiCost );

		// 임금을 더한다.
		if (pIndustrialComplex->GetInfo(&IndustrialComplexInfo)) {
			
			maxpays[i] = IndustrialComplexInfo.m_moMaxPay;
		}
		else
		{
			maxpays[i] = 0;

		}

		totalpay += maxpays[i];

		
	}

	MONEY averagepay = totalpay / ON_MAX_INDUSTRIAL_COMPLEX_NUM;

	// 평균값을 넘는 최고임금의 합.
	MONEY totalpay2 = 0;

	for( i = 0; i < ON_MAX_INDUSTRIAL_COMPLEX_NUM; i++ )
	{
		if (maxpays[i] >= averagepay)
		{
			totalpay2 += maxpays[i];

		}
				
	}


	float workrate,tot,cur,temp;
	UI32 uiLaborForEach;

	tot = totalpay2;

	// 일을한다.
	for( i = 0; i < ON_MAX_INDUSTRIAL_COMPLEX_NUM; i++ )
	{
		if( ( pIndustrialComplex = GetIndustrialComplexOwned( i ) ) == NULL )		continue;		
	
		if (maxpays[i] != 0 && maxpays[i] >= averagepay) {
			cur = maxpays[i];			
			workrate = cur / tot;			
			temp = uiNPCLabor * workrate;			
			uiLaborForEach = temp;
			
			pIndustrialComplex->DoNPCWork(uiLaborForEach);
		}

		
		
	}


	
		
		
			
		

	
}


