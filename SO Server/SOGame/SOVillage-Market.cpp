#include "SOMain.h"
#include "SOWorld.h"
#include "SOVillage.h"

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	衛濰縑憮 僭勒擊 骯棻.
//	熱薑 橾濠	:	2002-11-13 螃�� 4:50:44 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
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

	// 檜 葆擊縑 衛瞪檜 氈朝雖 匐餌и棻.
	if( DoesMarketExist() == FALSE )					return	ON_RET_NO;

	// Ы溯檜橫諦 檜 葆擊曖 鼻欽除曖 婦啗蒂 撲薑и棻.
	// 望萄 詹幗檣雖 憲嬴螞棻.
	if( ( uiGuildID != 0 ) && ( pPlayer->uiGuildID != 0 ) && ( pPlayer->siClassInGuild != clWorld->clIGuildClass.GetJWJCode() ) )
	{
		if( uiGuildID == pPlayer->uiGuildID )
		{
			siGuildRelation		=	ON_GUILD_RELATION_MINE;		// 檜 葆擊曖 鼻欽錳檜棻.
		}
		else if( clWorld->clIGuild.IsHostilityRelation( pPlayer->uiGuildID, uiGuildID ) == TRUE )	
		{
			siGuildRelation		=	ON_GUILD_RELATION_ENEMY;	// 瞳渠瞳檣 葆擊曖 鼻欽檜棻.
		}
		else if( clWorld->clIGuild.IsEnemyRelationWar( pPlayer->uiGuildID, uiGuildID ) == TRUE )	
		{
			siGuildRelation		=	ON_GUILD_RELATION_WAR;	// 奢撩 и棻.
		}
	}

	dwResponse	=	m_pMarket->BuyItem( pPlayer, uiFollowerID, uiItemID, uiItemQuantity, siPosInInventory, siGuildRelation, &mnItemPrice, &mnCapital );

	if( dwResponse != ON_RET_OK )						return	dwResponse;	

	// 餌辨濠曖 絲擊 梗朝棻.
	pPlayer->DecreaseMoney( mnItemPrice, FALSE );

	// Ы溯檜橫曖 褐辨紫蒂 螢溥遽棻.
	if( ( siTempCredit = pPlayer->GetGiveCredit( mnItemPrice ) ) > 0 )			siIncreasedCredit = pPlayer->IncreaseTradeCredit( siTempCredit, FALSE );

	// 斜葬堅 Ы溯檜橫 檣漸饜葬縑 嬴檜蠱擊 蹺陛п遽棻.
	pPlayer->IFollower.AddItem( uiFollowerID, uiItemID, uiItemQuantity, siPosInInventory );
	pMyItemData			=	pPlayer->GetItem( uiFollowerID, uiItemID );

	// 葆擊曖 熱櫛擊 螢溥遽棻.
	mnIncreasedCapital	=	IncreaseCapital( mnCapital, ON_PROFIT_TRADE, FALSE );

	// 贗塭檜樹お縑啪 撩奢ц棻堅 詭衛雖蒂 爾頂遽棻.
	pPlayer->SendFieldMsg( ON_RESPONSE_BUY_ITEM, SO_SFM_ONLYME, LPVOID( ON_RET_OK ), LPVOID( &pPlayer->siMoney ), LPVOID( pPlayer->uiTradeCredit ) );	

	// DB縑 盪濰и棻.
	ZeroMemory( szDBArg, sizeof( szDBArg ) );
	sprintf( szDBArg, "%d, %d, %d, %d, %u, %u, %u, %I64d, %I64d, %d", 	pPlayer->stAccountInfo.siDBAccount, pPlayer->stAccountInfo.uiSlot,
																		uiFollowerID, pMyItemData->siPosInInventory, uiItemID, uiItemQuantity, 
																		uiVillageCode, mnItemPrice, mnIncreasedCapital, siIncreasedCredit );
	clWorld->clDatabase->writeDBQuery( SO_DB_KIND_BUYITEMINMARKET, LPVOID( szDBArg ), strlen( szDBArg ) + 1 );

	return	ON_RET_OK;
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	衛濰縑 僭勒擊 っ棻.	
//	熱薑 橾濠	:	2002-11-13 螃�� 8:51:02 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
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

	// 檜 葆擊縑 衛瞪檜 氈朝雖 匐餌и棻.
	if( DoesMarketExist() == FALSE )					return	ON_RET_NO;

	// Ы溯檜橫諦 檜 葆擊曖 鼻欽除曖 婦啗蒂 撲薑и棻.
	// 望萄 詹幗檣雖 憲嬴螞棻.
	if( ( uiGuildID != 0 ) && ( pPlayer->uiGuildID != 0 ) && ( pPlayer->siClassInGuild != clWorld->clIGuildClass.GetJWJCode() ) )
	{
		if( uiGuildID == pPlayer->uiGuildID )
		{
			siGuildRelation		=	ON_GUILD_RELATION_MINE;		// 檜 葆擊曖 鼻欽錳檜棻.
		}
		else if( clWorld->clIGuild.IsHostilityRelation( pPlayer->uiGuildID, uiGuildID ) == TRUE )	
		{
			siGuildRelation		=	ON_GUILD_RELATION_ENEMY;	// 瞳渠瞳檣 葆擊曖 鼻欽檜棻.
		}
		else if( clWorld->clIGuild.IsEnemyRelationWar( pPlayer->uiGuildID, uiGuildID ) == TRUE )	
		{
			siGuildRelation		=	ON_GUILD_RELATION_WAR;	// 瞳渠瞳檣 葆擊曖 鼻欽檜棻.
		}

	}

	dwResponse	=	m_pMarket->SellItem( pPlayer, uiFollowerID, uiItemID, uiItemQuantity, siGuildRelation, &mnItemPrice, &mnCapital );

	if( dwResponse != ON_RET_OK )						return	dwResponse;

	// 餌辨濠縑啪 絲擊 厥朝棻.
	mnIncreasedMoney	=	pPlayer->IncreaseMoney( mnItemPrice, FALSE );

	// Ы溯檜橫曖 褐辨紫蒂 螢溥遽棻.
	if( ( siTempCredit = pPlayer->GetGiveCredit( mnItemPrice ) ) > 0 )			siIncreasedCredit = pPlayer->IncreaseTradeCredit( siTempCredit, FALSE );	

	// 斜葬堅 Ы溯檜橫 檣漸饜葬縑憮 嬴檜蠱擊 餉薯и棻.
	pPlayer->IFollower.DelItem( uiFollowerID, uiItemID, uiItemQuantity, &siItemPosInInventory );		

	// 葆擊曖 熱櫛擊 螢溥遽棻.
	mnIncreasedCapital	=	IncreaseCapital( mnCapital, ON_PROFIT_TRADE, FALSE );

	// 贗塭檜樹お縑啪 撩奢ц棻堅 詭衛雖蒂 爾頂遽棻.
	pPlayer->SendFieldMsg( ON_RESPONSE_SELL_ITEM, SO_SFM_ONLYME, LPVOID( ON_RET_OK ), LPVOID( &pPlayer->siMoney ), LPVOID( pPlayer->uiTradeCredit ) );	

	// DB縑 盪濰и棻.
	ZeroMemory( szDBArg, sizeof( szDBArg ) );
	sprintf( szDBArg, "%d, %d, %d, %d, %u, %u, %u, %I64d, %I64d, %d", 	pPlayer->stAccountInfo.siDBAccount, pPlayer->stAccountInfo.uiSlot,
																		uiFollowerID, siItemPosInInventory, uiItemID, uiItemQuantity, 
																		uiVillageCode, mnIncreasedMoney, mnIncreasedCapital, siIncreasedCredit );
	clWorld->clDatabase->writeDBQuery( SO_DB_KIND_SELLITEMINMARKET, LPVOID( szDBArg ), strlen( szDBArg ) + 1 );

	return	ON_RET_OK;
}

