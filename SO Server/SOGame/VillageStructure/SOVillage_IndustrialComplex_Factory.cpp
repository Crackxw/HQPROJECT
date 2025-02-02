#include "SOMain.h"
#include "SODatabase.h"
#include "SOVillage_IndustrialComplex_Factory.h"

CItem				*SOVillage_IndustrialComplex_Factory::m_pItem;
OnlineBuilding		*SOVillage_IndustrialComplex_Factory::m_pBuilding;
SODatabase			*SOVillage_IndustrialComplex_Factory::m_pDataBase;

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	儅撩濠.
//	熱薑 橾濠	:	2002-10-07 螃�� 5:00:42 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
SOVillage_IndustrialComplex_Factory::SOVillage_IndustrialComplex_Factory()
{
	m_StorageItem[ 0 ].uiItemCode	=	0;
	m_StorageItem[ 0 ].uiQuantity	=	0;

	m_StorageItem[ 1 ].uiItemCode	=	0;
	m_StorageItem[ 1 ].uiQuantity	=	0;

	m_StorageItem[ 2 ].uiItemCode	=	0;
	m_StorageItem[ 2 ].uiQuantity	=	0;

	m_StorageItem[ 3 ].uiItemCode	=	0;
	m_StorageItem[ 3 ].uiQuantity	=	0;

	m_StorageItem[ 4 ].uiItemCode	=	0;
	m_StorageItem[ 4 ].uiQuantity	=	0;

	m_StorageItem[ 5 ].uiItemCode	=	0;
	m_StorageItem[ 5 ].uiQuantity	=	0;

	m_StorageItem[ 6 ].uiItemCode	=	0;
	m_StorageItem[ 6 ].uiQuantity	=	0;

	m_StorageItem[ 7 ].uiItemCode	=	0;
	m_StorageItem[ 7 ].uiQuantity	=	0;

	m_StorageItem[ 8 ].uiItemCode	=	0;
	m_StorageItem[ 8 ].uiQuantity	=	0;

	m_StorageItem[ 9 ].uiItemCode	=	0;
	m_StorageItem[ 9 ].uiQuantity	=	0;

}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	模資濠.
//	熱薑 橾濠	:	2002-10-07 螃�� 5:00:50 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
SOVillage_IndustrialComplex_Factory::~SOVillage_IndustrialComplex_Factory()
{
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	蟾晦��.
//	熱薑 橾濠	:	2002-10-07 螃�� 6:02:14 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
VOID	SOVillage_IndustrialComplex_Factory::Init( UI16 uiVillageCode, SI32 siIndustrialComplexID, SI32 siFactoryID )
{
	m_uiVillageCode				=	uiVillageCode;
	m_siIndustrialComplexID		=	siIndustrialComplexID;
	m_siFactoryID				=	siFactoryID;

	ZeroMemory( m_cFactoryKind, sizeof( m_cFactoryKind ) );

	

	m_uiLevel					=	0;
	
	m_bStore					=	FALSE;

	m_PlayerMgr.Init( ON_MAX_FACTORY_PLAYER_NUM );
	m_WorkingPlayerMgr.Init( ON_MAX_FACTORY_PLAYER_NUM );

	ZeroMemory( m_StorageItem, sizeof( m_StorageItem ) );
	m_uiProducingItemID			=	0;
	m_uiMaxProduceItemQuantity	=	0;
	m_mnPay                     =   0;
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	奢濰擊 蟾晦�� и棻.
//	熱薑 橾濠	:	2002-10-15 螃�� 2:43:57 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
VOID	SOVillage_IndustrialComplex_Factory::InitFactory()
{
	m_uiLevel					=	0;
	ZeroMemory( m_cFactoryKind, sizeof( m_cFactoryKind ) );

	m_mnPay						=	0;

	m_uiRemainAmountOfWork		=	0;

	m_uiProducingItemID			=	0;
	
	m_uiMaxProduceItemQuantity	=	0;

	m_bStore					=	0;

	m_PlayerMgr.RemoveAllPlayer();
	m_WorkingPlayerMgr.RemoveAllPlayer();

	ZeroMemory( m_StorageItem, sizeof( m_StorageItem ) );


}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	п薯.
//	熱薑 橾濠	:	2002-10-07 螃�� 6:01:52 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
VOID	SOVillage_IndustrialComplex_Factory::Free()
{
	m_PlayerMgr.Free();
	m_WorkingPlayerMgr.Free();
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	濛機擊 蟾晦�� и棻
//	熱薑 橾濠	:	2002-10-12 螃�� 3:06:51 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
VOID	SOVillage_IndustrialComplex_Factory::InitWork()	
{
	m_mnPay						=	0;

	m_uiRemainAmountOfWork		=	0;
	
	m_uiProducingItemID			=	0;
	
	m_uiMaxProduceItemQuantity	=	0;			
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	Ы溯檜橫陛 菟橫諮棻.
//	熱薑 橾濠	:	2002-10-07 螃�� 5:56:56 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
BOOL	SOVillage_IndustrialComplex_Factory::EnterPlayer( SOPlayer *pPlayer, BOOL bIndustrialComplexOwner )
{
	if( m_PlayerMgr.AddPlayer( pPlayer ) == FALSE )		return	FALSE;
	

	if( SendFactoryInfo( pPlayer, bIndustrialComplexOwner ) == FALSE )
	{
		m_PlayerMgr.RemovePlayer( pPlayer );
		
		return	FALSE;

	}
	else
	{			
		return	TRUE;
	}
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	Ы溯檜橫陛 釭鬲棻.
//	熱薑 橾濠	:	2002-10-07 螃�� 5:59:17 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
BOOL	SOVillage_IndustrialComplex_Factory::LeavePlayer( SOPlayer *pPlayer )
{
	if( m_PlayerMgr.RemovePlayer( pPlayer ) == FALSE )		return	FALSE;	

	// ⑷營 濛機醞檜賊 濛機醞檣 鼻鷓縑憮紫 貍遽棻.
	// Ы溯檜橫陛 憮幗諦 翱唸檜 莒啣擊 衛蒂 渠綠и棻.
	if( m_WorkingPlayerMgr.IsAddedPlayer( pPlayer ) == TRUE )
	{	
		m_WorkingPlayerMgr.RemovePlayer( pPlayer );
	}
	
	pPlayer->SendFieldMsg( ON_RESPONSE_LEAVE_FACTORY, SO_SFM_ONLYME, LPVOID( ON_RET_OK ) );

	return	TRUE;
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	奢濰曖 薑爾蒂 橢橫螞棻.
//	熱薑 橾濠	:	2002-10-07 螃�� 5:26:55 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
DWORD	SOVillage_IndustrialComplex_Factory::GetInfo( OnFarmSlotList* pFactoryInfo )
{
	if( DidConstructFactory() == FALSE )		return	0;

	pFactoryInfo->SlotIndex		=		m_siFactoryID;
	pFactoryInfo->siPay			=		SI16( m_mnPay );
	pFactoryInfo->uiLabor		=		m_uiRemainAmountOfWork;
	pFactoryInfo->uiLevel		=		m_uiLevel;
	memcpy( pFactoryInfo->StructCode, m_cFactoryKind, sizeof( m_cFactoryKind ) );

	return	sizeof( OnFarmSlotList );
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	奢濰檜 勒撲腎歷朝雖 匐餌и棻.
//	熱薑 橾濠	:	2002-10-07 螃�� 5:28:34 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
BOOL	SOVillage_IndustrialComplex_Factory::DidConstructFactory()
{	
	if( m_cFactoryKind[ 0 ] == NULL )			return	FALSE;

	return	TRUE;
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	DB縑憮 橢橫螞 薑爾煎 璽堅蒂 蟾晦�� и棻.
//	熱薑 橾濠	:	2002-10-07 螃�� 9:33:16 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
BOOL	SOVillage_IndustrialComplex_Factory::InitStorageFromDB( UI08 uiLevel, MONEY mnPay, UI32 uiAmountOfWork, 
												UI16 uiItemID0, UI16 uiItemID1, UI16 uiItemID2, UI16 uiItemID3, UI16 uiItemID4, 
												UI16 uiItemID5, UI16 uiItemID6, UI16 uiItemID7, UI16 uiItemID8, UI16 uiItemID9, 
												UI16 uiQuantity0, UI16 uiQuantity1, UI16 uiQuantity2, UI16 uiQuantity3, UI16 uiQuantity4, 
												UI16 uiQuantity5, UI16 uiQuantity6, UI16 uiQuantity7, UI16 uiQuantity8, UI16 uiQuantity9 )
{
	stFacility		*pFacility;

	ZeroMemory( m_cFactoryKind, sizeof( m_cFactoryKind ) );
	strcpy( m_cFactoryKind, "STG" );

	if( ( pFacility = m_pBuilding->GetFacility( "STG" ) ) == NULL )		return	FALSE;

	m_uiLevel			=	uiLevel;
	m_mnPay				=	mnPay;
	

	m_uiRemainAmountOfWork			=	uiAmountOfWork;	

	
	m_StorageItem[ 0 ].uiItemCode	=	uiItemID0;
	m_StorageItem[ 0 ].uiQuantity	=	uiQuantity0;

	m_StorageItem[ 1 ].uiItemCode	=	uiItemID1;
	m_StorageItem[ 1 ].uiQuantity	=	uiQuantity1;

	m_StorageItem[ 2 ].uiItemCode	=	uiItemID2;
	m_StorageItem[ 2 ].uiQuantity	=	uiQuantity2;

	m_StorageItem[ 3 ].uiItemCode	=	uiItemID3;
	m_StorageItem[ 3 ].uiQuantity	=	uiQuantity3;

	m_StorageItem[ 4 ].uiItemCode	=	uiItemID4;
	m_StorageItem[ 4 ].uiQuantity	=	uiQuantity4;

	m_StorageItem[ 5 ].uiItemCode	=	uiItemID5;
	m_StorageItem[ 5 ].uiQuantity	=	uiQuantity5;

	m_StorageItem[ 6 ].uiItemCode	=	uiItemID6;
	m_StorageItem[ 6 ].uiQuantity	=	uiQuantity6;

	m_StorageItem[ 7 ].uiItemCode	=	uiItemID7;
	m_StorageItem[ 7 ].uiQuantity	=	uiQuantity7;

	m_StorageItem[ 8 ].uiItemCode	=	uiItemID8;
	m_StorageItem[ 8 ].uiQuantity	=	uiQuantity8;

	m_StorageItem[ 9 ].uiItemCode	=	uiItemID9;
	m_StorageItem[ 9 ].uiQuantity	=	uiQuantity9;

	m_bStore						=	TRUE;	

	if (m_uiRemainAmountOfWork)
	{
		m_uiProducingItemID				=	ON_PLANT_SLOTUPGRADE_CODE;	
	}
	else
	{
		m_uiProducingItemID             =   0;

	}


	

	
			
	return	TRUE;
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	DB縑憮 橢橫螞 薑爾煎 奢濰擊 蟾晦�� и棻.
//	熱薑 橾濠	:	2002-10-08 螃瞪 10:41:35 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
BOOL	SOVillage_IndustrialComplex_Factory::InitFactoryFromDB( UI08 uiLevel, MONEY mnPay, UI32 uiAmountOfWork, CHAR cFactoryKind[ 4 ],				
						UI16 uiProducingItemID, UI32 uiProducedItemQuantity, UI32 uiMaxProduceItemQuantity)
{
	stFacility		*pFacility;

	if( ( pFacility	 =	m_pBuilding->GetFacility( cFactoryKind ) ) == NULL )	return	FALSE;

	m_uiLevel					=	uiLevel;
	m_mnPay						=	mnPay;
	
	memcpy( m_cFactoryKind, cFactoryKind, sizeof( m_cFactoryKind ) );
	m_uiProducingItemID			=	uiProducingItemID;
	
	m_uiMaxProduceItemQuantity	=	uiMaxProduceItemQuantity;

	
	m_bStore					=	FALSE;

	if( m_mnPay > 0 )
	{	
		m_uiRemainAmountOfWork = uiAmountOfWork;
	}
	else
	{		
		m_uiProducingItemID			=	0;
		m_uiRemainAmountOfWork		=	0;
	}			

	return	TRUE;
}


//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	薑瞳 滲熱蒂 蟾晦�� и棻.		
//	熱薑 橾濠	:	2002-10-07 螃�� 9:49:44 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
VOID	SOVillage_IndustrialComplex_Factory::InitStaticVariable( CItem *pItem, SODatabase *pDataBase, OnlineBuilding *pBuilding )
{
	m_pItem			=	pItem;
	m_pDataBase		=	pDataBase;
	m_pBuilding		=	pBuilding;
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	奢濰擊 籀碟и棻.
//	熱薑 橾濠	:	2002-10-08 螃瞪 11:40:13 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
BOOL	SOVillage_IndustrialComplex_Factory::SellFactory( SOPlayer *pPlayer )
{
	return	FALSE;
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	Ы溯檜橫陛 檜 奢濰寰縑 菟橫諦 氈朝雖 匐餌и棻.
//	熱薑 橾濠	:	2002-10-08 螃�� 12:27:15 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
BOOL	SOVillage_IndustrialComplex_Factory::IsPlayerIn( SOPlayer *pPlayer )
{
	return	m_PlayerMgr.IsAddedPlayer( pPlayer );
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	橾擊 部葆蟻棻.
//	熱薑 橾濠	:	2002-10-08 螃�� 12:35:02 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
BOOL	SOVillage_IndustrialComplex_Factory::FinishWork( SOPlayer *pPlayer, SOVillage_IndustrialComplex *pComplex, OnReqFarmWorkMsg *pMsg )
{	
	MONEY					TempPay =0;		
	
	
	
	// 檜 奢濰縑憮 橾檜 陛棟и雖 匐餌и棻.
	if( CanWork() == FALSE )										return	FALSE;
	// 褒薯 橾擊 ж堅 氈朝 Ы溯檜橫檣雖 匐餌и棻.
	if( m_WorkingPlayerMgr.IsAddedPlayer( pPlayer ) == FALSE )		return	FALSE;
	// 橾擊 ж堅 氈朝 Ы溯檜橫 葬蝶お縑憮 餉薯и棻.
	m_WorkingPlayerMgr.RemovePlayer( pPlayer );	
	// 濛機榆檜 0檜賊 褒ぬ籀葬и棻.
	if( pMsg->siLabor == 0 )										return	FALSE;	
	// 濛機榆 п韁曖 辦溥陛 氈棻賊 Щ煎斜極 葬蝶お蒂 蹂羶ж堅 褒ぬ 籀葬и棻.
	if( !pPlayer->CheckLaborHack(pMsg->uiGameType, pMsg->siStep, pMsg->siLabor) )
	{
		writeInfoToFile( "LaborHackInfo.txt", "DBAccount(%d)  szID(%s)  uiGameType(%d)  siStep(%d)  siLabor(%d)",
						pPlayer->stAccountInfo.siDBAccount, pPlayer->szID, pMsg->uiGameType, pMsg->siStep, pMsg->siLabor );
		pPlayer->SendFieldMsg( ON_REQUEST_RUN_PROGRAM_LIST, SO_SFM_ONLYME, LPVOID(RUN_PROG_LIST_LABORHACK) );
		return FALSE;
	}


	// 纔蝶お 憮幗橾 唳辦縑朝 濛機榆曖 10寡虜躑 籀葬п遽棻.
	if( g_GameServerInfoParser.IsTestServer() )
	{
	 	pMsg->siLabor	*=	10;
	} 
	
	if( m_uiProducingItemID == ON_PLANT_SLOTUPGRADE_CODE )
	{
		// 勒撲 醞檜棻.

		if( pMsg->siLabor >= m_uiRemainAmountOfWork )
		{
			// 賅萇 濛機擊 部麻棻.			

			DBIKFarmWorkInfo            WorkInfo;

			TempPay					=	m_uiRemainAmountOfWork * m_mnPay;
			MONEY pay = pPlayer->IncreaseMoney( TempPay ,FALSE);	
			WorkInfo.DBAccount      =   pPlayer->stAccountInfo.siDBAccount;
			WorkInfo.uiSlot         =   pPlayer->stAccountInfo.uiSlot;
			WorkInfo.uiVillageCode	=	pPlayer->stAccountInfo.pVillage->uiVillageCode;
			WorkInfo.uiFarmID		=	m_siIndustrialComplexID;
			WorkInfo.uiFactoryID		=	m_siFactoryID;
			WorkInfo.Labor            = pMsg->siLabor;
			WorkInfo.moPay            = pay;		
			if( IsStorage() == TRUE )
			{	
				// 檜 奢濰擎 璽堅檜棻.

				m_pDataBase->writeDBQuery( SO_DB_INFOKIND_STORAGE_LEVELUP, &WorkInfo, sizeof( WorkInfo ) );
				//InitStorage();
				//pFarmSlot->Storage.Init();
			}
			else
			{	
				// 檜 奢濰擎 璽堅陛 嬴棲棻.
				m_pDataBase->writeDBQuery( SO_DB_INFOKIND_FARM_LEVELUP, &WorkInfo, sizeof( WorkInfo ) );				
				//InitFactroy();					
				//pFarmSlot->Structure.Init();
			}

			LevelUp( pPlayer );

		}
		else
		{

			// 嬴霜 賅萇 濛機擊 部頂雖 跤ц棻.

			DBIKFarmWorkInfo            WorkInfo;

			TempPay						=	pMsg->siLabor * m_mnPay;
			m_uiRemainAmountOfWork			-=		pMsg->siLabor;

			MONEY pay = pPlayer->IncreaseMoney( TempPay ,FALSE);	

			WorkInfo.DBAccount      =   pPlayer->stAccountInfo.siDBAccount;
			WorkInfo.uiSlot         =   pPlayer->stAccountInfo.uiSlot;

			WorkInfo.uiVillageCode	=	pPlayer->stAccountInfo.pVillage->uiVillageCode;
			WorkInfo.uiFarmID		=	m_siIndustrialComplexID;
			WorkInfo.uiFactoryID		=	m_siFactoryID;

			WorkInfo.Labor            = pMsg->siLabor;
			WorkInfo.moPay            = pay;
			
			if( m_bStore == TRUE )
			{	
				// 檜 奢濰擎 璽堅檜棻.

				m_pDataBase->writeDBQuery( SO_DB_INFOKIND_STORAGE_WORK, &WorkInfo, sizeof( WorkInfo ) );
			}
			else
			{				
				// 檜 奢濰擎 璽堅陛 嬴棲棻.

				m_pDataBase->writeDBQuery( SO_DB_INFOKIND_FARM_WORK, &WorkInfo, sizeof( WorkInfo ) );
			}

			
			

		}
		
	}
	// 嬴檜蠱 儅骯醞檜棻.
	else
	{
		

		
		if( pMsg->siLabor >= m_uiRemainAmountOfWork )
		{
			
			SI32 storageID,storageIndex;

			if (pComplex->CanAddItem(m_uiProducingItemID,m_uiMaxProduceItemQuantity,&storageID,&storageIndex))
			{
				
				DBIKFarmWorkEndInfo            WorkEndInfo;
				
				
				
				// и寞縑 棻釭螞棻.
				
				TempPay						=	m_uiRemainAmountOfWork * m_mnPay;
				
				WorkEndInfo.Labor            =  m_uiRemainAmountOfWork;
				
				m_uiRemainAmountOfWork      =   0;
				
				MONEY pay = pPlayer->IncreaseMoney( TempPay ,FALSE);	
				
				
				
				WorkEndInfo.DBAccount  =   pPlayer->stAccountInfo.siDBAccount;
				WorkEndInfo.uiSlot =   pPlayer->stAccountInfo.uiSlot;
				
				WorkEndInfo.uiVillageCode	=	pPlayer->stAccountInfo.pVillage->uiVillageCode;
				WorkEndInfo.uiFarmID		=	m_siIndustrialComplexID;
				WorkEndInfo.uiFactoryID		=	m_siFactoryID;
				
				
				WorkEndInfo.moPay            = pay;
				
				pComplex->AddItem(m_uiProducingItemID,m_uiMaxProduceItemQuantity,FALSE);
				
				WorkEndInfo.siStorageID = storageID;
				WorkEndInfo.siStoragePos = storageIndex;
				WorkEndInfo.uiItemCode = m_uiProducingItemID;
				WorkEndInfo.uiItemQnt = m_uiMaxProduceItemQuantity;

				m_pDataBase->writeDBQuery( SO_DB_INFOKIND_FARM_WORKEND, &WorkEndInfo, sizeof( WorkEndInfo ) );		
				
				
			}
			else			
			{

				return FALSE;
				

			}

			


			// 濛機擊 蟾晦�� и棻.
			WorkCompleted( pPlayer );

		}
		else
		{
			// 憮幗 機等檜お

			DBIKFarmWorkInfo            WorkInfo;

			TempPay						=	pMsg->siLabor * m_mnPay;
			m_uiRemainAmountOfWork			-=		pMsg->siLabor;

			MONEY pay = pPlayer->IncreaseMoney( TempPay ,FALSE);	

			WorkInfo.DBAccount      =   pPlayer->stAccountInfo.siDBAccount;
			WorkInfo.uiSlot         =   pPlayer->stAccountInfo.uiSlot;

			WorkInfo.uiVillageCode	=	pPlayer->stAccountInfo.pVillage->uiVillageCode;
			WorkInfo.uiFarmID		=	m_siIndustrialComplexID;
			WorkInfo.uiFactoryID		=	m_siFactoryID;

			WorkInfo.Labor            = pMsg->siLabor;
			WorkInfo.moPay            = pay;
						
			// DB 機等檜お			
			m_pDataBase->writeDBQuery( SO_DB_INFOKIND_FARM_WORK, &WorkInfo, sizeof(WorkInfo ) );
			
		}
		
		
	
		
	}
	

	pPlayer->SendFieldMsg( ON_RESPONSE_FARM_WORK, SO_SFM_ONLYME, LPVOID( ON_RET_OK ), LPVOID( m_uiRemainAmountOfWork ), LPVOID( &TempPay ) );

	return TRUE;		
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	葆擊輿團菟檜 橾擊 部葆蟻棻.
//	熱薑 橾濠	:	2002-10-08 螃�� 12:35:02 - 夢遽鷓
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
BOOL	SOVillage_IndustrialComplex_Factory::FinishWorkByNPC( SOVillage_IndustrialComplex *pComplex,UI32 uiAmountOfWork)
{	
	MONEY					TempPay =0;		
	
	
	
	// 檜 奢濰縑憮 橾檜 陛棟и雖 匐餌и棻.
	if( CanWork() == FALSE )										return	FALSE;
	// 橾и 曄擊 匐餌и棻.
	if( uiAmountOfWork == 0 )										return	FALSE;	

	
	if( m_uiProducingItemID == ON_PLANT_SLOTUPGRADE_CODE )
	{
		// 勒撲 醞檜棻.

		if( uiAmountOfWork >= m_uiRemainAmountOfWork )
		{
			// 賅萇 濛機擊 部麻棻.			

			DBIKFarmWorkInfo            WorkInfo;

			TempPay					=	m_uiRemainAmountOfWork * m_mnPay;
			MONEY pay               =   TempPay;	
			WorkInfo.uiVillageCode	=	pComplex->GetVillageCode();
			WorkInfo.uiFarmID		=	m_siIndustrialComplexID;
			WorkInfo.uiFactoryID	=	m_siFactoryID;
			WorkInfo.Labor          =   uiAmountOfWork;
			WorkInfo.moPay          =   pay;		
			if( IsStorage() == TRUE )
			{	
				// 檜 奢濰擎 璽堅檜棻.

				m_pDataBase->writeDBQuery( SO_DB_INFOKIND_STORAGE_LEVELUP_BYNPC, &WorkInfo, sizeof( WorkInfo ) );
			}
			else
			{	
				// 檜 奢濰擎 璽堅陛 嬴棲棻.
				m_pDataBase->writeDBQuery( SO_DB_INFOKIND_FARM_LEVELUP_BYNPC, &WorkInfo, sizeof( WorkInfo ) );				
			}

			// NPC陛 橾п憮 pPlayer 朝 NULL
			LevelUp( NULL );

		}
		else
		{

			// 嬴霜 賅萇 濛機擊 部頂雖 跤ц棻.

			DBIKFarmWorkInfo            WorkInfo;

			TempPay						=	uiAmountOfWork * m_mnPay;
			m_uiRemainAmountOfWork			-=		uiAmountOfWork;

			MONEY pay = TempPay;	

			WorkInfo.uiVillageCode	=	pComplex->GetVillageCode();
			WorkInfo.uiFarmID		=	m_siIndustrialComplexID;
			WorkInfo.uiFactoryID		=	m_siFactoryID;

			WorkInfo.Labor            = uiAmountOfWork;
			WorkInfo.moPay            = pay;
			
			if( m_bStore == TRUE )
			{	
				// 檜 奢濰擎 璽堅檜棻.

				m_pDataBase->writeDBQuery( SO_DB_INFOKIND_STORAGE_WORK_BYNPC, &WorkInfo, sizeof( WorkInfo ) );
			}
			else
			{				
				// 檜 奢濰擎 璽堅陛 嬴棲棻.

				m_pDataBase->writeDBQuery( SO_DB_INFOKIND_FARM_WORK_BYNPC, &WorkInfo, sizeof( WorkInfo ) );
			}

			
			

		}
		
	}
	// 嬴檜蠱 儅骯醞檜棻.
	else
	{
		

		
		
		
		if( uiAmountOfWork >= m_uiRemainAmountOfWork )
		{
			
			SI32 storageID,storageIndex;

			if (pComplex->CanAddItem(m_uiProducingItemID,m_uiMaxProduceItemQuantity,&storageID,&storageIndex))
			{
				
				DBIKFarmWorkEndInfo            WorkEndInfo;
				
				
				
				// и寞縑 棻釭螞棻.
				
				TempPay						=	m_uiRemainAmountOfWork * m_mnPay;
				
				WorkEndInfo.Labor            =  m_uiRemainAmountOfWork;
				
				m_uiRemainAmountOfWork      =   0;
				
				MONEY pay = TempPay;	
				
				WorkEndInfo.uiVillageCode	=	pComplex->GetVillageCode();
				WorkEndInfo.uiFarmID		=	m_siIndustrialComplexID;
				WorkEndInfo.uiFactoryID		=	m_siFactoryID;
				
				
				WorkEndInfo.moPay            = pay;
				
				pComplex->AddItem(m_uiProducingItemID,m_uiMaxProduceItemQuantity,FALSE);
				
				WorkEndInfo.siStorageID = storageID;
				WorkEndInfo.siStoragePos = storageIndex;
				WorkEndInfo.uiItemCode = m_uiProducingItemID;
				WorkEndInfo.uiItemQnt = m_uiMaxProduceItemQuantity;

				m_pDataBase->writeDBQuery( SO_DB_INFOKIND_FARM_WORKEND_BYNPC, &WorkEndInfo, sizeof( WorkEndInfo ) );		
				
				
			}
			else			
			{

				return FALSE;
				

			}

			


			// 濛機擊 蟾晦�� и棻 (NPC陛 橾п憮 pPlayer 朝 NULL).
			WorkCompleted( NULL );

		}
		else
		{
			// 憮幗 機等檜お

			DBIKFarmWorkInfo            WorkInfo;

			TempPay						=	uiAmountOfWork * m_mnPay;
			m_uiRemainAmountOfWork			-=		uiAmountOfWork;

			MONEY pay = TempPay;	

			WorkInfo.uiVillageCode	=	pComplex->GetVillageCode();
			WorkInfo.uiFarmID		=	m_siIndustrialComplexID;
			WorkInfo.uiFactoryID		=	m_siFactoryID;

			WorkInfo.Labor            = uiAmountOfWork;
			WorkInfo.moPay            = pay;
						
			// DB 機等檜お			
			m_pDataBase->writeDBQuery( SO_DB_INFOKIND_FARM_WORK_BYNPC, &WorkInfo, sizeof(WorkInfo ) );
			
		}
		
		
	
		
	}


	

	

	return TRUE;		
}


//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	溯漣擊 隸陛衛鑑棻.
//	熱薑 橾濠	:	2002-10-08 螃�� 12:52:49 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
VOID	SOVillage_IndustrialComplex_Factory::LevelUp( SOPlayer *pPlayerCompletedWork )
{	
	// 濛機檜 諫猿腎歷棻.
	WorkCompleted( pPlayerCompletedWork );

	m_uiLevel++;	
}



//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	骯機 欽雖縑 氈朝 奢濰曖 薑爾蒂 Ы溯檜橫縑啪 爾頂遽棻.
//	熱薑 橾濠	:	2002-10-08 螃�� 2:29:01 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
BOOL	SOVillage_IndustrialComplex_Factory::SendFactoryInfo( SOPlayer *pPlayer, BOOL bIndustrialComplexOwner )
{
	if( CanWork() == TRUE )
	{	
		// ⑷營 檜 奢濰縑憮 濛機擊 й 熱 氈棻賊 濛機曖 渠и 薑爾蒂 爾頂遽棻.
		if( m_uiProducingItemID == ON_PLANT_SLOTUPGRADE_CODE )
		{
			// 勒僭檜 勒撲醞檜棻.
			pPlayer->SendFieldMsg( ON_RESPONSE_FARM_WORKINFO, SO_SFM_ONLYME, LPVOID( ON_RET_OK ), LPVOID( m_uiProducingItemID ), 
																					LPVOID( m_uiRemainAmountOfWork ), LPVOID( &m_mnPay ) );
		}
		else
		{
			// 嬴檜蠱擊 儅骯醞檜棻.
			pPlayer->SendFieldMsg( ON_RESPONSE_FARM_WORKINFO, SO_SFM_ONLYME, LPVOID( ON_RET_OK ), LPVOID( m_uiProducingItemID ),	
																					LPVOID( m_uiMaxProduceItemQuantity ),
																					LPVOID( m_uiMaxProduceItemQuantity ),
																					LPVOID( &m_mnPay ) );
		}	
		
		return	TRUE;
	}
	else
	{
		// ⑷營 檜 奢濰縑憮朝 濛機擊 й 熱 橈棻.

		// 奢濰檜 璽堅檜堅 檜 奢濰檜 氈朝 骯機欽雖曖 輿檣檜塭賊 璽堅曖 薑爾蒂 爾頂遽棻.
		if( ( IsStorage() == TRUE ) && ( bIndustrialComplexOwner == TRUE ) )
		{
			// 璽堅曖 薑爾蒂 爾頂遽棻.
			pPlayer->SendFieldMsg( ON_RESPONSE_STORAGE_INFO, SO_SFM_ONLYME, LPVOID( ON_RET_OK ), LPVOID( m_StorageItem ) );		 
			
			return	TRUE;
		}
	}
	
	return	FALSE;
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	機斜溯檜萄陛 諫猿腎歷朝雖 匐餌и棻.
//	熱薑 橾濠	:	2002-10-12 螃�� 5:55:30 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
BOOL	SOVillage_IndustrialComplex_Factory::IsUpgradeCompleted()
{
	if( m_uiLevel < MAX_UPGRADESLOT_NUM )			return	FALSE;

	return	TRUE;
}
/*
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	骯機 欽雖縑 氈朝 璽堅曖 薑爾蒂 Ы溯檜橫縑啪 爾頂遽棻.
//	熱薑 橾濠	:	2002-10-08 螃�� 3:18:02 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
BOOL	SOVillage_IndustrialComplex_Factory::SendStorageInfo( SOPlayer *pPlayer )
{	
	if( IsStorage() == FALSE )		return	FALSE;	

	pPlayer->SendFieldMsg( ON_RESPONSE_STORAGE_INFO, SO_SFM_ONLYME, LPVOID( ON_RET_OK ), LPVOID( m_StorageItem ) );		 

	return	TRUE;
}*/


//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	檜 奢濰縑憮 橾擊 й 熱 氈朝雖 憲嬴螞棻. 
//	熱薑 橾濠	:	2002-10-08 螃�� 2:49:21 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
BOOL	SOVillage_IndustrialComplex_Factory::CanWork()
{	
	//
	if( m_mnPay <= 0 )
	{
		return	FALSE;
	}


	//陴擎 橾榆
	if( m_uiRemainAmountOfWork <= 0 )
	{
		return	FALSE;
	}

	return	TRUE;
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	檜 奢濰檜 璽堅檣雖 憲嬴螞棻.
//	熱薑 橾濠	:	2002-10-08 螃�� 3:19:22 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
BOOL	SOVillage_IndustrialComplex_Factory::IsStorage()
{
	return	m_bStore;	
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	檜 奢濰檜 橾奩瞳檣 奢濰檣雖 憲嬴螞棻.
//	熱薑 橾濠	:	2002-10-15 螃瞪 10:01:46 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
BOOL	SOVillage_IndustrialComplex_Factory::IsFactory()
{
	return	!m_bStore;	
}


//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	濛機榆擊 橢橫螞棻.
//	熱薑 橾濠	:	2002-10-08 螃�� 4:21:19 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
UI32	SOVillage_IndustrialComplex_Factory::GetAmountOfWork()
{
	if( CanWork() == FALSE )					return	0;

	return	m_uiRemainAmountOfWork;
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	奢濰擊 勒撲и棻.
//	熱薑 橾濠	:	2002-10-10 螃�� 9:14:52 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
BOOL	SOVillage_IndustrialComplex_Factory::ConstructFactory( SOPlayer *pPlayer, SI32 siFactoryID, CHAR cFactoryCode[ 4 ], MONEY mnPay, MONEY *pmnCapital )
{
	DBIKFarmBuildStructInfo				BuildStructInfo;	
	stFacility							*pFactoryInfo;		
	MONEY								mnTotalMoney;
	
	// 譆模 歜旎檜 10傖 檜鼻檜橫撿 и棻.
	if( mnPay < 10)																					return	FALSE;
		
	// 衛撲僭檜 嶸�褲捘� 匐餌и棻.
	if( ( pFactoryInfo = m_pBuilding->GetFacility( cFactoryCode ) ) == NULL)						return	FALSE;

	// 絲檜 醱碟и雖 匐餌и棻.
	mnTotalMoney		=	pFactoryInfo->UpLabor[ 0 ] * (mnPay + ON_TAX_PER_WORK);
	
	if( pPlayer->EnoughMoney( mnTotalMoney ) == FALSE )												return	FALSE;

	// 綠辨擊 雖碳и棻.
	pPlayer->DecreaseMoney( mnTotalMoney , FALSE);


	
		
	ZeroMemory( &BuildStructInfo, sizeof( BuildStructInfo ) );
	BuildStructInfo.uiVillageCode	=	pPlayer->stAccountInfo.pVillage->uiVillageCode;	
	BuildStructInfo.uiFarmID		=	m_siIndustrialComplexID;
	BuildStructInfo.uiFactoryID		=	m_siFactoryID;
	strcpy( BuildStructInfo.Code, pFactoryInfo->Code );	
	BuildStructInfo.moPay			=	mnPay;	
	BuildStructInfo.moTax           =   pFactoryInfo->UpLabor[ 0 ] * ON_TAX_PER_WORK;
	BuildStructInfo.moProfit        =   BuildStructInfo.moTax / 10;
	BuildStructInfo.uiWork          =   pFactoryInfo->UpLabor[ 0 ];
	BuildStructInfo.siAccount       =   pPlayer->stAccountInfo.siDBAccount;
	BuildStructInfo.uiSlot          =   pPlayer->stAccountInfo.uiSlot;

	
	
	m_uiProducingItemID						=	ON_PLANT_SLOTUPGRADE_CODE;
	memcpy( m_cFactoryKind, cFactoryCode, sizeof( m_cFactoryKind ) );
	m_mnPay									=	mnPay;
	
	m_uiLevel								=	0;	

	m_uiRemainAmountOfWork					=	pFactoryInfo->UpLabor[ 0 ];		

	*pmnCapital							=	BuildStructInfo.moProfit;

	// DB諦 憮幗縑 機等檜お и棻.
	if( strcmp( cFactoryCode, "STG" ) == 0 )
	{
		m_pDataBase->writeDBQuery( SO_DB_INFOKIND_STORAGE_BUILD, &BuildStructInfo, sizeof( BuildStructInfo ) );

		m_bStore						=	TRUE;		
		//pFarmSlot->Storage.Init();
	}
	else
	{
		//	DB縑憮 蝸煜塽 橾 й 啗�僚轀� 撮遴棻	
		m_pDataBase->writeDBQuery( SO_DB_INFOKIND_FARM_BUILDSTRUCT, &BuildStructInfo, sizeof( BuildStructInfo ) );

		m_bStore						=	FALSE;		
	

		m_uiMaxProduceItemQuantity		=	1;		
	}
	
		
	
	return	TRUE;
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	奢濰擊 だ惚и棻.
//	熱薑 橾濠	:	2002-10-12 螃�� 3:56:16 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
VOID	SOVillage_IndustrialComplex_Factory::DestroyFactory(  SOPlayer *pPlayer, MONEY *pmnCapital )
{
	
	if( IsStorage() == TRUE )
	{
		DBIKDestroyStorage		DS;

		// DB縑 盪濰и棻.
		DS.DBAccount		=	pPlayer->stAccountInfo.siDBAccount;
		DS.uiSlot			=	pPlayer->stAccountInfo.uiSlot;
		DS.uiVillageCode	=	pPlayer->stAccountInfo.pVillage->uiVillageCode;	
		DS.uiFarmID			=	m_siIndustrialComplexID;
		DS.uiFactoryID		=	m_siFactoryID;
	
		m_pDataBase->writeDBQuery( SO_DB_INFOKIND_FARM_DESTROY_STORAGE,&DS, sizeof( DS ) );
	}
	else
	{
		DBIKDestroyStorage		DF;

		DF.DBAccount		=	pPlayer->stAccountInfo.siDBAccount;
		DF.uiSlot			=	pPlayer->stAccountInfo.uiSlot;
		DF.uiVillageCode	=	pPlayer->stAccountInfo.pVillage->uiVillageCode;	
		DF.uiFarmID			=	m_siIndustrialComplexID;
		DF.uiFactoryID		=	m_siFactoryID;
	
		m_pDataBase->writeDBQuery( SO_DB_INFOKIND_FARM_DESTROY_FACTORY, &DF, sizeof( DF ) );
	}

	// 貍撿 ж朝 熱櫛旎擊 橢橫螞棻.
//	*pmnCapital				=	( m_uiRemainAmountOfWork * m_mnPay) * ON_CAPITAL_RATE_PLANT; 

	// Ы溯檜橫縑啪 詭衛雖蒂 爾頂遽棻.
	if( pPlayer != NULL )	pPlayer->SendFieldMsg( ON_RESPONSE_FARM_DESTROYSTRUCT, SO_SFM_ONLYME, LPVOID( ON_RET_OK ) );	

	// 奢濰擊 蟾晦�� и棻.
	InitFactory();

}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	橾擊 衛濛и棻.
//	熱薑 橾濠	:	2002-10-11 螃�� 7:27:06 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
BOOL	SOVillage_IndustrialComplex_Factory::BeginWork( SOPlayer *pPlayer )
{
	// 橾擊 й 熱 氈朝雖 匐餌и棻.
	if( CanWork() == FALSE )
	{
		return	FALSE;
	}
	
	if( m_WorkingPlayerMgr.AddPlayer( pPlayer ) == FALSE )
	{
		return	FALSE;
	}

	pPlayer->SendFieldMsg( ON_RESPONSE_FARM_WORKSTART, SO_SFM_ONLYME, LPVOID( ON_RET_OK ) );

	return	TRUE;
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	奢濰曖 歜旎擊 滲唳и棻.
//	熱薑 橾濠	:	2002-10-11 螃�� 9:00:24 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
BOOL	SOVillage_IndustrialComplex_Factory::ChangeFactoryPay( SOPlayer *pPlayer, MONEY mnPay, MONEY *pmnCapital )
{	
	MONEY						mnDifference;
	DBIKChangeFarmPay			stChangePay;

	
	// 濛機擊 й 熱 氈朝 奢濰檣雖 匐餌и棻.
	if( CanWork() == FALSE )																return	FALSE;

	if( mnPay < 10 )																		return	FALSE;
	
	mnDifference			=	(MONEY)( mnPay - m_mnPay ) * m_uiRemainAmountOfWork;

	if (mnDifference > 0)
	{
		if( pPlayer->EnoughMoney( mnDifference ) == FALSE )										return	FALSE;	// Ы溯檜橫縑啪 滲唳ж晦 嬪и 醱碟и 絲檜 橈棻.
	}
	else if(mnDifference < 0)
	{
		if(pPlayer->GetMaxMoneyToGet() < abs(mnDifference))
			return FALSE;
	}
	

	stChangePay.uiVillageCode	=	pPlayer->stAccountInfo.pVillage->uiVillageCode;	
	stChangePay.uiFarmID		=	m_siIndustrialComplexID;
	stChangePay.uiFactoryID		=	m_siFactoryID;
	stChangePay.moPay			=	mnPay;	
	stChangePay.siDBAccount		=	pPlayer->stAccountInfo.siDBAccount;
	stChangePay.uiSlot			=	pPlayer->stAccountInfo.uiSlot;
	
	

	m_mnPay					=	mnPay;	
	
	if( mnDifference > 0 )	{
		pPlayer->DecreaseMoney( mnDifference ,FALSE);
		stChangePay.moDifference = -mnDifference;
	}
	else					{
		MONEY ret = pPlayer->IncreaseMoney( -mnDifference ,FALSE);
		stChangePay.moDifference = ret;
	}

	if (IsStorage()) {

		m_pDataBase->writeDBQuery( SO_DB_INFOKIND_STORAGE_CHANGE_PAY, LPVOID( &stChangePay ), sizeof( DBIKChangeFarmPay ) );
	}
	else
	{
		m_pDataBase->writeDBQuery( SO_DB_INFOKIND_PLANT_CHANGE_PAY, LPVOID( &stChangePay ), sizeof( DBIKChangeFarmPay ) );

	}
	
	
	
	pPlayer->SendFieldMsg( ON_RESPONSE_PLANT_CHANGE_PAY, SO_SFM_ONLYME, LPVOID( ON_RET_OK ), LPVOID( &pPlayer->siMoney ) );

	return TRUE;
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	⑷營 濛機醞檣 奢濰縑憮 濛機醞檣匙擊 鏃模и棻.
//	熱薑 橾濠	:	2002-10-12 螃�� 2:50:09 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
BOOL	SOVillage_IndustrialComplex_Factory::CancelWork( SOPlayer *pPlayer, SI32 siFactoryID, MONEY *pmnCapital )
{
	MONEY					mnDifference;
	DBIKFarmCancelPlan	FarmCancelData;

	// 雖旎擎 橾檜 陛棟и 鼻鷓檣雖 匐餌и棻.	
	if( CanWork() == FALSE )				return	FALSE;

	mnDifference					=	m_mnPay * m_uiRemainAmountOfWork;

	FarmCancelData.DBAccount        =   pPlayer->stAccountInfo.siDBAccount;
	FarmCancelData.uiSlot           =   pPlayer->stAccountInfo.uiSlot;
	FarmCancelData.uiVillageCode	=	pPlayer->stAccountInfo.pVillage->uiVillageCode;	
	FarmCancelData.uiFarmID			=	m_siIndustrialComplexID;
	FarmCancelData.uiFactoryID		=	m_siFactoryID;

	MONEY ret = pPlayer->IncreaseMoney( mnDifference ,FALSE);

	FarmCancelData.moPay = ret;


	if( m_bStore )
	{
		m_pDataBase->writeDBQuery( SO_DB_INFOKIND_PLANT_CANCEL_STORAGE, &FarmCancelData, sizeof( FarmCancelData ) );
	}
	else
	{	

		m_pDataBase->writeDBQuery( SO_DB_INFOKIND_PLANT_CANCEL_PLAN, &FarmCancelData, sizeof( FarmCancelData ) );		
	}
		

	// 奢濰檜 勒撲醞檜塭賊 奢濰擊 橈撻棻.
	if( IsConstructing() == TRUE )	
	{
		WorkCompleted( pPlayer );	


		// Ы溯檜橫縑啪 詭衛雖蒂 爾頂遽棻.
		//if( pPlayer != NULL )	pPlayer->SendFieldMsg( ON_RESPONSE_FARM_DESTROYSTRUCT, SO_SFM_ONLYME, LPVOID( ON_RET_OK ) );			

		// 奢濰擊 蟾晦�� и棻.
		InitFactory();		
	}	
	else
	{

		WorkCompleted( pPlayer );	

	}
	
	pPlayer->SendFieldMsg( ON_RESPONSE_PLANT_CANCEL_PLAN, SO_SFM_ONLYME, LPVOID( ON_RET_OK ), LPVOID( &pPlayer->siMoney ) );	

	return	TRUE;
}


//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	勒僭檜 勒撲醞檣雖 憲嬴螞棻.
//	熱薑 橾濠	:	2002-10-12 螃�� 3:58:30 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
BOOL	SOVillage_IndustrialComplex_Factory::IsConstructing()
{
	if( ( m_uiLevel == 0 ) && ( IsUpgrading() == TRUE ) )		return	TRUE;
	else														return	FALSE;
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	奢濰擊 機斜溯檜萄醞檣雖 憲嬴螞棻.
//	熱薑 橾濠	:	2002-10-15 螃瞪 10:04:27 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
BOOL	SOVillage_IndustrialComplex_Factory::IsUpgrading()
{
	if( m_uiProducingItemID != ON_PLANT_SLOTUPGRADE_CODE )		return	FALSE;

	return	TRUE;
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	奢濰擊 機斜溯檜萄 衛鑑棻.
//	熱薑 橾濠	:	2002-10-12 螃�� 5:51:30 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
BOOL	SOVillage_IndustrialComplex_Factory::UpgradeFactory( SOPlayer *pPlayer, MONEY mnPay, MONEY *pmnCapital )
{
	DBIKSetUpgradeSlot				SetUpgradeSlot;
	stFacility						*pFacility;
	MONEY							mnTotalPay;
	UI32							uiAmountOfWork;


	// ⑷營 橾擊 й 熱 氈朝 鼻鷓塭賊 機斜溯檜蒂 й 熱 橈棻.
	if( CanWork() == TRUE )																			return	FALSE;

	// 譆模 歜旎檜 10傖 檜鼻檜橫撿 и棻.
	if( mnPay < 10)																					return	FALSE;

	if( IsUpgradeCompleted() == TRUE )																return	FALSE;

	if( ( pFacility	 =	m_pBuilding->GetFacility( m_cFactoryKind ) ) == NULL )						return	FALSE;	

	memset(&SetUpgradeSlot, 0, sizeof(DBIKSetUpgradeSlot));	
	
	uiAmountOfWork			=	pFacility->UpLabor[ m_uiLevel ];	
	
	mnTotalPay				=	mnPay * uiAmountOfWork + uiAmountOfWork * ON_TAX_PER_WORK;
	
	if( pPlayer->EnoughMoney( mnTotalPay ) == FALSE )												return	FALSE;

	pPlayer->DecreaseMoney( mnTotalPay ,FALSE);	

	// 憮幗 機等檜お.	
	m_mnPay						=	mnPay;
	m_uiProducingItemID			=	ON_PLANT_SLOTUPGRADE_CODE;
	m_uiMaxProduceItemQuantity	=	1;
	
	m_uiRemainAmountOfWork		=	uiAmountOfWork;

	// 葆擊曖 熱櫛旎.
	*pmnCapital					=	uiAmountOfWork * ON_TAX_PER_WORK;

	

	
	
	// DB縑 殮溘.
	SetUpgradeSlot.siDBAccount		=   pPlayer->stAccountInfo.siDBAccount;
	SetUpgradeSlot.uiSlot			=   pPlayer->stAccountInfo.uiSlot;
	SetUpgradeSlot.uiVillageCode	=	pPlayer->stAccountInfo.pVillage->uiVillageCode;	
	SetUpgradeSlot.uiFarmID			=	m_siIndustrialComplexID;
	SetUpgradeSlot.uiFactoryID		=	m_siFactoryID;
	SetUpgradeSlot.moPay 			=	mnPay ;
	SetUpgradeSlot.moTax			=   uiAmountOfWork * ON_TAX_PER_WORK;
	SetUpgradeSlot.moCapital        =   *pmnCapital;
	SetUpgradeSlot.Labor			=	uiAmountOfWork;
	/*SetUpgradeSlot.UpCode			=	ON_PLANT_SLOTUPGRADE_CODE;
	SetUpgradeSlot.Quantity			=	1;					*/
	
	m_pDataBase->writeDBQuery( SO_DB_INFOKIND_FARM_UPGRADESLOT, &SetUpgradeSlot, sizeof( SetUpgradeSlot ) );

	pPlayer->SendFieldMsg( ON_RESPONSE_FARM_UPGRADESLOT, SO_SFM_ONLYME, LPVOID( ON_RET_OK ) );	

	return	TRUE;
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	嬴檜蠱擊 儅骯и棻.
//	熱薑 橾濠	:	2002-10-12 螃�� 7:17:40 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
DWORD	SOVillage_IndustrialComplex_Factory::ProduceItem( SOPlayer *pPlayer, UI16 uiItemID, UI32 uiItemQuantity, MONEY mnPay, CHAR cNation, MONEY *pmnCapital )
{
	stFacility						*pFacility;
	DBIKSetFarmProduct				SetFarmProduct;
	MONEY							mnTotalPay,moTax;
	UI32							uiAmountOfWork;

	// 譆模 歜旎檜 10傖 檜鼻檜橫撿 и棻.
	if( mnPay < 10)
	{
		return	ON_RET_NO;
	}
	if( uiItemQuantity == 0 )
	{
		return	ON_RET_NO;
		//return	ON_RET_NO;
	}
	if( IsStorage() == TRUE )
	{
		return	ON_RET_NO;
	}
	if( CanWork() == TRUE )
	{
		return	ON_RET_NO;
	}
	if( ( pFacility	 =	m_pBuilding->GetFacility( m_cFactoryKind ) ) == NULL )
	{
		return	ON_RET_FARM_ITEMERROR;
	}
	if( m_pBuilding->GetIndex( m_cFactoryKind, uiItemID, m_uiLevel, cNation ) == -1 )								
	{
		return	ON_RET_NO;
	}
	if( ( uiAmountOfWork = GetAmountOfWorkByQuantity( uiItemID, uiItemQuantity ) ) == 0)							
	{
		return	ON_RET_NO;
	}


	moTax       =   uiAmountOfWork * ON_TAX_PER_WORK;
	
	mnTotalPay	=	mnPay * uiAmountOfWork + moTax;

	if( pPlayer->EnoughMoney( mnTotalPay ) == FALSE )
	{
		return	ON_RET_FARM_MONEY;	
	}
	
//	if( HasItemMaterialInStorage( pPlayer->stAccountInfo.pStructure, uiItem, Quantity) == FALSE )					return	ON_RET_FARM_MATERIAL;
//	if( HasItemToolInStorage( 0, pPlayer->stAccountInfo.pStructure->nIndex, uiItem) == FALSE)	return	ON_RET_FARM_TOOL;
//	SpendMaterial(pPlayer->stAccountInfo.pStructure->nIndex, uiItem, Quantity);

	// 啗�嘛� 虜萇棻. - DB縑 殮溘и棻.
	SetFarmProduct.siDBAccount              =   pPlayer->stAccountInfo.siDBAccount;
	SetFarmProduct.uiSlot                   =   pPlayer->stAccountInfo.uiSlot;
	SetFarmProduct.uiVillageCode		    =	pPlayer->stAccountInfo.pVillage->uiVillageCode;
	SetFarmProduct.uiFarmID					=	m_siIndustrialComplexID;
	SetFarmProduct.uiFactoryID				=	m_siFactoryID;
	SetFarmProduct.uiItemID					=	uiItemID;
	SetFarmProduct.Quantity					=	uiItemQuantity;			// 虜菟 榆
	SetFarmProduct.moPay					=	mnPay;			// 歜旎
	SetFarmProduct.Labor                    =   uiAmountOfWork;
	SetFarmProduct.moTax                    =   moTax;
	SetFarmProduct.moProfit                 =   moTax;
	

	m_pDataBase->writeDBQuery( SO_DB_INFOKIND_FARM_PRODUCTITEM, &SetFarmProduct, sizeof( SetFarmProduct ) );

	m_uiProducingItemID						=	uiItemID;	

	m_uiMaxProduceItemQuantity				=	uiItemQuantity;	
	m_mnPay									=	mnPay;
	
	m_uiRemainAmountOfWork					=	uiAmountOfWork;	

	// 葆擊曖 熱櫛旎.
	*pmnCapital								=	moTax;

	pPlayer->DecreaseMoney( mnTotalPay ,FALSE);

	pPlayer->SendFieldMsg( ON_RESPONSE_FARM_ITEMPRODUCT, SO_SFM_ONLYME, LPVOID( ON_RET_OK ) );

	// 檜 奢濰寰縑 Ы溯檜橫蒂 厥橫遽棻.
	m_PlayerMgr.AddPlayer( pPlayer );

	return ON_RET_OK;		
}


//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	嬴檜蠱 偃熱蒂 喻翕榆戲煎 夥羞棻.
//	熱薑 橾濠	:	2002-10-12 螃�� 7:56:15 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
UI32	SOVillage_IndustrialComplex_Factory::GetAmountOfWorkByQuantity( UI16 uiItemID, UI32 uiItemQuantity )
{
	UI32			uiAmountOfWork = 0;
	MakeItem		*pItemCompose	=	NULL;
	CItemHeader		*pItemHeader	=	NULL;

	if( ( pItemHeader = m_pItem->GetItem( uiItemID ) ) == NULL )			return	0;
	if( ( pItemCompose = pItemHeader->pMakeItem ) == NULL )					return	0;

	uiAmountOfWork = ( uiItemQuantity / pItemCompose->m_siProduction) * ( pItemCompose->m_siLabor );
	if( uiItemQuantity % pItemCompose->m_siProduction != 0)	
		uiAmountOfWork += pItemCompose->m_siLabor;
	
	return uiAmountOfWork;
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	喻翕榆擊 嬴檜蠱曖 偃熱煎 夥羞棻.
//	熱薑 橾濠	:	2002-10-14 螃�� 7:23:53 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
UI32	SOVillage_IndustrialComplex_Factory::GetItemQuantityByAmountOfWork( UI16 uiItemID, UI32 *puiAmountOfWork )
{
	UI32			uiItemQuantity;
	MakeItem		*pItemCompose	=	NULL;
	CItemHeader		*pItemHeader	=	NULL;

	if( ( pItemHeader = m_pItem->GetItem( uiItemID ) ) == NULL )			return	0;
	if( ( pItemCompose = pItemHeader->pMakeItem ) == NULL )					return	0;

	uiItemQuantity = ( ( *puiAmountOfWork ) / ( pItemCompose->m_siLabor ) ) * pItemCompose->m_siProduction;
	( *puiAmountOfWork )  = ( *puiAmountOfWork ) % ( pItemCompose->m_siLabor );

	return uiItemQuantity;
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	璽堅縑 氈朝 嬴檜蠱擊 橢橫螞棻.
//	熱薑 橾濠	:	2002-10-12 螃�� 8:56:28 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
STORAGE_ITEM*		SOVillage_IndustrialComplex_Factory::GetItem( UI16 uiItemID, SI32 *psiIndex )
{
	SI32	i;

	if( IsStorage() == FALSE )			return	NULL;

	for( i = 0; i < ON_MAX_STORAGEITEM; i++ )
	{
		// 0爾棻朝 鼠褻勒 醴撿и棻.
		if( m_StorageItem[ i ].uiItemCode == uiItemID && m_StorageItem[i].uiQuantity > 0)
		{
			if( psiIndex != NULL )			*psiIndex	=	i;

			return	&m_StorageItem[ i ];
		}
	}

	return	NULL;
}



//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	璽堅縑 綠橫氈朝 嬴檜蠱擊 橢橫螞棻.
//	熱薑 橾濠	:	2002-10-14 螃瞪 11:08:03 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
STORAGE_ITEM*		SOVillage_IndustrialComplex_Factory::GetEmptyItem( SI32 *psiIndex )
{
	SI32	i;

	if( IsStorage() == FALSE )			return	NULL;

	for( i = 0; i < ON_MAX_STORAGEITEM; i++ )
	{
		if( m_StorageItem[ i ].uiItemCode == 0 )
		{
			if( psiIndex != NULL )			*psiIndex	=	i;
			return	&m_StorageItem[ i ];
		}
	}

	return	NULL;
}



//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	璽堅縑 氈朝 嬴檜蠱曖 薑爾蒂 DB縑 盪濰и棻.
//	熱薑 橾濠	:	2002-10-14 螃�� 4:05:23 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
VOID	SOVillage_IndustrialComplex_Factory::SetItemInfoToDB( SI32 siItemIndex, UI16 uiItemID, UI16 uiItemQuantity )
{
	DBIKSetStorageItem		stSetStorageItem;

	
	// DB縑 盪濰и棻.
	stSetStorageItem.uiVillageCode		=	m_uiVillageCode;
	stSetStorageItem.uiPlantID			=	0;
	stSetStorageItem.uiPartID			=	m_siIndustrialComplexID;
	stSetStorageItem.uiSlot				=	m_siFactoryID;
	stSetStorageItem.uiItemIndex		=	siItemIndex;
	stSetStorageItem.uiItemCode			=	uiItemID;
	stSetStorageItem.uiItemQnt			=	uiItemQuantity;
	
	m_pDataBase->writeDBQuery( SO_DB_INFOKIND_STORAGE_SETDATA, &stSetStorageItem, sizeof( stSetStorageItem ) );
}

VOID	SOVillage_IndustrialComplex_Factory::DelItemInfoToDB( SI32 siItemIndex, UI16 uiItemID, UI16 uiItemQuantity )
{
	DBIKFarmSetItem		stSetStorageItem;

	
	// DB縑 盪濰и棻.
	stSetStorageItem.uiVillageCode		=	m_uiVillageCode;
	
	stSetStorageItem.uiFarmID			=	m_siIndustrialComplexID;
	stSetStorageItem.uiFactoryID		=	m_siFactoryID;
	stSetStorageItem.uiItemIndex		=	siItemIndex;
	stSetStorageItem.uiItemCode			=	uiItemID;
	stSetStorageItem.uiItemQnt			=	uiItemQuantity;
	
	m_pDataBase->writeDBQuery( SO_DB_INFOKIND_STORAGE_DELETEITEM, &stSetStorageItem, sizeof( stSetStorageItem ) );
}


//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	⑷營曖 濛機檜 諫猿腎歷棻.
//	熱薑 橾濠	:	2002-10-24 螃�� 9:41:14 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
VOID	SOVillage_IndustrialComplex_Factory::WorkCompleted( SOPlayer *pPlayerCompletedWork )
{
	SI32	siPlayerNumInFactory;
	UI16	uiWorkingPlayerID[ ON_MAX_FACTORY_PLAYER_NUM ];

	// 辦摹 濛機擊 蟾晦�� и棻.
	InitWork();

	// ⑷營 濛機醞檣 賅萇 Ы溯檜橫蒂 餉薯и棻.
	m_WorkingPlayerMgr.RemoveAllPlayer();	



	// 濛機擊 諫猿и Ы溯檜橫朝 詭衛雖蒂 嘐葬 薯剪и棻 (NPC橾陽僥縑 NULL檜 螢熱紫氈擠).

	if (pPlayerCompletedWork) {
		m_PlayerMgr.RemovePlayer( pPlayerCompletedWork );
	}

	// ⑷營 濛機醞檣 賅萇 Ы溯檜橫縑啪 濛機檜 諫猿腎歷棻堅 詭衛雖蒂 爾頂遽棻.

	if( ( siPlayerNumInFactory = m_PlayerMgr.GetAddedPlayerList( uiWorkingPlayerID ) ) == 0 )
	{
		return;
	}

	m_PlayerMgr.GetFirstPlayer()->SendFieldMsg( ON_FARMWORK_COMPLETED, SO_SFM_CUSTOM, LPVOID( siPlayerNumInFactory ), LPVOID( uiWorkingPlayerID ) );

	// ⑷營 檜 勒僭縑 菟橫諦毚朝 賅萇 Ы溯檜橫蒂 餉薯и棻.
	m_PlayerMgr.RemoveAllPlayer();	
	
}


//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	璽堅 寰縑 醱碟и 嬴檜蠱檜 氈朝雖 匐餌и棻.
//	熱薑 橾濠	:	2002-10-12 螃�� 8:30:14 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
BOOL	SOVillage_IndustrialComplex_Factory::HasItem( UI16 uiItemID, UI16 uiItemQuantity )
{
	STORAGE_ITEM	*pItem;

	if (m_uiLevel < 1)                                      return  FALSE;

	if( ( pItem = GetItem( uiItemID, NULL ) ) == NULL )		return	FALSE;	
	if( pItem->uiQuantity < uiItemQuantity )				return	FALSE;			
	
	return TRUE;
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	璽堅 寰縑 嬴檜蠱擊 厥朝棻.
//	熱薑 橾濠	:	2002-10-14 螃瞪 10:55:47 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
BOOL	SOVillage_IndustrialComplex_Factory::AddItem( UI16 uiItemID, UI16 uiItemQuantity ,BOOL bSendToDB)
{
	SI32					siItemIndex = -1;	
	STORAGE_ITEM			*pItem;	

	if (IsConstructing())											return  FALSE;

	// 璽堅縑 晦襄縑 偽擎 嬴檜蠱檜 氈朝雖 匐餌и棻	
	if( ( pItem = GetItem( uiItemID, &siItemIndex ) ) != NULL )
	{
		// 譆渠 陛韓 熱 氈朝 嬴檜蠱曖 ъ辨 и紫蒂 蟾婁ж朝雖 匐餌и棻
		if( ( 32767 - pItem->uiQuantity ) < uiItemQuantity )	{
			// и紫陛 剩戲賊 斜傖褒ぬ ( и 璽堅縑 и謙盟曖 嬴檜蠱檜 舒 蘊擊 離雖й熱 橈棻.)
			return	FALSE;
		}

		// 嬴檜蠱曖 熱榆擊 棺萼棻.
		pItem->uiQuantity	+=	uiItemQuantity;	

		// DB縑 盪濰и棻.
		if (bSendToDB) {
			SetItemInfoToDB( siItemIndex, pItem->uiItemCode, pItem->uiQuantity );
		}

		return	TRUE;
	}
	// 橈棻賊 嬴檜蠱擊 厥擊 億煎遴 濰模蒂 瓊朝棻.
	else  if( ( pItem = GetEmptyItem( &siItemIndex ) ) != NULL )
	{
		// 譆渠蒂 剩戲賊 褒ぬ
		if (uiItemQuantity >= 32767) return FALSE;

		pItem->uiItemCode	=	uiItemID;
		pItem->uiQuantity	=	uiItemQuantity;	

		// DB縑 盪濰и棻.
		if (bSendToDB) {
			SetItemInfoToDB( siItemIndex, pItem->uiItemCode, pItem->uiQuantity );
		}

		return	TRUE;
	}	
	
	return	FALSE;	

}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	璽堅寰縑憮 嬴檜蠱擊 餉薯и棻.
//	熱薑 橾濠	:	2002-10-12 螃�� 8:55:03 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
BOOL	SOVillage_IndustrialComplex_Factory::DeleteItem( UI16 uiItemID, UI16 uiItemQuantity ,BOOL bSendToDB)
{
	SI32					siItemIndex = -1;
	STORAGE_ITEM			*pItem;

	if (IsConstructing())											return  FALSE;

	if( HasItem( uiItemID, uiItemQuantity ) == FALSE )				return	FALSE;

	if( ( pItem = GetItem( uiItemID, &siItemIndex ) ) == NULL )		return	FALSE;

	if (siItemIndex == -1)                                          return  FALSE;
	
	pItem->uiQuantity	-=	uiItemQuantity;

	// 嬴檜蠱擊 橈撻棻.
	if( pItem->uiQuantity <= 0 )
	{
		pItem->uiItemCode	=	0;
		pItem->uiQuantity	=	0;
	}

	if (bSendToDB) {
		DelItemInfoToDB( siItemIndex, uiItemID, uiItemQuantity );
	}

	return	TRUE;	
}


//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	璽堅 寰縑 嬴檜蠱擊 厥朝棻.
//	熱薑 橾濠	:	2002-10-14 螃瞪 11:31:34 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
BOOL	SOVillage_IndustrialComplex_Factory::AddItem( SOPlayer *pPlayer, UI16 uiItemID, UI16 uiItemQuantity ,BOOL bSendToDB)
{
	if( AddItem( uiItemID, uiItemQuantity ,bSendToDB) == FALSE )		return	FALSE;
	

	
	return	TRUE;
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	璽堅寰縑憮 嬴檜蠱擊 餉薯и棻.	
//	熱薑 橾濠	:	2002-10-14 螃瞪 11:35:46 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
BOOL	SOVillage_IndustrialComplex_Factory::DeleteItem( SOPlayer *pPlayer, UI16 uiItemID, UI16 uiItemQuantity ,BOOL bSendToDB)
{

	
	if( DeleteItem( uiItemID, uiItemQuantity ,bSendToDB) == FALSE )		return	FALSE;
	

	
	return	TRUE;	
}


//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	璽堅 寰縑 嬴檜蠱擊 厥擊 熱 氈朝雖 匐餌и棻.
//	熱薑 橾濠	:	2002-12-17 螃瞪 9:51:00 - 夢遽鷓
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收

BOOL	SOVillage_IndustrialComplex_Factory::CanAddItem( UI16 uiItemID ,UI16 uiQuantity,SI32 *pStorageIndex)
{
	// 璽堅檣雖 匐餌и棻.
	*pStorageIndex = -1;
	if( IsStorage() == FALSE )						return	FALSE;

	// 勒撲醞檜賊 寰脾
	if( m_uiLevel < 1)                               return  FALSE;


	STORAGE_ITEM *pItem;

	pItem = GetItem( uiItemID, pStorageIndex );
	if( pItem != NULL )			{

		int currentitem = pItem->uiQuantity;
		int newitem = uiQuantity;
		// 偽擎嬴檜蠱檜 氈戲賊 偎熱蒂 匐餌и棻.
		if ( (currentitem + newitem) < 32767) {
			return TRUE;
		}
		// 偎熱陛 蟾婁ж賊 綴奢除檜 氈渦塭紫 FALSE蒂 葬欐п撿и棻.
		else
		{
			return FALSE;
		}
		
	}

	if( GetEmptyItem( pStorageIndex ) != NULL )				{
		return	TRUE;
	}
	
	return	FALSE;
}


//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	奢濰縑憮 儅骯醞檣 嬴檜蠱曖 ID蒂 橢橫螞棻.
//	熱薑 橾濠	:	2002-10-15 螃瞪 10:06:54 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
UI16	SOVillage_IndustrialComplex_Factory::GetProducingItemID()
{
	return	m_uiProducingItemID;
}
