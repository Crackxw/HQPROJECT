#include "SOMain.h"
#include "SOWorld.h"
#include "SODatabase.h"
#include "OnlineItem.h"
#include "OnlineProductStructure.h"
#include "SOVillage_IndustrialComplexsStructure.h"

CItem			*SOVillage_IndustrialComplexs::m_pItem;
SODatabase		*SOVillage_IndustrialComplexs::m_pDataBase;
OnlineBuilding	*SOVillage_IndustrialComplexs::m_pBuilding;

//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	���� ������ �ʱ�ȭ �Ѵ�.
//	���� ����	:	2002-10-04 ���� 3:08:31 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
VOID	SOVillage_IndustrialComplexs::InitStaticVariable( CItem *pItem, SODatabase *pDataBase, OnlineBuilding *pBuilding )
{
	m_pItem			=	pItem;
	m_pDataBase		=	pDataBase;
	m_pBuilding		=	pBuilding;

	SOVillage_IndustrialComplex::InitStaticVariable( pItem, pDataBase, pBuilding );
	SOVillage_IndustrialComplex_Factory::InitStaticVariable( pItem, pDataBase, pBuilding );
}


//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	������.
//	���� ����	:	2002-10-04 ���� 3:16:17 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
SOVillage_IndustrialComplexs::SOVillage_IndustrialComplexs()
{

}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	�Ҹ���.
//	���� ����	:	2002-10-04 ���� 3:16:31 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
SOVillage_IndustrialComplexs::~SOVillage_IndustrialComplexs()
{

}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	�ʱ�ȭ.
//	���� ����	:	2002-10-11 ���� 12:01:18 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
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

