#include "SOMain.h"
#include "SODefine.h"
#include "SOWorld.h"
#include "SOPlayer.h"
#include "OnlineCommonStruct.h"

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
// 葆擊縑憮 集陪棻.
//
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
VOID	SOPlayer::RecvLeaveVillage()
{	
	UI16							uiCharFItemInfo[2];			// 0廓擎 議葛攪 熱, 1廓擎 嬴檜蠱 熱,
	UI16							uiCharUniIDs[ON_STATUS_AREA_XSIZE * ON_STATUS_AREA_YSIZE];	
	OnlineReceiveFieldItemInfo		FieldItemInfo[ON_STATUS_AREA_XSIZE * ON_STATUS_AREA_YSIZE];
	POINT							ptPlayerPos;	
	BOOL							bSuccess = FALSE;
	
	// ⑷營 葆擊縑 氈朝陛?
	if( stAccountInfo.pVillage == NULL )								goto	END;

	// 辦摹 裘縑憮 釭螢 謝ル蒂 瓊朝棻. 
	// 葆擊曖 嬪纂蒂 晦遽戲煎 裘縑 褕擊 謝ル蒂 橢橫螞棻.
	ptPlayerPos		=	pMyWorld->clISOMap.SearchMap( stAccountInfo.pVillage->uiMapID,	
													stAccountInfo.pVillage->siVillageX, 
													stAccountInfo.pVillage->siVillageY, 
													ON_MAP_SI_CHAR, uiMoveableTileInfo );		

	// 葆擊縑憮 釭螢 夠擊 瓊朝等 褒ぬц朝雖 匐餌и棻.
	if( ( ptPlayerPos.x == -1 ) && ( ptPlayerPos.y == -1 ) )			goto	END;


	// 釭螃朝 葆擊曖 謝ル煎 熱薑и棻.
	if( ChangeMap( stAccountInfo.pVillage->uiMapID, SI16( ptPlayerPos.x ), SI16( ptPlayerPos.y ) ) == FALSE )	goto	END;

	
	// 裘縑 議葛攪 ID蒂 撲薑и棻.
	if( SetIDOnMap( 11 ) == FALSE )			
	{
		// 裘縑 議葛攪蒂 撲薑ж朝等 褒ぬц棻.

//		pMyWorld->clServer->WriteInfo("..\\SetIDOnMap Failed.txt", "8 [ %u ], [ %d ], [ %d ]", uiMapID, siX, siY );
		writeInfoToFile("SetIDOnMap Failed.txt", "8 [ %u ], [ %d ], [ %d ]", uiMapID, siX, siY );
		goto	END;
	}

	// 葆擊擊 緒螳 釭螞棻.
	LeaveVillage();
	
	bSuccess	=	TRUE;

END:

	if( bSuccess == TRUE )
	{
		// 葆擊縑憮 撩奢瞳戲煎 釭陞 熱 氈棻.

		uiCharFItemInfo[0]		=	pMyWorld->clISOMap.GetNearSectorPlayerList(uiMapID, siX, siY, uiCharUniIDs, ON_MAP_CHARID_FIELD);
		uiCharFItemInfo[1]		=	pMyWorld->clISOMap.GetNearSectorItemList(uiMapID, siX, siY, FieldItemInfo);		
		

		// 釭縑啪 頂 輿嬪曖 議葛攪諦 嬴檜蠱曖 薑爾蒂 爾頂遽棻.
		SendFieldMsg( ON_RESPONSE_LEAVE_VILLAGE, SO_SFM_ONLYME, LPVOID( ON_RET_LEAVE_VILLAGE_OK ), LPVOID( uiCharFItemInfo ), 
						LPVOID( uiCharUniIDs ), LPVOID( FieldItemInfo ) );
	}
	else
	{
		// 葆擊縑憮 釭陛朝等 褒ぬц棻.

		SendFieldMsg(ON_RESPONSE_LEAVE_VILLAGE, SO_SFM_ONLYME, LPVOID(ON_RET_LEAVE_VILLAGE_NO));

	}


}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	鼻薄縑憮 嬴檜蠱曖 葬蝶お蒂 蹂掘и棻.
//	熱薑 橾濠	:	2002-10-01 螃瞪 9:49:03 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
VOID	SOPlayer::RecvListItem()
{
	SI32					siItemNum;
	OnlineReceivedItemInfo	ItemsInfo[ ON_VILLAGE_MAX_ITEM ];
	
	// 葆擊縑憮 っ衙ж朝 嬴檜蠱曖 跡煙擊 橢橫螞棻.
	siItemNum	=	stAccountInfo.pVillage->m_pMarket->GetSellItemList( ItemsInfo );			

	SendFieldMsg( ON_RESPONSE_LIST_ITEM, SO_SFM_ONLYME, LPVOID( siItemNum ), LPVOID( ItemsInfo ) );
}

//----------------------------------------------------------------------------------------------------------------
//	撲貲		:	嬴檜蠱擊 骯棻.
//	熱薑 橾濠	:	2002-02-27 螃�� 2:03:04 - 曄薑賅
//----------------------------------------------------------------------------------------------------------------
VOID SOPlayer::RecvBuyItem()
{
	OnReqBuyItemMsg		*pReqBuyItemMsg;
	DWORD				dwResponse = ON_RET_NO;
	CItemHeader			*pItem;

	pReqBuyItemMsg		=	( OnReqBuyItemMsg* )cMsg;
	
	if( pReqBuyItemMsg->uiItemQuantity == 0 )												goto	END;
	if( pReqBuyItemMsg->uiItemQuantity > ON_MARKET_MAXQUANTITY )							goto	END;

	if( ( pItem = pMyWorld->clItem.GetItem( pReqBuyItemMsg->uiItemID ) ) == NULL )			goto	END;
	if( pItem->m_uiTradeGrade > GetTradeGrade() )											goto	END;
	if( IFollower.CanAddItem( pReqBuyItemMsg->uiFollowerID, 
								pReqBuyItemMsg->uiItemID, 
								pReqBuyItemMsg->uiItemQuantity ) == FALSE )					goto	END;
	

	dwResponse = stAccountInfo.pVillage->BuyItemInMarket( this, pReqBuyItemMsg->uiFollowerID, pReqBuyItemMsg->uiItemID, 
															pReqBuyItemMsg->uiItemQuantity, pReqBuyItemMsg->uiPosInInventory );


END:
	
	if( dwResponse != ON_RET_OK )
	{
		SendFieldMsg( ON_RESPONSE_BUY_ITEM, SO_SFM_ONLYME, LPVOID( dwResponse ) );	
	}

}

//----------------------------------------------------------------------------------------------------------------
//	撲貲		:	嬴檜蠱擊 っ棻.
//	熱薑 橾濠	:	2002-03-02 螃瞪 11:02:18 - 曄薑賅
//----------------------------------------------------------------------------------------------------------------
VOID	SOPlayer::RecvSellItem()
{
	OnReqSellItemMsg	*pReqSellItemMsg;
	DWORD				dwResponse = ON_RET_NO;
	CItemHeader			*pItem;

	pReqSellItemMsg		=	( OnReqSellItemMsg* )cMsg;	
	
	if( pReqSellItemMsg->uiItemQuantity == 0 )												goto	END;
	if( pReqSellItemMsg->uiItemQuantity > ON_MARKET_MAXQUANTITY )							goto	END;

	if( ( pItem = pMyWorld->clItem.GetItem( pReqSellItemMsg->uiItemID ) ) == NULL )			goto	END;		
	if( IFollower.HasItem( pReqSellItemMsg->uiFollowerID, 
							pReqSellItemMsg->uiItemID, 
							pReqSellItemMsg->uiItemQuantity ) == FALSE )					goto	END;
	

	dwResponse = stAccountInfo.pVillage->SellItemInMarket( this, pReqSellItemMsg->uiFollowerID, pReqSellItemMsg->uiItemID, 
															pReqSellItemMsg->uiItemQuantity );

END:
	
	if( dwResponse != ON_RET_OK )
	{
		SendFieldMsg( ON_RESPONSE_SELL_ITEM, SO_SFM_ONLYME, LPVOID( dwResponse ) );	
	}

/*
	OnReqSellItemMsg	*pReqSellItemMsg;
	MyItemData			*pPlayerItem;
	BOOL				bSuccess = FALSE;
	MONEY				uiRealItemsPrice;
	SI32				siTempCredit= 0;
	BOOL				siGuildRelation = ON_GUILD_RELATION_NEUTRAL;

	pReqSellItemMsg		=	(OnReqSellItemMsg*)cMsg;	

	// 釭縑啪 薑蜓 檜 嬴檜蠱菟檜 蹂掘и 熱榆虜躑 氈朝雖 匐餌и棻.	
	pPlayerItem		=	IFollower.GetItem(pReqSellItemMsg->uiFollowerID, pReqSellItemMsg->uiItemID, pReqSellItemMsg->uiPosInInventory);

	// 斜 辨煽縑啪朝 斜 檣漸饜葬 嬪纂縑 斜 嬴檜蠱檜 橈棻.
	if(pPlayerItem == NULL)		goto	END;

	// つ溥堅 ж朝 嬴檜蠱 爾棻 陛雖堅 氈朝 嬴檜蠱檜 號剪釭 偽擎雖 匐餌и棻.
	if(pPlayerItem->uiQuantity < pReqSellItemMsg->uiItemQuantity)	goto	END;

	// и廓縑 つ 熱 氈朝 熱榆擊 薯ии棻.
	if( pReqSellItemMsg->uiItemQuantity > ON_MARKET_MAXQUANTITY )	goto	END;


	// つ溥朝 嬴檜蠱檜 檜 衛濰縑憮 鏃晝ж朝 僭ヶ檣雖 匐餌и棻.	
	if( stAccountInfo.pVillage->m_pMarket->FindItem( pReqSellItemMsg->uiItemID ) != NULL )
	{
		// 鏃晝ж朝 僭ヶ檜棻.

		// Ы溯檜橫諦 檜 葆擊曖 鼻欽除曖 婦啗蒂 撲薑и棻.			

		// 望萄 詹幗檣雖 憲嬴螞棻.
		if((stAccountInfo.pVillage->uiGuildID != 0) && (uiGuildID != 0) && (siClassInGuild != pMyWorld->clIGuildClass.GetJWJCode()))
		{
			if(stAccountInfo.pVillage->uiGuildID == uiGuildID)							
				siGuildRelation		=	ON_GUILD_RELATION_MINE;		// 檜 葆擊曖 鼻欽錳檜棻.
			else if(pMyWorld->clIGuild.IsHostilityRelation(uiGuildID, stAccountInfo.pVillage->uiGuildID) == TRUE)	
				siGuildRelation		=	ON_GUILD_RELATION_ENEMY;	// 瞳渠瞳檣 葆擊曖 鼻欽檜棻.
		}

		// 嬴檜蠱曖 陛問擊 薯渠煎 嫡擊 熱 氈棻.		
		stAccountInfo.pVillage->m_pMarket->GetSellItemPrice( pReqSellItemMsg->uiItemID, siGuildRelation, &uiRealItemsPrice );
		uiRealItemsPrice	*=	pReqSellItemMsg->uiItemQuantity;

		// 憮幗曖 嬴檜蠱擊 隸陛衛麵遽棻.	
		stAccountInfo.pVillage->m_pMarket->IncreaseItemQuantity( pReqSellItemMsg->uiItemID, pReqSellItemMsg->uiItemQuantity );
	}	
	else
	{
		// 鏃晝ж雖 彊朝 僭ヶ檜棻.

		// 嬴檜蠱曖 陛問擊 薯渠煎 嫡雖 跤и棻.
		uiRealItemsPrice	=	pMyWorld->clItem.GetItem(pReqSellItemMsg->uiItemID)->m_siDBPrice  * pReqSellItemMsg->uiItemQuantity / 3;

		// 憮幗曖 嬴檜蠱擊 隸陛衛麵輿雖 彊朝棻.(嬴鼠橾紫 ж雖 彊朝棻.)
	}

	
	// 嬴檜蠱擊 橈撻棻.
	if(DelItem(pReqSellItemMsg->uiFollowerID, pReqSellItemMsg->uiItemID, pReqSellItemMsg->uiItemQuantity, pReqSellItemMsg->uiPosInInventory) == NULL)
		goto	END;


	// 釭曖 絲擊 隸陛衛麵遽棻.
	IncreaseMoney(uiRealItemsPrice);
	// 葆擊曖 鼠羲 濠獄擊 棺萼棻.
	stAccountInfo.pVillage->IncreaseCapital(MONEY(uiRealItemsPrice*ON_CAPITAL_RATE_TRADE),ON_PROFIT_TRADE);

	// 鼠羲 褐辨紫蒂 隸陛衛麵遽棻.
	siTempCredit	=	GetGiveCredit(uiRealItemsPrice);
	if( siTempCredit > 0 )
		IncreaseTradeCredit( siTempCredit, FALSE );

	bSuccess	=	TRUE;

END:
	if(bSuccess == TRUE)
	{		
		SendFieldMsg(ON_RESPONSE_SELL_ITEM, SO_SFM_ONLYME, LPVOID(ON_RET_OK), LPVOID(&siMoney), LPVOID(uiTradeCredit));
	}
	else
	{		
		SendFieldMsg(ON_RESPONSE_SELL_ITEM, SO_SFM_ONLYME, LPVOID(ON_RET_NO));
	}
*/
}

//----------------------------------------------------------------------------------------------------------------
//	撲貲	:	嬴檜蠱擊 蹺陛й陽 鼠啪 薯и縑 勘葬朝雖 �挫恉挬�.
//----------------------------------------------------------------------------------------------------------------
BOOL SOPlayer::CheckWeight(UI08 uiFollowerID, UI16 uiID, UI16 uiQuantity)
{
	UI32		AddWeight;
	CItemHeader	*pItemHeader = pMyWorld->clItem.GetItem(uiID);
	if(pItemHeader)
	{
		AddWeight	=	pItemHeader->m_uiWeight	* uiQuantity;
		return IFollower.CheckWeight(uiFollowerID, AddWeight);
	}
	return FALSE;
}

//----------------------------------------------------------------------------------------------------------------
//	撲貲	:	鼠啪蟾婁檣 辨煽檜 ж釭塭紫 襄營ж罹 務晦 ��萇 鼻鷓檜賊 TRUE蒂 葬欐и棻.
//----------------------------------------------------------------------------------------------------------------
BOOL SOPlayer::IsOverWeight()
{
	return IFollower.IsOverWeight();
}


