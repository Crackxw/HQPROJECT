#include "SOWorld.h"
#include "SODatabase.h"
#include "OnlineItem.h"
#include "OnlineProductStructure.h"
#include "SOVillage_FactoryStructure.h"

CItem			*SOVillage_Factory::m_pItem;
SODatabase		*SOVillage_Factory::m_pDataBase;
OnlineBuilding	*SOVillage_Factory::m_pBuilding;

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	薑瞳 滲熱蒂 蟾晦�� и棻.
//	熱薑 橾濠	:	2002-10-04 螃�� 3:08:31 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
VOID	SOVillage_Factory::InitStaticVariable( CItem *pItem, SODatabase *pDataBase, OnlineBuilding *pBuilding )
{
	m_pItem			=	pItem;
	m_pDataBase		=	pDataBase;
	m_pBuilding		=	pBuilding;

	SOVillage_IndustrialComplex_Factory::InitStaticVariable( pDataBase, pBuilding );
}


//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	儅撩濠.
//	熱薑 橾濠	:	2002-10-04 螃�� 3:16:17 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
SOVillage_Factory::SOVillage_Factory()
{

}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	模資濠.
//	熱薑 橾濠	:	2002-10-04 螃�� 3:16:31 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
SOVillage_Factory::~SOVillage_Factory()
{

}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	蟾晦��.
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
BOOL	SOVillage_Factory::Init( UI32 uiVillageID )
{
	m_uiVillageID		=	uiVillageID;

	return	TRUE;
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	Ы溯檜橫陛 檜 勒僭縑 菟橫諮棻.
//	熱薑 橾濠	:	2002-10-04 螃�� 8:57:46 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
BOOL	SOVillage_Factory::EnterPlayer( SOPlayer *pPlayer, SI32 siIndustrialComplexID )
{ 
	SOVillage_IndustrialComplex		*pIndustrialComplex;
	BOOL							bEnteredIndustrialComplex = FALSE; 
	BOOL							bSuccess = FALSE;

	if( ( pIndustrialComplex = GetIndustrialComplex( siIndustrialComplexID ) ) == NULL )	return	FALSE;

	// 骯機 欽雖縑 菟橫除棻.
	if( pIndustrialComplex->EnterPlayer( pPlayer ) == FALSE )								return	FALSE;

	
	
	bEnteredIndustrialComplex	=	TRUE;

	
	bSuccess	=	TRUE;



	if( bSuccess == FALSE )
	{
		// 骯機 欽雖縑憮 緒螳 釭螞棻.
		if( bEnteredIndustrialComplex )		pIndustrialComplex->LeavePlayer( pPlayer );
	}

	return	bSuccess;

	/*
	stFarmData	*pFactoryData;

	if( uiPlantID > PLANT_CODE_NUM )										return	FALSE;			// 儅骯 衛撲僭 ID陛 嶸�褲捘� 匐餌и棻.
	
	if(	( pFactoryData = m_pFarmGroup->GetFarm( uiFarmID ) ) == NULL )		return	FALSE;			// 菟橫陛溥朝 儅骯 衛撲僭檜 嶸�褲捘� 匐餌и棻.			

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
	// 濛機濰縑 菟橫諮棻.
	pPlayer->stAccountInfo.uiStatusInVillage	=	INVILLAGE_STATUS_FACTORY;

	pPlayer->stAccountInfo.pStructure	=	pFarm;

	pPlayer->SendFieldMsg(ON_RESPONSE_FARM_SLOTINFO, SO_SFM_ONLYME, LPVOID(ON_RET_FARM_OK), LPVOID(pFarm));
	*/

	return TRUE;
}


//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	骯機 欽雖菟曖 薑爾蒂 Ы溯檜橫縑啪 爾頂遽棻.
//	熱薑 橾濠	:	2002-10-04 螃�� 3:16:45 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
BOOL	SOVillage_Factory::SendIndustrialComplexList( SOPlayer *pPlayer )
{
	OnIndustrialComplexInfo		IndustrialComplexInfo[ ON_MAX_INDUSTRIAL_COMPLEX_NUM ]; 
	UI32						uiIndustrialComplexNum;
	DWORD						dwInfoLength;
	
	dwInfoLength	=	GetAllIndustrialComplexInfo( IndustrialComplexInfo, &uiIndustrialComplexNum );

	pPlayer->SendFieldMsg( ON_RESPONSE_FARM_INFO, SO_SFM_ONLYME, LPVOID( uiIndustrialComplexNum ), LPVOID( IndustrialComplexInfo ), LPVOID( dwInfoLength ) );

	return	TRUE;
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	檜 葆擊縑 氈朝 賅萇 儅骯 衛撲僭擊 橢橫螞棻.
//	熱薑 橾濠	:	2002-10-04 螃�� 6:31:57 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
stFarmGroup*	SOVillage_Factory::GetFactories()
{
//	return	m_pFarmGroup;

	return	NULL;
}

/*
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	陛翕ж堅 氈雖 彊朝 勒僭擎 橈撻棻.
//	熱薑 橾濠	:	2002-10-04 螃�� 7:15:30 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
VOID	SOVillage_Factory::DismantleFactoryNotWork( SOWorld *pWorld )
{
	DBIKBuyFarmByForceInfo		stUpdateWorkDay;
	SI32						i;
	stFarmData					*pFactoryData;	
	MONEY						mnProductFacilityPrice;				// 儅骯 衛撲檜 п羹腎堅 嫡朝 絲.
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


//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	Ы溯檜橫陛 模嶸и 儅骯勒僭擊 橢橫螞棻.
//	熱薑 橾濠	:	2002-10-04 螃�� 7:27:48 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
stFarmData*		SOVillage_Factory::GetOwnFactory( SI32 siPlayerDBAccount, UI08 uiPlayerDBSlot )
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

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	儅骯 衛撲擊 掘殮и棻.
//	熱薑 橾濠	:	2002-10-04 螃�� 8:29:54 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
BOOL	SOVillage_Factory::BuyFactory( SOPlayer *pPlayer, UI08 uiPlantID, UI08 uiFarmID, MONEY *pmnCost )
{
	DBIKBuyFarmInfo					BuyFarmInfo;	
	stFarmData						*pFarm = NULL;
	stFarmData						*pFactoryData;	

	if( ( pFarm = m_pFarmGroup->GetFarm( uiFarmID ) ) == NULL)			return	FALSE;
	
	if( pFarm->siOwnerDBAcnt != 0 )										return	FALSE;		// 儅骯 衛撲僭曖 模嶸輿陛 氈朝雖 匐餌и棻.		
	if( pFarm->pPlant->siReqCredit > pPlayer->GetTradeGrade() )			return	FALSE;		// 儅骯 衛撲僭擊 掘殮ж晦嬪и 褐辨紫陛 腎朝雖 匐餌и棻. 		
	if( pFarm->pPlant->uiCost > pPlayer->siMoney)						return	FALSE;		// 儅骯 衛撲僭擊 掘殮ж晦嬪и 絲檜 腎朝雖 匐餌и棻.		
	if( pPlayer->GetOwnFactory() !=	NULL )								return	FALSE;		// 檜嘐 儅骯 衛撲僭擊 陛雖堅 氈朝雖 匐餌и棻.		
	
	BuyFarmInfo.siAccount		=	pPlayer->stAccountInfo.siDBAccount;
	BuyFarmInfo.uiSlot			=	pPlayer->stAccountInfo.uiSlot;
	BuyFarmInfo.uiVillageCode	=	m_uiVillageCode;
	BuyFarmInfo.uiFarmID		=	uiFarmID;
	BuyFarmInfo.uiPlantID		=	m_pFarmGroup->pPlant->DBIndex;	
	BuyFarmInfo.uiAvail			=	m_pFarmGroup->pPlant->uiInitSlot;		
	m_pDataBase->writeDBQuery( SO_DB_INFOKIND_FARM_BUY, &BuyFarmInfo, sizeof( BuyFarmInfo ) );

	// 憮幗紫 機等檜お и棻.	
	pFactoryData			=	&m_pFarmGroup->Farm[ uiFarmID ];

	pFactoryData->Init();
	pFactoryData->siOwnerDBAcnt		=	pPlayer->stAccountInfo.siDBAccount;
	pFactoryData->uiOwnerSlot		=	pPlayer->stAccountInfo.uiSlot;

	// 儅骯 衛撲僭曖 輿檣擊 撲薑и棻.
	ZeroMemory( pFactoryData->FarmInfo.szOwner, sizeof( pFactoryData->FarmInfo.szOwner ) );
	strncpy( pFactoryData->FarmInfo.szOwner, pPlayer->szID, ON_ID_LENGTH );	

	// Ы溯檜橫曖 絲擊 馬模衛鑑棻.
	*pmnCost				=	m_pFarmGroup->pPlant->uiCost;	

	return TRUE;	
}


//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	勒僭檜 鬼薯煎 つ溜棻.
//	熱薑 橾濠	:	2002-10-04 螃�� 8:46:43 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
BOOL	SOVillage_Factory::SellFactoryByForce( UI08 uiFarmID, MONEY *pmnMoney )
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

	// 憮幗紫 機等檜お и棻.
	pFarm->Init();	

	return TRUE;	
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	橾ж溥堅 ж朝 夠檜 嶸�褲� 夠檣雖 匐餌и棻.
//	熱薑 橾濠	:	2002-10-07 螃�� 2:23:08 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
BOOL	SOVillage_Factory::CanWork( UI08 uiIndustrialComplexID, UI08 uiFarmID )
{	
	stFarmData				*pIndustrialComplex;
	stFarmSlot				*pFarmSlot;	

	if( ( pIndustrialComplex = GetIndustrialComplex( uiIndustrialComplexID ) ) == NULL )		return	FALSE;		// 骯機 欽雖蒂 橢橫螞棻.
	
	if( ( pFarmSlot	= pIndustrialComplex->GetFarmSlot( uiSlot ) ) == NULL )		return	FALSE;		// 橾ж溥堅 ж朝 夠檜 嶸�褲� 夠檣雖 匐餌и棻.

	// 儅骯 衛撲 頂縑憮 橾擊 й熱 氈朝 遽綠陛 乾醮螳 氈朝雖 匐餌и棻.
	pFarmSlot->bWorkable	=	FALSE;

	if( pFarmSlot->bOnSchedule() == FALSE )														return	FALSE;		// 橾ж溥堅 ж朝 夠曖 濛機榆檜 陴嬴氈橫 濛機擊 й 熱 氈朝雖 匐餌и棻.
	
	// 璽堅縑 橾擊 ж晦 嬪и 紫掘陛 氈朝雖 匐餌и棻.
	if( CheckStorageTool( pFarmGroup->pPlant->DBIndex, pPlayer->stAccountInfo.pStructure->nIndex, pFarmSlot->ProductItem ) == FALSE )
		return	FALSE;

	// 濛機擊 й 熱 氈棻.
	pFarmSlot->bWorkable	=	TRUE;

	return TRUE;	
}
*/
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	嶸�褲� 骯機 欽雖曖 ID檣雖 憲嬴螞棻.
//	熱薑 橾濠	:	2002-10-07 螃�� 3:48:55 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
BOOL	SOVillage_Factory::IsValidIndustrialComplexID( SI32 siIndustrialComplexID )
{
	if( siIndustrialComplexID >= 0 && siIndustrialComplexID < ON_MAX_INDUSTRIAL_COMPLEX_NUM )		return	TRUE;

	return	FALSE;
}


//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	骯機 欽雖蒂 橢橫螞棻.
//	熱薑 橾濠	:	2002-10-07 螃�� 3:03:36 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
SOVillage_IndustrialComplex*	SOVillage_Factory::GetIndustrialComplex( SI32 siIndustrialComplexID )
{
	if( IsValidIndustrialComplexID( siIndustrialComplexID ) == FALSE )								return	NULL;

	return	&m_IndustrialComplex[ siIndustrialComplexID ];	
}


//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	賅萇 骯機 欽雖曖 薑爾蒂 橢橫螞棻.
//	熱薑 橾濠	:	2002-10-07 螃�� 7:20:48 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
DWORD	SOVillage_Factory::GetAllIndustrialComplexInfo( OnIndustrialComplexInfo *pIndustrialComplexInfo, UI32 *puiIndustrialComplexInfoNum )
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
			*puiIndustrialComplexInfoNum++;
		}			
	}

	return	dwInfoLength;
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	DB縑憮 橢橫螞 薑爾煎 骯機 欽雖蒂 蟾晦�� и棻.
//	熱薑 橾濠	:	2002-10-07 螃�� 9:24:05 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
BOOL	SOVillage_Factory::InitIndustrialComplexFromDB( SI32 siIndustrialComplexID, 
												SI32 siOwnerDBAccount, UI08 uiOwnerDBSlot, CHAR *pszOwnerName, 
												SI32 siMaxConstructibleFactoryNum, SI32 siDayNotWorked )
{
	SOVillage_IndustrialComplex		*pIndustrialComplex;

	if( ( pIndustrialComplex = GetIndustrialComplex( siIndustrialComplexID ) ) == NULL )	return	FALSE;
	
	pIndustrialComplex->InitFromDB( siOwnerDBAccount, uiOwnerDBSlot, pszOwnerName, siMaxConstructibleFactoryNum, siDayNotWorked );

	return	TRUE;
}


