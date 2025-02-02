#include "SOMain.h"
#include "SODatabase.h"
#include "SOVillage_MarketStructure.h"

CItem			*SOVillage_Market::m_pItem;
cltIProfit		*SOVillage_Market::m_pIProfit;
SODatabase		*SOVillage_Market::m_pDataBase;

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	薑瞳 滲熱蒂 蟾晦�� и棻.
//	熱薑 橾濠	:	2002-10-01 螃�� 2:01:42 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
VOID	SOVillage_Market::InitStaticVariable( CItem *pItem, cltIProfit *pIProfit, SODatabase *pDataBase )
{
	m_pItem			=	pItem;
	m_pIProfit		=	pIProfit;
	m_pDataBase		=	pDataBase;
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	儅撩濠.
//	熱薑 橾濠	:	2002-09-30 螃�� 9:14:38 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
SOVillage_Market::SOVillage_Market()
{
	
}


//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	模資濠.
//	熱薑 橾濠	:	2002-09-30 螃�� 9:14:52 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
SOVillage_Market::~SOVillage_Market()
{

}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	蟾晦��.
//	熱薑 橾濠	:	2002-10-01 螃瞪 9:55:26 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
BOOL	SOVillage_Market::Init( UI16 uiVillageCode, UI32 uiItemsID[ ON_VILLAGE_MAX_ITEM ], SI32 siItemsAtb[ ON_VILLAGE_MAX_ITEM ], UI32 uiItemsQuantity[ ON_VILLAGE_MAX_ITEM ] )
{
	SI32				i;
	SI32				siItemInfo[ ON_VILLAGE_MAX_ITEM * 2 ];
//	DBIKVillageItem		VillageItem;
//	UI16				uiItemID;
//	UI16				uiItemQuantity;

	// 衛瞪縑 菟橫陞 熱 氈朝 Ы溯檜橫曖 熱蒂 SOVILLAGE_MAX_MAKRET_PLAYER_NUM煎 薯ии棻.
	SOVillage_BaseStructure::Init( uiVillageCode, SOVILLAGE_MAX_MAKRET_PLAYER_NUM );

	// 蟾晦�飛� и棻.
	ZeroMemory( m_SellItem, sizeof( m_SellItem ) );
	ZeroMemory( siItemInfo, sizeof( siItemInfo ) );

	// 衛瞪縑憮 鏃晝ж朝 嬴檜蠱擊 蟾晦�� и棻.
	for( i = 0; i < ON_VILLAGE_MAX_ITEM; i++ )
	{	
		// �倣� 薑爾縑憮朝 ID諦 樓撩, 陛問薑爾(滲ж雖 彊朝匙)虜 厥橫輿賊 脹棻, 熱榆擎 DB縑憮 橢橫諦撿 и棻.
		m_SellItem[ i ].uiItemID		=	UI16( uiItemsID[ i ] );
		m_SellItem[ i ].uiItemAtb		=	UI08( siItemsAtb[ i ] );				
	}	

	// DB縑憮 嬴檜蠱曖 熱榆擊 橢橫諦 撲薑 и棻 	
	if( m_pDataBase->GetDBInfo( SO_DB_INFOKIND_VILLAGEMARKETSELLITEM, LPVOID( &uiVillageCode ), LPVOID( siItemInfo ) ) == FALSE ) return FALSE;	
	for( i = 0; i < ON_VILLAGE_MAX_ITEM; i++ )	
	{
		if(uiItemsID[i] != 0)
		{
			if( siItemInfo[ 0 ] == 0 )
			{
				DBIKVillageItem	TempVillageItemInfo;
				
				TempVillageItemInfo.ItemID			=	uiItemsID[i];
				TempVillageItemInfo.uiItemQuantity		=	uiItemsQuantity[i];
				TempVillageItemInfo.VillageCode	=	uiVillageCode;
				TempVillageItemInfo.Slot			=	i;

				m_pDataBase->writeDBQuery( SO_DB_INFOKIND_VILLAGE_ITEM, &TempVillageItemInfo, sizeof( DBIKVillageItem ) );
			}
		}
		if( siItemInfo[ i * 2 ] != 0 )
			SetItemQuantity( UI16( siItemInfo[ i * 2 ] ), UI16( siItemInfo[ i * 2 + 1 ] ));
	}

	/*
		if( siItemInfo[ i * 2 ] == 0 )	continue;
			
		if( ( SetItemQuantity( UI16( siItemInfo[ i * 2 ] ), UI16( siItemInfo[ i * 2 + 1 ] ) ) ) == FALSE )			
		{
			// DB縑朝 氈雖虜 憮幗縑 п渡 嬴檜蠱檜 襄營ж雖 彊朝棻賊 DB蒂 雖遴棻.
			VillageItem.VillageCode			=	uiVillageCode;
			VillageItem.Slot				=	i;
			VillageItem.uiItemQuantity		=	0;
			VillageItem.ItemID				=	0;

			m_pDataBase->writeDBQuery( SO_DB_INFOKIND_SETVILLAGEITEM, &VillageItem, sizeof( VillageItem ) );
		}
	}*/

	/*
	// 臢蝶お縑朝 氈雖虜 DB縑 橈朝匙菟擊 撲薑 и棻 	
	for(i = 0; i < ON_VILLAGE_MAX_ITEM; i++)	
	{				
		if( GetItemID( i, &uiItemID ) == FALSE )							continue;		
		if( GetItemQuantity( uiItemID, &uiItemQuantity ) == FALSE )		continue;
		if( uiItemQuantity != 0 )													continue;

		VillageItem.VillageCode		=	uiVillageCode;
		VillageItem.Slot			=	i;
		VillageItem.ItemID			=	uiItemID;
		VillageItem.uiItemQuantity	=	UI16( uiItemsQuantity[ i ] );						
		m_pDataBase->writeDBQuery( SO_DB_INFOKIND_SETVILLAGEITEM, &VillageItem, sizeof( VillageItem ) );

		if( SetItemQuantity( uiItemID, UI16( uiItemsQuantity[ i ] ) )	== FALSE )	return	FALSE;		
	}	*/


	// 嬴檜蠱曖 陛問擊 唸薑и棻.
	UpdateAllItemsPrice();

	return	TRUE;
}


//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	詭賅葬蒂 п薯и棻.
//	熱薑 橾濠	:	2002-09-30 螃�� 9:16:16 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
VOID	SOVillage_Market::Free()
{
	SOVillage_BaseStructure::Free();	
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	嬴檜蠱曖 熱榆擊 撲薑и棻.
//	熱薑 橾濠	:	2002-10-01 螃瞪 10:05:06 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
BOOL	SOVillage_Market::SetItemQuantity( UI16 uiItemID, UI16 uiItemQuantity )
{
	MARKET_ITEM		*pMarketItem;		

	if( ( pMarketItem =	FindItem( uiItemID ) ) == NULL )	return	FALSE;

	pMarketItem->uiItemQuantity	=	uiItemQuantity;

	return	TRUE;
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	嬴檜蠱曖 熱榆擊 橢橫螞棻.
//	熱薑 橾濠	:	2002-10-01 螃瞪 10:17:37 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
BOOL	SOVillage_Market::GetItemQuantity( UI16 uiItemID, UI16 *puiItemQuantity )
{
	MARKET_ITEM		*pMarketItem;		

	if( ( pMarketItem =	FindItem( uiItemID ) ) == NULL )	return	FALSE;

	*puiItemQuantity	=	pMarketItem->uiItemQuantity;

	return	TRUE;
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	嬴檜蠱擊 瓊朝棻.
//	熱薑 橾濠	:	2002-10-01 螃瞪 10:08:02 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
MARKET_ITEM*	SOVillage_Market::FindItem( UI16 uiItemID )
{
	SI32	i;

	if( uiItemID == 0 )				return	NULL;

	for( i = 0; i < ON_VILLAGE_MAX_ITEM; i++ )
	{
		if( m_SellItem[ i ].uiItemID == uiItemID )
			return	&m_SellItem[ i ];
	}

	return	NULL;
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	嬴檜蠱曖 ID蒂 憲嬴螞棻.
//	熱薑 橾濠	:	2002-10-01 螃瞪 10:24:53 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
BOOL	SOVillage_Market::GetItemID( SI32 siIndex, UI16 *puiItemID )
{
	if( IsValidItemIndex( siIndex ) == FALSE )			return	FALSE;

	if( m_SellItem[ siIndex ].uiItemID != 0)			return	FALSE;

	*puiItemID	=	m_SellItem[ siIndex ].uiItemID;

	return	TRUE;
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	嶸�褲� 檣策蝶檣雖 匐餌и棻.
//	熱薑 橾濠	:	2002-10-01 螃瞪 10:26:18 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
BOOL	SOVillage_Market::IsValidItemIndex( SI32 siIndex )
{
	if( siIndex >= 0 && siIndex < ON_VILLAGE_MAX_ITEM )	return	TRUE;
	else												return	FALSE;
}


//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	陝 嬴檜蠱曖 陛問擊 唸薑и棻.
//	熱薑 橾濠	:	2002-10-01 螃瞪 10:35:29 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
BOOL	SOVillage_Market::UpdateAllItemsPrice()
{
	SI32			i;

	for( i = 0; i < ON_VILLAGE_MAX_ITEM; i++ )
	{
		if( m_SellItem[ i ].uiItemID == 0 )								continue;
		
		if( UpdateItemPrice( m_SellItem[ i ].uiItemID ) == FALSE )		return	FALSE;
	}

	return	TRUE;
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	嬴檜蠱曖 陛問擊 偵褐и棻.
//	熱薑 橾濠	:	2002-10-01 螃瞪 11:19:42 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
BOOL	SOVillage_Market::UpdateItemPrice( UI16 uiItemID, MONEY *pItemBasePrice )
{
	MARKET_ITEM		*pMarketItem;				
	MONEY			mnItemBasePrice;

	if( ( pMarketItem =	FindItem( uiItemID ) ) == NULL )							return	FALSE;
	
	// 嬴檜蠱曖 陛問擊 橢橫螞棻.
	GetItemBasePrice( uiItemID, &mnItemBasePrice );
	m_pIProfit->GetItemPrice( mnItemBasePrice, pMarketItem->uiItemQuantity, &pMarketItem->stItemPrices );

	if( pItemBasePrice != NULL )	*pItemBasePrice	=	mnItemBasePrice;

	return	TRUE;
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	嬴檜蠱曖 熱榆擊 隸陛衛鑑棻.
//	熱薑 橾濠	:	2002-10-01 螃瞪 11:14:52 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
BOOL	SOVillage_Market::IncreaseItemQuantity( UI16 uiItemID, UI16 uiItemQuantity, BOOL bSendToDB )
{
	MARKET_ITEM		*pMarketItem;		
	MONEY			mnItemBasePrice;

	if( ( pMarketItem =	FindItem( uiItemID ) ) == NULL )							return	FALSE;

	pMarketItem->uiItemQuantity	=	min( USHRT_MAX, pMarketItem->uiItemQuantity + uiItemQuantity );	

	// 嬴檜蠱曖 陛問 啗骯擊 億煎檜 偵褐и棻.
	if( UpdateItemPrice( pMarketItem->uiItemID, &mnItemBasePrice ) == FALSE )		return	FALSE;																		

	SendChangedItemInfogMsgToPlayer( pMarketItem->uiItemID, pMarketItem->uiItemQuantity, mnItemBasePrice );

	// DB縑 盪濰и棻.
	if( bSendToDB == TRUE )
	{
		UpdateItemInfoInDB( pMarketItem->uiItemID, pMarketItem->uiItemQuantity );
	}

	return	TRUE;
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	嬴檜蠱曖 熱榆擊 馬模衛鑑棻.
//	熱薑 橾濠	:	2002-10-01 螃瞪 11:26:50 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
BOOL	SOVillage_Market::DecreaseItemQuantity( UI16 uiItemID, UI16 uiItemQuantity, BOOL bSendToDB )
{
	MARKET_ITEM		*pMarketItem;		
	MONEY			mnItemBasePrice;

	if( ( pMarketItem =	FindItem( uiItemID ) ) == NULL )				return	FALSE;

	// 衛瞪縑 氈朝 嬴檜蠱爾棻 馬模衛酈溥朝 嬴檜蠱檜 渦 號擎雖 匐餌и棻.
	if( pMarketItem->uiItemQuantity < uiItemQuantity )					return	FALSE;
	
	// 嬴檜蠱擊 馬模衛鑑棻.
	pMarketItem->uiItemQuantity		-=	uiItemQuantity;	

	// 嬴檜蠱曖 陛問 啗骯擊 億煎檜 偵褐и棻.
	if( UpdateItemPrice( uiItemID, &mnItemBasePrice ) == FALSE )		return	FALSE;																		

	SendChangedItemInfogMsgToPlayer( pMarketItem->uiItemID, pMarketItem->uiItemQuantity, mnItemBasePrice );

	// DB縑 盪濰и棻.
	if( bSendToDB == TRUE )
	{
		UpdateItemInfoInDB( pMarketItem->uiItemID, pMarketItem->uiItemQuantity );
	}

	return	TRUE;
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	っ衙ж朝 嬴檜蠱曖 葬蝶お蒂 橢橫螞棻.
//	熱薑 橾濠	:	2002-10-01 螃瞪 11:32:12 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
SI16	SOVillage_Market::GetSellItemList( OnlineReceivedItemInfo *pItemInfo )
{
	SI32	i;
	SI16	siItemsNum;
	MONEY	mnItemBasePrice;
	
	for( siItemsNum = i = 0; i < ON_VILLAGE_MAX_ITEM; i++)
	{
		if( m_SellItem[ i ].uiItemID == 0 )															continue;
		if( GetItemBasePrice( m_SellItem[ i ].uiItemID, &mnItemBasePrice ) == FALSE )				continue;
	
		pItemInfo[ siItemsNum ].uiID			=	m_SellItem[ i ].uiItemID;			// 嬴檜蠱曖 ID.
		pItemInfo[ siItemsNum ].uiQuantity		=	m_SellItem[ i ].uiItemQuantity;		// 嬴檜蠱曖 熱榆.			
		pItemInfo[ siItemsNum ].BasePrice		=	mnItemBasePrice;					// 嬴檜蠱曖 晦遽 陛問

		siItemsNum++;	
	}

	return siItemsNum;
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	嬴檜蠱曖 晦遽 陛問擊 橢橫螞棻.
//	熱薑 橾濠	:	2002-10-01 螃瞪 11:37:59 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
BOOL	SOVillage_Market::GetItemBasePrice( UI16 uiItemID, MONEY *pMoney )
{
	CItemHeader		*pItemHeader;	

	if( ( pItemHeader = m_pItem->GetItem( uiItemID ) ) == NULL)		return	FALSE;

	*pMoney			=		MONEY( pItemHeader->m_siDBPrice );

	return	TRUE;
}


//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	嬴檜蠱擊 掘殮й陽曖 陛問擊 橢橫螞棻.
//	熱薑 橾濠	:	2002-10-01 螃�� 1:22:59 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
BOOL	SOVillage_Market::GetBuyItemPrice( UI16 uiItemID, SI32 siGuildRelation, MONEY *pItemPrice )
{
	MARKET_ITEM		*pMarketItem;		
	BOOL			bSuccess = TRUE;

	if( ( pMarketItem =	FindItem( uiItemID ) ) == NULL )	return	FALSE;

	switch( siGuildRelation )
	{

	case ON_GUILD_RELATION_ENEMY:
	case ON_GUILD_RELATION_WAR:
		*pItemPrice		=	pMarketItem->stItemPrices.EnemyGuildBuy;
		break;


	case ON_GUILD_RELATION_MINE:
	case ON_GUILD_RELATION_NEUTRAL:

		*pItemPrice		=	pMarketItem->stItemPrices.Buy;
		break;


	default:

		bSuccess	=	FALSE;
		break;
	}

	return	bSuccess;	
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	嬴檜蠱擊 っ衙й陽曖 陛問擊 橢橫螞棻.
//	熱薑 橾濠	:	2002-10-01 螃�� 1:26:34 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
BOOL	SOVillage_Market::GetSellItemPrice( UI16 uiItemID, SI32 siGuildRelation, MONEY *pItemPrice )
{
	MARKET_ITEM		*pMarketItem;		
	BOOL			bSuccess = TRUE;

	if( ( pMarketItem =	FindItem( uiItemID ) ) == NULL )	return	FALSE;

	switch( siGuildRelation )
	{

	case ON_GUILD_RELATION_ENEMY:
	case ON_GUILD_RELATION_WAR:
		*pItemPrice		=	pMarketItem->stItemPrices.EnemyGuildSell;
		break;
	case ON_GUILD_RELATION_MINE:
	case ON_GUILD_RELATION_NEUTRAL:

		*pItemPrice		=	pMarketItem->stItemPrices.Sell;
		break;


	default:

		bSuccess	=	FALSE;
		break;
	}

	return	bSuccess;	
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	嬴檜蠱擊 掘殮 塽 っ衙й陽曖 陛問擊 橢橫螞棻.
//	熱薑 橾濠	:	2002-10-01 螃�� 3:39:45 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
BOOL	SOVillage_Market::GetBuySellItemPrice( UI16 uiItemID, MONEY *pBuyItemPrice, MONEY *pSellItemPrice )
{
	MARKET_ITEM		*pMarketItem;		
	BOOL			bSuccess = TRUE;

	if( ( pMarketItem =	FindItem( uiItemID ) ) == NULL )	return	FALSE;

	*pBuyItemPrice	=	pMarketItem->stItemPrices.Buy;
	*pSellItemPrice	=	pMarketItem->stItemPrices.Sell;
	
	return	bSuccess;	
}



//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	滲唳脹 嬴檜蠱 薑爾蒂 衛瞪縑 氈朝 Ы溯檜橫縑啪 賅舒 爾頂遽棻.
//	熱薑 橾濠	:	2002-10-01 螃�� 1:44:08 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
VOID	SOVillage_Market::SendChangedItemInfogMsgToPlayer( UI16 uiItemID, UI16 uiItemQuantity, MONEY mnItemBasePrice )
{
	// 嬴檜蠱檜 滲唳腎歷棻朝 詭衛雖蒂 爾魚棻.
	SendMsgToAllPlayer( ON_RESPONSE_CHANGED_ITEMINFO, LPVOID( uiItemID ), LPVOID( uiItemQuantity ), LPVOID( &mnItemBasePrice ) );
	
}


//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	DB縑 氈朝 嬴檜蠱曖 薑爾蒂 偵褐и棻.
//	熱薑 橾濠	:	2002-10-01 螃�� 2:07:58 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
VOID	SOVillage_Market::UpdateItemInfoInDB( UI16 uiItemID, UI16 uiItemQuantity )
{
	DBIKVillageItemInfo		VillageItemInfo;	

	// DB縑 滲唳脹 嬴檜蠱曖 薑爾蒂 盪濰и棻.
	VillageItemInfo.uiVillageCode	=	m_uiVillageCode;
	VillageItemInfo.siID			=	uiItemID;
	VillageItemInfo.uiQuantity		=	uiItemQuantity;				

	m_pDataBase->writeDBQuery( SO_DB_INFOKIND_UPDATEVILLAGEITEM, &VillageItemInfo, sizeof( DBIKVillageItemInfo ) );
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	嬴檜蠱擊 儅骯 塽 模綠и棻
//	熱薑 橾濠	:	2002-10-01 螃�� 2:23:56 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
VOID	SOVillage_Market::ProcessProduceReduceItem( UI32 uiPopulation )
{
	SI32	i;
	UI32	uiProduceItem, uiReduceItem;
	FILE	*fp;
	CHAR	szBuffer[1024];
	CHAR	*pszToken;
	CHAR	cSeps[] = " ,\t\n";
	SI32	siProduceValueOfItem, siReduceValueOfItem;
	MONEY	mnItemBasePrice;
	
	if((fp = fopen("ProduceReduceItemNum.txt", "rt")) != NULL)
	{
		while(fgets(szBuffer, 1023, fp) != NULL)
		{
			if((pszToken = strtok(szBuffer, cSeps)) != NULL)
			{
				if(stricmp(pszToken, "PRODUCE:") == 0)
				{
					pszToken				=	strtok(NULL, cSeps);
					siProduceValueOfItem	=	atoi(pszToken);
				}
				else if(stricmp(pszToken, "REDUCE:") == 0)
				{
					pszToken				=	strtok(NULL, cSeps);
					siReduceValueOfItem		=	atoi(pszToken);
				}
			}				
		}

		fclose(fp);
	}
	else
	{	
		return;
	}

	// ⑷營 嬴檜蠱曖 薑爾蒂 橢橫諦憮
	// か骯ヶ檜賊 嬴檜蠱擊 隸陛衛酈堅
	// 模賅ヶ檜賊 嬴檜蠱擊 馬模衛鑑棻.
	for(i = 0; i < ON_VILLAGE_MAX_ITEM; i++)	
	{
		// 嶸�褲� 嬴檜蠱檣雖 羹觼и棻.
		if( m_SellItem[ i ].uiItemID == 0 )			continue;

		switch( m_SellItem[ i ].uiItemAtb ) 
		{
		case ON_VILLAGE_PRODUCTION :			// か骯ヶ檜棻.
			
			// 欽 ⑷營 60000偃 嘐虜 橾陽虜.. 嬴檜蠱擊 蹺陛 衛麵 遽棻.
			if( m_SellItem[ i ].uiItemQuantity < 60000 )
			{			
				GetItemBasePrice( m_SellItem[ i ].uiItemID, &mnItemBasePrice );

				uiProduceItem	=	max( ( ( uiPopulation * siProduceValueOfItem ) / 10) / mnItemBasePrice, 1 );

				// 嬴檜蠱擊 隸陛衛鑑棻.
				IncreaseItemQuantity( m_SellItem[ i ].uiItemID, uiProduceItem );
			}

			break;


		case ON_VILLAGE_CONSUMPTION :			// 模賅ヶ檜棻.

			// 欽 ⑷營 嬴檜蠱檜 12000偃 檜鼻橾陽虜 餉薯 п遽棻.
			if( m_SellItem[ i ].uiItemQuantity >= 12000)
			{
				GetItemBasePrice( m_SellItem[ i ].uiItemID, &mnItemBasePrice );

				uiReduceItem	=	max( ( ( uiPopulation * siReduceValueOfItem) / 10) / mnItemBasePrice, 1);

				// 嬴檜蠱擊 模賅и棻.
				DecreaseItemQuantity( m_SellItem[ i ].uiItemID, uiReduceItem);
			}

			break;

		}	
	}		
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	衛瞪縑憮 嬴檜蠱擊 掘殮и棻.
//	熱薑 橾濠	:	2002-10-21 螃�� 4:42:43 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
DWORD	SOVillage_Market::BuyItem( SOPlayer *pPlayer, UI08 uiFollowerID, UI16 uiItemID, UI16 uiItemQuantity, SI16 siPosInInventory, SI32 siGuildRelation, MONEY *pmnItemPrice, MONEY *pmnCapital )
{
	MONEY				uiRealItemsPrice;		
	MARKET_ITEM			*pMarketItem;		

	// 嬴檜蠱擊 橢橫螞棻.
	if( ( pMarketItem = FindItem( uiItemID ) ) == NULL )							return	ON_RET_NOSALE;
	
	// 餌溥堅 ж朝 虜躑曖 嬴檜蠱 熱陛 氈朝雖 匐餌и棻.
	if( pMarketItem->uiItemQuantity < uiItemQuantity )								return	ON_RET_NOITEM;	
	
	// ⑷營 絲戲煎 斜 嬴檜蠱擊 薯渠煎 髦 熱 氈朝雖 っ欽и棻.		
	GetBuyItemPrice( uiItemID, siGuildRelation, &uiRealItemsPrice );
	uiRealItemsPrice	*=	uiItemQuantity;	

	// 嬴檜蠱擊 髦 熱 氈擊 虜躑曖 絲擊 陛雖堅 氈朝雖 匐餌и棻.
	if( pPlayer->EnoughMoney( uiRealItemsPrice ) == FALSE )							return	ON_RET_NOMONEY;		
	
	// 憮幗曖 嬴檜蠱擊 馬模衛鑑棻.								
	DecreaseItemQuantity( uiItemID, uiItemQuantity, FALSE );

	*pmnItemPrice	=	uiRealItemsPrice;
	*pmnCapital		=	uiRealItemsPrice * ON_CAPITAL_RATE_TRADE;	

	return	ON_RET_OK;
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	衛瞪縑憮 嬴檜蠱擊 っ衙и棻.
//	熱薑 橾濠	:	2002-11-13 螃�� 8:52:56 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
DWORD	SOVillage_Market::SellItem( SOPlayer *pPlayer, UI08 uiFollowerID, UI16 uiItemID, UI16 uiItemQuantity, SI32 siGuildRelation, MONEY *pmnItemPrice, MONEY *pmnCapital )
{
	MONEY				uiRealItemsPrice;	
	SI32				siTempCredit=0;	
	MARKET_ITEM			*pMarketItem;

	// 檜 葆擊縑憮 鏃晝ж朝 嬴檜蠱檣雖 匐餌и棻.
	if( ( pMarketItem = FindItem( uiItemID ) ) != NULL )
	{
		GetSellItemPrice( uiItemID, siGuildRelation, &uiRealItemsPrice );
		uiRealItemsPrice	*=	uiItemQuantity;		
	}	
	else
	{		
		uiRealItemsPrice	=	( m_pItem->GetItem( uiItemID )->m_siDBPrice * uiItemQuantity ) / 3;
	}

	// 憮幗曖 嬴檜蠱擊 隸陛衛鑑棻.
	IncreaseItemQuantity( uiItemID, uiItemQuantity, FALSE );

	*pmnItemPrice	=	uiRealItemsPrice;
	*pmnCapital		=	uiRealItemsPrice * ON_CAPITAL_RATE_TRADE;

	return	ON_RET_OK;
}