//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	�޸� ����.
//	���� ����	:	2002-10-11 ���� 12:01:23 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
VOID	SOVillage_IndustrialComplexs::Free()
{
	SI32	i;

	for( i = 0; i < ON_MAX_INDUSTRIAL_COMPLEX_NUM; i++ )
	{
		m_IndustrialComplex[ i ].Free();
	}

	SOVillage_BaseStructure::Free();
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	�÷��̾ �� �ǹ��� ���Դ�.
//	���� ����	:	2002-10-04 ���� 8:57:46 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
BOOL	SOVillage_IndustrialComplexs::EnterPlayer( SOPlayer *pPlayer, SI32 siIndustrialComplexID )
{ 
	SOVillage_IndustrialComplex		*pIndustrialComplex;			

	if( SOVillage_BaseStructure::EnterPlayer( pPlayer )	== FALSE )							return	FALSE;		

	if( ( pIndustrialComplex = GetIndustrialComplex( siIndustrialComplexID ) ) == NULL )	return	FALSE;

	// ��� ������ ����.
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
//	����		:	�÷��̾ �� �ǹ����� ������.
//	���� ����	:	2002-10-11 ���� 1:25:51 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
BOOL	SOVillage_IndustrialComplexs::LeavePlayer( SOPlayer *pPlayer )
{

	SOVillage_IndustrialComplex		*pIndustrialComplex;

	// ��� ������ �� �ִٸ� ������������� ���� �� �ֵ��� ���ش�.
	if( ( pIndustrialComplex = GetEnteredIndustrialComplex( pPlayer ) ) != NULL )
	{
		pIndustrialComplex->LeavePlayer( pPlayer );
	}

	return	SOVillage_BaseStructure::LeavePlayer( pPlayer );
	
}	



//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	��� �������� ������ �÷��̾�� �����ش�.
//	���� ����	:	2002-10-04 ���� 3:16:45 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
BOOL	SOVillage_IndustrialComplexs::SendIndustrialComplexList( SOPlayer *pPlayer )
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
stFarmGroup*	SOVillage_IndustrialComplexs::GetFactories()
{
//	return	m_pFarmGroup;

	return	NULL;
}

/*
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	�����ϰ� ���� �ʴ� �ǹ��� ���ش�.
//	���� ����	:	2002-10-04 ���� 7:15:30 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
VOID	SOVillage_IndustrialComplexs::DismantleFactoryNotWork( SOWorld *pWorld )
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
*/
/*
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	�÷��̾ ������ ����ǹ��� ���´�.
//	���� ����	:	2002-10-04 ���� 7:27:48 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
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
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	���� �ü��� �����Ѵ�.
//	���� ����	:	2002-10-04 ���� 8:29:54 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
BOOL	SOVillage_IndustrialComplexs::BuyFactory( SOPlayer *pPlayer, UI08 uiPlantID, UI08 uiFarmID, MONEY *pmnCost )
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
/*

//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	�ǹ��� ������ �ȷȴ�.
//	���� ����	:	2002-10-04 ���� 8:46:43 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
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

	// ������ ������Ʈ �Ѵ�.
	pFarm->Init();	

	return TRUE;	
}
*/

//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	���Ϸ��� �ϴ� ���� ��ȿ�� ������ �˻��Ѵ�.
//	���� ����	:	2002-10-07 ���� 2:23:08 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
BOOL	SOVillage_IndustrialComplexs::CanWork( SOPlayer *pPlayer )
{		
	SOVillage_IndustrialComplex		*pIndustrialComplex;

	// ��� ������ �� �ִٸ� ������������� ���� �� �ֵ��� ���ش�.
	if( ( pIndustrialComplex = GetEnteredIndustrialComplex( pPlayer ) ) == NULL )		return	FALSE;
		
	return	pIndustrialComplex->CanWork( pPlayer );
	

	/*
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
	*/
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	��ȿ�� ��� ������ ID���� �˾ƿ´�.
//	���� ����	:	2002-10-07 ���� 3:48:55 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
BOOL	SOVillage_IndustrialComplexs::IsValidIndustrialComplexID( SI32 siIndustrialComplexID )
{
	if( siIndustrialComplexID >= 0 && siIndustrialComplexID < ON_MAX_INDUSTRIAL_COMPLEX_NUM )		return	TRUE;

	return	FALSE;
}


//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	��� ������ ���´�.
//	���� ����	:	2002-10-07 ���� 3:03:36 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
SOVillage_IndustrialComplex*	SOVillage_IndustrialComplexs::GetIndustrialComplex( SI32 siIndustrialComplexID )
{
	if( IsValidIndustrialComplexID( siIndustrialComplexID ) == FALSE )								return	NULL;

	return	&m_IndustrialComplex[ siIndustrialComplexID ];	
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	�÷��̾ �����ϰ� �ִ� ��� ������ ���´�.
//	���� ����	:	2002-10-15 ���� 2:07:30 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
SOVillage_IndustrialComplex*	SOVillage_IndustrialComplexs::GetIndustrialComplexOwned( SI32 siIndustrialComplexID )
{
	SOVillage_IndustrialComplex		*pIndustrialComplex;

	if( ( pIndustrialComplex = GetIndustrialComplex( siIndustrialComplexID ) ) == NULL )		return	NULL;

	if( pIndustrialComplex->HasOwner() == FALSE )												return	NULL;
	
	return	pIndustrialComplex;
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	�ش� �÷��̾ �����ϰ� �ִ� ��� ������ ���´�.
//	���� ����	:	2002-10-10 ���� 8:53:38 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
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

//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	�ش� �÷��̾ ���ִ� ��� ������ ���´�.
//	���� ����	:	2002-10-11 ���� 1:27:13 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
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

//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	�ش� �÷��̾ ���ִ� ��� ������ ���´�.
//	���� ����	:	2002-10-11 ���� 1:27:13 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
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


//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	��� ��� ������ ������ ���´�.
//	���� ����	:	2002-10-07 ���� 7:20:48 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
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

//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	DB���� ���� ������ ��� ������ �ʱ�ȭ �Ѵ�.
//	���� ����	:	2002-10-07 ���� 9:24:05 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
BOOL	SOVillage_IndustrialComplexs::InitIndustrialComplexFromDB( SI32 siIndustrialComplexID, 
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


//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	DB���� ���� ������ ������ �ʱ�ȭ �Ѵ�.
//	���� ����	:	2002-10-08 ���� 10:35:58 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
BOOL	SOVillage_IndustrialComplexs::InitFactoryFromDB( SI32 siIndustrialComplexID, SI32 siFactoryID, UI08 uiLevel, MONEY mnPay, UI32 uiAmountOfWork, CHAR cFactoryKind[ 4 ],
										UI16 uiProducingItemID, UI32 uiProducedItemQuantity, UI32 uiMaxProduceItemQuantity)
{
	SOVillage_IndustrialComplex		*pIndustrialComplex;

	if( ( pIndustrialComplex = GetIndustrialComplex( siIndustrialComplexID ) ) == NULL )	return	FALSE;

	return	pIndustrialComplex->InitFactoryFromDB( siFactoryID, uiLevel, mnPay, uiAmountOfWork, cFactoryKind, 
													uiProducingItemID, uiProducedItemQuantity, uiMaxProduceItemQuantity );		
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	������ ó���Ѵ�.
//	���� ����	:	2002-10-08 ���� 11:41:43 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
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


//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	���� �����ƴ�.
//	���� ����	:	2002-10-08 ���� 11:57:56 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
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
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	��� ������ �ִ� ������ ������ �÷��̾�� �����ش�.
//	���� ����	:	2002-10-08 ���� 2:17:55 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
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


//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	��� ������ �ִ� â���� ������ �÷��̾�� �����ش�.
//	���� ����	:	2002-10-08 ���� 3:16:13 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
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
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	��� ������ �����Ѵ�.
//	���� ����	:	2002-10-08 ���� 8:34:06 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
BOOL	SOVillage_IndustrialComplexs::BuyIndustrialComplex( SOPlayer *pPlayer, SI32 siIndustrialComplexID )
{
	SOVillage_IndustrialComplex		*pIndustrialComplex;	

	if( ( pIndustrialComplex = GetIndustrialComplex( siIndustrialComplexID ) ) == NULL )	return	FALSE;	

	if( m_pIndustrialComplexInfo->siReqCredit > pPlayer->GetTradeGrade() )			return	FALSE;		// ���� �ü����� �����ϱ����� �ſ뵵�� �Ǵ��� �˻��Ѵ�. 		
	if( m_pIndustrialComplexInfo->uiCost > pPlayer->siMoney)						return	FALSE;		// ���� �ü����� �����ϱ����� ���� �Ǵ��� �˻��Ѵ�.				
	
	if( pIndustrialComplex->BuyIndustrialComplex( pPlayer, m_pIndustrialComplexInfo->uiInitSlot ) == FALSE )		return	FALSE;

	// ����� �����Ѵ�.
	pPlayer->DecreaseMoney( m_pIndustrialComplexInfo->uiCost , FALSE);

	





	// DB�� �����Ѵ�.

	DBIKBuyFarmInfo				BuyFarmInfo;	
	BuyFarmInfo.siAccount		=	pPlayer->stAccountInfo.siDBAccount;
	BuyFarmInfo.uiSlot			=	pPlayer->stAccountInfo.uiSlot;
	BuyFarmInfo.uiVillageCode	=	pPlayer->stAccountInfo.pVillage->uiVillageCode;
	BuyFarmInfo.uiFarmID		=	siIndustrialComplexID;
	BuyFarmInfo.moMoney			=   m_pIndustrialComplexInfo->uiCost;


	
	
	m_pDataBase->writeDBQuery( SO_DB_INFOKIND_FARM_BUY, &BuyFarmInfo, sizeof( BuyFarmInfo ) );	

	// ���������� ��� ������ �����ߴ�.
	
	// ���������� �����ߴٰ� �˷��ش�.
	pPlayer->SendFieldMsg( ON_RESPONSE_FARM_BUY, SO_SFM_ONLYME, LPVOID( ON_RET_OK ) );

	return	TRUE;		
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	������ ��ô�Ѵ�.
//	���� ����	:	2002-10-10 ���� 5:31:43 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
BOOL	SOVillage_IndustrialComplexs::Reclamation( SOPlayer *pPlayer, SI32 siIndustrialComplexID, MONEY *pmnCapital )
{
	SOVillage_IndustrialComplex		*pIndustrialComplex;	

	if( ( pIndustrialComplex = GetIndustrialComplex( siIndustrialComplexID ) ) == NULL )				return	FALSE;	

	if( pIndustrialComplex->Reclamation( pPlayer, m_pIndustrialComplexInfo->uiMaxSlot ) == FALSE )		return	FALSE;

	// ���������� ������ ��ô�ߴ�.
	// ����� �����Ѵ�.
	pPlayer->DecreaseMoney( ON_PLANT_EXPANDSLOT_COST ,FALSE);

	// ������ ������ ���´�.
	*pmnCapital	=	MONEY( ON_PLANT_EXPANDSLOT_COST * ON_CAPITAL_RATE_PLANT );

	// ���������� ��ô�ߴٰ� �˷��ش�.
	pPlayer->SendFieldMsg( ON_RESPONSE_FARM_EXPANDSLOT, SO_SFM_ONLYME, LPVOID( ON_RET_OK ) );

	return	TRUE;
}


//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	������ �Ǽ��Ѵ�.
//	���� ����	:	2002-10-10 ���� 8:51:26 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
BOOL	SOVillage_IndustrialComplexs::ConstructFactory( SOPlayer *pPlayer, SI32 siFactoryID, CHAR cFactoryCode[ 4 ], MONEY mnPay, MONEY *pmnCapital )
{


	SOVillage_IndustrialComplex		*pIndustrialComplex;	

	if( ( pIndustrialComplex = GetOwnIndustrialComplex( pPlayer ) ) == NULL )										return	FALSE;		
	
	if( pIndustrialComplex->ConstructFactory( pPlayer, siFactoryID, cFactoryCode, mnPay, pmnCapital ) == FALSE )	return	FALSE;

	// ���������� ������ �Ǽ��ߴ�.
	
		
	// ���������� ������ �Ǽ��ߴٰ� �˷��ش�.
	pPlayer->SendFieldMsg( ON_RESPONSE_FARM_BUILDSTRUCT, SO_SFM_ONLYME, LPVOID( ON_RET_OK ) );

	return	TRUE;
}


//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	��� ������ ���忡 ����.
//	���� ����	:	2002-10-11 ���� 4:36:34 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
BOOL	SOVillage_IndustrialComplexs::EnterFactory( SOPlayer *pPlayer, SI32 siFactoryID )
{
	SOVillage_IndustrialComplex		*pIndustrialComplex;

	// �� ��� ������ ���� �ִ��� �˻��Ѵ�.
	if( ( pIndustrialComplex = GetEnteredIndustrialComplex( pPlayer ) ) == NULL )		return	FALSE;
	
	return	pIndustrialComplex->EnterFactory( pPlayer, siFactoryID );		
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	��� ������ ���忡�� ������.
//	���� ����	:	2002-10-11 ���� 4:48:28 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
BOOL	SOVillage_IndustrialComplexs::LeaveFactory( SOPlayer *pPlayer )
{
	SOVillage_IndustrialComplex		*pIndustrialComplex;

	// �� ��� ������ ���� �ִ��� �˻��Ѵ�.
	if( ( pIndustrialComplex = GetEnteredIndustrialComplex( pPlayer ) ) == NULL )		return	FALSE;
	
	return	pIndustrialComplex->LeaveFactory( pPlayer );				
}


//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	���� �����Ѵ�.
//	���� ����	:	2002-10-11 ���� 7:24:58 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
BOOL	SOVillage_IndustrialComplexs::BeginWork( SOPlayer *pPlayer )
{
	SOVillage_IndustrialComplex		*pIndustrialComplex;

	// �÷��̾ �� ��� ������ ���´�.
	if( ( pIndustrialComplex = GetEnteredIndustrialComplex( pPlayer ) ) == NULL )	
	{
		return	FALSE;
	}
	
	return	pIndustrialComplex->BeginWork( pPlayer );					
}


//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	������ �ӱ��� �����Ѵ�.
//	���� ����	:	2002-10-11 ���� 8:58:48 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
BOOL	SOVillage_IndustrialComplexs::ChangeFactoryPay( SOPlayer *pPlayer, MONEY mnPay, MONEY *pmnCapital )
{
	SOVillage_IndustrialComplex		*pIndustrialComplex;

	// �÷��̾ �� ��� ������ ���´�.
	if( ( pIndustrialComplex = GetEnteredIndustrialComplex( pPlayer ) ) == NULL )		return	FALSE;

	// ��� ������ �������� �˻��Ѵ�.
	if( pIndustrialComplex->IsOwner( pPlayer ) == FALSE )								return	FALSE;	

	return	pIndustrialComplex->ChangeFactoryPay( pPlayer, mnPay, pmnCapital );
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	���� �۾����� ���忡�� �۾����ΰ��� ����Ѵ�.
//	���� ����	:	2002-10-12 ���� 2:44:36 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
BOOL	SOVillage_IndustrialComplexs::CancelWork( SOPlayer *pPlayer, SI32 siFactoryID, MONEY *pmnCapital )
{
	SOVillage_IndustrialComplex		*pIndustrialComplex;

	// �÷��̾ �� ��� ������ ���´�.
	if( ( pIndustrialComplex = GetEnteredIndustrialComplex( pPlayer ) ) == NULL )		return	FALSE;

	// ��� ������ �������� �˻��Ѵ�.
	if( pIndustrialComplex->IsOwner( pPlayer ) == FALSE )								return	FALSE;	

	return	pIndustrialComplex->CancelWork( pPlayer, siFactoryID, pmnCapital );	
}


//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	������ ���׷��̵� ��Ų��.
//	���� ����	:	2002-10-12 ���� 5:49:40 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
BOOL	SOVillage_IndustrialComplexs::UpgradeFactory( SOPlayer *pPlayer, SI32 siFactoryID, MONEY mnPay, MONEY *pmnCapital )
{
	SOVillage_IndustrialComplex		*pIndustrialComplex;

	// �÷��̾ �� ��� ������ ���´�.
	if( ( pIndustrialComplex = GetEnteredIndustrialComplex( pPlayer ) ) == NULL )		return	FALSE;

	// ��� ������ �������� �˻��Ѵ�.
	if( pIndustrialComplex->IsOwner( pPlayer ) == FALSE )								return	FALSE;	

	return	pIndustrialComplex->UpgradeFactory( pPlayer, siFactoryID, mnPay, pmnCapital );		
}


//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	�������� �����Ѵ�.
//	���� ����	:	2002-10-12 ���� 7:15:39 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
DWORD	SOVillage_IndustrialComplexs::ProduceItem( SOPlayer *pPlayer, SI32 siFactoryID, UI16 uiItemID, UI32 uiItemQuantity, MONEY mnPay, CHAR cNation, MONEY *pmnCapital )
{
	SOVillage_IndustrialComplex		*pIndustrialComplex;

	// �÷��̾ �� ��� ������ ���´�.
	if( ( pIndustrialComplex = GetEnteredIndustrialComplex( pPlayer ) ) == NULL )		return	ON_RET_NO;

	// ��� ������ �������� �˻��Ѵ�.
	if( pIndustrialComplex->IsOwner( pPlayer ) == FALSE )								return	ON_RET_FARM_NOTOWNER;		

	return	pIndustrialComplex->ProduceItem( pPlayer, siFactoryID, uiItemID, uiItemQuantity, mnPay, cNation, pmnCapital );		
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	â�� �ȿ� �������� �ִ´�.
//	���� ����	:	2002-10-14 ���� 11:25:37 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
BOOL	SOVillage_IndustrialComplexs::AddItem( SOPlayer *pPlayer, UI16 uiItemID, UI16 uiItemQuantity ,BOOL bSendToDB)
{
	SOVillage_IndustrialComplex		*pIndustrialComplex;

	// �÷��̾ �� ��� ������ ���´�.
	if( ( pIndustrialComplex = GetEnteredIndustrialComplex( pPlayer ) ) == NULL )		return	FALSE;

	// ��� ������ �������� �˻��Ѵ�.
	if( pIndustrialComplex->IsOwner( pPlayer ) == FALSE )								return	FALSE;		

	return	pIndustrialComplex->AddItem( pPlayer, uiItemID, uiItemQuantity ,bSendToDB);		
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	â��ȿ��� �������� �����Ѵ�.
//	���� ����	:	2002-10-14 ���� 11:26:15 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
BOOL	SOVillage_IndustrialComplexs::DeleteItem( SOPlayer *pPlayer, UI16 uiItemID, UI16 uiItemQuantity ,BOOL bSendToDB)
{
	SOVillage_IndustrialComplex		*pIndustrialComplex;

	// �÷��̾ �� ��� ������ ���´�.
	if( ( pIndustrialComplex = GetEnteredIndustrialComplex( pPlayer ) ) == NULL )		return	FALSE;

	// ��� ������ �������� �˻��Ѵ�.
	if( pIndustrialComplex->IsOwner( pPlayer ) == FALSE )								return	FALSE;		

	return	pIndustrialComplex->DeleteItem( pPlayer, uiItemID, uiItemQuantity ,bSendToDB);		
}


//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	������ �ı��Ѵ�.
//	���� ����	:	2002-10-15 ���� 11:12:21 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
BOOL	SOVillage_IndustrialComplexs::DestroyFactory( SOPlayer *pPlayer, SI32 siFactoryID, MONEY *pmnCapital )
{
	SOVillage_IndustrialComplex		*pIndustrialComplex;

	// �÷��̾ �� ��� ������ ���´�.
	if( ( pIndustrialComplex = GetEnteredIndustrialComplex( pPlayer ) ) == NULL )		return	FALSE;

	// ��� ������ �������� �˻��Ѵ�.
	if( pIndustrialComplex->IsOwner( pPlayer ) == FALSE )								return	FALSE;		

	return	pIndustrialComplex->DestroyFactory( pPlayer, siFactoryID, pmnCapital );			
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	�Ϸ簡 ������.
//	���� ����	:	2002-10-15 ���� 2:03:18 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
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

	
	// �ְ��ӱ� ���
/*	for( i = 0; i < ON_MAX_INDUSTRIAL_COMPLEX_NUM; i++ )
	{
		if( ( pIndustrialComplex = GetIndustrialComplexOwned( i ) ) == NULL )		continue;		
	
		pIndustrialComplex->GetOwner( &siOwnerDBAccount, &uiOwnerDBSlot );

		pPlayer	=	pWorld->GetPlayerByDBAccount( siOwnerDBAccount, uiOwnerDBSlot );

		// �Ϸ簡 ���������� �۾��� ���ش�.
		pIndustrialComplex->DayChanged( pPlayer, m_pIndustrialComplexInfo->uiCost );

		// �����ӱ��� �����ִ°��� ���س��´� (�ʱ��ӱ��� 0���� �س��⶧���� �۾����� ���°��� ã���ʴ´�.)
		if (pIndustrialComplex->GetInfo(&IndustrialComplexInfo)) {
			
			if (IndustrialComplexInfo.m_moMaxPay > maxPay)
			{
				siMaxPayComplexIndex = i;
				maxPay = IndustrialComplexInfo.m_moMaxPay;
				
			}
		}

		
	}

	// �����Ѵ�.
	if (siMaxPayComplexIndex >= 0 && uiNPCLabor > 0)
	{
		pIndustrialComplex = GetIndustrialComplexOwned( siMaxPayComplexIndex );
		if (pIndustrialComplex != NULL)
		{
			pIndustrialComplex->DoNPCWork(uiNPCLabor);
		}

	}*/

	// �ְ��ӱ� ���� ���

	MONEY totalpay = 0;

	

	for( i = 0; i < ON_MAX_INDUSTRIAL_COMPLEX_NUM; i++ )
	{
		if( ( pIndustrialComplex = GetIndustrialComplexOwned( i ) ) == NULL )		{
			maxpays[i] = 0;
			continue;		
		}
	
		pIndustrialComplex->GetOwner( &siOwnerDBAccount, &uiOwnerDBSlot );

		pPlayer	=	pWorld->GetPlayerByDBAccount( siOwnerDBAccount, uiOwnerDBSlot );

		// �Ϸ簡 ���������� �۾��� ���ش�.
		pIndustrialComplex->DayChanged( pPlayer, m_pIndustrialComplexInfo->uiCost );

		// �ӱ��� ���Ѵ�.
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

	// ��հ��� �Ѵ� �ְ��ӱ��� ��.
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

	// �����Ѵ�.
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


