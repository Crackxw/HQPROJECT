#include "SOMain.h"
#include "SODatabase.h"
#include "SOVillage_IndustrialComplex_Factory.h"

CItem				*SOVillage_IndustrialComplex_Factory::m_pItem;
OnlineBuilding		*SOVillage_IndustrialComplex_Factory::m_pBuilding;
SODatabase			*SOVillage_IndustrialComplex_Factory::m_pDataBase;

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	생성자.
//	수정 일자	:	2002-10-07 오후 5:00:42 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
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

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	소멸자.
//	수정 일자	:	2002-10-07 오후 5:00:50 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
SOVillage_IndustrialComplex_Factory::~SOVillage_IndustrialComplex_Factory()
{
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	초기화.
//	수정 일자	:	2002-10-07 오후 6:02:14 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
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

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	공장을 초기화 한다.
//	수정 일자	:	2002-10-15 오후 2:43:57 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
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

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	해제.
//	수정 일자	:	2002-10-07 오후 6:01:52 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
VOID	SOVillage_IndustrialComplex_Factory::Free()
{
	m_PlayerMgr.Free();
	m_WorkingPlayerMgr.Free();
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	작업을 초기화 한다
//	수정 일자	:	2002-10-12 오후 3:06:51 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
VOID	SOVillage_IndustrialComplex_Factory::InitWork()	
{
	m_mnPay						=	0;

	m_uiRemainAmountOfWork		=	0;
	
	m_uiProducingItemID			=	0;
	
	m_uiMaxProduceItemQuantity	=	0;			
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	플레이어가 들어왔다.
//	수정 일자	:	2002-10-07 오후 5:56:56 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
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

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	플레이어가 나갔다.
//	수정 일자	:	2002-10-07 오후 5:59:17 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
BOOL	SOVillage_IndustrialComplex_Factory::LeavePlayer( SOPlayer *pPlayer )
{
	if( m_PlayerMgr.RemovePlayer( pPlayer ) == FALSE )		return	FALSE;	

	// 현재 작업중이면 작업중인 상태에서도 빼준다.
	// 플레이어가 서버와 연결이 끊겼을 시를 대비한다.
	if( m_WorkingPlayerMgr.IsAddedPlayer( pPlayer ) == TRUE )
	{	
		m_WorkingPlayerMgr.RemovePlayer( pPlayer );
	}
	
	pPlayer->SendFieldMsg( ON_RESPONSE_LEAVE_FACTORY, SO_SFM_ONLYME, LPVOID( ON_RET_OK ) );

	return	TRUE;
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	공장의 정보를 얻어온다.
//	수정 일자	:	2002-10-07 오후 5:26:55 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
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

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	공장이 건설되었는지 검사한다.
//	수정 일자	:	2002-10-07 오후 5:28:34 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
BOOL	SOVillage_IndustrialComplex_Factory::DidConstructFactory()
{	
	if( m_cFactoryKind[ 0 ] == NULL )			return	FALSE;

	return	TRUE;
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	DB에서 얻어온 정보로 창고를 초기화 한다.
//	수정 일자	:	2002-10-07 오후 9:33:16 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
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

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	DB에서 얻어온 정보로 공장을 초기화 한다.
//	수정 일자	:	2002-10-08 오전 10:41:35 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
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


//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	정적 변수를 초기화 한다.		
//	수정 일자	:	2002-10-07 오후 9:49:44 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
VOID	SOVillage_IndustrialComplex_Factory::InitStaticVariable( CItem *pItem, SODatabase *pDataBase, OnlineBuilding *pBuilding )
{
	m_pItem			=	pItem;
	m_pDataBase		=	pDataBase;
	m_pBuilding		=	pBuilding;
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	공장을 처분한다.
//	수정 일자	:	2002-10-08 오전 11:40:13 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
BOOL	SOVillage_IndustrialComplex_Factory::SellFactory( SOPlayer *pPlayer )
{
	return	FALSE;
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	플레이어가 이 공장안에 들어와 있는지 검사한다.
//	수정 일자	:	2002-10-08 오후 12:27:15 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
BOOL	SOVillage_IndustrialComplex_Factory::IsPlayerIn( SOPlayer *pPlayer )
{
	return	m_PlayerMgr.IsAddedPlayer( pPlayer );
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	일을 끝마쳤다.
//	수정 일자	:	2002-10-08 오후 12:35:02 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
BOOL	SOVillage_IndustrialComplex_Factory::FinishWork( SOPlayer *pPlayer, SOVillage_IndustrialComplex *pComplex, OnReqFarmWorkMsg *pMsg )
{	
	MONEY					TempPay =0;		
	
	
	
	// 이 공장에서 일이 가능한지 검사한다.
	if( CanWork() == FALSE )										return	FALSE;
	// 실제 일을 하고 있는 플레이어인지 검사한다.
	if( m_WorkingPlayerMgr.IsAddedPlayer( pPlayer ) == FALSE )		return	FALSE;
	// 일을 하고 있는 플레이어 리스트에서 삭제한다.
	m_WorkingPlayerMgr.RemovePlayer( pPlayer );	
	// 작업량이 0이면 실패처리한다.
	if( pMsg->siLabor == 0 )										return	FALSE;	
	// 작업량 해킹의 우려가 있다면 프로그램 리스트를 요청하고 실패 처리한다.
	if( !pPlayer->CheckLaborHack(pMsg->uiGameType, pMsg->siStep, pMsg->siLabor) )
	{
		writeInfoToFile( "LaborHackInfo.txt", "DBAccount(%d)  szID(%s)  uiGameType(%d)  siStep(%d)  siLabor(%d)",
						pPlayer->stAccountInfo.siDBAccount, pPlayer->szID, pMsg->uiGameType, pMsg->siStep, pMsg->siLabor );
		pPlayer->SendFieldMsg( ON_REQUEST_RUN_PROGRAM_LIST, SO_SFM_ONLYME, LPVOID(RUN_PROG_LIST_LABORHACK) );
		return FALSE;
	}


	// 테스트 서버일 경우에는 작업량의 10배만큼 처리해준다.
	if( g_GameServerInfoParser.IsTestServer() )
	{
	 	pMsg->siLabor	*=	10;
	} 
	
	if( m_uiProducingItemID == ON_PLANT_SLOTUPGRADE_CODE )
	{
		// 건설 중이다.

		if( pMsg->siLabor >= m_uiRemainAmountOfWork )
		{
			// 모든 작업을 끝냈다.			

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
				// 이 공장은 창고이다.

				m_pDataBase->writeDBQuery( SO_DB_INFOKIND_STORAGE_LEVELUP, &WorkInfo, sizeof( WorkInfo ) );
				//InitStorage();
				//pFarmSlot->Storage.Init();
			}
			else
			{	
				// 이 공장은 창고가 아니다.
				m_pDataBase->writeDBQuery( SO_DB_INFOKIND_FARM_LEVELUP, &WorkInfo, sizeof( WorkInfo ) );				
				//InitFactroy();					
				//pFarmSlot->Structure.Init();
			}

			LevelUp( pPlayer );

		}
		else
		{

			// 아직 모든 작업을 끝내지 못했다.

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
				// 이 공장은 창고이다.

				m_pDataBase->writeDBQuery( SO_DB_INFOKIND_STORAGE_WORK, &WorkInfo, sizeof( WorkInfo ) );
			}
			else
			{				
				// 이 공장은 창고가 아니다.

				m_pDataBase->writeDBQuery( SO_DB_INFOKIND_FARM_WORK, &WorkInfo, sizeof( WorkInfo ) );
			}

			
			

		}
		
	}
	// 아이템 생산중이다.
	else
	{
		

		
		if( pMsg->siLabor >= m_uiRemainAmountOfWork )
		{
			
			SI32 storageID,storageIndex;

			if (pComplex->CanAddItem(m_uiProducingItemID,m_uiMaxProduceItemQuantity,&storageID,&storageIndex))
			{
				
				DBIKFarmWorkEndInfo            WorkEndInfo;
				
				
				
				// 한방에 다나온다.
				
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

			


			// 작업을 초기화 한다.
			WorkCompleted( pPlayer );

		}
		else
		{
			// 서버 업데이트

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
						
			// DB 업데이트			
			m_pDataBase->writeDBQuery( SO_DB_INFOKIND_FARM_WORK, &WorkInfo, sizeof(WorkInfo ) );
			
		}
		
		
	
		
	}
	

	pPlayer->SendFieldMsg( ON_RESPONSE_FARM_WORK, SO_SFM_ONLYME, LPVOID( ON_RET_OK ), LPVOID( m_uiRemainAmountOfWork ), LPVOID( &TempPay ) );

	return TRUE;		
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	마을주민들이 일을 끝마쳤다.
//	수정 일자	:	2002-10-08 오후 12:35:02 - 박준태
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
BOOL	SOVillage_IndustrialComplex_Factory::FinishWorkByNPC( SOVillage_IndustrialComplex *pComplex,UI32 uiAmountOfWork)
{	
	MONEY					TempPay =0;		
	
	
	
	// 이 공장에서 일이 가능한지 검사한다.
	if( CanWork() == FALSE )										return	FALSE;
	// 일한 양을 검사한다.
	if( uiAmountOfWork == 0 )										return	FALSE;	

	
	if( m_uiProducingItemID == ON_PLANT_SLOTUPGRADE_CODE )
	{
		// 건설 중이다.

		if( uiAmountOfWork >= m_uiRemainAmountOfWork )
		{
			// 모든 작업을 끝냈다.			

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
				// 이 공장은 창고이다.

				m_pDataBase->writeDBQuery( SO_DB_INFOKIND_STORAGE_LEVELUP_BYNPC, &WorkInfo, sizeof( WorkInfo ) );
			}
			else
			{	
				// 이 공장은 창고가 아니다.
				m_pDataBase->writeDBQuery( SO_DB_INFOKIND_FARM_LEVELUP_BYNPC, &WorkInfo, sizeof( WorkInfo ) );				
			}

			// NPC가 일해서 pPlayer 는 NULL
			LevelUp( NULL );

		}
		else
		{

			// 아직 모든 작업을 끝내지 못했다.

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
				// 이 공장은 창고이다.

				m_pDataBase->writeDBQuery( SO_DB_INFOKIND_STORAGE_WORK_BYNPC, &WorkInfo, sizeof( WorkInfo ) );
			}
			else
			{				
				// 이 공장은 창고가 아니다.

				m_pDataBase->writeDBQuery( SO_DB_INFOKIND_FARM_WORK_BYNPC, &WorkInfo, sizeof( WorkInfo ) );
			}

			
			

		}
		
	}
	// 아이템 생산중이다.
	else
	{
		

		
		
		
		if( uiAmountOfWork >= m_uiRemainAmountOfWork )
		{
			
			SI32 storageID,storageIndex;

			if (pComplex->CanAddItem(m_uiProducingItemID,m_uiMaxProduceItemQuantity,&storageID,&storageIndex))
			{
				
				DBIKFarmWorkEndInfo            WorkEndInfo;
				
				
				
				// 한방에 다나온다.
				
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

			


			// 작업을 초기화 한다 (NPC가 일해서 pPlayer 는 NULL).
			WorkCompleted( NULL );

		}
		else
		{
			// 서버 업데이트

			DBIKFarmWorkInfo            WorkInfo;

			TempPay						=	uiAmountOfWork * m_mnPay;
			m_uiRemainAmountOfWork			-=		uiAmountOfWork;

			MONEY pay = TempPay;	

			WorkInfo.uiVillageCode	=	pComplex->GetVillageCode();
			WorkInfo.uiFarmID		=	m_siIndustrialComplexID;
			WorkInfo.uiFactoryID		=	m_siFactoryID;

			WorkInfo.Labor            = uiAmountOfWork;
			WorkInfo.moPay            = pay;
						
			// DB 업데이트			
			m_pDataBase->writeDBQuery( SO_DB_INFOKIND_FARM_WORK_BYNPC, &WorkInfo, sizeof(WorkInfo ) );
			
		}
		
		
	
		
	}


	

	

	return TRUE;		
}


//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	레벨을 증가시킨다.
//	수정 일자	:	2002-10-08 오후 12:52:49 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
VOID	SOVillage_IndustrialComplex_Factory::LevelUp( SOPlayer *pPlayerCompletedWork )
{	
	// 작업이 완료되었다.
	WorkCompleted( pPlayerCompletedWork );

	m_uiLevel++;	
}



//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	산업 단지에 있는 공장의 정보를 플레이어에게 보내준다.
//	수정 일자	:	2002-10-08 오후 2:29:01 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
BOOL	SOVillage_IndustrialComplex_Factory::SendFactoryInfo( SOPlayer *pPlayer, BOOL bIndustrialComplexOwner )
{
	if( CanWork() == TRUE )
	{	
		// 현재 이 공장에서 작업을 할 수 있다면 작업의 대한 정보를 보내준다.
		if( m_uiProducingItemID == ON_PLANT_SLOTUPGRADE_CODE )
		{
			// 건물이 건설중이다.
			pPlayer->SendFieldMsg( ON_RESPONSE_FARM_WORKINFO, SO_SFM_ONLYME, LPVOID( ON_RET_OK ), LPVOID( m_uiProducingItemID ), 
																					LPVOID( m_uiRemainAmountOfWork ), LPVOID( &m_mnPay ) );
		}
		else
		{
			// 아이템을 생산중이다.
			pPlayer->SendFieldMsg( ON_RESPONSE_FARM_WORKINFO, SO_SFM_ONLYME, LPVOID( ON_RET_OK ), LPVOID( m_uiProducingItemID ),	
																					LPVOID( m_uiMaxProduceItemQuantity ),
																					LPVOID( m_uiMaxProduceItemQuantity ),
																					LPVOID( &m_mnPay ) );
		}	
		
		return	TRUE;
	}
	else
	{
		// 현재 이 공장에서는 작업을 할 수 없다.

		// 공장이 창고이고 이 공장이 있는 산업단지의 주인이라면 창고의 정보를 보내준다.
		if( ( IsStorage() == TRUE ) && ( bIndustrialComplexOwner == TRUE ) )
		{
			// 창고의 정보를 보내준다.
			pPlayer->SendFieldMsg( ON_RESPONSE_STORAGE_INFO, SO_SFM_ONLYME, LPVOID( ON_RET_OK ), LPVOID( m_StorageItem ) );		 
			
			return	TRUE;
		}
	}
	
	return	FALSE;
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	업그레이드가 완료되었는지 검사한다.
//	수정 일자	:	2002-10-12 오후 5:55:30 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
BOOL	SOVillage_IndustrialComplex_Factory::IsUpgradeCompleted()
{
	if( m_uiLevel < MAX_UPGRADESLOT_NUM )			return	FALSE;

	return	TRUE;
}
/*
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	산업 단지에 있는 창고의 정보를 플레이어에게 보내준다.
//	수정 일자	:	2002-10-08 오후 3:18:02 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
BOOL	SOVillage_IndustrialComplex_Factory::SendStorageInfo( SOPlayer *pPlayer )
{	
	if( IsStorage() == FALSE )		return	FALSE;	

	pPlayer->SendFieldMsg( ON_RESPONSE_STORAGE_INFO, SO_SFM_ONLYME, LPVOID( ON_RET_OK ), LPVOID( m_StorageItem ) );		 

	return	TRUE;
}*/


//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	이 공장에서 일을 할 수 있는지 알아온다. 
//	수정 일자	:	2002-10-08 오후 2:49:21 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
BOOL	SOVillage_IndustrialComplex_Factory::CanWork()
{	
	//
	if( m_mnPay <= 0 )
	{
		return	FALSE;
	}


	//남은 일량
	if( m_uiRemainAmountOfWork <= 0 )
	{
		return	FALSE;
	}

	return	TRUE;
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	이 공장이 창고인지 알아온다.
//	수정 일자	:	2002-10-08 오후 3:19:22 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
BOOL	SOVillage_IndustrialComplex_Factory::IsStorage()
{
	return	m_bStore;	
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	이 공장이 일반적인 공장인지 알아온다.
//	수정 일자	:	2002-10-15 오전 10:01:46 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
BOOL	SOVillage_IndustrialComplex_Factory::IsFactory()
{
	return	!m_bStore;	
}


//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	작업량을 얻어온다.
//	수정 일자	:	2002-10-08 오후 4:21:19 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
UI32	SOVillage_IndustrialComplex_Factory::GetAmountOfWork()
{
	if( CanWork() == FALSE )					return	0;

	return	m_uiRemainAmountOfWork;
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	공장을 건설한다.
//	수정 일자	:	2002-10-10 오후 9:14:52 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
BOOL	SOVillage_IndustrialComplex_Factory::ConstructFactory( SOPlayer *pPlayer, SI32 siFactoryID, CHAR cFactoryCode[ 4 ], MONEY mnPay, MONEY *pmnCapital )
{
	DBIKFarmBuildStructInfo				BuildStructInfo;	
	stFacility							*pFactoryInfo;		
	MONEY								mnTotalMoney;
	
	// 최소 임금이 10냥 이상이어야 한다.
	if( mnPay < 10)																					return	FALSE;
		
	// 시설물이 유효한지 검사한다.
	if( ( pFactoryInfo = m_pBuilding->GetFacility( cFactoryCode ) ) == NULL)						return	FALSE;

	// 돈이 충분한지 검사한다.
	mnTotalMoney		=	pFactoryInfo->UpLabor[ 0 ] * (mnPay + ON_TAX_PER_WORK);
	
	if( pPlayer->EnoughMoney( mnTotalMoney ) == FALSE )												return	FALSE;

	// 비용을 지불한다.
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

	// DB와 서버에 업데이트 한다.
	if( strcmp( cFactoryCode, "STG" ) == 0 )
	{
		m_pDataBase->writeDBQuery( SO_DB_INFOKIND_STORAGE_BUILD, &BuildStructInfo, sizeof( BuildStructInfo ) );

		m_bStore						=	TRUE;		
		//pFarmSlot->Storage.Init();
	}
	else
	{
		//	DB에서 슬롯및 일 할 계획까지 세운다	
		m_pDataBase->writeDBQuery( SO_DB_INFOKIND_FARM_BUILDSTRUCT, &BuildStructInfo, sizeof( BuildStructInfo ) );

		m_bStore						=	FALSE;		
	

		m_uiMaxProduceItemQuantity		=	1;		
	}
	
		
	
	return	TRUE;
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	공장을 파괴한다.
//	수정 일자	:	2002-10-12 오후 3:56:16 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
VOID	SOVillage_IndustrialComplex_Factory::DestroyFactory(  SOPlayer *pPlayer, MONEY *pmnCapital )
{
	
	if( IsStorage() == TRUE )
	{
		DBIKDestroyStorage		DS;

		// DB에 저장한다.
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

	// 빼야 하는 수익금을 얻어온다.
//	*pmnCapital				=	( m_uiRemainAmountOfWork * m_mnPay) * ON_CAPITAL_RATE_PLANT; 

	// 플레이어에게 메시지를 보내준다.
	if( pPlayer != NULL )	pPlayer->SendFieldMsg( ON_RESPONSE_FARM_DESTROYSTRUCT, SO_SFM_ONLYME, LPVOID( ON_RET_OK ) );	

	// 공장을 초기화 한다.
	InitFactory();

}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	일을 시작한다.
//	수정 일자	:	2002-10-11 오후 7:27:06 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
BOOL	SOVillage_IndustrialComplex_Factory::BeginWork( SOPlayer *pPlayer )
{
	// 일을 할 수 있는지 검사한다.
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

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	공장의 임금을 변경한다.
//	수정 일자	:	2002-10-11 오후 9:00:24 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
BOOL	SOVillage_IndustrialComplex_Factory::ChangeFactoryPay( SOPlayer *pPlayer, MONEY mnPay, MONEY *pmnCapital )
{	
	MONEY						mnDifference;
	DBIKChangeFarmPay			stChangePay;

	
	// 작업을 할 수 있는 공장인지 검사한다.
	if( CanWork() == FALSE )																return	FALSE;

	if( mnPay < 10 )																		return	FALSE;
	
	mnDifference			=	(MONEY)( mnPay - m_mnPay ) * m_uiRemainAmountOfWork;

	if (mnDifference > 0)
	{
		if( pPlayer->EnoughMoney( mnDifference ) == FALSE )										return	FALSE;	// 플레이어에게 변경하기 위한 충분한 돈이 없다.
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

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	현재 작업중인 공장에서 작업중인것을 취소한다.
//	수정 일자	:	2002-10-12 오후 2:50:09 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
BOOL	SOVillage_IndustrialComplex_Factory::CancelWork( SOPlayer *pPlayer, SI32 siFactoryID, MONEY *pmnCapital )
{
	MONEY					mnDifference;
	DBIKFarmCancelPlan	FarmCancelData;

	// 지금은 일이 가능한 상태인지 검사한다.	
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
		

	// 공장이 건설중이라면 공장을 없앤다.
	if( IsConstructing() == TRUE )	
	{
		WorkCompleted( pPlayer );	


		// 플레이어에게 메시지를 보내준다.
		//if( pPlayer != NULL )	pPlayer->SendFieldMsg( ON_RESPONSE_FARM_DESTROYSTRUCT, SO_SFM_ONLYME, LPVOID( ON_RET_OK ) );			

		// 공장을 초기화 한다.
		InitFactory();		
	}	
	else
	{

		WorkCompleted( pPlayer );	

	}
	
	pPlayer->SendFieldMsg( ON_RESPONSE_PLANT_CANCEL_PLAN, SO_SFM_ONLYME, LPVOID( ON_RET_OK ), LPVOID( &pPlayer->siMoney ) );	

	return	TRUE;
}


//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	건물이 건설중인지 알아온다.
//	수정 일자	:	2002-10-12 오후 3:58:30 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
BOOL	SOVillage_IndustrialComplex_Factory::IsConstructing()
{
	if( ( m_uiLevel == 0 ) && ( IsUpgrading() == TRUE ) )		return	TRUE;
	else														return	FALSE;
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	공장을 업그레이드중인지 알아온다.
//	수정 일자	:	2002-10-15 오전 10:04:27 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
BOOL	SOVillage_IndustrialComplex_Factory::IsUpgrading()
{
	if( m_uiProducingItemID != ON_PLANT_SLOTUPGRADE_CODE )		return	FALSE;

	return	TRUE;
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	공장을 업그레이드 시킨다.
//	수정 일자	:	2002-10-12 오후 5:51:30 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
BOOL	SOVillage_IndustrialComplex_Factory::UpgradeFactory( SOPlayer *pPlayer, MONEY mnPay, MONEY *pmnCapital )
{
	DBIKSetUpgradeSlot				SetUpgradeSlot;
	stFacility						*pFacility;
	MONEY							mnTotalPay;
	UI32							uiAmountOfWork;


	// 현재 일을 할 수 있는 상태라면 업그레이를 할 수 없다.
	if( CanWork() == TRUE )																			return	FALSE;

	// 최소 임금이 10냥 이상이어야 한다.
	if( mnPay < 10)																					return	FALSE;

	if( IsUpgradeCompleted() == TRUE )																return	FALSE;

	if( ( pFacility	 =	m_pBuilding->GetFacility( m_cFactoryKind ) ) == NULL )						return	FALSE;	

	memset(&SetUpgradeSlot, 0, sizeof(DBIKSetUpgradeSlot));	
	
	uiAmountOfWork			=	pFacility->UpLabor[ m_uiLevel ];	
	
	mnTotalPay				=	mnPay * uiAmountOfWork + uiAmountOfWork * ON_TAX_PER_WORK;
	
	if( pPlayer->EnoughMoney( mnTotalPay ) == FALSE )												return	FALSE;

	pPlayer->DecreaseMoney( mnTotalPay ,FALSE);	

	// 서버 업데이트.	
	m_mnPay						=	mnPay;
	m_uiProducingItemID			=	ON_PLANT_SLOTUPGRADE_CODE;
	m_uiMaxProduceItemQuantity	=	1;
	
	m_uiRemainAmountOfWork		=	uiAmountOfWork;

	// 마을의 수익금.
	*pmnCapital					=	uiAmountOfWork * ON_TAX_PER_WORK;

	

	
	
	// DB에 입력.
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

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	아이템을 생산한다.
//	수정 일자	:	2002-10-12 오후 7:17:40 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
DWORD	SOVillage_IndustrialComplex_Factory::ProduceItem( SOPlayer *pPlayer, UI16 uiItemID, UI32 uiItemQuantity, MONEY mnPay, CHAR cNation, MONEY *pmnCapital )
{
	stFacility						*pFacility;
	DBIKSetFarmProduct				SetFarmProduct;
	MONEY							mnTotalPay,moTax;
	UI32							uiAmountOfWork;

	// 최소 임금이 10냥 이상이어야 한다.
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

	// 계획을 만든다. - DB에 입력한다.
	SetFarmProduct.siDBAccount              =   pPlayer->stAccountInfo.siDBAccount;
	SetFarmProduct.uiSlot                   =   pPlayer->stAccountInfo.uiSlot;
	SetFarmProduct.uiVillageCode		    =	pPlayer->stAccountInfo.pVillage->uiVillageCode;
	SetFarmProduct.uiFarmID					=	m_siIndustrialComplexID;
	SetFarmProduct.uiFactoryID				=	m_siFactoryID;
	SetFarmProduct.uiItemID					=	uiItemID;
	SetFarmProduct.Quantity					=	uiItemQuantity;			// 만들 량
	SetFarmProduct.moPay					=	mnPay;			// 임금
	SetFarmProduct.Labor                    =   uiAmountOfWork;
	SetFarmProduct.moTax                    =   moTax;
	SetFarmProduct.moProfit                 =   moTax;
	

	m_pDataBase->writeDBQuery( SO_DB_INFOKIND_FARM_PRODUCTITEM, &SetFarmProduct, sizeof( SetFarmProduct ) );

	m_uiProducingItemID						=	uiItemID;	

	m_uiMaxProduceItemQuantity				=	uiItemQuantity;	
	m_mnPay									=	mnPay;
	
	m_uiRemainAmountOfWork					=	uiAmountOfWork;	

	// 마을의 수익금.
	*pmnCapital								=	moTax;

	pPlayer->DecreaseMoney( mnTotalPay ,FALSE);

	pPlayer->SendFieldMsg( ON_RESPONSE_FARM_ITEMPRODUCT, SO_SFM_ONLYME, LPVOID( ON_RET_OK ) );

	// 이 공장안에 플레이어를 넣어준다.
	m_PlayerMgr.AddPlayer( pPlayer );

	return ON_RET_OK;		
}


//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명	:	아이템 개수를 노동량으로 바꾼다.
//	수정 일자	:	2002-10-12 오후 7:56:15 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
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

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	노동량을 아이템의 개수로 바꾼다.
//	수정 일자	:	2002-10-14 오후 7:23:53 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
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

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	창고에 있는 아이템을 얻어온다.
//	수정 일자	:	2002-10-12 오후 8:56:28 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
STORAGE_ITEM*		SOVillage_IndustrialComplex_Factory::GetItem( UI16 uiItemID, SI32 *psiIndex )
{
	SI32	i;

	if( IsStorage() == FALSE )			return	NULL;

	for( i = 0; i < ON_MAX_STORAGEITEM; i++ )
	{
		// 0보다는 무조건 커야한다.
		if( m_StorageItem[ i ].uiItemCode == uiItemID && m_StorageItem[i].uiQuantity > 0)
		{
			if( psiIndex != NULL )			*psiIndex	=	i;

			return	&m_StorageItem[ i ];
		}
	}

	return	NULL;
}



//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	창고에 비어있는 아이템을 얻어온다.
//	수정 일자	:	2002-10-14 오전 11:08:03 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
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



//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	창고에 있는 아이템의 정보를 DB에 저장한다.
//	수정 일자	:	2002-10-14 오후 4:05:23 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
VOID	SOVillage_IndustrialComplex_Factory::SetItemInfoToDB( SI32 siItemIndex, UI16 uiItemID, UI16 uiItemQuantity )
{
	DBIKSetStorageItem		stSetStorageItem;

	
	// DB에 저장한다.
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

	
	// DB에 저장한다.
	stSetStorageItem.uiVillageCode		=	m_uiVillageCode;
	
	stSetStorageItem.uiFarmID			=	m_siIndustrialComplexID;
	stSetStorageItem.uiFactoryID		=	m_siFactoryID;
	stSetStorageItem.uiItemIndex		=	siItemIndex;
	stSetStorageItem.uiItemCode			=	uiItemID;
	stSetStorageItem.uiItemQnt			=	uiItemQuantity;
	
	m_pDataBase->writeDBQuery( SO_DB_INFOKIND_STORAGE_DELETEITEM, &stSetStorageItem, sizeof( stSetStorageItem ) );
}


//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	현재의 작업이 완료되었다.
//	수정 일자	:	2002-10-24 오후 9:41:14 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
VOID	SOVillage_IndustrialComplex_Factory::WorkCompleted( SOPlayer *pPlayerCompletedWork )
{
	SI32	siPlayerNumInFactory;
	UI16	uiWorkingPlayerID[ ON_MAX_FACTORY_PLAYER_NUM ];

	// 우선 작업을 초기화 한다.
	InitWork();

	// 현재 작업중인 모든 플레이어를 삭제한다.
	m_WorkingPlayerMgr.RemoveAllPlayer();	



	// 작업을 완료한 플레이어는 메시지를 미리 제거한다 (NPC일때문에 NULL이 올수도있음).

	if (pPlayerCompletedWork) {
		m_PlayerMgr.RemovePlayer( pPlayerCompletedWork );
	}

	// 현재 작업중인 모든 플레이어에게 작업이 완료되었다고 메시지를 보내준다.

	if( ( siPlayerNumInFactory = m_PlayerMgr.GetAddedPlayerList( uiWorkingPlayerID ) ) == 0 )
	{
		return;
	}

	m_PlayerMgr.GetFirstPlayer()->SendFieldMsg( ON_FARMWORK_COMPLETED, SO_SFM_CUSTOM, LPVOID( siPlayerNumInFactory ), LPVOID( uiWorkingPlayerID ) );

	// 현재 이 건물에 들어와잇는 모든 플레이어를 삭제한다.
	m_PlayerMgr.RemoveAllPlayer();	
	
}


//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	창고 안에 충분한 아이템이 있는지 검사한다.
//	수정 일자	:	2002-10-12 오후 8:30:14 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
BOOL	SOVillage_IndustrialComplex_Factory::HasItem( UI16 uiItemID, UI16 uiItemQuantity )
{
	STORAGE_ITEM	*pItem;

	if (m_uiLevel < 1)                                      return  FALSE;

	if( ( pItem = GetItem( uiItemID, NULL ) ) == NULL )		return	FALSE;	
	if( pItem->uiQuantity < uiItemQuantity )				return	FALSE;			
	
	return TRUE;
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	창고 안에 아이템을 넣는다.
//	수정 일자	:	2002-10-14 오전 10:55:47 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
BOOL	SOVillage_IndustrialComplex_Factory::AddItem( UI16 uiItemID, UI16 uiItemQuantity ,BOOL bSendToDB)
{
	SI32					siItemIndex = -1;	
	STORAGE_ITEM			*pItem;	

	if (IsConstructing())											return  FALSE;

	// 창고에 기존에 같은 아이템이 있는지 검사한다	
	if( ( pItem = GetItem( uiItemID, &siItemIndex ) ) != NULL )
	{
		// 최대 가질 수 있는 아이템의 허용 한도를 초과하는지 검사한다
		if( ( 32767 - pItem->uiQuantity ) < uiItemQuantity )	{
			// 한도가 넘으면 그냥실패 ( 한 창고에 한종류의 아이템이 두 칸을 차지할수 없다.)
			return	FALSE;
		}

		// 아이템의 수량을 늘린다.
		pItem->uiQuantity	+=	uiItemQuantity;	

		// DB에 저장한다.
		if (bSendToDB) {
			SetItemInfoToDB( siItemIndex, pItem->uiItemCode, pItem->uiQuantity );
		}

		return	TRUE;
	}
	// 없다면 아이템을 넣을 새로운 장소를 찾는다.
	else  if( ( pItem = GetEmptyItem( &siItemIndex ) ) != NULL )
	{
		// 최대를 넘으면 실패
		if (uiItemQuantity >= 32767) return FALSE;

		pItem->uiItemCode	=	uiItemID;
		pItem->uiQuantity	=	uiItemQuantity;	

		// DB에 저장한다.
		if (bSendToDB) {
			SetItemInfoToDB( siItemIndex, pItem->uiItemCode, pItem->uiQuantity );
		}

		return	TRUE;
	}	
	
	return	FALSE;	

}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	창고안에서 아이템을 삭제한다.
//	수정 일자	:	2002-10-12 오후 8:55:03 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
BOOL	SOVillage_IndustrialComplex_Factory::DeleteItem( UI16 uiItemID, UI16 uiItemQuantity ,BOOL bSendToDB)
{
	SI32					siItemIndex = -1;
	STORAGE_ITEM			*pItem;

	if (IsConstructing())											return  FALSE;

	if( HasItem( uiItemID, uiItemQuantity ) == FALSE )				return	FALSE;

	if( ( pItem = GetItem( uiItemID, &siItemIndex ) ) == NULL )		return	FALSE;

	if (siItemIndex == -1)                                          return  FALSE;
	
	pItem->uiQuantity	-=	uiItemQuantity;

	// 아이템을 없앤다.
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


//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	창고 안에 아이템을 넣는다.
//	수정 일자	:	2002-10-14 오전 11:31:34 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
BOOL	SOVillage_IndustrialComplex_Factory::AddItem( SOPlayer *pPlayer, UI16 uiItemID, UI16 uiItemQuantity ,BOOL bSendToDB)
{
	if( AddItem( uiItemID, uiItemQuantity ,bSendToDB) == FALSE )		return	FALSE;
	

	
	return	TRUE;
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	창고안에서 아이템을 삭제한다.	
//	수정 일자	:	2002-10-14 오전 11:35:46 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
BOOL	SOVillage_IndustrialComplex_Factory::DeleteItem( SOPlayer *pPlayer, UI16 uiItemID, UI16 uiItemQuantity ,BOOL bSendToDB)
{

	
	if( DeleteItem( uiItemID, uiItemQuantity ,bSendToDB) == FALSE )		return	FALSE;
	

	
	return	TRUE;	
}


//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	창고 안에 아이템을 넣을 수 있는지 검사한다.
//	수정 일자	:	2002-12-17 오전 9:51:00 - 박준태
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

BOOL	SOVillage_IndustrialComplex_Factory::CanAddItem( UI16 uiItemID ,UI16 uiQuantity,SI32 *pStorageIndex)
{
	// 창고인지 검사한다.
	*pStorageIndex = -1;
	if( IsStorage() == FALSE )						return	FALSE;

	// 건설중이면 안됨
	if( m_uiLevel < 1)                               return  FALSE;


	STORAGE_ITEM *pItem;

	pItem = GetItem( uiItemID, pStorageIndex );
	if( pItem != NULL )			{

		int currentitem = pItem->uiQuantity;
		int newitem = uiQuantity;
		// 같은아이템이 있으면 갯수를 검사한다.
		if ( (currentitem + newitem) < 32767) {
			return TRUE;
		}
		// 갯수가 초과하면 빈공간이 있더라도 FALSE를 리턴해야한다.
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


//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	공장에서 생산중인 아이템의 ID를 얻어온다.
//	수정 일자	:	2002-10-15 오전 10:06:54 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
UI16	SOVillage_IndustrialComplex_Factory::GetProducingItemID()
{
	return	m_uiProducingItemID;
}
