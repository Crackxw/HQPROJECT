#include "SOMain.h"
#include "SODatabase.h"
#include "SOPlayer.h"
#include "SOVillage_IndustrialComplex_Factory.h"
#include "SOVillage_IndustrialComplex.h"

CItem			*SOVillage_IndustrialComplex::m_pItem;
SODatabase		*SOVillage_IndustrialComplex::m_pDataBase;
OnlineBuilding	*SOVillage_IndustrialComplex::m_pBuilding;

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	정적 변수를 초기화 한다.		
//	수정 일자	:	2002-10-08 오후 9:47:22 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
VOID	SOVillage_IndustrialComplex::InitStaticVariable( CItem *pItem, SODatabase *pDataBase, OnlineBuilding *pBuilding )
{
	m_pItem			=	pItem;
	m_pDataBase		=	pDataBase;
	m_pBuilding		=	pBuilding;
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	생성자.	
//	수정 일자	:	2002-10-07 오후 3:24:56 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
SOVillage_IndustrialComplex::SOVillage_IndustrialComplex()
{	
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	소멸자.	
//	수정 일자	:	2002-10-07 오후 3:27:52 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
SOVillage_IndustrialComplex::~SOVillage_IndustrialComplex()
{	
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	초기화.
//	수정 일자	:	2002-10-07 오후 3:30:22 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
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

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	산업 단지가 팔렸을 시에 해준다.
//	수정 일자	:	2002-10-15 오후 2:39:28 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
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

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	메모리 해제.
//	수정 일자	:	2002-10-11 오후 1:24:12 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
VOID	SOVillage_IndustrialComplex::Free()
{

	SI32	i;

	for( i = 0; i < ON_MAX_FACTORY_NUM; i++ )
	{
		m_Factory[ i ].Free();
	}

	m_PlayerMgr.Free();
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	공장을 얻어온다.
//	수정 일자	:	2002-10-07 오후 3:41:00 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
SOVillage_IndustrialComplex_Factory*	SOVillage_IndustrialComplex::GetFactory( SI32 siFactoryID )
{
	if( IsValidFactoryID( siFactoryID ) == FALSE )		return	NULL;	

	return &m_Factory[ siFactoryID ];
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	공장이 건설된 공장 객체를 얻어온다.
//	수정 일자	:	2002-10-14 오후 2:31:09 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
SOVillage_IndustrialComplex_Factory*	SOVillage_IndustrialComplex::GetFactoryConstructed( SI32 siFactoryID )
{
	SOVillage_IndustrialComplex_Factory		*pFactory;

	if( ( pFactory = GetFactory( siFactoryID ) ) == NULL )			return	NULL;

	if( pFactory->DidConstructFactory() == FALSE )					return	NULL;

	return	pFactory;
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	플레이어가 들어간 공장을 얻어온다.
//	수정 일자	:	2002-10-11 오후 4:29:01 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
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

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	플레이어가 들어간 공장을 얻어온다.
//	수정 일자	:	2002-10-11 오후 4:29:01 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
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



//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	유효한 공장 ID 인가?
//	수정 일자	:	2002-10-07 오후 3:41:33 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
BOOL	SOVillage_IndustrialComplex::IsValidFactoryID( SI32 siFactoryID )
{
	if( ( siFactoryID >= 0 ) && ( siFactoryID < ON_MAX_FACTORY_NUM ) )		return	TRUE;

	return	FALSE;
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	플레이어가 들어왔다.
//	수정 일자	:	2002-10-07 오후 4:03:27 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
BOOL	SOVillage_IndustrialComplex::EnterPlayer( SOPlayer *pPlayer )
{
	SI32				siFactoryNum;
	OnFarmSlotList		FactoryInfo[ ON_MAX_FACTORY_NUM ];	
	DWORD				dwInfoLength;
	
	dwInfoLength	=	GetInfo( FactoryInfo, &siFactoryNum );

	// 공장의 정보를 플레이어에게 보내준다.
	if( m_PlayerMgr.AddPlayer( pPlayer ) == TRUE)
	{
		pPlayer->SendFieldMsg( ON_RESPONSE_FARM_SLOTINFO, SO_SFM_ONLYME, LPVOID( siFactoryNum ), LPVOID( FactoryInfo ), LPVOID( dwInfoLength ), 
								LPVOID( m_siMaxConstructibleFactoryNum ) );
		return	TRUE;
	}
	else
		return	FALSE;
	
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	플레이어가 나갔다.
//	수정 일자	:	2002-10-07 오후 4:23:50 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
BOOL	SOVillage_IndustrialComplex::LeavePlayer( SOPlayer *pPlayer )
{
	// 현재 들어가 있는 공장에서도 내보낸다.
	LeaveFactory( pPlayer );
	
	return	m_PlayerMgr.RemovePlayer( pPlayer );
}


//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	산업 단지에 있는 공장들의 정보를 얻어온다.
//	수정 일자	:	2002-10-07 오후 4:50:01 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
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

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	산업 단지의 정보를 얻어온다.
//					2차 수정) 최대 임금이라고 하더라도 일을 할 수 환경이면(도구 부족) 최대 임금으로 인정하지 않는다.
//	수정 일자	:	2003-07-03 오후 06:31:00 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
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


//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	이 산업단지의 주인이 있는가.
//	수정 일자	:	2002-10-07 오후 7:44:47 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
BOOL	SOVillage_IndustrialComplex::HasOwner()
{
	if( m_siOwnerDBAccount != 0 )		return	TRUE;

	return	FALSE;	
}


//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	DB에서 얻어온 정보로 산업 단지를 초기화 한다.
//	수정 일자	:	2002-10-07 오후 8:23:51 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
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


//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	DB에서 얻어온 정보로 창고를 초기화 한다.
//	수정 일자	:	2002-10-07 오후 9:27:47 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
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


//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	DB에서 얻어온 정보로 공장을 초기화 한다.
//	수정 일자	:	2002-10-08 오후 4:49:25 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
BOOL	SOVillage_IndustrialComplex::InitFactoryFromDB( SI32 siFactoryID, UI08 uiLevel, MONEY mnPay, UI32 uiAmountOfWork, CHAR cFactoryKind[ 4 ],
									UI16 uiProducingItemID, UI32 uiProducedItemQuantity, UI32 uiMaxProduceItemQuantity )
{
	SOVillage_IndustrialComplex_Factory		*pFactory;

	if( ( pFactory = GetFactory( siFactoryID ) ) == NULL )	return	FALSE;

	return	pFactory->InitFactoryFromDB( uiLevel, mnPay, uiAmountOfWork, cFactoryKind, 
											uiProducingItemID, uiProducedItemQuantity, uiMaxProduceItemQuantity );
}


//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	공장을 처분한다.
//	수정 일자	:	2002-10-08 오전 11:38:58 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
BOOL	SOVillage_IndustrialComplex::SellFactory( SOPlayer *pPlayer, SI32 siFactoryID )
{
	SOVillage_IndustrialComplex_Factory		*pFactory;

	if( ( pFactory = GetFactoryConstructed( siFactoryID ) ) == NULL )	return	FALSE;	

	return	pFactory->SellFactory( pPlayer );
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	이 산업단지의 주인을 얻어온다.
//	수정 일자	:	2002-10-08 오전 11:49:18 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
BOOL	SOVillage_IndustrialComplex::GetOwner( SI32 *psiOwnerDBAccount, UI08 *puiOwnerDBSlot )
{
	if( HasOwner() == FALSE )		return	FALSE;

	*psiOwnerDBAccount	=	m_siOwnerDBAccount;
	*puiOwnerDBSlot		=	UI08( m_uiOwnerDBSlot );
	
	return	TRUE;
}


//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	플레이어가 이 산업 단지안에 들어와 있는지 검사한다.
//	수정 일자	:	2002-10-08 오후 12:25:29 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
BOOL	SOVillage_IndustrialComplex::IsPlayerIn( SOPlayer *pPlayer )
{
	return	m_PlayerMgr.IsAddedPlayer( pPlayer );
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	일을 끝마쳤다.
//	수정 일자	:	2002-10-08 오후 12:28:51 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
BOOL	SOVillage_IndustrialComplex::FinishWork( SOPlayer *pPlayer, OnReqFarmWorkMsg *pMsg )
{
	SOVillage_IndustrialComplex_Factory		*pFactory;
	UI16									uiProducedItemID = 0;
	UI16									uiProducedItemQuantity = 0;
	
	if( ( pFactory = GetFactoryPlayerIn( pPlayer ) ) == NULL )																return	FALSE;

	if( pFactory->FinishWork(pPlayer, this, pMsg) == FALSE )		return	FALSE;
	
	// 생산되어진 아이템이 있는지 검사한다.
	/*if( ( uiProducedItemID != 0 ) && ( uiProducedItemQuantity != 0 ) )
	{
		// 아무 창고 안에 아이템을 넣는다.
		// 실패해도 어쩔수가 없다.
		AddItem( uiProducedItemID , uiProducedItemQuantity );
	}*/

	m_siDayNotWorked	=	0;

	return	TRUE;
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	플레이어가 들어가 있는 공장을 얻어온다.
//	수정 일자	:	2002-10-08 오후 12:31:22 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
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

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	아이템을 생산하기 위하여 창고에 충분한 아이템 재료가 있는지 검사한다.
//	수정 일자	:	2002-10-12 오후 8:10:40 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
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
		// 재료 아이템의 ID가 0일때까지만 수행한다.
		if( ( uiMaterialItemID = pstMakeitem->m_uiItemID[ i ] ) == 0 )		break;

		uiMaterialItemQuantity	= uiItemQuantity / pstMakeitem->m_siProduction * pstMakeitem->m_siItemNum[ i ];
		if( ( uiItemQuantity % pstMakeitem->m_siProduction ) != 0 )		uiMaterialItemQuantity	+=	pstMakeitem->m_siItemNum[ i ];

		for( j = 0; j < ON_MAX_FACTORY_NUM; j++ )
		{			
			if( ( m_Factory[ j ].DidConstructFactory() == TRUE ) && ( m_Factory[ j ].IsStorage() == TRUE ) )
			{				
				// 창고에서 현재 재료 아이템이 있는지 검사한다.
				if( m_Factory[ j ].HasItem( uiMaterialItemID, uiMaterialItemQuantity ) == TRUE )	break;				
			}
		}		

		// 창고에 아이템이 없다.
		if( j == ON_MAX_FACTORY_NUM )
		{
			return	FALSE;
		}
	}
	
	return	TRUE;
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	아이템을 생산하기 위하여 창고에 충분한 아이템 도구가 있는지 검사한다.
//	수정 일자	:	2002-10-12 오후 8:36:07 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
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
		// 재료 아이템의 ID가 0일때까지만 수행한다.
		if( ( uiMaterialItemID = pstMakeitem->m_uiToolID[ i ] ) == 0 )		
		{
			break;	
		}

		for( j = 0; j < ON_MAX_FACTORY_NUM; j++ )
		{
			if( ( m_Factory[ j ].DidConstructFactory() == TRUE ) && ( m_Factory[ j ].IsStorage() == TRUE ) )
			{
				// 창고에서 현재 도구 아이템이 있는지 검사한다.
				if( m_Factory[ j ].HasItem( uiMaterialItemID, 1) == TRUE )		
				{
					break;
				}
			}
		}

		// 창고에 아이템이 없다.
		if( j == ON_MAX_FACTORY_NUM )
		{
			return	FALSE;
		}
	}

	return TRUE;
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	이 아이템을 생산하기 위하여 필요한 재료를 삭제한다.
//	수정 일자	:	2002-10-12 오후 9:32:40 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
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
		// 재료 아이템의 ID가 0일때까지만 수행한다.
		if( ( uiMaterialItemID = pstMakeitem->m_uiItemID[ i ] ) == 0 )		break;

		uiMaterialItemQuantity	= (uiItemQuantity / pstMakeitem->m_siProduction) * pstMakeitem->m_siItemNum[ i ];
		if( ( uiItemQuantity % pstMakeitem->m_siProduction ) != 0 )		uiMaterialItemQuantity	+=	pstMakeitem->m_siItemNum[ i ];

		for( j = 0; j < ON_MAX_FACTORY_NUM; j++ )
		{			
			if( ( m_Factory[ j ].DidConstructFactory() == TRUE ) && ( m_Factory[ j ].IsStorage() == TRUE ))
			{				
				// 창고에서 현재 재료 아이템이 있는지 검사한다.
				if( m_Factory[ j ].HasItem( uiMaterialItemID, uiMaterialItemQuantity ) == TRUE )
				{
					// 창고에서 아이템을 삭제한다.
					m_Factory[ j ].DeleteItem( uiMaterialItemID, uiMaterialItemQuantity );

					break;				
				}
			}
		}				
	}	
	
	return	TRUE;
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	이 아이템을 생산하기 위하여 필요한 도구를 랜덤하게 파괴 시킨다.
//	수정 일자	:	2002-10-12 오후 9:04:29 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
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
		// 재료 아이템의 ID가 0일때까지만 수행한다.
		if( ( uiMaterialItemID = pstMakeitem->m_uiToolID[ i ] ) == 0 )		break;	

		for( j = 0; j < ON_MAX_FACTORY_NUM; j++ )
		{
			if( ( m_Factory[ j ].DidConstructFactory() == TRUE ) && ( m_Factory[ j ].IsStorage() == TRUE ) )
			{
				// 창고에서 현재 도구 아이템이 있는지 검사한다.
				if( m_Factory[ j ].HasItem( uiMaterialItemID, 1) == TRUE )
				{
					// 랜덤하게 도구가 없어지도록 한다.
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
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	산업 단지에 있는 공장의 정보를 플레이어에게 보내준다.
//	수정 일자	:	2002-10-08 오후 2:25:10 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
BOOL	SOVillage_IndustrialComplex::SendFactoryInfo( SOPlayer *pPlayer, SI32 siFactoryID )
{
	SOVillage_IndustrialComplex_Factory		*pFactory;

	if( ( pFactory = GetFactory( siFactoryID ) ) == NULL )							return	FALSE;

	return	pFactory->SendFactoryInfo( pPlayer );
}


//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	산업 단지에 있는 창고의 정보를 플레이어에게 보내준다.
//	수정 일자	:	2002-10-08 오후 3:17:13 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
BOOL	SOVillage_IndustrialComplex::SendStorageInfo( SOPlayer *pPlayer, SI32 siFactoryID )
{
	SOVillage_IndustrialComplex_Factory		*pFactory;

	if( ( pFactory = GetFactoryConstructed( siFactoryID ) ) == NULL )					return	FALSE;

	return	pFactory->SendStorageInfo( pPlayer );	
}

*/
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	산업 단지를 구입한다.
//	수정 일자	:	2002-10-08 오후 8:37:25 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
BOOL	SOVillage_IndustrialComplex::BuyIndustrialComplex( SOPlayer *pPlayer, SI32 siMaxConstructibleFactoryNum)
{
	

	// 이미 주인이 있는 검사한다.
	if( HasOwner() == TRUE )						return	FALSE;

	
	m_siOwnerDBAccount					=	pPlayer->stAccountInfo.siDBAccount;
	m_uiOwnerDBSlot						=	pPlayer->stAccountInfo.uiSlot;
	m_siMaxConstructibleFactoryNum		=	siMaxConstructibleFactoryNum;

	ZeroMemory( m_szOwnerName, sizeof( m_szOwnerName ) );
	strncpy( m_szOwnerName, pPlayer->szID, ON_ID_LENGTH );

	

	return	TRUE;
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	토지를 개척한다.
//	수정 일자	:	2002-10-10 오후 5:33:02 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
BOOL	SOVillage_IndustrialComplex::Reclamation( SOPlayer *pPlayer, SI32 siMaxFactoryNum )
{
	DBIKReclamation		DBReclamation;

	// 주인인지 검사한다.
	if( IsOwner( pPlayer ) == FALSE )								return	FALSE;

	// 이 산업 단지에서 지을 수 있는 최대 건물 수를 초과하는지 검사한다.
	if( m_siMaxConstructibleFactoryNum >= siMaxFactoryNum )			return	FALSE;
	
	m_siMaxConstructibleFactoryNum++;


	 
	
	// DB에 저장한다.
	DBReclamation.uiVillageCode	=	pPlayer->stAccountInfo.pVillage->uiVillageCode;	
	DBReclamation.uiFarmID		=	UI08( m_siIndustrialComplexID );
	DBReclamation.moCapital     =   ON_PLANT_EXPANDSLOT_COST * ON_CAPITAL_RATE_PLANT;
	DBReclamation.moPay         =   ON_PLANT_EXPANDSLOT_COST;
	DBReclamation.DBAccount     =   pPlayer->stAccountInfo.siDBAccount;
	DBReclamation.uiSlot        =   pPlayer->stAccountInfo.uiSlot;

		
	m_pDataBase->writeDBQuery( SO_DB_INFOKIND_FARM_EXPANDSLOT, &DBReclamation, sizeof (DBReclamation ) );

	return TRUE;

}


//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	이 산업단지의 주인인지 검사한다.
//	수정 일자	:	2002-10-10 오후 5:36:09 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
BOOL	SOVillage_IndustrialComplex::IsOwner( SOPlayer *pPlayer )
{
	if( pPlayer->stAccountInfo.siDBAccount == m_siOwnerDBAccount && pPlayer->stAccountInfo.uiSlot == m_uiOwnerDBSlot )		return	TRUE;

	return	FALSE;
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	주인을 변경한다.
//	수정 일자	:	2002-10-10 오후 5:36:09 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
BOOL	SOVillage_IndustrialComplex::ChangeOwner( SOPlayer *pPlayer )
{
	// 주인변경후.
	m_siOwnerDBAccount = pPlayer->stAccountInfo.siDBAccount;
	m_uiOwnerDBSlot = pPlayer->stAccountInfo.uiSlot;

	strcpy(m_szOwnerName,pPlayer->szID);

	// 일을 안한날짜를 초기화해준다.
	m_siDayNotWorked = 0;

	return	TRUE;


}


//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	공장을 건설한다.
//	수정 일자	:	2002-10-10 오후 8:55:05 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
BOOL	SOVillage_IndustrialComplex::ConstructFactory( SOPlayer *pPlayer, SI32 siFactoryID, CHAR cFactoryCode[ 4 ], MONEY mnPay, MONEY *pmnCapital )
{		

	// 건설하려는 건물에 이미 건물이 건설되었는지 검사한다.
	if( m_Factory[ siFactoryID ].DidConstructFactory() == TRUE )			return	FALSE;

	return	m_Factory[ siFactoryID ].ConstructFactory( pPlayer, siFactoryID, cFactoryCode, mnPay, pmnCapital );

}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	공장을 파괴한다.
//	수정 일자	:	2002-10-15 오전 11:13:30 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
BOOL	SOVillage_IndustrialComplex::DestroyFactory( SOPlayer *pPlayer, SI32 siFactoryID, MONEY *pmnCapital )
{
	SOVillage_IndustrialComplex_Factory		*pFactory;

	if( ( pFactory = GetFactoryConstructed( siFactoryID ) ) == NULL )		return	FALSE;

	pFactory->DestroyFactory( pPlayer, pmnCapital );

	return	TRUE;
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	공장에서 일을 할 수 있는지 검사한다.
//	수정 일자	:	2002-10-15 오전 10:41:23 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
BOOL	SOVillage_IndustrialComplex::CanWork( SOPlayer *pPlayer )
{
	SOVillage_IndustrialComplex_Factory		*pFactory;

	if( ( pFactory = GetEnteredFactory( pPlayer ) ) == NULL )						return	FALSE;

	return	CanWork( pFactory );
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	일을 할 수 있는지 검사한다.
//	수정 일자	:	2002-10-11 오후 4:21:13 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
BOOL	SOVillage_IndustrialComplex::CanWork( SOVillage_IndustrialComplex_Factory *pFactory )
{
	UI16 uiProducingItemID;	
	
	if( pFactory->CanWork( ) == FALSE )
	{
		return	FALSE;
	}

	// 공장에서 일을 하였을 시 생기는 아이템을 창고에 넣을 수 있는지 검사한다.
	if( ( pFactory->IsFactory() == TRUE ) && ( pFactory->IsUpgrading() == FALSE ) )
	{
		uiProducingItemID	=	pFactory->GetProducingItemID();

		SI32 siStorageIndex = -1;
		SI32 siStoragePos = -1;

		//아래 함수로 들어올 경우 아이템 생성시에 빈 slot 이 있는 지를 체크 하는 것임
		//빈 slot 이 없다면, 
		if( CanAddItem( uiProducingItemID , pFactory->GetProducingItemNumber(), &siStorageIndex, &siStoragePos ) == FALSE )
		{
			return	FALSE;
		}

		// 창고에 일을 하기 위한 도구가 있는지 검사한다.
		if( IsMaterialToolEnoughToProduce( uiProducingItemID ) == FALSE )
		{
			return	FALSE;
		}
	}
	
	return	TRUE;
}


//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	산업 단지의 공장에 들어간다.
//	수정 일자	:	2002-10-11 오후 4:37:56 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
BOOL	SOVillage_IndustrialComplex::EnterFactory( SOPlayer *pPlayer, SI32 siFactoryID )
{	
	SOVillage_IndustrialComplex_Factory		*pFactory;	

	// 이미 들어간 공장이 있다면 들어갈 수가 없다.
	if( ( GetEnteredFactory( pPlayer ) ) != NULL )									return	FALSE;

	if( ( pFactory = GetFactoryConstructed( siFactoryID ) ) == NULL )				return	FALSE;

	return	pFactory->EnterPlayer( pPlayer, IsOwner( pPlayer ) );
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	산업 단지의 공장에서 나간다.
//	수정 일자	:	2002-10-11 오후 4:50:03 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
BOOL	SOVillage_IndustrialComplex::LeaveFactory( SOPlayer *pPlayer )
{
	SOVillage_IndustrialComplex_Factory		*pFactory;

	// 이미 들어간 공장이 없다면 나갈 수 없다.
	if( ( pFactory = GetEnteredFactory( pPlayer ) ) == NULL )							return	FALSE;

	return	pFactory->LeavePlayer( pPlayer );
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	일을 시작한다.
//	수정 일자	:	2002-10-11 오후 7:25:53 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
BOOL	SOVillage_IndustrialComplex::BeginWork( SOPlayer *pPlayer )
{
	SOVillage_IndustrialComplex_Factory		*pFactory;

	// 플레이어가 들어간 공장을 얻어온다.
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

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	공장의 임금을 변경한다.
//	수정 일자	:	2002-10-11 오후 8:59:44 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
BOOL	SOVillage_IndustrialComplex::ChangeFactoryPay( SOPlayer *pPlayer, MONEY mnPay, MONEY *pmnCapital )
{
	SOVillage_IndustrialComplex_Factory		*pFactory;

	// 플레이어가 들어간 공장을 얻어온다.
	if( ( pFactory = GetEnteredFactory( pPlayer ) ) == NULL )							return	FALSE;

	return	pFactory->ChangeFactoryPay( pPlayer, mnPay, pmnCapital );	
}


//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	현재 작업중인 공장에서 작업중인것을 취소한다.
//	수정 일자	:	2002-10-12 오후 2:45:13 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
BOOL	SOVillage_IndustrialComplex::CancelWork( SOPlayer *pPlayer, SI32 siFactoryID, MONEY *pmnCapital )
{
	SOVillage_IndustrialComplex_Factory		*pFactory;

	// 플레이어가 들어간 공장을 얻어온다.
	if( ( pFactory = GetEnteredFactory( pPlayer ) ) == NULL )							return	FALSE;

	return	pFactory->CancelWork( pPlayer, siFactoryID, pmnCapital );		
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	공장을 업그레이드 시킨다.
//	수정 일자	:	2002-10-12 오후 5:50:21 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
BOOL	SOVillage_IndustrialComplex::UpgradeFactory( SOPlayer *pPlayer, SI32 siFactoryID, MONEY mnPay, MONEY *pmnCapital )
{
	SOVillage_IndustrialComplex_Factory		*pFactory;
	
	if( ( pFactory = GetFactoryConstructed( siFactoryID ) ) == NULL )					return	FALSE;

	return	pFactory->UpgradeFactory( pPlayer, mnPay, pmnCapital );			
}


//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	아이템을 생산한다.
//	수정 일자	:	2002-10-12 오후 7:16:47 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
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
		// 아이템을 소모한다.

		// 아이템을 생산하는데 사용되는 재료 아이템을 삭제한다.
		DeleteMaterialItem( uiItemID, uiItemQuantity );

		// 아이템을 생산하는데 사용하는 도구를 무작위로 파괴 시킨다.
		DestroyToolItemRandomly( uiItemID );

	}

	return	dwRet;			
}


//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	창고 안에 아이템을 넣는다.
//	수정 일자	:	2002-10-14 오전 11:24:21 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
BOOL	SOVillage_IndustrialComplex::AddItem( SOPlayer *pPlayer, UI16 uiItemID, UI16 uiItemQuantity ,BOOL bSendToDB)
{
	SOVillage_IndustrialComplex_Factory		*pFactory;
	
	if( ( pFactory = GetEnteredFactory( pPlayer ) ) == NULL )							return	FALSE;

	return	pFactory->AddItem( pPlayer, uiItemID, uiItemQuantity ,bSendToDB);			
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	창고안에서 아이템을 삭제한다.
//	수정 일자	:	2002-10-14 오전 11:25:01 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
BOOL	SOVillage_IndustrialComplex::DeleteItem( SOPlayer *pPlayer, UI16 uiItemID, UI16 uiItemQuantity ,BOOL bSendToDB)
{
	SOVillage_IndustrialComplex_Factory		*pFactory;
	
	if( ( pFactory = GetEnteredFactory( pPlayer ) ) == NULL )							return	FALSE;

	return	pFactory->DeleteItem( pPlayer, uiItemID, uiItemQuantity ,bSendToDB);			
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	아무 창고 안에 아이템을 넣는다.
//	수정 일자	:	2002-10-14 오후 2:12:57 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
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

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	창고 안에 아이템을 넣을 수 있는지 검사한다.
//	수정 일자	:	2002-12-17 오후 9:49:37 - 박준태
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

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


//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	가동중이지 않는 산업 단지는 없앤다.
//	수정 일자	:	2002-10-15 오후 2:26:16 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
VOID	SOVillage_IndustrialComplex::DestroyIndustrialComplexNotWork( MONEY *pmnIndustrialComplexPrice, MONEY mnIndustrialComplexCost )
{	
	DBIKBuyFarmByForceInfo			SellFarmInfo;	

	
	// DB를 업데이트한다.
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

	// 서버를 업데이트한다.
	InitIndustrialComplex();
	

	
	
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	하루가 지났다.
//	수정 일자	:	2002-10-15 오후 2:21:02 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
VOID	SOVillage_IndustrialComplex::DayChanged( SOPlayer *pPlayer, MONEY mnIndustrialComplexCost )
{	
	DBIKBuyFarmByForceInfo		stUpdateWorkDay;
	MONEY						mnIndustrialComplexPrice;

	m_siDayNotWorked++;	

	if( m_siDayNotWorked > ON_FORCE_TAKE_DAY)
	{
		// 오랫동안 아무도 일을 하지 않는 산업 단지는 없앤다.

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


// 2003/07/03 18:30 양정모 수정
// NPC가 일을 하는 곳은 일반 유저와 마찬가지로 일을 할 수 있는 환경에서만 일을 하도록 한다.
VOID	SOVillage_IndustrialComplex::DoNPCWork( UI32 uiNPCLabor )
{	
	SOVillage_IndustrialComplex_Factory		*pFactory = NULL, *pTempFactory;

	MONEY moMaxPay = 0;
	
	// 최고임금을 찾아서
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

	// 일할곳이 있으면
	if( pFactory != NULL )
	{
		// 일을해준다. 
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

