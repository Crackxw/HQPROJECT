#include "SOMain.h"
#include "SODatabase.h"
#include "SOVillage_IndustrialComplex_Factory.h"

CItem				*SOVillage_IndustrialComplex_Factory::m_pItem;
OnlineBuilding		*SOVillage_IndustrialComplex_Factory::m_pBuilding;
SODatabase			*SOVillage_IndustrialComplex_Factory::m_pDataBase;

//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	������.
//	���� ����	:	2002-10-07 ���� 5:00:42 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
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

//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	�Ҹ���.
//	���� ����	:	2002-10-07 ���� 5:00:50 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
SOVillage_IndustrialComplex_Factory::~SOVillage_IndustrialComplex_Factory()
{
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	�ʱ�ȭ.
//	���� ����	:	2002-10-07 ���� 6:02:14 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
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

//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	������ �ʱ�ȭ �Ѵ�.
//	���� ����	:	2002-10-15 ���� 2:43:57 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
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

//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	����.
//	���� ����	:	2002-10-07 ���� 6:01:52 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
VOID	SOVillage_IndustrialComplex_Factory::Free()
{
	m_PlayerMgr.Free();
	m_WorkingPlayerMgr.Free();
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	�۾��� �ʱ�ȭ �Ѵ�
//	���� ����	:	2002-10-12 ���� 3:06:51 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
VOID	SOVillage_IndustrialComplex_Factory::InitWork()	
{
	m_mnPay						=	0;

	m_uiRemainAmountOfWork		=	0;
	
	m_uiProducingItemID			=	0;
	
	m_uiMaxProduceItemQuantity	=	0;			
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	�÷��̾ ���Դ�.
//	���� ����	:	2002-10-07 ���� 5:56:56 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
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

//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	�÷��̾ ������.
//	���� ����	:	2002-10-07 ���� 5:59:17 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
BOOL	SOVillage_IndustrialComplex_Factory::LeavePlayer( SOPlayer *pPlayer )
{
	if( m_PlayerMgr.RemovePlayer( pPlayer ) == FALSE )		return	FALSE;	

	// ���� �۾����̸� �۾����� ���¿����� ���ش�.
	// �÷��̾ ������ ������ ������ �ø� ����Ѵ�.
	if( m_WorkingPlayerMgr.IsAddedPlayer( pPlayer ) == TRUE )
	{	
		m_WorkingPlayerMgr.RemovePlayer( pPlayer );
	}
	
	pPlayer->SendFieldMsg( ON_RESPONSE_LEAVE_FACTORY, SO_SFM_ONLYME, LPVOID( ON_RET_OK ) );

	return	TRUE;
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	������ ������ ���´�.
//	���� ����	:	2002-10-07 ���� 5:26:55 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
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

//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	������ �Ǽ��Ǿ����� �˻��Ѵ�.
//	���� ����	:	2002-10-07 ���� 5:28:34 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
BOOL	SOVillage_IndustrialComplex_Factory::DidConstructFactory()
{	
	if( m_cFactoryKind[ 0 ] == NULL )			return	FALSE;

	return	TRUE;
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	DB���� ���� ������ â�� �ʱ�ȭ �Ѵ�.
//	���� ����	:	2002-10-07 ���� 9:33:16 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
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

//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	DB���� ���� ������ ������ �ʱ�ȭ �Ѵ�.
//	���� ����	:	2002-10-08 ���� 10:41:35 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
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


//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	���� ������ �ʱ�ȭ �Ѵ�.		
//	���� ����	:	2002-10-07 ���� 9:49:44 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
VOID	SOVillage_IndustrialComplex_Factory::InitStaticVariable( CItem *pItem, SODatabase *pDataBase, OnlineBuilding *pBuilding )
{
	m_pItem			=	pItem;
	m_pDataBase		=	pDataBase;
	m_pBuilding		=	pBuilding;
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	������ ó���Ѵ�.
//	���� ����	:	2002-10-08 ���� 11:40:13 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
BOOL	SOVillage_IndustrialComplex_Factory::SellFactory( SOPlayer *pPlayer )
{
	return	FALSE;
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	�÷��̾ �� ����ȿ� ���� �ִ��� �˻��Ѵ�.
//	���� ����	:	2002-10-08 ���� 12:27:15 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
BOOL	SOVillage_IndustrialComplex_Factory::IsPlayerIn( SOPlayer *pPlayer )
{
	return	m_PlayerMgr.IsAddedPlayer( pPlayer );
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	���� �����ƴ�.
//	���� ����	:	2002-10-08 ���� 12:35:02 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
BOOL	SOVillage_IndustrialComplex_Factory::FinishWork( SOPlayer *pPlayer, SOVillage_IndustrialComplex *pComplex, OnReqFarmWorkMsg *pMsg )
{	
	MONEY					TempPay =0;		
	
	
	
	// �� ���忡�� ���� �������� �˻��Ѵ�.
	if( CanWork() == FALSE )										return	FALSE;
	// ���� ���� �ϰ� �ִ� �÷��̾����� �˻��Ѵ�.
	if( m_WorkingPlayerMgr.IsAddedPlayer( pPlayer ) == FALSE )		return	FALSE;
	// ���� �ϰ� �ִ� �÷��̾� ����Ʈ���� �����Ѵ�.
	m_WorkingPlayerMgr.RemovePlayer( pPlayer );	
	// �۾����� 0�̸� ����ó���Ѵ�.
	if( pMsg->siLabor == 0 )										return	FALSE;	
	// �۾��� ��ŷ�� ����� �ִٸ� ���α׷� ����Ʈ�� ��û�ϰ� ���� ó���Ѵ�.
	if( !pPlayer->CheckLaborHack(pMsg->uiGameType, pMsg->siStep, pMsg->siLabor) )
	{
		writeInfoToFile( "LaborHackInfo.txt", "DBAccount(%d)  szID(%s)  uiGameType(%d)  siStep(%d)  siLabor(%d)",
						pPlayer->stAccountInfo.siDBAccount, pPlayer->szID, pMsg->uiGameType, pMsg->siStep, pMsg->siLabor );
		pPlayer->SendFieldMsg( ON_REQUEST_RUN_PROGRAM_LIST, SO_SFM_ONLYME, LPVOID(RUN_PROG_LIST_LABORHACK) );
		return FALSE;
	}


	// �׽�Ʈ ������ ��쿡�� �۾����� 10�踸ŭ ó�����ش�.
	if( g_GameServerInfoParser.IsTestServer() )
	{
	 	pMsg->siLabor	*=	10;
	} 
	
	if( m_uiProducingItemID == ON_PLANT_SLOTUPGRADE_CODE )
	{
		// �Ǽ� ���̴�.

		if( pMsg->siLabor >= m_uiRemainAmountOfWork )
		{
			// ��� �۾��� ���´�.			

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
				// �� ������ â���̴�.

				m_pDataBase->writeDBQuery( SO_DB_INFOKIND_STORAGE_LEVELUP, &WorkInfo, sizeof( WorkInfo ) );
				//InitStorage();
				//pFarmSlot->Storage.Init();
			}
			else
			{	
				// �� ������ â�� �ƴϴ�.
				m_pDataBase->writeDBQuery( SO_DB_INFOKIND_FARM_LEVELUP, &WorkInfo, sizeof( WorkInfo ) );				
				//InitFactroy();					
				//pFarmSlot->Structure.Init();
			}

			LevelUp( pPlayer );

		}
		else
		{

			// ���� ��� �۾��� ������ ���ߴ�.

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
				// �� ������ â���̴�.

				m_pDataBase->writeDBQuery( SO_DB_INFOKIND_STORAGE_WORK, &WorkInfo, sizeof( WorkInfo ) );
			}
			else
			{				
				// �� ������ â�� �ƴϴ�.

				m_pDataBase->writeDBQuery( SO_DB_INFOKIND_FARM_WORK, &WorkInfo, sizeof( WorkInfo ) );
			}

			
			

		}
		
	}
	// ������ �������̴�.
	else
	{
		

		
		if( pMsg->siLabor >= m_uiRemainAmountOfWork )
		{
			
			SI32 storageID,storageIndex;

			if (pComplex->CanAddItem(m_uiProducingItemID,m_uiMaxProduceItemQuantity,&storageID,&storageIndex))
			{
				
				DBIKFarmWorkEndInfo            WorkEndInfo;
				
				
				
				// �ѹ濡 �ٳ��´�.
				
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

			


			// �۾��� �ʱ�ȭ �Ѵ�.
			WorkCompleted( pPlayer );

		}
		else
		{
			// ���� ������Ʈ

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
						
			// DB ������Ʈ			
			m_pDataBase->writeDBQuery( SO_DB_INFOKIND_FARM_WORK, &WorkInfo, sizeof(WorkInfo ) );
			
		}
		
		
	
		
	}
	

	pPlayer->SendFieldMsg( ON_RESPONSE_FARM_WORK, SO_SFM_ONLYME, LPVOID( ON_RET_OK ), LPVOID( m_uiRemainAmountOfWork ), LPVOID( &TempPay ) );

	return TRUE;		
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	�����ֹε��� ���� �����ƴ�.
//	���� ����	:	2002-10-08 ���� 12:35:02 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
BOOL	SOVillage_IndustrialComplex_Factory::FinishWorkByNPC( SOVillage_IndustrialComplex *pComplex,UI32 uiAmountOfWork)
{	
	MONEY					TempPay =0;		
	
	
	
	// �� ���忡�� ���� �������� �˻��Ѵ�.
	if( CanWork() == FALSE )										return	FALSE;
	// ���� ���� �˻��Ѵ�.
	if( uiAmountOfWork == 0 )										return	FALSE;	

	
	if( m_uiProducingItemID == ON_PLANT_SLOTUPGRADE_CODE )
	{
		// �Ǽ� ���̴�.

		if( uiAmountOfWork >= m_uiRemainAmountOfWork )
		{
			// ��� �۾��� ���´�.			

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
				// �� ������ â���̴�.

				m_pDataBase->writeDBQuery( SO_DB_INFOKIND_STORAGE_LEVELUP_BYNPC, &WorkInfo, sizeof( WorkInfo ) );
			}
			else
			{	
				// �� ������ â�� �ƴϴ�.
				m_pDataBase->writeDBQuery( SO_DB_INFOKIND_FARM_LEVELUP_BYNPC, &WorkInfo, sizeof( WorkInfo ) );				
			}

			// NPC�� ���ؼ� pPlayer �� NULL
			LevelUp( NULL );

		}
		else
		{

			// ���� ��� �۾��� ������ ���ߴ�.

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
				// �� ������ â���̴�.

				m_pDataBase->writeDBQuery( SO_DB_INFOKIND_STORAGE_WORK_BYNPC, &WorkInfo, sizeof( WorkInfo ) );
			}
			else
			{				
				// �� ������ â�� �ƴϴ�.

				m_pDataBase->writeDBQuery( SO_DB_INFOKIND_FARM_WORK_BYNPC, &WorkInfo, sizeof( WorkInfo ) );
			}

			
			

		}
		
	}
	// ������ �������̴�.
	else
	{
		

		
		
		
		if( uiAmountOfWork >= m_uiRemainAmountOfWork )
		{
			
			SI32 storageID,storageIndex;

			if (pComplex->CanAddItem(m_uiProducingItemID,m_uiMaxProduceItemQuantity,&storageID,&storageIndex))
			{
				
				DBIKFarmWorkEndInfo            WorkEndInfo;
				
				
				
				// �ѹ濡 �ٳ��´�.
				
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

			


			// �۾��� �ʱ�ȭ �Ѵ� (NPC�� ���ؼ� pPlayer �� NULL).
			WorkCompleted( NULL );

		}
		else
		{
			// ���� ������Ʈ

			DBIKFarmWorkInfo            WorkInfo;

			TempPay						=	uiAmountOfWork * m_mnPay;
			m_uiRemainAmountOfWork			-=		uiAmountOfWork;

			MONEY pay = TempPay;	

			WorkInfo.uiVillageCode	=	pComplex->GetVillageCode();
			WorkInfo.uiFarmID		=	m_siIndustrialComplexID;
			WorkInfo.uiFactoryID		=	m_siFactoryID;

			WorkInfo.Labor            = uiAmountOfWork;
			WorkInfo.moPay            = pay;
						
			// DB ������Ʈ			
			m_pDataBase->writeDBQuery( SO_DB_INFOKIND_FARM_WORK_BYNPC, &WorkInfo, sizeof(WorkInfo ) );
			
		}
		
		
	
		
	}


	

	

	return TRUE;		
}


//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	������ ������Ų��.
//	���� ����	:	2002-10-08 ���� 12:52:49 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
VOID	SOVillage_IndustrialComplex_Factory::LevelUp( SOPlayer *pPlayerCompletedWork )
{	
	// �۾��� �Ϸ�Ǿ���.
	WorkCompleted( pPlayerCompletedWork );

	m_uiLevel++;	
}



//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	��� ������ �ִ� ������ ������ �÷��̾�� �����ش�.
//	���� ����	:	2002-10-08 ���� 2:29:01 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
BOOL	SOVillage_IndustrialComplex_Factory::SendFactoryInfo( SOPlayer *pPlayer, BOOL bIndustrialComplexOwner )
{
	if( CanWork() == TRUE )
	{	
		// ���� �� ���忡�� �۾��� �� �� �ִٸ� �۾��� ���� ������ �����ش�.
		if( m_uiProducingItemID == ON_PLANT_SLOTUPGRADE_CODE )
		{
			// �ǹ��� �Ǽ����̴�.
			pPlayer->SendFieldMsg( ON_RESPONSE_FARM_WORKINFO, SO_SFM_ONLYME, LPVOID( ON_RET_OK ), LPVOID( m_uiProducingItemID ), 
																					LPVOID( m_uiRemainAmountOfWork ), LPVOID( &m_mnPay ) );
		}
		else
		{
			// �������� �������̴�.
			pPlayer->SendFieldMsg( ON_RESPONSE_FARM_WORKINFO, SO_SFM_ONLYME, LPVOID( ON_RET_OK ), LPVOID( m_uiProducingItemID ),	
																					LPVOID( m_uiMaxProduceItemQuantity ),
																					LPVOID( m_uiMaxProduceItemQuantity ),
																					LPVOID( &m_mnPay ) );
		}	
		
		return	TRUE;
	}
	else
	{
		// ���� �� ���忡���� �۾��� �� �� ����.

		// ������ â���̰� �� ������ �ִ� ��������� �����̶�� â���� ������ �����ش�.
		if( ( IsStorage() == TRUE ) && ( bIndustrialComplexOwner == TRUE ) )
		{
			// â���� ������ �����ش�.
			pPlayer->SendFieldMsg( ON_RESPONSE_STORAGE_INFO, SO_SFM_ONLYME, LPVOID( ON_RET_OK ), LPVOID( m_StorageItem ) );		 
			
			return	TRUE;
		}
	}
	
	return	FALSE;
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	���׷��̵尡 �Ϸ�Ǿ����� �˻��Ѵ�.
//	���� ����	:	2002-10-12 ���� 5:55:30 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
BOOL	SOVillage_IndustrialComplex_Factory::IsUpgradeCompleted()
{
	if( m_uiLevel < MAX_UPGRADESLOT_NUM )			return	FALSE;

	return	TRUE;
}
/*
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	��� ������ �ִ� â���� ������ �÷��̾�� �����ش�.
//	���� ����	:	2002-10-08 ���� 3:18:02 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
BOOL	SOVillage_IndustrialComplex_Factory::SendStorageInfo( SOPlayer *pPlayer )
{	
	if( IsStorage() == FALSE )		return	FALSE;	

	pPlayer->SendFieldMsg( ON_RESPONSE_STORAGE_INFO, SO_SFM_ONLYME, LPVOID( ON_RET_OK ), LPVOID( m_StorageItem ) );		 

	return	TRUE;
}*/


//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	�� ���忡�� ���� �� �� �ִ��� �˾ƿ´�. 
//	���� ����	:	2002-10-08 ���� 2:49:21 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
BOOL	SOVillage_IndustrialComplex_Factory::CanWork()
{	
	//
	if( m_mnPay <= 0 )
	{
		return	FALSE;
	}


	//���� �Ϸ�
	if( m_uiRemainAmountOfWork <= 0 )
	{
		return	FALSE;
	}

	return	TRUE;
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	�� ������ â������ �˾ƿ´�.
//	���� ����	:	2002-10-08 ���� 3:19:22 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
BOOL	SOVillage_IndustrialComplex_Factory::IsStorage()
{
	return	m_bStore;	
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	�� ������ �Ϲ����� �������� �˾ƿ´�.
//	���� ����	:	2002-10-15 ���� 10:01:46 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
BOOL	SOVillage_IndustrialComplex_Factory::IsFactory()
{
	return	!m_bStore;	
}


//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	�۾����� ���´�.
//	���� ����	:	2002-10-08 ���� 4:21:19 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
UI32	SOVillage_IndustrialComplex_Factory::GetAmountOfWork()
{
	if( CanWork() == FALSE )					return	0;

	return	m_uiRemainAmountOfWork;
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	������ �Ǽ��Ѵ�.
//	���� ����	:	2002-10-10 ���� 9:14:52 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
BOOL	SOVillage_IndustrialComplex_Factory::ConstructFactory( SOPlayer *pPlayer, SI32 siFactoryID, CHAR cFactoryCode[ 4 ], MONEY mnPay, MONEY *pmnCapital )
{
	DBIKFarmBuildStructInfo				BuildStructInfo;	
	stFacility							*pFactoryInfo;		
	MONEY								mnTotalMoney;
	
	// �ּ� �ӱ��� 10�� �̻��̾�� �Ѵ�.
	if( mnPay < 10)																					return	FALSE;
		
	// �ü����� ��ȿ���� �˻��Ѵ�.
	if( ( pFactoryInfo = m_pBuilding->GetFacility( cFactoryCode ) ) == NULL)						return	FALSE;

	// ���� ������� �˻��Ѵ�.
	mnTotalMoney		=	pFactoryInfo->UpLabor[ 0 ] * (mnPay + ON_TAX_PER_WORK);
	
	if( pPlayer->EnoughMoney( mnTotalMoney ) == FALSE )												return	FALSE;

	// ����� �����Ѵ�.
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

	// DB�� ������ ������Ʈ �Ѵ�.
	if( strcmp( cFactoryCode, "STG" ) == 0 )
	{
		m_pDataBase->writeDBQuery( SO_DB_INFOKIND_STORAGE_BUILD, &BuildStructInfo, sizeof( BuildStructInfo ) );

		m_bStore						=	TRUE;		
		//pFarmSlot->Storage.Init();
	}
	else
	{
		//	DB���� ���Թ� �� �� ��ȹ���� �����	
		m_pDataBase->writeDBQuery( SO_DB_INFOKIND_FARM_BUILDSTRUCT, &BuildStructInfo, sizeof( BuildStructInfo ) );

		m_bStore						=	FALSE;		
	

		m_uiMaxProduceItemQuantity		=	1;		
	}
	
		
	
	return	TRUE;
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	������ �ı��Ѵ�.
//	���� ����	:	2002-10-12 ���� 3:56:16 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
VOID	SOVillage_IndustrialComplex_Factory::DestroyFactory(  SOPlayer *pPlayer, MONEY *pmnCapital )
{
	
	if( IsStorage() == TRUE )
	{
		DBIKDestroyStorage		DS;

		// DB�� �����Ѵ�.
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

	// ���� �ϴ� ���ͱ��� ���´�.
//	*pmnCapital				=	( m_uiRemainAmountOfWork * m_mnPay) * ON_CAPITAL_RATE_PLANT; 

	// �÷��̾�� �޽����� �����ش�.
	if( pPlayer != NULL )	pPlayer->SendFieldMsg( ON_RESPONSE_FARM_DESTROYSTRUCT, SO_SFM_ONLYME, LPVOID( ON_RET_OK ) );	

	// ������ �ʱ�ȭ �Ѵ�.
	InitFactory();

}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	���� �����Ѵ�.
//	���� ����	:	2002-10-11 ���� 7:27:06 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
BOOL	SOVillage_IndustrialComplex_Factory::BeginWork( SOPlayer *pPlayer )
{
	// ���� �� �� �ִ��� �˻��Ѵ�.
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

//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	������ �ӱ��� �����Ѵ�.
//	���� ����	:	2002-10-11 ���� 9:00:24 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
BOOL	SOVillage_IndustrialComplex_Factory::ChangeFactoryPay( SOPlayer *pPlayer, MONEY mnPay, MONEY *pmnCapital )
{	
	MONEY						mnDifference;
	DBIKChangeFarmPay			stChangePay;

	
	// �۾��� �� �� �ִ� �������� �˻��Ѵ�.
	if( CanWork() == FALSE )																return	FALSE;

	if( mnPay < 10 )																		return	FALSE;
	
	mnDifference			=	(MONEY)( mnPay - m_mnPay ) * m_uiRemainAmountOfWork;

	if (mnDifference > 0)
	{
		if( pPlayer->EnoughMoney( mnDifference ) == FALSE )										return	FALSE;	// �÷��̾�� �����ϱ� ���� ����� ���� ����.
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

//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	���� �۾����� ���忡�� �۾����ΰ��� ����Ѵ�.
//	���� ����	:	2002-10-12 ���� 2:50:09 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
BOOL	SOVillage_IndustrialComplex_Factory::CancelWork( SOPlayer *pPlayer, SI32 siFactoryID, MONEY *pmnCapital )
{
	MONEY					mnDifference;
	DBIKFarmCancelPlan	FarmCancelData;

	// ������ ���� ������ �������� �˻��Ѵ�.	
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
		

	// ������ �Ǽ����̶�� ������ ���ش�.
	if( IsConstructing() == TRUE )	
	{
		WorkCompleted( pPlayer );	


		// �÷��̾�� �޽����� �����ش�.
		//if( pPlayer != NULL )	pPlayer->SendFieldMsg( ON_RESPONSE_FARM_DESTROYSTRUCT, SO_SFM_ONLYME, LPVOID( ON_RET_OK ) );			

		// ������ �ʱ�ȭ �Ѵ�.
		InitFactory();		
	}	
	else
	{

		WorkCompleted( pPlayer );	

	}
	
	pPlayer->SendFieldMsg( ON_RESPONSE_PLANT_CANCEL_PLAN, SO_SFM_ONLYME, LPVOID( ON_RET_OK ), LPVOID( &pPlayer->siMoney ) );	

	return	TRUE;
}


//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	�ǹ��� �Ǽ������� �˾ƿ´�.
//	���� ����	:	2002-10-12 ���� 3:58:30 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
BOOL	SOVillage_IndustrialComplex_Factory::IsConstructing()
{
	if( ( m_uiLevel == 0 ) && ( IsUpgrading() == TRUE ) )		return	TRUE;
	else														return	FALSE;
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	������ ���׷��̵������� �˾ƿ´�.
//	���� ����	:	2002-10-15 ���� 10:04:27 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
BOOL	SOVillage_IndustrialComplex_Factory::IsUpgrading()
{
	if( m_uiProducingItemID != ON_PLANT_SLOTUPGRADE_CODE )		return	FALSE;

	return	TRUE;
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	������ ���׷��̵� ��Ų��.
//	���� ����	:	2002-10-12 ���� 5:51:30 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
BOOL	SOVillage_IndustrialComplex_Factory::UpgradeFactory( SOPlayer *pPlayer, MONEY mnPay, MONEY *pmnCapital )
{
	DBIKSetUpgradeSlot				SetUpgradeSlot;
	stFacility						*pFacility;
	MONEY							mnTotalPay;
	UI32							uiAmountOfWork;


	// ���� ���� �� �� �ִ� ���¶�� ���׷��̸� �� �� ����.
	if( CanWork() == TRUE )																			return	FALSE;

	// �ּ� �ӱ��� 10�� �̻��̾�� �Ѵ�.
	if( mnPay < 10)																					return	FALSE;

	if( IsUpgradeCompleted() == TRUE )																return	FALSE;

	if( ( pFacility	 =	m_pBuilding->GetFacility( m_cFactoryKind ) ) == NULL )						return	FALSE;	

	memset(&SetUpgradeSlot, 0, sizeof(DBIKSetUpgradeSlot));	
	
	uiAmountOfWork			=	pFacility->UpLabor[ m_uiLevel ];	
	
	mnTotalPay				=	mnPay * uiAmountOfWork + uiAmountOfWork * ON_TAX_PER_WORK;
	
	if( pPlayer->EnoughMoney( mnTotalPay ) == FALSE )												return	FALSE;

	pPlayer->DecreaseMoney( mnTotalPay ,FALSE);	

	// ���� ������Ʈ.	
	m_mnPay						=	mnPay;
	m_uiProducingItemID			=	ON_PLANT_SLOTUPGRADE_CODE;
	m_uiMaxProduceItemQuantity	=	1;
	
	m_uiRemainAmountOfWork		=	uiAmountOfWork;

	// ������ ���ͱ�.
	*pmnCapital					=	uiAmountOfWork * ON_TAX_PER_WORK;

	

	
	
	// DB�� �Է�.
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

//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	�������� �����Ѵ�.
//	���� ����	:	2002-10-12 ���� 7:17:40 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
DWORD	SOVillage_IndustrialComplex_Factory::ProduceItem( SOPlayer *pPlayer, UI16 uiItemID, UI32 uiItemQuantity, MONEY mnPay, CHAR cNation, MONEY *pmnCapital )
{
	stFacility						*pFacility;
	DBIKSetFarmProduct				SetFarmProduct;
	MONEY							mnTotalPay,moTax;
	UI32							uiAmountOfWork;

	// �ּ� �ӱ��� 10�� �̻��̾�� �Ѵ�.
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

	// ��ȹ�� �����. - DB�� �Է��Ѵ�.
	SetFarmProduct.siDBAccount              =   pPlayer->stAccountInfo.siDBAccount;
	SetFarmProduct.uiSlot                   =   pPlayer->stAccountInfo.uiSlot;
	SetFarmProduct.uiVillageCode		    =	pPlayer->stAccountInfo.pVillage->uiVillageCode;
	SetFarmProduct.uiFarmID					=	m_siIndustrialComplexID;
	SetFarmProduct.uiFactoryID				=	m_siFactoryID;
	SetFarmProduct.uiItemID					=	uiItemID;
	SetFarmProduct.Quantity					=	uiItemQuantity;			// ���� ��
	SetFarmProduct.moPay					=	mnPay;			// �ӱ�
	SetFarmProduct.Labor                    =   uiAmountOfWork;
	SetFarmProduct.moTax                    =   moTax;
	SetFarmProduct.moProfit                 =   moTax;
	

	m_pDataBase->writeDBQuery( SO_DB_INFOKIND_FARM_PRODUCTITEM, &SetFarmProduct, sizeof( SetFarmProduct ) );

	m_uiProducingItemID						=	uiItemID;	

	m_uiMaxProduceItemQuantity				=	uiItemQuantity;	
	m_mnPay									=	mnPay;
	
	m_uiRemainAmountOfWork					=	uiAmountOfWork;	

	// ������ ���ͱ�.
	*pmnCapital								=	moTax;

	pPlayer->DecreaseMoney( mnTotalPay ,FALSE);

	pPlayer->SendFieldMsg( ON_RESPONSE_FARM_ITEMPRODUCT, SO_SFM_ONLYME, LPVOID( ON_RET_OK ) );

	// �� ����ȿ� �÷��̾ �־��ش�.
	m_PlayerMgr.AddPlayer( pPlayer );

	return ON_RET_OK;		
}


//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	������ ������ �뵿������ �ٲ۴�.
//	���� ����	:	2002-10-12 ���� 7:56:15 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������
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

//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	�뵿���� �������� ������ �ٲ۴�.
//	���� ����	:	2002-10-14 ���� 7:23:53 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
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

//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	â�� �ִ� �������� ���´�.
//	���� ����	:	2002-10-12 ���� 8:56:28 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
STORAGE_ITEM*		SOVillage_IndustrialComplex_Factory::GetItem( UI16 uiItemID, SI32 *psiIndex )
{
	SI32	i;

	if( IsStorage() == FALSE )			return	NULL;

	for( i = 0; i < ON_MAX_STORAGEITEM; i++ )
	{
		// 0���ٴ� ������ Ŀ���Ѵ�.
		if( m_StorageItem[ i ].uiItemCode == uiItemID && m_StorageItem[i].uiQuantity > 0)
		{
			if( psiIndex != NULL )			*psiIndex	=	i;

			return	&m_StorageItem[ i ];
		}
	}

	return	NULL;
}



//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	â�� ����ִ� �������� ���´�.
//	���� ����	:	2002-10-14 ���� 11:08:03 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
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



//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	â�� �ִ� �������� ������ DB�� �����Ѵ�.
//	���� ����	:	2002-10-14 ���� 4:05:23 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
VOID	SOVillage_IndustrialComplex_Factory::SetItemInfoToDB( SI32 siItemIndex, UI16 uiItemID, UI16 uiItemQuantity )
{
	DBIKSetStorageItem		stSetStorageItem;

	
	// DB�� �����Ѵ�.
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

	
	// DB�� �����Ѵ�.
	stSetStorageItem.uiVillageCode		=	m_uiVillageCode;
	
	stSetStorageItem.uiFarmID			=	m_siIndustrialComplexID;
	stSetStorageItem.uiFactoryID		=	m_siFactoryID;
	stSetStorageItem.uiItemIndex		=	siItemIndex;
	stSetStorageItem.uiItemCode			=	uiItemID;
	stSetStorageItem.uiItemQnt			=	uiItemQuantity;
	
	m_pDataBase->writeDBQuery( SO_DB_INFOKIND_STORAGE_DELETEITEM, &stSetStorageItem, sizeof( stSetStorageItem ) );
}


//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	������ �۾��� �Ϸ�Ǿ���.
//	���� ����	:	2002-10-24 ���� 9:41:14 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
VOID	SOVillage_IndustrialComplex_Factory::WorkCompleted( SOPlayer *pPlayerCompletedWork )
{
	SI32	siPlayerNumInFactory;
	UI16	uiWorkingPlayerID[ ON_MAX_FACTORY_PLAYER_NUM ];

	// �켱 �۾��� �ʱ�ȭ �Ѵ�.
	InitWork();

	// ���� �۾����� ��� �÷��̾ �����Ѵ�.
	m_WorkingPlayerMgr.RemoveAllPlayer();	



	// �۾��� �Ϸ��� �÷��̾�� �޽����� �̸� �����Ѵ� (NPC�϶����� NULL�� �ü�������).

	if (pPlayerCompletedWork) {
		m_PlayerMgr.RemovePlayer( pPlayerCompletedWork );
	}

	// ���� �۾����� ��� �÷��̾�� �۾��� �Ϸ�Ǿ��ٰ� �޽����� �����ش�.

	if( ( siPlayerNumInFactory = m_PlayerMgr.GetAddedPlayerList( uiWorkingPlayerID ) ) == 0 )
	{
		return;
	}

	m_PlayerMgr.GetFirstPlayer()->SendFieldMsg( ON_FARMWORK_COMPLETED, SO_SFM_CUSTOM, LPVOID( siPlayerNumInFactory ), LPVOID( uiWorkingPlayerID ) );

	// ���� �� �ǹ��� �����մ� ��� �÷��̾ �����Ѵ�.
	m_PlayerMgr.RemoveAllPlayer();	
	
}


//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	â�� �ȿ� ����� �������� �ִ��� �˻��Ѵ�.
//	���� ����	:	2002-10-12 ���� 8:30:14 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
BOOL	SOVillage_IndustrialComplex_Factory::HasItem( UI16 uiItemID, UI16 uiItemQuantity )
{
	STORAGE_ITEM	*pItem;

	if (m_uiLevel < 1)                                      return  FALSE;

	if( ( pItem = GetItem( uiItemID, NULL ) ) == NULL )		return	FALSE;	
	if( pItem->uiQuantity < uiItemQuantity )				return	FALSE;			
	
	return TRUE;
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	â�� �ȿ� �������� �ִ´�.
//	���� ����	:	2002-10-14 ���� 10:55:47 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
BOOL	SOVillage_IndustrialComplex_Factory::AddItem( UI16 uiItemID, UI16 uiItemQuantity ,BOOL bSendToDB)
{
	SI32					siItemIndex = -1;	
	STORAGE_ITEM			*pItem;	

	if (IsConstructing())											return  FALSE;

	// â�� ������ ���� �������� �ִ��� �˻��Ѵ�	
	if( ( pItem = GetItem( uiItemID, &siItemIndex ) ) != NULL )
	{
		// �ִ� ���� �� �ִ� �������� ��� �ѵ��� �ʰ��ϴ��� �˻��Ѵ�
		if( ( 32767 - pItem->uiQuantity ) < uiItemQuantity )	{
			// �ѵ��� ������ �׳ɽ��� ( �� â�� �������� �������� �� ĭ�� �����Ҽ� ����.)
			return	FALSE;
		}

		// �������� ������ �ø���.
		pItem->uiQuantity	+=	uiItemQuantity;	

		// DB�� �����Ѵ�.
		if (bSendToDB) {
			SetItemInfoToDB( siItemIndex, pItem->uiItemCode, pItem->uiQuantity );
		}

		return	TRUE;
	}
	// ���ٸ� �������� ���� ���ο� ��Ҹ� ã�´�.
	else  if( ( pItem = GetEmptyItem( &siItemIndex ) ) != NULL )
	{
		// �ִ븦 ������ ����
		if (uiItemQuantity >= 32767) return FALSE;

		pItem->uiItemCode	=	uiItemID;
		pItem->uiQuantity	=	uiItemQuantity;	

		// DB�� �����Ѵ�.
		if (bSendToDB) {
			SetItemInfoToDB( siItemIndex, pItem->uiItemCode, pItem->uiQuantity );
		}

		return	TRUE;
	}	
	
	return	FALSE;	

}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	â��ȿ��� �������� �����Ѵ�.
//	���� ����	:	2002-10-12 ���� 8:55:03 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
BOOL	SOVillage_IndustrialComplex_Factory::DeleteItem( UI16 uiItemID, UI16 uiItemQuantity ,BOOL bSendToDB)
{
	SI32					siItemIndex = -1;
	STORAGE_ITEM			*pItem;

	if (IsConstructing())											return  FALSE;

	if( HasItem( uiItemID, uiItemQuantity ) == FALSE )				return	FALSE;

	if( ( pItem = GetItem( uiItemID, &siItemIndex ) ) == NULL )		return	FALSE;

	if (siItemIndex == -1)                                          return  FALSE;
	
	pItem->uiQuantity	-=	uiItemQuantity;

	// �������� ���ش�.
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


//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	â�� �ȿ� �������� �ִ´�.
//	���� ����	:	2002-10-14 ���� 11:31:34 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
BOOL	SOVillage_IndustrialComplex_Factory::AddItem( SOPlayer *pPlayer, UI16 uiItemID, UI16 uiItemQuantity ,BOOL bSendToDB)
{
	if( AddItem( uiItemID, uiItemQuantity ,bSendToDB) == FALSE )		return	FALSE;
	

	
	return	TRUE;
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	â��ȿ��� �������� �����Ѵ�.	
//	���� ����	:	2002-10-14 ���� 11:35:46 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
BOOL	SOVillage_IndustrialComplex_Factory::DeleteItem( SOPlayer *pPlayer, UI16 uiItemID, UI16 uiItemQuantity ,BOOL bSendToDB)
{

	
	if( DeleteItem( uiItemID, uiItemQuantity ,bSendToDB) == FALSE )		return	FALSE;
	

	
	return	TRUE;	
}


//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	â�� �ȿ� �������� ���� �� �ִ��� �˻��Ѵ�.
//	���� ����	:	2002-12-17 ���� 9:51:00 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������

BOOL	SOVillage_IndustrialComplex_Factory::CanAddItem( UI16 uiItemID ,UI16 uiQuantity,SI32 *pStorageIndex)
{
	// â������ �˻��Ѵ�.
	*pStorageIndex = -1;
	if( IsStorage() == FALSE )						return	FALSE;

	// �Ǽ����̸� �ȵ�
	if( m_uiLevel < 1)                               return  FALSE;


	STORAGE_ITEM *pItem;

	pItem = GetItem( uiItemID, pStorageIndex );
	if( pItem != NULL )			{

		int currentitem = pItem->uiQuantity;
		int newitem = uiQuantity;
		// ������������ ������ ������ �˻��Ѵ�.
		if ( (currentitem + newitem) < 32767) {
			return TRUE;
		}
		// ������ �ʰ��ϸ� ������� �ִ��� FALSE�� �����ؾ��Ѵ�.
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


//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	���忡�� �������� �������� ID�� ���´�.
//	���� ����	:	2002-10-15 ���� 10:06:54 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
UI16	SOVillage_IndustrialComplex_Factory::GetProducingItemID()
{
	return	m_uiProducingItemID;
}
