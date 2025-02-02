#include "SOMain.h"
#include "SOWorld.h"
#include "SOBaseChar.h"
#include "OnlineProtocol.h"
#include "OnlineCommonStruct.h"
#include "OnlineGameMsg.h"


//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	Trade蒂 ж濠堅 蹂羶и棻. 
//	熱薑 橾濠	:	2002-07-04 螃瞪 10:28:41 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
VOID	SOPlayer::RecvAskTrade()
{
	SOPlayer				*pTargetPlayer;
	OnReqAskTradeMsg		*pAskTradeMsg;
	BOOL					bSuccess  = FALSE;

	pAskTradeMsg	=	(OnReqAskTradeMsg*) cMsg;	

	// ⑷營 頂陛 Trade陛 陛棟и雖 憲嬴螞棻.
	if(CanTrade() == FALSE)													goto	END;

	// Trade蒂 ж堅濠ж朝 Ы溯檜橫陛 嶸�褲捘� 匐餌и棻.
	if((pTargetPlayer = pMyWorld->GetPlayer(pAskTradeMsg->UserID)) == NULL)	goto	END;		

	// Target Ы溯檜橫陛 Trade陛 陛棟и雖 憲嬴螞棻.
	if(pTargetPlayer->CanTrade() == FALSE)									goto	END;
	
	// 顫 嶸盪縑啪 だじ 褐羶醞檜剪釭 
	// 棻艇 嶸盪縑 だじ 褐羶擊 嫡懊棻賊 檜蒂 鏃模и棻.
	CloseRequestingJoinParty();

	bSuccess	=	TRUE;	

END:
	if(bSuccess == TRUE)
	{
		
		clTrade.SetState(TRADE_STATE_REQUESTING, LPARAM(pTargetPlayer->uiAccount));
		pTargetPlayer->clTrade.SetState(TRADE_STATE_REQUESTED, LPARAM(uiAccount));

		// 撩奢瞳戲煎 Trade蒂 ж堅濠ж朝 Ы溯檜橫縑啪 Trade蒂 錳и棻朝 詭衛雖蒂 爾麻棻.
		SendFieldMsg(ON_RESPONSE_TRADE, SO_SFM_ONLYME, LPVOID(ON_RET_OK));

		// Trade蒂 錳ж朝 Ы溯檜橫縑啪紫 Trade蒂 錳и棻朝 詭衛雖蒂 爾頂遽棻.
		pTargetPlayer->SendFieldMsg(ON_RECEIVE_ASK_TRADE, SO_SFM_ONLYME, LPVOID(uiAccount), LPVOID(szID));
	}
	else
	{
		// Trade蒂 ж堅濠ж朝 Ы溯檜橫諦 Trade蒂 ж朝匙檜 碳陛棟ж棻.
		SendFieldMsg(ON_RESPONSE_TRADE, SO_SFM_ONLYME, LPVOID(ON_RET_NO));
	}
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	Trade蒂 ж濠堅 蹂羶и匙縑 渠и 鼻渠寞曖 擬港擊 嫡懊棻.
//	熱薑 橾濠	:	2002-07-04 螃�� 1:04:06 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
VOID	SOPlayer::RecvAcceptTrade()
{
	BOOL					bSuccess = FALSE;		
	SOPlayer				*pTargetPlayer;
	OnReqAcceptTradeMsg		*pAcceptTradeMsg;	
	
	pAcceptTradeMsg		=	(OnReqAcceptTradeMsg*)cMsg;

	// ⑷營 棻艇 Ы溯檜橫陛 釭縑啪 掖羲擊 蹂羶ц朝雖 憲嬴螞棻.
	if(IsWaitingAcceptTrade() == FALSE)													goto	END;
	
	// 釭諦 掖羲ж溥朝 Ы溯檜橫曖 偌羹蒂 橢橫螞棻.
	if((pTargetPlayer =	pMyWorld->GetPlayer(clTrade.GetTradePlayerAccount())) == NULL)	goto	END;
	
	// 釭諦 掖羲ж溥朝 Ы溯檜橫 羲衛 掖羲擊 蹂羶ж堅 氈朝雖 憲嬴螞棻.
	if(pTargetPlayer->IsWaitingAcceptTrade(uiAccount) == FALSE)							goto	END;

	if(pAcceptTradeMsg->bAccept == TRUE)
	{
		// 掖羲擊 й 熱 氈紫煙 撲薑и棻.		
		AcceptedTrade( pTargetPlayer->uiGuildID, pTargetPlayer->siClassInGuild, pTargetPlayer->IFollower.GetFollowerParameterLevel( 0 ) );
		pTargetPlayer->AcceptedTrade( uiGuildID, siClassInGuild, IFollower.GetFollowerParameterLevel( 0 ) );
	}
	else
	{
		// 掖羲擊 ж雖 彊朝棻堅 撲薑и棻.	
		NotAcceptedTrade();
		pTargetPlayer->NotAcceptedTrade();
	}			

	bSuccess	=	TRUE;
	
END:
	if(bSuccess == FALSE)
	{
		SendFieldMsg(ON_RESPONSE_ACCEPT_TRADE, SO_SFM_ONLYME, LPVOID(ON_RET_NO));
	}
}


//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	⑷營 霞ч醞檣 掖羲擊 鏃模и棻.
//	熱薑 橾濠	:	2002-07-04 螃�� 1:23:34 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
VOID	SOPlayer::RecvCancelTrade()
{	
	BOOL				bSuccess  = FALSE;	
	SOPlayer			*pTargetPlayer;	

	
	// ⑷營 頂陛 掖羲擊 ж堅 氈朝雖 憲嬴螞棻.
	if(IsAcceptedTrading() == FALSE)														goto	END;

	// 釭諦 掖羲ж溥朝 Ы溯檜橫曖 偌羹蒂 橢橫螞棻.
	if((pTargetPlayer =	pMyWorld->GetPlayer(clTrade.GetTradePlayerAccount())) == NULL)		goto	END;

	// 渠鼻 Ы溯檜橫 羲衛 釭諦 掖羲醞檣雖 憲嬴螞棻.
	if(pTargetPlayer->IsAcceptedTrading(uiAccount) == FALSE)							goto	END;

	// 舒 Ы溯檜橫曖 掖羲檜 鏃模腎歷棻.
	CancelTrade();	
	pTargetPlayer->CancelTrade();


	bSuccess	=	TRUE;

END:
	if(bSuccess == FALSE)
	{
		// 掖羲擊 鏃模ж朝等 僥薯陛 儅啣棻賊 掖羲擊 й 熱 橈棻堅 憲溥遽棻.
		SendFieldMsg(ON_RESPONSE_CANCEL_TRADE, SO_SFM_ONLYME, LPVOID(ON_RET_NO));
	}

	return;
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	Trade衛 嬴檜蠱 摹鷗擊 賅舒 部麻棻.
//	熱薑 橾濠	:	2002-07-06 螃�� 6:34:08 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
VOID	SOPlayer::RecvFinishChooseTradeItem()
{
	BOOL							bSuccess = FALSE;
	SOPlayer						*pTargetPlayer;	
	OnReqFinishChooseTradeItemMsg	*pFinishChooseTradeItemMsg;
	
	pFinishChooseTradeItemMsg	=	(OnReqFinishChooseTradeItemMsg*)cMsg;

	// ⑷營 頂陛 掖羲擊 ж堅 氈朝雖 憲嬴螞棻.
	if(IsAcceptedTrading() == FALSE && IsChoseTradeItem() == FALSE)													goto	END;

	// 釭諦 掖羲ж溥朝 Ы溯檜橫曖 偌羹蒂 橢橫螞棻.
	if((pTargetPlayer =	pMyWorld->GetPlayer(clTrade.GetTradePlayerAccount())) == NULL)								goto	END;

	// 渠鼻 Ы溯檜橫 羲衛 釭諦 掖羲醞檣雖 憲嬴螞棻.
	if(pTargetPlayer->IsAcceptedTrading(uiAccount) == FALSE && pTargetPlayer->IsChoseTradeItem(uiAccount) == FALSE)	goto	END;

	if(pFinishChooseTradeItemMsg->bFinishChooseItem == TRUE)
	{
		// 掖羲й 嬴檜蠱擊 摹鷗ц棻.
		if(ChooseTradeItem() == FALSE)																					goto	END;
	}
	else
	{
		// 堅羲й 嬴檜蠱擊 摹鷗и 匙擊 鏃模и棻.
		if(CancelChooseTradeItem() == FALSE)																			goto	END;
	}
	
	bSuccess	=	TRUE;
END:
	if(bSuccess == TRUE)
	{
		// 舒 贗塭檜樹お縑啪 爾頂遽棻.
		SendFieldMsg(ON_RESPONSE_FINISH_CHOOSE_TRADEITEM, SO_SFM_ONLYME, LPVOID(ON_RET_OK), LPVOID(TRUE), LPVOID(pFinishChooseTradeItemMsg->bFinishChooseItem));
		pTargetPlayer->SendFieldMsg(ON_RESPONSE_FINISH_CHOOSE_TRADEITEM, SO_SFM_ONLYME, LPVOID(ON_RET_OK), LPVOID(FALSE), LPVOID(pFinishChooseTradeItemMsg->bFinishChooseItem));
	}
	else
	{
		// 掖羲й 嬴檜蠱擊 摹鷗ж朝等 褒ぬц棻.
		SendFieldMsg(ON_RESPONSE_FINISH_CHOOSE_TRADEITEM, SO_SFM_ONLYME, LPVOID(ON_RET_NO));
	}
}


//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	Trade衛 掖羲擊 и棻朝 匙擊 唸薑и棻.
//	熱薑 橾濠	:	2002-07-07 螃瞪 1:05:27 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
VOID	SOPlayer::RecvDecideTrade()
{
	BOOL					bTradeSuccess	=	FALSE;
	DWORD					dwFailReason	=	ON_RET_NO;
	OnReqDecideTradeMsg		*pDecideTradeMsg;
	SOPlayer				*pTargetPlayer	=	NULL;		
	BOOL					bCancelTrade	=	FALSE;	
	BOOL					bInvalidMsg		=	FALSE;
	
	pDecideTradeMsg		=	(OnReqDecideTradeMsg*)cMsg;	

	// ⑷營 剪楚й 嬴檜蠱菟擊 賅舒 摹鷗ц朝雖 憲嬴螞棻.
	if(IsChoseTradeItem() == FALSE)															{	bInvalidMsg	=	TRUE;	goto	END; }
	
	// 釭諦 掖羲ж溥朝 Ы溯檜橫曖 偌羹蒂 橢橫螞棻.
	if((pTargetPlayer =	pMyWorld->GetPlayer(clTrade.GetTradePlayerAccount())) == NULL)		{	bCancelTrade =  TRUE;	goto	END; }

	// 釭諦 掖羲ж溥朝 Ы溯檜橫 羲衛 嬴檜蠱擊 賅舒 摹鷗ц朝雖 憲嬴螞棻.
	if(pTargetPlayer->IsChoseTradeItem(uiAccount) == FALSE)									{	bInvalidMsg	=	TRUE;	goto	END; }

	if(pDecideTradeMsg->bDecideTrade == TRUE)
	{
		if(pTargetPlayer->IsDecideTrading() == TRUE)
		{			
			// 曄薹檜 剪楚蒂 и棻堅 唸薑ц棻.

			// 辦摹 и薹檜塭紫 濰睡陛 氈朝雖 匐餌и棻.
			if( ( clTrade.DoesExistTradeGoods( TRADE_GOODS_TRADE_BOOK ) == TRUE ) || 
				( pTargetPlayer->clTrade.DoesExistTradeGoods( TRADE_GOODS_TRADE_BOOK )  == TRUE ) )
			{
				// 曄薹縑 濰睡蒂 瞪歎醞檜塭堅 撲薑и棻.
				clTrade.SetState( TRADE_STATE_SENDING_TRADE_BOOK );
				pTargetPlayer->clTrade.SetState( TRADE_STATE_SENDING_TRADE_BOOK );
				
				// 曄薹縑 濰睡蒂 瞪歎ж塭堅 憲溥遽棻.
				SendFieldMsg( ON_RESPONSE_DECIDE_TRADE, SO_SFM_ONLYME, LPVOID( ON_RET_SEND_TRADE_BOOK ) );
				pTargetPlayer->SendFieldMsg( ON_RESPONSE_DECIDE_TRADE, SO_SFM_ONLYME, LPVOID( ON_RET_SEND_TRADE_BOOK ) );				
			}
			else
			{
				// 剪楚蒂 ж紫煙 и棻.
				if( Trade( pTargetPlayer ) == FALSE )										{	bCancelTrade = TRUE;	goto	END; }
			}
		}		
		else
		{	
			DecideTrade();
			// 剪楚 蹂羶 薑爾蒂 歎褐, 濠褐/顫檣 setting
			SendFieldMsg( ON_RESPONSE_DECIDE_TRADE, SO_SFM_ONLYME, LPVOID( ON_RET_OK ), LPVOID(TRUE) );			
			pTargetPlayer->SendFieldMsg ( ON_RESPONSE_DECIDE_TRADE, SO_SFM_ONLYME, LPVOID( ON_RET_OK ), LPVOID(FALSE));
		}
	}
	else
	{
		// 檜 睡碟擎 嬴霜 啪歜縑憮 餌辨ж雖 彊朝 睡碟檜棻.

		// 嬴檜蠱 掖羲ж溥朝 匙擊 鏃模ц棻.	

		// 嬴檜蠱 掖羲ж溥朝 匙擊 鏃模и棻.
		if(CancelDecideTrading() == FALSE)													{	bCancelTrade = TRUE;	goto	END; }
		
		// 釭縑啪朝 貲滄檜 薯渠煎 蕾熱腎歷棻堅 憲溥遽棻.
		SendFieldMsg( ON_RESPONSE_DECIDE_TRADE, SO_SFM_ONLYME, LPVOID( ON_RET_OK ) );

		// 掖羲 唸薑縑 渠п憮 鏃模и棻堅 鼻渠寞縑啪 憲溥遽棻.
		pTargetPlayer->SendFieldMsg( ON_RESPONSE_DECIDE_TRADE, SO_SFM_ONLYME, LPVOID( ON_RET_CANCEL_DECIDE_TRADE ) );
	}		


END:

	if( bInvalidMsg	== TRUE )
	{
		SendFieldMsg( ON_RESPONSE_DECIDE_TRADE, SO_SFM_ONLYME, LPVOID( ON_RET_NO ) );
	}

	if( bCancelTrade == TRUE )
	{
		// 掖羲擊 鏃模п撿 и棻.

		// 曄薹縑啪 掖羲擊 鏃模ж塭堅 憲溥遽棻.
		CancelTrade();	
		if( pTargetPlayer != NULL ) pTargetPlayer->CancelTrade();
	}
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	Trade衛 掖羲醞檣 嬴檜蠱擊 熱薑и棻.
//	熱薑 橾濠	:	2002-07-07 螃瞪 5:15:07 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
VOID	SOPlayer::RecvChangeTradeGoods()
{
	OnReqChangeTradeGoods				*pChangeTradeGoods;
	BOOL								bSuccess = FALSE;
	SOPlayer							*pTargetPlayer;
	CHAR								cFollowerBuffer[10240];
	DWORD								dwFollowerDataSize	=	0;
	OnIndustrialComplexTradeInfo		IndustrialComplexTradeInfo;

	pChangeTradeGoods	=	(OnReqChangeTradeGoods*)cMsg;

	// 釭諦 掖羲ж溥朝 Ы溯檜橫曖 偌羹蒂 橢橫螞棻.
	if((pTargetPlayer =	pMyWorld->GetPlayer(clTrade.GetTradePlayerAccount())) == NULL)														goto	END;

	// 釭諦 掖羲ж溥朝 Ы溯檜橫陛 頂陛 輿溥朝 ヶ跡擊 嫡擊 熱 氈朝雖 憲嬴螞棻.	
	if(pTargetPlayer->CanReceiveGoods(pChangeTradeGoods->Kind, pChangeTradeGoods->lParam1, pChangeTradeGoods->lParam2) == FALSE)			goto	END;

	if(AddTradeGoods(pChangeTradeGoods->uiSlot, pChangeTradeGoods->Kind, pChangeTradeGoods->lParam1, pChangeTradeGoods->lParam2, 
						(OnFollowerInfo*)cFollowerBuffer, &dwFollowerDataSize, &IndustrialComplexTradeInfo ) == FALSE)	goto	END;
		
	bSuccess	=	TRUE;

END:
	if(bSuccess == TRUE)
	{		
		SendFieldMsg(ON_RESPONSE_CHANGE_TRADE_GOODS, SO_SFM_ONLYME, LPVOID(ON_RET_OK));
		/*
		pTargetPlayer->SendFieldMsg(ON_CHANGE_TRADE_GOODS, SO_SFM_ONLYME, LPVOID(pChangeTradeGoods->uiSlot), LPVOID(&pChangeTradeGoods->Kind), 
							LPVOID(pChangeTradeGoods->lParam1), LPVOID(pChangeTradeGoods->lParam2), LPVOID(cFollowerBuffer),
							LPVOID( &IndustrialComplexTradeInfo ) );
		*/
		pTargetPlayer->SendFieldMsg(ON_CHANGE_TRADE_GOODS, SO_SFM_ONLYME, LPVOID(pChangeTradeGoods), LPVOID(cFollowerBuffer),
							LPVOID(dwFollowerDataSize), LPVOID( &IndustrialComplexTradeInfo ) );
	}
	else
	{
		SendFieldMsg(ON_RESPONSE_CHANGE_TRADE_GOODS, SO_SFM_ONLYME, LPVOID(ON_RET_NO));
	}
}


//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	Trade衛 濰睡 剪楚陛 諫猿腎歷棻.
//	熱薑 橾濠	:	2002-09-25 螃瞪 11:06:44 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
VOID	SOPlayer::RecvCompleteTradeBook()
{	
	OnReqCompleteTradeBoookMsg	*pCompleteTradeBoookMsg;
	SOPlayer					*pTargetPlayer;
	BOOL						bSuccess = FALSE;	

	pCompleteTradeBoookMsg		=	( OnReqCompleteTradeBoookMsg* )cMsg;

	// 頂陛 濰睡蒂 剪楚醞檣雖 匐餌и棻.
	if( clTrade.IsSendingTradeBook() == FALSE )														goto	END;

	// 釭諦 掖羲ж溥朝 Ы溯檜橫曖 偌羹蒂 橢橫螞棻.
	if( ( pTargetPlayer = pMyWorld->GetPlayer( clTrade.GetTradePlayerAccount() ) ) == NULL )		goto	END;

	// 釭諦 掖羲ж溥朝 Ы溯檜橫 羲衛 濰睡蒂 剪楚醞檣雖 匐餌и棻.
	if( ( pTargetPlayer->clTrade.IsSendingTradeBook() == FALSE ) && 
		( pTargetPlayer->clTrade.DoesCompleteSendingTradeBook() == FALSE ) )						goto	END;	

	switch( pCompleteTradeBoookMsg->dwResponse )
	{
	case ON_RET_OK:
		// 濰睡蒂 撩奢瞳戲煎 剪楚 諫猿ц棻.
		
		// 鼻渠寞 羲衛 濰睡 剪楚蒂 諫猿ц朝雖 匐餌и棻.
		if( pTargetPlayer->clTrade.DoesCompleteSendingTradeBook() == TRUE )
		{
			// 鼻渠寞 羲衛 濰睡 剪楚蒂 諫猿ц棻.

			// 掖羲擊 и棻.
			if( Trade( pTargetPlayer ) == FALSE )
			{
				// 舒 Ы溯檜橫曖 掖羲擊 鏃模 衛鑑棻.				
				goto	CANCEL_TRADE;
			}
		}
		else
		{
			clTrade.SetState( TRADE_STATE_COMPLETE_SENDING_TRADE_BOOK );
			SendFieldMsg( ON_RESPONSE_COMPLETE_TRADE_BOOK, SO_SFM_ONLYME, LPVOID( ON_RET_OK ) ); 
		}

		break;

	case ON_RET_NO:
		// 濰睡蒂 剪楚ж朝等 褒ぬц棻.		

		// 舒 Ы溯檜橫曖 掖羲擊 鏃模 衛鑑棻.
		goto	CANCEL_TRADE;
	}

	bSuccess	=	TRUE;

END:
	if( bSuccess == FALSE )			SendFieldMsg( ON_RESPONSE_COMPLETE_TRADE_BOOK, SO_SFM_ONLYME, LPVOID( ON_RET_NO ) ); 		

	return;

CANCEL_TRADE:

	// 舒 Ы溯檜橫曖 掖羲擊 鏃模衛鑑棻.
	CancelTrade();
	pTargetPlayer->CancelTrade();

	return;
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	掖羲擊 熱辨ц棻.
//	熱薑 橾濠	:	2002-07-06 螃�� 2:14:32 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
BOOL	SOPlayer::AcceptedTrade( UI32 uiOppGuildID, SI32 siOppClassInGuild, SI32 siOppLevel )
{
	CHAR	*pszOppGuildID;
	CHAR	szOppGuildID[ ON_GUILDNAME_LENGTH + 1 ];	

	// 堅羲擊 熱辨и棻.
	if(clTrade.AcceptedTrade() == FALSE)	return	FALSE;	

	// Ы溯檜橫縑啪 掖羲擊 熱辨ж塭堅 爾頂遽棻.
	ZeroMemory( szOppGuildID, sizeof( szOppGuildID ) );
	if( uiOppGuildID != 0 )
	{	
		if( ( pszOppGuildID = pMyWorld->clIGuild.GetGuildName( uiOppGuildID ) ) != NULL )		
			strncpy( szOppGuildID, pszOppGuildID, ON_ID_LENGTH );		
	}

	SendFieldMsg( ON_RESPONSE_ACCEPT_TRADE, SO_SFM_ONLYME, LPVOID( ON_RET_OK ), LPVOID( siOppLevel ), LPVOID( szOppGuildID ), LPVOID( siOppClassInGuild ) );

	return	TRUE;
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	掖羲擊 熱辨ж雖 彊懊棻.
//	熱薑 橾濠	:	2002-07-06 螃�� 3:15:10 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
BOOL	SOPlayer::NotAcceptedTrade()
{
	// 堅羲擊 熱辨и棻.
	if(clTrade.NotAcceptedTrade() == FALSE)	return	FALSE;
	
	// Ы溯檜橫縑啪 掖羲擊 跤и棻堅 憲溥遽棻.
	SendFieldMsg(ON_RESPONSE_ACCEPT_TRADE, SO_SFM_ONLYME, LPVOID(ON_RET_NO));

	return	TRUE;
}


//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	⑷營 霞ч醞檣 掖羲擊 鏃模и棻.
//	熱薑 橾濠	:	2002-07-06 螃�� 2:37:49 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
BOOL	SOPlayer::CancelTrade()
{
	// 掖羲擊 鏃模и棻.
	if(clTrade.CancelTrade() == FALSE)	return	FALSE;
	
	// Ы溯檜橫啪 掖羲檜 鏃模腎歷棻堅 爾頂遽棻.
	SendFieldMsg(ON_RESPONSE_CANCEL_TRADE, SO_SFM_ONLYME, LPVOID(ON_RET_OK));

	return	TRUE;
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	⑷營 棻艇 Ы溯檜橫陛 掖羲擊 熱塊п憮 掖羲擊 ж堅 氈朝雖 憲嬴螞棻.
//	熱薑 橾濠	:	2002-07-06 螃�� 2:48:53 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
BOOL	SOPlayer::IsAcceptedTrading(UI16 uiAccount)
{
	return	 clTrade.IsAcceptedTrading(uiAccount);
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	鼻渠寞檜 掖羲擊 蝓釩ж朝 匙擊 渠晦ж朝雖 憲嬴螞棻.
//	熱薑 橾濠	:	2002-07-06 螃�� 3:10:07 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
BOOL	SOPlayer::IsWaitingAcceptTrade(UI16 uiAccount)
{
	return	clTrade.IsWaitingAcceptTrade(uiAccount);
}


//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	掖羲й 嬴檜蠱擊 摹鷗ц棻.
//	熱薑 橾濠	:	2002-07-06 螃�� 6:41:43 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
BOOL	SOPlayer::ChooseTradeItem()
{
	return	clTrade.ChooseTradeItem();
}


//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	掖羲ж溥堅 ц湍 匙擊 鏃模и棻.
//	熱薑 橾濠	:	2002-07-07 螃瞪 2:06:28 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
BOOL	SOPlayer::CancelDecideTrading()
{
	return	clTrade.CancelDecideTrading();
}


//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	掖羲擊 ж溥堅 唸薑擊 ц朝雖 憲嬴螞棻.
//	熱薑 橾濠	:	2002-07-07 螃瞪 2:18:10 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
BOOL	SOPlayer::IsDecideTrading(UI16 uiAccount)
{
	return	clTrade.IsDecideTrading();
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	掖羲 僭ヶ擊 蹺陛и棻.
//	熱薑 橾濠	:	2002-09-30 螃�� 3:45:07 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
BOOL	SOPlayer::AddTradeGoods(UI08 uiSlotID, TradeGoodsKind Kind, LPARAM lParam1, LPARAM lParam2, OnFollowerInfo *pTradeFollowerInfo,
								DWORD *pdwFollowerDataSize, OnIndustrialComplexTradeInfo *pIndustrialComplexTradeInfo )
{
	UI16							uiFollowerID;
	UI16							uiItemID, uiQuantity;
	MyItemData						*pItemInfo;
	SOVillage_IndustrialComplex		*pIndustrialComplex;


	switch( Kind ) 
	{
	case TRADE_GOODS_ITEM :						// 嬴檜蠱擊 掖羲и棻.		
		

		// 辨煽擊 憲嬴魚棻.
		uiFollowerID	=	UI16(lParam1);	

		// 嬴檜蠱曖 ID蒂 憲嬴魚棻.
		uiItemID		=	HIWORD(lParam2);

		// ⑷營 剪楚醞檣 嬴檜蠱檣雖 匐餌и棻.
		// 剪楚醞檣 嬴檜蠱擊 棻艇 偃熱煎 ж罹 棻衛 螢葬熱朝 橈棻.
		if( clTrade.IsTradingItem( uiFollowerID, uiItemID ) == TRUE )					return	FALSE;

		// 嬴檜蠱曖 熱榆擊 憲嬴魚棻.
		uiQuantity		=	LOWORD(lParam2);

		// п渡 辨煽縑啪 檜 嬴檜蠱檜 熱榆虜躑 氈朝雖 憲嬴魚棻.
		if((pItemInfo  = IFollower.GetItemByID(uiFollowerID, uiItemID)) == NULL)		return	FALSE;

		// 嬴檜蠱曖 熱榆檜 腎朝雖 羹觼и棻.
		if(pItemInfo->uiQuantity < uiQuantity)											return	FALSE;

		break;


	case TRADE_GOODS_FOLLOWER :					// 辨煽擊 掖羲и棻.

		// 辨煽擊 憲嬴魚棻.
		uiFollowerID	=	UI16(lParam1);		

		// 掖羲ж溥朝 辨煽曖 薑爾蒂 橢橫螞棻.
		//if(IFollower.GetFollowerTradeInfo(uiFollowerID, pTradeFollowerInfo) == FALSE)	return	FALSE;
		if( (*pdwFollowerDataSize = GetFollowerInfo( uiFollowerID, pTradeFollowerInfo )) == 0 )		return FALSE;

		break;


	case TRADE_GOODS_MONEY : 					// 絲擊 掖羲и棻.		

		// 絲檜 醱碟�� 氈朝雖 匐餌и棻.
		if(UI32(lParam1) > siMoney )													return	FALSE;

		break;

	case TRADE_GOODS_REALESTATE :				// 睡翕骯擊 掖羲и棻.

		// 頂陛 睡翕骯擊 陛雖堅 氈朝雖 匐餌и棻.
		if( ( pIndustrialComplex = pMyWorld->GetIndustrialComplex( this ) ) == NULL )	return	FALSE;
		
		pIndustrialComplex->GetTradeInfo( pIndustrialComplexTradeInfo );	
		
		break;

	default:
		return	FALSE;

	}

	return	clTrade.AddTradeGoods(uiSlotID, Kind, lParam1, lParam2);
	
}


//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	鼻渠寞檜 輿溥堅 ж朝 僭ヶ菟擊 賅舒 嫡擊 熱 氈朝雖 羹觼и棻.
//	熱薑 橾濠	:	2002-07-07 螃瞪 6:23:43 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
BOOL	SOPlayer::CanReceiveGoods(TradeGoodsKind Kind, LPARAM lParam1, LPARAM lParam2)
{
	SOPlayer		*pTargetPlayer;
	strTradeGoods	*pstTradeGoods;
	UI16			uiItemID[MAX_TRADE_WINDOW];
	UI16			uiItemQuantity[MAX_TRADE_WINDOW];
	UI16			uiItemNum;
	SI32			i;
	UI16			uiFollowerNum;
	UI16			uiFollowerID;
	UI16			uiMonsterNum;

	// 釭諦 掖羲ж溥朝 Ы溯檜橫曖 偌羹蒂 橢橫螞棻.
	if((pTargetPlayer =	pMyWorld->GetPlayer(clTrade.GetTradePlayerAccount())) == NULL)	return	FALSE;

	// 鼻渠寞檜 輿溥堅 ж朝 僭ヶ菟擊 橢橫螞棻.
	if((pstTradeGoods = pTargetPlayer->clTrade.GetTradeGoods()) == NULL)				return	FALSE;

	uiItemNum		=	uiFollowerNum	=	0;
	uiMonsterNum	=	0;

	for(i = 0; i < MAX_TRADE_WINDOW; i++)
	{
		if(pstTradeGoods[i].Kind == TRADE_GOODS_ITEM)
		{
			uiItemID[uiItemNum]				=	HIWORD(pstTradeGoods[i].lParam2);
			uiItemQuantity[uiItemNum]		=	LOWORD(pstTradeGoods[i].lParam2);
			
			uiItemNum++;
		}
		else if(pstTradeGoods[i].Kind == TRADE_GOODS_FOLLOWER)
		{
			uiFollowerID		=	(UI16)lParam1;
			uiFollowerNum++;										// 識 辨煽曖 熱
			if( pTargetPlayer->IsMonster(uiFollowerID) == TRUE )	// 識 跨蝶攪曖 熱
				uiMonsterNum++;
		}
	}

	switch(Kind)
	{
	case TRADE_GOODS_ITEM:

		uiFollowerID					=	UI16(lParam1);

		uiItemID[uiItemNum]				=	HIWORD(lParam2);
		uiItemQuantity[uiItemNum]		=	LOWORD(lParam2);
		
		uiItemNum++;

		// 嬴檜蠱擊 蹺陛 衛鑒 熱 氈朝雖 憲嬴螞棻.
		// 雖旎擎 螃煎雖 輿檣奢 議葛攪虜 嫡朝棻.
		if(CanAddItem(0, uiItemID, uiItemQuantity, uiItemNum) == FALSE)										return	FALSE;
		break;


	case TRADE_GOODS_FOLLOWER:
	{
		uiFollowerID					=	UI16(lParam1);		

		// 頂陛 譆渠 陛韓 熱 氈朝 辨煽爾棻 蟾婁ж雖 彊朝雖 憲嬴螞棻.
		if(uiFollowerNum >= GetMaxFollowerNum())															return	FALSE;

		// 虜擒 剩啖輿溥朝 議葛攪陛 輿檣奢 議葛攪塭賊 褒ぬи棻.
		if(uiFollowerID == 0)																				return	FALSE;

		// 剩啖輿溥朝 議葛攪陛 嶸�褲捘� 憲嬴螞棻.		
		if(pTargetPlayer->IFollower.IsValidFollower(uiFollowerID) == FALSE)									return	FALSE;
		
		// 鼻渠寞檜 濰熱蒂 輿溥堅 ж朝雖 憲嬴螞棻.
		if(pTargetPlayer->IsGeneral(uiFollowerID) == TRUE)					 								return	FALSE;

		// 鼻渠寞檜 跨蝶攪蒂 輿溥堅 ж朝 唳辦 檜嘐 乾堅 氈朝 跨蝶攪熱諦 剪楚璽縑 螢塭螞 跨蝶攪熱曖 м擎 ON_MAXMONSTER爾棻 濛嬴撿 и棻.
		if( pTargetPlayer->IsMonster(uiFollowerID) == TRUE )
		{
			if( GetMonsterNum() + uiMonsterNum >= ON_MAXMONSTER )											return	FALSE;
		}

		uiFollowerNum++;	

		// 辨煽擊 蹺陛 衛鑒 熱 氈朝雖 憲嬴螞棻.
		if( CanAddFollower( uiFollowerNum ) == FALSE)														return	FALSE;
		//if(IFollower.CanAddFollower(uiFollowerNum) == FALSE)												

		//for(SI16 i = 0 ;i < ON_MAX_FOLLOWER_NUM ; i++)
		//{
		OnlineFollower*	 lpOnlineFollower = IFollower.GetFollower(uiFollowerID);
		
		if(lpOnlineFollower != NULL)
		{
			for(SI16 j = 0; j < FIELD_BATTLE_ATTACK_UNIT_NUM ; j++)
			{
				MyItemData*		pMyItemData;
				SI16 siItemID = (SI16)pMyWorld->m_FieldBattleParser.GetItemID(j);
				
				if((pMyItemData = lpOnlineFollower->GetItem(siItemID)) != NULL)
				{
					return FALSE;
				}
			}
		}
	}
	break;



	case TRADE_GOODS_MONEY:			

		// 頂陛 陛韓 熱 氈朝 絲爾棻 號擎 絲擊 輿溥堅 ж朝雖 憲嬴螞棻.
		if( GetMaxMoneyToGet() < UI32( lParam1 ) )															return	FALSE;

		break;

	case TRADE_GOODS_REALESTATE:

		// 檜嘐 頂陛 睡翕骯擊 陛雖堅 氈朝雖 匐餌и棻.
		if( pMyWorld->IsIndustrialComplexOwner( this ) == TRUE )											return	FALSE;
		
		//褐辨紫陛 30 嘐虜 檜塭賊 餌堅 つ 熱 橈棻. - 鼻團  2003. 2. 27- 
		if ( GetTradeGrade() < 30 )
		{
			return FALSE;
		}


		break;

	}

	return	TRUE;
}


//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	鼻渠寞婁 掖羲擊 и棻.
//	熱薑 橾濠	:	2002-09-30 螃�� 3:18:08 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
VOID	SOPlayer::TradeGoods( SOPlayer *pTargetPlayer, DWORD *pdwFollowerDataSize, OnFollowerInfo *pFollowerInfo, UI16 *puiFollowerNum, 
							  DWORD *pdwTradeItemDataSize, UI16 puiTradeItem[ ON_MYDATA_MAX_ITEM_NUM * 2 ], UI16 *puiTradeItemNum, CHAR *pszDBTradeArg )
{	
	SI32			i;
	strTradeGoods	*pstTradeGoods;
	UI08			uiFollowerID, uiOppFollowerID;
	UI16			uiItemID, uiItemQuantity;
	MONEY			mnTradeMoney;
	DWORD			dwFollowerDataSize;
	SI16			siInventoryPos, siOppInventoryPos;
	CHAR			szTempDBArgList[1024];	
	SI32			siDBGoodKind, siDBGoodParam1, siDBGoodParam2, siDBGoodParam3, siDBGoodParam4, siDBGoodParam5, siDBGoodParam6;	

	// 高擊 蟾晦�� и棻.
	*pdwTradeItemDataSize	=	0;
	*pdwFollowerDataSize	=	0;
	*puiFollowerNum			=	0;		
	*puiTradeItemNum		=	0;

	// 鼻渠寞檜 輿溥堅 ц湍 ヶ跡菟擊 賅舒 嫡嬴螞棻.	
	pstTradeGoods	=	pTargetPlayer->clTrade.GetTradeGoods();
	
	// 絲擊 嫡朝棻.
	mnTradeMoney	=	MONEY( pTargetPlayer->clTrade.GetTradeMoney() );
	
	//IncreaseMoney(uiTradeMoney);
	if( pTargetPlayer->GiveMoney( &mnTradeMoney, this ) == FALSE )		mnTradeMoney	=	0;	
	ZeroMemory( szTempDBArgList, sizeof( szTempDBArgList ) );
	sprintf( szTempDBArgList, "%I64d", mnTradeMoney );
	strcat( pszDBTradeArg, szTempDBArgList );	
	
	for(i = 0; i < MAX_TRADE_WINDOW; i++)
	{	
		ZeroMemory( szTempDBArgList, sizeof( szTempDBArgList ) );
		siDBGoodKind = siDBGoodParam1 = siDBGoodParam2 = siDBGoodParam3 = siDBGoodParam4 = siDBGoodParam5 =  siDBGoodParam6 = 0;

		switch( pstTradeGoods[i].Kind )
		{
		case TRADE_GOODS_ITEM :					// 嬴檜蠱檜棻.			

			uiFollowerID	=	UI16(pstTradeGoods[i].lParam1);
			uiItemID		=	HIWORD(pstTradeGoods[i].lParam2);
			uiItemQuantity	=	LOWORD(pstTradeGoods[i].lParam2);
						
			if( pTargetPlayer->GiveItem( uiFollowerID, uiItemID, uiItemQuantity, this, 0, &siInventoryPos, &siOppInventoryPos ) == TRUE )
			{			
				siDBGoodKind	=	1;
				siDBGoodParam1	=	uiItemID;
				siDBGoodParam2	=	uiItemQuantity;
				siDBGoodParam3	=	uiFollowerID;
				siDBGoodParam4	=	siInventoryPos;
				siDBGoodParam5	=	0;
				siDBGoodParam6	=	siOppInventoryPos;

				puiTradeItem[ (*puiTradeItemNum) * 2 ]		=	uiItemID;
				puiTradeItem[ (*puiTradeItemNum) * 2 + 1]	=	siOppInventoryPos;
				(*puiTradeItemNum)++;

				*pdwTradeItemDataSize	+=	( sizeof( UI16 ) * 2 );
			}

			//AddItem(0, uiItemID, uiItemQuantity);

			break;

		case TRADE_GOODS_FOLLOWER :				// 辨煽檜棻.

			uiFollowerID	=	UI16(pstTradeGoods[i].lParam1);

			// 辨煽擊 嫡朝棻.			
			if( ( dwFollowerDataSize = pTargetPlayer->TradeFollower( this, uiFollowerID, pFollowerInfo, &uiOppFollowerID ) ) != 0 )
			{
				// 撩奢瞳戲煎 辨煽擊 嫡懊棻.								
				pFollowerInfo	=	(OnFollowerInfo*)((CHAR*)pFollowerInfo + dwFollowerDataSize);
				(*puiFollowerNum)++;

				*pdwFollowerDataSize	+=	dwFollowerDataSize;

				siDBGoodKind	=	2;
				siDBGoodParam1	=	uiFollowerID;
				siDBGoodParam2	=	uiOppFollowerID;
			}

			break;		

		case TRADE_GOODS_REALESTATE :

			if( pMyWorld->SellIndustrialComplex( pTargetPlayer, this ) == FALSE )
			{
//				clServer->WriteInfo( "..\\SellIndustrialComplex failed.txt", "[ %u ] [ %u ] --> [ %u ] [ %u ]", 
				writeInfoToFile( "SellIndustrialComplex failed.txt", "[ %u ] [ %u ] --> [ %u ] [ %u ]", 
								pTargetPlayer->stAccountInfo.siDBAccount, pTargetPlayer->stAccountInfo.uiSlot,
								stAccountInfo.siDBAccount, stAccountInfo.uiSlot );
			}			

			siDBGoodKind		=	3;

			break;
			
		}		

		sprintf( szTempDBArgList, " , %d, %d, %d, %d, %d, %d, %d", siDBGoodKind, siDBGoodParam1, siDBGoodParam2, siDBGoodParam3, 
														siDBGoodParam4, siDBGoodParam5, siDBGoodParam6 );	
		strcat( pszDBTradeArg, szTempDBArgList );
	}	
	
/*
	// 頂陛 輿溥堅 ц湍 匙菟擊 賅舒 橈撻棻.
	
	// 頂陛 輿溥堅 ц湍 ヶ跡菟擊 賅舒 嫡嬴螞棻.	
	pstTradeGoods	=	clTrade.GetTradeGoods();

	// 絲擊 橈撻棻.
	uiTradeMoney	=	clTrade.GetTradeMoney();
	DecreaseMoney(uiTradeMoney);
	
	for(i = 0; i < MAX_TRADE_WINDOW; i++)
	{	
		switch( pstTradeGoods[i].Kind )
		{
		case TRADE_GOODS_ITEM :				// 嬴檜蠱檜棻.
			
			uiFollowerID	=	UI16(pstTradeGoods[i].lParam1);
			uiItemID		=	HIWORD(pstTradeGoods[i].lParam2);
			uiItemQuantity	=	LOWORD(pstTradeGoods[i].lParam2);
			
			DelItem(uiFollowerID, uiItemID, uiItemQuantity);

			break;

		case TRADE_GOODS_FOLLOWER :			// 辨煽檜棻.

			// 辨煽擎 鼻渠寞檜 釭曖 Follower蒂 陛螳陛賊憮 橈橫霞棻.

			break;
		}
	}		
*/
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	pTradePlayer縑啪 uiFollowerID曖 辨煽擊 遽棻.
//	熱薑 橾濠	:	2002-07-07 螃瞪 11:45:11 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
DWORD	SOPlayer::TradeFollower(SOPlayer *pTradePlayer, UI16 uiFollowerID, OnFollowerInfo *pFollowerInfo, UI08 *puiOppFollowerID)
{
	DWORD				dwFollowerDataSize;
	SI32				siNation;
	//DBIKGiveFollower	DBGiveFollower;
	SI16				siEmptyFollowerSlot;

	// 輿檣奢檜賊 跤遽棻.
	if(uiFollowerID == 0)		return	0;

	// 辨煽曖 薑爾蒂 橢橫螃朝等 撩奢п撿и棻.
	if((dwFollowerDataSize = GetFollowerInfo(uiFollowerID, pFollowerInfo)) == 0)		return	0;

	// 辨煽曖 措陛蒂 橢橫螞棻.
	siNation	=	IFollower.GetFollowerParameterNation(uiFollowerID);

	// 辦摹 頂 辨煽擎 餉薯и棻. 
	if(IFollower.DeleteFollower(uiFollowerID) == FALSE)									return	0;
	
	// 辨煽擊 嫡朝 Ы溯檜橫陛 辨煽擊 橫蛤縑 蹺陛衛麵撿 ж朝雖 憲嬴螞棻.
	if((siEmptyFollowerSlot = pTradePlayer->IFollower.FindEmptyFollowerSlot()) == -1)	return	0;

	// 鼻渠寞縑啪 頂 辨煽擊 蹺陛衛麵遽棻.
	if(pTradePlayer->IFollower.InitFollower(pFollowerInfo, siNation, &pMyWorld->clItem) == TRUE)	
	{
		*puiOppFollowerID				=	siEmptyFollowerSlot;

		if(uiFollowerID == siVMercenarySlot)
		{
			RecvVMercenaryChange(0);
		}

		return	dwFollowerDataSize;
	}
	else	
	{
//		clServer->WriteInfo( "..\\FollowerTradeError.txt", 	"InitFollower failed" );
		writeInfoToFile( "FollowerTradeError.txt", 	"InitFollower failed" );
		return	0;
	}
}	


//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	掖羲璽擊 殘朝棻.
//	熱薑 橾濠	:	2002-07-08 螃�� 8:08:38 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
VOID	SOPlayer::CloseTrade()
{	
	SOPlayer		*pTargetPlayer;		

	// ⑷營 掖羲醞檜 嬴棲塭賊 斜傖 釭除棻.
	if(IsTrading() == FALSE)															return;

	// 釭諦 掖羲ж溥朝 Ы溯檜橫曖 偌羹蒂 橢橫螞棻.
	if((pTargetPlayer =	pMyWorld->GetPlayer(clTrade.GetTradePlayerAccount())) == NULL)	return;

	// 鼻渠寞檜 掖羲縑 擬港ж晦蒂 晦棻葬堅 氈朝 鼻鷓諦
	// 掖羲醞縑 氈擊陽朝 詭衛雖蒂 殖葬п遽棻.
	if(pTargetPlayer->IsWaitingAcceptTrade(uiAccount) == TRUE)		pTargetPlayer->SendFieldMsg(ON_RESPONSE_ACCEPT_TRADE, SO_SFM_ONLYME, LPVOID(ON_RET_NO));
	if(pTargetPlayer->IsAcceptedTrading(uiAccount) == TRUE)			pTargetPlayer->SendFieldMsg(ON_RESPONSE_CANCEL_TRADE, SO_SFM_ONLYME, LPVOID(ON_RET_NO));
	
	// 釭諦 鼻渠寞曖 掖羲璽擊 蟾晦�� и棻.
	clTrade.Init();
	pTargetPlayer->clTrade.Init();
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	頂陛 掖羲ж溥朝 ヶ跡檜 嬴霜 嶸�褲捘� 匐餌и棻
//	熱薑 橾濠	:	2002-09-23 螃�� 12:19:38 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
BOOL	SOPlayer::IsValidTradeGoods()
{
	SI32			i;
	MyItemData		*pMyItemData;
	strTradeGoods	*pstTradeGoods;
	UI16			uiFollowerID;
	UI16			uiItemID;
	UI16			uiItemQuantity;

	if(clTrade.IsTrading() == FALSE)	return	FALSE;
	
	pstTradeGoods	=	clTrade.GetTradeGoods();

	for(i = 0; i < MAX_TRADE_WINDOW; i++)
	{	
		switch( pstTradeGoods[ i ].Kind )
		{
		case TRADE_GOODS_ITEM :

			// 嬴檜蠱擊 嫡朝棻.
			uiFollowerID	=	UI16(pstTradeGoods[i].lParam1);
			uiItemID		=	HIWORD(pstTradeGoods[i].lParam2);
			uiItemQuantity	=	LOWORD(pstTradeGoods[i].lParam2);
			
			if( ( pMyItemData =	IFollower.GetItemByID( uiFollowerID, uiItemID ) ) == NULL)	return	FALSE;			
			
			// 熱榆擊 匐餌и棻.
			if( pMyItemData->uiQuantity < uiItemQuantity )									return	FALSE;			

			break;

		case TRADE_GOODS_FOLLOWER :
			
			uiFollowerID	=	UI16(pstTradeGoods[i].lParam1);			

			if( IFollower.GetFollower( uiFollowerID ) == NULL)								return	FALSE;

			break;

		case TRADE_GOODS_REALESTATE:

			if( pMyWorld->IsIndustrialComplexOwner( this ) == FALSE )						return	FALSE;

			break;
		}		
	}		

	if( !EnoughMoney( clTrade.GetTradeMoney() )	)											return	FALSE;
	
	return	TRUE;
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	掖羲醞檣 ヶ跡縑 渠п憮 剪楚蒂 ж塭堅 憲溥遽棻.
//	熱薑 橾濠	:	2002-09-24 螃�� 8:09:18 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
BOOL	SOPlayer::Trade( SOPlayer *pTargetPlayer )
{
	DWORD					dwFollowerDataSize1, dwFollowerDataSize2;
	DWORD					dwTradeItemDataSize1, dwTradeItemDataSize2;
	BOOL					bSuccess = FALSE;
	CHAR					cFollowerBuffer1[10240], cFollowerBuffer2[10240];
	UI16					uiFollowerNum1, uiFollowerNum2;		
	UI16					uiTradeItemNum1, uiTradeItemNum2;
	CHAR					szAccountDBArg[1024];
	CHAR					szCompletedDBArg[1024];	
	CHAR					szTrade1DBArg[1024];
	CHAR					szTrade2DBArg[1024];
	UI16					uiTradeItem1[ ON_MYDATA_MAX_ITEM_NUM * 2 ], uiTradeItem2[ ON_MYDATA_MAX_ITEM_NUM * 2 ];

	ZeroMemory( szAccountDBArg, sizeof( szAccountDBArg ) );	
	ZeroMemory( szTrade1DBArg, sizeof( szTrade1DBArg ) );
	ZeroMemory( szTrade2DBArg, sizeof( szTrade2DBArg ) );

	// 釭諦 鼻渠寞檜 掖羲ж溥朝 ヶ跡菟檜 賅舒 嶸�褲捘� 憲嬴螞棻.
	if(IsValidTradeGoods() == FALSE)																		goto	END;
	if(pTargetPlayer->IsValidTradeGoods() == FALSE)															goto	END;							

	// 鼻渠寞 羲衛 嬴檜蠱擊 掖羲й 匙擊 唸薑ц棻.
	TradeGoods(pTargetPlayer, &dwFollowerDataSize1, (OnFollowerInfo*)cFollowerBuffer1, &uiFollowerNum1, 
					&dwTradeItemDataSize1, uiTradeItem1, &uiTradeItemNum1, szTrade2DBArg );
	pTargetPlayer->TradeGoods(this, &dwFollowerDataSize2, (OnFollowerInfo*)cFollowerBuffer2, &uiFollowerNum2,
					&dwTradeItemDataSize2, uiTradeItem2, &uiTradeItemNum2, szTrade1DBArg );
	
	// 棻衛 掖羲擊 й 熱 氈朝 鼻鷓煎 撲薑и棻.
	clTrade.Init();
	pTargetPlayer->clTrade.Init();
	
	// DB 貲滄僥擊 虜萇棻.
	sprintf( szAccountDBArg, "%d, %d, %d, %d",	stAccountInfo.siDBAccount, stAccountInfo.uiSlot, 
													pTargetPlayer->stAccountInfo.siDBAccount, pTargetPlayer->stAccountInfo.uiSlot );	

	// DB縑 盪濰и棻.
	ZeroMemory( szCompletedDBArg, sizeof( szCompletedDBArg ) );
	sprintf( szCompletedDBArg, "%s, %s, %s", szAccountDBArg, szTrade1DBArg, szTrade2DBArg );
	pMyWorld->clDatabase->writeDBQuery( SO_DB_INFOKIND_TRADE, LPVOID( szCompletedDBArg ), strlen( szCompletedDBArg ) + 1 );

	bSuccess	=	TRUE;

END:

	if( bSuccess == TRUE)
	{
		// 掖羲檜 撩董腎歷棻.
		SendFieldMsg( ON_COMPLETE_TRADE, SO_SFM_ONLYME, LPVOID( dwFollowerDataSize1 ), LPVOID( cFollowerBuffer1 ), LPVOID( uiFollowerNum1 ),
										LPVOID( dwTradeItemDataSize1 ), LPVOID( uiTradeItem1 ), LPVOID( uiTradeItemNum1 ) );
		pTargetPlayer->SendFieldMsg( ON_COMPLETE_TRADE , SO_SFM_ONLYME, LPVOID( dwFollowerDataSize2 ), 	LPVOID( cFollowerBuffer2 ), LPVOID( uiFollowerNum2 ),
										LPVOID( dwTradeItemDataSize2 ), LPVOID( uiTradeItem2 ), LPVOID( uiTradeItemNum2 ) );	
	}
	else
	{
		// 掖羲檜 褒ぬц棻. 
		// 掖羲擊 鏃模 衛鑑棻.

		CancelTrade();
		pTargetPlayer->CancelTrade();
	}
	
	return	bSuccess;
}
	
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	嬴檜蠱擊 模賅и棻.
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
BOOL	SOPlayer::SpendItem(UI08 uiFollowerID, UI16 uiItemID, UI16 uiPosInventory, UI16 uiQuantity)
{
	CItemHeader				*pItemHeader = NULL;
	_ParametaBox			*pParaBox=NULL;	
	SI32					NewValue=0;
	BOOL                    bSendDeleteQuery = TRUE;



	if( IFollower.IsValidFollower(uiFollowerID) == FALSE )
	{
		
		return FALSE;
	}	

	pItemHeader = pMyWorld->clItem.GetItem(uiItemID);
	if(pItemHeader == NULL)
	{
		
		return FALSE;
	}

	pParaBox	= &IFollower.GetFollower(uiFollowerID)->ParametaBox;
	if(pParaBox == NULL)
	{
		
		return FALSE;
	}

	if( pItemHeader->m_Attribute & ON_ITEM_USE_POSSIBLE )
	{
		if( HasItem(uiFollowerID, uiItemID , uiQuantity, uiPosInventory) != NULL)
		{
			// ん虜馬擎 斜傖螃艇棻.
			if(pItemHeader->m_siHealth != 0)
			{
				if(pItemHeader->m_siGroupCode &  ON_ITEM_ALL_RECOVER )
					ChangeSatiety( static_cast<SI32>((float)ON_MAX_HEALTH * ((float)pItemHeader->m_siHealth / 100.0)) );
				else
					ChangeSatiety(pItemHeader->m_siHealth);
			}
			
			// 瞪羹瞳辨嬴檜蠱檜 嬴棲賊 
			if ( !(pItemHeader->m_siGroupCode & ON_ITEM_TOALL) )
			{				
				if(pItemHeader->m_siHpGain != 0)
				{
					NewValue = ((pParaBox->GetMaxLife()) < (pParaBox->IP.GetLife() + (pItemHeader->m_siHpGain * uiQuantity)) ? pParaBox->GetMaxLife():(pParaBox->IP.GetLife() + pItemHeader->m_siHpGain));
					pParaBox->IP.SetLife(NewValue);
				}
				if(pItemHeader->m_siMpGain != 0)
				{
					NewValue = ((pParaBox->GetMaxMana()) < (pParaBox->IP.GetMana() + pItemHeader->m_siMpGain * uiQuantity) ? pParaBox->GetMaxMana():(pParaBox->IP.GetMana() + pItemHeader->m_siMpGain));					
					pParaBox->IP.SetMana(NewValue, 3);
				}

				// か熱 �膩�陛 氈朝 嬴檜蠱檜棻.
				if (pItemHeader->m_siGroupCode & ON_ITEM_SPECIAL)
				{

					switch(pItemHeader->m_siSpecial)
					{
					case ON_ITEM_SPECIAL_INITIALIZE:
						{							
							OnlineFollower *pFollower = IFollower.GetFollower(uiFollowerID);
							
							SI32 siBonusDelta;
							
							if (pFollower == NULL) return FALSE;
							
							// 罹晦憮 uiFollowerID 朝 賃廓簞 辨煽檜傍蒂 嗆ж堅
							// uiCharID 朝 辨煽 堅嶸曖 嬴檜蛤檜棻.
							IFollower.ChangeFollowerParameterToBonus(uiFollowerID, &(pMyWorld->clCharParser), pFollower->uiCharID,&siBonusDelta,&(pMyWorld->clItem));
							
							DBIKInitMercenaryParamByItem MercenaryParam;
							MercenaryParam.siDBAccount = stAccountInfo.siDBAccount;
							MercenaryParam.uiDBSlot = stAccountInfo.uiSlot;
							MercenaryParam.uiMercenarySlot = uiFollowerID;
							MercenaryParam.uiItemID = uiItemID;
							MercenaryParam.siBonus = siBonusDelta;
							
							pMyWorld->clDatabase->writeDBQuery( SO_DB_INFOKIND_INITMERCENARYPARAM, LPVOID( &MercenaryParam ), sizeof(MercenaryParam) );
							
							bSendDeleteQuery = FALSE;
						}
						break;
					case ON_ITEM_SPECIAL_SHOWMERCENARY:
						{
							// 0廓辨煽檜 噙賊 褒ぬ
							if(uiFollowerID == 0)
							{
								return FALSE;
							}
							// 檜嘐 檜辨煽縑 瞳辨腋橫 氈戲賊 褒ぬ
							if(siVMercenarySlot == uiFollowerID)
							{
								return FALSE;
							}

							// 辨煽 爾歜 撲薑
							RecvVMercenaryChange(uiFollowerID);

							DBIKVMercenaryShow			VMercenaryShow;
							
							VMercenaryShow.siDBAccount     = stAccountInfo.siDBAccount;
							VMercenaryShow.uiDBSlot        = stAccountInfo.uiSlot;
							VMercenaryShow.uiMercenarySlot = uiFollowerID;
							VMercenaryShow.uiItemID        = uiItemID;
							pMyWorld->clDatabase->writeDBQuery(SO_DB_INFOKIND_VMERCENARYSHOW, LPVOID(&VMercenaryShow), sizeof(DBIKVMercenaryShow));

							bSendDeleteQuery = FALSE;
						}
						break;
					
					case ON_ITEM_SPECIAL_CHANGESEX:
						{


							// 0廓辨煽檜 嬴棲賊 褒ぬ
							if(uiFollowerID != 0)
							{
								return FALSE;
							}

							OnlineFollower *pFollower = IFollower.GetFollower(uiFollowerID);

							// 殮堅氈朝 濰綠蒂 橢橫螞棻.
							UI16 puiWearItems[WEAR_ITEM_NUM];
							pFollower->GetWearItems(puiWearItems);

							// 鼠晦蒂 濰綠ж堅氈戲賊 褒ぬ
							if (puiWearItems[ON_WEAR_WEAPON_ITEM] != 0)							
							{
								return FALSE;
							}

							switch (pFollower->uiCharID)
							{
								// 褻摹陴
							case 67:
								pFollower->uiCharID = 323;
								uiKind = pFollower->uiCharID;
								break;
								// 褻摹喪
							case 323:
								pFollower->uiCharID = 67;
								uiKind = pFollower->uiCharID;
								
								break;
								// 橾獄陴
							case 8771:
								pFollower->uiCharID = 9027;
								uiKind = pFollower->uiCharID;
								
								break;
								// 橾獄喪
							case 9027:
								pFollower->uiCharID = 8771;
								uiKind = pFollower->uiCharID;

								break;
								// 渠虜陴
							case 10307:
								pFollower->uiCharID = 10563;
								uiKind = pFollower->uiCharID;
								break;
								// 渠虜喪
							case 10563:
								pFollower->uiCharID = 10307;
								uiKind = pFollower->uiCharID;
								break;
								// 醞措陴
							case 12867:
								pFollower->uiCharID = 13123;
								uiKind = pFollower->uiCharID;
								break;
								// 醞措喪
							case 13123:
								pFollower->uiCharID = 12867;
								uiKind = pFollower->uiCharID;
								break;
														

							}

							if (IsInVillage()) {
								SendFieldMsg(ON_CHANGE_CHARACTERCODE, SO_SFM_ONLYME, LPVOID(this),LPVOID(pFollower->uiCharID));
							}
							else {
								SendFieldMsg(ON_CHANGE_CHARACTERCODE, SO_SFM_MEANDNEARCHAR, LPVOID(this),LPVOID(pFollower->uiCharID));
							}
							
							// 辨煽 爾歜 撲薑
							

							DBIKChangeSex			ChangeSex;
							
							ChangeSex.siDBAccount     = stAccountInfo.siDBAccount;
							ChangeSex.uiDBSlot        = stAccountInfo.uiSlot;
							ChangeSex.uiNewCharactorCode = pFollower->uiCharID;
							ChangeSex.uiItemID        = uiItemID;
							pMyWorld->clDatabase->writeDBQuery(SO_DB_INFOKIND_CHANGESEX, LPVOID(&ChangeSex), sizeof(ChangeSex));

							bSendDeleteQuery = FALSE;
						}
						break;
					case ON_ITEM_SPECIAL_IMPERIUM:
						{
							UI16 uiPrevKind;
							BOOL bCaptain;
							SI16 siHp = 0;

							if(uiFollowerID != 0)
							{
								return FALSE;
							}
							//奢撩擊 й熱 氈朝 陳檣陛 羹觼.
							if(pMyWorld->clIGuild.GetGuildWarCheck(uiGuildID) == FALSE)
							{
								return FALSE;
							}
							OnlineFollower *pFollower = IFollower.GetFollower(uiFollowerID);
							
							//奢撩嶸棉->奢撩嶸棉 戲煎 滲и 唳辦塭賊 盪濰п 場擎 PrevKind蒂 棻衛 盪濰.
							//斜溢雖 彊擎 唳辦塭賊 橾奩 嶸棉曖 PrevKind蒂 盪濰.
							if(pMyWorld->m_FieldBattleParser.IsFieldAttackUnit(uiKind) == TRUE)
								uiPrevKind = GetPrevKind();								
							else
								uiPrevKind = uiKind;

							//Character 諦 Mercenary 曖 議葛攪 Kind萃棻 滲唳и棻.
							//憮幗縑摹 萃曖 Kind虜 滲唳п輿堅 DB縑 盪濰й陽朝 CharactorWar纔檜綰縑 盪濰и棻.
							uiKind				= pItemHeader->m_pImperium->m_uiKind;
							pFollower->uiCharID = pItemHeader->m_pImperium->m_uiKind;
							
							//uiQuantity : 渠濰曖 唳辦 2濰曖 模餵掏擊 餌辨и棻.
							//             1濰橾 唳辦 橾奩嶸棉 2濰橾 唳辦 渠濰嶸棉.
							//渠濰橾唳辦縛 渠濰檜塭朝 ル衛蒂 爾頂堅 羹溘擊 舒寡煎 虜萇棻.
							if(uiQuantity == 1)
							{
								bCaptain = FALSE;
								siHp = pItemHeader->m_pImperium->m_siHp;
							}
							else
							{
								bCaptain = TRUE;
								siHp = pItemHeader->m_pImperium->m_siHp*2;
							}
							
							if (IsInVillage()) 
								SendFieldMsg(ON_IMPERIUM_USE, SO_SFM_ONLYME, LPVOID(this),LPVOID(pItemHeader->m_pImperium->m_uiKind)
									,LPVOID(siHp),LPVOID(pItemHeader->m_pImperium->m_siSupply)
									,LPVOID(bCaptain),LPVOID(uiPrevKind));
							else
								SendFieldMsg(ON_IMPERIUM_USE, SO_SFM_MEANDNEARCHAR, LPVOID(this),LPVOID(pItemHeader->m_pImperium->m_uiKind)
									,LPVOID(siHp),LPVOID(pItemHeader->m_pImperium->m_siSupply)
									,LPVOID(bCaptain),LPVOID(uiPrevKind));						

							DBIKImperumUse		ImperumUse;
/*
							ImperumUse.siDBAccount	   = stAccountInfo.siDBAccount;
							ImperumUse.uiDBSlot        = stAccountInfo.uiSlot;
							ImperumUse.uiNewKind	   = pItemHeader->m_pImperium->m_uiKind;
							ImperumUse.uiItemID		   = uiItemID; //Item ID煎 陛雖 彊堅 Index煎 陞蛭.
							ImperumUse.uiQuantity	   = uiQuantity;
							ImperumUse.bCaptain		   = bCaptain;
							ImperumUse.siHp			   = siHp;
							ImperumUse.siSupply		   = 0;        // 橾奩 嶸棉擎 爾晝 啪檜雖蒂 0戲煎 и棻.
*/
							if(uiQuantity == 1)
								ImperumUse.bCaptain		   = FALSE;
							else 
								ImperumUse.bCaptain		   = TRUE;
							
							SetFieldHp(siHp);			//羹溘   撲薑.

							//pMyWorld->m_FieldBattleParser.
							
							SetPrevKind(uiPrevKind);								//Prev Kind 撲薑.
							SetCaptainUnit(bCaptain);								//渠濰嶸棉檣雖 撲薑.

							//橾奩 嶸盪菟擎 爾晝擊 撲薑и棻.
							//橾奩 奢撩嶸棉曖 爾晝榆擎 憮幗縑憮 婦葬ж雖 彊朝棻.
							if(pMyWorld->m_FieldBattleParser.GetUnitKind(pItemHeader->m_pImperium->m_uiKind) != FIELD_BATTLE_SUPPLY)
							{
	//							SetFieldSupply((SI32)pItemHeader->m_pImperium->m_siSupply);   //爾晝榆 撲薑.
	//							InitFieldSupplyCharge(0);
							}
							//爾晝熱溯朝 爾晝醱瞪榆擊 撲薑и棻.
							else if(pMyWorld->m_FieldBattleParser.GetUnitKind(pItemHeader->m_pImperium->m_uiKind) == FIELD_BATTLE_SUPPLY)
							{
	//							InitFieldSupplyCharge((SI32)pItemHeader->m_pImperium->m_siSupply); // 爾晝醱瞪榆 撲薑.
//								ImperumUse.siSupply = (SI16)pItemHeader->m_pImperium->m_siSupply; // 蛤綠縑 爾晝醱瞪 啪檜雖蒂 盪濰.
							}

							pMyWorld->clDatabase->writeDBQuery(SO_DB_INFOKIND_IMPERIUM_USE, LPVOID(&ImperumUse), sizeof(DBIKImperumUse));
							bSendDeleteQuery = FALSE;

						}
						break;
					case ON_ITEM_SPECIAL_CHANGENATION:
						{
							// 0廓辨煽檜 嬴棲賊 褒ぬ
							if(uiFollowerID != 0)
							{
								return FALSE;
							}

							OnlineFollower *pFollower = IFollower.GetFollower(uiFollowerID);

							

							// 殮堅氈朝 濰綠蒂 橢橫螞棻.
							UI16 puiWearItems[WEAR_ITEM_NUM];
							pFollower->GetWearItems(puiWearItems);

							// 鼠晦蒂 濰綠ж堅氈戲賊 褒ぬ
							if (puiWearItems[ON_WEAR_WEAPON_ITEM] != 0)							
							{
								return FALSE;
							}

							// 措瞳檜 滲ж溥朝 措陛諦 偽戲賊 褒ぬ
							if (pFollower->Nation == pItemHeader->m_siSpecialParam) return FALSE;


							// 議葛 kind 滲唳
							switch(pItemHeader->m_siSpecialParam)
							{
							// 褻摹敝��
							case 1:
								switch (pFollower->uiCharID)
								{
									// 陴濠
								case 67:
								case 8771:
								case 10307:
								case 12867:
									// 褻摹陴戲煎 滲褐
									pFollower->uiCharID = 67;
									uiKind = pFollower->uiCharID;
								
									
									break;
									// 罹濠.
								case 323:
								case 9027:
								case 10563:
								case 13123:
									pFollower->uiCharID = 323;
									uiKind = pFollower->uiCharID;
								
									break;
								}
								
								break;
							// 橾獄敝��
							case 10:
								switch (pFollower->uiCharID)
								{
									// 陴濠
								case 67:
								case 8771:
								case 10307:
								case 12867:

									pFollower->uiCharID = 8771;
									uiKind = pFollower->uiCharID;
								
									break;
									// 罹濠.
								case 323:
								case 9027:
								case 10563:
								case 13123:

									pFollower->uiCharID = 9027;
									uiKind = pFollower->uiCharID;
								
									break;
								}
								
								break;
							// 渠虜敝��
							case 1000:
								switch (pFollower->uiCharID)
								{
									// 陴濠
								case 67:
								case 8771:
								case 10307:
								case 12867:

									pFollower->uiCharID = 10307;
									uiKind = pFollower->uiCharID;
								
									break;
									// 罹濠.
								case 323:
								case 9027:
								case 10563:
								case 13123:

									pFollower->uiCharID = 10563;
									uiKind = pFollower->uiCharID;
								
									break;
								}
								
								break;
							// 醞措敝��
							case 100:
								switch (pFollower->uiCharID)
								{
									// 陴濠
								case 67:
								case 8771:
								case 10307:
								case 12867:
									pFollower->uiCharID = 12867;
									uiKind = pFollower->uiCharID;								
									break;
									// 罹濠.
								case 323:
								case 9027:
								case 10563:
								case 13123:
									pFollower->uiCharID = 13123;
									uiKind = pFollower->uiCharID;								
									break;
								}								
								break;
							}

							// 措瞳滲唳
							pFollower->Nation = pItemHeader->m_siSpecialParam;

							// 蠡蝶お 餉薯

							Quest_AllClear();

							
							// 滲褐 詭衛雖 陳葡

							if (IsInVillage()) {
								SendFieldMsg(ON_CHANGE_CHARACTERCODE, SO_SFM_ONLYME, LPVOID(this),LPVOID(pFollower->uiCharID));
							}
							else {
								SendFieldMsg(ON_CHANGE_CHARACTERCODE, SO_SFM_MEANDNEARCHAR, LPVOID(this),LPVOID(pFollower->uiCharID));
							}

							
							
							// 滲褐 db蘭葬陳葡
							

							DBIKChangeNation			ChangeNation;
							
							ChangeNation.siDBAccount     = stAccountInfo.siDBAccount;
							ChangeNation.uiDBSlot        = stAccountInfo.uiSlot;
							ChangeNation.uiNewCharactorCode = pFollower->uiCharID;
							ChangeNation.uiItemID        = uiItemID;
							pMyWorld->clDatabase->writeDBQuery(SO_DB_INFOKIND_CHANGENATION, LPVOID(&ChangeNation), sizeof(ChangeNation));

							bSendDeleteQuery = FALSE;
						}
						break;
					case ON_ITEM_SPECIAL_CREDITUP:
						{
							IncreaseTradeCredit(pItemHeader->m_siSpecialParam,FALSE);

							DBIKConsumeCreditItem  DBInfo;
							
							DBInfo.siDBAccount = stAccountInfo.siDBAccount;
							DBInfo.uiDBSlot = stAccountInfo.uiSlot;
							DBInfo.uiFollowerID = uiFollowerID;
							DBInfo.uiCreditUp = pItemHeader->m_siSpecialParam;
							DBInfo.uiItemID = uiItemID;

							pMyWorld->clDatabase->writeDBQuery(SO_DB_INFOKIND_CONSUMECREDITITEM, LPVOID(&DBInfo), sizeof(DBInfo));
							
							bSendDeleteQuery = FALSE;

						}
						break;
					case ON_ITEM_SPECIAL_TACTICS:		// 槳濠 煽徹 餌辨戲煎 譆渠 堅辨 濰熱 隸陛 譆渠 5貲 梱雖 隸陛 陛棟л.
						{
							if( m_siMaxGeneral < ON_MAXGENERAL_HIGH )		m_siMaxGeneral++;
							else											return FALSE;

							DBIKIncreaseMaxGeneral	DBInfo;

							DBInfo.siDBAccount	=	stAccountInfo.siDBAccount;
							DBInfo.uiDBSlot		=	stAccountInfo.uiSlot;
							DBInfo.uiFollowerID	=	uiFollowerID;
							DBInfo.uiItemID		=	uiItemID;

							pMyWorld->clDatabase->writeDBQuery(SO_DB_INFOKIND_INCREASEMAXGENERAL, LPVOID(&DBInfo), sizeof(DBInfo));

							bSendDeleteQuery = FALSE;
						}
						break;
					case ON_ITEM_SPECIAL_DONKEY:		// 渡釭敝蒂 餌辨ж賊 辨煽 蝸煜縑 渡釭敝陛 и葆葬 菟橫除棻.
						{
							if( !OnUseDonkeyItem(uiFollowerID, uiItemID) )		return FALSE;
							bSendDeleteQuery	=	FALSE;
						}
						break;

					case ON_ITEM_SPECIAL_MAGICSTONERED:
					case ON_ITEM_SPECIAL_MAGICSTONEBLUE:
					case ON_ITEM_SPECIAL_MAGICSTONEBLACK:
						{
							if( IFollower.CanWeaponBeUpgraded(uiFollowerID) )					// 機斜溯檜萄 陛棟и 鼠晦檣雖 匐餌и棻.
							{
								OnItemUpgradeResultInfo		*stItemUpgradeResultInfo;
								UI16						puiWearItems[WEAR_ITEM_NUM];
								SI32						siMagicStoneKind;
								
								switch( pItemHeader->m_siSpecial )
								{
								case ON_ITEM_SPECIAL_MAGICSTONERED:			siMagicStoneKind	=	MAGIC_STONE_RED;		break;
								case ON_ITEM_SPECIAL_MAGICSTONEBLUE:		siMagicStoneKind	=	MAGIC_STONE_BLUE;		break;
								case ON_ITEM_SPECIAL_MAGICSTONEBLACK:		siMagicStoneKind	=	MAGIC_STONE_BLACK;		break;
								}
								// п渡 辨煽檜 雜辨醞檣 嬴檜蠱菟擊 橢橫螞棻.
								IFollower.GetWearItems( uiFollowerID, puiWearItems );
								// п渡 葆戮戲煎 雜辨醞檣 鼠晦 嬴檜蠱擊 陛奢и 唸婁蒂 橢朝棻.
								stItemUpgradeResultInfo		=	pMyWorld->m_ItemUpgradeParser.GetUpgradeResult( puiWearItems[ON_WEAR_WEAPON_ITEM], siMagicStoneKind );

								// 陛奢 唸婁 陛奢 碳陛棟檜賊 斜傖 葬欐衛鑑棻. (葆戮擎 模賅腎雖 彊朝棻.)
								if( stItemUpgradeResultInfo->siResult == ITEM_UPGRADE_IMPOSSIBLE )			return FALSE;
								
								if( stItemUpgradeResultInfo->siResult == ITEM_UPGRADE_SUCCESS )
								{
									// 嬴檜蠱擊 機斜溯檜萄 п輿堅 DB縑 撩奢 蘭葬蒂 陳溥遽棻.
									IFollower.UpgradeWeapon( uiFollowerID, stItemUpgradeResultInfo->uiItemID );		// 嬴檜蠱 機斜溯檜萄

									DBIKUpgradeWeaponSuccess	DBInfo;

									DBInfo.siDBAccount = stAccountInfo.siDBAccount;
									DBInfo.uiDBSlot = stAccountInfo.uiSlot;
									DBInfo.uiFollowerID = uiFollowerID;
									DBInfo.uiUpgradeItemID = stItemUpgradeResultInfo->uiItemID;
									DBInfo.uiItemID = uiItemID;

									pMyWorld->clDatabase->writeDBQuery(SO_DB_INFOKIND_UPGRADEWEAPONSUCCESS, LPVOID(&DBInfo), sizeof(DBInfo));
									
									bSendDeleteQuery = FALSE;
								}
								else if( stItemUpgradeResultInfo->siResult == ITEM_UPGRADE_FAIL )
								{
									// DB縑 褒ぬ 蘭葬虜 陳溥遽棻.
									IFollower.UpgradeWeapon( uiFollowerID, stItemUpgradeResultInfo->uiItemID );		// 嬴檜蠱 機斜溯檜萄
									DBIKUpgradeWeaponFailure	DBInfo;

									DBInfo.siDBAccount = stAccountInfo.siDBAccount;
									DBInfo.uiDBSlot = stAccountInfo.uiSlot;
									DBInfo.uiFollowerID = uiFollowerID;
									DBInfo.uiItemID = uiItemID;

									pMyWorld->clDatabase->writeDBQuery(SO_DB_INFOKIND_UPGRADEWEAPONFAILURE, LPVOID(&DBInfo), sizeof(DBInfo));
									
									bSendDeleteQuery = FALSE;
								}
								else if( stItemUpgradeResultInfo->siResult == ITEM_UPGRADE_BROKEN )
								{
									// 嬴檜蠱檜 だ惚腎歷戲嘎煎 餉薯ж堅 DB縑 だ惚腎歷棻朝 蘭葬蒂 陳溥遽棻.
									IFollower.BrakeWeapon( uiFollowerID );											// 嬴檜蠱 だ惚

									DBIKUpgradeWeaponBroken		DBInfo;

									DBInfo.siDBAccount = stAccountInfo.siDBAccount;
									DBInfo.uiDBSlot = stAccountInfo.uiSlot;
									DBInfo.uiFollowerID = uiFollowerID;
									DBInfo.uiItemID = uiItemID;

									pMyWorld->clDatabase->writeDBQuery(SO_DB_INFOKIND_UPGRADEWEAPONBROKEN, LPVOID(&DBInfo), sizeof(DBInfo));
									
									bSendDeleteQuery = FALSE;
								}
							}
							break;
						}	// end of case
					}	// end of switch
				}	// end of if(か熱�膩�陛 氈朝 嬴檜蠱檜棻)
			}	// end of if(瞪羹 瞳辨 嬴檜蠱檜 嬴棲棻)
			// 瞪羹瞳辨嬴檜蠱檜賊
			else {
				
				SI32 siHpGain = pItemHeader->m_siHpGain;
				SI32 siMpGain = pItemHeader->m_siMpGain;
				
				SI32 siMaxFollower = ON_MAX_FOLLOWER_NUM;
				
				for (int i = 0; i < siMaxFollower; i++)
				{
					// 襄營ж朝 睡ж檜賊 
					if (IFollower.IsValidFollower(i))
					{					
						// parameterbox蒂 掘п憮
						pParaBox	= &IFollower.GetFollower(i)->ParametaBox;
						if(pParaBox)
						{
							// 睡�偏ぜ旼衈抩ぐ牉� 避擎辨煽擎 纂猿ж雖彊朝棻.
							if ( !(pItemHeader->m_siGroupCode & ON_ITEM_REVIVE) )
							{
								if (pParaBox->IP.GetLife() <= 0) continue;								
							}
							

							// 瞪羹 �蛹� | 100%煎 �蛹�
							if( pItemHeader->m_siGroupCode & ON_ITEM_ALL_RECOVER )
							{
								NewValue = ( (float)pParaBox->GetMaxLife() * ( (float)siHpGain / 100.0) ) + pParaBox->IP.GetLife();
								NewValue = NewValue >= pParaBox->GetMaxLife() ? pParaBox->GetMaxLife() : NewValue;
								pParaBox->IP.SetLife(NewValue);								

								NewValue = ( (float)pParaBox->GetMaxMana() * ( (float)siMpGain / 100.0) ) + pParaBox->IP.GetMana();
								NewValue = NewValue >= pParaBox->GetMaxMana() ? pParaBox->GetMaxMana() : NewValue;
								pParaBox->IP.SetMana(NewValue, 4);
							}
							else
							{
								// 羹溘 葆溘 �蛹�!
								if(siHpGain != 0)
								{
									NewValue = ((pParaBox->GetMaxLife()) < (pParaBox->IP.GetLife() + siHpGain) ? pParaBox->GetMaxLife():(pParaBox->IP.GetLife() + siHpGain));
									pParaBox->IP.SetLife(NewValue);
								}
								if(siMpGain != 0)
								{
									NewValue = ((pParaBox->GetMaxMana()) < (pParaBox->IP.GetMana() + siMpGain) ? pParaBox->GetMaxMana():(pParaBox->IP.GetMana() + siMpGain));					
									pParaBox->IP.SetMana(NewValue, 4);
								}
							}
							
						}
					}
					
					
				}
				
				
			}
		}
	}

	if (bSendDeleteQuery)
	{
		if( DelItemNoQuery(uiFollowerID, uiItemID, uiQuantity, uiPosInventory) == NULL)
		{
			return FALSE;
		}
		else
		{
			DBIKConsumeItem	DBPlayerItemInfo;

			DBPlayerItemInfo.siDBAccount			=	stAccountInfo.siDBAccount;
			DBPlayerItemInfo.uiSlot					=   stAccountInfo.uiSlot;
			DBPlayerItemInfo.uiFollowerID			=	uiFollowerID;
			DBPlayerItemInfo.uiItemID				=	uiItemID;	
			
			pMyWorld->clDatabase->writeDBQuery(SO_DB_INFOKIND_CONSUMEITEM, &DBPlayerItemInfo, sizeof(DBPlayerItemInfo));		
		}
	}
	else
	{
		if( DelItemNoQuery(uiFollowerID, uiItemID, uiQuantity, uiPosInventory) == NULL )
		{
			return FALSE;

		}		
	}
	return TRUE;
}