//----------------------------------------------------------------------------------------------------------------
//	撲貲	:	擎ч曖 薑爾蒂 橢橫螞棻.
//----------------------------------------------------------------------------------------------------------------
VOID SOPlayer::RecvBankInfo()
{
	OnReqBankInfoMsg	*pReqBankInfoMsg;
//	SI32				siBalance;
//	DBIKBankInfo		DBBankInfo;

	pReqBankInfoMsg		=	(OnReqBankInfoMsg*)cMsg;

	if( IsInVillage() == TRUE )
	{
		stAccountInfo.pVillage->SendBankInfo( this );

		/*
		if(stAccountInfo.pVillage->pstBankData != NULL)
		{
			// ⑷營 擎ч縑憮 Ы溯檜橫曖 濤擋擊 橢橫螞棻.
//			pMyWorld->clDatabase->GetDBInfo(SO_DB_INFOKIND_BANKBALANCE, LPVOID(&DBBankInfo), LPVOID(&DBBankInfo));
//			siBalance	=	DBBankInfo.siBankCapital;

			// 檜徽, 擎ч 濤擋, 頂 絲擊 爾頂遽棻.

			SendFieldMsg(ON_RESPONSE_BANK_INFO, SO_SFM_ONLYME, LPVOID(stAccountInfo.pVillage->pstBankData));
						//LPVOID(siBalance),
						//LPVOID(uiMoney));
		}	
		*/
	}
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	擎ч縑 蕨旎擊 и棻.
//	熱薑 橾濠	:	2002-10-18 螃�� 4:27:42 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
VOID SOPlayer::RecvBankDeposit()
{
	OnReqBankDepositMsg		*pReqBankDepositMsg;
	BOOL					bSuccess  = FALSE;

	pReqBankDepositMsg	=	(OnReqBankDepositMsg*)cMsg;
	
	// ⑷營 葆擊 寰縑 氈朝雖 匐餌и棻.
	if( IsInVillage() == FALSE )																				goto	END;	

	// 殮旎 ж溥朝 絲檜 嶸�褲捘� 匐餌и棻.
	if( ( pReqBankDepositMsg->siMoney <= 0 ) || ( EnoughMoney( pReqBankDepositMsg->siMoney ) == FALSE ) )		goto	END;

	bSuccess	=	stAccountInfo.pVillage->BankDeposit( this, pReqBankDepositMsg );

END:

	if( bSuccess == FALSE )
	{
		SendFieldMsg( ON_RESPONSE_BANK_DEPOSIT, SO_SFM_ONLYME, LPVOID( ON_RET_BANK_NO ) );	
	}

	/*
	// 蕨旎 ж溥朝 旎擋檜 ⑷營 陛雖堅 氈朝 絲爾棻 偽剪釭 濛嬴撿 и棻.
	if( (pReqBankDepositMsg->siMoney <= siMoney) && (pReqBankDepositMsg->siMoney > 0) )
	{
		
		if(stAccountInfo.pVillage != NULL)
		{
			bSuccess	=	stAccountInfo.pVillage->BankDeposit(this, pReqBankDepositMsg);
		}
	}
	
	if(bSuccess == FALSE)
	{
		SendFieldMsg(ON_RESPONSE_BANK_DEPOSIT, SO_SFM_ONLYME, LPVOID(ON_RET_BANK_NO));
	}	
	*/
}

//----------------------------------------------------------------------------------------------------------------
//	撲貲	:	擎ч縑憮 轎旎擊 и棻.
//----------------------------------------------------------------------------------------------------------------
VOID SOPlayer::RecvBankDrawing()
{
	OnReqBankDrawingMsg		*pReqBankDrawingMsg;	
	BOOL					bSuccess  = FALSE;
	
	pReqBankDrawingMsg	=	(OnReqBankDrawingMsg*)cMsg;

	// ⑷營 葆擊 寰縑 氈朝雖 匐餌и棻.
	if( IsInVillage() == FALSE )										goto	END;	

	// 轎旎 ж溥朝 絲檜 嶸�褲捘� 匐餌и棻.
	if( pReqBankDrawingMsg->siMoney <= 0 )								goto	END;
	
	// п渡 Ы溯檜橫陛 模雖й 熱 氈朝 旎擋檜 蟾婁腎朝雖 匐餌и棻.
	if( GetMaxMoneyToGet() < pReqBankDrawingMsg->siMoney )				goto	END;

	bSuccess	=	stAccountInfo.pVillage->BankDrawing( this, pReqBankDrawingMsg );

END:
	
	if( bSuccess == FALSE )
	{
		SendFieldMsg( ON_RESPONSE_BANK_DRAWING, SO_SFM_ONLYME, LPVOID( ON_RET_BANK_NO ) );
	}

	/*
	//DB縑憮 ⑷營 頂絲擊橢橫螞棻.	
	pReqBankDrawingMsg	=	(OnReqBankDrawingMsg*)cMsg;
	
	stAccountInfo.pVillage->BankDrawing(this,pReqBankDrawingMsg);

*/

}

/*
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	輿薄 啪衛っ縑 菟橫除棻朝 詭衛雖蒂 嫡懊棻.
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
VOID	SOPlayer::RecvEnterNoticeBoard()
{
	if(stAccountInfo.pVillage != NULL)
	{
		// 菟橫 鬲擊 唳辦曖 詭衛雖朝 Village 贗楚蝶縑憮 爾頂遽棻.		
		stAccountInfo.pVillage->EnterNoticeBoard(this);				
	}
	else
	{		
		// 啪衛っ縑 菟橫陞 熱陛 橈棻.
		SendFieldMsg(ON_RESPONSE_ENTER_NOTICEBOARD, SO_SFM_ONLYME, LPVOID(ON_RET_INN_NO));
	}
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	輿薄縑 菟橫除棻朝 詭衛雖蒂 嫡懊棻.
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
VOID	SOPlayer::RecvEnterInn()
{
	// 輿薄縑朝 鼠褻勒 菟橫除棻.
	SendFieldMsg(ON_RESPONSE_ENTER_INN, SO_SFM_ONLYME);

	// 輿薄縑 菟橫鬲棻堅 撲薑и棻.
	//stAccountInfo.uiStatusInVillage			=	INVILLAGE_STATUS_INN;
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	輿薄縑憮 釭除棻朝 詭衛雖蒂 嫡懊棻.
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
VOID	SOPlayer::RecvLeaveInn()
{
	BOOL					bSuccess = FALSE;	

	if(stAccountInfo.pVillage != NULL)
	{
		// 釭鬲擊 唳辦曖 詭衛雖朝 Village 贗楚蝶縑憮 爾頂遽棻.
		bSuccess	=	stAccountInfo.pVillage->LeaveInn(this);

		if(bSuccess == TRUE)
		{
			stAccountInfo.uiStatusInVillage	=	INVILLAGE_STATUS_NONE;			
		}
	}	

	if(bSuccess == FALSE)
	{
		// 渠�食瞈□� 釭陞 熱 橈棻.
		SendFieldMsg(ON_RESPONSE_LEAVE_INN, SO_SFM_ONLYME, LPVOID(ON_RET_INN_NO));
	}
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	輿薄 瓣た 渠晦褒縑 菟橫除棻朝 詭衛雖蒂 嫡懊棻.
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
VOID	SOPlayer::RecvEnterWaitRoom()
{
	BOOL					bSuccess = FALSE;	

	if(stAccountInfo.pVillage != NULL)	
	{
		// 菟橫 鬲擊 唳辦曖 詭衛雖朝 Village 贗楚蝶縑憮 爾頂遽棻.		
		bSuccess	=	stAccountInfo.pVillage->EnterWaitRoom(this);				
	}	

	if(bSuccess == FALSE)
	{
		// 渠�食瞈� 菟橫陞 熱 橈棻.
		SendFieldMsg(ON_RESPONSE_ENTER_VWR, SO_SFM_ONLYME,	LPVOID(ON_RET_INN_NO));
	}
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	輿薄縑憮 渠�食磍� 虜萇棻朝 詭衛雖蒂 嫡懊棻.
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
VOID	SOPlayer::RecvCreateChatRoomInInn()
{
	BOOL					bSuccess = FALSE;	
	OnReqCreateChatRoomMsg	*pCreateChatRoomMsg;
	CHAR					szChatRoomName[ON_MAX_CHATROOM_STRING_LENGTH];
	CHAR					*pszChatRoomName;


	pCreateChatRoomMsg	=	(OnReqCreateChatRoomMsg*)cMsg;
	pszChatRoomName		=	(CHAR*)pCreateChatRoomMsg + sizeof(OnReqCreateChatRoomMsg);

	ZeroMemory(szChatRoomName, sizeof(szChatRoomName));
	strncpy(szChatRoomName, pszChatRoomName, min(ON_MAX_CHATROOM_STRING_LENGTH - 1, pCreateChatRoomMsg->uiStringLength));	

	if(stAccountInfo.pVillage != NULL)	
	{		

		// 渠�食磍� 虜萇棻.		
		// 撩奢瞳戲煎 渠�食磍� 虜菟歷棻賊 斜 渠�食瞈� 蝶蝶煎 菟橫除棻.
		// 婦溼 詭衛雖朝 Village 贗楚蝶縑憮 爾頂遽棻.
		bSuccess	=	stAccountInfo.pVillage->CreateChatRoomInInn(this, szChatRoomName);				
	}

	if(bSuccess == FALSE)
	{
		// 瓣た寞擊 虜菟熱 橈棻.
		SendFieldMsg(ON_RESPONSE_INN_CREATECHATROOM, SO_SFM_ONLYME,
						LPVOID(ON_RET_INN_NO));
	}
}

//----------------------------------------------------------------------------------------------------------------
//	撲貲	:	輿薄 渠�食瞈� 菟橫除棻朝 詭衛雖蒂 嫡懊棻.
//----------------------------------------------------------------------------------------------------------------
VOID	SOPlayer::RecvJoinInnChatRoom()
{
	BOOL						bSuccess = FALSE;
	OnReqJoinVilInnChatRoomMsg	*pJoinVilInnChatRoomMsg;

	pJoinVilInnChatRoomMsg	=	(OnReqJoinVilInnChatRoomMsg*)cMsg;


	if(stAccountInfo.pVillage != NULL)	
	{
		// 菟橫 鬲擊 唳辦曖 詭衛雖朝 Village 贗楚蝶縑憮 爾頂遽棻.		
		bSuccess	=	stAccountInfo.pVillage->JoinChatRoomInInn(pJoinVilInnChatRoomMsg->uiChatRoomID, this);				
	}	

	if(bSuccess == FALSE)
	{
		// 渠�食瞈� 菟橫陞 熱 橈棻.
		SendFieldMsg(ON_RESPONSE_VIL_INN_JOIN_CHAT_ROOM, SO_SFM_ONLYME,
						LPVOID(ON_RET_INN_NO));
	}
}

//----------------------------------------------------------------------------------------------------------------
//	撲貲	:	Ы溯檜橫陛 輿薄 瓣た寞縑憮 釭鬲棻.
//----------------------------------------------------------------------------------------------------------------
VOID	SOPlayer::RecvLeaveInnChatRoom()
{
	BOOL	bSuccess = FALSE;
	if(stAccountInfo.pVillage != NULL)
	{
		// 釭鬲擊 唳辦曖 詭衛雖朝 Village 贗楚蝶縑憮 爾頂遽棻.
		bSuccess	=	stAccountInfo.pVillage->LeaveChatRoomInInn(this);
	}	

	if(bSuccess == FALSE)
	{
		// 渠�食瞈□� 釭陞 熱 橈棻.
		SendFieldMsg(ON_RESPONSE_VIL_INN_LEAVE_CHAT_ROOM, SO_SFM_ONLYME,
								LPVOID(ON_RET_INN_NO));
	}
}
*/

//----------------------------------------------------------------------------------------------------------------
//	撲貲	:	葆擊 衛濰縑 菟橫除棻.
//----------------------------------------------------------------------------------------------------------------
VOID	SOPlayer::RecvEnterMarket()
{
	BOOL	bSuccess = FALSE;

	if(stAccountInfo.pVillage != NULL)	
	{
		// 菟橫 鬲擊 唳辦曖 詭衛雖朝 Village 贗楚蝶縑憮 爾頂遽棻.		
		bSuccess	=	stAccountInfo.pVillage->m_pMarket->EnterPlayer( this );				

		if(bSuccess == TRUE)
		{
			// 衛濰縑 菟橫除 鼻鷓檜棻.
			stAccountInfo.uiStatusInVillage	=	INVILLAGE_STATUS_MARKET;

			SendFieldMsg( ON_RESPONSE_ENTER_MARKET, SO_SFM_ONLYME, LPVOID( ON_RET_MARKET_OK ) );
		}
	}	

	if(bSuccess == FALSE)
	{
		// 渠�食瞈� 菟橫陞 熱 橈棻.
		SendFieldMsg(ON_RESPONSE_ENTER_MARKET, SO_SFM_ONLYME, LPVOID(ON_RET_MARKET_NO));
	}
}

//----------------------------------------------------------------------------------------------------------------
//	撲貲	:	葆擊 衛濰縑憮 釭除棻.
//----------------------------------------------------------------------------------------------------------------
VOID	SOPlayer::RecvLeaveMarket()
{
	BOOL	bSuccess = FALSE;	

	if(stAccountInfo.pVillage != NULL)
	{
		// 釭鬲擊 唳辦曖 詭衛雖朝 Village 贗楚蝶縑憮 爾頂遽棻.
		bSuccess	=	stAccountInfo.pVillage->m_pMarket->LeavePlayer( this );

		if(bSuccess == TRUE)
		{
			stAccountInfo.uiStatusInVillage	=	INVILLAGE_STATUS_NONE;			

			SendFieldMsg(ON_RESPONSE_LEAVE_MARKET, SO_SFM_ONLYME, LPVOID(ON_RET_MARKET_OK));
		}
	}	

	if(bSuccess == FALSE)
	{
		// 渠�食瞈□� 釭陞 熱 橈棻.
		SendFieldMsg(ON_RESPONSE_LEAVE_MARKET, SO_SFM_ONLYME, LPVOID(ON_RET_INN_NO));
	}
}



//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	億煎遴 啗薑(鱔濰)擊 虜萇棻朝 詭衛雖蒂 嫡懊棻.
//	熱薑 橾濠	:	2002-10-18 螃�� 3:46:18 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
VOID	SOPlayer::RecvOpenSavingAccount()
{	
	OnReqBankOpenSavingAccountMsg	*pOpenSavingAccountMsg;	
	DWORD							dwResponse = ON_RET_BANK_NO;

	pOpenSavingAccountMsg	=	(OnReqBankOpenSavingAccountMsg*)cMsg;

	// ⑷營 葆擊 寰縑 氈朝雖 匐餌и棻.
	if( IsInVillage() == FALSE )																											goto	END;

	// 蕨諼籀葬.....( 嬴霜 雖錳ж雖 彊朝 睡碟檜棻. )
	if( pOpenSavingAccountMsg->uiKind == 4 )																								goto	END;

	// 褐辨 渠轎檜塭賊 渠轎 и紫 �挫�..
	if( pOpenSavingAccountMsg->uiKind == ON_BANK_LOAN_TYPE ) 
	{	
		// 渠轎й 熱 氈朝 и紫擋擊 匐餌и棻.
		if( pOpenSavingAccountMsg->siFirstDepositMoney > uiMaxLoan )									 									goto	END;				
		if( pMyWorld->clDatabase->GetDBInfo( SO_DB_INFOKIND_CHECKLOAN, LPVOID( this ), NULL ) == FALSE)										goto	END;		
		if( pOpenSavingAccountMsg->siFirstDepositMoney > stPropertyData.siTotalProperty )													goto	END;
	}

	// 籀擠 殮旎 ж溥朝 絲檜 嶸�褲捘� 匐餌и棻.
	if( ( pOpenSavingAccountMsg->siFirstDepositMoney <= 0 ) || ( EnoughMoney( pOpenSavingAccountMsg->siFirstDepositMoney ) == FALSE ) )		goto	END;
		
	// 譆渠 虜菟 熱 氈朝 鱔濰曖 偃熱蒂 蟾婁ц朝雖 匐餌и棻.
	if( GetBankBootNum() >= ON_MAX_BANKBOOK_NUM )																							goto	END;

	dwResponse	=	stAccountInfo.pVillage->OpenSavingAccount( this, pOpenSavingAccountMsg, NULL );

END:

	if( dwResponse != ON_RET_OK )
	{
		// 啗謝蒂 虜萄朝等 褒ぬц棻.
		SendFieldMsg( ON_RESPONSE_BANK_OPENSAVINGACCOUNT, SO_SFM_ONLYME, LPVOID( dwResponse ) );
		
	}


	/*
	OnReqBankOpenSavingAccountMsg*	pOpenSavingAccountMsg;	
	SI32							siRet	=	ON_RET_BANK_OK;

	 pOpenSavingAccountMsg	=	(OnReqBankOpenSavingAccountMsg*)cMsg;


	 if( pOpenSavingAccountMsg->uiKind  == 3 ) // 褐辨 渠轎檜塭賊 渠轎 и紫 �挫�..
	 {
		 if( pOpenSavingAccountMsg->siFirstDepositMoney > uiMaxLoan )
		 {			 
			 siRet	=	ON_RET_BANK_NO;
		 }
		 
		 if(pMyWorld->clDatabase->GetDBInfo(SO_DB_INFOKIND_CHECKLOAN, LPVOID(this), LPVOID(&siRet)) == FALSE)
		 {
			 siRet	=	ON_RET_BANK_NO;
		 }
	 }

	 if( pOpenSavingAccountMsg->siFirstDepositMoney <= 0 )
		 siRet = ON_RET_BANK_NO;

	 if( siRet == ON_RET_BANK_OK )
	 {
		 if ( GetBankBootNum() < ON_MAX_BANKBOOK_NUM )
		 {
			 if(stAccountInfo.pVillage != NULL)
			 {
				 // 啗謝曖 褐羶擎 葆擊曖 擎ч縑憮 熱чи棻.
				 // 擎ч啗謝蒂 撩奢瞳戲煎 虜菟唳辦 擎ч縑憮 霜蕾 餌辨濠縑啪 詭衛雖蒂 爾頂遽棻.
				 siRet	=	stAccountInfo.pVillage->OpenSavingAccount(this, pOpenSavingAccountMsg,NULL);
			 }
		 }
	 }

	if(siRet != ON_RET_BANK_OK)
	{		
		// 擎ч啗謝蒂 虜菟 熱 橈棻.
		SendFieldMsg(ON_RESPONSE_BANK_OPENSAVINGACCOUNT, SO_SFM_ONLYME, LPVOID(siRet));
	}
	*/
}


//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	鱔濰 葬蝶お蒂 殖塭朝 蹂掘蒂 嫡懊棻.
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
VOID	SOPlayer::RecvBankbookList()
{
	DBIKBankBookList			BankBookList;
	DBIKVillageBankBookInfo		VillageBankBookInfo;
	BOOL						bGetBankBookListSuccess = TRUE;

	// 辦摹 斜 議葛攪陛 葆擊縑 橫替 葆擊縑 菟橫陛 氈橫撿 и棻.
	if(stAccountInfo.pVillage != NULL)
	{
		// DB縑憮 鱔濰曖 薑爾蒂 橢橫螞棻.	
		VillageBankBookInfo.pPlayer			=	this;
		VillageBankBookInfo.uiVillageCode	=	stAccountInfo.pVillage->uiVillageCode;
		VillageBankBookInfo.uiVillageCode	=	stAccountInfo.pVillage->uiVillageCode;

		memset(&BankBookList, 0, sizeof(BankBookList));
		if(pMyWorld->clDatabase->GetDBInfo(SO_DB_INFOKIND_MYBANKBOOK, LPVOID(&VillageBankBookInfo), LPVOID(&BankBookList)) == TRUE)
		{
			// 鱔濰曖 薑爾蒂 撩奢瞳戲煎 橢橫諮棻.			
			SendFieldMsg(ON_RESPONSE_BANK_BANKBOOK_LIST, SO_SFM_ONLYME, LPVOID(ON_RET_BANK_OK), LPVOID(&BankBookList));
			
		}
		else
			bGetBankBookListSuccess	=	FALSE;
	}
	else
		bGetBankBookListSuccess	=	FALSE;

	if(bGetBankBookListSuccess == FALSE)
	{
		// 鱔濰曖 薑爾蒂 橢橫螃朝等 褒ぬц棻.
		SendFieldMsg(ON_RESPONSE_BANK_BANKBOOK_LIST, SO_SFM_ONLYME, LPVOID(ON_RET_BANK_NO));
	}
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	鱔濰 葬蝶お曖 熱蒂 殖塭朝 蹂掘蒂 嫡懊棻.
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
VOID	SOPlayer::RecvBankbookListCount()
{
	MONEY		Temp;

	Temp = m_MoneySaving;
	SendFieldMsg(ON_RESPONSE_BANK_BANKBOOK_LISTCOUNT, SO_SFM_ONLYME, LPVOID(ON_RET_OK), LPVOID(&Temp));
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	氬爾蒂 蜆曹棻朝 蹂掘蒂 嫡懊棻.
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
VOID	SOPlayer::RecvBankCollateral()
{
	DBIKVillageCollateral			stVillageCollateral;
	OnReqCollateralLoanMsg			*pCollateralLoan;
	OnAccountNumber					stAccountNum;	
	EconomyHeader					*pEconomyHeader;
	DBIKCollateralList				stCollateralList;
	char							*pIndex;
	int								nSize;
	MONEY							MaxLoan = 0;

	OnReqBankOpenSavingAccountMsg	stOpenSavingAccountMsg;

	SI32						siRet	=	ON_RET_BANK_OK;

	pCollateralLoan		=	(OnReqCollateralLoanMsg*)cMsg;
	stCollateralList.siCollateralNum		=	pCollateralLoan->dwItemNum;

	pIndex	=			(CHAR*)cMsg+sizeof(OnReqCollateralLoanMsg);
	nSize	=			(sizeof UI08) * (pCollateralLoan->dwItemNum);
	memcpy(stCollateralList.uiFollowerID, pIndex, nSize);

	pIndex		+=		nSize;
	nSize		=		(sizeof UI16) * (pCollateralLoan->dwItemNum);		
	memcpy(stCollateralList.uiITemCode, pIndex, nSize);

	pIndex		+=		nSize;
	nSize		=		(sizeof UI16) * (pCollateralLoan->dwItemNum);		
	memcpy(stCollateralList.uiNum, pIndex, nSize);
	
	pIndex		+=		nSize;
	nSize		=		(sizeof SI16) * (pCollateralLoan->dwItemNum);		
	memcpy(stCollateralList.siInvenLoc, pIndex, nSize);

	// 氬爾 鱔濰縑 婦и 薑爾蒂 橢橫螞棻.
	pEconomyHeader = pMyWorld->clOnlineEconomy.GetHeader(3);

	if(stAccountInfo.pVillage != NULL && pEconomyHeader != NULL)
	{
		stVillageCollateral.pPlayer			=	this;
		stVillageCollateral.uiVillageCode	=	stAccountInfo.pVillage->uiVillageCode;
		stVillageCollateral.uiVillageCode	=	stAccountInfo.pVillage->uiVillageCode;
		stVillageCollateral.siMoney			=	pCollateralLoan->siMoney;
		stVillageCollateral.uiMonth			=	pCollateralLoan->uiMonth;
		stVillageCollateral.pCollateralList	=	&stCollateralList;

		// 渠轎 晦и и紫陛 剩歷朝雖 �挫恉挬�.
		if(stVillageCollateral.uiMonth > pEconomyHeader->siMaxPeriod )
		{			
			siRet = ON_RET_BANK_NO;
		}

		if(siRet == ON_RET_BANK_OK)
		{
			if ( GetBankBootNum() >= ON_MAX_BANKBOOK_NUM )
				siRet	=	ON_RET_BANK_NO;
		}

		if(siRet == ON_RET_BANK_OK)
		{
			
			// 渠轎 擋熱 и紫陛 剩歷朝雖 �挫恉挬�.
			for(int i=0	;	i < (SI32)pCollateralLoan->dwItemNum ; i++)
			{
				if( stCollateralList.uiNum[i] > 1000 )
				{
					siRet = ON_RET_BANK_NO;
					break;
				}
				MaxLoan		+=	(pMyWorld->clItem.GetItem(stCollateralList.uiITemCode[i])->m_siDBPrice) * ( stCollateralList.uiNum[i] ) * 7 / 10;
			}			
			
			if(stVillageCollateral.siMoney > MaxLoan )   //1000 渠褐 渠轎и紫陛 諦撿 и棻.
			{				
				siRet = ON_RET_BANK_NO;
			}
		}
		//檜薯 db縑 盪濰и棻. DB縑憮 渠轎 鱔濰檜 氈朝雖紫 �挫� 

		if(siRet == ON_RET_BANK_OK && stAccountInfo.pVillage != NULL)
		{	
			// DB縑 氬爾縑 渠и 晦獄 薑爾蒂 厥堅 �挫恉挬�.
			pMyWorld->clDatabase->GetDBInfo(SO_DB_INFOKIND_COLLATERAL_1, LPVOID(&stVillageCollateral), LPVOID(&siRet));				
			if(siRet == 1)
			{
				// 4廓 鱔濰(氬爾 鱔濰)擊 虜菟堅 嬴檜蠱擊 薯剪и棻.				
				stOpenSavingAccountMsg.dwMsgType	=	ON_REQUEST_BANK_OPENSAVINGACCOUNT;
				stOpenSavingAccountMsg.uiKind		=	4;
				stOpenSavingAccountMsg.siFirstDepositMoney	=	pCollateralLoan->siMoney;
				stOpenSavingAccountMsg.uiExpireMonth		=	pCollateralLoan->uiMonth;
				
				siRet = stAccountInfo.pVillage->OpenSavingAccount(this, &stOpenSavingAccountMsg,&stAccountNum);
				if( siRet == ON_RET_BANK_OK)
				{
					for(int i=0; i<stCollateralList.siCollateralNum;i++)
					{
						if( DelItem( stCollateralList.uiFollowerID[i], stCollateralList.uiITemCode[i], stCollateralList.uiNum[i], stCollateralList.siInvenLoc[i]) == NULL )
						{
							for(int j=0; j<i; j++)
								AddItem( stCollateralList.uiFollowerID[j], stCollateralList.uiITemCode[j], stCollateralList.uiNum[j], stCollateralList.siInvenLoc[j]);
							siRet = ON_RET_BANK_NO;
							break;
						}
					}
					SendFieldMsg(ON_RESPONSE_BANK_COLLATERAL, SO_SFM_ONLYME, LPVOID(siRet),LPVOID(&stAccountNum));  // pParam2朝 鱔濰 廓��.
				}
			}
			else
				siRet = ON_RET_BANK_NO;
		}	
		
	}

	if(siRet != ON_RET_BANK_OK)
	{
		SendFieldMsg(ON_RESPONSE_BANK_COLLATERAL, SO_SFM_ONLYME, LPVOID(ON_RET_BANK_NO));
	}

}


//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	氬爾 葬蝶お蒂 殖塭朝 蹂掘蒂 嫡懊棻.
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
VOID	SOPlayer::RecvBankCollateralList()
{
	DBIKVillageCollateralInfo				stVillageCollateralInfo;
	DBIKCollateralList						stCollateralList;
	BOOL									bGetCollateralListSuccess	=	TRUE;

	stVillageCollateralInfo.siAccount		= stAccountInfo.siDBAccount;
	stVillageCollateralInfo.uiSlot			= stAccountInfo.uiSlot;
	stVillageCollateralInfo.uiVillageCode	= stAccountInfo.pVillage->uiVillageCode;

	if(stAccountInfo.pVillage != NULL)
	{
		if(pMyWorld->clDatabase->GetDBInfo(SO_DB_INFOKIND_COLLATERALLIST, LPVOID(&stVillageCollateralInfo), LPVOID(&stCollateralList)) == TRUE)
		{	
			// 鱔濰曖 薑爾蒂 撩奢瞳戲煎 橢橫諮棻.			
			SendFieldMsg(ON_RESPONSE_BANK_COLLATERALLIST, SO_SFM_ONLYME, LPVOID(ON_RET_BANK_OK), LPVOID(&stCollateralList));
		}
		else
			bGetCollateralListSuccess	=	FALSE;
	}
	else
		bGetCollateralListSuccess	=	FALSE;

	if(bGetCollateralListSuccess == FALSE)
	{		
		// 氬爾 薑爾蒂 橢橫螃朝等 褒ぬц棻.
		SendFieldMsg(ON_RESPONSE_BANK_COLLATERALLIST, SO_SFM_ONLYME, LPVOID(ON_RET_BANK_NO));
	}
}


//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	氬爾蒂 給溥 殖塭朝 蹂掘蒂 嫡懊棻.
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
VOID	SOPlayer::RecvBankTakeCollateral()
{
	DBIKVillageCollateralInfo				stVillageCollateralInfo;
	OnReqTakeCollateralMsg					*pTakeCollateralMsg;	
	DBIKCollateralList						stCollateralList;
	char									*pIndex;
	int										nSize;
	SI32						siResult	= ON_RET_BANK_NO;
	
	pTakeCollateralMsg					=	(OnReqTakeCollateralMsg*)cMsg;
	stCollateralList.siCollateralNum	=	pTakeCollateralMsg->dwItemNum;
	
	pIndex	=			(CHAR*)cMsg+sizeof(OnReqTakeCollateralMsg);
	nSize	=			(sizeof UI08) * (pTakeCollateralMsg->dwItemNum);
	memcpy(stCollateralList.uiFollowerID, pIndex, nSize);

	pIndex		+=		nSize;
	nSize		=		(sizeof UI16) * (pTakeCollateralMsg->dwItemNum);		
	memcpy(stCollateralList.uiITemCode, pIndex, nSize);

	pIndex		+=		nSize;
	nSize		=		(sizeof UI16) * (pTakeCollateralMsg->dwItemNum);		
	memcpy(stCollateralList.uiNum, pIndex, nSize);
	
	pIndex		+=		nSize;
	nSize		=		(sizeof SI16) * (pTakeCollateralMsg->dwItemNum);
	memcpy(stCollateralList.siInvenLoc, pIndex, nSize);

	stVillageCollateralInfo.siAccount		= stAccountInfo.siDBAccount;
	stVillageCollateralInfo.uiSlot			= stAccountInfo.uiSlot;
	stVillageCollateralInfo.uiVillageCode	= stAccountInfo.pVillage->uiVillageCode;

	if( stCollateralList.siCollateralNum != 0 )
	{		
		//pMyWorld->clDatabase->SetDBInfo(SO_DB_INFOKIND_TAKECOLLATERAL, LPVOID(&stVillageCollateralInfo), NULL);
		pMyWorld->clDatabase->writeDBQuery(SO_DB_INFOKIND_TAKECOLLATERAL, LPVOID(&stVillageCollateralInfo), sizeof(DBIKVillageCollateralInfo));
		
		siResult	=	ON_RET_BANK_OK;
		for(int i=0; i<stCollateralList.siCollateralNum;i++)
		{
			AddItem( stCollateralList.uiFollowerID[i], stCollateralList.uiITemCode[i],
				stCollateralList.uiNum[i], stCollateralList.siInvenLoc[i] );
		}
		// 鱔濰曖 薑爾蒂 撩奢瞳戲煎 橢橫諮棻.			
		SendFieldMsg(ON_RESPONSE_BANK_TAKECOLLATERAL, SO_SFM_ONLYME, LPVOID(ON_RET_BANK_OK));		
	}
	if(siResult == ON_RET_BANK_NO)
	{
		
		SendFieldMsg(ON_RESPONSE_BANK_TAKECOLLATERAL, SO_SFM_ONLYME, LPVOID(ON_RET_BANK_NO));
	}
}

/*
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	啪衛僭擊 蛔煙и棻.
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
VOID	SOPlayer::RecvRegistNotice()
{
	BOOL					bSuccessRegistNotice  = FALSE;
	CHAR					szTitle[4096], szNotice[10240];
	CHAR					*pszTitle;
	CHAR					*pszNotice;
	OnReqRegistNoticeMsg	*pRegistNoticeMsg;
	
	// 旋擊 蛔煙ж朝等 褒ぬц棻堅 憲溥遽棻.
	SendFieldMsg(ON_RESPONSE_REGIST_NOTICE, SO_SFM_ONLYME, LPVOID(ON_RET_INN_NO));
	return;

	pRegistNoticeMsg	=	(OnReqRegistNoticeMsg*)cMsg;

	if(stAccountInfo.pVillage != NULL)
	{
		pszTitle		=	(CHAR*)pRegistNoticeMsg + sizeof(OnReqRegistNoticeMsg);
		pszNotice		=	(CHAR*)pszTitle + pRegistNoticeMsg->uiTitleStringLength;

		// Title擊 犒餌.
		strncpy(szTitle, pszTitle, pRegistNoticeMsg->uiTitleStringLength);
		szTitle[pRegistNoticeMsg->uiTitleStringLength]		=	NULL;

		// Notice蒂 犒餌.
		strncpy(szNotice, pszNotice, pRegistNoticeMsg->uiNoticeStringLength);
		szNotice[pRegistNoticeMsg->uiNoticeStringLength]	=	NULL;

		// 噩 熱 橈朝 旋濠陛 氈朝 雖 �挫恉挬�.
		strtok(szTitle, ON_FORBIDDEN_CHAR);
		strtok(szNotice, ON_FORBIDDEN_CHAR);
		if(szTitle[0]	==	'\'')	szTitle[0]	=	20;
		if(szNotice[0]	==	'\'')	szNotice[0]	=	20;
		
		bSuccessRegistNotice	=	stAccountInfo.pVillage->RegistNotice(this, szTitle, szNotice);
	}
	
	if(bSuccessRegistNotice == FALSE)
	{
		// 旋擊 蛔煙ж朝等 褒ぬц棻堅 憲溥遽棻.
		SendFieldMsg(ON_RESPONSE_REGIST_NOTICE, SO_SFM_ONLYME, LPVOID(ON_RET_INN_NO));
	}

}


//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	啪衛っ曖 旋擊 餉薯и棻.
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
VOID	SOPlayer::RecvDeleteNotice()
{
	BOOL					bSuccess = FALSE;
	OnReqDeleteNoticeMsg	*pDeleteNoticeMsg;

	pDeleteNoticeMsg	=	(OnReqDeleteNoticeMsg*)cMsg;

	if(stAccountInfo.pVillage != NULL)
	{			
		bSuccess	=	stAccountInfo.pVillage->DeleteNotice(this, pDeleteNoticeMsg->uiNoticeIndex);
	}
	
	if(bSuccess == FALSE)
	{
		// 旋擊 蛔煙ж朝等 褒ぬц棻堅 憲溥遽棻.
		SendFieldMsg(ON_RESPONSE_DELETE_NOTICE, SO_SFM_ONLYME, LPVOID(ON_RET_INN_NO));
	}
}
		

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	啪衛僭曖 葬蝶お蒂 橢橫螞棻.
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
VOID	SOPlayer::RecvNoticesList()
{
	BOOL					bSuccess	=	FALSE;
	OnReqNoticesListMsg		*pNoticesListMsg;

	pNoticesListMsg		=	(OnReqNoticesListMsg*)cMsg;	

	if(stAccountInfo.pVillage != NULL)
	{
		bSuccess	=	stAccountInfo.pVillage->SendNoticesList(this, pNoticesListMsg->uiPage);
	}
	
	if(bSuccess == FALSE)
	{
		// 旋曖 葬蝶お蒂 橢橫螃朝等 褒ぬц棻.
		SendFieldMsg(ON_RESPONSE_NOTICEBOARD_NOTICELIST, SO_SFM_ONLYME, LPVOID(ON_RET_INN_NO));
	}
}



//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	啪衛僭擊 獄棻.
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
VOID	SOPlayer::RecvViewNotice()
{
	BOOL					bSuccess	=	FALSE;
	OnReqViewNoticeMsg		*pViewNoticeMsg;

	pViewNoticeMsg	=	(OnReqViewNoticeMsg*)cMsg;

	if(stAccountInfo.pVillage != NULL)
	{
		bSuccess	=	stAccountInfo.pVillage->SendNoticeContent(this, pViewNoticeMsg->uiNoticeIndex);
	}

	if(bSuccess == FALSE)
	{
		// 旋擊 橢橫螃朝等 褒ぬц棻.
		SendFieldMsg(ON_RESPONSE_VIEW_NOTICE, SO_SFM_ONLYME, LPVOID(ON_RET_INN_NO));
	}
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	啪衛僭擊 熱薑и棻.
//	熱薑 橾濠	:	2002-04-15 螃�� 2:30:17 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
VOID	SOPlayer::RecvModifyNotice()
{
	BOOL					bSuccess	=	FALSE;
	OnReqModifyNoticeMsg	*pModifyNoticeMsg;
	CHAR					*pszNoticeTitleString;
	CHAR					*pszNoticeContentString;
	CHAR					szNoticeTitle[256];
	CHAR					szNoticeContent[4096];

	// 旋擊 蛔煙ж朝等 褒ぬц棻堅 憲溥遽棻.
	SendFieldMsg(ON_RESPONSE_MODIFY_NOTICE, SO_SFM_ONLYME, LPVOID(ON_RET_INN_NO));
	return;

	pModifyNoticeMsg	=	(OnReqModifyNoticeMsg*)cMsg;

	if(stAccountInfo.pVillage != NULL)
	{
		pszNoticeTitleString	=	(CHAR*)((CHAR*)pModifyNoticeMsg + sizeof(OnReqModifyNoticeMsg));
		pszNoticeContentString	=	(CHAR*)((CHAR*)pszNoticeTitleString + pModifyNoticeMsg->uiTitleStringLength);

		ZeroMemory(szNoticeTitle, sizeof(szNoticeTitle));
		ZeroMemory(szNoticeContent, sizeof(szNoticeContent));

		strncpy(szNoticeTitle, pszNoticeTitleString, min(sizeof(szNoticeTitle), pModifyNoticeMsg->uiTitleStringLength));
		strncpy(szNoticeContent, pszNoticeContentString, min(sizeof(szNoticeContent), pModifyNoticeMsg->uiNoticeStringLength));

		bSuccess	=	stAccountInfo.pVillage->ModifyNotice(this, pModifyNoticeMsg->dwNoticeNum, szNoticeTitle, szNoticeContent);
	}

	if(bSuccess == FALSE)
	{
		// 旋擊 橢橫螃朝等 褒ぬц棻.
		SendFieldMsg(ON_RESPONSE_MODIFY_NOTICE, SO_SFM_ONLYME, LPVOID(ON_RET_INN_NO));
	}

}	
*/




//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	辨煽 List蒂 蹂羶и棻.
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
VOID	SOPlayer::RecvBarrackGetSolderList()
{
	if(stAccountInfo.pVillage != NULL)
	{

		if( stAccountInfo.pVillage->m_Barrack != NULL )
		{

			// 菟橫 鬲擊 唳辦曖 詭衛雖朝 Village 贗楚蝶縑憮 爾頂遽棻.		
			stAccountInfo.pVillage->m_Barrack->BarrackGetSolderList(this);				

		}
		else
		{
			// 辨煽 堅辨塢縑 List蒂 還熱 橈棻.
			SendFieldMsg(ON_RESPONSE_BARRACK_GETSOLDERLIST, SO_SFM_ONLYME, LPVOID(ON_RET_NO));
		}

	}
	else
	{		
		// 辨煽 堅辨塢縑 List蒂 還熱 橈棻.
		SendFieldMsg(ON_RESPONSE_BARRACK_GETSOLDERLIST, SO_SFM_ONLYME, LPVOID(ON_RET_NO));
	}
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	辨煽擊 堅辨и棻.
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
VOID	SOPlayer::RecvBarrackBuySolder()
{
	OnReqBarrackBuySolder		*pOnReqBarrackBuySolder;

	pOnReqBarrackBuySolder	=	(OnReqBarrackBuySolder*)cMsg;

	if(stAccountInfo.pVillage != NULL)
	{
		if( stAccountInfo.pVillage->m_Barrack != NULL )
		{

			// 辨煽 堅辨縑 渠и 薑爾朝 Village 贗楚蝶縑憮 爾頂遽棻.
			if(stAccountInfo.pVillage->m_Barrack->BarrackBuySolder(this, pOnReqBarrackBuySolder->dwBuySolderID, (BYTE*)pOnReqBarrackBuySolder->szName) == FALSE)
			{
				// 褒ぬ衛 Message 虜 瞪歎п 遽棻. 撩奢ц擊陽朝 頂睡縑憮 Message蒂 爾魚棻.
				SendFieldMsg(ON_RESPONSE_BARRACK_BUYSOLDER, SO_SFM_ONLYME, LPVOID(ON_RET_NO));
			}
		}
		else
		{
			// 辨煽 堅辨塢縑憮 辨煽擊 堅辨й熱 橈棻.
			SendFieldMsg(ON_RESPONSE_BARRACK_BUYSOLDER, SO_SFM_ONLYME, LPVOID(ON_RET_NO));
		}

	}
	else
	{		
		// 辨煽 堅辨塢縑憮 辨煽擊 堅辨й熱 橈棻.
		SendFieldMsg(ON_RESPONSE_BARRACK_BUYSOLDER, SO_SFM_ONLYME, LPVOID(ON_RET_NO));
	}
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	п堅й 辨煽 List蒂 蹂羶и棻.
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
VOID	SOPlayer::RecvBarrackGetDismissalSolderList()
{
	if(stAccountInfo.pVillage != NULL)
	{
		if( stAccountInfo.pVillage->m_Barrack != NULL )
		{

			// 菟橫 鬲擊 唳辦曖 詭衛雖朝 Village 贗楚蝶縑憮 爾頂遽棻.		
			stAccountInfo.pVillage->m_Barrack->BarrackGetDismissalSolderList(this);				

		}
		else
		{

			// 辨煽 堅辨塢縑 List蒂 還熱 橈棻.
			SendFieldMsg(ON_RESPONSE_BARRACK_GETDISMISSALSOLDERLIST, SO_SFM_ONLYME, LPVOID(ON_RET_NO));

		}
	}
	else
	{		
		// 辨煽 堅辨塢縑 List蒂 還熱 橈棻.
		SendFieldMsg(ON_RESPONSE_BARRACK_GETDISMISSALSOLDERLIST, SO_SFM_ONLYME, LPVOID(ON_RET_NO));
	}
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	辨煽擊 п堅и棻.
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
VOID	SOPlayer::RecvBarrackDismissalSolder()
{
	OnReqBarrackDismissalSolder		*pOnReqBarrackDismissalSolder;

	pOnReqBarrackDismissalSolder	=	(OnReqBarrackDismissalSolder*)cMsg;

	if(stAccountInfo.pVillage != NULL)
	{
		if( stAccountInfo.pVillage->m_Barrack != NULL )
		{

			// 辨煽 п堅縑 渠и 薑爾朝 Village 贗楚蝶縑憮 爾頂遽棻.
			if(stAccountInfo.pVillage->m_Barrack->BarrackDismissalSolder(this, pOnReqBarrackDismissalSolder->dwSlot) == FALSE)
			{
				// 褒ぬ衛 Message 虜 瞪歎п 遽棻. 撩奢ц擊陽朝 頂睡縑憮 Message蒂 爾魚棻.
				SendFieldMsg(ON_RESPONSE_BARRACK_DISMISSALSOLDER, SO_SFM_ONLYME, LPVOID(ON_RET_NO));
			}
		}
		else
		{
			// 辨煽 堅辨塢縑憮 辨煽擊 п堅й熱 橈棻.
			SendFieldMsg(ON_RESPONSE_BARRACK_DISMISSALSOLDER, SO_SFM_ONLYME, LPVOID(ON_RET_NO));
		}
		
	}
	else
	{		
		// 辨煽 堅辨塢縑憮 辨煽擊 п堅й熱 橈棻.
		SendFieldMsg(ON_RESPONSE_BARRACK_DISMISSALSOLDER, SO_SFM_ONLYME, LPVOID(ON_RET_NO));
	}
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	濰熱辨煽 List蒂 蹂羶и棻.
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
VOID	SOPlayer::RecvGBarrackGetSolderList()
{
	if(stAccountInfo.pVillage != NULL)
	{
		if( stAccountInfo.pVillage->m_Barrack != NULL )
		{

			// 菟橫 鬲擊 唳辦曖 詭衛雖朝 Village 贗楚蝶縑憮 爾頂遽棻.		
			stAccountInfo.pVillage->m_Barrack->GBarrackGetSolderList(this);				
		}
		else
		{
			// 濰熱辨煽 堅辨塢縑 List蒂 還熱 橈棻.
			SendFieldMsg(ON_RESPONSE_GBARRACK_GETSOLDERLIST, SO_SFM_ONLYME, LPVOID(ON_RET_NO));
		}
	}
	else
	{		
		// 濰熱辨煽 堅辨塢縑 List蒂 還熱 橈棻.
		SendFieldMsg(ON_RESPONSE_GBARRACK_GETSOLDERLIST, SO_SFM_ONLYME, LPVOID(ON_RET_NO));
	}
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	濰熱辨煽擊 堅辨и棻.
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
VOID	SOPlayer::RecvGBarrackBuySolder()
{
	OnReqGBarrackBuySolder		*pOnReqGBarrackBuySolder;

	pOnReqGBarrackBuySolder	=	(OnReqGBarrackBuySolder*)cMsg;

	if(stAccountInfo.pVillage != NULL)
	{
		if( stAccountInfo.pVillage->m_Barrack != NULL )
		{

			// 濰熱辨煽 堅辨縑 渠и 薑爾朝 Village 贗楚蝶縑憮 爾頂遽棻.
			if(stAccountInfo.pVillage->m_Barrack->GBarrackBuySolder(this, pOnReqGBarrackBuySolder->dwBuySolderID, (BYTE*)pOnReqGBarrackBuySolder->szName) == FALSE)
			{
				// 褒ぬ衛 Message 虜 瞪歎п 遽棻. 撩奢ц擊陽朝 頂睡縑憮 Message蒂 爾魚棻.
				SendFieldMsg(ON_RESPONSE_GBARRACK_BUYSOLDER, SO_SFM_ONLYME, LPVOID(ON_RET_NO));
			}

		}
		else
		{
			// 濰熱辨煽 堅辨塢縑憮 辨煽擊 堅辨й熱 橈棻.
			SendFieldMsg(ON_RESPONSE_GBARRACK_BUYSOLDER, SO_SFM_ONLYME, LPVOID(ON_RET_NO));
		}
	}
	else
	{		
		// 濰熱辨煽 堅辨塢縑憮 辨煽擊 堅辨й熱 橈棻.
		SendFieldMsg(ON_RESPONSE_GBARRACK_BUYSOLDER, SO_SFM_ONLYME, LPVOID(ON_RET_NO));
	}
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	瞪霜й 濰熱辨煽 List蒂 蹂羶и棻.
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
VOID	SOPlayer::RecvGBarrackGetChangeJobSolderList()
{
	if(stAccountInfo.pVillage != NULL)
	{
		if( stAccountInfo.pVillage->m_Barrack != NULL )
		{

			// 菟橫 鬲擊 唳辦曖 詭衛雖朝 Village 贗楚蝶縑憮 爾頂遽棻.		
			stAccountInfo.pVillage->m_Barrack->GBarrackGetChangeJobSolderList(this);				
		}
		else
		{	
			// 濰熱辨煽 瞪霜塢縑 List蒂 還熱 橈棻.
			SendFieldMsg(ON_RESPONSE_GBARRACK_GETCHANGEJOBSOLDERLIST, SO_SFM_ONLYME, LPVOID(ON_RET_NO));
		}
	}
	else
	{		
		// 濰熱辨煽 瞪霜塢縑 List蒂 還熱 橈棻.
		SendFieldMsg(ON_RESPONSE_GBARRACK_GETCHANGEJOBSOLDERLIST, SO_SFM_ONLYME, LPVOID(ON_RET_NO));
	}
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	濰熱辨煽擊 瞪霜и棻.
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
VOID	SOPlayer::RecvGBarrackChangeJobSolder()
{
	OnReqGBarrackChangeJobSolder		*pOnReqGBarrackChangeJobSolder;

	pOnReqGBarrackChangeJobSolder	=	(OnReqGBarrackChangeJobSolder*)cMsg;

	if(stAccountInfo.pVillage != NULL)
	{
		if( stAccountInfo.pVillage->m_Barrack != NULL )
		{

			// 濰熱辨煽 瞪霜縑 渠и 薑爾朝 Village 贗楚蝶縑憮 爾頂遽棻.
			if(stAccountInfo.pVillage->m_Barrack->GBarrackChangeJobSolder(this, pOnReqGBarrackChangeJobSolder->dwChangeJobSlot, pOnReqGBarrackChangeJobSolder->dwChangeJobKind) == FALSE)
			{
				// 褒ぬ衛 Message 虜 瞪歎п 遽棻. 撩奢ц擊陽朝 頂睡縑憮 Message蒂 爾魚棻.
				SendFieldMsg(ON_RESPONSE_GBARRACK_CHANGEJOBSOLDER, SO_SFM_ONLYME, LPVOID(ON_RET_NO));
			}
		}
		else
		{
			// 濰熱辨煽 堅辨塢縑憮 辨煽擊 瞪霜й熱 橈棻.
			SendFieldMsg(ON_RESPONSE_GBARRACK_CHANGEJOBSOLDER, SO_SFM_ONLYME, LPVOID(ON_RET_NO));
		}
	}
	else
	{		
		// 濰熱辨煽 堅辨塢縑憮 辨煽擊 瞪霜й熱 橈棻.
		SendFieldMsg(ON_RESPONSE_GBARRACK_CHANGEJOBSOLDER, SO_SFM_ONLYME, LPVOID(ON_RET_NO));
	}
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	п堅й 濰熱辨煽 List蒂 蹂羶и棻.
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
VOID	SOPlayer::RecvGBarrackGetDismissalSolderList()
{
	if(stAccountInfo.pVillage != NULL)
	{
		if( stAccountInfo.pVillage->m_Barrack != NULL )
		{

			// 菟橫 鬲擊 唳辦曖 詭衛雖朝 Village 贗楚蝶縑憮 爾頂遽棻.		
			stAccountInfo.pVillage->m_Barrack->GBarrackGetDismissalSolderList(this);				
		}
		else
		{
			// 濰熱辨煽 堅辨塢縑 List蒂 還熱 橈棻.
			SendFieldMsg(ON_RESPONSE_GBARRACK_GETDISMISSALSOLDERLIST, SO_SFM_ONLYME, LPVOID(ON_RET_NO));
		}
	}
	else
	{		
		// 濰熱辨煽 堅辨塢縑 List蒂 還熱 橈棻.
		SendFieldMsg(ON_RESPONSE_GBARRACK_GETDISMISSALSOLDERLIST, SO_SFM_ONLYME, LPVOID(ON_RET_NO));
	}
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	濰熱辨煽擊 п堅и棻.
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
VOID	SOPlayer::RecvGBarrackDismissalSolder()
{
	OnReqGBarrackDismissalSolder		*pOnReqGBarrackDismissalSolder;

	pOnReqGBarrackDismissalSolder	=	(OnReqGBarrackDismissalSolder*)cMsg;

	if(stAccountInfo.pVillage != NULL)
	{
		if( stAccountInfo.pVillage->m_Barrack != NULL )
		{
			// 濰熱辨煽 п堅縑 渠и 薑爾朝 Village 贗楚蝶縑憮 爾頂遽棻.
			if(stAccountInfo.pVillage->m_Barrack->GBarrackDismissalSolder(this, pOnReqGBarrackDismissalSolder->dwSlot) == FALSE)
			{
				// 褒ぬ衛 Message 虜 瞪歎п 遽棻. 撩奢ц擊陽朝 頂睡縑憮 Message蒂 爾魚棻.
				SendFieldMsg(ON_RESPONSE_GBARRACK_DISMISSALSOLDER, SO_SFM_ONLYME, LPVOID(ON_RET_NO));
			}
		}
		else
		{
			// 濰熱辨煽 堅辨塢縑憮 辨煽擊 п堅й熱 橈棻.
			SendFieldMsg(ON_RESPONSE_GBARRACK_DISMISSALSOLDER, SO_SFM_ONLYME, LPVOID(ON_RET_NO));
		}
	}
	else
	{		
		// 濰熱辨煽 堅辨塢縑憮 辨煽擊 п堅й熱 橈棻.
		SendFieldMsg(ON_RESPONSE_GBARRACK_DISMISSALSOLDER, SO_SFM_ONLYME, LPVOID(ON_RET_NO));
	}
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	2離 濰熱 堅辨 List蒂 蹂羶
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
VOID	SOPlayer::RecvGBarrackGetGeneralExList()
{
	if(stAccountInfo.pVillage != NULL)
	{
		if( stAccountInfo.pVillage->m_Barrack != NULL )
		{
			// 菟橫 鬲擊 唳辦曖 詭衛雖朝 Village 贗楚蝶縑憮 爾頂遽棻.		
			stAccountInfo.pVillage->m_Barrack->GBarrackGetGeneralExList(this);
		}
		else
		{
			SendFieldMsg(ON_RESPONSE_GBARRACK_GET_GENERAL_LIST, SO_SFM_ONLYME, LPVOID(ON_RET_NO));
		}
	}
	else
	{		
		SendFieldMsg(ON_RESPONSE_GBARRACK_GET_GENERAL_LIST, SO_SFM_ONLYME, LPVOID(ON_RET_NO));
	}
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	2離 濰熱 堅辨 蹂羶
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
VOID	SOPlayer::RecvGBarrackBuyGeneralEx()
{
	OnReqGBarrackBuySolder		*pOnReqGBarrackBuySolder;

	pOnReqGBarrackBuySolder	= (OnReqGBarrackBuySolder*)cMsg;

	if(stAccountInfo.pVillage != NULL)
	{
		if( stAccountInfo.pVillage->m_Barrack != NULL )
		{
			// 2離 濰熱 堅辨縑 渠и 薑爾朝 Village 贗楚蝶縑憮 爾頂遽棻.
			if(stAccountInfo.pVillage->m_Barrack->GBarrackBuyGeneralEx(this, pOnReqGBarrackBuySolder->dwBuySolderID, (BYTE*)pOnReqGBarrackBuySolder->szName) == FALSE)
			{
				// 褒ぬ衛 Message 虜 瞪歎п 遽棻. 撩奢ц擊陽朝 頂睡縑憮 Message蒂 爾魚棻.
				SendFieldMsg(ON_RESPONSE_GBARRACK_BUY_GENERAL, SO_SFM_ONLYME, LPVOID(ON_RET_NO));
			}
		}
		else
		{
			SendFieldMsg(ON_RESPONSE_GBARRACK_BUY_GENERAL, SO_SFM_ONLYME, LPVOID(ON_RET_NO));
		}
	}
	else
	{		
		SendFieldMsg(ON_RESPONSE_GBARRACK_BUY_GENERAL, SO_SFM_ONLYME, LPVOID(ON_RET_NO));
	}


}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	2離 濰熱 瞪霜 List 蹂羶 
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
VOID	SOPlayer::RecvGBarrackGetChangeGeneralExList()
{
	if(stAccountInfo.pVillage != NULL)
	{
		if( stAccountInfo.pVillage->m_Barrack != NULL )
		{
			// 菟橫 鬲擊 唳辦曖 詭衛雖朝 Village 贗楚蝶縑憮 爾頂遽棻.		
			stAccountInfo.pVillage->m_Barrack->GBarrackGetChangeGeneralExList(this);				
		}
		else
		{	
			SendFieldMsg(ON_RESPONSE_GBARRACK_GET_CHANGE_GENERAL_LIST, SO_SFM_ONLYME, LPVOID(ON_RET_NO));
		}
	}
	else
	{		
		SendFieldMsg(ON_RESPONSE_GBARRACK_GET_CHANGE_GENERAL_LIST, SO_SFM_ONLYME, LPVOID(ON_RET_NO));
	}
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	2離 濰熱 瞪霜 蹂羶
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
VOID	SOPlayer::RecvGBarrackChangeGeneralEx()
{
	OnReqGBarrackChangeJobSolder*		pOnReqGBarrackChangeJobSolder;

	pOnReqGBarrackChangeJobSolder	=	(OnReqGBarrackChangeJobSolder*)cMsg;

	if(stAccountInfo.pVillage != NULL)
	{
		if( stAccountInfo.pVillage->m_Barrack != NULL )
		{

			// 濰熱辨煽 瞪霜縑 渠и 薑爾朝 Village 贗楚蝶縑憮 爾頂遽棻.
			if(stAccountInfo.pVillage->m_Barrack->GBarrackChangeGeneralEx(this, pOnReqGBarrackChangeJobSolder->dwChangeJobSlot, pOnReqGBarrackChangeJobSolder->dwChangeJobKind) == FALSE)
			{
				// 褒ぬ衛 Message 虜 瞪歎п 遽棻. 撩奢ц擊陽朝 頂睡縑憮 Message蒂 爾魚棻.
				SendFieldMsg(ON_RESPONSE_GBARRACK_CHANGE_GENERAL, SO_SFM_ONLYME, LPVOID(ON_RET_NO));
			}
		}
		else
		{
			SendFieldMsg(ON_RESPONSE_GBARRACK_CHANGE_GENERAL, SO_SFM_ONLYME, LPVOID(ON_RET_NO));
		}
	}
	else
	{		
		SendFieldMsg(ON_RESPONSE_GBARRACK_CHANGE_GENERAL, SO_SFM_ONLYME, LPVOID(ON_RET_NO));
	}

}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	2離 濰熱 п堅 List 蹂羶
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
VOID	SOPlayer::RecvGBarrackGetDismissalGeneralExList()
{
	if(stAccountInfo.pVillage != NULL)
	{
		if( stAccountInfo.pVillage->m_Barrack != NULL )
		{
			// 菟橫 鬲擊 唳辦曖 詭衛雖朝 Village 贗楚蝶縑憮 爾頂遽棻.		
			stAccountInfo.pVillage->m_Barrack->GBarrackGetDismissalGeneralExList(this);				
		}
		else
		{
			// 濰熱辨煽 堅辨塢縑 List蒂 還熱 橈棻.
			SendFieldMsg(ON_RESPONSE_GBARRACK_GETDISMISSAL_GENERAL_LIST, SO_SFM_ONLYME, LPVOID(ON_RET_NO));
		}
	}
	else
	{		
		// 濰熱辨煽 堅辨塢縑 List蒂 還熱 橈棻.
		SendFieldMsg(ON_RESPONSE_GBARRACK_GETDISMISSAL_GENERAL_LIST, SO_SFM_ONLYME, LPVOID(ON_RET_NO));
	}
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	2離 濰熱 п堅 蹂羶
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
VOID	SOPlayer::RecvGBarrackDismissalGeneralEx()
{
	OnReqGBarrackDismissalSolder*			pOnReqGBarrackDismissalSolder;

	pOnReqGBarrackDismissalSolder	=		(OnReqGBarrackDismissalSolder*)cMsg;

	if(stAccountInfo.pVillage != NULL)
	{
		if( stAccountInfo.pVillage->m_Barrack != NULL )
		{
			// 濰熱辨煽 п堅縑 渠и 薑爾朝 Village 贗楚蝶縑憮 爾頂遽棻.
			if(stAccountInfo.pVillage->m_Barrack->GBarrackSellDismissalGeneralEx(this, pOnReqGBarrackDismissalSolder->dwSlot) == FALSE)
			{
				// 褒ぬ衛 Message 虜 瞪歎п 遽棻. 撩奢ц擊陽朝 頂睡縑憮 Message蒂 爾魚棻.
				SendFieldMsg(ON_RESPONSE_GBARRACK_DISMISSAL_GENERAL, SO_SFM_ONLYME, LPVOID(ON_RET_NO));
			}
		}
		else
		{
			// 濰熱辨煽 堅辨塢縑憮 辨煽擊 п堅й熱 橈棻.
			SendFieldMsg(ON_RESPONSE_GBARRACK_DISMISSAL_GENERAL, SO_SFM_ONLYME, LPVOID(ON_RET_NO));
		}
	}
	else
	{		
		// 濰熱辨煽 堅辨塢縑憮 辨煽擊 п堅й熱 橈棻.
		SendFieldMsg(ON_RESPONSE_GBARRACK_DISMISSAL_GENERAL, SO_SFM_ONLYME, LPVOID(ON_RET_NO));
	}
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	跨蝶攪 List蒂 蹂羶и棻.
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
VOID	SOPlayer::RecvMBarrackGetMonsterList()
{
	if( stAccountInfo.pVillage != NULL )
	{
		if( stAccountInfo.pVillage->m_Barrack != NULL )
		{

			// 菟橫 鬲擊 唳辦曖 詭衛雖朝 Village 贗楚蝶縑憮 爾頂遽棻.		
			stAccountInfo.pVillage->m_Barrack->MBarrackGetMonsterList( this );				
		}
		else
		{
			SendFieldMsg( ON_RESPONSE_MBARRACK_GETMONSTERLIST, SO_SFM_ONLYME, LPVOID(ON_RET_NO) );
		}
	}
	else
	{
		SendFieldMsg( ON_RESPONSE_MBARRACK_GETMONSTERLIST, SO_SFM_ONLYME, LPVOID(ON_RET_NO) );
	}
}


//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	跨蝶攪蒂 堅辨и棻.
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
VOID	SOPlayer::RecvMBarrackBuyMonster()
{
	OnReqMBarrackBuyMonster		*pOnReqMBarrackBuyMonster;

	pOnReqMBarrackBuyMonster	=	(OnReqMBarrackBuyMonster*)cMsg;

	if( stAccountInfo.pVillage != NULL )
	{
		if( stAccountInfo.pVillage->m_Barrack != NULL )
		{

			// 跨蝶攪 堅辨縑 渠и 薑爾朝 Village 贗楚蝶縑憮 爾頂遽棻.
			if( stAccountInfo.pVillage->m_Barrack->MBarrackBuyMonster(this, pOnReqMBarrackBuyMonster->dwBuyMonsterID, (BYTE*)pOnReqMBarrackBuyMonster->szName) == FALSE )
			{
				// 褒ぬ衛 Message 虜 瞪歎п 遽棻. 撩奢ц擊陽朝 頂睡縑憮 Message蒂 爾魚棻.
				SendFieldMsg( ON_RESPONSE_MBARRACK_BUYMONSTER, SO_SFM_ONLYME, LPVOID(ON_RET_NO) );
			}
		}
		else
		{
			// 跨蝶攪 堅辨塢縑憮 跨蝶攪蒂 堅辨й熱 橈棻.
			SendFieldMsg( ON_RESPONSE_MBARRACK_BUYMONSTER, SO_SFM_ONLYME, LPVOID(ON_RET_NO) );
		}

	}
	else
	{		
		// 跨蝶攪 堅辨塢縑憮 跨蝶攪蒂 堅辨й熱 橈棻.
		SendFieldMsg( ON_RESPONSE_MBARRACK_BUYMONSTER, SO_SFM_ONLYME, LPVOID(ON_RET_NO) );
	}
}


//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	п堅й 跨蝶攪 List蒂 蹂羶и棻.
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
VOID	SOPlayer::RecvMBarrackGetDismissMonsterList()
{
	if( stAccountInfo.pVillage != NULL )
	{
		if( stAccountInfo.pVillage->m_Barrack != NULL )
		{

			// 菟橫 鬲擊 唳辦曖 詭衛雖朝 Village 贗楚蝶縑憮 爾頂遽棻.		
			stAccountInfo.pVillage->m_Barrack->MBarrackGetDismissMonsterList( this );				
		}
		else
		{
			// 跨蝶攪 堅辨塢縑 List蒂 還熱 橈棻.
			SendFieldMsg( ON_RESPONSE_MBARRACK_GETDISMISSMONSTERLIST, SO_SFM_ONLYME, LPVOID(ON_RET_NO) );
		}

	}
	else
	{		
		// 跨蝶攪 堅辨塢縑 List蒂 還熱 橈棻.
		SendFieldMsg( ON_RESPONSE_MBARRACK_GETDISMISSMONSTERLIST, SO_SFM_ONLYME, LPVOID(ON_RET_NO) );
	}
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	跨蝶攪蒂 п堅и棻.
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
VOID	SOPlayer::RecvMBarrackDismissMonster()
{
	OnReqMBarrackDismissMonster			*pOnReqMBarrackDismissMonster;

	pOnReqMBarrackDismissMonster	=	(OnReqMBarrackDismissMonster*)cMsg;

	if( stAccountInfo.pVillage != NULL )
	{
		if( stAccountInfo.pVillage->m_Barrack != NULL )
		{

			// 跨蝶攪 п堅縑 渠и 薑爾朝 Village 贗楚蝶縑憮 爾頂遽棻.
			if( stAccountInfo.pVillage->m_Barrack->MBarrackDismissMonster(this, pOnReqMBarrackDismissMonster->dwSlot) == FALSE )
			{
				// 褒ぬ衛 Message 虜 瞪歎п 遽棻. 撩奢ц擊陽朝 頂睡縑憮 Message蒂 爾魚棻.
				SendFieldMsg( ON_RESPONSE_MBARRACK_DISMISSMONSTER, SO_SFM_ONLYME, LPVOID(ON_RET_NO) );
			}
		}
		else
		{
			// 跨蝶攪 堅辨塢縑憮 跨蝶攪蒂 п堅й熱 橈棻.
			SendFieldMsg( ON_RESPONSE_MBARRACK_DISMISSMONSTER, SO_SFM_ONLYME, LPVOID(ON_RET_NO) );			
		}
	}
	else
	{		
		// 跨蝶攪 堅辨塢縑憮 跨蝶攪蒂 п堅й熱 橈棻.
		SendFieldMsg( ON_RESPONSE_MBARRACK_DISMISSMONSTER, SO_SFM_ONLYME, LPVOID(ON_RET_NO) );
	}
}


//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	鱔濰偃熱蒂 橢橫螞棻.
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
SI32	SOPlayer::GetBankBootNum()
{
	SI32			siNum;
	if(pMyWorld->clDatabase->GetDBInfo(SO_DB_INFOKIND_BANKBOOK_NUM, this, LPVOID(&siNum)) == FALSE)
		siNum	=	-1;
	return siNum;
}


//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	褐辨 氬爾 渠轎 譆渠 旎擋.
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
VOID	SOPlayer::RecvMaxCreditLoan()
{		
	BOOL			bResult = ON_RET_AUDIT_NO;

	if( GetTotalProperty( stPropertyData ) == ON_RET_BANK_OK )
	{
		if( stPropertyData.siTotalProperty > 0 )
		{
			uiMaxLoan	=	uiTradeCredit;

			if( uiMaxLoan > 0 )
			{
				//siMaxLoan擊 盪濰п撿 и棻.
				SendFieldMsg(ON_RESPONSE_BANK_MAXCREDITLOAN, SO_SFM_ONLYME, LPVOID(ON_RET_BANK_OK), LPVOID(uiMaxLoan),  LPVOID(0));  
				return;
			}
		}
		bResult	=	ON_RET_BANK_NO;
	}
	if( bResult	==	ON_RET_BANK_NO )
		SendFieldMsg(ON_RESPONSE_BANK_MAXCREDITLOAN, SO_SFM_ONLYME, LPVOID(ON_RET_BANK_NO));
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	辨煽 纂猿List蒂 蹂羶и棻.
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
VOID	SOPlayer::RecvHospitalGetCureList()
{
	if(stAccountInfo.pVillage != NULL)
	{
		// 菟橫 鬲擊 唳辦曖 詭衛雖朝 Village 贗楚蝶縑憮 爾頂遽棻.		
		stAccountInfo.pVillage->m_Hospital->HospitalGetCureList(this);				
	}
	else
	{		
		// 辨煽 纂猿塢縑 List蒂 還熱 橈棻.
		SendFieldMsg(ON_RESPONSE_HOSPITAL_GETCURELIST, SO_SFM_ONLYME);
	}
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	辨煽擊 纂猿и棻.
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
VOID	SOPlayer::RecvHospitalCureSolder()
{
	OnReqHospitalCureSolder		*pOnReqHospitalCureSolder;

	pOnReqHospitalCureSolder	=	(OnReqHospitalCureSolder*)cMsg;

	if(stAccountInfo.pVillage != NULL)
	{
		// 辨煽 纂猿縑 渠и 薑爾朝 Village 贗楚蝶縑憮 爾頂遽棻.
		if(stAccountInfo.pVillage->m_Hospital->HospitalCureSolder(this, pOnReqHospitalCureSolder->dwCureSolderSlot) == FALSE)
		{
			// 褒ぬ衛 Message 虜 瞪歎п 遽棻. 撩奢ц擊陽朝 頂睡縑憮 Message蒂 爾魚棻.
			SendFieldMsg(ON_RESPONSE_HOSPITAL_CURESOLDER, SO_SFM_ONLYME, LPVOID(ON_RET_NO));
		}
	}
	else
	{		
		// 辨煽 纂猿塢縑憮 辨煽擊 纂猿й熱 橈棻.
		SendFieldMsg(ON_RESPONSE_HOSPITAL_CURESOLDER, SO_SFM_ONLYME, LPVOID(ON_RET_NO));
	}
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	賅萇 辨煽擊 纂猿и棻.
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
VOID	SOPlayer::RecvHospitalCureAllSolder()
{
	OnReqHospitalCureAllSolder		*pOnReqHospitalCureAllSolder;

	pOnReqHospitalCureAllSolder	=	(OnReqHospitalCureAllSolder*)cMsg;

	if(stAccountInfo.pVillage != NULL)
	{
		// 辨煽 纂猿縑 渠и 薑爾朝 Village 贗楚蝶縑憮 爾頂遽棻.
		if(stAccountInfo.pVillage->m_Hospital->HospitalCureAllSolder(this) == FALSE)
		{
			// 褒ぬ衛 Message 虜 瞪歎п 遽棻. 撩奢ц擊陽朝 頂睡縑憮 Message蒂 爾魚棻.
			SendFieldMsg(ON_RESPONSE_HOSPITAL_CUREALLSOLDER, SO_SFM_ONLYME, LPVOID(ON_RET_NO));
		}
	}
	else
	{		
		// 辨煽 纂猿塢縑憮 辨煽擊 纂猿й熱 橈棻.
		SendFieldMsg(ON_RESPONSE_HOSPITAL_CUREALLSOLDER, SO_SFM_ONLYME, LPVOID(ON_RET_NO));
	}
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	睡舒縑 菟橫馬
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
VOID	SOPlayer::RecvWharfEnter()
{
	if(stAccountInfo.pVillage != NULL)
	{
		// 菟橫 鬲擊 唳辦曖 詭衛雖朝 Village 贗楚蝶縑憮 爾頂遽棻.		
		stAccountInfo.pVillage->m_Wharf->WharfEnter(this);				
	}
	else
	{		
		// 睡舒煎 菟橫陞熱 橈棻.
		SendFieldMsg(ON_RESPONSE_WHARF_ENTER, SO_SFM_ONLYME, LPVOID(ON_RET_NO));
	}
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	ル蒂 鳴
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
VOID	SOPlayer::RecvWharfBuyTicket()
{
	OnReqWharf_BuyTicket*			lpOnReqWharf_BuyTicket;

	if(stAccountInfo.pVillage != NULL)
	{
		// 菟橫 鬲擊 唳辦曖 詭衛雖朝 Village 贗楚蝶縑憮 爾頂遽棻.		
		lpOnReqWharf_BuyTicket = (OnReqWharf_BuyTicket*)cMsg;

		stAccountInfo.pVillage->m_Wharf->WharfBuyTicket(this, lpOnReqWharf_BuyTicket->siTicketKind);
	}
	else
	{		
		// 睡舒縑憮 Ticket擊 髦熱 橈棻.
		SendFieldMsg(ON_RESPONSE_WHARF_BUYTICKET, SO_SFM_ONLYME, LPVOID(ON_RET_NO));
	}
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	睡舒蒂 釭馬
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
VOID	SOPlayer::RecvWharfExit()
{
	if(stAccountInfo.pVillage != NULL)
	{
		// 菟橫 鬲擊 唳辦曖 詭衛雖朝 Village 贗楚蝶縑憮 爾頂遽棻.		
		stAccountInfo.pVillage->m_Wharf->WharfExit(this);				
	}
	else
	{		
		// 睡舒縑憮 釭陞熱 橈棻.
		SendFieldMsg(ON_RESPONSE_WHARF_EXIT, SO_SFM_ONLYME, LPVOID(ON_RET_NO));
	}
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	睡舒 渠晦褒縑 菟橫馬
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
VOID	SOPlayer::RecvWharfWaitRoomEnter()
{
	if(stAccountInfo.pVillage != NULL)
	{
		// 菟橫 鬲擊 唳辦曖 詭衛雖朝 Village 贗楚蝶縑憮 爾頂遽棻.		
		stAccountInfo.pVillage->m_Wharf->WharfWaitRoomEnter(this);				
	}
	else
	{		
		// 睡舒渠晦褒縑 菟橫陞熱 橈棻.
		SendFieldMsg(ON_RESPONSE_WHARFWAITROOM_ENTER, SO_SFM_ONLYME, LPVOID(ON_RET_NO));
	}
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	睡舒 渠晦褒擊 釭馬
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
VOID	SOPlayer::RecvWharfWaitRoomExit()
{
	if(stAccountInfo.pVillage != NULL)
	{
		// 菟橫 鬲擊 唳辦曖 詭衛雖朝 Village 贗楚蝶縑憮 爾頂遽棻.		
		stAccountInfo.pVillage->m_Wharf->WharfWaitRoomExit(this);				
	}
	else
	{		
		// 睡舒渠晦褒縑憮 釭陞熱 橈棻.
		SendFieldMsg(ON_RESPONSE_WHARFWAITROOM_EXIT, SO_SFM_ONLYME, LPVOID(ON_RET_NO));
	}
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	DB縑憮 Ticket 薑爾蒂 橢橫褥
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
VOID	SOPlayer::WharfGetDB()
{
	DBIKGetTicket		TempDBIKGetTicket;
	DBIKGetTicketInfo	TempDBIKGetTicketInfo;

	TempDBIKGetTicket.DBAccount    = stAccountInfo.siDBAccount;
	TempDBIKGetTicket.uiSlot       = stAccountInfo.uiSlot;
	clServer->clDatabase->GetDBInfo(SO_DB_INFOKIND_GETTICKET, LPVOID(&TempDBIKGetTicket), LPVOID(&TempDBIKGetTicketInfo));
	stTicketInfo.siTicketKind = TempDBIKGetTicketInfo.siTicketKind;
}

VOID	SOPlayer::RecvGovernmentEnter()
{
	// 鼠褻勒 OK
	SendFieldMsg(ON_RESPONSE_GOVERNMENT_MAIN_ENTER, SO_SFM_ONLYME, LPVOID(ON_RET_OK));
}

VOID	SOPlayer::RecvGovernmentLeave()
{
	// 鼠褻勒 OK
	SendFieldMsg(ON_RESPONSE_GOVERNMENT_MAIN_LEAVE, SO_SFM_ONLYME, LPVOID(ON_RET_OK));
}

VOID	SOPlayer::RecvGovernmentInvestmentEnter()
{
	OnReqGovernmentEnterInvestment*			lpOnReqGovernmentEnterInvestment;

	lpOnReqGovernmentEnterInvestment = (OnReqGovernmentEnterInvestment*)cMsg;
	if(stAccountInfo.pVillage != NULL)
	{
		// 菟橫 鬲擊 唳辦曖 詭衛雖朝 Village 贗楚蝶縑憮 爾頂遽棻.		
		stAccountInfo.pVillage->m_Government->GovernmentInvestmentEnter(this);				
	}
	else
	{		
		SendFieldMsg(ON_RESPONSE_GOVERNMENT_INVESTMENT_ENTER, SO_SFM_ONLYME, LPVOID(ON_RET_NO));
	}
}

VOID	SOPlayer::RecvGovernmentInvestmentLeave()
{
	if(stAccountInfo.pVillage != NULL)
	{
		// 菟橫 鬲擊 唳辦曖 詭衛雖朝 Village 贗楚蝶縑憮 爾頂遽棻.		
		stAccountInfo.pVillage->m_Government->GovernmentInvestmentLeave(this);				
	}
	else
	{		
		SendFieldMsg(ON_RESPONSE_GOVERNMENT_INVESTMENT_LEAVE, SO_SFM_ONLYME, LPVOID(ON_RET_OK));
	}
}

VOID	SOPlayer::RecvGovernmentInvestmentGetList()
{
	SI16		siVillageCode;
	OnReqGovernmentGetInvestmentList*			lpOnReqGovernmentGetInvestmentList;

	lpOnReqGovernmentGetInvestmentList = (OnReqGovernmentGetInvestmentList*)cMsg;
	siVillageCode	=	lpOnReqGovernmentGetInvestmentList->siVillageCode;

	// siVillageCode縑 -1檜 菟橫螃賊 Ы溯檜橫陛 ⑷營 菟橫陛 氈朝 葆擊曖 癱濠濠 薑爾蒂 爾頂遽棻.
	// siVillageCode陛 -1檜 嬴棋 高檜賊 Ы溯檜橫陛 菟橫陛 氈朝 葆擊檜 嬴棋 siVillageCode曖 癱濠濠 薑爾蒂 爾頂遽棻.
	if( siVillageCode != -1 )
	{
		for( int i = 0; i < pMyWorld->siTotalVillageNum; i++ )
		{
			if( pMyWorld->pclVillage[i].uiVillageCode == siVillageCode )
			{
				pMyWorld->pclVillage[i].m_Government->GovernmentInvestmentGetList(this);
			}
		}
	}
	else if(stAccountInfo.pVillage != NULL)
	{
		// 菟橫 鬲擊 唳辦曖 詭衛雖朝 Village 贗楚蝶縑憮 爾頂遽棻.		
		stAccountInfo.pVillage->m_Government->GovernmentInvestmentGetList(this);				
	}
	else
	{		
		SendFieldMsg(ON_RESPONSE_GOVERNMENT_INVESTMENT_GETLIST, SO_SFM_ONLYME, LPVOID(NULL), LPVOID(0xffff));
	}
}

VOID	SOPlayer::RecvGovernmentInvestmentSetInvestment()
{
	OnReqGovernmentSetInvestment*		lpOnReqGovernmentSetInvestment;

	lpOnReqGovernmentSetInvestment = (OnReqGovernmentSetInvestment*)cMsg;
	if(stAccountInfo.pVillage != NULL)
	{
		// 菟橫 鬲擊 唳辦曖 詭衛雖朝 Village 贗楚蝶縑憮 爾頂遽棻.		
		//if(stAccountInfo.pVillage->m_Government->GovernmentInvestmentSetInvestment(this, lpOnReqGovernmentSetInvestment->m_InvestmentMoney) == FALSE)
		if(stAccountInfo.pVillage->m_Government->GovernmentInvestmentSetInvestment(this, lpOnReqGovernmentSetInvestment->m_InvestmentMoney,lpOnReqGovernmentSetInvestment->m_InvestmentSaveMoney) == FALSE)
		{
			SendFieldMsg(ON_RESPONSE_GOVERNMENT_INVESTMENT_SETINVESTMENT, SO_SFM_ONLYME, LPVOID(ON_RET_NO));
		}
	}
	else
	{		
		SendFieldMsg(ON_RESPONSE_GOVERNMENT_INVESTMENT_SETINVESTMENT, SO_SFM_ONLYME, LPVOID(ON_RET_NO));
	}
}


VOID	SOPlayer::RecvWriteDescript()
{
	OnReqWriteDescript		*pWriteDescript;
	char					Temp[256];
	char					*Content;
	UI32					uiResult = ON_RET_DESCRIPT_NO;

	if(stAccountInfo.pVillage != NULL)
	{
		pWriteDescript			=	(OnReqWriteDescript*)	cMsg;
		Content		=	(CHAR*)pWriteDescript + sizeof(OnReqWriteDescript);
		memset( Temp, 0, sizeof(Temp));
		memcpy( Temp, Content,pWriteDescript->uiLength);
		
		strtok(Temp, ON_FORBIDDEN_CHAR);
		/*
			SendFieldMsg(ON_RESPONSE_WRITE_DESCRIPT, SO_SFM_ONLYME, LPVOID(ON_RET_DESCRIPT_NO));
			printf("寰頂僥-旎雖脹僥濠 餌辨[%s]-[%s]-[%c]\n",Temp,cTemp,*cTemp);
			return;
		*/
		uiResult = stAccountInfo.pVillage->m_Government->WriteDescription(this, Temp);
	}
	SendFieldMsg(ON_RESPONSE_WRITE_DESCRIPT, SO_SFM_ONLYME, LPVOID(uiResult));
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	婦羶, 葆擊薑爾 橢晦
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
VOID	SOPlayer::RecvGovernmentVillageInfo()
{
	if(stAccountInfo.pVillage != NULL)
	{
		stAccountInfo.pVillage->GetVillageInfo(this);
	}
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	婦羶, 癱濠и絲 陛雖堅 釭陛晦
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
VOID	SOPlayer::RecvGovernmentInvestmentGetOut()
{
	OnReqGovernmentInvestmentGetOut*			lpOnReqGovernmentInvestmentGetOut;

	lpOnReqGovernmentInvestmentGetOut = (OnReqGovernmentInvestmentGetOut*)cMsg;
	if(stAccountInfo.pVillage != NULL)
	{
		stAccountInfo.pVillage->m_Government->InvestmentGetOut(this, lpOnReqGovernmentInvestmentGetOut->GetOutMoney);
	}
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	賃喇除 葆擊曖 熱櫛 鱔啗 爾晦.
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
VOID	SOPlayer::RecvProfitStatistics()
{
	stCapital		*pstCapital;

	if(stAccountInfo.pVillage != NULL)
	{
		pstCapital	=	stAccountInfo.pVillage->m_Government->GetVillageCapital();
		SendFieldMsg( ON_RESPONSE_PROFIT_STATISTICS, SO_SFM_ONLYME, LPVOID(pstCapital) );
		//stAccountInfo.pVillage->m_Government->SendVillageCapital(this);
	}
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	葆擊寞橫溘縑 癱濠й熱 氈朝 薑爾蒂 橢朝棻
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
VOID	SOPlayer::RecvReqMaxVillageDefenceInvestment()
{
	if(stAccountInfo.pVillage != NULL)
	{
		stAccountInfo.pVillage->m_Government->SendVillageReqMaxVillageDefenceInvestment(this);
	}
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	葆擊 寞橫溘縑 癱濠ж晦
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
VOID	SOPlayer::RecvReqSetVillageDefenceInvestment()
{
	OnReqSetVillageDefenceInvestment*		lpOnReqSetVillageDefenceInvestment;

	lpOnReqSetVillageDefenceInvestment = (OnReqSetVillageDefenceInvestment*)cMsg;
	if(stAccountInfo.pVillage != NULL)
	{
		stAccountInfo.pVillage->m_Government->SendVillageReqSetVillageDefenceInvestment(this, lpOnReqSetVillageDefenceInvestment->InvestmentMoney);
	}
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	鼻欽 儅撩縑 в蹂и 綠辨擊 蹂掘ж朝 詭衛雖蒂 嫡懊棻.
//	熱薑 橾濠	:	2003-01-27 螃�� 2:18:10 - 寥鼻⑷
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
VOID	SOPlayer::RecvReqGuildCreateCost()
{
	MONEY	mnGuildCreateCost	=	pMyWorld->clIGuild.GetGuildCreateCost();
	SendFieldMsg(ON_RESPONSE_GUILD_CREATE_COST, SO_SFM_ONLYME, (LPVOID*)&mnGuildCreateCost );
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	望萄蒂 儅撩и棻朝 詭衛雖蒂 嫡懊棻.
//	熱薑 橾濠	:	2002-02-25 螃瞪 11:26:10 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
VOID	SOPlayer::RecvCreateGuild()
{

	// DBм艦夠

	OnReqCreateGuildMsg		*pCreateGuildMsg;
	CHAR					*pGuildName;	
	BOOL					bDBSuccess = FALSE;		
	CHAR					szCharID[ON_ID_LENGTH + 1];
	CHAR					szGuildName[ON_GUILDNAME_LENGTH + 1];
	BOOL					bCreateGuildSuccess = FALSE;
	MONEY					mnGuildCreateCost;

	if(stAccountInfo.pVillage == NULL)														goto	END;	// 葆擊縑 氈雖 彊戲賊 鼻欽擊 儅撩й 熱 橈棻.
	if(stAccountInfo.pVillage->uiGuildID !=0)												goto	END;	// ⑷營 鼻欽擊 虜菟溥堅 ж朝 葆擊縑 檜嘐 棻艇 鼻欽檜 氈戲賊 鼻欽擊 虜菟熱陛 橈棻.
	// 濠褐檜 棻艇鼻欽縑 鼻欽錳檜賊 鼻欽擊 虜菟熱橈棻.
	if (uiGuildID != 0 ) goto END;
	
	// 頂陛 檜 葆擊曖 譆堅 癱濠濠檣雖 獄棻.
	ZeroMemory(szCharID, sizeof(szCharID));
	strncpy(szCharID, szID, ON_ID_LENGTH);
	if(stAccountInfo.pVillage->m_Government->CheckInvestmentMaxInvestmentorName(szCharID, 0) == FALSE)	goto END;		

	// 望萄 儅撩衛 в蹂и 濠旎
	mnGuildCreateCost = pMyWorld->clIGuild.GetGuildCreateCost();

	// 望萄 儅撩衛 絲檜 醱碟�� 氈橫撿 и棻.
	if(EnoughMoney(mnGuildCreateCost) == FALSE)												goto	END;

	pCreateGuildMsg	=	(OnReqCreateGuildMsg*)cMsg;	
	pGuildName		=	(CHAR*)((CHAR*)pCreateGuildMsg + sizeof(OnReqCreateGuildMsg));	
	
	ZeroMemory(szGuildName, sizeof(szGuildName));
	strncpy(szGuildName, pGuildName, min(pCreateGuildMsg->uiGuildNameLength, ON_GUILDNAME_LENGTH));

	// 鼻欽 檜葷檜 嶸�褲捘� っ欽и棻.
	if(CheckForbiddenName(szGuildName) == FALSE)											goto	END;
	if(pMyWorld->clIGuild.IsUsedName(szGuildName) == TRUE )                                 goto    END;

	

	uiGuildID		=	pMyWorld->clIGuild.CreateGuild(szGuildName, this, stAccountInfo.pVillage);

	
	if(uiGuildID == 0)																		goto	END;
	
	ChangeMoney(-mnGuildCreateCost,FALSE);

	// 撩奢瞳戲煎 鼻欽擊 儅撩ц棻.
	bCreateGuildSuccess	=	TRUE;	

END:
	if(bCreateGuildSuccess == TRUE)
	{	
		// 鼻欽擊 儅撩ж朝等 撩奢ц棻.		
		SendFieldMsg(ON_RESPONSE_CREATE_GUILD, SO_SFM_ONLYME, LPVOID(ON_RET_OK));
	}
	else
	{
		// 鼻欽擊 儅撩ж朝等 褒ぬц棻.
		SendFieldMsg(ON_RESPONSE_CREATE_GUILD, SO_SFM_ONLYME, LPVOID(ON_RET_NO));
	}
}


//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	鼻欽擊 餉薯и棻朝 詭衛雖蒂 嫡懊棻
//	熱薑 橾濠	:	2002-02-25 螃瞪 11:26:05 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
VOID	SOPlayer::RecvDeleteGuild()
{
	OnReqDeleteGuildMsg		*pDeleteGuildMsg;	

	pDeleteGuildMsg		=	(OnReqDeleteGuildMsg*)cMsg;	

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// for debug
	if( uiGuildID == 0 )
	{
//		pMyWorld->clServer->WriteInfo( "..\\data\\DeletedGuild.txt", "Received current message: 'ON_REQUEST_DELETE_GUILD', But the Guild ID is 0" );
		writeInfoToFile( "DeletedGuild.txt", "Received current message: 'ON_REQUEST_DELETE_GUILD', But the Guild ID is 0" );
	}
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//奢撩ж朝 陳檣陛
	if(pMyWorld->m_WarTimeParser.IsWarCheck(pMyWorld->pRealTime->tm_wday, pMyWorld->pRealTime->tm_hour))
	{
		if(pMyWorld->IsAttackVillageTimeZone()) //奢撩 衛除檣雖 羹觼
		{
			SendFieldMsg(ON_RESPONSE_DELETE_GUILD, SO_SFM_ONLYME, LPVOID(ON_RET_NO));
			return;
		}
	}

	//瞳渠陛 1偃塭紫 勘萼 鼻欽擎 鼻欽擊 п羹й熱 橈棻.
	if(pMyWorld->clIGuild.GetGuildWarCheck(uiGuildID)) 
	{  
		SendFieldMsg(ON_RESPONSE_DELETE_GUILD, SO_SFM_ONLYME, LPVOID(ON_RET_NO));
		return;
	}

	// Guild ID陛 嶸�褲捘� 羹觼и棻.
	if(pMyWorld->clIGuild.DeleteGuild(uiGuildID, stAccountInfo.pVillage) == TRUE)
	{
		// 鼻欽擊 餉薯ж朝等 撩奢ц棻.		
		SendFieldMsg(ON_RESPONSE_DELETE_GUILD, SO_SFM_ONLYME, LPVOID(ON_RET_OK));
	}		
	else
	{
		// 鼻欽擊 餉薯ж朝等 褒ぬц棻.
		SendFieldMsg(ON_RESPONSE_DELETE_GUILD, SO_SFM_ONLYME, LPVOID(ON_RET_NO));
	}	
}


//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	鼻欽縑 霤罹ж剪釭 集陪棻.
//	熱薑 橾濠	:	2002-03-04 螃瞪 10:41:08 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
VOID	SOPlayer::RecvJoinLeaveGuild()
{
	OnReqJoinLeaveGuildMsg		*pJoinLeaveGuildMsg;	
	BOOL						bDBSuccess = FALSE, bSuccess = FALSE;
	BOOL						bDeleteGuild	=	FALSE;	
	BOOL						bCandidate;
	DWORD						dwResponse = ON_RET_NO;

	pJoinLeaveGuildMsg	=	(OnReqJoinLeaveGuildMsg*)cMsg;

    //奢撩檜 氈朝陳檜賊憮 奢撩衛除縑朝 鼻欽陛殮, 驍黴陛 彊脹棻.
	if(pMyWorld->m_WarTimeParser.IsWarCheck(pMyWorld->pRealTime->tm_wday, pMyWorld->pRealTime->tm_hour))
	if(pMyWorld->m_WarTimeParser.IsWarTime(pMyWorld->pRealTime->tm_hour))
	{
		SendFieldMsg(ON_RESPONSE_JOIN_LEAVE_GUILD, SO_SFM_ONLYME, LPVOID(dwResponse));
		return;
	}

	if(uiGuildID == 0)
	{
		// 霤罹蒂 п撿 и棻.
		dwResponse		=	ON_RET_GLD_JOIN_NO;

		// ⑷營 葆擊檜 嶸�褲捘� 憲嬴螞棻.
		if(stAccountInfo.pVillage != NULL)
		{
			// 斜 葆擊曖 Guild ID蒂 掘и棻.
			if(stAccountInfo.pVillage->uiGuildID != 0)
			{
				// 鼻欽縑 雖錳и棻.
				if(pMyWorld->clIGuild.AddCandidate(stAccountInfo.pVillage->uiGuildID, this) == TRUE)									
					dwResponse			=	ON_RET_GLD_JOIN_OK;												
			}
		}
	}
	else
	{
		// 霤罹и 鼻欽縑憮 集陪棻.
		dwResponse			=	ON_RET_GLD_LEAVE_NO;

		// ⑷營 葆擊檜 嶸�褲捘� 憲嬴螞棻.
		if(stAccountInfo.pVillage != NULL)
		{
			// 斜 葆擊曖 Guild ID諦 濠晦曖 Guild ID諦 偽嬴撿 и棻.
			// 闊 濠晦 鼻欽檜 爾嶸ж堅 氈朝 葆擊檜橫撿 и棻.
			if(stAccountInfo.pVillage->uiGuildID == uiGuildID)
			{
				if(pMyWorld->m_WarTimeParser.IsWarCheck(pMyWorld->pRealTime->tm_wday, pMyWorld->pRealTime->tm_hour)) return;
				if(pMyWorld->m_WarTimeParser.IsWarTime(pMyWorld->pRealTime->tm_hour)) return;
								
				// 渠寞檣雖 獄棻.
				if(siClassInGuild == pMyWorld->clIGuildClass.GetNumericClassCode(SO_GUILD_SC_DAI))
				{
					// 渠寞檜棻. 					
					
					///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
					// for debug
					if( uiGuildID == 0 )
					{
//						pMyWorld->clServer->WriteInfo( "..\\data\\DeletedGuild.txt", "Clan Master has left the guild. Guild disorganizes. But the Guild ID is 0" );
						writeInfoToFile( "DeletedGuild.txt", "Clan Master has left the guild. Guild disorganizes. But the Guild ID is 0" );
					}
					///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

					// 鼻欽擊 橈撻棻.
					if(pMyWorld->clIGuild.DeleteGuild(uiGuildID,stAccountInfo.pVillage) == TRUE)
					{						
						dwResponse		=	ON_RET_GLD_LEAVE_OK_DELETEGLD;										
					}
				}
				else
				{
					// 渠寞檜 嬴棲棻.

					// 鼻欽縑 и貲檜 緒螳釭鬲棻堅 憲溥遽棻.
					if(siClassInGuild == pMyWorld->clIGuildClass.GetJWJCode())				bCandidate	=	TRUE;
					else																	bCandidate	=	FALSE;

					if(pMyWorld->clIGuild.DelGuildMember(stAccountInfo.pVillage->uiGuildID, szID, bCandidate, FALSE ,FALSE) == TRUE)					
						dwResponse		=	ON_RET_GLD_LEAVE_OK;										
				}				
			}
		}
	}

	// 撩奢ц棻.
	SendFieldMsg(ON_RESPONSE_JOIN_LEAVE_GUILD, SO_SFM_ONLYME, LPVOID(dwResponse));	
}


//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	鼻欽縑 霤罹ж晦蒂 晦棻葬朝 餌辨濠菟曖 葬蝶お蒂 橢橫螞棻.
//	熱薑 橾濠	:	2002-02-25 螃�� 1:15:50 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
VOID	SOPlayer::RecvReqGuildWaitingJoinMembersList()
{
	cltGuild	*pclGuild;
	BOOL		bSuccess = FALSE;
	OnReqGuildWaitingJoinMembersListMsg		*pGuildWaitingJoinMembersListMsg;

	pGuildWaitingJoinMembersListMsg		=	(OnReqGuildWaitingJoinMembersListMsg*)cMsg;

	// 鼻欽縑 奩萄衛 模樓腎橫 氈橫撿 и棻.
	if(uiGuildID == 0)	goto	END;

	// 鼻欽檜 奩萄衛 襄營ж罹撿 и棻.
	if((pclGuild = pMyWorld->clIGuild.GetGuildByID(uiGuildID)) == NULL)	goto	END;		

	// 撩奢ц棻.
	bSuccess	=	TRUE;

END:
	if(bSuccess == TRUE)
	{
		// 望萄錳菟曖 葬蝶お蒂 爾頂 還 熱 氈棻.
		SendFieldMsg(ON_RESPONSE_GUILD_WAITING_JOIN_MEMBERS_LIST, SO_SFM_ONLYME, LPVOID(ON_RET_OK), LPVOID(pclGuild), 
						LPVOID(pGuildWaitingJoinMembersListMsg->dwPage));
	}
	else
	{
		// 望萄錳菟曖 葬蝶お蒂 爾頂 還 熱 橈棻
		SendFieldMsg(ON_RESPONSE_GUILD_WAITING_JOIN_MEMBERS_LIST, SO_SFM_ONLYME, LPVOID(ON_RET_NO));
	}
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	鼻欽縑 霤罹蒂 ъ塊 塽 剪睡и棻.
//	熱薑 橾濠	:	2002-02-25 螃�� 1:46:04 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
VOID	SOPlayer::RecvJoinGuildOkNo()
{
	CHAR						szCharID[1024];
	OnReqJoinGuildOkNoMsg		*pJoinGuildOkNoMsg;	
	DWORD						dwResponse = ON_RET_NO;

	pJoinGuildOkNoMsg		=	(OnReqJoinGuildOkNoMsg*)cMsg;	
	
	ZeroMemory(szCharID, sizeof(szCharID));
	strncpy(szCharID, pJoinGuildOkNoMsg->szID, ON_ID_LENGTH);

	if(pJoinGuildOkNoMsg->dwResponse == ON_RET_OK)			// '殮欽ъ塊'幗が擊 揚毓擊 唳辦
	{	
		cltGuild* pGuild = pMyWorld->clIGuild.GetGuildByID(uiGuildID);
		if(pGuild != NULL)
		{
			if(pGuild->m_GuildMemberManager.GetCandidateMember( szCharID ) != NULL)
			{		
				if(pMyWorld->m_WarTimeParser.IsWarCheck(pMyWorld->pRealTime->tm_wday, pMyWorld->pRealTime->tm_hour))
				if(pMyWorld->m_WarTimeParser.IsWarTime(pMyWorld->pRealTime->tm_hour))
					dwResponse	=	ON_RET_NO_GLD_ADMIT_MAXINVESTOR;
				else
				{
					// 蛔晝擊 褻薑п遽棻. (殮欽 ъ塊)
					if(pMyWorld->clIGuild.AddGuildMember(uiGuildID, szCharID) == TRUE)
						dwResponse	=	ON_RET_OK;
					else		
						dwResponse	=	ON_RET_NO_GLD_ADMIT_FULL;
				}
			}
		}		
	}
	else if(pJoinGuildOkNoMsg->dwResponse == ON_RET_NO)		// '殮欽鏃模'幗が擊 揚毓擊 唳辦
	{
		// 霤罹蒂 剪睡и棻.
		if(pMyWorld->clIGuild.DelGuildMember(uiGuildID, szCharID, TRUE, TRUE ,TRUE) == TRUE)	dwResponse	=	ON_RET_OK;		
	}

	// 澀 嫡懊棻堅 擬港擊 爾頂遽棻.
	SendFieldMsg(ON_RESPONSE_JOINGUILD_OKNO, SO_SFM_ONLYME, LPVOID(dwResponse));
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	鼻欽曖 渠и 鼻撮 薑爾蒂 橢橫螞棻.	
//	熱薑 橾濠	:	2002-04-22 螃瞪 9:35:33 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
VOID	SOPlayer::RecvGuildDetailInfo()
{
	OnReqGuildDetailInfoMsg	*pGuildDetailInfoMsg;
	CHAR					szGuildID[ON_GUILDNAME_LENGTH + 1];	// 望萄曖 檜葷
	cltGuild				*pGuild;

	pGuildDetailInfoMsg	=	(OnReqGuildDetailInfoMsg*)cMsg;	

	ZeroMemory(szGuildID, sizeof(szGuildID));
	strncpy(szGuildID, pGuildDetailInfoMsg->szGuildID, ON_GUILDNAME_LENGTH);	

	if((pGuild = pMyWorld->clIGuild.GetGuildByName(szGuildID)) != NULL)
	{	
		SendFieldMsg(ON_RESPONSE_GUILD_DETAIL_INFO, SO_SFM_ONLYME, LPVOID(ON_RET_OK), LPVOID(pGuild));						
	}
	else
	{
		SendFieldMsg(ON_RESPONSE_GUILD_DETAIL_INFO, SO_SFM_ONLYME, LPVOID(ON_RET_NO));
	}
}	

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	鼻欽錳 菟曖 葬蝶お蒂 橢橫螞棻.
//	熱薑 橾濠	:	2002-02-25 螃�� 2:18:10 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
VOID	SOPlayer::RecvReqGuildMembersList()
{
	OnReqGuildMembersListMsg	*pGuildMembersListMsg;
	cltGuild					*pclGuild;
	UI32						uiReqGuild;

	pGuildMembersListMsg	=	(OnReqGuildMembersListMsg*)cMsg;	

	// 虜擒縑 鼻欽 ID陛 0檜塭賊 釭曖 鼻欽錳 薑爾蒂 蹂掘ж朝匙檜棻.
	if(pGuildMembersListMsg->uiGuidlID == 0)
		uiReqGuild	=	uiGuildID;
	else
		uiReqGuild	=	pGuildMembersListMsg->uiGuidlID;

	if((pclGuild = pMyWorld->clIGuild.GetGuildByID(uiReqGuild)) != NULL)	
		SendFieldMsg(ON_RESPONSE_GUILD_MEMBERS_LIST, SO_SFM_ONLYME, LPVOID(ON_RET_OK), LPVOID(pclGuild), LPVOID(pGuildMembersListMsg->dwPage));	
	else	
		SendFieldMsg(ON_RESPONSE_GUILD_MEMBERS_LIST, SO_SFM_ONLYME, LPVOID(ON_RET_NO));	
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	鼻欽錳擊 п堅и棻.
//	熱薑 橾濠	:	2002-03-25 螃�� 2:05:32 - 寥鼻⑷
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
VOID	SOPlayer::RecvGuildDismissal()
{
	CHAR				szCharID[ON_ID_LENGTH + 1];
	OnReqDismissalMsg	*pDismissalMsg;
	cltGuild			*pclGuild;
	BOOL				bSuccess	=	FALSE;

	pDismissalMsg	=	(OnReqDismissalMsg*)cMsg;

	ZeroMemory(szCharID, sizeof(szCharID));
	strncpy(szCharID, pDismissalMsg->szID, ON_ID_LENGTH);

	//奢撩 衛除縑朝 п堅й熱 橈棻.
	if(pMyWorld->m_WarTimeParser.IsWarCheck(pMyWorld->pRealTime->tm_wday, pMyWorld->pRealTime->tm_hour)) goto END;
	if(pMyWorld->m_WarTimeParser.IsWarTime(pMyWorld->pRealTime->tm_hour)) goto END;
	// 望萄 偌羹蒂 橢橫螞棻.
	if( (pclGuild	=	pMyWorld->clIGuild.GetGuildByID(uiGuildID)) == NULL )		goto END;
	
	// п堅ж溥朝 餌塋檜 渠寞檜塭賊 褒ぬ 籀葬и棻.
	if( strcmp(pclGuild->GetDAIID(), szCharID) == 0 )								goto END;

	// 望萄錳 餉薯
	if( pMyWorld->clIGuild.DelGuildMember(uiGuildID, szCharID, FALSE, TRUE ,TRUE) == TRUE )		bSuccess	=	TRUE;


END:
	if( bSuccess )
	{
		// 鼻欽縑啪紫 憲溥遽棻.
		SendFieldMsg(ON_RESPONSE_GUILD_DISMISSAL, SO_SFM_ONLYME, LPVOID(ON_RET_OK));
	}
	else
	{
		SendFieldMsg(ON_RESPONSE_GUILD_DISMISSAL, SO_SFM_ONLYME, LPVOID(ON_RET_NO));
	}
}


//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	鼻欽錳擊 か薑 霜晝戲煎 歜貲и棻朝 詭衛雖蒂 籀葬и棻.
//	熱薑 橾濠	:	2003-02-11 螃�� 1:43 - 寥鼻⑷
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
VOID	SOPlayer::RecvReqGuildPromotion()
{
	CHAR						szCharID[ON_ID_LENGTH+1];
	OnRequestGuildPromotion		*pPromotionMsg;
	SI32						siSHNCode	=	pMyWorld->clIGuildClass.GetSHNCode();			// 餌��
	SI32						siHNSCode	=	pMyWorld->clIGuildClass.GetHNSCode();			// ч熱
	SI32						siDHSCode	=	pMyWorld->clIGuildClass.GetDHSCode();			// 渠ч熱
	SI32						siHNDCode	=	pMyWorld->clIGuildClass.GetHNDCode();			// ч翕渠濰
	BOOL						bSuccess	=	FALSE;

	pPromotionMsg = (OnRequestGuildPromotion*)cMsg;

	ZeroMemory( szCharID, sizeof(szCharID) );
	strncpy( szCharID, pPromotionMsg->szCharID, ON_ID_LENGTH );

	cltGuild *pGuild = pMyWorld->clIGuild.GetGuildByID( uiGuildID );

	strGuildCommonMemberInfo MemberInfo;

	// 議葛攪 檜葷擊 餌辨ж罹 賅萇 薑爾蒂 橢橫螞棻.
	if( pGuild != NULL )
	{
		if( pGuild->m_GuildMemberManager.GetGuildMemberByCharName(szCharID, &MemberInfo) == FALSE )		goto	END;
	}
	else
	{
		goto	END;
	}

	if( pPromotionMsg->siClassCode == siDHSCode )
	{
		// 渠ч熱煎 歜貲 蹂羶
		// 褻勒1: 歜貲嫡擊 餌塋曖 ⑷霜晝擎 餌�� 檜鼻檜橫撿 и棻.
		// 褻勒2: ⑷營 渠ч熱 濠葬陛 綠橫氈橫撿 и棻.

		// 餌�笑葩� 堪擎 霜晝檜 嬴棲賊 歜貲 褒ぬ
		if( pMyWorld->clIGuildClass.ClassComp(MemberInfo.siClassInGuild, siSHNCode) == SO_GUILD_CLASSCOMP_LOW )			goto	END;
		// 檜嘐 渠ч熱陛 氈戲賊 歜貲 褒ぬ
		if( pGuild->m_GuildMemberManager.GetDHSID() != NULL )															goto	END;

		bSuccess	=	pMyWorld->clIGuild.ChangeClass( uiGuildID, szCharID, siDHSCode );
	}
	else if( pPromotionMsg->siClassCode == siHNDCode )
	{
		// ч翕渠濰戲煎 歜貲 蹂羶
		// 褻勒1: 歜貲嫡擊 餌塋曖 ⑷霜晝擎 餌�素抩蹌� и棻.
		// 褻勒2: ч翕渠濰曖 熱朝 爾嶸 葆擊 5偃渡 1貲檜棻.		奢衝: (n + 4) / 5		n: 爾嶸葆擊熱

		if( MemberInfo.siClassInGuild != siSHNCode )	goto	END;				// 餌�素� 嬴棲賊 歜貲 褒ぬ
		// 歜貲й 熱 氈朝 餌�素� 熱蒂 剩橫陛賊 歜貲 褒ぬ
		if( pGuild->m_GuildMemberManager.GetTotalHNDNum() >= (UI32)((pGuild->GetOwnerVillageNum() + 4) / 5) )	goto	END;

		bSuccess	=	pMyWorld->clIGuild.ChangeClass( uiGuildID, szCharID, siHNDCode );
	}

END:
	if( bSuccess )
		SendFieldMsg( ON_RESPONSE_GUILD_PROMOTION, SO_SFM_ONLYME, LPVOID(ON_RET_OK) );
	else		
		SendFieldMsg( ON_RESPONSE_GUILD_PROMOTION, SO_SFM_ONLYME, LPVOID(ON_RET_NO) );
}


//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	鼻欽錳擊 か薑 霜晝縑憮 п歜и棻朝 詭衛雖蒂 籀葬и棻.
//	熱薑 橾濠	:	2003-02-11 螃�� 1:47 - 寥鼻⑷
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
VOID	SOPlayer::RecvReqGuildDemotion()
{
	CHAR						szCharID[ON_ID_LENGTH+1];
	OnRequestGuildDemotion		*pDemotionMsg;
	SI32						siSHNCode	=	pMyWorld->clIGuildClass.GetSHNCode();			// 餌��
	SI32						siHNSCode	=	pMyWorld->clIGuildClass.GetHNSCode();			// ч熱
	SI32						siDHSCode	=	pMyWorld->clIGuildClass.GetDHSCode();			// 渠ч熱
	SI32						siHNDCode	=	pMyWorld->clIGuildClass.GetHNDCode();			// ч翕渠濰
	SI32						siDAICode	=	pMyWorld->clIGuildClass.GetDAICode();			// ч翕渠濰
	BOOL						bSuccess	=	FALSE;
	SI32						siNewClass;

	pDemotionMsg = (OnRequestGuildDemotion*)cMsg;
	ZeroMemory( szCharID, sizeof(szCharID) );
	strncpy( szCharID, pDemotionMsg->szCharID, ON_ID_LENGTH );

	cltGuild *pGuild = pMyWorld->clIGuild.GetGuildByID( uiGuildID );

	strGuildCommonMemberInfo MemberInfo;

	// 議葛攪 檜葷擊 餌辨ж罹 賅萇 薑爾蒂 橢橫螞棻.
	if( pGuild != NULL )
	{
		if( pGuild->m_GuildMemberManager.GetGuildMemberByCharName(szCharID, &MemberInfo) == FALSE )		goto	END;
	}
	else
	{
		goto	END;
	}

	if( MemberInfo.siClassInGuild == siDAICode )		goto END;							// 渠寞檜賊 п歜й 熱 橈棻.

	if( MemberInfo.siClassInGuild == siDHSCode )
	{
		// 渠ч熱蒂 п歜й 唳辦 --> 餌�素� 腆 熱紫 氈堅 ч熱陛 腆 熱紫 氈棻.
		siNewClass	=	pMyWorld->GetHighestGuildClass( szCharID );							// 陝 葆擊擊 匐餌ж罹 檜 議葛攪曖 譆渠 霜晝擊 橢橫螞棻.
		bSuccess	=	pMyWorld->clIGuild.ChangeClass( uiGuildID, szCharID, siNewClass );	// 匐餌ж罹 橢橫螞 譆渠霜晝戲煎 夥脯遽棻.
	}
	else if( MemberInfo.siClassInGuild == siHNDCode )
	{
		// ч翕渠濰擊 п歜й 唳辦 --> 餌�素� 脹棻.
		bSuccess	=	pMyWorld->clIGuild.ChangeClass( uiGuildID, szCharID, siSHNCode );
	}

END:

	if( bSuccess )
		SendFieldMsg( ON_RESPONSE_GUILD_DEMOTION, SO_SFM_ONLYME, LPVOID(ON_RET_OK) );
	else
		SendFieldMsg( ON_RESPONSE_GUILD_DEMOTION, SO_SFM_ONLYME, LPVOID(ON_RET_NO) );
}


//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	鼻欽曖 葬蝶お蒂 蹂掘и棻.
//	熱薑 橾濠	:	2002-02-27 螃�� 2:45:30 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
VOID	SOPlayer::RecvGuildList()
{	
	// 望萄曖 薑爾蒂 爾頂遽棻.
	SendFieldMsg(ON_RESPONSE_GUILD_LIST, SO_SFM_ONLYME);
}


//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	鼻欽擊 撲薑и棻.
//	熱薑 橾濠	:	2002-02-27 螃�� 4:27:53 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
VOID	SOPlayer::SetGuild(UI32 uiguildid)
{
	// 鼻欽擊 撲薑и棻.
	uiGuildID		=	uiguildid;	

	if(uiGuildID != 0)
	{
		siFlag	=	pMyWorld->clIGuild.GetFlag(uiGuildID);
	}
	else
	{
		SetClassInGuild(0);
		siFlag	=	0;
	}
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	霜晝擊 撲薑и棻.
//	熱薑 橾濠	:	2002-02-27 螃�� 4:27:53 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
VOID	SOPlayer::SetClassInGuild(SI32 siclassinguild)
{
	// 霜晝擊 撲薑и棻.
	siClassInGuild	=	siclassinguild;	
}


//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	霜晝擊 橢橫螞棻.
//	熱薑 橾濠	:	2003-02-11 螃�� 2:173 - 寥鼻⑷
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
SI32	SOPlayer::GetClassInGuild()
{
	return siClassInGuild;
}
	
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	鼻欽曖 梓嫦擊 滲唳и棻.
//	熱薑 橾濠	:	2002-04-04 螃�� 1:35:28 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
VOID	SOPlayer::RecvGuildChangeFlag()
{

	// DBм艦夠

	BOOL						bSuccess = FALSE;
	OnReqGuildChangeFlag		*pGuildChangeFlag;

	// 渠寞檣雖 羹觼и棻.
	if(uiGuildID == 0)	goto	END;
	if(siClassInGuild != pMyWorld->clIGuildClass.GetNumericClassCode(SO_GUILD_SC_DAI))	goto	END;

	if(EnoughMoney(ON_GUILD_COST_CHANGE_FLAG) == FALSE)				goto	END;

	pGuildChangeFlag	=	(OnReqGuildChangeFlag*)cMsg;

	if(pMyWorld->clIGuild.SetFlag(uiGuildID, pGuildChangeFlag->siFlag) == FALSE)
		goto	END;

	// 撩奢瞳戲煎 梓嫦擊 撲薑ж啪 腎賊
	// SetFlag л熱 頂縑憮 濠翕瞳戲煎 ⑷營 蕾樓腎橫 氈朝 鼻欽錳縑啪 詭衛雖蒂 爾頂遽棻.

	ChangeMoney(-ON_GUILD_COST_CHANGE_FLAG,FALSE);

	// DB縑 盪濰и棻.

	DBIKGuildFlag	GuildFlag;

	GuildFlag.GuildID	=	uiGuildID;
	GuildFlag.Flag		=	pGuildChangeFlag->siFlag;

	GuildFlag.siAccount = stAccountInfo.siDBAccount;
	GuildFlag.uiSlot = stAccountInfo.uiSlot;
	GuildFlag.moMoney = ON_GUILD_COST_CHANGE_FLAG;

	pMyWorld->clDatabase->writeDBQuery(SO_DB_INFOKIND_GUILD_FLAG, &GuildFlag, sizeof(GuildFlag) );	
	

	bSuccess	=	TRUE;
END:
	if(bSuccess == FALSE)
	{
		SendFieldMsg(ON_RESPONSE_CHANGE_GUILD_FLAG, SO_SFM_ONLYME, LPVOID(ON_RET_NO), 0);
	}	
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	餌辨脹 梓嫦曖 葬蝶お蒂 橢橫除棻.
//	熱薑 橾濠	:	2002-04-04 螃�� 2:18:02 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
VOID	SOPlayer::RecvReqUsedFlagList()
{	
	if(uiGuildID != 0)
	{
		// 鼻欽檜 氈擊 唳辦縑虜 剩啖 遽棻.		
		SendFieldMsg(ON_RESPONSE_USED_FLAG_LIST, SO_SFM_ONLYME, LPVOID(uiGuildID));
	}
}
/*
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	葆擊擊 熱葬и棻朝 詭衛雖蒂 嫡懊棻.
//	熱薑 橾濠	:	2002-04-08 螃�� 2:31:00 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
VOID	SOPlayer::RecvDefenceVillage()
{
	OnReqRepairVillageMsg	*pRepairVillageMsg;
	BOOL					bSuccess = FALSE;

	pRepairVillageMsg	=	(OnReqRepairVillageMsg*)cMsg;

	if(stAccountInfo.pVillage == NULL)	goto	END;

	// 撩奢ц擊 唳辦縑朝 IncreaseDefence() 頂縑憮 婦溼脹 Ы溯檜橫菟縑啪 詭衛雖蒂 爾頂遽棻.
	stAccountInfo.pVillage->RepairDefence(this, pRepairVillageMsg->vdDefence);

	// 撩奢ц棻.
	bSuccess	=	TRUE;
END:
	if(bSuccess == FALSE)
	{
		// 褒ぬ
		SendFieldMsg(ON_RESPONSE_REPAIR_VILLAGE, SO_SFM_ONLYME, LPVOID(ON_RET_NO));
	}	
}
*/

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	葆擊縑憮 か薑 勒僭縑 菟橫除棻朝 詭衛雖蒂 嫡懊棻.
//	熱薑 橾濠	:	2002-10-02 螃�� 7:16:46 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
VOID	SOPlayer::RecvEnterStructure()
{
	OnReqEnterStructureMsg		*pEnterStructureMsg;
	BOOL						bSuccess	=	FALSE;
	
	pEnterStructureMsg		=	( OnReqEnterStructureMsg* )cMsg;
	
	// ⑷營 葆擊 寰縑 氈朝雖 匐餌и棻.
	if( IsInVillage() == FALSE )																							goto	END;
	
	// ⑷營 菟橫陛 氈朝 勒僭檜 氈朝雖 匐餌и棻.
	if( IsEnteredStructure() == TRUE )																						goto	END;
	
	// か薑 勒僭縑 菟橫除棻.
	if( stAccountInfo.pVillage->EnterStructure( this, pEnterStructureMsg->siStructureKind,
												pEnterStructureMsg->lParam1, pEnterStructureMsg->lParam2 ) == FALSE )		goto	END;
	
	bSuccess	=	TRUE;
	
END:
	
	if( bSuccess == TRUE )	
	{
		SendFieldMsg( ON_RESPONSE_ENTER_STRUCTURE, SO_SFM_ONLYME, LPVOID( ON_RET_OK ) );
	}
	else
	{
		SendFieldMsg( ON_RESPONSE_ENTER_STRUCTURE, SO_SFM_ONLYME, LPVOID( ON_RET_NO ) );
	}
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	葆擊縑憮 か薑 勒僭縑憮 釭除棻朝 詭衛雖蒂 嫡懊棻.
//	熱薑 橾濠	:	2002-10-02 螃�� 7:31:20 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
VOID	SOPlayer::RecvLeaveStructure()
{
	BOOL						bSuccess	=	FALSE;
	
	// ⑷營 葆擊 寰縑 氈朝雖 匐餌и棻.
	if( IsInVillage() == FALSE )																			goto	END;
	
	// ⑷營 菟橫陛 氈朝 勒僭檜 氈朝雖 匐餌и棻.
	if( IsEnteredStructure() == TRUE )																		goto	END;
	
	// か薑 勒僭縑憮 釭螞棻.
	if( stAccountInfo.pVillage->LeaveStructure( this ) == FALSE )											goto	END;
	
	bSuccess	=	TRUE;	
	
END:
	
	if( bSuccess == TRUE )	
	{
		SendFieldMsg( ON_RESPONSE_LEAVE_STRUCTURE, SO_SFM_ONLYME, LPVOID( ON_RET_OK ) );
	}
	else
	{
		SendFieldMsg( ON_RESPONSE_LEAVE_STRUCTURE, SO_SFM_ONLYME, LPVOID( ON_RET_NO ) );
	}
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	葆擊 骯機 欽雖曖 奢濰縑 菟橫除棻.
//	熱薑 橾濠	:	2002-10-11 螃�� 4:57:09 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
VOID	SOPlayer::RecvEnterFactory()
{
	OnReqFarmWorkInfoMsg		*pFarmWorkInfoMsg;
	BOOL						bSuccess = FALSE;
	
	pFarmWorkInfoMsg		=	(OnReqFarmWorkInfoMsg*) cMsg;
	
	// ⑷營 葆擊 寰縑 氈朝雖 匐餌и棻.
	if( IsInVillage() == FALSE )																							goto	END;
		
	bSuccess	=	stAccountInfo.pVillage->EnterIndustrialComplexFactory( this, SI32( pFarmWorkInfoMsg->uiSlotIndex ) );
	//bSuccess	=	stAccountInfo.pVillage->m_pIndustrialComplexs->SendFactoryInfo( this, pFarmWorkInfoMsg->uiSlotIndex );

		/*
	if(stAccountInfo.pStructure)
	{
		pFarmWorkInfoMsg		=	(OnReqFarmWorkInfoMsg*) cMsg;
		pFarmSlot	= stAccountInfo.pStructure->GetFarmSlot(pFarmWorkInfoMsg->uiSlotIndex);
		if(pFarmSlot)
		{
			if( pFarmSlot->bOnSchedule() )
			{
				SendFieldMsg(ON_RESPONSE_FARM_WORKINFO, SO_SFM_ONLYME, LPVOID(ON_RET_OK), LPVOID(pFarmSlot));
				return;
			}
			else if( pFarmSlot->bStore )
			{
				if( stAccountInfo.pVillage->IsFarmOwner( this ) )
				{
					SendFieldMsg(ON_RESPONSE_STORAGE_INFO, SO_SFM_ONLYME, LPVOID(ON_RET_OK),LPVOID(pFarmSlot));
					return;
				}
			}
		}
	}
	*/



END:	

	if( bSuccess == FALSE )
	{
		SendFieldMsg(ON_RESPONSE_FARM_WORKINFO, SO_SFM_ONLYME, LPVOID(ON_RET_NO));
	}

}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	葆擊 骯機 欽雖曖 奢濰縑憮 釭除棻.
//	熱薑 橾濠	:	2002-10-11 螃�� 4:56:58 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
VOID	SOPlayer::RecvLeaveFactory()
{
	BOOL	bSuccess = FALSE;

	// ⑷營 葆擊 寰縑 氈朝雖 匐餌и棻.
	if( IsInVillage() == FALSE )																							goto	END;
	
	bSuccess	=	stAccountInfo.pVillage->LeaveIndustrialComplexFactory( this );
	
END:

	if( bSuccess == FALSE )
	{

		SendFieldMsg( ON_RESPONSE_LEAVE_FACTORY, SO_SFM_ONLYME, LPVOID( ON_RET_NO ) );

	}

}

VOID    SOPlayer::RecvBankSave()
{
	OnReqBankSaveMsg *pMsg = (OnReqBankSaveMsg *)cMsg;

	OnResBankSaveMsg ret;

	
	if (SaveMoneyAtBank(pMsg->moMoney))
	{
		ret.dwMsgType = ON_RESPONSE_BANK_SAVE;
		ret.dwResponse = ON_RET_BANK_OK; 
		ret.moMoney = siMoney;
		ret.moSaving = m_MoneySaving;
		SendFieldMsg( ON_RESPONSE_BANK_SAVE, SO_SFM_ONLYME, LPVOID( &ret ) );
	}
	else
	{
		ret.dwMsgType = ON_RESPONSE_BANK_SAVE;
		ret.dwResponse = ON_RET_BANK_NO; 
		SendFieldMsg( ON_RESPONSE_BANK_SAVE, SO_SFM_ONLYME, LPVOID( &ret ) );
		

	}

}

VOID    SOPlayer::RecvBankWithdraw()
{
	OnReqBankWithdrawMsg *pMsg = (OnReqBankWithdrawMsg *)cMsg;
	OnResBankWithdrawMsg ret;

	if (WithdrawMoneyAtBank(pMsg->moMoney))
	{
		ret.dwMsgType = ON_RESPONSE_BANK_WITHDRAW;
		ret.dwResponse = ON_RET_BANK_OK; 
		ret.moMoney = siMoney;
		ret.moSaving = m_MoneySaving;
		
		SendFieldMsg( ON_RESPONSE_BANK_WITHDRAW, SO_SFM_ONLYME, LPVOID( &ret ) );
	}
	else
	{
		ret.dwMsgType = ON_RESPONSE_BANK_WITHDRAW;
		ret.dwResponse = ON_RET_BANK_NO; 

		SendFieldMsg( ON_RESPONSE_BANK_WITHDRAW, SO_SFM_ONLYME, LPVOID( &ret ) );
		
	}

}

VOID	SOPlayer::RecvItemList()
{
	
	char	buf[ 1024 ];

	DWORD	cmd = ON_RESPONSE_ITEMLIST;
	UI16	count = m_ItemBankList.GetItemCount();
	UI32	size;

	memcpy( buf, &cmd, 4 );
	memcpy( &buf[ 4 ], &count, 2 );
	memcpy( &buf[ 6 ], m_ItemBankList.GetList(), count * 4 );

	size = 4 + 2 + count * 4;

	Write( buf, size );
	
}

VOID	SOPlayer::RecvTakeOutItem()
{

	OnReqTakeOutItemMsg *pMsg = (OnReqTakeOutItemMsg *)cMsg;
	
	OnResTakeOutItemMsg resMsg;
	
	SI16 siPosInventory;
	UI32 uiResult = 0;

	if( m_ItemBankList.FindItem( pMsg->uiItemID, pMsg->uiQuantity ) == FALSE ) {
		uiResult = 1;
		goto END;
	}

	if( IFollower.CanAddItem( pMsg->uiFollowerID, pMsg->uiItemID, pMsg->uiQuantity ) == FALSE ) {
		uiResult = 1;
		goto END;
	}

	if( uiResult == 0 ) {

		if( IFollower.AddItem( pMsg->uiFollowerID, pMsg->uiItemID, pMsg->uiQuantity, &siPosInventory ) == FALSE ) {
			uiResult = 1;
			goto END;
		}
	}

END:
	if( uiResult == 0 ) {
		// 撩奢檜賊,

		m_ItemBankList.TakeOutItem( pMsg->uiItemID, pMsg->uiQuantity );

		DBIKPutItemFromItemBank iteminfo;

		iteminfo.DBAccount = stAccountInfo.siDBAccount;
		iteminfo.uiSlot = stAccountInfo.uiSlot;
		iteminfo.uiFollowerID = pMsg->uiFollowerID;
		iteminfo.uiPosition = (UI08)siPosInventory;
		iteminfo.siItemID = pMsg->uiItemID;
		iteminfo.siQuantity = pMsg->uiQuantity;
		
		//pMyWorld->clDatabase->GetDBInfo( SO_DB_INFOKIND_PUTITEM_FROMITEMBANK, &iteminfo, &uiResult );
		pMyWorld->clDatabase->writeDBQuery( SO_DB_INFOKIND_PUTITEM_FROMITEMBANK, &iteminfo, sizeof( DBIKPutItemFromItemBank ) );
		
	} 	

	// 擬港 詭衛雖 瞪歎 
	resMsg.dwMsgType = ON_RESPONSE_TAKEOUTITEM;
	resMsg.dwResponse = uiResult;

	Write( &resMsg, 8 );
}

VOID	SOPlayer::RecvSaveItemIntoItemBank()
{
	OnReqSaveItemMsg *pMsg = (OnReqSaveItemMsg *)cMsg;
	
	OnResSaveItemMsg resMsg;

	UI32 uiResult = 0;

	if( IFollower.HasItem( pMsg->uiFollowerID, pMsg->uiItemID, pMsg->uiQuantity ) == FALSE ) {		
		uiResult = 1;
		goto END;
	}
	
	
	if( GetMoney() < 100000000 ) {		
		uiResult = 1;
		goto END;
	}


END:
	if( uiResult == 0 ) {

		IFollower.DelItem( pMsg->uiFollowerID, pMsg->uiItemID, pMsg->uiQuantity );

		DecreaseMoney( 100000000, FALSE );

		DBIKSaveItemIntoItemBank iteminfo;

		iteminfo.DBAccount = stAccountInfo.siDBAccount;
		iteminfo.uiSlot = stAccountInfo.uiSlot;
		iteminfo.uiFollowerID = pMsg->uiFollowerID;
		iteminfo.siItemID = pMsg->uiItemID;
		iteminfo.siQuantity = pMsg->uiQuantity;

		pMyWorld->clDatabase->writeDBQuery( SO_DB_INFOKIND_SAVEITEM_ITEMBANK, &iteminfo, sizeof( DBIKSaveItemIntoItemBank ) );

		writeInfoToFile( "itembank.txt", "Account:%d, Slot:%d, FollowerID:%d, ItemID:%d, Quantity:%d",
						 iteminfo.DBAccount, iteminfo.uiSlot, iteminfo.uiFollowerID, iteminfo.siItemID, iteminfo.siQuantity );
	}

	// 擬港 詭衛雖 瞪歎 
	resMsg.dwMsgType = ON_RESPONSE_SAVEITEM;
	resMsg.dwResponse = uiResult;

	Write( &resMsg, 8 );

}

VOID    SOPlayer::RecvRequestPersonalInfo()
{	

	OnReqPersonalInfo *pMsg = (OnReqPersonalInfo *)cMsg;	
	OnResPersonalInfo resMsg;

	resMsg.dwMsgType = ON_RESPONSE_PERSONAL_INFO; 

	cltGuild *pclGuild;

	SOPlayer *pPlayer = pMyWorld->GetPlayer(pMsg->szID);

	ZeroMemory(resMsg.szID,sizeof(resMsg.szID));
	memcpy(resMsg.szID,pMsg->szID,12);

	if (pPlayer != NULL)
	{
		resMsg.dwResponse = ON_RET_OK;

		resMsg.siClassInGuildID = pPlayer->siClassInGuild;

		// 鼻欽縑 模樓脹 餌塋檜 嬴棲賊
		if (pPlayer->siClassInGuild == 0) {

			ZeroMemory(resMsg.szGuildName,sizeof(resMsg.szGuildName));	
		}
		else
		{
			// 鼻欽檜 奩萄衛 襄營ж罹撿 и棻.
			if((pclGuild = pMyWorld->clIGuild.GetGuildByID(pPlayer->uiGuildID)) != NULL)
			{
				strcpy(resMsg.szGuildName,pclGuild->GetGuildName());		
			}
			else
			{
				ZeroMemory(resMsg.szGuildName,sizeof(resMsg.szGuildName));
			}
		}
	}
	else
	{
		resMsg.dwResponse = ON_RET_NO;
	}

	Write( &resMsg, sizeof(resMsg) );

}


BOOL	SOPlayer::OnUseDonkeyItem( UI08 uiFollowerID, UI16 uiItemID )
{
	BOOL			bFound	=	FALSE;
	SI32			uiDonkeySlot;
	CharHead		*pCharHead;
	OnFollowerInfo	stFollowerInfo;

	// 綴 辨煽 蝸煜檜 氈朝雖 匐儀и棻.
	for( uiDonkeySlot = 0; uiDonkeySlot < ON_MAX_FOLLOWER_NUM; uiDonkeySlot++ )
	{
		if( IFollower.IsValidFollower(uiDonkeySlot) == FALSE )
		{
			bFound	=	TRUE;
			break;
		}
	}

	// 綴 蝸煜檜 氈棻賊 渡釭敝 議葛攪曖 薑爾蒂 撮たж堅 DB縑 憲溥輿堅 贗塭檜樹お煎 陳萼棻.
	if( bFound )
	{
		// 議葛攪 葬蝶お縑憮 渡釭敝蒂 瓊朝棻.
		for( SI32 i = 0; i < clServer->clWorld->clCharParser.GetCharTotalNum(); i++)
		{
			pCharHead = clServer->clWorld->clCharParser.GetCharHeader(i);
			
			if( pCharHead->uiID == clServer->clWorld->clCharParser.GetID("U85") )	break;
		}

		// Player曖 辨煽蘊縑 渡釭敝蒂 蹺陛и棻.(DB縑 盪濰梱雖 и棻.)
		ZeroMemory( &stFollowerInfo, sizeof(OnFollowerInfo) );
		stFollowerInfo.uiSlot			   = uiDonkeySlot;
		stFollowerInfo.uiKind              = pCharHead->uiID;
		stFollowerInfo.uiItemNum           = 0;
		stFollowerInfo.siLevel             = 1;
		stFollowerInfo.siExp               = 0;
		stFollowerInfo.siStr               = pCharHead->Parameta.GetStr();
		stFollowerInfo.siDex               = pCharHead->Parameta.GetDex();
		stFollowerInfo.siVit               = pCharHead->Parameta.GetVit();
		stFollowerInfo.siInt               = pCharHead->Parameta.GetInt();
		stFollowerInfo.siLife              = pCharHead->Parameta.GetLife();
		stFollowerInfo.siMana              = pCharHead->siManaMax;
		stFollowerInfo.siBonus             = 0;
		stFollowerInfo.siLiftConstant      = pCharHead->siCon;
		stFollowerInfo.siEquipMinDamage    = pCharHead->Parameta.EquipMinDamage;
		stFollowerInfo.siEquipMaxDamage    = pCharHead->Parameta.EquipMaxDamage;
		stFollowerInfo.siAC                = pCharHead->Parameta.GetAC();
		stFollowerInfo.siDamageRegist      = pCharHead->Parameta.DamageResist;
		stFollowerInfo.siMagicRegist       = pCharHead->Parameta.MagicResist;
		// OnlineText縑憮 uiCharName戲煎 議葛攪檜葷(渡釭敝) 橢晦
		strncpy( stFollowerInfo.Name, clServer->clWorld->clOnlineText.Get( pCharHead->uiCharName ), ON_ID_LENGTH );
		CHAR	szCharName[256];
		ZeroMemory( szCharName, 256 );
		CopyMemory( szCharName, stFollowerInfo.Name, ON_ID_LENGTH );

		IFollower.InitFollower( uiDonkeySlot, &stFollowerInfo, pCharHead->siNation, &clServer->clWorld->clItem );

		DBIKConsumeDonkeyItem		DBInfo;

		ZeroMemory( &DBInfo, sizeof(DBIKConsumeDonkeyItem) );

		DBInfo.DBAccount		=	stAccountInfo.siDBAccount;
		DBInfo.uiSlot			=	stAccountInfo.uiSlot;
		DBInfo.uiFollowerID		=	uiFollowerID;					// 嬴檜蠱擊 餌辨и 辨煽
		DBInfo.uiItemID			=	uiItemID;						// 嬴檜蠱 ID
		DBInfo.uiDonkeySlot		=	uiDonkeySlot;					// 渡釭敝陛 菟橫陞 蝸煜	
		DBInfo.siKind			=	stFollowerInfo.uiKind;
		clServer->clWorld->clDatabase->writeDBQuery( SO_DB_INFOKIND_CONSUMEDONKEYITEM, LPVOID(&DBInfo), sizeof(DBInfo) );

		// Client縑 渡釭敝蒂 蹺陛ж塭堅 爾魚棻.
		SendFieldMsg( ON_ADD_FOLLOWER, SO_SFM_ONLYME, LPVOID(&stFollowerInfo) );

		return TRUE;
	}

	return FALSE;
}