/*
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	衛濰 寰戲煎 菟橫除棻.
//	熱薑 橾濠	:	2002-09-30 螃�� 8:28:28 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
BOOL	SOVillage::EnterMarket( SOPlayer *pPlayer )
{
	if( m_pMarket->EnterPlayer( pPlayer ) == FALSE )	return	FALSE;
	
	pPlayer->SendFieldMsg( ON_RESPONSE_ENTER_MARKET, SO_SFM_ONLYME, LPVOID( ON_RET_MARKET_OK ) );
	
	return	TRUE;	


}
*/
/*
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	衛濰縑憮 釭除棻.
//	熱薑 橾濠	:	2002-09-30 螃�� 9:40:49 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
BOOL	SOVillage::LeaveMarket( SOPlayer *pPlayer )
{	
	if( m_pMarket->LeavePlayer( pPlayer ) == FALSE )	return	FALSE;
		
	

	return	TRUE;

	


	return FALSE;

}	*/
/*

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	嬴檜蠱擊 餌堅 つ陽曖 陛問擊 橢橫螞棻. 橾奩 陛問虜 橢橫螞棻
//	熱薑 橾濠	:	2002-04-24 螃瞪 9:56:26 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
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
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
// 熱榆縑 曖и 嬴檜蠱曖 褒薯 陛問擊 掘и棻.
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
SI32 SOVillage::GetTotalValue(UI16 uiTempItemCode, UI16 uiTempQuantity)
{
	return 	GetItemPrice(uiTempItemCode) * (UI32)uiTempQuantity;	// 僭陛縑評艇 嬴檜蠱曖 陛問擊 掘и棻.	
}
*/


/*
//----------------------------------------------------------------------------------------------------------------
//	撲貲	:	⑷營 檜 葆擊縑憮 鏃晝ж朝 嬴檜蠱檣陛?
//----------------------------------------------------------------------------------------------------------------
BOOL	SOVillage::IsSaleItem(UI16 uiID)
{
	if(FindItem(uiID) != -1)
		return TRUE;
	else
		return FALSE;
}

*/