#include "SOMain.h"
#include "SOWorld.h"
#include "SODatabase.h"
#include "OnlineItem.h"
#include "OnlineProductStructure.h"
#include "SOVillage_IndustrialComplexsStructure.h"

CItem			*SOVillage_IndustrialComplexs::m_pItem;
SODatabase		*SOVillage_IndustrialComplexs::m_pDataBase;
OnlineBuilding	*SOVillage_IndustrialComplexs::m_pBuilding;

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	薑瞳 滲熱蒂 蟾晦�� и棻.
//	熱薑 橾濠	:	2002-10-04 螃�� 3:08:31 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
VOID	SOVillage_IndustrialComplexs::InitStaticVariable( CItem *pItem, SODatabase *pDataBase, OnlineBuilding *pBuilding )
{
	m_pItem			=	pItem;
	m_pDataBase		=	pDataBase;
	m_pBuilding		=	pBuilding;

	SOVillage_IndustrialComplex::InitStaticVariable( pItem, pDataBase, pBuilding );
	SOVillage_IndustrialComplex_Factory::InitStaticVariable( pItem, pDataBase, pBuilding );
}


//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	儅撩濠.
//	熱薑 橾濠	:	2002-10-04 螃�� 3:16:17 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
SOVillage_IndustrialComplexs::SOVillage_IndustrialComplexs()
{

}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	模資濠.
//	熱薑 橾濠	:	2002-10-04 螃�� 3:16:31 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
SOVillage_IndustrialComplexs::~SOVillage_IndustrialComplexs()
{

}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	蟾晦��.
//	熱薑 橾濠	:	2002-10-11 螃�� 12:01:18 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
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

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	詭賅葬 п薯.
//	熱薑 橾濠	:	2002-10-11 螃�� 12:01:23 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
VOID	SOVillage_IndustrialComplexs::Free()
{
	SI32	i;

	for( i = 0; i < ON_MAX_INDUSTRIAL_COMPLEX_NUM; i++ )
	{
		m_IndustrialComplex[ i ].Free();
	}

	SOVillage_BaseStructure::Free();
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	Ы溯檜橫陛 檜 勒僭縑 菟橫諮棻.
//	熱薑 橾濠	:	2002-10-04 螃�� 8:57:46 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
BOOL	SOVillage_IndustrialComplexs::EnterPlayer( SOPlayer *pPlayer, SI32 siIndustrialComplexID )
{ 
	SOVillage_IndustrialComplex		*pIndustrialComplex;			

	if( SOVillage_BaseStructure::EnterPlayer( pPlayer )	== FALSE )							return	FALSE;		

	if( ( pIndustrialComplex = GetIndustrialComplex( siIndustrialComplexID ) ) == NULL )	return	FALSE;

	// 骯機 欽雖縑 菟橫除棻.
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
//	撲貲		:	Ы溯檜橫陛 檜 勒僭縑憮 釭鬲棻.
//	熱薑 橾濠	:	2002-10-11 螃�� 1:25:51 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
BOOL	SOVillage_IndustrialComplexs::LeavePlayer( SOPlayer *pPlayer )
{

	SOVillage_IndustrialComplex		*pIndustrialComplex;

	// 骯機 欽雖縑 菟橫陛 氈棻賊 骯機欽雖縑憮紫 釭陞 熱 氈紫煙 п遽棻.
	if( ( pIndustrialComplex = GetEnteredIndustrialComplex( pPlayer ) ) != NULL )
	{
		pIndustrialComplex->LeavePlayer( pPlayer );
	}

	return	SOVillage_BaseStructure::LeavePlayer( pPlayer );
	
}	



//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	骯機 欽雖菟曖 薑爾蒂 Ы溯檜橫縑啪 爾頂遽棻.
//	熱薑 橾濠	:	2002-10-04 螃�� 3:16:45 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
BOOL	SOVillage_IndustrialComplexs::SendIndustrialComplexList( SOPlayer *pPlayer )
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
stFarmGroup*	SOVillage_IndustrialComplexs::GetFactories()
{
//	return	m_pFarmGroup;

	return	NULL;
}

/*
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	陛翕ж堅 氈雖 彊朝 勒僭擎 橈撻棻.
//	熱薑 橾濠	:	2002-10-04 螃�� 7:15:30 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
VOID	SOVillage_IndustrialComplexs::DismantleFactoryNotWork( SOWorld *pWorld )
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
*/
/*
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	Ы溯檜橫陛 模嶸и 儅骯勒僭擊 橢橫螞棻.
//	熱薑 橾濠	:	2002-10-04 螃�� 7:27:48 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
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
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	儅骯 衛撲擊 掘殮и棻.
//	熱薑 橾濠	:	2002-10-04 螃�� 8:29:54 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
BOOL	SOVillage_IndustrialComplexs::BuyFactory( SOPlayer *pPlayer, UI08 uiPlantID, UI08 uiFarmID, MONEY *pmnCost )
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
/*

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	勒僭檜 鬼薯煎 つ溜棻.
//	熱薑 橾濠	:	2002-10-04 螃�� 8:46:43 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
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

	// 憮幗紫 機等檜お и棻.
	pFarm->Init();	

	return TRUE;	
}
*/

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	橾ж溥堅 ж朝 夠檜 嶸�褲� 夠檣雖 匐餌и棻.
//	熱薑 橾濠	:	2002-10-07 螃�� 2:23:08 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
BOOL	SOVillage_IndustrialComplexs::CanWork( SOPlayer *pPlayer )
{		
	SOVillage_IndustrialComplex		*pIndustrialComplex;

	// 骯機 欽雖縑 菟橫陛 氈棻賊 骯機欽雖縑憮紫 釭陞 熱 氈紫煙 п遽棻.
	if( ( pIndustrialComplex = GetEnteredIndustrialComplex( pPlayer ) ) == NULL )		return	FALSE;
		
	return	pIndustrialComplex->CanWork( pPlayer );
	

	/*
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
	*/
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	嶸�褲� 骯機 欽雖曖 ID檣雖 憲嬴螞棻.
//	熱薑 橾濠	:	2002-10-07 螃�� 3:48:55 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
BOOL	SOVillage_IndustrialComplexs::IsValidIndustrialComplexID( SI32 siIndustrialComplexID )
{
	if( siIndustrialComplexID >= 0 && siIndustrialComplexID < ON_MAX_INDUSTRIAL_COMPLEX_NUM )		return	TRUE;

	return	FALSE;
}


//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	骯機 欽雖蒂 橢橫螞棻.
//	熱薑 橾濠	:	2002-10-07 螃�� 3:03:36 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
SOVillage_IndustrialComplex*	SOVillage_IndustrialComplexs::GetIndustrialComplex( SI32 siIndustrialComplexID )
{
	if( IsValidIndustrialComplexID( siIndustrialComplexID ) == FALSE )								return	NULL;

	return	&m_IndustrialComplex[ siIndustrialComplexID ];	
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	Ы溯檜橫陛 模嶸ж堅 氈朝 骯機 欽雖蒂 橢橫螞棻.
//	熱薑 橾濠	:	2002-10-15 螃�� 2:07:30 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
SOVillage_IndustrialComplex*	SOVillage_IndustrialComplexs::GetIndustrialComplexOwned( SI32 siIndustrialComplexID )
{
	SOVillage_IndustrialComplex		*pIndustrialComplex;

	if( ( pIndustrialComplex = GetIndustrialComplex( siIndustrialComplexID ) ) == NULL )		return	NULL;

	if( pIndustrialComplex->HasOwner() == FALSE )												return	NULL;
	
	return	pIndustrialComplex;
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	п渡 Ы溯檜橫陛 模嶸ж堅 氈朝 骯機 欽雖蒂 橢橫螞棻.
//	熱薑 橾濠	:	2002-10-10 螃�� 8:53:38 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
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

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	п渡 Ы溯檜橫陛 菟橫陛氈朝 骯機 欽雖蒂 橢橫螞棻.
//	熱薑 橾濠	:	2002-10-11 螃�� 1:27:13 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
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

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	п渡 Ы溯檜橫陛 菟橫陛氈朝 骯機 欽雖蒂 橢橫螞棻.
//	熱薑 橾濠	:	2002-10-11 螃�� 1:27:13 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
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


//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	賅萇 骯機 欽雖曖 薑爾蒂 橢橫螞棻.
//	熱薑 橾濠	:	2002-10-07 螃�� 7:20:48 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
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

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	DB縑憮 橢橫螞 薑爾煎 骯機 欽雖蒂 蟾晦�� и棻.
//	熱薑 橾濠	:	2002-10-07 螃�� 9:24:05 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
BOOL	SOVillage_IndustrialComplexs::InitIndustrialComplexFromDB( SI32 siIndustrialComplexID, 
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


//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	DB縑憮 橢橫螞 薑爾煎 奢濰擊 蟾晦�� и棻.
//	熱薑 橾濠	:	2002-10-08 螃瞪 10:35:58 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
BOOL	SOVillage_IndustrialComplexs::InitFactoryFromDB( SI32 siIndustrialComplexID, SI32 siFactoryID, UI08 uiLevel, MONEY mnPay, UI32 uiAmountOfWork, CHAR cFactoryKind[ 4 ],
										UI16 uiProducingItemID, UI32 uiProducedItemQuantity, UI32 uiMaxProduceItemQuantity)
{
	SOVillage_IndustrialComplex		*pIndustrialComplex;

	if( ( pIndustrialComplex = GetIndustrialComplex( siIndustrialComplexID ) ) == NULL )	return	FALSE;

	return	pIndustrialComplex->InitFactoryFromDB( siFactoryID, uiLevel, mnPay, uiAmountOfWork, cFactoryKind, 
													uiProducingItemID, uiProducedItemQuantity, uiMaxProduceItemQuantity );		
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	奢濰擊 籀碟и棻.
//	熱薑 橾濠	:	2002-10-08 螃瞪 11:41:43 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
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


//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	橾擊 部葆蟻棻.
//	熱薑 橾濠	:	2002-10-08 螃瞪 11:57:56 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
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
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	骯機 欽雖縑 氈朝 奢濰曖 薑爾蒂 Ы溯檜橫縑啪 爾頂遽棻.
//	熱薑 橾濠	:	2002-10-08 螃�� 2:17:55 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
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


//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	骯機 欽雖縑 氈朝 璽堅曖 薑爾蒂 Ы溯檜橫縑啪 爾頂遽棻.
//	熱薑 橾濠	:	2002-10-08 螃�� 3:16:13 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
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
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	骯機 欽雖蒂 掘殮и棻.
//	熱薑 橾濠	:	2002-10-08 螃�� 8:34:06 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
BOOL	SOVillage_IndustrialComplexs::BuyIndustrialComplex( SOPlayer *pPlayer, SI32 siIndustrialComplexID )
{
	SOVillage_IndustrialComplex		*pIndustrialComplex;	

	if( ( pIndustrialComplex = GetIndustrialComplex( siIndustrialComplexID ) ) == NULL )	return	FALSE;	

	if( m_pIndustrialComplexInfo->siReqCredit > pPlayer->GetTradeGrade() )			return	FALSE;		// 儅骯 衛撲僭擊 掘殮ж晦嬪и 褐辨紫陛 腎朝雖 匐餌и棻. 		
	if( m_pIndustrialComplexInfo->uiCost > pPlayer->siMoney)						return	FALSE;		// 儅骯 衛撲僭擊 掘殮ж晦嬪и 絲檜 腎朝雖 匐餌и棻.				
	
	if( pIndustrialComplex->BuyIndustrialComplex( pPlayer, m_pIndustrialComplexInfo->uiInitSlot ) == FALSE )		return	FALSE;

	// 綠辨擊 雖碳и棻.
	pPlayer->DecreaseMoney( m_pIndustrialComplexInfo->uiCost , FALSE);

	





	// DB縑 盪濰и棻.

	DBIKBuyFarmInfo				BuyFarmInfo;	
	BuyFarmInfo.siAccount		=	pPlayer->stAccountInfo.siDBAccount;
	BuyFarmInfo.uiSlot			=	pPlayer->stAccountInfo.uiSlot;
	BuyFarmInfo.uiVillageCode	=	pPlayer->stAccountInfo.pVillage->uiVillageCode;
	BuyFarmInfo.uiFarmID		=	siIndustrialComplexID;
	BuyFarmInfo.moMoney			=   m_pIndustrialComplexInfo->uiCost;


	
	
	m_pDataBase->writeDBQuery( SO_DB_INFOKIND_FARM_BUY, &BuyFarmInfo, sizeof( BuyFarmInfo ) );	

	// 撩奢瞳戲煎 骯機 欽雖蒂 掘殮ц棻.
	
	// 撩奢瞳戲煎 掘殮ц棻堅 憲溥遽棻.
	pPlayer->SendFieldMsg( ON_RESPONSE_FARM_BUY, SO_SFM_ONLYME, LPVOID( ON_RET_OK ) );

	return	TRUE;		
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	饜雖蒂 偃繫и棻.
//	熱薑 橾濠	:	2002-10-10 螃�� 5:31:43 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
BOOL	SOVillage_IndustrialComplexs::Reclamation( SOPlayer *pPlayer, SI32 siIndustrialComplexID, MONEY *pmnCapital )
{
	SOVillage_IndustrialComplex		*pIndustrialComplex;	

	if( ( pIndustrialComplex = GetIndustrialComplex( siIndustrialComplexID ) ) == NULL )				return	FALSE;	

	if( pIndustrialComplex->Reclamation( pPlayer, m_pIndustrialComplexInfo->uiMaxSlot ) == FALSE )		return	FALSE;

	// 撩奢瞳戲煎 饜雖蒂 偃繫ц棻.
	// 綠辨擊 雖碳и棻.
	pPlayer->DecreaseMoney( ON_PLANT_EXPANDSLOT_COST ,FALSE);

	// 葆擊曖 熱櫛擊 橢橫螞棻.
	*pmnCapital	=	MONEY( ON_PLANT_EXPANDSLOT_COST * ON_CAPITAL_RATE_PLANT );

	// 撩奢瞳戲煎 偃繫ц棻堅 憲溥遽棻.
	pPlayer->SendFieldMsg( ON_RESPONSE_FARM_EXPANDSLOT, SO_SFM_ONLYME, LPVOID( ON_RET_OK ) );

	return	TRUE;
}


//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	奢濰擊 勒撲и棻.
//	熱薑 橾濠	:	2002-10-10 螃�� 8:51:26 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
BOOL	SOVillage_IndustrialComplexs::ConstructFactory( SOPlayer *pPlayer, SI32 siFactoryID, CHAR cFactoryCode[ 4 ], MONEY mnPay, MONEY *pmnCapital )
{


	SOVillage_IndustrialComplex		*pIndustrialComplex;	

	if( ( pIndustrialComplex = GetOwnIndustrialComplex( pPlayer ) ) == NULL )										return	FALSE;		
	
	if( pIndustrialComplex->ConstructFactory( pPlayer, siFactoryID, cFactoryCode, mnPay, pmnCapital ) == FALSE )	return	FALSE;

	// 撩奢瞳戲煎 奢濰擊 勒撲ц棻.
	
		
	// 撩奢瞳戲煎 奢濰擊 勒撲ц棻堅 憲溥遽棻.
	pPlayer->SendFieldMsg( ON_RESPONSE_FARM_BUILDSTRUCT, SO_SFM_ONLYME, LPVOID( ON_RET_OK ) );

	return	TRUE;
}


//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	骯機 欽雖曖 奢濰縑 菟橫除棻.
//	熱薑 橾濠	:	2002-10-11 螃�� 4:36:34 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
BOOL	SOVillage_IndustrialComplexs::EnterFactory( SOPlayer *pPlayer, SI32 siFactoryID )
{
	SOVillage_IndustrialComplex		*pIndustrialComplex;

	// 檜 骯機 欽雖縑 菟橫諦 氈朝雖 匐餌и棻.
	if( ( pIndustrialComplex = GetEnteredIndustrialComplex( pPlayer ) ) == NULL )		return	FALSE;
	
	return	pIndustrialComplex->EnterFactory( pPlayer, siFactoryID );		
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	骯機 欽雖曖 奢濰縑憮 釭除棻.
//	熱薑 橾濠	:	2002-10-11 螃�� 4:48:28 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
BOOL	SOVillage_IndustrialComplexs::LeaveFactory( SOPlayer *pPlayer )
{
	SOVillage_IndustrialComplex		*pIndustrialComplex;

	// 檜 骯機 欽雖縑 菟橫諦 氈朝雖 匐餌и棻.
	if( ( pIndustrialComplex = GetEnteredIndustrialComplex( pPlayer ) ) == NULL )		return	FALSE;
	
	return	pIndustrialComplex->LeaveFactory( pPlayer );				
}


//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	橾擊 衛濛и棻.
//	熱薑 橾濠	:	2002-10-11 螃�� 7:24:58 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
BOOL	SOVillage_IndustrialComplexs::BeginWork( SOPlayer *pPlayer )
{
	SOVillage_IndustrialComplex		*pIndustrialComplex;

	// Ы溯檜橫陛 菟橫除 骯機 欽雖蒂 橢橫螞棻.
	if( ( pIndustrialComplex = GetEnteredIndustrialComplex( pPlayer ) ) == NULL )	
	{
		return	FALSE;
	}
	
	return	pIndustrialComplex->BeginWork( pPlayer );					
}


//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	奢濰曖 歜旎擊 滲唳и棻.
//	熱薑 橾濠	:	2002-10-11 螃�� 8:58:48 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
BOOL	SOVillage_IndustrialComplexs::ChangeFactoryPay( SOPlayer *pPlayer, MONEY mnPay, MONEY *pmnCapital )
{
	SOVillage_IndustrialComplex		*pIndustrialComplex;

	// Ы溯檜橫陛 菟橫除 骯機 欽雖蒂 橢橫螞棻.
	if( ( pIndustrialComplex = GetEnteredIndustrialComplex( pPlayer ) ) == NULL )		return	FALSE;

	// 骯機 欽雖曖 輿檣檣雖 匐餌и棻.
	if( pIndustrialComplex->IsOwner( pPlayer ) == FALSE )								return	FALSE;	

	return	pIndustrialComplex->ChangeFactoryPay( pPlayer, mnPay, pmnCapital );
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	⑷營 濛機醞檣 奢濰縑憮 濛機醞檣匙擊 鏃模и棻.
//	熱薑 橾濠	:	2002-10-12 螃�� 2:44:36 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
BOOL	SOVillage_IndustrialComplexs::CancelWork( SOPlayer *pPlayer, SI32 siFactoryID, MONEY *pmnCapital )
{
	SOVillage_IndustrialComplex		*pIndustrialComplex;

	// Ы溯檜橫陛 菟橫除 骯機 欽雖蒂 橢橫螞棻.
	if( ( pIndustrialComplex = GetEnteredIndustrialComplex( pPlayer ) ) == NULL )		return	FALSE;

	// 骯機 欽雖曖 輿檣檣雖 匐餌и棻.
	if( pIndustrialComplex->IsOwner( pPlayer ) == FALSE )								return	FALSE;	

	return	pIndustrialComplex->CancelWork( pPlayer, siFactoryID, pmnCapital );	
}


//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	奢濰擊 機斜溯檜萄 衛鑑棻.
//	熱薑 橾濠	:	2002-10-12 螃�� 5:49:40 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
BOOL	SOVillage_IndustrialComplexs::UpgradeFactory( SOPlayer *pPlayer, SI32 siFactoryID, MONEY mnPay, MONEY *pmnCapital )
{
	SOVillage_IndustrialComplex		*pIndustrialComplex;

	// Ы溯檜橫陛 菟橫除 骯機 欽雖蒂 橢橫螞棻.
	if( ( pIndustrialComplex = GetEnteredIndustrialComplex( pPlayer ) ) == NULL )		return	FALSE;

	// 骯機 欽雖曖 輿檣檣雖 匐餌и棻.
	if( pIndustrialComplex->IsOwner( pPlayer ) == FALSE )								return	FALSE;	

	return	pIndustrialComplex->UpgradeFactory( pPlayer, siFactoryID, mnPay, pmnCapital );		
}


//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	嬴檜蠱擊 儅骯и棻.
//	熱薑 橾濠	:	2002-10-12 螃�� 7:15:39 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
DWORD	SOVillage_IndustrialComplexs::ProduceItem( SOPlayer *pPlayer, SI32 siFactoryID, UI16 uiItemID, UI32 uiItemQuantity, MONEY mnPay, CHAR cNation, MONEY *pmnCapital )
{
	SOVillage_IndustrialComplex		*pIndustrialComplex;

	// Ы溯檜橫陛 菟橫除 骯機 欽雖蒂 橢橫螞棻.
	if( ( pIndustrialComplex = GetEnteredIndustrialComplex( pPlayer ) ) == NULL )		return	ON_RET_NO;

	// 骯機 欽雖曖 輿檣檣雖 匐餌и棻.
	if( pIndustrialComplex->IsOwner( pPlayer ) == FALSE )								return	ON_RET_FARM_NOTOWNER;		

	return	pIndustrialComplex->ProduceItem( pPlayer, siFactoryID, uiItemID, uiItemQuantity, mnPay, cNation, pmnCapital );		
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	璽堅 寰縑 嬴檜蠱擊 厥朝棻.
//	熱薑 橾濠	:	2002-10-14 螃瞪 11:25:37 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
BOOL	SOVillage_IndustrialComplexs::AddItem( SOPlayer *pPlayer, UI16 uiItemID, UI16 uiItemQuantity ,BOOL bSendToDB)
{
	SOVillage_IndustrialComplex		*pIndustrialComplex;

	// Ы溯檜橫陛 菟橫除 骯機 欽雖蒂 橢橫螞棻.
	if( ( pIndustrialComplex = GetEnteredIndustrialComplex( pPlayer ) ) == NULL )		return	FALSE;

	// 骯機 欽雖曖 輿檣檣雖 匐餌и棻.
	if( pIndustrialComplex->IsOwner( pPlayer ) == FALSE )								return	FALSE;		

	return	pIndustrialComplex->AddItem( pPlayer, uiItemID, uiItemQuantity ,bSendToDB);		
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	璽堅寰縑憮 嬴檜蠱擊 餉薯и棻.
//	熱薑 橾濠	:	2002-10-14 螃瞪 11:26:15 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
BOOL	SOVillage_IndustrialComplexs::DeleteItem( SOPlayer *pPlayer, UI16 uiItemID, UI16 uiItemQuantity ,BOOL bSendToDB)
{
	SOVillage_IndustrialComplex		*pIndustrialComplex;

	// Ы溯檜橫陛 菟橫除 骯機 欽雖蒂 橢橫螞棻.
	if( ( pIndustrialComplex = GetEnteredIndustrialComplex( pPlayer ) ) == NULL )		return	FALSE;

	// 骯機 欽雖曖 輿檣檣雖 匐餌и棻.
	if( pIndustrialComplex->IsOwner( pPlayer ) == FALSE )								return	FALSE;		

	return	pIndustrialComplex->DeleteItem( pPlayer, uiItemID, uiItemQuantity ,bSendToDB);		
}


//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	奢濰擊 だ惚и棻.
//	熱薑 橾濠	:	2002-10-15 螃瞪 11:12:21 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
BOOL	SOVillage_IndustrialComplexs::DestroyFactory( SOPlayer *pPlayer, SI32 siFactoryID, MONEY *pmnCapital )
{
	SOVillage_IndustrialComplex		*pIndustrialComplex;

	// Ы溯檜橫陛 菟橫除 骯機 欽雖蒂 橢橫螞棻.
	if( ( pIndustrialComplex = GetEnteredIndustrialComplex( pPlayer ) ) == NULL )		return	FALSE;

	// 骯機 欽雖曖 輿檣檣雖 匐餌и棻.
	if( pIndustrialComplex->IsOwner( pPlayer ) == FALSE )								return	FALSE;		

	return	pIndustrialComplex->DestroyFactory( pPlayer, siFactoryID, pmnCapital );			
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	ж瑞陛 雖陬棻.
//	熱薑 橾濠	:	2002-10-15 螃�� 2:03:18 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
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

	
	// 譆堅歜旎 寞衝
/*	for( i = 0; i < ON_MAX_INDUSTRIAL_COMPLEX_NUM; i++ )
	{
		if( ( pIndustrialComplex = GetIndustrialComplexOwned( i ) ) == NULL )		continue;		
	
		pIndustrialComplex->GetOwner( &siOwnerDBAccount, &uiOwnerDBSlot );

		pPlayer	=	pWorld->GetPlayerByDBAccount( siOwnerDBAccount, uiOwnerDBSlot );

		// ж瑞陛 雖陬擊陽曖 濛機擊 п遽棻.
		pIndustrialComplex->DayChanged( pPlayer, m_pIndustrialComplexInfo->uiCost );

		// 陛濰歜旎擊 號檜輿朝夠擊 掘п場朝棻 (蟾晦歜旎擊 0戲煎 п堡晦陽僥縑 濛機榆檜 橈朝夠擎 瓊雖彊朝棻.)
		if (pIndustrialComplex->GetInfo(&IndustrialComplexInfo)) {
			
			if (IndustrialComplexInfo.m_moMaxPay > maxPay)
			{
				siMaxPayComplexIndex = i;
				maxPay = IndustrialComplexInfo.m_moMaxPay;
				
			}
		}

		
	}

	// 橾擊и棻.
	if (siMaxPayComplexIndex >= 0 && uiNPCLabor > 0)
	{
		pIndustrialComplex = GetIndustrialComplexOwned( siMaxPayComplexIndex );
		if (pIndustrialComplex != NULL)
		{
			pIndustrialComplex->DoNPCWork(uiNPCLabor);
		}

	}*/

	// 譆堅歜旎 綠徽 寞衝

	MONEY totalpay = 0;

	

	for( i = 0; i < ON_MAX_INDUSTRIAL_COMPLEX_NUM; i++ )
	{
		if( ( pIndustrialComplex = GetIndustrialComplexOwned( i ) ) == NULL )		{
			maxpays[i] = 0;
			continue;		
		}
	
		pIndustrialComplex->GetOwner( &siOwnerDBAccount, &uiOwnerDBSlot );

		pPlayer	=	pWorld->GetPlayerByDBAccount( siOwnerDBAccount, uiOwnerDBSlot );

		// ж瑞陛 雖陬擊陽曖 濛機擊 п遽棻.
		pIndustrialComplex->DayChanged( pPlayer, m_pIndustrialComplexInfo->uiCost );

		// 歜旎擊 渦и棻.
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

	// ゎ敕高擊 剩朝 譆堅歜旎曖 м.
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

	// 橾擊и棻.
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


