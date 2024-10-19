#include "SOMain.h"
#include "SOWorld.h"
#include "SOVillage.h"

//¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬
//	¼³¸í		:	½ÃÀå¿¡¼­ ¹°°ÇÀ» »ê´Ù.
//	¼öÁ¤ ÀÏÀÚ	:	2002-11-13 ¿ÀÈÄ 4:50:44 - ¾çÁ¤¸ð
//¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬
DWORD	SOVillage::BuyItemInMarket( SOPlayer *pPlayer, UI08 uiFollowerID, UI16 uiItemID, UI16 uiItemQuantity, SI16 siPosInInventory )
{
	SI32			siGuildRelation = ON_GUILD_RELATION_NEUTRAL;
	DWORD			dwResponse;
	MONEY			mnCapital;
	MONEY			mnItemPrice;
	SI32			siTempCredit = 0, siIncreasedCredit = 0;
	MONEY			mnIncreasedCapital;
	MyItemData		*pMyItemData;
	CHAR			szDBArg[ 1024 ];

	// ÀÌ ¸¶À»¿¡ ½ÃÀüÀÌ ÀÖ´ÂÁö °Ë»çÇÑ´Ù.
	if( DoesMarketExist() == FALSE )					return	ON_RET_NO;

	// ÇÃ·¹ÀÌ¾î¿Í ÀÌ ¸¶À»ÀÇ »ó´Ü°£ÀÇ °ü°è¸¦ ¼³Á¤ÇÑ´Ù.
	// ±æµå ¸â¹öÀÎÁö ¾Ë¾Æ¿Â´Ù.
	if( ( uiGuildID != 0 ) && ( pPlayer->uiGuildID != 0 ) && ( pPlayer->siClassInGuild != clWorld->clIGuildClass.GetJWJCode() ) )
	{
		if( uiGuildID == pPlayer->uiGuildID )
		{
			siGuildRelation		=	ON_GUILD_RELATION_MINE;		// ÀÌ ¸¶À»ÀÇ »ó´Ü¿øÀÌ´Ù.
		}
		else if( clWorld->clIGuild.IsHostilityRelation( pPlayer->uiGuildID, uiGuildID ) == TRUE )	
		{
			siGuildRelation		=	ON_GUILD_RELATION_ENEMY;	// Àû´ëÀûÀÎ ¸¶À»ÀÇ »ó´ÜÀÌ´Ù.
		}
		else if( clWorld->clIGuild.IsEnemyRelationWar( pPlayer->uiGuildID, uiGuildID ) == TRUE )	
		{
			siGuildRelation		=	ON_GUILD_RELATION_WAR;	// °ø¼º ÇÑ´Ù.
		}
	}

	dwResponse	=	m_pMarket->BuyItem( pPlayer, uiFollowerID, uiItemID, uiItemQuantity, siPosInInventory, siGuildRelation, &mnItemPrice, &mnCapital );

	if( dwResponse != ON_RET_OK )						return	dwResponse;	

	// »ç¿ëÀÚÀÇ µ·À» ±ð´Â´Ù.
	pPlayer->DecreaseMoney( mnItemPrice, FALSE );

	// ÇÃ·¹ÀÌ¾îÀÇ ½Å¿ëµµ¸¦ ¿Ã·ÁÁØ´Ù.
	if( ( siTempCredit = pPlayer->GetGiveCredit( mnItemPrice ) ) > 0 )			siIncreasedCredit = pPlayer->IncreaseTradeCredit( siTempCredit, FALSE );

	// ±×¸®°í ÇÃ·¹ÀÌ¾î ÀÎº¥Åä¸®¿¡ ¾ÆÀÌÅÛÀ» Ãß°¡ÇØÁØ´Ù.
	pPlayer->IFollower.AddItem( uiFollowerID, uiItemID, uiItemQuantity, siPosInInventory );
	pMyItemData			=	pPlayer->GetItem( uiFollowerID, uiItemID );

	// ¸¶À»ÀÇ ¼öÀÍÀ» ¿Ã·ÁÁØ´Ù.
	mnIncreasedCapital	=	IncreaseCapital( mnCapital, ON_PROFIT_TRADE, FALSE );

	// Å¬¶óÀÌ¾ðÆ®¿¡°Ô ¼º°øÇß´Ù°í ¸Þ½ÃÁö¸¦ º¸³»ÁØ´Ù.
	pPlayer->SendFieldMsg( ON_RESPONSE_BUY_ITEM, SO_SFM_ONLYME, LPVOID( ON_RET_OK ), LPVOID( &pPlayer->siMoney ), LPVOID( pPlayer->uiTradeCredit ) );	

	// DB¿¡ ÀúÀåÇÑ´Ù.
	ZeroMemory( szDBArg, sizeof( szDBArg ) );
	sprintf( szDBArg, "%d, %d, %d, %d, %u, %u, %u, %I64d, %I64d, %d", 	pPlayer->stAccountInfo.siDBAccount, pPlayer->stAccountInfo.uiSlot,
																		uiFollowerID, pMyItemData->siPosInInventory, uiItemID, uiItemQuantity, 
																		uiVillageCode, mnItemPrice, mnIncreasedCapital, siIncreasedCredit );
	clWorld->clDatabase->writeDBQuery( SO_DB_KIND_BUYITEMINMARKET, LPVOID( szDBArg ), strlen( szDBArg ) + 1 );

	return	ON_RET_OK;
}

//¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬
//	¼³¸í		:	½ÃÀå¿¡ ¹°°ÇÀ» ÆÇ´Ù.	
//	¼öÁ¤ ÀÏÀÚ	:	2002-11-13 ¿ÀÈÄ 8:51:02 - ¾çÁ¤¸ð
//¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬
DWORD	SOVillage::SellItemInMarket( SOPlayer *pPlayer, UI08 uiFollowerID, UI16 uiItemID, UI16 uiItemQuantity )
{
	SI32			siGuildRelation = ON_GUILD_RELATION_NEUTRAL;
	DWORD			dwResponse;
	MONEY			mnCapital;
	MONEY			mnItemPrice;
	SI32			siTempCredit = 0, siIncreasedCredit = 0;
	MONEY			mnIncreasedCapital;
	CHAR			szDBArg[ 1024 ];
	MONEY			mnIncreasedMoney;
	SI16			siItemPosInInventory;

	// ÀÌ ¸¶À»¿¡ ½ÃÀüÀÌ ÀÖ´ÂÁö °Ë»çÇÑ´Ù.
	if( DoesMarketExist() == FALSE )					return	ON_RET_NO;

	// ÇÃ·¹ÀÌ¾î¿Í ÀÌ ¸¶À»ÀÇ »ó´Ü°£ÀÇ °ü°è¸¦ ¼³Á¤ÇÑ´Ù.
	// ±æµå ¸â¹öÀÎÁö ¾Ë¾Æ¿Â´Ù.
	if( ( uiGuildID != 0 ) && ( pPlayer->uiGuildID != 0 ) && ( pPlayer->siClassInGuild != clWorld->clIGuildClass.GetJWJCode() ) )
	{
		if( uiGuildID == pPlayer->uiGuildID )
		{
			siGuildRelation		=	ON_GUILD_RELATION_MINE;		// ÀÌ ¸¶À»ÀÇ »ó´Ü¿øÀÌ´Ù.
		}
		else if( clWorld->clIGuild.IsHostilityRelation( pPlayer->uiGuildID, uiGuildID ) == TRUE )	
		{
			siGuildRelation		=	ON_GUILD_RELATION_ENEMY;	// Àû´ëÀûÀÎ ¸¶À»ÀÇ »ó´ÜÀÌ´Ù.
		}
		else if( clWorld->clIGuild.IsEnemyRelationWar( pPlayer->uiGuildID, uiGuildID ) == TRUE )	
		{
			siGuildRelation		=	ON_GUILD_RELATION_WAR;	// Àû´ëÀûÀÎ ¸¶À»ÀÇ »ó´ÜÀÌ´Ù.
		}

	}

	dwResponse	=	m_pMarket->SellItem( pPlayer, uiFollowerID, uiItemID, uiItemQuantity, siGuildRelation, &mnItemPrice, &mnCapital );

	if( dwResponse != ON_RET_OK )						return	dwResponse;

	// »ç¿ëÀÚ¿¡°Ô µ·À» ³Ö´Â´Ù.
	mnIncreasedMoney	=	pPlayer->IncreaseMoney( mnItemPrice, FALSE );

	// ÇÃ·¹ÀÌ¾îÀÇ ½Å¿ëµµ¸¦ ¿Ã·ÁÁØ´Ù.
	if( ( siTempCredit = pPlayer->GetGiveCredit( mnItemPrice ) ) > 0 )			siIncreasedCredit = pPlayer->IncreaseTradeCredit( siTempCredit, FALSE );	

	// ±×¸®°í ÇÃ·¹ÀÌ¾î ÀÎº¥Åä¸®¿¡¼­ ¾ÆÀÌÅÛÀ» »èÁ¦ÇÑ´Ù.
	pPlayer->IFollower.DelItem( uiFollowerID, uiItemID, uiItemQuantity, &siItemPosInInventory );		

	// ¸¶À»ÀÇ ¼öÀÍÀ» ¿Ã·ÁÁØ´Ù.
	mnIncreasedCapital	=	IncreaseCapital( mnCapital, ON_PROFIT_TRADE, FALSE );

	// Å¬¶óÀÌ¾ðÆ®¿¡°Ô ¼º°øÇß´Ù°í ¸Þ½ÃÁö¸¦ º¸³»ÁØ´Ù.
	pPlayer->SendFieldMsg( ON_RESPONSE_SELL_ITEM, SO_SFM_ONLYME, LPVOID( ON_RET_OK ), LPVOID( &pPlayer->siMoney ), LPVOID( pPlayer->uiTradeCredit ) );	

	// DB¿¡ ÀúÀåÇÑ´Ù.
	ZeroMemory( szDBArg, sizeof( szDBArg ) );
	sprintf( szDBArg, "%d, %d, %d, %d, %u, %u, %u, %I64d, %I64d, %d", 	pPlayer->stAccountInfo.siDBAccount, pPlayer->stAccountInfo.uiSlot,
																		uiFollowerID, siItemPosInInventory, uiItemID, uiItemQuantity, 
																		uiVillageCode, mnIncreasedMoney, mnIncreasedCapital, siIncreasedCredit );
	clWorld->clDatabase->writeDBQuery( SO_DB_KIND_SELLITEMINMARKET, LPVOID( szDBArg ), strlen( szDBArg ) + 1 );

	return	ON_RET_OK;
}

