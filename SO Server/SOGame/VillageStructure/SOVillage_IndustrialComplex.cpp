#include "SOMain.h"
#include "SODatabase.h"
#include "SOPlayer.h"
#include "SOVillage_IndustrialComplex_Factory.h"
#include "SOVillage_IndustrialComplex.h"

CItem			*SOVillage_IndustrialComplex::m_pItem;
SODatabase		*SOVillage_IndustrialComplex::m_pDataBase;
OnlineBuilding	*SOVillage_IndustrialComplex::m_pBuilding;

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	薑瞳 滲熱蒂 蟾晦�� и棻.		
//	熱薑 橾濠	:	2002-10-08 螃�� 9:47:22 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
VOID	SOVillage_IndustrialComplex::InitStaticVariable( CItem *pItem, SODatabase *pDataBase, OnlineBuilding *pBuilding )
{
	m_pItem			=	pItem;
	m_pDataBase		=	pDataBase;
	m_pBuilding		=	pBuilding;
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	儅撩濠.	
//	熱薑 橾濠	:	2002-10-07 螃�� 3:24:56 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
SOVillage_IndustrialComplex::SOVillage_IndustrialComplex()
{	
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	模資濠.	
//	熱薑 橾濠	:	2002-10-07 螃�� 3:27:52 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
SOVillage_IndustrialComplex::~SOVillage_IndustrialComplex()
{	
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	蟾晦��.
//	熱薑 橾濠	:	2002-10-07 螃�� 3:30:22 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
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

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	骯機 欽雖陛 つ溜擊 衛縑 п遽棻.
//	熱薑 橾濠	:	2002-10-15 螃�� 2:39:28 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
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

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	詭賅葬 п薯.
//	熱薑 橾濠	:	2002-10-11 螃�� 1:24:12 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
VOID	SOVillage_IndustrialComplex::Free()
{

	SI32	i;

	for( i = 0; i < ON_MAX_FACTORY_NUM; i++ )
	{
		m_Factory[ i ].Free();
	}

	m_PlayerMgr.Free();
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	奢濰擊 橢橫螞棻.
//	熱薑 橾濠	:	2002-10-07 螃�� 3:41:00 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
SOVillage_IndustrialComplex_Factory*	SOVillage_IndustrialComplex::GetFactory( SI32 siFactoryID )
{
	if( IsValidFactoryID( siFactoryID ) == FALSE )		return	NULL;	

	return &m_Factory[ siFactoryID ];
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	奢濰檜 勒撲脹 奢濰 偌羹蒂 橢橫螞棻.
//	熱薑 橾濠	:	2002-10-14 螃�� 2:31:09 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
SOVillage_IndustrialComplex_Factory*	SOVillage_IndustrialComplex::GetFactoryConstructed( SI32 siFactoryID )
{
	SOVillage_IndustrialComplex_Factory		*pFactory;

	if( ( pFactory = GetFactory( siFactoryID ) ) == NULL )			return	NULL;

	if( pFactory->DidConstructFactory() == FALSE )					return	NULL;

	return	pFactory;
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	Ы溯檜橫陛 菟橫除 奢濰擊 橢橫螞棻.
//	熱薑 橾濠	:	2002-10-11 螃�� 4:29:01 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
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

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	Ы溯檜橫陛 菟橫除 奢濰擊 橢橫螞棻.
//	熱薑 橾濠	:	2002-10-11 螃�� 4:29:01 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
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



//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	嶸�褲� 奢濰 ID 檣陛?
//	熱薑 橾濠	:	2002-10-07 螃�� 3:41:33 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
BOOL	SOVillage_IndustrialComplex::IsValidFactoryID( SI32 siFactoryID )
{
	if( ( siFactoryID >= 0 ) && ( siFactoryID < ON_MAX_FACTORY_NUM ) )		return	TRUE;

	return	FALSE;
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	Ы溯檜橫陛 菟橫諮棻.
//	熱薑 橾濠	:	2002-10-07 螃�� 4:03:27 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
BOOL	SOVillage_IndustrialComplex::EnterPlayer( SOPlayer *pPlayer )
{
	SI32				siFactoryNum;
	OnFarmSlotList		FactoryInfo[ ON_MAX_FACTORY_NUM ];	
	DWORD				dwInfoLength;
	
	dwInfoLength	=	GetInfo( FactoryInfo, &siFactoryNum );

	// 奢濰曖 薑爾蒂 Ы溯檜橫縑啪 爾頂遽棻.
	if( m_PlayerMgr.AddPlayer( pPlayer ) == TRUE)
	{
		pPlayer->SendFieldMsg( ON_RESPONSE_FARM_SLOTINFO, SO_SFM_ONLYME, LPVOID( siFactoryNum ), LPVOID( FactoryInfo ), LPVOID( dwInfoLength ), 
								LPVOID( m_siMaxConstructibleFactoryNum ) );
		return	TRUE;
	}
	else
		return	FALSE;
	
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	Ы溯檜橫陛 釭鬲棻.
//	熱薑 橾濠	:	2002-10-07 螃�� 4:23:50 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
BOOL	SOVillage_IndustrialComplex::LeavePlayer( SOPlayer *pPlayer )
{
	// ⑷營 菟橫陛 氈朝 奢濰縑憮紫 頂爾魚棻.
	LeaveFactory( pPlayer );
	
	return	m_PlayerMgr.RemovePlayer( pPlayer );
}


//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	骯機 欽雖縑 氈朝 奢濰菟曖 薑爾蒂 橢橫螞棻.
//	熱薑 橾濠	:	2002-10-07 螃�� 4:50:01 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
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

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	骯機 欽雖曖 薑爾蒂 橢橫螞棻.
//					2離 熱薑) 譆渠 歜旎檜塭堅 ж渦塭紫 橾擊 й 熱 �秣磍抶�(紫掘 睡褶) 譆渠 歜旎戲煎 檣薑ж雖 彊朝棻.
//	熱薑 橾濠	:	2003-07-03 螃�� 06:31:00 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
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


//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	檜 骯機欽雖曖 輿檣檜 氈朝陛.
//	熱薑 橾濠	:	2002-10-07 螃�� 7:44:47 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
BOOL	SOVillage_IndustrialComplex::HasOwner()
{
	if( m_siOwnerDBAccount != 0 )		return	TRUE;

	return	FALSE;	
}


//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	DB縑憮 橢橫螞 薑爾煎 骯機 欽雖蒂 蟾晦�� и棻.
//	熱薑 橾濠	:	2002-10-07 螃�� 8:23:51 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
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


//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	DB縑憮 橢橫螞 薑爾煎 璽堅蒂 蟾晦�� и棻.
//	熱薑 橾濠	:	2002-10-07 螃�� 9:27:47 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
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


//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	DB縑憮 橢橫螞 薑爾煎 奢濰擊 蟾晦�� и棻.
//	熱薑 橾濠	:	2002-10-08 螃�� 4:49:25 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
BOOL	SOVillage_IndustrialComplex::InitFactoryFromDB( SI32 siFactoryID, UI08 uiLevel, MONEY mnPay, UI32 uiAmountOfWork, CHAR cFactoryKind[ 4 ],
									UI16 uiProducingItemID, UI32 uiProducedItemQuantity, UI32 uiMaxProduceItemQuantity )
{
	SOVillage_IndustrialComplex_Factory		*pFactory;

	if( ( pFactory = GetFactory( siFactoryID ) ) == NULL )	return	FALSE;

	return	pFactory->InitFactoryFromDB( uiLevel, mnPay, uiAmountOfWork, cFactoryKind, 
											uiProducingItemID, uiProducedItemQuantity, uiMaxProduceItemQuantity );
}


//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	奢濰擊 籀碟и棻.
//	熱薑 橾濠	:	2002-10-08 螃瞪 11:38:58 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
BOOL	SOVillage_IndustrialComplex::SellFactory( SOPlayer *pPlayer, SI32 siFactoryID )
{
	SOVillage_IndustrialComplex_Factory		*pFactory;

	if( ( pFactory = GetFactoryConstructed( siFactoryID ) ) == NULL )	return	FALSE;	

	return	pFactory->SellFactory( pPlayer );
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	檜 骯機欽雖曖 輿檣擊 橢橫螞棻.
//	熱薑 橾濠	:	2002-10-08 螃瞪 11:49:18 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
BOOL	SOVillage_IndustrialComplex::GetOwner( SI32 *psiOwnerDBAccount, UI08 *puiOwnerDBSlot )
{
	if( HasOwner() == FALSE )		return	FALSE;

	*psiOwnerDBAccount	=	m_siOwnerDBAccount;
	*puiOwnerDBSlot		=	UI08( m_uiOwnerDBSlot );
	
	return	TRUE;
}


//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	Ы溯檜橫陛 檜 骯機 欽雖寰縑 菟橫諦 氈朝雖 匐餌и棻.
//	熱薑 橾濠	:	2002-10-08 螃�� 12:25:29 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
BOOL	SOVillage_IndustrialComplex::IsPlayerIn( SOPlayer *pPlayer )
{
	return	m_PlayerMgr.IsAddedPlayer( pPlayer );
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	橾擊 部葆蟻棻.
//	熱薑 橾濠	:	2002-10-08 螃�� 12:28:51 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
BOOL	SOVillage_IndustrialComplex::FinishWork( SOPlayer *pPlayer, OnReqFarmWorkMsg *pMsg )
{
	SOVillage_IndustrialComplex_Factory		*pFactory;
	UI16									uiProducedItemID = 0;
	UI16									uiProducedItemQuantity = 0;
	
	if( ( pFactory = GetFactoryPlayerIn( pPlayer ) ) == NULL )																return	FALSE;

	if( pFactory->FinishWork(pPlayer, this, pMsg) == FALSE )		return	FALSE;
	
	// 儅骯腎橫霞 嬴檜蠱檜 氈朝雖 匐餌и棻.
	/*if( ( uiProducedItemID != 0 ) && ( uiProducedItemQuantity != 0 ) )
	{
		// 嬴鼠 璽堅 寰縑 嬴檜蠱擊 厥朝棻.
		// 褒ぬп紫 橫聶熱陛 橈棻.
		AddItem( uiProducedItemID , uiProducedItemQuantity );
	}*/

	m_siDayNotWorked	=	0;

	return	TRUE;
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	Ы溯檜橫陛 菟橫陛 氈朝 奢濰擊 橢橫螞棻.
//	熱薑 橾濠	:	2002-10-08 螃�� 12:31:22 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
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

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	嬴檜蠱擊 儅骯ж晦 嬪ж罹 璽堅縑 醱碟и 嬴檜蠱 營猿陛 氈朝雖 匐餌и棻.
//	熱薑 橾濠	:	2002-10-12 螃�� 8:10:40 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
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
		// 營猿 嬴檜蠱曖 ID陛 0橾陽梱雖虜 熱чи棻.
		if( ( uiMaterialItemID = pstMakeitem->m_uiItemID[ i ] ) == 0 )		break;

		uiMaterialItemQuantity	= uiItemQuantity / pstMakeitem->m_siProduction * pstMakeitem->m_siItemNum[ i ];
		if( ( uiItemQuantity % pstMakeitem->m_siProduction ) != 0 )		uiMaterialItemQuantity	+=	pstMakeitem->m_siItemNum[ i ];

		for( j = 0; j < ON_MAX_FACTORY_NUM; j++ )
		{			
			if( ( m_Factory[ j ].DidConstructFactory() == TRUE ) && ( m_Factory[ j ].IsStorage() == TRUE ) )
			{				
				// 璽堅縑憮 ⑷營 營猿 嬴檜蠱檜 氈朝雖 匐餌и棻.
				if( m_Factory[ j ].HasItem( uiMaterialItemID, uiMaterialItemQuantity ) == TRUE )	break;				
			}
		}		

		// 璽堅縑 嬴檜蠱檜 橈棻.
		if( j == ON_MAX_FACTORY_NUM )
		{
			return	FALSE;
		}
	}
	
	return	TRUE;
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	嬴檜蠱擊 儅骯ж晦 嬪ж罹 璽堅縑 醱碟и 嬴檜蠱 紫掘陛 氈朝雖 匐餌и棻.
//	熱薑 橾濠	:	2002-10-12 螃�� 8:36:07 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
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
		// 營猿 嬴檜蠱曖 ID陛 0橾陽梱雖虜 熱чи棻.
		if( ( uiMaterialItemID = pstMakeitem->m_uiToolID[ i ] ) == 0 )		
		{
			break;	
		}

		for( j = 0; j < ON_MAX_FACTORY_NUM; j++ )
		{
			if( ( m_Factory[ j ].DidConstructFactory() == TRUE ) && ( m_Factory[ j ].IsStorage() == TRUE ) )
			{
				// 璽堅縑憮 ⑷營 紫掘 嬴檜蠱檜 氈朝雖 匐餌и棻.
				if( m_Factory[ j ].HasItem( uiMaterialItemID, 1) == TRUE )		
				{
					break;
				}
			}
		}

		// 璽堅縑 嬴檜蠱檜 橈棻.
		if( j == ON_MAX_FACTORY_NUM )
		{
			return	FALSE;
		}
	}

	return TRUE;
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	檜 嬴檜蠱擊 儅骯ж晦 嬪ж罹 в蹂и 營猿蒂 餉薯и棻.
//	熱薑 橾濠	:	2002-10-12 螃�� 9:32:40 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
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
		// 營猿 嬴檜蠱曖 ID陛 0橾陽梱雖虜 熱чи棻.
		if( ( uiMaterialItemID = pstMakeitem->m_uiItemID[ i ] ) == 0 )		break;

		uiMaterialItemQuantity	= (uiItemQuantity / pstMakeitem->m_siProduction) * pstMakeitem->m_siItemNum[ i ];
		if( ( uiItemQuantity % pstMakeitem->m_siProduction ) != 0 )		uiMaterialItemQuantity	+=	pstMakeitem->m_siItemNum[ i ];

		for( j = 0; j < ON_MAX_FACTORY_NUM; j++ )
		{			
			if( ( m_Factory[ j ].DidConstructFactory() == TRUE ) && ( m_Factory[ j ].IsStorage() == TRUE ))
			{				
				// 璽堅縑憮 ⑷營 營猿 嬴檜蠱檜 氈朝雖 匐餌и棻.
				if( m_Factory[ j ].HasItem( uiMaterialItemID, uiMaterialItemQuantity ) == TRUE )
				{
					// 璽堅縑憮 嬴檜蠱擊 餉薯и棻.
					m_Factory[ j ].DeleteItem( uiMaterialItemID, uiMaterialItemQuantity );

					break;				
				}
			}
		}				
	}	
	
	return	TRUE;
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	檜 嬴檜蠱擊 儅骯ж晦 嬪ж罹 в蹂и 紫掘蒂 楠渾ж啪 だ惚 衛鑑棻.
//	熱薑 橾濠	:	2002-10-12 螃�� 9:04:29 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
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
		// 營猿 嬴檜蠱曖 ID陛 0橾陽梱雖虜 熱чи棻.
		if( ( uiMaterialItemID = pstMakeitem->m_uiToolID[ i ] ) == 0 )		break;	

		for( j = 0; j < ON_MAX_FACTORY_NUM; j++ )
		{
			if( ( m_Factory[ j ].DidConstructFactory() == TRUE ) && ( m_Factory[ j ].IsStorage() == TRUE ) )
			{
				// 璽堅縑憮 ⑷營 紫掘 嬴檜蠱檜 氈朝雖 匐餌и棻.
				if( m_Factory[ j ].HasItem( uiMaterialItemID, 1) == TRUE )
				{
					// 楠渾ж啪 紫掘陛 橈橫雖紫煙 и棻.
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
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	骯機 欽雖縑 氈朝 奢濰曖 薑爾蒂 Ы溯檜橫縑啪 爾頂遽棻.
//	熱薑 橾濠	:	2002-10-08 螃�� 2:25:10 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
BOOL	SOVillage_IndustrialComplex::SendFactoryInfo( SOPlayer *pPlayer, SI32 siFactoryID )
{
	SOVillage_IndustrialComplex_Factory		*pFactory;

	if( ( pFactory = GetFactory( siFactoryID ) ) == NULL )							return	FALSE;

	return	pFactory->SendFactoryInfo( pPlayer );
}


//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	骯機 欽雖縑 氈朝 璽堅曖 薑爾蒂 Ы溯檜橫縑啪 爾頂遽棻.
//	熱薑 橾濠	:	2002-10-08 螃�� 3:17:13 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
BOOL	SOVillage_IndustrialComplex::SendStorageInfo( SOPlayer *pPlayer, SI32 siFactoryID )
{
	SOVillage_IndustrialComplex_Factory		*pFactory;

	if( ( pFactory = GetFactoryConstructed( siFactoryID ) ) == NULL )					return	FALSE;

	return	pFactory->SendStorageInfo( pPlayer );	
}

*/
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	骯機 欽雖蒂 掘殮и棻.
//	熱薑 橾濠	:	2002-10-08 螃�� 8:37:25 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
BOOL	SOVillage_IndustrialComplex::BuyIndustrialComplex( SOPlayer *pPlayer, SI32 siMaxConstructibleFactoryNum)
{
	

	// 檜嘐 輿檣檜 氈朝 匐餌и棻.
	if( HasOwner() == TRUE )						return	FALSE;

	
	m_siOwnerDBAccount					=	pPlayer->stAccountInfo.siDBAccount;
	m_uiOwnerDBSlot						=	pPlayer->stAccountInfo.uiSlot;
	m_siMaxConstructibleFactoryNum		=	siMaxConstructibleFactoryNum;

	ZeroMemory( m_szOwnerName, sizeof( m_szOwnerName ) );
	strncpy( m_szOwnerName, pPlayer->szID, ON_ID_LENGTH );

	

	return	TRUE;
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	饜雖蒂 偃繫и棻.
//	熱薑 橾濠	:	2002-10-10 螃�� 5:33:02 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
BOOL	SOVillage_IndustrialComplex::Reclamation( SOPlayer *pPlayer, SI32 siMaxFactoryNum )
{
	DBIKReclamation		DBReclamation;

	// 輿檣檣雖 匐餌и棻.
	if( IsOwner( pPlayer ) == FALSE )								return	FALSE;

	// 檜 骯機 欽雖縑憮 雖擊 熱 氈朝 譆渠 勒僭 熱蒂 蟾婁ж朝雖 匐餌и棻.
	if( m_siMaxConstructibleFactoryNum >= siMaxFactoryNum )			return	FALSE;
	
	m_siMaxConstructibleFactoryNum++;


	 
	
	// DB縑 盪濰и棻.
	DBReclamation.uiVillageCode	=	pPlayer->stAccountInfo.pVillage->uiVillageCode;	
	DBReclamation.uiFarmID		=	UI08( m_siIndustrialComplexID );
	DBReclamation.moCapital     =   ON_PLANT_EXPANDSLOT_COST * ON_CAPITAL_RATE_PLANT;
	DBReclamation.moPay         =   ON_PLANT_EXPANDSLOT_COST;
	DBReclamation.DBAccount     =   pPlayer->stAccountInfo.siDBAccount;
	DBReclamation.uiSlot        =   pPlayer->stAccountInfo.uiSlot;

		
	m_pDataBase->writeDBQuery( SO_DB_INFOKIND_FARM_EXPANDSLOT, &DBReclamation, sizeof (DBReclamation ) );

	return TRUE;

}


//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	檜 骯機欽雖曖 輿檣檣雖 匐餌и棻.
//	熱薑 橾濠	:	2002-10-10 螃�� 5:36:09 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
BOOL	SOVillage_IndustrialComplex::IsOwner( SOPlayer *pPlayer )
{
	if( pPlayer->stAccountInfo.siDBAccount == m_siOwnerDBAccount && pPlayer->stAccountInfo.uiSlot == m_uiOwnerDBSlot )		return	TRUE;

	return	FALSE;
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	輿檣擊 滲唳и棻.
//	熱薑 橾濠	:	2002-10-10 螃�� 5:36:09 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
BOOL	SOVillage_IndustrialComplex::ChangeOwner( SOPlayer *pPlayer )
{
	// 輿檣滲唳��.
	m_siOwnerDBAccount = pPlayer->stAccountInfo.siDBAccount;
	m_uiOwnerDBSlot = pPlayer->stAccountInfo.uiSlot;

	strcpy(m_szOwnerName,pPlayer->szID);

	// 橾擊 寰и陳瞼蒂 蟾晦�倆媮奡�.
	m_siDayNotWorked = 0;

	return	TRUE;


}


//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	奢濰擊 勒撲и棻.
//	熱薑 橾濠	:	2002-10-10 螃�� 8:55:05 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
BOOL	SOVillage_IndustrialComplex::ConstructFactory( SOPlayer *pPlayer, SI32 siFactoryID, CHAR cFactoryCode[ 4 ], MONEY mnPay, MONEY *pmnCapital )
{		

	// 勒撲ж溥朝 勒僭縑 檜嘐 勒僭檜 勒撲腎歷朝雖 匐餌и棻.
	if( m_Factory[ siFactoryID ].DidConstructFactory() == TRUE )			return	FALSE;

	return	m_Factory[ siFactoryID ].ConstructFactory( pPlayer, siFactoryID, cFactoryCode, mnPay, pmnCapital );

}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	奢濰擊 だ惚и棻.
//	熱薑 橾濠	:	2002-10-15 螃瞪 11:13:30 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
BOOL	SOVillage_IndustrialComplex::DestroyFactory( SOPlayer *pPlayer, SI32 siFactoryID, MONEY *pmnCapital )
{
	SOVillage_IndustrialComplex_Factory		*pFactory;

	if( ( pFactory = GetFactoryConstructed( siFactoryID ) ) == NULL )		return	FALSE;

	pFactory->DestroyFactory( pPlayer, pmnCapital );

	return	TRUE;
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	奢濰縑憮 橾擊 й 熱 氈朝雖 匐餌и棻.
//	熱薑 橾濠	:	2002-10-15 螃瞪 10:41:23 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
BOOL	SOVillage_IndustrialComplex::CanWork( SOPlayer *pPlayer )
{
	SOVillage_IndustrialComplex_Factory		*pFactory;

	if( ( pFactory = GetEnteredFactory( pPlayer ) ) == NULL )						return	FALSE;

	return	CanWork( pFactory );
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	橾擊 й 熱 氈朝雖 匐餌и棻.
//	熱薑 橾濠	:	2002-10-11 螃�� 4:21:13 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
BOOL	SOVillage_IndustrialComplex::CanWork( SOVillage_IndustrialComplex_Factory *pFactory )
{
	UI16 uiProducingItemID;	
	
	if( pFactory->CanWork( ) == FALSE )
	{
		return	FALSE;
	}

	// 奢濰縑憮 橾擊 ж艘擊 衛 儅晦朝 嬴檜蠱擊 璽堅縑 厥擊 熱 氈朝雖 匐餌и棻.
	if( ( pFactory->IsFactory() == TRUE ) && ( pFactory->IsUpgrading() == FALSE ) )
	{
		uiProducingItemID	=	pFactory->GetProducingItemID();

		SI32 siStorageIndex = -1;
		SI32 siStoragePos = -1;

		//嬴楚 л熱煎 菟橫螢 唳辦 嬴檜蠱 儅撩衛縑 綴 slot 檜 氈朝 雖蒂 羹觼 ж朝 匙歜
		//綴 slot 檜 橈棻賊, 
		if( CanAddItem( uiProducingItemID , pFactory->GetProducingItemNumber(), &siStorageIndex, &siStoragePos ) == FALSE )
		{
			return	FALSE;
		}

		// 璽堅縑 橾擊 ж晦 嬪и 紫掘陛 氈朝雖 匐餌и棻.
		if( IsMaterialToolEnoughToProduce( uiProducingItemID ) == FALSE )
		{
			return	FALSE;
		}
	}
	
	return	TRUE;
}


//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	骯機 欽雖曖 奢濰縑 菟橫除棻.
//	熱薑 橾濠	:	2002-10-11 螃�� 4:37:56 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
BOOL	SOVillage_IndustrialComplex::EnterFactory( SOPlayer *pPlayer, SI32 siFactoryID )
{	
	SOVillage_IndustrialComplex_Factory		*pFactory;	

	// 檜嘐 菟橫除 奢濰檜 氈棻賊 菟橫陞 熱陛 橈棻.
	if( ( GetEnteredFactory( pPlayer ) ) != NULL )									return	FALSE;

	if( ( pFactory = GetFactoryConstructed( siFactoryID ) ) == NULL )				return	FALSE;

	return	pFactory->EnterPlayer( pPlayer, IsOwner( pPlayer ) );
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	骯機 欽雖曖 奢濰縑憮 釭除棻.
//	熱薑 橾濠	:	2002-10-11 螃�� 4:50:03 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
BOOL	SOVillage_IndustrialComplex::LeaveFactory( SOPlayer *pPlayer )
{
	SOVillage_IndustrialComplex_Factory		*pFactory;

	// 檜嘐 菟橫除 奢濰檜 橈棻賊 釭陞 熱 橈棻.
	if( ( pFactory = GetEnteredFactory( pPlayer ) ) == NULL )							return	FALSE;

	return	pFactory->LeavePlayer( pPlayer );
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	橾擊 衛濛и棻.
//	熱薑 橾濠	:	2002-10-11 螃�� 7:25:53 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
BOOL	SOVillage_IndustrialComplex::BeginWork( SOPlayer *pPlayer )
{
	SOVillage_IndustrialComplex_Factory		*pFactory;

	// Ы溯檜橫陛 菟橫除 奢濰擊 橢橫螞棻.
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

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	奢濰曖 歜旎擊 滲唳и棻.
//	熱薑 橾濠	:	2002-10-11 螃�� 8:59:44 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
BOOL	SOVillage_IndustrialComplex::ChangeFactoryPay( SOPlayer *pPlayer, MONEY mnPay, MONEY *pmnCapital )
{
	SOVillage_IndustrialComplex_Factory		*pFactory;

	// Ы溯檜橫陛 菟橫除 奢濰擊 橢橫螞棻.
	if( ( pFactory = GetEnteredFactory( pPlayer ) ) == NULL )							return	FALSE;

	return	pFactory->ChangeFactoryPay( pPlayer, mnPay, pmnCapital );	
}


//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	⑷營 濛機醞檣 奢濰縑憮 濛機醞檣匙擊 鏃模и棻.
//	熱薑 橾濠	:	2002-10-12 螃�� 2:45:13 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
BOOL	SOVillage_IndustrialComplex::CancelWork( SOPlayer *pPlayer, SI32 siFactoryID, MONEY *pmnCapital )
{
	SOVillage_IndustrialComplex_Factory		*pFactory;

	// Ы溯檜橫陛 菟橫除 奢濰擊 橢橫螞棻.
	if( ( pFactory = GetEnteredFactory( pPlayer ) ) == NULL )							return	FALSE;

	return	pFactory->CancelWork( pPlayer, siFactoryID, pmnCapital );		
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	奢濰擊 機斜溯檜萄 衛鑑棻.
//	熱薑 橾濠	:	2002-10-12 螃�� 5:50:21 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
BOOL	SOVillage_IndustrialComplex::UpgradeFactory( SOPlayer *pPlayer, SI32 siFactoryID, MONEY mnPay, MONEY *pmnCapital )
{
	SOVillage_IndustrialComplex_Factory		*pFactory;
	
	if( ( pFactory = GetFactoryConstructed( siFactoryID ) ) == NULL )					return	FALSE;

	return	pFactory->UpgradeFactory( pPlayer, mnPay, pmnCapital );			
}


//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	嬴檜蠱擊 儅骯и棻.
//	熱薑 橾濠	:	2002-10-12 螃�� 7:16:47 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
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
		// 嬴檜蠱擊 模賅и棻.

		// 嬴檜蠱擊 儅骯ж朝等 餌辨腎朝 營猿 嬴檜蠱擊 餉薯и棻.
		DeleteMaterialItem( uiItemID, uiItemQuantity );

		// 嬴檜蠱擊 儅骯ж朝等 餌辨ж朝 紫掘蒂 鼠濛嬪煎 だ惚 衛鑑棻.
		DestroyToolItemRandomly( uiItemID );

	}

	return	dwRet;			
}


//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	璽堅 寰縑 嬴檜蠱擊 厥朝棻.
//	熱薑 橾濠	:	2002-10-14 螃瞪 11:24:21 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
BOOL	SOVillage_IndustrialComplex::AddItem( SOPlayer *pPlayer, UI16 uiItemID, UI16 uiItemQuantity ,BOOL bSendToDB)
{
	SOVillage_IndustrialComplex_Factory		*pFactory;
	
	if( ( pFactory = GetEnteredFactory( pPlayer ) ) == NULL )							return	FALSE;

	return	pFactory->AddItem( pPlayer, uiItemID, uiItemQuantity ,bSendToDB);			
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	璽堅寰縑憮 嬴檜蠱擊 餉薯и棻.
//	熱薑 橾濠	:	2002-10-14 螃瞪 11:25:01 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
BOOL	SOVillage_IndustrialComplex::DeleteItem( SOPlayer *pPlayer, UI16 uiItemID, UI16 uiItemQuantity ,BOOL bSendToDB)
{
	SOVillage_IndustrialComplex_Factory		*pFactory;
	
	if( ( pFactory = GetEnteredFactory( pPlayer ) ) == NULL )							return	FALSE;

	return	pFactory->DeleteItem( pPlayer, uiItemID, uiItemQuantity ,bSendToDB);			
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	嬴鼠 璽堅 寰縑 嬴檜蠱擊 厥朝棻.
//	熱薑 橾濠	:	2002-10-14 螃�� 2:12:57 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
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

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	璽堅 寰縑 嬴檜蠱擊 厥擊 熱 氈朝雖 匐餌и棻.
//	熱薑 橾濠	:	2002-12-17 螃�� 9:49:37 - 夢遽鷓
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收

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


//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	陛翕醞檜雖 彊朝 骯機 欽雖朝 橈撻棻.
//	熱薑 橾濠	:	2002-10-15 螃�� 2:26:16 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
VOID	SOVillage_IndustrialComplex::DestroyIndustrialComplexNotWork( MONEY *pmnIndustrialComplexPrice, MONEY mnIndustrialComplexCost )
{	
	DBIKBuyFarmByForceInfo			SellFarmInfo;	

	
	// DB蒂 機等檜おи棻.
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

	// 憮幗蒂 機等檜おи棻.
	InitIndustrialComplex();
	

	
	
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	ж瑞陛 雖陬棻.
//	熱薑 橾濠	:	2002-10-15 螃�� 2:21:02 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
VOID	SOVillage_IndustrialComplex::DayChanged( SOPlayer *pPlayer, MONEY mnIndustrialComplexCost )
{	
	DBIKBuyFarmByForceInfo		stUpdateWorkDay;
	MONEY						mnIndustrialComplexPrice;

	m_siDayNotWorked++;	

	if( m_siDayNotWorked > ON_FORCE_TAKE_DAY)
	{
		// 螃概翕寰 嬴鼠紫 橾擊 ж雖 彊朝 骯機 欽雖朝 橈撻棻.

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


// 2003/07/03 18:30 曄薑賅 熱薑
// NPC陛 橾擊 ж朝 夠擎 橾奩 嶸盪諦 葆雙陛雖煎 橾擊 й 熱 氈朝 �秣瞈□飛� 橾擊 ж紫煙 и棻.
VOID	SOVillage_IndustrialComplex::DoNPCWork( UI32 uiNPCLabor )
{	
	SOVillage_IndustrialComplex_Factory		*pFactory = NULL, *pTempFactory;

	MONEY moMaxPay = 0;
	
	// 譆堅歜旎擊 瓊嬴憮
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

	// 橾й夠檜 氈戲賊
	if( pFactory != NULL )
	{
		// 橾擊п遽棻. 
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

