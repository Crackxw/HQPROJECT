#include "SOMain.h"
#include "SODatabase.h"
#include "SOVillage_MarketStructure.h"

CItem			*SOVillage_Market::m_pItem;
cltIProfit		*SOVillage_Market::m_pIProfit;
SODatabase		*SOVillage_Market::m_pDataBase;

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	정적 변수를 초기화 한다.
//	수정 일자	:	2002-10-01 오후 2:01:42 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
VOID	SOVillage_Market::InitStaticVariable( CItem *pItem, cltIProfit *pIProfit, SODatabase *pDataBase )
{
	m_pItem			=	pItem;
	m_pIProfit		=	pIProfit;
	m_pDataBase		=	pDataBase;
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	생성자.
//	수정 일자	:	2002-09-30 오후 9:14:38 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
SOVillage_Market::SOVillage_Market()
{
	
}


//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	소멸자.
//	수정 일자	:	2002-09-30 오후 9:14:52 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
SOVillage_Market::~SOVillage_Market()
{

}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	초기화.
//	수정 일자	:	2002-10-01 오전 9:55:26 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
BOOL	SOVillage_Market::Init( UI16 uiVillageCode, UI32 uiItemsID[ ON_VILLAGE_MAX_ITEM ], SI32 siItemsAtb[ ON_VILLAGE_MAX_ITEM ], UI32 uiItemsQuantity[ ON_VILLAGE_MAX_ITEM ] )
{
	SI32				i;
	SI32				siItemInfo[ ON_VILLAGE_MAX_ITEM * 2 ];
//	DBIKVillageItem		VillageItem;
//	UI16				uiItemID;
//	UI16				uiItemQuantity;

	// 시전에 들어갈 수 있는 플레이어의 수를 SOVILLAGE_MAX_MAKRET_PLAYER_NUM로 제한한다.
	SOVillage_BaseStructure::Init( uiVillageCode, SOVILLAGE_MAX_MAKRET_PLAYER_NUM );

	// 초기화를 한다.
	ZeroMemory( m_SellItem, sizeof( m_SellItem ) );
	ZeroMemory( siItemInfo, sizeof( siItemInfo ) );

	// 시전에서 취급하는 아이템을 초기화 한다.
	for( i = 0; i < ON_VILLAGE_MAX_ITEM; i++ )
	{	
		// 화일 정보에서는 ID와 속성, 가격정보(변하지 않는것)만 넣어주면 된다, 수량은 DB에서 얻어와야 한다.
		m_SellItem[ i ].uiItemID		=	UI16( uiItemsID[ i ] );
		m_SellItem[ i ].uiItemAtb		=	UI08( siItemsAtb[ i ] );				
	}	

	// DB에서 아이템의 수량을 얻어와 설정 한다 	
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
			// DB에는 있지만 서버에 해당 아이템이 존재하지 않는다면 DB를 지운다.
			VillageItem.VillageCode			=	uiVillageCode;
			VillageItem.Slot				=	i;
			VillageItem.uiItemQuantity		=	0;
			VillageItem.ItemID				=	0;

			m_pDataBase->writeDBQuery( SO_DB_INFOKIND_SETVILLAGEITEM, &VillageItem, sizeof( VillageItem ) );
		}
	}*/

	/*
	// 텍스트에는 있지만 DB에 없는것들을 설정 한다 	
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


	// 아이템의 가격을 결정한다.
	UpdateAllItemsPrice();

	return	TRUE;
}


//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	메모리를 해제한다.
//	수정 일자	:	2002-09-30 오후 9:16:16 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
VOID	SOVillage_Market::Free()
{
	SOVillage_BaseStructure::Free();	
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	아이템의 수량을 설정한다.
//	수정 일자	:	2002-10-01 오전 10:05:06 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
BOOL	SOVillage_Market::SetItemQuantity( UI16 uiItemID, UI16 uiItemQuantity )
{
	MARKET_ITEM		*pMarketItem;		

	if( ( pMarketItem =	FindItem( uiItemID ) ) == NULL )	return	FALSE;

	pMarketItem->uiItemQuantity	=	uiItemQuantity;

	return	TRUE;
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	아이템의 수량을 얻어온다.
//	수정 일자	:	2002-10-01 오전 10:17:37 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
BOOL	SOVillage_Market::GetItemQuantity( UI16 uiItemID, UI16 *puiItemQuantity )
{
	MARKET_ITEM		*pMarketItem;		

	if( ( pMarketItem =	FindItem( uiItemID ) ) == NULL )	return	FALSE;

	*puiItemQuantity	=	pMarketItem->uiItemQuantity;

	return	TRUE;
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	아이템을 찾는다.
//	수정 일자	:	2002-10-01 오전 10:08:02 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
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

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	아이템의 ID를 알아온다.
//	수정 일자	:	2002-10-01 오전 10:24:53 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
BOOL	SOVillage_Market::GetItemID( SI32 siIndex, UI16 *puiItemID )
{
	if( IsValidItemIndex( siIndex ) == FALSE )			return	FALSE;

	if( m_SellItem[ siIndex ].uiItemID != 0)			return	FALSE;

	*puiItemID	=	m_SellItem[ siIndex ].uiItemID;

	return	TRUE;
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	유효한 인덱스인지 검사한다.
//	수정 일자	:	2002-10-01 오전 10:26:18 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
BOOL	SOVillage_Market::IsValidItemIndex( SI32 siIndex )
{
	if( siIndex >= 0 && siIndex < ON_VILLAGE_MAX_ITEM )	return	TRUE;
	else												return	FALSE;
}


//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	각 아이템의 가격을 결정한다.
//	수정 일자	:	2002-10-01 오전 10:35:29 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
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

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	아이템의 가격을 갱신한다.
//	수정 일자	:	2002-10-01 오전 11:19:42 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
BOOL	SOVillage_Market::UpdateItemPrice( UI16 uiItemID, MONEY *pItemBasePrice )
{
	MARKET_ITEM		*pMarketItem;				
	MONEY			mnItemBasePrice;

	if( ( pMarketItem =	FindItem( uiItemID ) ) == NULL )							return	FALSE;
	
	// 아이템의 가격을 얻어온다.
	GetItemBasePrice( uiItemID, &mnItemBasePrice );
	m_pIProfit->GetItemPrice( mnItemBasePrice, pMarketItem->uiItemQuantity, &pMarketItem->stItemPrices );

	if( pItemBasePrice != NULL )	*pItemBasePrice	=	mnItemBasePrice;

	return	TRUE;
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	아이템의 수량을 증가시킨다.
//	수정 일자	:	2002-10-01 오전 11:14:52 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
BOOL	SOVillage_Market::IncreaseItemQuantity( UI16 uiItemID, UI16 uiItemQuantity, BOOL bSendToDB )
{
	MARKET_ITEM		*pMarketItem;		
	MONEY			mnItemBasePrice;

	if( ( pMarketItem =	FindItem( uiItemID ) ) == NULL )							return	FALSE;

	pMarketItem->uiItemQuantity	=	min( USHRT_MAX, pMarketItem->uiItemQuantity + uiItemQuantity );	

	// 아이템의 가격 계산을 새로이 갱신한다.
	if( UpdateItemPrice( pMarketItem->uiItemID, &mnItemBasePrice ) == FALSE )		return	FALSE;																		

	SendChangedItemInfogMsgToPlayer( pMarketItem->uiItemID, pMarketItem->uiItemQuantity, mnItemBasePrice );

	// DB에 저장한다.
	if( bSendToDB == TRUE )
	{
		UpdateItemInfoInDB( pMarketItem->uiItemID, pMarketItem->uiItemQuantity );
	}

	return	TRUE;
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	아이템의 수량을 감소시킨다.
//	수정 일자	:	2002-10-01 오전 11:26:50 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
BOOL	SOVillage_Market::DecreaseItemQuantity( UI16 uiItemID, UI16 uiItemQuantity, BOOL bSendToDB )
{
	MARKET_ITEM		*pMarketItem;		
	MONEY			mnItemBasePrice;

	if( ( pMarketItem =	FindItem( uiItemID ) ) == NULL )				return	FALSE;

	// 시전에 있는 아이템보다 감소시키려는 아이템이 더 많은지 검사한다.
	if( pMarketItem->uiItemQuantity < uiItemQuantity )					return	FALSE;
	
	// 아이템을 감소시킨다.
	pMarketItem->uiItemQuantity		-=	uiItemQuantity;	

	// 아이템의 가격 계산을 새로이 갱신한다.
	if( UpdateItemPrice( uiItemID, &mnItemBasePrice ) == FALSE )		return	FALSE;																		

	SendChangedItemInfogMsgToPlayer( pMarketItem->uiItemID, pMarketItem->uiItemQuantity, mnItemBasePrice );

	// DB에 저장한다.
	if( bSendToDB == TRUE )
	{
		UpdateItemInfoInDB( pMarketItem->uiItemID, pMarketItem->uiItemQuantity );
	}

	return	TRUE;
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	판매하는 아이템의 리스트를 얻어온다.
//	수정 일자	:	2002-10-01 오전 11:32:12 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
SI16	SOVillage_Market::GetSellItemList( OnlineReceivedItemInfo *pItemInfo )
{
	SI32	i;
	SI16	siItemsNum;
	MONEY	mnItemBasePrice;
	
	for( siItemsNum = i = 0; i < ON_VILLAGE_MAX_ITEM; i++)
	{
		if( m_SellItem[ i ].uiItemID == 0 )															continue;
		if( GetItemBasePrice( m_SellItem[ i ].uiItemID, &mnItemBasePrice ) == FALSE )				continue;
	
		pItemInfo[ siItemsNum ].uiID			=	m_SellItem[ i ].uiItemID;			// 아이템의 ID.
		pItemInfo[ siItemsNum ].uiQuantity		=	m_SellItem[ i ].uiItemQuantity;		// 아이템의 수량.			
		pItemInfo[ siItemsNum ].BasePrice		=	mnItemBasePrice;					// 아이템의 기준 가격

		siItemsNum++;	
	}

	return siItemsNum;
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	아이템의 기준 가격을 얻어온다.
//	수정 일자	:	2002-10-01 오전 11:37:59 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
BOOL	SOVillage_Market::GetItemBasePrice( UI16 uiItemID, MONEY *pMoney )
{
	CItemHeader		*pItemHeader;	

	if( ( pItemHeader = m_pItem->GetItem( uiItemID ) ) == NULL)		return	FALSE;

	*pMoney			=		MONEY( pItemHeader->m_siDBPrice );

	return	TRUE;
}


//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	아이템을 구입할때의 가격을 얻어온다.
//	수정 일자	:	2002-10-01 오후 1:22:59 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
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

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	아이템을 판매할때의 가격을 얻어온다.
//	수정 일자	:	2002-10-01 오후 1:26:34 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
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

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	아이템을 구입 및 판매할때의 가격을 얻어온다.
//	수정 일자	:	2002-10-01 오후 3:39:45 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
BOOL	SOVillage_Market::GetBuySellItemPrice( UI16 uiItemID, MONEY *pBuyItemPrice, MONEY *pSellItemPrice )
{
	MARKET_ITEM		*pMarketItem;		
	BOOL			bSuccess = TRUE;

	if( ( pMarketItem =	FindItem( uiItemID ) ) == NULL )	return	FALSE;

	*pBuyItemPrice	=	pMarketItem->stItemPrices.Buy;
	*pSellItemPrice	=	pMarketItem->stItemPrices.Sell;
	
	return	bSuccess;	
}



//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	변경된 아이템 정보를 시전에 있는 플레이어에게 모두 보내준다.
//	수정 일자	:	2002-10-01 오후 1:44:08 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
VOID	SOVillage_Market::SendChangedItemInfogMsgToPlayer( UI16 uiItemID, UI16 uiItemQuantity, MONEY mnItemBasePrice )
{
	// 아이템이 변경되었다는 메시지를 보낸다.
	SendMsgToAllPlayer( ON_RESPONSE_CHANGED_ITEMINFO, LPVOID( uiItemID ), LPVOID( uiItemQuantity ), LPVOID( &mnItemBasePrice ) );
	
}


//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	DB에 있는 아이템의 정보를 갱신한다.
//	수정 일자	:	2002-10-01 오후 2:07:58 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
VOID	SOVillage_Market::UpdateItemInfoInDB( UI16 uiItemID, UI16 uiItemQuantity )
{
	DBIKVillageItemInfo		VillageItemInfo;	

	// DB에 변경된 아이템의 정보를 저장한다.
	VillageItemInfo.uiVillageCode	=	m_uiVillageCode;
	VillageItemInfo.siID			=	uiItemID;
	VillageItemInfo.uiQuantity		=	uiItemQuantity;				

	m_pDataBase->writeDBQuery( SO_DB_INFOKIND_UPDATEVILLAGEITEM, &VillageItemInfo, sizeof( DBIKVillageItemInfo ) );
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	아이템을 생산 및 소비한다
//	수정 일자	:	2002-10-01 오후 2:23:56 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
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

	// 현재 아이템의 정보를 얻어와서
	// 특산품이면 아이템을 증가시키고
	// 소모품이면 아이템을 감소시킨다.
	for(i = 0; i < ON_VILLAGE_MAX_ITEM; i++)	
	{
		// 유효한 아이템인지 체크한다.
		if( m_SellItem[ i ].uiItemID == 0 )			continue;

		switch( m_SellItem[ i ].uiItemAtb ) 
		{
		case ON_VILLAGE_PRODUCTION :			// 특산품이다.
			
			// 단 현재 60000개 미만 일때만.. 아이템을 추가 시켜 준다.
			if( m_SellItem[ i ].uiItemQuantity < 60000 )
			{			
				GetItemBasePrice( m_SellItem[ i ].uiItemID, &mnItemBasePrice );

				uiProduceItem	=	max( ( ( uiPopulation * siProduceValueOfItem ) / 10) / mnItemBasePrice, 1 );

				// 아이템을 증가시킨다.
				IncreaseItemQuantity( m_SellItem[ i ].uiItemID, uiProduceItem );
			}

			break;


		case ON_VILLAGE_CONSUMPTION :			// 소모품이다.

			// 단 현재 아이템이 12000개 이상일때만 삭제 해준다.
			if( m_SellItem[ i ].uiItemQuantity >= 12000)
			{
				GetItemBasePrice( m_SellItem[ i ].uiItemID, &mnItemBasePrice );

				uiReduceItem	=	max( ( ( uiPopulation * siReduceValueOfItem) / 10) / mnItemBasePrice, 1);

				// 아이템을 소모한다.
				DecreaseItemQuantity( m_SellItem[ i ].uiItemID, uiReduceItem);
			}

			break;

		}	
	}		
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	시전에서 아이템을 구입한다.
//	수정 일자	:	2002-10-21 오후 4:42:43 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
DWORD	SOVillage_Market::BuyItem( SOPlayer *pPlayer, UI08 uiFollowerID, UI16 uiItemID, UI16 uiItemQuantity, SI16 siPosInInventory, SI32 siGuildRelation, MONEY *pmnItemPrice, MONEY *pmnCapital )
{
	MONEY				uiRealItemsPrice;		
	MARKET_ITEM			*pMarketItem;		

	// 아이템을 얻어온다.
	if( ( pMarketItem = FindItem( uiItemID ) ) == NULL )							return	ON_RET_NOSALE;
	
	// 사려고 하는 만큼의 아이템 수가 있는지 검사한다.
	if( pMarketItem->uiItemQuantity < uiItemQuantity )								return	ON_RET_NOITEM;	
	
	// 현재 돈으로 그 아이템을 제대로 살 수 있는지 판단한다.		
	GetBuyItemPrice( uiItemID, siGuildRelation, &uiRealItemsPrice );
	uiRealItemsPrice	*=	uiItemQuantity;	

	// 아이템을 살 수 있을 만큼의 돈을 가지고 있는지 검사한다.
	if( pPlayer->EnoughMoney( uiRealItemsPrice ) == FALSE )							return	ON_RET_NOMONEY;		
	
	// 서버의 아이템을 감소시킨다.								
	DecreaseItemQuantity( uiItemID, uiItemQuantity, FALSE );

	*pmnItemPrice	=	uiRealItemsPrice;
	*pmnCapital		=	uiRealItemsPrice * ON_CAPITAL_RATE_TRADE;	

	return	ON_RET_OK;
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	시전에서 아이템을 판매한다.
//	수정 일자	:	2002-11-13 오후 8:52:56 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
DWORD	SOVillage_Market::SellItem( SOPlayer *pPlayer, UI08 uiFollowerID, UI16 uiItemID, UI16 uiItemQuantity, SI32 siGuildRelation, MONEY *pmnItemPrice, MONEY *pmnCapital )
{
	MONEY				uiRealItemsPrice;	
	SI32				siTempCredit=0;	
	MARKET_ITEM			*pMarketItem;

	// 이 마을에서 취급하는 아이템인지 검사한다.
	if( ( pMarketItem = FindItem( uiItemID ) ) != NULL )
	{
		GetSellItemPrice( uiItemID, siGuildRelation, &uiRealItemsPrice );
		uiRealItemsPrice	*=	uiItemQuantity;		
	}	
	else
	{		
		uiRealItemsPrice	=	( m_pItem->GetItem( uiItemID )->m_siDBPrice * uiItemQuantity ) / 3;
	}

	// 서버의 아이템을 증가시킨다.
	IncreaseItemQuantity( uiItemID, uiItemQuantity, FALSE );

	*pmnItemPrice	=	uiRealItemsPrice;
	*pmnCapital		=	uiRealItemsPrice * ON_CAPITAL_RATE_TRADE;

	return	ON_RET_OK;
}

