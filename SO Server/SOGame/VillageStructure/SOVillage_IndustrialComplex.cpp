#include "SOMain.h"
#include "SODatabase.h"
#include "SOPlayer.h"
#include "SOVillage_IndustrialComplex_Factory.h"
#include "SOVillage_IndustrialComplex.h"

CItem			*SOVillage_IndustrialComplex::m_pItem;
SODatabase		*SOVillage_IndustrialComplex::m_pDataBase;
OnlineBuilding	*SOVillage_IndustrialComplex::m_pBuilding;

//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	���� ������ �ʱ�ȭ �Ѵ�.		
//	���� ����	:	2002-10-08 ���� 9:47:22 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
VOID	SOVillage_IndustrialComplex::InitStaticVariable( CItem *pItem, SODatabase *pDataBase, OnlineBuilding *pBuilding )
{
	m_pItem			=	pItem;
	m_pDataBase		=	pDataBase;
	m_pBuilding		=	pBuilding;
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	������.	
//	���� ����	:	2002-10-07 ���� 3:24:56 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
SOVillage_IndustrialComplex::SOVillage_IndustrialComplex()
{	
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	�Ҹ���.	
//	���� ����	:	2002-10-07 ���� 3:27:52 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
SOVillage_IndustrialComplex::~SOVillage_IndustrialComplex()
{	
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	�ʱ�ȭ.
//	���� ����	:	2002-10-07 ���� 3:30:22 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
VOID	SOVillage_IndustrialComplex::Init( SI32 siIndustrialComplexID ,UI16 uiVillageCode )
{
	SI32	i;
	
	m_PlayerMgr.Init( ON_MAX_INDUSTRIAL_COMPLEX_PLAYER_NUM );

	m_uiVillageCode             =   uiVillageCode;
	m_siIndustrialComplexID		=	siIndustrialComplexID;
	m_siOwnerDBAccount			=	0;
	m_uiOwnerDBSlot				=	0;

	m_siDayNotWorked			=	0;	
	
	for( i = 0; i < ON_MAX_FACTORY_NUM; i++ )
	{
		m_Factory[ i ].Init( m_uiVillageCode,m_siIndustrialComplexID, i );
	}
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	��� ������ �ȷ��� �ÿ� ���ش�.
//	���� ����	:	2002-10-15 ���� 2:39:28 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
VOID	SOVillage_IndustrialComplex::InitIndustrialComplex()
{
	SI32	i;

	m_PlayerMgr.RemoveAllPlayer();

	m_siOwnerDBAccount				=	0;
	m_uiOwnerDBSlot					=	0;
	ZeroMemory( m_szOwnerName, sizeof( m_szOwnerName ) );

	m_siMaxConstructibleFactoryNum	=	0;
	m_siDayNotWorked				=	0;		

	for( i = 0; i < ON_MAX_FACTORY_NUM; i++ )
	{
		m_Factory[ i ].InitFactory();
	}	
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	�޸� ����.
//	���� ����	:	2002-10-11 ���� 1:24:12 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
VOID	SOVillage_IndustrialComplex::Free()
{

	SI32	i;

	for( i = 0; i < ON_MAX_FACTORY_NUM; i++ )
	{
		m_Factory[ i ].Free();
	}

	m_PlayerMgr.Free();
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	������ ���´�.
//	���� ����	:	2002-10-07 ���� 3:41:00 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
SOVillage_IndustrialComplex_Factory*	SOVillage_IndustrialComplex::GetFactory( SI32 siFactoryID )
{
	if( IsValidFactoryID( siFactoryID ) == FALSE )		return	NULL;	

	return &m_Factory[ siFactoryID ];
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	������ �Ǽ��� ���� ��ü�� ���´�.
//	���� ����	:	2002-10-14 ���� 2:31:09 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
SOVillage_IndustrialComplex_Factory*	SOVillage_IndustrialComplex::GetFactoryConstructed( SI32 siFactoryID )
{
	SOVillage_IndustrialComplex_Factory		*pFactory;

	if( ( pFactory = GetFactory( siFactoryID ) ) == NULL )			return	NULL;

	if( pFactory->DidConstructFactory() == FALSE )					return	NULL;

	return	pFactory;
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	�÷��̾ �� ������ ���´�.
//	���� ����	:	2002-10-11 ���� 4:29:01 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
SOVillage_IndustrialComplex_Factory*		SOVillage_IndustrialComplex::GetEnteredFactory( SOPlayer *pPlayer )
{
	SI32	i;
	
	for( i = 0; i < ON_MAX_FACTORY_NUM; i++ )
	{
		if( m_Factory[ i ].IsPlayerIn( pPlayer ) == TRUE )
			return	&m_Factory[ i ];
	}

	return	NULL;
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	�÷��̾ �� ������ ���´�.
//	���� ����	:	2002-10-11 ���� 4:29:01 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
SI32 SOVillage_IndustrialComplex::GetEnteredFactoryIndex( SOPlayer *pPlayer )
{
	SI32	i;
	
	for( i = 0; i < ON_MAX_FACTORY_NUM; i++ )
	{
		if( m_Factory[ i ].IsPlayerIn( pPlayer ) == TRUE )
			return	i;
	}

	return	-1;
}



//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	��ȿ�� ���� ID �ΰ�?
//	���� ����	:	2002-10-07 ���� 3:41:33 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
BOOL	SOVillage_IndustrialComplex::IsValidFactoryID( SI32 siFactoryID )
{
	if( ( siFactoryID >= 0 ) && ( siFactoryID < ON_MAX_FACTORY_NUM ) )		return	TRUE;

	return	FALSE;
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	�÷��̾ ���Դ�.
//	���� ����	:	2002-10-07 ���� 4:03:27 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
BOOL	SOVillage_IndustrialComplex::EnterPlayer( SOPlayer *pPlayer )
{
	SI32				siFactoryNum;
	OnFarmSlotList		FactoryInfo[ ON_MAX_FACTORY_NUM ];	
	DWORD				dwInfoLength;
	
	dwInfoLength	=	GetInfo( FactoryInfo, &siFactoryNum );

	// ������ ������ �÷��̾�� �����ش�.
	if( m_PlayerMgr.AddPlayer( pPlayer ) == TRUE)
	{
		pPlayer->SendFieldMsg( ON_RESPONSE_FARM_SLOTINFO, SO_SFM_ONLYME, LPVOID( siFactoryNum ), LPVOID( FactoryInfo ), LPVOID( dwInfoLength ), 
								LPVOID( m_siMaxConstructibleFactoryNum ) );
		return	TRUE;
	}
	else
		return	FALSE;
	
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	�÷��̾ ������.
//	���� ����	:	2002-10-07 ���� 4:23:50 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
BOOL	SOVillage_IndustrialComplex::LeavePlayer( SOPlayer *pPlayer )
{
	// ���� �� �ִ� ���忡���� ��������.
	LeaveFactory( pPlayer );
	
	return	m_PlayerMgr.RemovePlayer( pPlayer );
}


//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	��� ������ �ִ� ������� ������ ���´�.
//	���� ����	:	2002-10-07 ���� 4:50:01 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
DWORD	SOVillage_IndustrialComplex::GetInfo( OnFarmSlotList* pFactoryInfo, SI32 *psiFactoryNum )
{
	SI32		i;
	DWORD		dwTempInfoLength, dwInfoLength;		

	dwInfoLength		=	0;
	*psiFactoryNum		=	0;
	for( i = 0; i < ON_MAX_FACTORY_NUM; i++ )
	{
		dwTempInfoLength	=	m_Factory[ i ].GetInfo( ( ( OnFarmSlotList* ) ( ( CHAR*) pFactoryInfo + dwInfoLength ) ) );

		if( dwTempInfoLength != 0 )
		{
			dwInfoLength	+=	dwTempInfoLength;
			( *psiFactoryNum )++;
		}
	}

	return	dwInfoLength;
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	��� ������ ������ ���´�.
//					2�� ����) �ִ� �ӱ��̶�� �ϴ��� ���� �� �� ȯ���̸�(���� ����) �ִ� �ӱ����� �������� �ʴ´�.
//	���� ����	:	2003-07-03 ���� 06:31:00 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
DWORD	SOVillage_IndustrialComplex::GetInfo( OnIndustrialComplexInfo *pIndustrialComplexInfo )
{
	SI32									i;
	SOVillage_IndustrialComplex_Factory		*pTempFactory;

	if( HasOwner() == FALSE )		return	0;

	pIndustrialComplexInfo->siID					=	m_siIndustrialComplexID;	
	pIndustrialComplexInfo->m_moMaxPay              =   0;

	pIndustrialComplexInfo->m_uiTotalAmountOfWork	=	0;
	for( i = 0; i < ON_MAX_FACTORY_NUM; i++ )
	{
		if( ( pTempFactory = GetFactoryConstructed( i ) ) == NULL )		continue;
		if( CanWork( pTempFactory ) == FALSE )							continue;		
		
		pIndustrialComplexInfo->m_uiTotalAmountOfWork	+=		pTempFactory->GetAmountOfWork();
		pIndustrialComplexInfo->m_moMaxPay = max( pIndustrialComplexInfo->m_moMaxPay, pTempFactory->GetPay() );

	}
	
	ZeroMemory( pIndustrialComplexInfo->szName, sizeof( pIndustrialComplexInfo->szName ) );
	strncpy( pIndustrialComplexInfo->szName, m_szOwnerName, ON_ID_LENGTH );

	return	sizeof( OnIndustrialComplexInfo );
}


//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	�� ��������� ������ �ִ°�.
//	���� ����	:	2002-10-07 ���� 7:44:47 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
BOOL	SOVillage_IndustrialComplex::HasOwner()
{
	if( m_siOwnerDBAccount != 0 )		return	TRUE;

	return	FALSE;	
}


//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	DB���� ���� ������ ��� ������ �ʱ�ȭ �Ѵ�.
//	���� ����	:	2002-10-07 ���� 8:23:51 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
VOID	SOVillage_IndustrialComplex::InitFromDB( SI32 siOwnerDBAccount, UI08 uiOwnerDBSSlot, CHAR *pszOwnerName, 
													SI32 siMaxConstructibleFactoryNum, SI32 siDayNotWorked )
{
	m_siOwnerDBAccount	=	siOwnerDBAccount;
	m_uiOwnerDBSlot		=	uiOwnerDBSSlot;

	ZeroMemory( m_szOwnerName, sizeof( m_szOwnerName ) );
	strncpy( m_szOwnerName, pszOwnerName, ON_ID_LENGTH );

	m_siMaxConstructibleFactoryNum		=	siMaxConstructibleFactoryNum;
	m_siDayNotWorked					=	siDayNotWorked;
}


//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	DB���� ���� ������ â�� �ʱ�ȭ �Ѵ�.
//	���� ����	:	2002-10-07 ���� 9:27:47 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
BOOL	SOVillage_IndustrialComplex::InitStorageFromDB( SI32 siFactoryID, UI08 uiLevel, MONEY mnPay, UI32 uiAmountOfWork, 
									UI16 uiItemID0, UI16 uiItemID1, UI16 uiItemID2, UI16 uiItemID3, UI16 uiItemID4, 
									UI16 uiItemID5, UI16 uiItemID6, UI16 uiItemID7, UI16 uiItemID8, UI16 uiItemID9, 
									UI16 uiQuantity0, UI16 uiQuantity1, UI16 uiQuantity2, UI16 uiQuantity3, UI16 uiQuantity4, 
									UI16 uiQuantity5, UI16 uiQuantity6, UI16 uiQuantity7, UI16 uiQuantity8, UI16 uiQuantity9 )
{
	SOVillage_IndustrialComplex_Factory		*pFactory;

	if( ( pFactory = GetFactory( siFactoryID ) ) == NULL )	return	FALSE;

	return	pFactory->InitStorageFromDB( uiLevel, mnPay, uiAmountOfWork,
										uiItemID0, uiItemID1, uiItemID2, uiItemID3, uiItemID4, 
										uiItemID5, uiItemID6, uiItemID7, uiItemID8, uiItemID9,
										uiQuantity0, uiQuantity1, uiQuantity2, uiQuantity3, uiQuantity4, 
										uiQuantity5, uiQuantity6, uiQuantity7, uiQuantity8, uiQuantity9 );
}


//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	DB���� ���� ������ ������ �ʱ�ȭ �Ѵ�.
//	���� ����	:	2002-10-08 ���� 4:49:25 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
BOOL	SOVillage_IndustrialComplex::InitFactoryFromDB( SI32 siFactoryID, UI08 uiLevel, MONEY mnPay, UI32 uiAmountOfWork, CHAR cFactoryKind[ 4 ],
									UI16 uiProducingItemID, UI32 uiProducedItemQuantity, UI32 uiMaxProduceItemQuantity )
{
	SOVillage_IndustrialComplex_Factory		*pFactory;

	if( ( pFactory = GetFactory( siFactoryID ) ) == NULL )	return	FALSE;

	return	pFactory->InitFactoryFromDB( uiLevel, mnPay, uiAmountOfWork, cFactoryKind, 
											uiProducingItemID, uiProducedItemQuantity, uiMaxProduceItemQuantity );
}


//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	������ ó���Ѵ�.
//	���� ����	:	2002-10-08 ���� 11:38:58 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
BOOL	SOVillage_IndustrialComplex::SellFactory( SOPlayer *pPlayer, SI32 siFactoryID )
{
	SOVillage_IndustrialComplex_Factory		*pFactory;

	if( ( pFactory = GetFactoryConstructed( siFactoryID ) ) == NULL )	return	FALSE;	

	return	pFactory->SellFactory( pPlayer );
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	�� ��������� ������ ���´�.
//	���� ����	:	2002-10-08 ���� 11:49:18 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
BOOL	SOVillage_IndustrialComplex::GetOwner( SI32 *psiOwnerDBAccount, UI08 *puiOwnerDBSlot )
{
	if( HasOwner() == FALSE )		return	FALSE;

	*psiOwnerDBAccount	=	m_siOwnerDBAccount;
	*puiOwnerDBSlot		=	UI08( m_uiOwnerDBSlot );
	
	return	TRUE;
}


//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	�÷��̾ �� ��� �����ȿ� ���� �ִ��� �˻��Ѵ�.
//	���� ����	:	2002-10-08 ���� 12:25:29 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
BOOL	SOVillage_IndustrialComplex::IsPlayerIn( SOPlayer *pPlayer )
{
	return	m_PlayerMgr.IsAddedPlayer( pPlayer );
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	���� �����ƴ�.
//	���� ����	:	2002-10-08 ���� 12:28:51 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
BOOL	SOVillage_IndustrialComplex::FinishWork( SOPlayer *pPlayer, OnReqFarmWorkMsg *pMsg )
{
	SOVillage_IndustrialComplex_Factory		*pFactory;
	UI16									uiProducedItemID = 0;
	UI16									uiProducedItemQuantity = 0;
	
	if( ( pFactory = GetFactoryPlayerIn( pPlayer ) ) == NULL )																return	FALSE;

	if( pFactory->FinishWork(pPlayer, this, pMsg) == FALSE )		return	FALSE;
	
	// ����Ǿ��� �������� �ִ��� �˻��Ѵ�.
	/*if( ( uiProducedItemID != 0 ) && ( uiProducedItemQuantity != 0 ) )
	{
		// �ƹ� â�� �ȿ� �������� �ִ´�.
		// �����ص� ��¿���� ����.
		AddItem( uiProducedItemID , uiProducedItemQuantity );
	}*/

	m_siDayNotWorked	=	0;

	return	TRUE;
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	�÷��̾ �� �ִ� ������ ���´�.
//	���� ����	:	2002-10-08 ���� 12:31:22 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
SOVillage_IndustrialComplex_Factory*	SOVillage_IndustrialComplex::GetFactoryPlayerIn( SOPlayer *pPlayer )
{
	SI32	i;

	for( i = 0; i < ON_MAX_FACTORY_NUM; i++ )
	{
		if( m_Factory[ i ].IsPlayerIn( pPlayer ) == TRUE )
			return	&m_Factory[ i ];
	}

	return	NULL;
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	�������� �����ϱ� ���Ͽ� â�� ����� ������ ��ᰡ �ִ��� �˻��Ѵ�.
//	���� ����	:	2002-10-12 ���� 8:10:40 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
BOOL	SOVillage_IndustrialComplex::IsMaterialItemEnoughToProduce( UI16 uiItemID, UI32 uiItemQuantity )
{
	UI16						uiMaterialItemID;
	UI16						uiMaterialItemQuantity;	
	CItemHeader					*pItemHeader;
	MakeItem					*pstMakeitem;
	SI32						i, j;
	
	if( ( pItemHeader = m_pItem->GetItem( uiItemID ) ) == NULL )
	{
		return	FALSE;
	}
	if( ( pstMakeitem = pItemHeader->pMakeItem ) == NULL )
	{
		return	FALSE;
	}

	
	for( i = 0; i < ON_ITEM_MAX_ADDMAKE; i++)
	{			
		// ��� �������� ID�� 0�϶������� �����Ѵ�.
		if( ( uiMaterialItemID = pstMakeitem->m_uiItemID[ i ] ) == 0 )		break;

		uiMaterialItemQuantity	= uiItemQuantity / pstMakeitem->m_siProduction * pstMakeitem->m_siItemNum[ i ];
		if( ( uiItemQuantity % pstMakeitem->m_siProduction ) != 0 )		uiMaterialItemQuantity	+=	pstMakeitem->m_siItemNum[ i ];

		for( j = 0; j < ON_MAX_FACTORY_NUM; j++ )
		{			
			if( ( m_Factory[ j ].DidConstructFactory() == TRUE ) && ( m_Factory[ j ].IsStorage() == TRUE ) )
			{				
				// â���� ���� ��� �������� �ִ��� �˻��Ѵ�.
				if( m_Factory[ j ].HasItem( uiMaterialItemID, uiMaterialItemQuantity ) == TRUE )	break;				
			}
		}		

		// â�� �������� ����.
		if( j == ON_MAX_FACTORY_NUM )
		{
			return	FALSE;
		}
	}
	
	return	TRUE;
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	�������� �����ϱ� ���Ͽ� â�� ����� ������ ������ �ִ��� �˻��Ѵ�.
//	���� ����	:	2002-10-12 ���� 8:36:07 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
BOOL	SOVillage_IndustrialComplex::IsMaterialToolEnoughToProduce( UI16 uiItemID )
{
	UI16						uiMaterialItemID;
	MakeItem					*pstMakeitem;		
	CItemHeader					*pItemHeader;
	SI32						i, j;

	if( ( pItemHeader = m_pItem->GetItem( uiItemID ) ) == NULL )
	{
		return	FALSE;
	}
	if( ( pstMakeitem = pItemHeader->pMakeItem ) == NULL )					
	{
		return	FALSE;	
	}
	
	for( i = 0; i < ON_ITEM_MAX_TOOL; i++ )
	{	
		// ��� �������� ID�� 0�϶������� �����Ѵ�.
		if( ( uiMaterialItemID = pstMakeitem->m_uiToolID[ i ] ) == 0 )		
		{
			break;	
		}

		for( j = 0; j < ON_MAX_FACTORY_NUM; j++ )
		{
			if( ( m_Factory[ j ].DidConstructFactory() == TRUE ) && ( m_Factory[ j ].IsStorage() == TRUE ) )
			{
				// â���� ���� ���� �������� �ִ��� �˻��Ѵ�.
				if( m_Factory[ j ].HasItem( uiMaterialItemID, 1) == TRUE )		
				{
					break;
				}
			}
		}

		// â�� �������� ����.
		if( j == ON_MAX_FACTORY_NUM )
		{
			return	FALSE;
		}
	}

	return TRUE;
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	�� �������� �����ϱ� ���Ͽ� �ʿ��� ��Ḧ �����Ѵ�.
//	���� ����	:	2002-10-12 ���� 9:32:40 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
BOOL	SOVillage_IndustrialComplex::DeleteMaterialItem( UI16 uiItemID, UI32 uiItemQuantity )
{
	UI16						uiMaterialItemID;
	UI32						uiMaterialItemQuantity = 0;	
	CItemHeader					*pItemHeader;
	MakeItem					*pstMakeitem;
	SI32						i, j;
	
	if( ( pItemHeader = m_pItem->GetItem( uiItemID ) ) == NULL )			return	FALSE;
	if( ( pstMakeitem = pItemHeader->pMakeItem ) == NULL )					return	FALSE;

	
	for( i = 0; i < ON_ITEM_MAX_ADDMAKE; i++)
	{			
		// ��� �������� ID�� 0�϶������� �����Ѵ�.
		if( ( uiMaterialItemID = pstMakeitem->m_uiItemID[ i ] ) == 0 )		break;

		uiMaterialItemQuantity	= (uiItemQuantity / pstMakeitem->m_siProduction) * pstMakeitem->m_siItemNum[ i ];
		if( ( uiItemQuantity % pstMakeitem->m_siProduction ) != 0 )		uiMaterialItemQuantity	+=	pstMakeitem->m_siItemNum[ i ];

		for( j = 0; j < ON_MAX_FACTORY_NUM; j++ )
		{			
			if( ( m_Factory[ j ].DidConstructFactory() == TRUE ) && ( m_Factory[ j ].IsStorage() == TRUE ))
			{				
				// â���� ���� ��� �������� �ִ��� �˻��Ѵ�.
				if( m_Factory[ j ].HasItem( uiMaterialItemID, uiMaterialItemQuantity ) == TRUE )
				{
					// â���� �������� �����Ѵ�.
					m_Factory[ j ].DeleteItem( uiMaterialItemID, uiMaterialItemQuantity );

					break;				
				}
			}
		}				
	}	
	
	return	TRUE;
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	�� �������� �����ϱ� ���Ͽ� �ʿ��� ������ �����ϰ� �ı� ��Ų��.
//	���� ����	:	2002-10-12 ���� 9:04:29 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
BOOL	SOVillage_IndustrialComplex::DestroyToolItemRandomly( UI16 uiItemID )
{
	UI16						uiMaterialItemID;
	MakeItem					*pstMakeitem;		
	CItemHeader					*pItemHeader;
	SI32						i, j;
	
	if( ( pItemHeader = m_pItem->GetItem( uiItemID ) ) == NULL )			return	FALSE;
	if( ( pstMakeitem = pItemHeader->pMakeItem ) == NULL )					return	FALSE;	
	
	for( i = 0; i < ON_ITEM_MAX_TOOL; i++ )
	{	
		// ��� �������� ID�� 0�϶������� �����Ѵ�.
		if( ( uiMaterialItemID = pstMakeitem->m_uiToolID[ i ] ) == 0 )		break;	

		for( j = 0; j < ON_MAX_FACTORY_NUM; j++ )
		{
			if( ( m_Factory[ j ].DidConstructFactory() == TRUE ) && ( m_Factory[ j ].IsStorage() == TRUE ) )
			{
				// â���� ���� ���� �������� �ִ��� �˻��Ѵ�.
				if( m_Factory[ j ].HasItem( uiMaterialItemID, 1) == TRUE )
				{
					// �����ϰ� ������ ���������� �Ѵ�.
					if( ( rand() % 20 ) == 0 )
						m_Factory[ j ].DeleteItem( uiMaterialItemID, 1 );

					break;
				}
			}
		}		
	}

	return TRUE;
}


/*
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	��� ������ �ִ� ������ ������ �÷��̾�� �����ش�.
//	���� ����	:	2002-10-08 ���� 2:25:10 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
BOOL	SOVillage_IndustrialComplex::SendFactoryInfo( SOPlayer *pPlayer, SI32 siFactoryID )
{
	SOVillage_IndustrialComplex_Factory		*pFactory;

	if( ( pFactory = GetFactory( siFactoryID ) ) == NULL )							return	FALSE;

	return	pFactory->SendFactoryInfo( pPlayer );
}


//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	��� ������ �ִ� â���� ������ �÷��̾�� �����ش�.
//	���� ����	:	2002-10-08 ���� 3:17:13 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
BOOL	SOVillage_IndustrialComplex::SendStorageInfo( SOPlayer *pPlayer, SI32 siFactoryID )
{
	SOVillage_IndustrialComplex_Factory		*pFactory;

	if( ( pFactory = GetFactoryConstructed( siFactoryID ) ) == NULL )					return	FALSE;

	return	pFactory->SendStorageInfo( pPlayer );	
}

*/
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	��� ������ �����Ѵ�.
//	���� ����	:	2002-10-08 ���� 8:37:25 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
BOOL	SOVillage_IndustrialComplex::BuyIndustrialComplex( SOPlayer *pPlayer, SI32 siMaxConstructibleFactoryNum)
{
	

	// �̹� ������ �ִ� �˻��Ѵ�.
	if( HasOwner() == TRUE )						return	FALSE;

	
	m_siOwnerDBAccount					=	pPlayer->stAccountInfo.siDBAccount;
	m_uiOwnerDBSlot						=	pPlayer->stAccountInfo.uiSlot;
	m_siMaxConstructibleFactoryNum		=	siMaxConstructibleFactoryNum;

	ZeroMemory( m_szOwnerName, sizeof( m_szOwnerName ) );
	strncpy( m_szOwnerName, pPlayer->szID, ON_ID_LENGTH );

	

	return	TRUE;
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	������ ��ô�Ѵ�.
//	���� ����	:	2002-10-10 ���� 5:33:02 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
BOOL	SOVillage_IndustrialComplex::Reclamation( SOPlayer *pPlayer, SI32 siMaxFactoryNum )
{
	DBIKReclamation		DBReclamation;

	// �������� �˻��Ѵ�.
	if( IsOwner( pPlayer ) == FALSE )								return	FALSE;

	// �� ��� �������� ���� �� �ִ� �ִ� �ǹ� ���� �ʰ��ϴ��� �˻��Ѵ�.
	if( m_siMaxConstructibleFactoryNum >= siMaxFactoryNum )			return	FALSE;
	
	m_siMaxConstructibleFactoryNum++;


	 
	
	// DB�� �����Ѵ�.
	DBReclamation.uiVillageCode	=	pPlayer->stAccountInfo.pVillage->uiVillageCode;	
	DBReclamation.uiFarmID		=	UI08( m_siIndustrialComplexID );
	DBReclamation.moCapital     =   ON_PLANT_EXPANDSLOT_COST * ON_CAPITAL_RATE_PLANT;
	DBReclamation.moPay         =   ON_PLANT_EXPANDSLOT_COST;
	DBReclamation.DBAccount     =   pPlayer->stAccountInfo.siDBAccount;
	DBReclamation.uiSlot        =   pPlayer->stAccountInfo.uiSlot;

		
	m_pDataBase->writeDBQuery( SO_DB_INFOKIND_FARM_EXPANDSLOT, &DBReclamation, sizeof (DBReclamation ) );

	return TRUE;

}


//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	�� ��������� �������� �˻��Ѵ�.
//	���� ����	:	2002-10-10 ���� 5:36:09 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
BOOL	SOVillage_IndustrialComplex::IsOwner( SOPlayer *pPlayer )
{
	if( pPlayer->stAccountInfo.siDBAccount == m_siOwnerDBAccount && pPlayer->stAccountInfo.uiSlot == m_uiOwnerDBSlot )		return	TRUE;

	return	FALSE;
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	������ �����Ѵ�.
//	���� ����	:	2002-10-10 ���� 5:36:09 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
BOOL	SOVillage_IndustrialComplex::ChangeOwner( SOPlayer *pPlayer )
{
	// ���κ�����.
	m_siOwnerDBAccount = pPlayer->stAccountInfo.siDBAccount;
	m_uiOwnerDBSlot = pPlayer->stAccountInfo.uiSlot;

	strcpy(m_szOwnerName,pPlayer->szID);

	// ���� ���ѳ�¥�� �ʱ�ȭ���ش�.
	m_siDayNotWorked = 0;

	return	TRUE;


}


//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	������ �Ǽ��Ѵ�.
//	���� ����	:	2002-10-10 ���� 8:55:05 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
BOOL	SOVillage_IndustrialComplex::ConstructFactory( SOPlayer *pPlayer, SI32 siFactoryID, CHAR cFactoryCode[ 4 ], MONEY mnPay, MONEY *pmnCapital )
{		

	// �Ǽ��Ϸ��� �ǹ��� �̹� �ǹ��� �Ǽ��Ǿ����� �˻��Ѵ�.
	if( m_Factory[ siFactoryID ].DidConstructFactory() == TRUE )			return	FALSE;

	return	m_Factory[ siFactoryID ].ConstructFactory( pPlayer, siFactoryID, cFactoryCode, mnPay, pmnCapital );

}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	������ �ı��Ѵ�.
//	���� ����	:	2002-10-15 ���� 11:13:30 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
BOOL	SOVillage_IndustrialComplex::DestroyFactory( SOPlayer *pPlayer, SI32 siFactoryID, MONEY *pmnCapital )
{
	SOVillage_IndustrialComplex_Factory		*pFactory;

	if( ( pFactory = GetFactoryConstructed( siFactoryID ) ) == NULL )		return	FALSE;

	pFactory->DestroyFactory( pPlayer, pmnCapital );

	return	TRUE;
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	���忡�� ���� �� �� �ִ��� �˻��Ѵ�.
//	���� ����	:	2002-10-15 ���� 10:41:23 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
BOOL	SOVillage_IndustrialComplex::CanWork( SOPlayer *pPlayer )
{
	SOVillage_IndustrialComplex_Factory		*pFactory;

	if( ( pFactory = GetEnteredFactory( pPlayer ) ) == NULL )						return	FALSE;

	return	CanWork( pFactory );
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	���� �� �� �ִ��� �˻��Ѵ�.
//	���� ����	:	2002-10-11 ���� 4:21:13 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
BOOL	SOVillage_IndustrialComplex::CanWork( SOVillage_IndustrialComplex_Factory *pFactory )
{
	UI16 uiProducingItemID;	
	
	if( pFactory->CanWork( ) == FALSE )
	{
		return	FALSE;
	}

	// ���忡�� ���� �Ͽ��� �� ����� �������� â�� ���� �� �ִ��� �˻��Ѵ�.
	if( ( pFactory->IsFactory() == TRUE ) && ( pFactory->IsUpgrading() == FALSE ) )
	{
		uiProducingItemID	=	pFactory->GetProducingItemID();

		SI32 siStorageIndex = -1;
		SI32 siStoragePos = -1;

		//�Ʒ� �Լ��� ���� ��� ������ �����ÿ� �� slot �� �ִ� ���� üũ �ϴ� ����
		//�� slot �� ���ٸ�, 
		if( CanAddItem( uiProducingItemID , pFactory->GetProducingItemNumber(), &siStorageIndex, &siStoragePos ) == FALSE )
		{
			return	FALSE;
		}

		// â�� ���� �ϱ� ���� ������ �ִ��� �˻��Ѵ�.
		if( IsMaterialToolEnoughToProduce( uiProducingItemID ) == FALSE )
		{
			return	FALSE;
		}
	}
	
	return	TRUE;
}


//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	��� ������ ���忡 ����.
//	���� ����	:	2002-10-11 ���� 4:37:56 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
BOOL	SOVillage_IndustrialComplex::EnterFactory( SOPlayer *pPlayer, SI32 siFactoryID )
{	
	SOVillage_IndustrialComplex_Factory		*pFactory;	

	// �̹� �� ������ �ִٸ� �� ���� ����.
	if( ( GetEnteredFactory( pPlayer ) ) != NULL )									return	FALSE;

	if( ( pFactory = GetFactoryConstructed( siFactoryID ) ) == NULL )				return	FALSE;

	return	pFactory->EnterPlayer( pPlayer, IsOwner( pPlayer ) );
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	��� ������ ���忡�� ������.
//	���� ����	:	2002-10-11 ���� 4:50:03 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
BOOL	SOVillage_IndustrialComplex::LeaveFactory( SOPlayer *pPlayer )
{
	SOVillage_IndustrialComplex_Factory		*pFactory;

	// �̹� �� ������ ���ٸ� ���� �� ����.
	if( ( pFactory = GetEnteredFactory( pPlayer ) ) == NULL )							return	FALSE;

	return	pFactory->LeavePlayer( pPlayer );
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	���� �����Ѵ�.
//	���� ����	:	2002-10-11 ���� 7:25:53 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
BOOL	SOVillage_IndustrialComplex::BeginWork( SOPlayer *pPlayer )
{
	SOVillage_IndustrialComplex_Factory		*pFactory;

	// �÷��̾ �� ������ ���´�.
	if( ( pFactory = GetEnteredFactory( pPlayer ) ) == NULL )
	{
		return	FALSE;
	}

	if( CanWork( pFactory ) == FALSE )
	{
		return	FALSE;
	}

	return	pFactory->BeginWork( pPlayer );	
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	������ �ӱ��� �����Ѵ�.
//	���� ����	:	2002-10-11 ���� 8:59:44 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
BOOL	SOVillage_IndustrialComplex::ChangeFactoryPay( SOPlayer *pPlayer, MONEY mnPay, MONEY *pmnCapital )
{
	SOVillage_IndustrialComplex_Factory		*pFactory;

	// �÷��̾ �� ������ ���´�.
	if( ( pFactory = GetEnteredFactory( pPlayer ) ) == NULL )							return	FALSE;

	return	pFactory->ChangeFactoryPay( pPlayer, mnPay, pmnCapital );	
}


//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	���� �۾����� ���忡�� �۾����ΰ��� ����Ѵ�.
//	���� ����	:	2002-10-12 ���� 2:45:13 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
BOOL	SOVillage_IndustrialComplex::CancelWork( SOPlayer *pPlayer, SI32 siFactoryID, MONEY *pmnCapital )
{
	SOVillage_IndustrialComplex_Factory		*pFactory;

	// �÷��̾ �� ������ ���´�.
	if( ( pFactory = GetEnteredFactory( pPlayer ) ) == NULL )							return	FALSE;

	return	pFactory->CancelWork( pPlayer, siFactoryID, pmnCapital );		
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	������ ���׷��̵� ��Ų��.
//	���� ����	:	2002-10-12 ���� 5:50:21 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
BOOL	SOVillage_IndustrialComplex::UpgradeFactory( SOPlayer *pPlayer, SI32 siFactoryID, MONEY mnPay, MONEY *pmnCapital )
{
	SOVillage_IndustrialComplex_Factory		*pFactory;
	
	if( ( pFactory = GetFactoryConstructed( siFactoryID ) ) == NULL )					return	FALSE;

	return	pFactory->UpgradeFactory( pPlayer, mnPay, pmnCapital );			
}


//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	�������� �����Ѵ�.
//	���� ����	:	2002-10-12 ���� 7:16:47 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
DWORD	SOVillage_IndustrialComplex::ProduceItem( SOPlayer *pPlayer, SI32 siFactoryID, UI16 uiItemID, UI32 uiItemQuantity, MONEY mnPay, CHAR cNation, MONEY *pmnCapital )
{
	SOVillage_IndustrialComplex_Factory		*pFactory;		
	DWORD									dwRet;

	if( ( pFactory = GetFactoryConstructed( siFactoryID ) ) == NULL )					
	{
		return	ON_RET_NO;
	}

	if( IsMaterialItemEnoughToProduce( uiItemID, uiItemQuantity ) == FALSE )			
	{
		return	ON_RET_FARM_MATERIAL;
	}

	if( IsMaterialToolEnoughToProduce( uiItemID ) == FALSE )
	{
		return	ON_RET_FARM_TOOL;
	}

	dwRet	=	pFactory->ProduceItem( pPlayer, uiItemID, uiItemQuantity, mnPay, cNation,  pmnCapital );

	if( dwRet == ON_RET_OK )
	{
		// �������� �Ҹ��Ѵ�.

		// �������� �����ϴµ� ���Ǵ� ��� �������� �����Ѵ�.
		DeleteMaterialItem( uiItemID, uiItemQuantity );

		// �������� �����ϴµ� ����ϴ� ������ �������� �ı� ��Ų��.
		DestroyToolItemRandomly( uiItemID );

	}

	return	dwRet;			
}


//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	â�� �ȿ� �������� �ִ´�.
//	���� ����	:	2002-10-14 ���� 11:24:21 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
BOOL	SOVillage_IndustrialComplex::AddItem( SOPlayer *pPlayer, UI16 uiItemID, UI16 uiItemQuantity ,BOOL bSendToDB)
{
	SOVillage_IndustrialComplex_Factory		*pFactory;
	
	if( ( pFactory = GetEnteredFactory( pPlayer ) ) == NULL )							return	FALSE;

	return	pFactory->AddItem( pPlayer, uiItemID, uiItemQuantity ,bSendToDB);			
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	â��ȿ��� �������� �����Ѵ�.
//	���� ����	:	2002-10-14 ���� 11:25:01 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
BOOL	SOVillage_IndustrialComplex::DeleteItem( SOPlayer *pPlayer, UI16 uiItemID, UI16 uiItemQuantity ,BOOL bSendToDB)
{
	SOVillage_IndustrialComplex_Factory		*pFactory;
	
	if( ( pFactory = GetEnteredFactory( pPlayer ) ) == NULL )							return	FALSE;

	return	pFactory->DeleteItem( pPlayer, uiItemID, uiItemQuantity ,bSendToDB);			
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	�ƹ� â�� �ȿ� �������� �ִ´�.
//	���� ����	:	2002-10-14 ���� 2:12:57 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
BOOL	SOVillage_IndustrialComplex::AddItem( UI16 uiItemID, UI16 uiItemQuantity ,BOOL bSendToDB)
{
	SI32	i;

	for( i = 0; i < ON_MAX_FACTORY_NUM; i++ )
	{			
		if( ( m_Factory[ i ].DidConstructFactory() == TRUE ) && ( m_Factory[ i ].IsStorage() == TRUE ) )
		{				
			
			if( m_Factory[ i ].AddItem( uiItemID, uiItemQuantity ,bSendToDB) == TRUE )	return	TRUE;
		}
	}		

	return	FALSE;
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	â�� �ȿ� �������� ���� �� �ִ��� �˻��Ѵ�.
//	���� ����	:	2002-12-17 ���� 9:49:37 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������

BOOL	SOVillage_IndustrialComplex::CanAddItem( UI16 uiItemID ,UI16 uiQuantity,SI32 *pStorageID,SI32 *pStoragePos)
{	
	SI32									i;
	SOVillage_IndustrialComplex_Factory		*pFactory;

	*pStorageID = -1;
	*pStoragePos = -1;

	for( i = 0; i < ON_MAX_FACTORY_NUM; i++ )
	{
		if( ( pFactory = GetFactoryConstructed( i ) ) == NULL )	continue;

		if( pFactory->CanAddItem( uiItemID ,uiQuantity , pStoragePos) == TRUE )	
		{
			*pStorageID = i;
			return	TRUE;
		}
	}

	return	FALSE;
}


//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	���������� �ʴ� ��� ������ ���ش�.
//	���� ����	:	2002-10-15 ���� 2:26:16 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
VOID	SOVillage_IndustrialComplex::DestroyIndustrialComplexNotWork( MONEY *pmnIndustrialComplexPrice, MONEY mnIndustrialComplexCost )
{	
	DBIKBuyFarmByForceInfo			SellFarmInfo;	

	
	// DB�� ������Ʈ�Ѵ�.
	SellFarmInfo.uiVillageCode	=	m_uiVillageCode;	
	SellFarmInfo.uiFarmID		=	m_siIndustrialComplexID;
	SellFarmInfo.mnPrice		=	mnIndustrialComplexCost;
	SellFarmInfo.siAccount      =   m_siOwnerDBAccount;
	SellFarmInfo.uiSlot         =   m_uiOwnerDBSlot;

	if( pmnIndustrialComplexPrice != NULL )		
	{
		*pmnIndustrialComplexPrice		=	mnIndustrialComplexCost;
		//SellFarmInfo.bSendMsgToPlayer	=	TRUE;	
	}
	else
	{
		//SellFarmInfo.bSendMsgToPlayer	=	FALSE;	
	}
	
	m_pDataBase->writeDBQuery( SO_DB_INFOKIND_FARM_SELL_BYFORCE, &SellFarmInfo, sizeof( SellFarmInfo ) );

	// ������ ������Ʈ�Ѵ�.
	InitIndustrialComplex();
	

	
	
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	�Ϸ簡 ������.
//	���� ����	:	2002-10-15 ���� 2:21:02 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
VOID	SOVillage_IndustrialComplex::DayChanged( SOPlayer *pPlayer, MONEY mnIndustrialComplexCost )
{	
	DBIKBuyFarmByForceInfo		stUpdateWorkDay;
	MONEY						mnIndustrialComplexPrice;

	m_siDayNotWorked++;	

	if( m_siDayNotWorked > ON_FORCE_TAKE_DAY)
	{
		// �������� �ƹ��� ���� ���� �ʴ� ��� ������ ���ش�.

		if( pPlayer != NULL)		
		{
			DestroyIndustrialComplexNotWork( &mnIndustrialComplexPrice, mnIndustrialComplexCost );
			//pPlayer->GiveBonusMoney( mnIndustrialComplexPrice, ON_RET_CHNGEMONEY_PRODUCTFACILITY, FALSE );
		}
		else
		{
			DestroyIndustrialComplexNotWork( NULL, mnIndustrialComplexCost );	

			
		}		
	}
	else if( ( m_siDayNotWorked % 5) == 0)
	{
		stUpdateWorkDay.uiVillageCode	=	m_uiVillageCode;
		stUpdateWorkDay.uiFarmID		=	m_siIndustrialComplexID;
		stUpdateWorkDay.siWorkDay		=	m_siDayNotWorked;

		m_pDataBase->writeDBQuery( SO_DB_INFOKIND_FARM_SETREMAINDAY, LPVOID( &stUpdateWorkDay ), sizeof( DBIKBuyFarmByForceInfo ) );		
	}
}


// 2003/07/03 18:30 ������ ����
// NPC�� ���� �ϴ� ���� �Ϲ� ������ ���������� ���� �� �� �ִ� ȯ�濡���� ���� �ϵ��� �Ѵ�.
VOID	SOVillage_IndustrialComplex::DoNPCWork( UI32 uiNPCLabor )
{	
	SOVillage_IndustrialComplex_Factory		*pFactory = NULL, *pTempFactory;

	MONEY moMaxPay = 0;
	
	// �ְ��ӱ��� ã�Ƽ�
	for( int i = 0; i < ON_MAX_FACTORY_NUM; i++ )
	{
		if( ( pTempFactory = GetFactoryConstructed( i ) ) == NULL )		continue;
		if( CanWork( pTempFactory ) == FALSE )							continue;

		MONEY pay = pTempFactory->GetPay();
			
		if (pay > moMaxPay)
		{
			moMaxPay	=	pay;
			pFactory	=	pTempFactory;
		}		

	}

	// ���Ұ��� ������
	if( pFactory != NULL )
	{
		// �������ش�. 
		pFactory->FinishWorkByNPC(this,uiNPCLabor);

		m_siDayNotWorked = 0;

	}	
	
}

VOID  SOVillage_IndustrialComplex::GetTradeInfo(OnIndustrialComplexTradeInfo *pTradeInfo)
{
	if (pTradeInfo)
	{
		pTradeInfo->uiVillageCode = this->m_uiVillageCode;
		pTradeInfo->uiIndustrialComplexID = this->m_siIndustrialComplexID;

		for (int i = 0; i < ON_MAX_FACTORY_NUM; i++)
		{
			memcpy(pTradeInfo->cFactoryKind[i],m_Factory[i].GetFactoryKind(),4);
		}
		
	}
}