/*
//¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬
//	¼³¸í		:	½ÃÀå ¾ÈÀ¸·Î µé¾î°£´Ù.
//	¼öÁ¤ ÀÏÀÚ	:	2002-09-30 ¿ÀÈÄ 8:28:28 - ¾çÁ¤¸ð
//¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬
BOOL	SOVillage::EnterMarket( SOPlayer *pPlayer )
{
	if( m_pMarket->EnterPlayer( pPlayer ) == FALSE )	return	FALSE;
	
	pPlayer->SendFieldMsg( ON_RESPONSE_ENTER_MARKET, SO_SFM_ONLYME, LPVOID( ON_RET_MARKET_OK ) );
	
	return	TRUE;	


}
*/
/*
//¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬
//	¼³¸í		:	½ÃÀå¿¡¼­ ³ª°£´Ù.
//	¼öÁ¤ ÀÏÀÚ	:	2002-09-30 ¿ÀÈÄ 9:40:49 - ¾çÁ¤¸ð
//¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬
BOOL	SOVillage::LeaveMarket( SOPlayer *pPlayer )
{	
	if( m_pMarket->LeavePlayer( pPlayer ) == FALSE )	return	FALSE;
		
	

	return	TRUE;

	


	return FALSE;

}	*/
/*

//¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬
//	¼³¸í		:	¾ÆÀÌÅÛÀ» »ç°í ÆÈ¶§ÀÇ °¡°ÝÀ» ¾ò¾î¿Â´Ù. ÀÏ¹Ý °¡°Ý¸¸ ¾ò¾î¿Â´Ù
//	¼öÁ¤ ÀÏÀÚ	:	2002-04-24 ¿ÀÀü 9:56:26 - ¾çÁ¤¸ð
//¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬
BOOL	SOVillage::GetItemPrice(UI16 uiItemCode, MONEY *pmnBuy, MONEY *pmnSell)
{
	SI32	siIndex;

	siIndex = FindItem(uiItemCode);

	if(siIndex >= 0)
	{		
		*pmnBuy		=	stMarketItem[siIndex].stItemPrices.Buy;
		*pmnSell	=	stMarketItem[siIndex].stItemPrices.Sell;

		return	TRUE;
	}	
	
	return	FALSE;	
}
*/
/*
//¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬
// ¼ö·®¿¡ ÀÇÇÑ ¾ÆÀÌÅÛÀÇ ½ÇÁ¦ °¡°ÝÀ» ±¸ÇÑ´Ù.
//¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬
SI32 SOVillage::GetTotalValue(UI16 uiTempItemCode, UI16 uiTempQuantity)
{
	return 	GetItemPrice(uiTempItemCode) * (UI32)uiTempQuantity;	// ¹°°¡¿¡µû¸¥ ¾ÆÀÌÅÛÀÇ °¡°ÝÀ» ±¸ÇÑ´Ù.	
}
*/


/*
//----------------------------------------------------------------------------------------------------------------
//	¼³¸í	:	ÇöÀç ÀÌ ¸¶À»¿¡¼­ Ãë±ÞÇÏ´Â ¾ÆÀÌÅÛÀÎ°¡?
//----------------------------------------------------------------------------------------------------------------
BOOL	SOVillage::IsSaleItem(UI16 uiID)
{
	if(FindItem(uiID) != -1)
		return TRUE;
	else
		return FALSE;
}

*/