//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	DB縑憮 橢橫螞 薑爾煎 璽堅蒂 蟾晦�� и棻.
//	熱薑 橾濠	:	2002-10-07 螃�� 9:24:30 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
BOOL	SOVillage_Factory::InitStorageFromDB( SI32 siIndustrialComplexID, SI32 siFactoryID, UI08 uiLevel, MONEY mnPay, UI32 uiAmountOfWork, 
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


//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	DB縑憮 橢橫螞 薑爾煎 奢濰擊 蟾晦�� и棻.
//	熱薑 橾濠	:	2002-10-08 螃瞪 10:35:58 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
BOOL	SOVillage_Factory::InitFactoryFromDB( SI32 siIndustrialComplexID, SI32 siFactoryID, UI08 uiLevel, MONEY mnPay, UI32 uiAmountOfWork, CHAR cFactoryKind[ 4 ],
										UI16 uiProducingItemID, UI32 uiProducedItemQuantity, UI32 uiMaxProduceItemQuantity, 
										UI08 uiEquipment0Level, UI08 uiEquipment1Level, UI08 uiEquipment2Level, UI08 uiEquipment3Level, UI08 uiEquipment4Level )
{
	SOVillage_IndustrialComplex		*pIndustrialComplex;

	if( ( pIndustrialComplex = GetIndustrialComplex( siIndustrialComplexID ) ) == NULL )	return	FALSE;

	return	pIndustrialComplex->InitFactoryFromDB( siFactoryID, uiLevel, mnPay, uiAmountOfWork, cFactoryKind, 
													uiProducingItemID, uiProducingItemID, uiProducedItemQuantity, 
													uiEquipment0Level, uiEquipment1Level, uiEquipment2Level, uiEquipment3Level, uiEquipment4Level );		
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	奢濰擊 籀碟и棻.
//	熱薑 橾濠	:	2002-10-08 螃瞪 11:41:43 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
BOOL	SOVillage_Factory::SellFactory( SOPlayer *pPlayer, SI32 siFactoryID )
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


//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	奢濰縑憮 橾擊 ц棻.
//	熱薑 橾濠	:	2002-10-08 螃瞪 11:57:56 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
BOOL	SOVillage_Factory::WorkedInFactory( SOPlayer *pPlayer, UI32 uiAmountOfWork, MONEY *pmnPay )
{	
	SI32	i;
	
	for( i = 0; i < ON_MAX_INDUSTRIAL_COMPLEX_NUM; i++ )
	{
		if( m_IndustrialComplex[ i ].HasOwner() == TRUE )
		{
			if( m_IndustrialComplex[ i ].IsPlayerIn( pPlayer ) == TRUE )
			{
				m_IndustrialComplex[ i ].WorkedInFactory( pPlayer, uiAmountOfWork, pmnPay );
			}
		}
	}

	return	FALSE;
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	骯機 欽雖縑 氈朝 奢濰曖 薑爾蒂 Ы溯檜橫縑啪 爾頂遽棻.
//	熱薑 橾濠	:	2002-10-08 螃�� 2:17:55 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
BOOL	SOVillage_Factory::SendFactoryInfo( SOPlayer *pPlayer, SI32 siFactoryID )
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


//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	骯機 欽雖縑 氈朝 璽堅曖 薑爾蒂 Ы溯檜橫縑啪 爾頂遽棻.
//	熱薑 橾濠	:	2002-10-08 螃�� 3:16:13 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
BOOL	SOVillage_Factory::SendStorageInfo( SOPlayer *pPlayer, SI32 siFactoryID )
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