#include "SOWorld.h"
#include "SODatabase.h"
#include "OnlineItem.h"
#include "OnlineProductStructure.h"
#include "SOVillage_FactoryStructure.h"

CItem			*SOVillage_Factory::m_pItem;
SODatabase		*SOVillage_Factory::m_pDataBase;
OnlineBuilding	*SOVillage_Factory::m_pBuilding;

//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	���� ������ �ʱ�ȭ �Ѵ�.
//	���� ����	:	2002-10-04 ���� 3:08:31 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
VOID	SOVillage_Factory::InitStaticVariable( CItem *pItem, SODatabase *pDataBase, OnlineBuilding *pBuilding )
{
	m_pItem			=	pItem;
	m_pDataBase		=	pDataBase;
	m_pBuilding		=	pBuilding;

	SOVillage_IndustrialComplex_Factory::InitStaticVariable( pDataBase, pBuilding );
}


//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	������.
//	���� ����	:	2002-10-04 ���� 3:16:17 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
SOVillage_Factory::SOVillage_Factory()
{

}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	�Ҹ���.
//	���� ����	:	2002-10-04 ���� 3:16:31 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
SOVillage_Factory::~SOVillage_Factory()
{

}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	�ʱ�ȭ.
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
BOOL	SOVillage_Factory::Init( UI32 uiVillageID )
{
	m_uiVillageID		=	uiVillageID;

	return	TRUE;
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	�÷��̾ �� �ǹ��� ���Դ�.
//	���� ����	:	2002-10-04 ���� 8:57:46 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
BOOL	SOVillage_Factory::EnterPlayer( SOPlayer *pPlayer, SI32 siIndustrialComplexID )
{ 
	SOVillage_IndustrialComplex		*pIndustrialComplex;
	BOOL							bEnteredIndustrialComplex = FALSE; 
	BOOL							bSuccess = FALSE;

	if( ( pIndustrialComplex = GetIndustrialComplex( siIndustrialComplexID ) ) == NULL )	return	FALSE;

	// ��� ������ ����.
	if( pIndustrialComplex->EnterPlayer( pPlayer ) == FALSE )								return	FALSE;

	
	
	bEnteredIndustrialComplex	=	TRUE;

	
	bSuccess	=	TRUE;



	if( bSuccess == FALSE )
	{
		// ��� �������� ���� ���´�.
		if( bEnteredIndustrialComplex )		pIndustrialComplex->LeavePlayer( pPlayer );
	}

	return	bSuccess;

	/*
	stFarmData	*pFactoryData;

	if( uiPlantID > PLANT_CODE_NUM )										return	FALSE;			// ���� �ü��� ID�� ��ȿ���� �˻��Ѵ�.
	
	if(	( pFactoryData = m_pFarmGroup->GetFarm( uiFarmID ) ) == NULL )		return	FALSE;			// ������ ���� �ü����� ��ȿ���� �˻��Ѵ�.			

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
	// �۾��忡 ���Դ�.
	pPlayer->stAccountInfo.uiStatusInVillage	=	INVILLAGE_STATUS_FACTORY;

	pPlayer->stAccountInfo.pStructure	=	pFarm;

	pPlayer->SendFieldMsg(ON_RESPONSE_FARM_SLOTINFO, SO_SFM_ONLYME, LPVOID(ON_RET_FARM_OK), LPVOID(pFarm));
	*/

	return TRUE;
}


//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	��� �������� ������ �÷��̾�� �����ش�.
//	���� ����	:	2002-10-04 ���� 3:16:45 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
BOOL	SOVillage_Factory::SendIndustrialComplexList( SOPlayer *pPlayer )
{
	OnIndustrialComplexInfo		IndustrialComplexInfo[ ON_MAX_INDUSTRIAL_COMPLEX_NUM ]; 
	UI32						uiIndustrialComplexNum;
	DWORD						dwInfoLength;
	
	dwInfoLength	=	GetAllIndustrialComplexInfo( IndustrialComplexInfo, &uiIndustrialComplexNum );

	pPlayer->SendFieldMsg( ON_RESPONSE_FARM_INFO, SO_SFM_ONLYME, LPVOID( uiIndustrialComplexNum ), LPVOID( IndustrialComplexInfo ), LPVOID( dwInfoLength ) );

	return	TRUE;
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	�� ������ �ִ� ��� ���� �ü����� ���´�.
//	���� ����	:	2002-10-04 ���� 6:31:57 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
stFarmGroup*	SOVillage_Factory::GetFactories()
{
//	return	m_pFarmGroup;

	return	NULL;
}

/*
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	�����ϰ� ���� �ʴ� �ǹ��� ���ش�.
//	���� ����	:	2002-10-04 ���� 7:15:30 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
VOID	SOVillage_Factory::DismantleFactoryNotWork( SOWorld *pWorld )
{
	DBIKBuyFarmByForceInfo		stUpdateWorkDay;
	SI32						i;
	stFarmData					*pFactoryData;	
	MONEY						mnProductFacilityPrice;				// ���� �ü��� ��ü�ǰ� �޴� ��.
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


//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	�÷��̾ ������ ����ǹ��� ���´�.
//	���� ����	:	2002-10-04 ���� 7:27:48 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
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

//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	���� �ü��� �����Ѵ�.
//	���� ����	:	2002-10-04 ���� 8:29:54 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
BOOL	SOVillage_Factory::BuyFactory( SOPlayer *pPlayer, UI08 uiPlantID, UI08 uiFarmID, MONEY *pmnCost )
{
	DBIKBuyFarmInfo					BuyFarmInfo;	
	stFarmData						*pFarm = NULL;
	stFarmData						*pFactoryData;	

	if( ( pFarm = m_pFarmGroup->GetFarm( uiFarmID ) ) == NULL)			return	FALSE;
	
	if( pFarm->siOwnerDBAcnt != 0 )										return	FALSE;		// ���� �ü����� �����ְ� �ִ��� �˻��Ѵ�.		
	if( pFarm->pPlant->siReqCredit > pPlayer->GetTradeGrade() )			return	FALSE;		// ���� �ü����� �����ϱ����� �ſ뵵�� �Ǵ��� �˻��Ѵ�. 		
	if( pFarm->pPlant->uiCost > pPlayer->siMoney)						return	FALSE;		// ���� �ü����� �����ϱ����� ���� �Ǵ��� �˻��Ѵ�.		
	if( pPlayer->GetOwnFactory() !=	NULL )								return	FALSE;		// �̹� ���� �ü����� ������ �ִ��� �˻��Ѵ�.		
	
	BuyFarmInfo.siAccount		=	pPlayer->stAccountInfo.siDBAccount;
	BuyFarmInfo.uiSlot			=	pPlayer->stAccountInfo.uiSlot;
	BuyFarmInfo.uiVillageCode	=	m_uiVillageCode;
	BuyFarmInfo.uiFarmID		=	uiFarmID;
	BuyFarmInfo.uiPlantID		=	m_pFarmGroup->pPlant->DBIndex;	
	BuyFarmInfo.uiAvail			=	m_pFarmGroup->pPlant->uiInitSlot;		
	m_pDataBase->writeDBQuery( SO_DB_INFOKIND_FARM_BUY, &BuyFarmInfo, sizeof( BuyFarmInfo ) );

	// ������ ������Ʈ �Ѵ�.	
	pFactoryData			=	&m_pFarmGroup->Farm[ uiFarmID ];

	pFactoryData->Init();
	pFactoryData->siOwnerDBAcnt		=	pPlayer->stAccountInfo.siDBAccount;
	pFactoryData->uiOwnerSlot		=	pPlayer->stAccountInfo.uiSlot;

	// ���� �ü����� ������ �����Ѵ�.
	ZeroMemory( pFactoryData->FarmInfo.szOwner, sizeof( pFactoryData->FarmInfo.szOwner ) );
	strncpy( pFactoryData->FarmInfo.szOwner, pPlayer->szID, ON_ID_LENGTH );	

	// �÷��̾��� ���� ���ҽ�Ų��.
	*pmnCost				=	m_pFarmGroup->pPlant->uiCost;	

	return TRUE;	
}


//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	�ǹ��� ������ �ȷȴ�.
//	���� ����	:	2002-10-04 ���� 8:46:43 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
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

	// ������ ������Ʈ �Ѵ�.
	pFarm->Init();	

	return TRUE;	
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	���Ϸ��� �ϴ� ���� ��ȿ�� ������ �˻��Ѵ�.
//	���� ����	:	2002-10-07 ���� 2:23:08 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
BOOL	SOVillage_Factory::CanWork( UI08 uiIndustrialComplexID, UI08 uiFarmID )
{	
	stFarmData				*pIndustrialComplex;
	stFarmSlot				*pFarmSlot;	

	if( ( pIndustrialComplex = GetIndustrialComplex( uiIndustrialComplexID ) ) == NULL )		return	FALSE;		// ��� ������ ���´�.
	
	if( ( pFarmSlot	= pIndustrialComplex->GetFarmSlot( uiSlot ) ) == NULL )		return	FALSE;		// ���Ϸ��� �ϴ� ���� ��ȿ�� ������ �˻��Ѵ�.

	// ���� �ü� ������ ���� �Ҽ� �ִ� �غ� ������ �ִ��� �˻��Ѵ�.
	pFarmSlot->bWorkable	=	FALSE;

	if( pFarmSlot->bOnSchedule() == FALSE )														return	FALSE;		// ���Ϸ��� �ϴ� ���� �۾����� �����־� �۾��� �� �� �ִ��� �˻��Ѵ�.
	
	// â�� ���� �ϱ� ���� ������ �ִ��� �˻��Ѵ�.
	if( CheckStorageTool( pFarmGroup->pPlant->DBIndex, pPlayer->stAccountInfo.pStructure->nIndex, pFarmSlot->ProductItem ) == FALSE )
		return	FALSE;

	// �۾��� �� �� �ִ�.
	pFarmSlot->bWorkable	=	TRUE;

	return TRUE;	
}
*/
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	��ȿ�� ��� ������ ID���� �˾ƿ´�.
//	���� ����	:	2002-10-07 ���� 3:48:55 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
BOOL	SOVillage_Factory::IsValidIndustrialComplexID( SI32 siIndustrialComplexID )
{
	if( siIndustrialComplexID >= 0 && siIndustrialComplexID < ON_MAX_INDUSTRIAL_COMPLEX_NUM )		return	TRUE;

	return	FALSE;
}


//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	��� ������ ���´�.
//	���� ����	:	2002-10-07 ���� 3:03:36 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
SOVillage_IndustrialComplex*	SOVillage_Factory::GetIndustrialComplex( SI32 siIndustrialComplexID )
{
	if( IsValidIndustrialComplexID( siIndustrialComplexID ) == FALSE )								return	NULL;

	return	&m_IndustrialComplex[ siIndustrialComplexID ];	
}


//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	��� ��� ������ ������ ���´�.
//	���� ����	:	2002-10-07 ���� 7:20:48 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
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

//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	DB���� ���� ������ ��� ������ �ʱ�ȭ �Ѵ�.
//	���� ����	:	2002-10-07 ���� 9:24:05 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
BOOL	SOVillage_Factory::InitIndustrialComplexFromDB( SI32 siIndustrialComplexID, 
												SI32 siOwnerDBAccount, UI08 uiOwnerDBSlot, CHAR *pszOwnerName, 
												SI32 siMaxConstructibleFactoryNum, SI32 siDayNotWorked )
{
	SOVillage_IndustrialComplex		*pIndustrialComplex;

	if( ( pIndustrialComplex = GetIndustrialComplex( siIndustrialComplexID ) ) == NULL )	return	FALSE;
	
	pIndustrialComplex->InitFromDB( siOwnerDBAccount, uiOwnerDBSlot, pszOwnerName, siMaxConstructibleFactoryNum, siDayNotWorked );

	return	TRUE;
}


//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	DB���� ���� ������ â�� �ʱ�ȭ �Ѵ�.
//	���� ����	:	2002-10-07 ���� 9:24:30 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
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


//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	DB���� ���� ������ ������ �ʱ�ȭ �Ѵ�.
//	���� ����	:	2002-10-08 ���� 10:35:58 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
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

//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	������ ó���Ѵ�.
//	���� ����	:	2002-10-08 ���� 11:41:43 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
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


//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	���忡�� ���� �ߴ�.
//	���� ����	:	2002-10-08 ���� 11:57:56 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
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

//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	��� ������ �ִ� ������ ������ �÷��̾�� �����ش�.
//	���� ����	:	2002-10-08 ���� 2:17:55 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
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


//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	��� ������ �ִ� â���� ������ �÷��̾�� �����ش�.
//	���� ����	:	2002-10-08 ���� 3:16:13 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
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