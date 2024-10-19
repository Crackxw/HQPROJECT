#include "SOMain.h"
#include "SODefine.h"
#include "SOWorld.h"
#include "SOPlayer.h"
#include "OnlineCommonStruct.h"

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
// 마을에서 떠난다.
//
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
VOID	SOPlayer::RecvLeaveVillage()
{	
	UI16							uiCharFItemInfo[2];			// 0번은 캐릭터 수, 1번은 아이템 수,
	UI16							uiCharUniIDs[ON_STATUS_AREA_XSIZE * ON_STATUS_AREA_YSIZE];	
	OnlineReceiveFieldItemInfo		FieldItemInfo[ON_STATUS_AREA_XSIZE * ON_STATUS_AREA_YSIZE];
	POINT							ptPlayerPos;	
	BOOL							bSuccess = FALSE;
	
	// 현재 마을에 있는가?
	if( stAccountInfo.pVillage == NULL )								goto	END;

	// 우선 맵에서 나올 좌표를 찾는다. 
	// 마을의 위치를 기준으로 맵에 심을 좌표를 얻어온다.
	ptPlayerPos		=	pMyWorld->clISOMap.SearchMap( stAccountInfo.pVillage->uiMapID,	
													stAccountInfo.pVillage->siVillageX, 
													stAccountInfo.pVillage->siVillageY, 
													ON_MAP_SI_CHAR, uiMoveableTileInfo );		

	// 마을에서 나올 곳을 찾는데 실패했는지 검사한다.
	if( ( ptPlayerPos.x == -1 ) && ( ptPlayerPos.y == -1 ) )			goto	END;


	// 나오는 마을의 좌표로 수정한다.
	if( ChangeMap( stAccountInfo.pVillage->uiMapID, SI16( ptPlayerPos.x ), SI16( ptPlayerPos.y ) ) == FALSE )	goto	END;

	
	// 맵에 캐릭터 ID를 설정한다.
	if( SetIDOnMap( 11 ) == FALSE )			
	{
		// 맵에 캐릭터를 설정하는데 실패했다.

//		pMyWorld->clServer->WriteInfo("..\\SetIDOnMap Failed.txt", "8 [ %u ], [ %d ], [ %d ]", uiMapID, siX, siY );
		writeInfoToFile("SetIDOnMap Failed.txt", "8 [ %u ], [ %d ], [ %d ]", uiMapID, siX, siY );
		goto	END;
	}

	// 마을을 빠져 나온다.
	LeaveVillage();
	
	bSuccess	=	TRUE;

END:

	if( bSuccess == TRUE )
	{
		// 마을에서 성공적으로 나갈 수 있다.

		uiCharFItemInfo[0]		=	pMyWorld->clISOMap.GetNearSectorPlayerList(uiMapID, siX, siY, uiCharUniIDs, ON_MAP_CHARID_FIELD);
		uiCharFItemInfo[1]		=	pMyWorld->clISOMap.GetNearSectorItemList(uiMapID, siX, siY, FieldItemInfo);		
		

		// 나에게 내 주위의 캐릭터와 아이템의 정보를 보내준다.
		SendFieldMsg( ON_RESPONSE_LEAVE_VILLAGE, SO_SFM_ONLYME, LPVOID( ON_RET_LEAVE_VILLAGE_OK ), LPVOID( uiCharFItemInfo ), 
						LPVOID( uiCharUniIDs ), LPVOID( FieldItemInfo ) );
	}
	else
	{
		// 마을에서 나가는데 실패했다.

		SendFieldMsg(ON_RESPONSE_LEAVE_VILLAGE, SO_SFM_ONLYME, LPVOID(ON_RET_LEAVE_VILLAGE_NO));

	}


}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	상점에서 아이템의 리스트를 요구한다.
//	수정 일자	:	2002-10-01 오전 9:49:03 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
VOID	SOPlayer::RecvListItem()
{
	SI32					siItemNum;
	OnlineReceivedItemInfo	ItemsInfo[ ON_VILLAGE_MAX_ITEM ];
	
	// 마을에서 판매하는 아이템의 목록을 얻어온다.
	siItemNum	=	stAccountInfo.pVillage->m_pMarket->GetSellItemList( ItemsInfo );			

	SendFieldMsg( ON_RESPONSE_LIST_ITEM, SO_SFM_ONLYME, LPVOID( siItemNum ), LPVOID( ItemsInfo ) );
}

//----------------------------------------------------------------------------------------------------------------
//	설명		:	아이템을 산다.
//	수정 일자	:	2002-02-27 오후 2:03:04 - 양정모
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
//	설명		:	아이템을 판다.
//	수정 일자	:	2002-03-02 오전 11:02:18 - 양정모
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

	// 나에게 정말 이 아이템들이 요구한 수량만큼 있는지 검사한다.	
	pPlayerItem		=	IFollower.GetItem(pReqSellItemMsg->uiFollowerID, pReqSellItemMsg->uiItemID, pReqSellItemMsg->uiPosInInventory);

	// 그 용병에게는 그 인벤토리 위치에 그 아이템이 없다.
	if(pPlayerItem == NULL)		goto	END;

	// 팔려고 하는 아이템 보다 가지고 있는 아이템이 많거나 같은지 검사한다.
	if(pPlayerItem->uiQuantity < pReqSellItemMsg->uiItemQuantity)	goto	END;

	// 한번에 팔 수 있는 수량을 제한한다.
	if( pReqSellItemMsg->uiItemQuantity > ON_MARKET_MAXQUANTITY )	goto	END;


	// 팔려는 아이템이 이 시장에서 취급하는 물품인지 검사한다.	
	if( stAccountInfo.pVillage->m_pMarket->FindItem( pReqSellItemMsg->uiItemID ) != NULL )
	{
		// 취급하는 물품이다.

		// 플레이어와 이 마을의 상단간의 관계를 설정한다.			

		// 길드 멤버인지 알아온다.
		if((stAccountInfo.pVillage->uiGuildID != 0) && (uiGuildID != 0) && (siClassInGuild != pMyWorld->clIGuildClass.GetJWJCode()))
		{
			if(stAccountInfo.pVillage->uiGuildID == uiGuildID)							
				siGuildRelation		=	ON_GUILD_RELATION_MINE;		// 이 마을의 상단원이다.
			else if(pMyWorld->clIGuild.IsHostilityRelation(uiGuildID, stAccountInfo.pVillage->uiGuildID) == TRUE)	
				siGuildRelation		=	ON_GUILD_RELATION_ENEMY;	// 적대적인 마을의 상단이다.
		}

		// 아이템의 가격을 제대로 받을 수 있다.		
		stAccountInfo.pVillage->m_pMarket->GetSellItemPrice( pReqSellItemMsg->uiItemID, siGuildRelation, &uiRealItemsPrice );
		uiRealItemsPrice	*=	pReqSellItemMsg->uiItemQuantity;

		// 서버의 아이템을 증가시켜준다.	
		stAccountInfo.pVillage->m_pMarket->IncreaseItemQuantity( pReqSellItemMsg->uiItemID, pReqSellItemMsg->uiItemQuantity );
	}	
	else
	{
		// 취급하지 않는 물품이다.

		// 아이템의 가격을 제대로 받지 못한다.
		uiRealItemsPrice	=	pMyWorld->clItem.GetItem(pReqSellItemMsg->uiItemID)->m_siDBPrice  * pReqSellItemMsg->uiItemQuantity / 3;

		// 서버의 아이템을 증가시켜주지 않는다.(아무일도 하지 않는다.)
	}

	
	// 아이템을 없앤다.
	if(DelItem(pReqSellItemMsg->uiFollowerID, pReqSellItemMsg->uiItemID, pReqSellItemMsg->uiItemQuantity, pReqSellItemMsg->uiPosInInventory) == NULL)
		goto	END;


	// 나의 돈을 증가시켜준다.
	IncreaseMoney(uiRealItemsPrice);
	// 마을의 무역 자본을 늘린다.
	stAccountInfo.pVillage->IncreaseCapital(MONEY(uiRealItemsPrice*ON_CAPITAL_RATE_TRADE),ON_PROFIT_TRADE);

	// 무역 신용도를 증가시켜준다.
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
//	설명	:	아이템을 추가할때 무게 제한에 걸리는지 확인한다.
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
//	설명	:	무게초과인 용병이 하나라도 존재하여 걷기 힘든 상태이면 TRUE를 리턴한다.
//----------------------------------------------------------------------------------------------------------------
BOOL SOPlayer::IsOverWeight()
{
	return IFollower.IsOverWeight();
}


//----------------------------------------------------------------------------------------------------------------
//	설명	:	은행의 정보를 얻어온다.
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
			// 현재 은행에서 플레이어의 잔액을 얻어온다.
//			pMyWorld->clDatabase->GetDBInfo(SO_DB_INFOKIND_BANKBALANCE, LPVOID(&DBBankInfo), LPVOID(&DBBankInfo));
//			siBalance	=	DBBankInfo.siBankCapital;

			// 이율, 은행 잔액, 내 돈을 보내준다.

			SendFieldMsg(ON_RESPONSE_BANK_INFO, SO_SFM_ONLYME, LPVOID(stAccountInfo.pVillage->pstBankData));
						//LPVOID(siBalance),
						//LPVOID(uiMoney));
		}	
		*/
	}
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	은행에 예금을 한다.
//	수정 일자	:	2002-10-18 오후 4:27:42 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
VOID SOPlayer::RecvBankDeposit()
{
	OnReqBankDepositMsg		*pReqBankDepositMsg;
	BOOL					bSuccess  = FALSE;

	pReqBankDepositMsg	=	(OnReqBankDepositMsg*)cMsg;
	
	// 현재 마을 안에 있는지 검사한다.
	if( IsInVillage() == FALSE )																				goto	END;	

	// 입금 하려는 돈이 유효한지 검사한다.
	if( ( pReqBankDepositMsg->siMoney <= 0 ) || ( EnoughMoney( pReqBankDepositMsg->siMoney ) == FALSE ) )		goto	END;

	bSuccess	=	stAccountInfo.pVillage->BankDeposit( this, pReqBankDepositMsg );

END:

	if( bSuccess == FALSE )
	{
		SendFieldMsg( ON_RESPONSE_BANK_DEPOSIT, SO_SFM_ONLYME, LPVOID( ON_RET_BANK_NO ) );	
	}

	/*
	// 예금 하려는 금액이 현재 가지고 있는 돈보다 같거나 작아야 한다.
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
//	설명	:	은행에서 출금을 한다.
//----------------------------------------------------------------------------------------------------------------
VOID SOPlayer::RecvBankDrawing()
{
	OnReqBankDrawingMsg		*pReqBankDrawingMsg;	
	BOOL					bSuccess  = FALSE;
	
	pReqBankDrawingMsg	=	(OnReqBankDrawingMsg*)cMsg;

	// 현재 마을 안에 있는지 검사한다.
	if( IsInVillage() == FALSE )										goto	END;	

	// 출금 하려는 돈이 유효한지 검사한다.
	if( pReqBankDrawingMsg->siMoney <= 0 )								goto	END;
	
	// 해당 플레이어가 소지할 수 있는 금액이 초과되는지 검사한다.
	if( GetMaxMoneyToGet() < pReqBankDrawingMsg->siMoney )				goto	END;

	bSuccess	=	stAccountInfo.pVillage->BankDrawing( this, pReqBankDrawingMsg );

END:
	
	if( bSuccess == FALSE )
	{
		SendFieldMsg( ON_RESPONSE_BANK_DRAWING, SO_SFM_ONLYME, LPVOID( ON_RET_BANK_NO ) );
	}

	/*
	//DB에서 현재 내돈을얻어온다.	
	pReqBankDrawingMsg	=	(OnReqBankDrawingMsg*)cMsg;
	
	stAccountInfo.pVillage->BankDrawing(this,pReqBankDrawingMsg);

*/

}

/*
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명	:	주점 게시판에 들어간다는 메시지를 받았다.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
VOID	SOPlayer::RecvEnterNoticeBoard()
{
	if(stAccountInfo.pVillage != NULL)
	{
		// 들어 갔을 경우의 메시지는 Village 클래스에서 보내준다.		
		stAccountInfo.pVillage->EnterNoticeBoard(this);				
	}
	else
	{		
		// 게시판에 들어갈 수가 없다.
		SendFieldMsg(ON_RESPONSE_ENTER_NOTICEBOARD, SO_SFM_ONLYME, LPVOID(ON_RET_INN_NO));
	}
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명	:	주점에 들어간다는 메시지를 받았다.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
VOID	SOPlayer::RecvEnterInn()
{
	// 주점에는 무조건 들어간다.
	SendFieldMsg(ON_RESPONSE_ENTER_INN, SO_SFM_ONLYME);

	// 주점에 들어갔다고 설정한다.
	//stAccountInfo.uiStatusInVillage			=	INVILLAGE_STATUS_INN;
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명	:	주점에서 나간다는 메시지를 받았다.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
VOID	SOPlayer::RecvLeaveInn()
{
	BOOL					bSuccess = FALSE;	

	if(stAccountInfo.pVillage != NULL)
	{
		// 나갔을 경우의 메시지는 Village 클래스에서 보내준다.
		bSuccess	=	stAccountInfo.pVillage->LeaveInn(this);

		if(bSuccess == TRUE)
		{
			stAccountInfo.uiStatusInVillage	=	INVILLAGE_STATUS_NONE;			
		}
	}	

	if(bSuccess == FALSE)
	{
		// 대화방에서 나갈 수 없다.
		SendFieldMsg(ON_RESPONSE_LEAVE_INN, SO_SFM_ONLYME, LPVOID(ON_RET_INN_NO));
	}
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명	:	주점 채팅 대기실에 들어간다는 메시지를 받았다.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
VOID	SOPlayer::RecvEnterWaitRoom()
{
	BOOL					bSuccess = FALSE;	

	if(stAccountInfo.pVillage != NULL)	
	{
		// 들어 갔을 경우의 메시지는 Village 클래스에서 보내준다.		
		bSuccess	=	stAccountInfo.pVillage->EnterWaitRoom(this);				
	}	

	if(bSuccess == FALSE)
	{
		// 대화방에 들어갈 수 없다.
		SendFieldMsg(ON_RESPONSE_ENTER_VWR, SO_SFM_ONLYME,	LPVOID(ON_RET_INN_NO));
	}
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명	:	주점에서 대화방을 만든다는 메시지를 받았다.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
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

		// 대화방을 만든다.		
		// 성공적으로 대화방을 만들었다면 그 대화방에 스스로 들어간다.
		// 관련 메시지는 Village 클래스에서 보내준다.
		bSuccess	=	stAccountInfo.pVillage->CreateChatRoomInInn(this, szChatRoomName);				
	}

	if(bSuccess == FALSE)
	{
		// 채팅방을 만들수 없다.
		SendFieldMsg(ON_RESPONSE_INN_CREATECHATROOM, SO_SFM_ONLYME,
						LPVOID(ON_RET_INN_NO));
	}
}

//----------------------------------------------------------------------------------------------------------------
//	설명	:	주점 대화방에 들어간다는 메시지를 받았다.
//----------------------------------------------------------------------------------------------------------------
VOID	SOPlayer::RecvJoinInnChatRoom()
{
	BOOL						bSuccess = FALSE;
	OnReqJoinVilInnChatRoomMsg	*pJoinVilInnChatRoomMsg;

	pJoinVilInnChatRoomMsg	=	(OnReqJoinVilInnChatRoomMsg*)cMsg;


	if(stAccountInfo.pVillage != NULL)	
	{
		// 들어 갔을 경우의 메시지는 Village 클래스에서 보내준다.		
		bSuccess	=	stAccountInfo.pVillage->JoinChatRoomInInn(pJoinVilInnChatRoomMsg->uiChatRoomID, this);				
	}	

	if(bSuccess == FALSE)
	{
		// 대화방에 들어갈 수 없다.
		SendFieldMsg(ON_RESPONSE_VIL_INN_JOIN_CHAT_ROOM, SO_SFM_ONLYME,
						LPVOID(ON_RET_INN_NO));
	}
}

//----------------------------------------------------------------------------------------------------------------
//	설명	:	플레이어가 주점 채팅방에서 나갔다.
//----------------------------------------------------------------------------------------------------------------
VOID	SOPlayer::RecvLeaveInnChatRoom()
{
	BOOL	bSuccess = FALSE;
	if(stAccountInfo.pVillage != NULL)
	{
		// 나갔을 경우의 메시지는 Village 클래스에서 보내준다.
		bSuccess	=	stAccountInfo.pVillage->LeaveChatRoomInInn(this);
	}	

	if(bSuccess == FALSE)
	{
		// 대화방에서 나갈 수 없다.
		SendFieldMsg(ON_RESPONSE_VIL_INN_LEAVE_CHAT_ROOM, SO_SFM_ONLYME,
								LPVOID(ON_RET_INN_NO));
	}
}
*/

//----------------------------------------------------------------------------------------------------------------
//	설명	:	마을 시장에 들어간다.
//----------------------------------------------------------------------------------------------------------------
VOID	SOPlayer::RecvEnterMarket()
{
	BOOL	bSuccess = FALSE;

	if(stAccountInfo.pVillage != NULL)	
	{
		// 들어 갔을 경우의 메시지는 Village 클래스에서 보내준다.		
		bSuccess	=	stAccountInfo.pVillage->m_pMarket->EnterPlayer( this );				

		if(bSuccess == TRUE)
		{
			// 시장에 들어간 상태이다.
			stAccountInfo.uiStatusInVillage	=	INVILLAGE_STATUS_MARKET;

			SendFieldMsg( ON_RESPONSE_ENTER_MARKET, SO_SFM_ONLYME, LPVOID( ON_RET_MARKET_OK ) );
		}
	}	

	if(bSuccess == FALSE)
	{
		// 대화방에 들어갈 수 없다.
		SendFieldMsg(ON_RESPONSE_ENTER_MARKET, SO_SFM_ONLYME, LPVOID(ON_RET_MARKET_NO));
	}
}

//----------------------------------------------------------------------------------------------------------------
//	설명	:	마을 시장에서 나간다.
//----------------------------------------------------------------------------------------------------------------
VOID	SOPlayer::RecvLeaveMarket()
{
	BOOL	bSuccess = FALSE;	

	if(stAccountInfo.pVillage != NULL)
	{
		// 나갔을 경우의 메시지는 Village 클래스에서 보내준다.
		bSuccess	=	stAccountInfo.pVillage->m_pMarket->LeavePlayer( this );

		if(bSuccess == TRUE)
		{
			stAccountInfo.uiStatusInVillage	=	INVILLAGE_STATUS_NONE;			

			SendFieldMsg(ON_RESPONSE_LEAVE_MARKET, SO_SFM_ONLYME, LPVOID(ON_RET_MARKET_OK));
		}
	}	

	if(bSuccess == FALSE)
	{
		// 대화방에서 나갈 수 없다.
		SendFieldMsg(ON_RESPONSE_LEAVE_MARKET, SO_SFM_ONLYME, LPVOID(ON_RET_INN_NO));
	}
}



//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	새로운 계정(통장)을 만든다는 메시지를 받았다.
//	수정 일자	:	2002-10-18 오후 3:46:18 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
VOID	SOPlayer::RecvOpenSavingAccount()
{	
	OnReqBankOpenSavingAccountMsg	*pOpenSavingAccountMsg;	
	DWORD							dwResponse = ON_RET_BANK_NO;

	pOpenSavingAccountMsg	=	(OnReqBankOpenSavingAccountMsg*)cMsg;

	// 현재 마을 안에 있는지 검사한다.
	if( IsInVillage() == FALSE )																											goto	END;

	// 예외처리.....( 아직 지원하지 않는 부분이다. )
	if( pOpenSavingAccountMsg->uiKind == 4 )																								goto	END;

	// 신용 대출이라면 대출 한도 확인..
	if( pOpenSavingAccountMsg->uiKind == ON_BANK_LOAN_TYPE ) 
	{	
		// 대출할 수 있는 한도액을 검사한다.
		if( pOpenSavingAccountMsg->siFirstDepositMoney > uiMaxLoan )									 									goto	END;				
		if( pMyWorld->clDatabase->GetDBInfo( SO_DB_INFOKIND_CHECKLOAN, LPVOID( this ), NULL ) == FALSE)										goto	END;		
		if( pOpenSavingAccountMsg->siFirstDepositMoney > stPropertyData.siTotalProperty )													goto	END;
	}

	// 처음 입금 하려는 돈이 유효한지 검사한다.
	if( ( pOpenSavingAccountMsg->siFirstDepositMoney <= 0 ) || ( EnoughMoney( pOpenSavingAccountMsg->siFirstDepositMoney ) == FALSE ) )		goto	END;
		
	// 최대 만들 수 있는 통장의 개수를 초과했는지 검사한다.
	if( GetBankBootNum() >= ON_MAX_BANKBOOK_NUM )																							goto	END;

	dwResponse	=	stAccountInfo.pVillage->OpenSavingAccount( this, pOpenSavingAccountMsg, NULL );

END:

	if( dwResponse != ON_RET_OK )
	{
		// 계좌를 만드는데 실패했다.
		SendFieldMsg( ON_RESPONSE_BANK_OPENSAVINGACCOUNT, SO_SFM_ONLYME, LPVOID( dwResponse ) );
		
	}


	/*
	OnReqBankOpenSavingAccountMsg*	pOpenSavingAccountMsg;	
	SI32							siRet	=	ON_RET_BANK_OK;

	 pOpenSavingAccountMsg	=	(OnReqBankOpenSavingAccountMsg*)cMsg;


	 if( pOpenSavingAccountMsg->uiKind  == 3 ) // 신용 대출이라면 대출 한도 확인..
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
				 // 계좌의 신청은 마을의 은행에서 수행한다.
				 // 은행계좌를 성공적으로 만들경우 은행에서 직접 사용자에게 메시지를 보내준다.
				 siRet	=	stAccountInfo.pVillage->OpenSavingAccount(this, pOpenSavingAccountMsg,NULL);
			 }
		 }
	 }

	if(siRet != ON_RET_BANK_OK)
	{		
		// 은행계좌를 만들 수 없다.
		SendFieldMsg(ON_RESPONSE_BANK_OPENSAVINGACCOUNT, SO_SFM_ONLYME, LPVOID(siRet));
	}
	*/
}


//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명	:	통장 리스트를 달라는 요구를 받았다.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
VOID	SOPlayer::RecvBankbookList()
{
	DBIKBankBookList			BankBookList;
	DBIKVillageBankBookInfo		VillageBankBookInfo;
	BOOL						bGetBankBookListSuccess = TRUE;

	// 우선 그 캐릭터가 마을에 어느 마을에 들어가 있어야 한다.
	if(stAccountInfo.pVillage != NULL)
	{
		// DB에서 통장의 정보를 얻어온다.	
		VillageBankBookInfo.pPlayer			=	this;
		VillageBankBookInfo.uiVillageCode	=	stAccountInfo.pVillage->uiVillageCode;
		VillageBankBookInfo.uiVillageCode	=	stAccountInfo.pVillage->uiVillageCode;

		memset(&BankBookList, 0, sizeof(BankBookList));
		if(pMyWorld->clDatabase->GetDBInfo(SO_DB_INFOKIND_MYBANKBOOK, LPVOID(&VillageBankBookInfo), LPVOID(&BankBookList)) == TRUE)
		{
			// 통장의 정보를 성공적으로 얻어왔다.			
			SendFieldMsg(ON_RESPONSE_BANK_BANKBOOK_LIST, SO_SFM_ONLYME, LPVOID(ON_RET_BANK_OK), LPVOID(&BankBookList));
			
		}
		else
			bGetBankBookListSuccess	=	FALSE;
	}
	else
		bGetBankBookListSuccess	=	FALSE;

	if(bGetBankBookListSuccess == FALSE)
	{
		// 통장의 정보를 얻어오는데 실패했다.
		SendFieldMsg(ON_RESPONSE_BANK_BANKBOOK_LIST, SO_SFM_ONLYME, LPVOID(ON_RET_BANK_NO));
	}
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명	:	통장 리스트의 수를 달라는 요구를 받았다.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
VOID	SOPlayer::RecvBankbookListCount()
{
	MONEY		Temp;

	Temp = m_MoneySaving;
	SendFieldMsg(ON_RESPONSE_BANK_BANKBOOK_LISTCOUNT, SO_SFM_ONLYME, LPVOID(ON_RET_OK), LPVOID(&Temp));
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명	:	담보를 맡긴다는 요구를 받았다.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
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

	// 담보 통장에 관한 정보를 얻어온다.
	pEconomyHeader = pMyWorld->clOnlineEconomy.GetHeader(3);

	if(stAccountInfo.pVillage != NULL && pEconomyHeader != NULL)
	{
		stVillageCollateral.pPlayer			=	this;
		stVillageCollateral.uiVillageCode	=	stAccountInfo.pVillage->uiVillageCode;
		stVillageCollateral.uiVillageCode	=	stAccountInfo.pVillage->uiVillageCode;
		stVillageCollateral.siMoney			=	pCollateralLoan->siMoney;
		stVillageCollateral.uiMonth			=	pCollateralLoan->uiMonth;
		stVillageCollateral.pCollateralList	=	&stCollateralList;

		// 대출 기한 한도가 넘었는지 확인한다.
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
			
			// 대출 액수 한도가 넘었는지 확인한다.
			for(int i=0	;	i < (SI32)pCollateralLoan->dwItemNum ; i++)
			{
				if( stCollateralList.uiNum[i] > 1000 )
				{
					siRet = ON_RET_BANK_NO;
					break;
				}
				MaxLoan		+=	(pMyWorld->clItem.GetItem(stCollateralList.uiITemCode[i])->m_siDBPrice) * ( stCollateralList.uiNum[i] ) * 7 / 10;
			}			
			
			if(stVillageCollateral.siMoney > MaxLoan )   //1000 대신 대출한도가 와야 한다.
			{				
				siRet = ON_RET_BANK_NO;
			}
		}
		//이제 db에 저장한다. DB에서 대출 통장이 있는지도 확인 

		if(siRet == ON_RET_BANK_OK && stAccountInfo.pVillage != NULL)
		{	
			// DB에 담보에 대한 기본 정보를 넣고 확인한다.
			pMyWorld->clDatabase->GetDBInfo(SO_DB_INFOKIND_COLLATERAL_1, LPVOID(&stVillageCollateral), LPVOID(&siRet));				
			if(siRet == 1)
			{
				// 4번 통장(담보 통장)을 만들고 아이템을 제거한다.				
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
					SendFieldMsg(ON_RESPONSE_BANK_COLLATERAL, SO_SFM_ONLYME, LPVOID(siRet),LPVOID(&stAccountNum));  // pParam2는 통장 번호.
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


//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명	:	담보 리스트를 달라는 요구를 받았다.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
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
			// 통장의 정보를 성공적으로 얻어왔다.			
			SendFieldMsg(ON_RESPONSE_BANK_COLLATERALLIST, SO_SFM_ONLYME, LPVOID(ON_RET_BANK_OK), LPVOID(&stCollateralList));
		}
		else
			bGetCollateralListSuccess	=	FALSE;
	}
	else
		bGetCollateralListSuccess	=	FALSE;

	if(bGetCollateralListSuccess == FALSE)
	{		
		// 담보 정보를 얻어오는데 실패했다.
		SendFieldMsg(ON_RESPONSE_BANK_COLLATERALLIST, SO_SFM_ONLYME, LPVOID(ON_RET_BANK_NO));
	}
}


//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명	:	담보를 돌려 달라는 요구를 받았다.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
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
		// 통장의 정보를 성공적으로 얻어왔다.			
		SendFieldMsg(ON_RESPONSE_BANK_TAKECOLLATERAL, SO_SFM_ONLYME, LPVOID(ON_RET_BANK_OK));		
	}
	if(siResult == ON_RET_BANK_NO)
	{
		
		SendFieldMsg(ON_RESPONSE_BANK_TAKECOLLATERAL, SO_SFM_ONLYME, LPVOID(ON_RET_BANK_NO));
	}
}

/*
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명	:	게시물을 등록한다.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
VOID	SOPlayer::RecvRegistNotice()
{
	BOOL					bSuccessRegistNotice  = FALSE;
	CHAR					szTitle[4096], szNotice[10240];
	CHAR					*pszTitle;
	CHAR					*pszNotice;
	OnReqRegistNoticeMsg	*pRegistNoticeMsg;
	
	// 글을 등록하는데 실패했다고 알려준다.
	SendFieldMsg(ON_RESPONSE_REGIST_NOTICE, SO_SFM_ONLYME, LPVOID(ON_RET_INN_NO));
	return;

	pRegistNoticeMsg	=	(OnReqRegistNoticeMsg*)cMsg;

	if(stAccountInfo.pVillage != NULL)
	{
		pszTitle		=	(CHAR*)pRegistNoticeMsg + sizeof(OnReqRegistNoticeMsg);
		pszNotice		=	(CHAR*)pszTitle + pRegistNoticeMsg->uiTitleStringLength;

		// Title을 복사.
		strncpy(szTitle, pszTitle, pRegistNoticeMsg->uiTitleStringLength);
		szTitle[pRegistNoticeMsg->uiTitleStringLength]		=	NULL;

		// Notice를 복사.
		strncpy(szNotice, pszNotice, pRegistNoticeMsg->uiNoticeStringLength);
		szNotice[pRegistNoticeMsg->uiNoticeStringLength]	=	NULL;

		// 쓸 수 없는 글자가 있는 지 확인한다.
		strtok(szTitle, ON_FORBIDDEN_CHAR);
		strtok(szNotice, ON_FORBIDDEN_CHAR);
		if(szTitle[0]	==	'\'')	szTitle[0]	=	20;
		if(szNotice[0]	==	'\'')	szNotice[0]	=	20;
		
		bSuccessRegistNotice	=	stAccountInfo.pVillage->RegistNotice(this, szTitle, szNotice);
	}
	
	if(bSuccessRegistNotice == FALSE)
	{
		// 글을 등록하는데 실패했다고 알려준다.
		SendFieldMsg(ON_RESPONSE_REGIST_NOTICE, SO_SFM_ONLYME, LPVOID(ON_RET_INN_NO));
	}

}


//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명	:	게시판의 글을 삭제한다.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
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
		// 글을 등록하는데 실패했다고 알려준다.
		SendFieldMsg(ON_RESPONSE_DELETE_NOTICE, SO_SFM_ONLYME, LPVOID(ON_RET_INN_NO));
	}
}
		

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명	:	게시물의 리스트를 얻어온다.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
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
		// 글의 리스트를 얻어오는데 실패했다.
		SendFieldMsg(ON_RESPONSE_NOTICEBOARD_NOTICELIST, SO_SFM_ONLYME, LPVOID(ON_RET_INN_NO));
	}
}



//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명	:	게시물을 본다.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
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
		// 글을 얻어오는데 실패했다.
		SendFieldMsg(ON_RESPONSE_VIEW_NOTICE, SO_SFM_ONLYME, LPVOID(ON_RET_INN_NO));
	}
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	게시물을 수정한다.
//	수정 일자	:	2002-04-15 오후 2:30:17 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
VOID	SOPlayer::RecvModifyNotice()
{
	BOOL					bSuccess	=	FALSE;
	OnReqModifyNoticeMsg	*pModifyNoticeMsg;
	CHAR					*pszNoticeTitleString;
	CHAR					*pszNoticeContentString;
	CHAR					szNoticeTitle[256];
	CHAR					szNoticeContent[4096];

	// 글을 등록하는데 실패했다고 알려준다.
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
		// 글을 얻어오는데 실패했다.
		SendFieldMsg(ON_RESPONSE_MODIFY_NOTICE, SO_SFM_ONLYME, LPVOID(ON_RET_INN_NO));
	}

}	
*/




//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명	:	용병 List를 요청한다.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
VOID	SOPlayer::RecvBarrackGetSolderList()
{
	if(stAccountInfo.pVillage != NULL)
	{

		if( stAccountInfo.pVillage->m_Barrack != NULL )
		{

			// 들어 갔을 경우의 메시지는 Village 클래스에서 보내준다.		
			stAccountInfo.pVillage->m_Barrack->BarrackGetSolderList(this);				

		}
		else
		{
			// 용병 고용란에 List를 줄수 없다.
			SendFieldMsg(ON_RESPONSE_BARRACK_GETSOLDERLIST, SO_SFM_ONLYME, LPVOID(ON_RET_NO));
		}

	}
	else
	{		
		// 용병 고용란에 List를 줄수 없다.
		SendFieldMsg(ON_RESPONSE_BARRACK_GETSOLDERLIST, SO_SFM_ONLYME, LPVOID(ON_RET_NO));
	}
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명	:	용병을 고용한다.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
VOID	SOPlayer::RecvBarrackBuySolder()
{
	OnReqBarrackBuySolder		*pOnReqBarrackBuySolder;

	pOnReqBarrackBuySolder	=	(OnReqBarrackBuySolder*)cMsg;

	if(stAccountInfo.pVillage != NULL)
	{
		if( stAccountInfo.pVillage->m_Barrack != NULL )
		{

			// 용병 고용에 대한 정보는 Village 클래스에서 보내준다.
			if(stAccountInfo.pVillage->m_Barrack->BarrackBuySolder(this, pOnReqBarrackBuySolder->dwBuySolderID, (BYTE*)pOnReqBarrackBuySolder->szName) == FALSE)
			{
				// 실패시 Message 만 전송해 준다. 성공했을때는 내부에서 Message를 보낸다.
				SendFieldMsg(ON_RESPONSE_BARRACK_BUYSOLDER, SO_SFM_ONLYME, LPVOID(ON_RET_NO));
			}
		}
		else
		{
			// 용병 고용란에서 용병을 고용할수 없다.
			SendFieldMsg(ON_RESPONSE_BARRACK_BUYSOLDER, SO_SFM_ONLYME, LPVOID(ON_RET_NO));
		}

	}
	else
	{		
		// 용병 고용란에서 용병을 고용할수 없다.
		SendFieldMsg(ON_RESPONSE_BARRACK_BUYSOLDER, SO_SFM_ONLYME, LPVOID(ON_RET_NO));
	}
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명	:	해고할 용병 List를 요청한다.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
VOID	SOPlayer::RecvBarrackGetDismissalSolderList()
{
	if(stAccountInfo.pVillage != NULL)
	{
		if( stAccountInfo.pVillage->m_Barrack != NULL )
		{

			// 들어 갔을 경우의 메시지는 Village 클래스에서 보내준다.		
			stAccountInfo.pVillage->m_Barrack->BarrackGetDismissalSolderList(this);				

		}
		else
		{

			// 용병 고용란에 List를 줄수 없다.
			SendFieldMsg(ON_RESPONSE_BARRACK_GETDISMISSALSOLDERLIST, SO_SFM_ONLYME, LPVOID(ON_RET_NO));

		}
	}
	else
	{		
		// 용병 고용란에 List를 줄수 없다.
		SendFieldMsg(ON_RESPONSE_BARRACK_GETDISMISSALSOLDERLIST, SO_SFM_ONLYME, LPVOID(ON_RET_NO));
	}
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명	:	용병을 해고한다.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
VOID	SOPlayer::RecvBarrackDismissalSolder()
{
	OnReqBarrackDismissalSolder		*pOnReqBarrackDismissalSolder;

	pOnReqBarrackDismissalSolder	=	(OnReqBarrackDismissalSolder*)cMsg;

	if(stAccountInfo.pVillage != NULL)
	{
		if( stAccountInfo.pVillage->m_Barrack != NULL )
		{

			// 용병 해고에 대한 정보는 Village 클래스에서 보내준다.
			if(stAccountInfo.pVillage->m_Barrack->BarrackDismissalSolder(this, pOnReqBarrackDismissalSolder->dwSlot) == FALSE)
			{
				// 실패시 Message 만 전송해 준다. 성공했을때는 내부에서 Message를 보낸다.
				SendFieldMsg(ON_RESPONSE_BARRACK_DISMISSALSOLDER, SO_SFM_ONLYME, LPVOID(ON_RET_NO));
			}
		}
		else
		{
			// 용병 고용란에서 용병을 해고할수 없다.
			SendFieldMsg(ON_RESPONSE_BARRACK_DISMISSALSOLDER, SO_SFM_ONLYME, LPVOID(ON_RET_NO));
		}
		
	}
	else
	{		
		// 용병 고용란에서 용병을 해고할수 없다.
		SendFieldMsg(ON_RESPONSE_BARRACK_DISMISSALSOLDER, SO_SFM_ONLYME, LPVOID(ON_RET_NO));
	}
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명	:	장수용병 List를 요청한다.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
VOID	SOPlayer::RecvGBarrackGetSolderList()
{
	if(stAccountInfo.pVillage != NULL)
	{
		if( stAccountInfo.pVillage->m_Barrack != NULL )
		{

			// 들어 갔을 경우의 메시지는 Village 클래스에서 보내준다.		
			stAccountInfo.pVillage->m_Barrack->GBarrackGetSolderList(this);				
		}
		else
		{
			// 장수용병 고용란에 List를 줄수 없다.
			SendFieldMsg(ON_RESPONSE_GBARRACK_GETSOLDERLIST, SO_SFM_ONLYME, LPVOID(ON_RET_NO));
		}
	}
	else
	{		
		// 장수용병 고용란에 List를 줄수 없다.
		SendFieldMsg(ON_RESPONSE_GBARRACK_GETSOLDERLIST, SO_SFM_ONLYME, LPVOID(ON_RET_NO));
	}
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명	:	장수용병을 고용한다.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
VOID	SOPlayer::RecvGBarrackBuySolder()
{
	OnReqGBarrackBuySolder		*pOnReqGBarrackBuySolder;

	pOnReqGBarrackBuySolder	=	(OnReqGBarrackBuySolder*)cMsg;

	if(stAccountInfo.pVillage != NULL)
	{
		if( stAccountInfo.pVillage->m_Barrack != NULL )
		{

			// 장수용병 고용에 대한 정보는 Village 클래스에서 보내준다.
			if(stAccountInfo.pVillage->m_Barrack->GBarrackBuySolder(this, pOnReqGBarrackBuySolder->dwBuySolderID, (BYTE*)pOnReqGBarrackBuySolder->szName) == FALSE)
			{
				// 실패시 Message 만 전송해 준다. 성공했을때는 내부에서 Message를 보낸다.
				SendFieldMsg(ON_RESPONSE_GBARRACK_BUYSOLDER, SO_SFM_ONLYME, LPVOID(ON_RET_NO));
			}

		}
		else
		{
			// 장수용병 고용란에서 용병을 고용할수 없다.
			SendFieldMsg(ON_RESPONSE_GBARRACK_BUYSOLDER, SO_SFM_ONLYME, LPVOID(ON_RET_NO));
		}
	}
	else
	{		
		// 장수용병 고용란에서 용병을 고용할수 없다.
		SendFieldMsg(ON_RESPONSE_GBARRACK_BUYSOLDER, SO_SFM_ONLYME, LPVOID(ON_RET_NO));
	}
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명	:	전직할 장수용병 List를 요청한다.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
VOID	SOPlayer::RecvGBarrackGetChangeJobSolderList()
{
	if(stAccountInfo.pVillage != NULL)
	{
		if( stAccountInfo.pVillage->m_Barrack != NULL )
		{

			// 들어 갔을 경우의 메시지는 Village 클래스에서 보내준다.		
			stAccountInfo.pVillage->m_Barrack->GBarrackGetChangeJobSolderList(this);				
		}
		else
		{	
			// 장수용병 전직란에 List를 줄수 없다.
			SendFieldMsg(ON_RESPONSE_GBARRACK_GETCHANGEJOBSOLDERLIST, SO_SFM_ONLYME, LPVOID(ON_RET_NO));
		}
	}
	else
	{		
		// 장수용병 전직란에 List를 줄수 없다.
		SendFieldMsg(ON_RESPONSE_GBARRACK_GETCHANGEJOBSOLDERLIST, SO_SFM_ONLYME, LPVOID(ON_RET_NO));
	}
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명	:	장수용병을 전직한다.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
VOID	SOPlayer::RecvGBarrackChangeJobSolder()
{
	OnReqGBarrackChangeJobSolder		*pOnReqGBarrackChangeJobSolder;

	pOnReqGBarrackChangeJobSolder	=	(OnReqGBarrackChangeJobSolder*)cMsg;

	if(stAccountInfo.pVillage != NULL)
	{
		if( stAccountInfo.pVillage->m_Barrack != NULL )
		{

			// 장수용병 전직에 대한 정보는 Village 클래스에서 보내준다.
			if(stAccountInfo.pVillage->m_Barrack->GBarrackChangeJobSolder(this, pOnReqGBarrackChangeJobSolder->dwChangeJobSlot, pOnReqGBarrackChangeJobSolder->dwChangeJobKind) == FALSE)
			{
				// 실패시 Message 만 전송해 준다. 성공했을때는 내부에서 Message를 보낸다.
				SendFieldMsg(ON_RESPONSE_GBARRACK_CHANGEJOBSOLDER, SO_SFM_ONLYME, LPVOID(ON_RET_NO));
			}
		}
		else
		{
			// 장수용병 고용란에서 용병을 전직할수 없다.
			SendFieldMsg(ON_RESPONSE_GBARRACK_CHANGEJOBSOLDER, SO_SFM_ONLYME, LPVOID(ON_RET_NO));
		}
	}
	else
	{		
		// 장수용병 고용란에서 용병을 전직할수 없다.
		SendFieldMsg(ON_RESPONSE_GBARRACK_CHANGEJOBSOLDER, SO_SFM_ONLYME, LPVOID(ON_RET_NO));
	}
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명	:	해고할 장수용병 List를 요청한다.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
VOID	SOPlayer::RecvGBarrackGetDismissalSolderList()
{
	if(stAccountInfo.pVillage != NULL)
	{
		if( stAccountInfo.pVillage->m_Barrack != NULL )
		{

			// 들어 갔을 경우의 메시지는 Village 클래스에서 보내준다.		
			stAccountInfo.pVillage->m_Barrack->GBarrackGetDismissalSolderList(this);				
		}
		else
		{
			// 장수용병 고용란에 List를 줄수 없다.
			SendFieldMsg(ON_RESPONSE_GBARRACK_GETDISMISSALSOLDERLIST, SO_SFM_ONLYME, LPVOID(ON_RET_NO));
		}
	}
	else
	{		
		// 장수용병 고용란에 List를 줄수 없다.
		SendFieldMsg(ON_RESPONSE_GBARRACK_GETDISMISSALSOLDERLIST, SO_SFM_ONLYME, LPVOID(ON_RET_NO));
	}
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명	:	장수용병을 해고한다.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
VOID	SOPlayer::RecvGBarrackDismissalSolder()
{
	OnReqGBarrackDismissalSolder		*pOnReqGBarrackDismissalSolder;

	pOnReqGBarrackDismissalSolder	=	(OnReqGBarrackDismissalSolder*)cMsg;

	if(stAccountInfo.pVillage != NULL)
	{
		if( stAccountInfo.pVillage->m_Barrack != NULL )
		{
			// 장수용병 해고에 대한 정보는 Village 클래스에서 보내준다.
			if(stAccountInfo.pVillage->m_Barrack->GBarrackDismissalSolder(this, pOnReqGBarrackDismissalSolder->dwSlot) == FALSE)
			{
				// 실패시 Message 만 전송해 준다. 성공했을때는 내부에서 Message를 보낸다.
				SendFieldMsg(ON_RESPONSE_GBARRACK_DISMISSALSOLDER, SO_SFM_ONLYME, LPVOID(ON_RET_NO));
			}
		}
		else
		{
			// 장수용병 고용란에서 용병을 해고할수 없다.
			SendFieldMsg(ON_RESPONSE_GBARRACK_DISMISSALSOLDER, SO_SFM_ONLYME, LPVOID(ON_RET_NO));
		}
	}
	else
	{		
		// 장수용병 고용란에서 용병을 해고할수 없다.
		SendFieldMsg(ON_RESPONSE_GBARRACK_DISMISSALSOLDER, SO_SFM_ONLYME, LPVOID(ON_RET_NO));
	}
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명	:	2차 장수 고용 List를 요청
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
VOID	SOPlayer::RecvGBarrackGetGeneralExList()
{
	if(stAccountInfo.pVillage != NULL)
	{
		if( stAccountInfo.pVillage->m_Barrack != NULL )
		{
			// 들어 갔을 경우의 메시지는 Village 클래스에서 보내준다.		
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

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명	:	2차 장수 고용 요청
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
VOID	SOPlayer::RecvGBarrackBuyGeneralEx()
{
	OnReqGBarrackBuySolder		*pOnReqGBarrackBuySolder;

	pOnReqGBarrackBuySolder	= (OnReqGBarrackBuySolder*)cMsg;

	if(stAccountInfo.pVillage != NULL)
	{
		if( stAccountInfo.pVillage->m_Barrack != NULL )
		{
			// 2차 장수 고용에 대한 정보는 Village 클래스에서 보내준다.
			if(stAccountInfo.pVillage->m_Barrack->GBarrackBuyGeneralEx(this, pOnReqGBarrackBuySolder->dwBuySolderID, (BYTE*)pOnReqGBarrackBuySolder->szName) == FALSE)
			{
				// 실패시 Message 만 전송해 준다. 성공했을때는 내부에서 Message를 보낸다.
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

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명	:	2차 장수 전직 List 요청 
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
VOID	SOPlayer::RecvGBarrackGetChangeGeneralExList()
{
	if(stAccountInfo.pVillage != NULL)
	{
		if( stAccountInfo.pVillage->m_Barrack != NULL )
		{
			// 들어 갔을 경우의 메시지는 Village 클래스에서 보내준다.		
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

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명	:	2차 장수 전직 요청
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
VOID	SOPlayer::RecvGBarrackChangeGeneralEx()
{
	OnReqGBarrackChangeJobSolder*		pOnReqGBarrackChangeJobSolder;

	pOnReqGBarrackChangeJobSolder	=	(OnReqGBarrackChangeJobSolder*)cMsg;

	if(stAccountInfo.pVillage != NULL)
	{
		if( stAccountInfo.pVillage->m_Barrack != NULL )
		{

			// 장수용병 전직에 대한 정보는 Village 클래스에서 보내준다.
			if(stAccountInfo.pVillage->m_Barrack->GBarrackChangeGeneralEx(this, pOnReqGBarrackChangeJobSolder->dwChangeJobSlot, pOnReqGBarrackChangeJobSolder->dwChangeJobKind) == FALSE)
			{
				// 실패시 Message 만 전송해 준다. 성공했을때는 내부에서 Message를 보낸다.
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

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명	:	2차 장수 해고 List 요청
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
VOID	SOPlayer::RecvGBarrackGetDismissalGeneralExList()
{
	if(stAccountInfo.pVillage != NULL)
	{
		if( stAccountInfo.pVillage->m_Barrack != NULL )
		{
			// 들어 갔을 경우의 메시지는 Village 클래스에서 보내준다.		
			stAccountInfo.pVillage->m_Barrack->GBarrackGetDismissalGeneralExList(this);				
		}
		else
		{
			// 장수용병 고용란에 List를 줄수 없다.
			SendFieldMsg(ON_RESPONSE_GBARRACK_GETDISMISSAL_GENERAL_LIST, SO_SFM_ONLYME, LPVOID(ON_RET_NO));
		}
	}
	else
	{		
		// 장수용병 고용란에 List를 줄수 없다.
		SendFieldMsg(ON_RESPONSE_GBARRACK_GETDISMISSAL_GENERAL_LIST, SO_SFM_ONLYME, LPVOID(ON_RET_NO));
	}
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명	:	2차 장수 해고 요청
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
VOID	SOPlayer::RecvGBarrackDismissalGeneralEx()
{
	OnReqGBarrackDismissalSolder*			pOnReqGBarrackDismissalSolder;

	pOnReqGBarrackDismissalSolder	=		(OnReqGBarrackDismissalSolder*)cMsg;

	if(stAccountInfo.pVillage != NULL)
	{
		if( stAccountInfo.pVillage->m_Barrack != NULL )
		{
			// 장수용병 해고에 대한 정보는 Village 클래스에서 보내준다.
			if(stAccountInfo.pVillage->m_Barrack->GBarrackSellDismissalGeneralEx(this, pOnReqGBarrackDismissalSolder->dwSlot) == FALSE)
			{
				// 실패시 Message 만 전송해 준다. 성공했을때는 내부에서 Message를 보낸다.
				SendFieldMsg(ON_RESPONSE_GBARRACK_DISMISSAL_GENERAL, SO_SFM_ONLYME, LPVOID(ON_RET_NO));
			}
		}
		else
		{
			// 장수용병 고용란에서 용병을 해고할수 없다.
			SendFieldMsg(ON_RESPONSE_GBARRACK_DISMISSAL_GENERAL, SO_SFM_ONLYME, LPVOID(ON_RET_NO));
		}
	}
	else
	{		
		// 장수용병 고용란에서 용병을 해고할수 없다.
		SendFieldMsg(ON_RESPONSE_GBARRACK_DISMISSAL_GENERAL, SO_SFM_ONLYME, LPVOID(ON_RET_NO));
	}
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명	:	몬스터 List를 요청한다.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
VOID	SOPlayer::RecvMBarrackGetMonsterList()
{
	if( stAccountInfo.pVillage != NULL )
	{
		if( stAccountInfo.pVillage->m_Barrack != NULL )
		{

			// 들어 갔을 경우의 메시지는 Village 클래스에서 보내준다.		
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


//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명	:	몬스터를 고용한다.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
VOID	SOPlayer::RecvMBarrackBuyMonster()
{
	OnReqMBarrackBuyMonster		*pOnReqMBarrackBuyMonster;

	pOnReqMBarrackBuyMonster	=	(OnReqMBarrackBuyMonster*)cMsg;

	if( stAccountInfo.pVillage != NULL )
	{
		if( stAccountInfo.pVillage->m_Barrack != NULL )
		{

			// 몬스터 고용에 대한 정보는 Village 클래스에서 보내준다.
			if( stAccountInfo.pVillage->m_Barrack->MBarrackBuyMonster(this, pOnReqMBarrackBuyMonster->dwBuyMonsterID, (BYTE*)pOnReqMBarrackBuyMonster->szName) == FALSE )
			{
				// 실패시 Message 만 전송해 준다. 성공했을때는 내부에서 Message를 보낸다.
				SendFieldMsg( ON_RESPONSE_MBARRACK_BUYMONSTER, SO_SFM_ONLYME, LPVOID(ON_RET_NO) );
			}
		}
		else
		{
			// 몬스터 고용란에서 몬스터를 고용할수 없다.
			SendFieldMsg( ON_RESPONSE_MBARRACK_BUYMONSTER, SO_SFM_ONLYME, LPVOID(ON_RET_NO) );
		}

	}
	else
	{		
		// 몬스터 고용란에서 몬스터를 고용할수 없다.
		SendFieldMsg( ON_RESPONSE_MBARRACK_BUYMONSTER, SO_SFM_ONLYME, LPVOID(ON_RET_NO) );
	}
}


//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명	:	해고할 몬스터 List를 요청한다.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
VOID	SOPlayer::RecvMBarrackGetDismissMonsterList()
{
	if( stAccountInfo.pVillage != NULL )
	{
		if( stAccountInfo.pVillage->m_Barrack != NULL )
		{

			// 들어 갔을 경우의 메시지는 Village 클래스에서 보내준다.		
			stAccountInfo.pVillage->m_Barrack->MBarrackGetDismissMonsterList( this );				
		}
		else
		{
			// 몬스터 고용란에 List를 줄수 없다.
			SendFieldMsg( ON_RESPONSE_MBARRACK_GETDISMISSMONSTERLIST, SO_SFM_ONLYME, LPVOID(ON_RET_NO) );
		}

	}
	else
	{		
		// 몬스터 고용란에 List를 줄수 없다.
		SendFieldMsg( ON_RESPONSE_MBARRACK_GETDISMISSMONSTERLIST, SO_SFM_ONLYME, LPVOID(ON_RET_NO) );
	}
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명	:	몬스터를 해고한다.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
VOID	SOPlayer::RecvMBarrackDismissMonster()
{
	OnReqMBarrackDismissMonster			*pOnReqMBarrackDismissMonster;

	pOnReqMBarrackDismissMonster	=	(OnReqMBarrackDismissMonster*)cMsg;

	if( stAccountInfo.pVillage != NULL )
	{
		if( stAccountInfo.pVillage->m_Barrack != NULL )
		{

			// 몬스터 해고에 대한 정보는 Village 클래스에서 보내준다.
			if( stAccountInfo.pVillage->m_Barrack->MBarrackDismissMonster(this, pOnReqMBarrackDismissMonster->dwSlot) == FALSE )
			{
				// 실패시 Message 만 전송해 준다. 성공했을때는 내부에서 Message를 보낸다.
				SendFieldMsg( ON_RESPONSE_MBARRACK_DISMISSMONSTER, SO_SFM_ONLYME, LPVOID(ON_RET_NO) );
			}
		}
		else
		{
			// 몬스터 고용란에서 몬스터를 해고할수 없다.
			SendFieldMsg( ON_RESPONSE_MBARRACK_DISMISSMONSTER, SO_SFM_ONLYME, LPVOID(ON_RET_NO) );			
		}
	}
	else
	{		
		// 몬스터 고용란에서 몬스터를 해고할수 없다.
		SendFieldMsg( ON_RESPONSE_MBARRACK_DISMISSMONSTER, SO_SFM_ONLYME, LPVOID(ON_RET_NO) );
	}
}


//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명	:	통장개수를 얻어온다.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
SI32	SOPlayer::GetBankBootNum()
{
	SI32			siNum;
	if(pMyWorld->clDatabase->GetDBInfo(SO_DB_INFOKIND_BANKBOOK_NUM, this, LPVOID(&siNum)) == FALSE)
		siNum	=	-1;
	return siNum;
}


//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명	:	신용 담보 대출 최대 금액.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
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
				//siMaxLoan을 저장해야 한다.
				SendFieldMsg(ON_RESPONSE_BANK_MAXCREDITLOAN, SO_SFM_ONLYME, LPVOID(ON_RET_BANK_OK), LPVOID(uiMaxLoan),  LPVOID(0));  
				return;
			}
		}
		bResult	=	ON_RET_BANK_NO;
	}
	if( bResult	==	ON_RET_BANK_NO )
		SendFieldMsg(ON_RESPONSE_BANK_MAXCREDITLOAN, SO_SFM_ONLYME, LPVOID(ON_RET_BANK_NO));
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명	:	용병 치료List를 요청한다.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
VOID	SOPlayer::RecvHospitalGetCureList()
{
	if(stAccountInfo.pVillage != NULL)
	{
		// 들어 갔을 경우의 메시지는 Village 클래스에서 보내준다.		
		stAccountInfo.pVillage->m_Hospital->HospitalGetCureList(this);				
	}
	else
	{		
		// 용병 치료란에 List를 줄수 없다.
		SendFieldMsg(ON_RESPONSE_HOSPITAL_GETCURELIST, SO_SFM_ONLYME);
	}
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명	:	용병을 치료한다.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
VOID	SOPlayer::RecvHospitalCureSolder()
{
	OnReqHospitalCureSolder		*pOnReqHospitalCureSolder;

	pOnReqHospitalCureSolder	=	(OnReqHospitalCureSolder*)cMsg;

	if(stAccountInfo.pVillage != NULL)
	{
		// 용병 치료에 대한 정보는 Village 클래스에서 보내준다.
		if(stAccountInfo.pVillage->m_Hospital->HospitalCureSolder(this, pOnReqHospitalCureSolder->dwCureSolderSlot) == FALSE)
		{
			// 실패시 Message 만 전송해 준다. 성공했을때는 내부에서 Message를 보낸다.
			SendFieldMsg(ON_RESPONSE_HOSPITAL_CURESOLDER, SO_SFM_ONLYME, LPVOID(ON_RET_NO));
		}
	}
	else
	{		
		// 용병 치료란에서 용병을 치료할수 없다.
		SendFieldMsg(ON_RESPONSE_HOSPITAL_CURESOLDER, SO_SFM_ONLYME, LPVOID(ON_RET_NO));
	}
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명	:	모든 용병을 치료한다.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
VOID	SOPlayer::RecvHospitalCureAllSolder()
{
	OnReqHospitalCureAllSolder		*pOnReqHospitalCureAllSolder;

	pOnReqHospitalCureAllSolder	=	(OnReqHospitalCureAllSolder*)cMsg;

	if(stAccountInfo.pVillage != NULL)
	{
		// 용병 치료에 대한 정보는 Village 클래스에서 보내준다.
		if(stAccountInfo.pVillage->m_Hospital->HospitalCureAllSolder(this) == FALSE)
		{
			// 실패시 Message 만 전송해 준다. 성공했을때는 내부에서 Message를 보낸다.
			SendFieldMsg(ON_RESPONSE_HOSPITAL_CUREALLSOLDER, SO_SFM_ONLYME, LPVOID(ON_RET_NO));
		}
	}
	else
	{		
		// 용병 치료란에서 용병을 치료할수 없다.
		SendFieldMsg(ON_RESPONSE_HOSPITAL_CUREALLSOLDER, SO_SFM_ONLYME, LPVOID(ON_RET_NO));
	}
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명	:	부두에 들어감
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
VOID	SOPlayer::RecvWharfEnter()
{
	if(stAccountInfo.pVillage != NULL)
	{
		// 들어 갔을 경우의 메시지는 Village 클래스에서 보내준다.		
		stAccountInfo.pVillage->m_Wharf->WharfEnter(this);				
	}
	else
	{		
		// 부두로 들어갈수 없다.
		SendFieldMsg(ON_RESPONSE_WHARF_ENTER, SO_SFM_ONLYME, LPVOID(ON_RET_NO));
	}
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명	:	표를 삼
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
VOID	SOPlayer::RecvWharfBuyTicket()
{
	OnReqWharf_BuyTicket*			lpOnReqWharf_BuyTicket;

	if(stAccountInfo.pVillage != NULL)
	{
		// 들어 갔을 경우의 메시지는 Village 클래스에서 보내준다.		
		lpOnReqWharf_BuyTicket = (OnReqWharf_BuyTicket*)cMsg;

		stAccountInfo.pVillage->m_Wharf->WharfBuyTicket(this, lpOnReqWharf_BuyTicket->siTicketKind);
	}
	else
	{		
		// 부두에서 Ticket을 살수 없다.
		SendFieldMsg(ON_RESPONSE_WHARF_BUYTICKET, SO_SFM_ONLYME, LPVOID(ON_RET_NO));
	}
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명	:	부두를 나감
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
VOID	SOPlayer::RecvWharfExit()
{
	if(stAccountInfo.pVillage != NULL)
	{
		// 들어 갔을 경우의 메시지는 Village 클래스에서 보내준다.		
		stAccountInfo.pVillage->m_Wharf->WharfExit(this);				
	}
	else
	{		
		// 부두에서 나갈수 없다.
		SendFieldMsg(ON_RESPONSE_WHARF_EXIT, SO_SFM_ONLYME, LPVOID(ON_RET_NO));
	}
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명	:	부두 대기실에 들어감
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
VOID	SOPlayer::RecvWharfWaitRoomEnter()
{
	if(stAccountInfo.pVillage != NULL)
	{
		// 들어 갔을 경우의 메시지는 Village 클래스에서 보내준다.		
		stAccountInfo.pVillage->m_Wharf->WharfWaitRoomEnter(this);				
	}
	else
	{		
		// 부두대기실에 들어갈수 없다.
		SendFieldMsg(ON_RESPONSE_WHARFWAITROOM_ENTER, SO_SFM_ONLYME, LPVOID(ON_RET_NO));
	}
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명	:	부두 대기실을 나감
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
VOID	SOPlayer::RecvWharfWaitRoomExit()
{
	if(stAccountInfo.pVillage != NULL)
	{
		// 들어 갔을 경우의 메시지는 Village 클래스에서 보내준다.		
		stAccountInfo.pVillage->m_Wharf->WharfWaitRoomExit(this);				
	}
	else
	{		
		// 부두대기실에서 나갈수 없다.
		SendFieldMsg(ON_RESPONSE_WHARFWAITROOM_EXIT, SO_SFM_ONLYME, LPVOID(ON_RET_NO));
	}
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명	:	DB에서 Ticket 정보를 얻어옴
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
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
	// 무조건 OK
	SendFieldMsg(ON_RESPONSE_GOVERNMENT_MAIN_ENTER, SO_SFM_ONLYME, LPVOID(ON_RET_OK));
}

VOID	SOPlayer::RecvGovernmentLeave()
{
	// 무조건 OK
	SendFieldMsg(ON_RESPONSE_GOVERNMENT_MAIN_LEAVE, SO_SFM_ONLYME, LPVOID(ON_RET_OK));
}

VOID	SOPlayer::RecvGovernmentInvestmentEnter()
{
	OnReqGovernmentEnterInvestment*			lpOnReqGovernmentEnterInvestment;

	lpOnReqGovernmentEnterInvestment = (OnReqGovernmentEnterInvestment*)cMsg;
	if(stAccountInfo.pVillage != NULL)
	{
		// 들어 갔을 경우의 메시지는 Village 클래스에서 보내준다.		
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
		// 들어 갔을 경우의 메시지는 Village 클래스에서 보내준다.		
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

	// siVillageCode에 -1이 들어오면 플레이어가 현재 들어가 있는 마을의 투자자 정보를 보내준다.
	// siVillageCode가 -1이 아닌 값이면 플레이어가 들어가 있는 마을이 아닌 siVillageCode의 투자자 정보를 보내준다.
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
		// 들어 갔을 경우의 메시지는 Village 클래스에서 보내준다.		
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
		// 들어 갔을 경우의 메시지는 Village 클래스에서 보내준다.		
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
			printf("안내문-금지된문자 사용[%s]-[%s]-[%c]\n",Temp,cTemp,*cTemp);
			return;
		*/
		uiResult = stAccountInfo.pVillage->m_Government->WriteDescription(this, Temp);
	}
	SendFieldMsg(ON_RESPONSE_WRITE_DESCRIPT, SO_SFM_ONLYME, LPVOID(uiResult));
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	관청, 마을정보 얻기
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
VOID	SOPlayer::RecvGovernmentVillageInfo()
{
	if(stAccountInfo.pVillage != NULL)
	{
		stAccountInfo.pVillage->GetVillageInfo(this);
	}
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	관청, 투자한돈 가지고 나가기
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
VOID	SOPlayer::RecvGovernmentInvestmentGetOut()
{
	OnReqGovernmentInvestmentGetOut*			lpOnReqGovernmentInvestmentGetOut;

	lpOnReqGovernmentInvestmentGetOut = (OnReqGovernmentInvestmentGetOut*)cMsg;
	if(stAccountInfo.pVillage != NULL)
	{
		stAccountInfo.pVillage->m_Government->InvestmentGetOut(this, lpOnReqGovernmentInvestmentGetOut->GetOutMoney);
	}
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	몇년간 마을의 수익 통계 보기.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
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

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	마을방어력에 투자할수 있는 정보를 얻는다
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
VOID	SOPlayer::RecvReqMaxVillageDefenceInvestment()
{
	if(stAccountInfo.pVillage != NULL)
	{
		stAccountInfo.pVillage->m_Government->SendVillageReqMaxVillageDefenceInvestment(this);
	}
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	마을 방어력에 투자하기
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
VOID	SOPlayer::RecvReqSetVillageDefenceInvestment()
{
	OnReqSetVillageDefenceInvestment*		lpOnReqSetVillageDefenceInvestment;

	lpOnReqSetVillageDefenceInvestment = (OnReqSetVillageDefenceInvestment*)cMsg;
	if(stAccountInfo.pVillage != NULL)
	{
		stAccountInfo.pVillage->m_Government->SendVillageReqSetVillageDefenceInvestment(this, lpOnReqSetVillageDefenceInvestment->InvestmentMoney);
	}
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	상단 생성에 필요한 비용을 요구하는 메시지를 받았다.
//	수정 일자	:	2003-01-27 오후 2:18:10 - 백상현
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
VOID	SOPlayer::RecvReqGuildCreateCost()
{
	MONEY	mnGuildCreateCost	=	pMyWorld->clIGuild.GetGuildCreateCost();
	SendFieldMsg(ON_RESPONSE_GUILD_CREATE_COST, SO_SFM_ONLYME, (LPVOID*)&mnGuildCreateCost );
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	길드를 생성한다는 메시지를 받았다.
//	수정 일자	:	2002-02-25 오전 11:26:10 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
VOID	SOPlayer::RecvCreateGuild()
{

	// DB합칠곳

	OnReqCreateGuildMsg		*pCreateGuildMsg;
	CHAR					*pGuildName;	
	BOOL					bDBSuccess = FALSE;		
	CHAR					szCharID[ON_ID_LENGTH + 1];
	CHAR					szGuildName[ON_GUILDNAME_LENGTH + 1];
	BOOL					bCreateGuildSuccess = FALSE;
	MONEY					mnGuildCreateCost;

	if(stAccountInfo.pVillage == NULL)														goto	END;	// 마을에 있지 않으면 상단을 생성할 수 없다.
	if(stAccountInfo.pVillage->uiGuildID !=0)												goto	END;	// 현재 상단을 만들려고 하는 마을에 이미 다른 상단이 있으면 상단을 만들수가 없다.
	// 자신이 다른상단에 상단원이면 상단을 만들수없다.
	if (uiGuildID != 0 ) goto END;
	
	// 내가 이 마을의 최고 투자자인지 본다.
	ZeroMemory(szCharID, sizeof(szCharID));
	strncpy(szCharID, szID, ON_ID_LENGTH);
	if(stAccountInfo.pVillage->m_Government->CheckInvestmentMaxInvestmentorName(szCharID, 0) == FALSE)	goto END;		

	// 길드 생성시 필요한 자금
	mnGuildCreateCost = pMyWorld->clIGuild.GetGuildCreateCost();

	// 길드 생성시 돈이 충분히 있어야 한다.
	if(EnoughMoney(mnGuildCreateCost) == FALSE)												goto	END;

	pCreateGuildMsg	=	(OnReqCreateGuildMsg*)cMsg;	
	pGuildName		=	(CHAR*)((CHAR*)pCreateGuildMsg + sizeof(OnReqCreateGuildMsg));	
	
	ZeroMemory(szGuildName, sizeof(szGuildName));
	strncpy(szGuildName, pGuildName, min(pCreateGuildMsg->uiGuildNameLength, ON_GUILDNAME_LENGTH));

	// 상단 이름이 유효한지 판단한다.
	if(CheckForbiddenName(szGuildName) == FALSE)											goto	END;
	if(pMyWorld->clIGuild.IsUsedName(szGuildName) == TRUE )                                 goto    END;

	

	uiGuildID		=	pMyWorld->clIGuild.CreateGuild(szGuildName, this, stAccountInfo.pVillage);

	
	if(uiGuildID == 0)																		goto	END;
	
	ChangeMoney(-mnGuildCreateCost,FALSE);

	// 성공적으로 상단을 생성했다.
	bCreateGuildSuccess	=	TRUE;	

END:
	if(bCreateGuildSuccess == TRUE)
	{	
		// 상단을 생성하는데 성공했다.		
		SendFieldMsg(ON_RESPONSE_CREATE_GUILD, SO_SFM_ONLYME, LPVOID(ON_RET_OK));
	}
	else
	{
		// 상단을 생성하는데 실패했다.
		SendFieldMsg(ON_RESPONSE_CREATE_GUILD, SO_SFM_ONLYME, LPVOID(ON_RET_NO));
	}
}


//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	상단을 삭제한다는 메시지를 받았다
//	수정 일자	:	2002-02-25 오전 11:26:05 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
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
	//공성하는 날인가
	if(pMyWorld->m_WarTimeParser.IsWarCheck(pMyWorld->pRealTime->tm_wday, pMyWorld->pRealTime->tm_hour))
	{
		if(pMyWorld->IsAttackVillageTimeZone()) //공성 시간인지 체크
		{
			SendFieldMsg(ON_RESPONSE_DELETE_GUILD, SO_SFM_ONLYME, LPVOID(ON_RET_NO));
			return;
		}
	}

	//적대가 1개라도 걸린 상단은 상단을 해체할수 없다.
	if(pMyWorld->clIGuild.GetGuildWarCheck(uiGuildID)) 
	{  
		SendFieldMsg(ON_RESPONSE_DELETE_GUILD, SO_SFM_ONLYME, LPVOID(ON_RET_NO));
		return;
	}

	// Guild ID가 유효한지 체크한다.
	if(pMyWorld->clIGuild.DeleteGuild(uiGuildID, stAccountInfo.pVillage) == TRUE)
	{
		// 상단을 삭제하는데 성공했다.		
		SendFieldMsg(ON_RESPONSE_DELETE_GUILD, SO_SFM_ONLYME, LPVOID(ON_RET_OK));
	}		
	else
	{
		// 상단을 삭제하는데 실패했다.
		SendFieldMsg(ON_RESPONSE_DELETE_GUILD, SO_SFM_ONLYME, LPVOID(ON_RET_NO));
	}	
}


//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	상단에 참여하거나 떠난다.
//	수정 일자	:	2002-03-04 오전 10:41:08 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
VOID	SOPlayer::RecvJoinLeaveGuild()
{
	OnReqJoinLeaveGuildMsg		*pJoinLeaveGuildMsg;	
	BOOL						bDBSuccess = FALSE, bSuccess = FALSE;
	BOOL						bDeleteGuild	=	FALSE;	
	BOOL						bCandidate;
	DWORD						dwResponse = ON_RET_NO;

	pJoinLeaveGuildMsg	=	(OnReqJoinLeaveGuildMsg*)cMsg;

    //공성이 있는날이면서 공성시간에는 상단가입, 탈퇴가 않된다.
	if(pMyWorld->m_WarTimeParser.IsWarCheck(pMyWorld->pRealTime->tm_wday, pMyWorld->pRealTime->tm_hour))
	if(pMyWorld->m_WarTimeParser.IsWarTime(pMyWorld->pRealTime->tm_hour))
	{
		SendFieldMsg(ON_RESPONSE_JOIN_LEAVE_GUILD, SO_SFM_ONLYME, LPVOID(dwResponse));
		return;
	}

	if(uiGuildID == 0)
	{
		// 참여를 해야 한다.
		dwResponse		=	ON_RET_GLD_JOIN_NO;

		// 현재 마을이 유효한지 알아온다.
		if(stAccountInfo.pVillage != NULL)
		{
			// 그 마을의 Guild ID를 구한다.
			if(stAccountInfo.pVillage->uiGuildID != 0)
			{
				// 상단에 지원한다.
				if(pMyWorld->clIGuild.AddCandidate(stAccountInfo.pVillage->uiGuildID, this) == TRUE)									
					dwResponse			=	ON_RET_GLD_JOIN_OK;												
			}
		}
	}
	else
	{
		// 참여한 상단에서 떠난다.
		dwResponse			=	ON_RET_GLD_LEAVE_NO;

		// 현재 마을이 유효한지 알아온다.
		if(stAccountInfo.pVillage != NULL)
		{
			// 그 마을의 Guild ID와 자기의 Guild ID와 같아야 한다.
			// 즉 자기 상단이 보유하고 있는 마을이어야 한다.
			if(stAccountInfo.pVillage->uiGuildID == uiGuildID)
			{
				if(pMyWorld->m_WarTimeParser.IsWarCheck(pMyWorld->pRealTime->tm_wday, pMyWorld->pRealTime->tm_hour)) return;
				if(pMyWorld->m_WarTimeParser.IsWarTime(pMyWorld->pRealTime->tm_hour)) return;
								
				// 대방인지 본다.
				if(siClassInGuild == pMyWorld->clIGuildClass.GetNumericClassCode(SO_GUILD_SC_DAI))
				{
					// 대방이다. 					
					
					///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
					// for debug
					if( uiGuildID == 0 )
					{
//						pMyWorld->clServer->WriteInfo( "..\\data\\DeletedGuild.txt", "Clan Master has left the guild. Guild disorganizes. But the Guild ID is 0" );
						writeInfoToFile( "DeletedGuild.txt", "Clan Master has left the guild. Guild disorganizes. But the Guild ID is 0" );
					}
					///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

					// 상단을 없앤다.
					if(pMyWorld->clIGuild.DeleteGuild(uiGuildID,stAccountInfo.pVillage) == TRUE)
					{						
						dwResponse		=	ON_RET_GLD_LEAVE_OK_DELETEGLD;										
					}
				}
				else
				{
					// 대방이 아니다.

					// 상단에 한명이 빠져나갔다고 알려준다.
					if(siClassInGuild == pMyWorld->clIGuildClass.GetJWJCode())				bCandidate	=	TRUE;
					else																	bCandidate	=	FALSE;

					if(pMyWorld->clIGuild.DelGuildMember(stAccountInfo.pVillage->uiGuildID, szID, bCandidate, FALSE ,FALSE) == TRUE)					
						dwResponse		=	ON_RET_GLD_LEAVE_OK;										
				}				
			}
		}
	}

	// 성공했다.
	SendFieldMsg(ON_RESPONSE_JOIN_LEAVE_GUILD, SO_SFM_ONLYME, LPVOID(dwResponse));	
}


//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	상단에 참여하기를 기다리는 사용자들의 리스트를 얻어온다.
//	수정 일자	:	2002-02-25 오후 1:15:50 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
VOID	SOPlayer::RecvReqGuildWaitingJoinMembersList()
{
	cltGuild	*pclGuild;
	BOOL		bSuccess = FALSE;
	OnReqGuildWaitingJoinMembersListMsg		*pGuildWaitingJoinMembersListMsg;

	pGuildWaitingJoinMembersListMsg		=	(OnReqGuildWaitingJoinMembersListMsg*)cMsg;

	// 상단에 반드시 소속되어 있어야 한다.
	if(uiGuildID == 0)	goto	END;

	// 상단이 반드시 존재하여야 한다.
	if((pclGuild = pMyWorld->clIGuild.GetGuildByID(uiGuildID)) == NULL)	goto	END;		

	// 성공했다.
	bSuccess	=	TRUE;

END:
	if(bSuccess == TRUE)
	{
		// 길드원들의 리스트를 보내 줄 수 있다.
		SendFieldMsg(ON_RESPONSE_GUILD_WAITING_JOIN_MEMBERS_LIST, SO_SFM_ONLYME, LPVOID(ON_RET_OK), LPVOID(pclGuild), 
						LPVOID(pGuildWaitingJoinMembersListMsg->dwPage));
	}
	else
	{
		// 길드원들의 리스트를 보내 줄 수 없다
		SendFieldMsg(ON_RESPONSE_GUILD_WAITING_JOIN_MEMBERS_LIST, SO_SFM_ONLYME, LPVOID(ON_RET_NO));
	}
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	상단에 참여를 허락 및 거부한다.
//	수정 일자	:	2002-02-25 오후 1:46:04 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
VOID	SOPlayer::RecvJoinGuildOkNo()
{
	CHAR						szCharID[1024];
	OnReqJoinGuildOkNoMsg		*pJoinGuildOkNoMsg;	
	DWORD						dwResponse = ON_RET_NO;

	pJoinGuildOkNoMsg		=	(OnReqJoinGuildOkNoMsg*)cMsg;	
	
	ZeroMemory(szCharID, sizeof(szCharID));
	strncpy(szCharID, pJoinGuildOkNoMsg->szID, ON_ID_LENGTH);

	if(pJoinGuildOkNoMsg->dwResponse == ON_RET_OK)			// '입단허락'버튼을 눌렀을 경우
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
					// 등급을 조정해준다. (입단 허락)
					if(pMyWorld->clIGuild.AddGuildMember(uiGuildID, szCharID) == TRUE)
						dwResponse	=	ON_RET_OK;
					else		
						dwResponse	=	ON_RET_NO_GLD_ADMIT_FULL;
				}
			}
		}		
	}
	else if(pJoinGuildOkNoMsg->dwResponse == ON_RET_NO)		// '입단취소'버튼을 눌렀을 경우
	{
		// 참여를 거부한다.
		if(pMyWorld->clIGuild.DelGuildMember(uiGuildID, szCharID, TRUE, TRUE ,TRUE) == TRUE)	dwResponse	=	ON_RET_OK;		
	}

	// 잘 받았다고 응답을 보내준다.
	SendFieldMsg(ON_RESPONSE_JOINGUILD_OKNO, SO_SFM_ONLYME, LPVOID(dwResponse));
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	상단의 대한 상세 정보를 얻어온다.	
//	수정 일자	:	2002-04-22 오전 9:35:33 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
VOID	SOPlayer::RecvGuildDetailInfo()
{
	OnReqGuildDetailInfoMsg	*pGuildDetailInfoMsg;
	CHAR					szGuildID[ON_GUILDNAME_LENGTH + 1];	// 길드의 이름
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

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	상단원 들의 리스트를 얻어온다.
//	수정 일자	:	2002-02-25 오후 2:18:10 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
VOID	SOPlayer::RecvReqGuildMembersList()
{
	OnReqGuildMembersListMsg	*pGuildMembersListMsg;
	cltGuild					*pclGuild;
	UI32						uiReqGuild;

	pGuildMembersListMsg	=	(OnReqGuildMembersListMsg*)cMsg;	

	// 만약에 상단 ID가 0이라면 나의 상단원 정보를 요구하는것이다.
	if(pGuildMembersListMsg->uiGuidlID == 0)
		uiReqGuild	=	uiGuildID;
	else
		uiReqGuild	=	pGuildMembersListMsg->uiGuidlID;

	if((pclGuild = pMyWorld->clIGuild.GetGuildByID(uiReqGuild)) != NULL)	
		SendFieldMsg(ON_RESPONSE_GUILD_MEMBERS_LIST, SO_SFM_ONLYME, LPVOID(ON_RET_OK), LPVOID(pclGuild), LPVOID(pGuildMembersListMsg->dwPage));	
	else	
		SendFieldMsg(ON_RESPONSE_GUILD_MEMBERS_LIST, SO_SFM_ONLYME, LPVOID(ON_RET_NO));	
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	상단원을 해고한다.
//	수정 일자	:	2002-03-25 오후 2:05:32 - 백상현
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
VOID	SOPlayer::RecvGuildDismissal()
{
	CHAR				szCharID[ON_ID_LENGTH + 1];
	OnReqDismissalMsg	*pDismissalMsg;
	cltGuild			*pclGuild;
	BOOL				bSuccess	=	FALSE;

	pDismissalMsg	=	(OnReqDismissalMsg*)cMsg;

	ZeroMemory(szCharID, sizeof(szCharID));
	strncpy(szCharID, pDismissalMsg->szID, ON_ID_LENGTH);

	//공성 시간에는 해고할수 없다.
	if(pMyWorld->m_WarTimeParser.IsWarCheck(pMyWorld->pRealTime->tm_wday, pMyWorld->pRealTime->tm_hour)) goto END;
	if(pMyWorld->m_WarTimeParser.IsWarTime(pMyWorld->pRealTime->tm_hour)) goto END;
	// 길드 객체를 얻어온다.
	if( (pclGuild	=	pMyWorld->clIGuild.GetGuildByID(uiGuildID)) == NULL )		goto END;
	
	// 해고하려는 사람이 대방이라면 실패 처리한다.
	if( strcmp(pclGuild->GetDAIID(), szCharID) == 0 )								goto END;

	// 길드원 삭제
	if( pMyWorld->clIGuild.DelGuildMember(uiGuildID, szCharID, FALSE, TRUE ,TRUE) == TRUE )		bSuccess	=	TRUE;


END:
	if( bSuccess )
	{
		// 상단에게도 알려준다.
		SendFieldMsg(ON_RESPONSE_GUILD_DISMISSAL, SO_SFM_ONLYME, LPVOID(ON_RET_OK));
	}
	else
	{
		SendFieldMsg(ON_RESPONSE_GUILD_DISMISSAL, SO_SFM_ONLYME, LPVOID(ON_RET_NO));
	}
}


//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	상단원을 특정 직급으로 임명한다는 메시지를 처리한다.
//	수정 일자	:	2003-02-11 오후 1:43 - 백상현
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
VOID	SOPlayer::RecvReqGuildPromotion()
{
	CHAR						szCharID[ON_ID_LENGTH+1];
	OnRequestGuildPromotion		*pPromotionMsg;
	SI32						siSHNCode	=	pMyWorld->clIGuildClass.GetSHNCode();			// 사환
	SI32						siHNSCode	=	pMyWorld->clIGuildClass.GetHNSCode();			// 행수
	SI32						siDHSCode	=	pMyWorld->clIGuildClass.GetDHSCode();			// 대행수
	SI32						siHNDCode	=	pMyWorld->clIGuildClass.GetHNDCode();			// 행동대장
	BOOL						bSuccess	=	FALSE;

	pPromotionMsg = (OnRequestGuildPromotion*)cMsg;

	ZeroMemory( szCharID, sizeof(szCharID) );
	strncpy( szCharID, pPromotionMsg->szCharID, ON_ID_LENGTH );

	cltGuild *pGuild = pMyWorld->clIGuild.GetGuildByID( uiGuildID );

	strGuildCommonMemberInfo MemberInfo;

	// 캐릭터 이름을 사용하여 모든 정보를 얻어온다.
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
		// 대행수로 임명 요청
		// 조건1: 임명받을 사람의 현직급은 사환 이상이어야 한다.
		// 조건2: 현재 대행수 자리가 비어있어야 한다.

		// 사환보다 높은 직급이 아니면 임명 실패
		if( pMyWorld->clIGuildClass.ClassComp(MemberInfo.siClassInGuild, siSHNCode) == SO_GUILD_CLASSCOMP_LOW )			goto	END;
		// 이미 대행수가 있으면 임명 실패
		if( pGuild->m_GuildMemberManager.GetDHSID() != NULL )															goto	END;

		bSuccess	=	pMyWorld->clIGuild.ChangeClass( uiGuildID, szCharID, siDHSCode );
	}
	else if( pPromotionMsg->siClassCode == siHNDCode )
	{
		// 행동대장으로 임명 요청
		// 조건1: 임명받을 사람의 현직급은 사환이어야 한다.
		// 조건2: 행동대장의 수는 보유 마을 5개당 1명이다.		공식: (n + 4) / 5		n: 보유마을수

		if( MemberInfo.siClassInGuild != siSHNCode )	goto	END;				// 사환이 아니면 임명 실패
		// 임명할 수 있는 사환의 수를 넘어가면 임명 실패
		if( pGuild->m_GuildMemberManager.GetTotalHNDNum() >= (UI32)((pGuild->GetOwnerVillageNum() + 4) / 5) )	goto	END;

		bSuccess	=	pMyWorld->clIGuild.ChangeClass( uiGuildID, szCharID, siHNDCode );
	}

END:
	if( bSuccess )
		SendFieldMsg( ON_RESPONSE_GUILD_PROMOTION, SO_SFM_ONLYME, LPVOID(ON_RET_OK) );
	else		
		SendFieldMsg( ON_RESPONSE_GUILD_PROMOTION, SO_SFM_ONLYME, LPVOID(ON_RET_NO) );
}


//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	상단원을 특정 직급에서 해임한다는 메시지를 처리한다.
//	수정 일자	:	2003-02-11 오후 1:47 - 백상현
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
VOID	SOPlayer::RecvReqGuildDemotion()
{
	CHAR						szCharID[ON_ID_LENGTH+1];
	OnRequestGuildDemotion		*pDemotionMsg;
	SI32						siSHNCode	=	pMyWorld->clIGuildClass.GetSHNCode();			// 사환
	SI32						siHNSCode	=	pMyWorld->clIGuildClass.GetHNSCode();			// 행수
	SI32						siDHSCode	=	pMyWorld->clIGuildClass.GetDHSCode();			// 대행수
	SI32						siHNDCode	=	pMyWorld->clIGuildClass.GetHNDCode();			// 행동대장
	SI32						siDAICode	=	pMyWorld->clIGuildClass.GetDAICode();			// 행동대장
	BOOL						bSuccess	=	FALSE;
	SI32						siNewClass;

	pDemotionMsg = (OnRequestGuildDemotion*)cMsg;
	ZeroMemory( szCharID, sizeof(szCharID) );
	strncpy( szCharID, pDemotionMsg->szCharID, ON_ID_LENGTH );

	cltGuild *pGuild = pMyWorld->clIGuild.GetGuildByID( uiGuildID );

	strGuildCommonMemberInfo MemberInfo;

	// 캐릭터 이름을 사용하여 모든 정보를 얻어온다.
	if( pGuild != NULL )
	{
		if( pGuild->m_GuildMemberManager.GetGuildMemberByCharName(szCharID, &MemberInfo) == FALSE )		goto	END;
	}
	else
	{
		goto	END;
	}

	if( MemberInfo.siClassInGuild == siDAICode )		goto END;							// 대방이면 해임할 수 없다.

	if( MemberInfo.siClassInGuild == siDHSCode )
	{
		// 대행수를 해임할 경우 --> 사환이 될 수도 있고 행수가 될 수도 있다.
		siNewClass	=	pMyWorld->GetHighestGuildClass( szCharID );							// 각 마을을 검사하여 이 캐릭터의 최대 직급을 얻어온다.
		bSuccess	=	pMyWorld->clIGuild.ChangeClass( uiGuildID, szCharID, siNewClass );	// 검사하여 얻어온 최대직급으로 바꿔준다.
	}
	else if( MemberInfo.siClassInGuild == siHNDCode )
	{
		// 행동대장을 해임할 경우 --> 사환이 된다.
		bSuccess	=	pMyWorld->clIGuild.ChangeClass( uiGuildID, szCharID, siSHNCode );
	}

END:

	if( bSuccess )
		SendFieldMsg( ON_RESPONSE_GUILD_DEMOTION, SO_SFM_ONLYME, LPVOID(ON_RET_OK) );
	else
		SendFieldMsg( ON_RESPONSE_GUILD_DEMOTION, SO_SFM_ONLYME, LPVOID(ON_RET_NO) );
}


//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	상단의 리스트를 요구한다.
//	수정 일자	:	2002-02-27 오후 2:45:30 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
VOID	SOPlayer::RecvGuildList()
{	
	// 길드의 정보를 보내준다.
	SendFieldMsg(ON_RESPONSE_GUILD_LIST, SO_SFM_ONLYME);
}


//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	상단을 설정한다.
//	수정 일자	:	2002-02-27 오후 4:27:53 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
VOID	SOPlayer::SetGuild(UI32 uiguildid)
{
	// 상단을 설정한다.
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

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	직급을 설정한다.
//	수정 일자	:	2002-02-27 오후 4:27:53 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
VOID	SOPlayer::SetClassInGuild(SI32 siclassinguild)
{
	// 직급을 설정한다.
	siClassInGuild	=	siclassinguild;	
}


//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	직급을 얻어온다.
//	수정 일자	:	2003-02-11 오후 2:173 - 백상현
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
SI32	SOPlayer::GetClassInGuild()
{
	return siClassInGuild;
}
	
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	상단의 깃발을 변경한다.
//	수정 일자	:	2002-04-04 오후 1:35:28 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
VOID	SOPlayer::RecvGuildChangeFlag()
{

	// DB합칠곳

	BOOL						bSuccess = FALSE;
	OnReqGuildChangeFlag		*pGuildChangeFlag;

	// 대방인지 체크한다.
	if(uiGuildID == 0)	goto	END;
	if(siClassInGuild != pMyWorld->clIGuildClass.GetNumericClassCode(SO_GUILD_SC_DAI))	goto	END;

	if(EnoughMoney(ON_GUILD_COST_CHANGE_FLAG) == FALSE)				goto	END;

	pGuildChangeFlag	=	(OnReqGuildChangeFlag*)cMsg;

	if(pMyWorld->clIGuild.SetFlag(uiGuildID, pGuildChangeFlag->siFlag) == FALSE)
		goto	END;

	// 성공적으로 깃발을 설정하게 되면
	// SetFlag 함수 내에서 자동적으로 현재 접속되어 있는 상단원에게 메시지를 보내준다.

	ChangeMoney(-ON_GUILD_COST_CHANGE_FLAG,FALSE);

	// DB에 저장한다.

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

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	사용된 깃발의 리스트를 얻어간다.
//	수정 일자	:	2002-04-04 오후 2:18:02 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
VOID	SOPlayer::RecvReqUsedFlagList()
{	
	if(uiGuildID != 0)
	{
		// 상단이 있을 경우에만 넘겨 준다.		
		SendFieldMsg(ON_RESPONSE_USED_FLAG_LIST, SO_SFM_ONLYME, LPVOID(uiGuildID));
	}
}
/*
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	마을을 수리한다는 메시지를 받았다.
//	수정 일자	:	2002-04-08 오후 2:31:00 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
VOID	SOPlayer::RecvDefenceVillage()
{
	OnReqRepairVillageMsg	*pRepairVillageMsg;
	BOOL					bSuccess = FALSE;

	pRepairVillageMsg	=	(OnReqRepairVillageMsg*)cMsg;

	if(stAccountInfo.pVillage == NULL)	goto	END;

	// 성공했을 경우에는 IncreaseDefence() 내에서 관련된 플레이어들에게 메시지를 보내준다.
	stAccountInfo.pVillage->RepairDefence(this, pRepairVillageMsg->vdDefence);

	// 성공했다.
	bSuccess	=	TRUE;
END:
	if(bSuccess == FALSE)
	{
		// 실패
		SendFieldMsg(ON_RESPONSE_REPAIR_VILLAGE, SO_SFM_ONLYME, LPVOID(ON_RET_NO));
	}	
}
*/

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	마을에서 특정 건물에 들어간다는 메시지를 받았다.
//	수정 일자	:	2002-10-02 오후 7:16:46 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
VOID	SOPlayer::RecvEnterStructure()
{
	OnReqEnterStructureMsg		*pEnterStructureMsg;
	BOOL						bSuccess	=	FALSE;
	
	pEnterStructureMsg		=	( OnReqEnterStructureMsg* )cMsg;
	
	// 현재 마을 안에 있는지 검사한다.
	if( IsInVillage() == FALSE )																							goto	END;
	
	// 현재 들어가 있는 건물이 있는지 검사한다.
	if( IsEnteredStructure() == TRUE )																						goto	END;
	
	// 특정 건물에 들어간다.
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

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	마을에서 특정 건물에서 나간다는 메시지를 받았다.
//	수정 일자	:	2002-10-02 오후 7:31:20 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
VOID	SOPlayer::RecvLeaveStructure()
{
	BOOL						bSuccess	=	FALSE;
	
	// 현재 마을 안에 있는지 검사한다.
	if( IsInVillage() == FALSE )																			goto	END;
	
	// 현재 들어가 있는 건물이 있는지 검사한다.
	if( IsEnteredStructure() == TRUE )																		goto	END;
	
	// 특정 건물에서 나온다.
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

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	마을 산업 단지의 공장에 들어간다.
//	수정 일자	:	2002-10-11 오후 4:57:09 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
VOID	SOPlayer::RecvEnterFactory()
{
	OnReqFarmWorkInfoMsg		*pFarmWorkInfoMsg;
	BOOL						bSuccess = FALSE;
	
	pFarmWorkInfoMsg		=	(OnReqFarmWorkInfoMsg*) cMsg;
	
	// 현재 마을 안에 있는지 검사한다.
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

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	마을 산업 단지의 공장에서 나간다.
//	수정 일자	:	2002-10-11 오후 4:56:58 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
VOID	SOPlayer::RecvLeaveFactory()
{
	BOOL	bSuccess = FALSE;

	// 현재 마을 안에 있는지 검사한다.
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
		// 성공이면,

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

	// 응답 메시지 전송 
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

	// 응답 메시지 전송 
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

		// 상단에 소속된 사람이 아니면
		if (pPlayer->siClassInGuild == 0) {

			ZeroMemory(resMsg.szGuildName,sizeof(resMsg.szGuildName));	
		}
		else
		{
			// 상단이 반드시 존재하여야 한다.
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

	// 빈 용병 슬롯이 있는지 검색한다.
	for( uiDonkeySlot = 0; uiDonkeySlot < ON_MAX_FOLLOWER_NUM; uiDonkeySlot++ )
	{
		if( IFollower.IsValidFollower(uiDonkeySlot) == FALSE )
		{
			bFound	=	TRUE;
			break;
		}
	}

	// 빈 슬롯이 있다면 당나귀 캐릭터의 정보를 세팅하고 DB에 알려주고 클라이언트로 날린다.
	if( bFound )
	{
		// 캐릭터 리스트에서 당나귀를 찾는다.
		for( SI32 i = 0; i < clServer->clWorld->clCharParser.GetCharTotalNum(); i++)
		{
			pCharHead = clServer->clWorld->clCharParser.GetCharHeader(i);
			
			if( pCharHead->uiID == clServer->clWorld->clCharParser.GetID("U85") )	break;
		}

		// Player의 용병칸에 당나귀를 추가한다.(DB에 저장까지 한다.)
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
		// OnlineText에서 uiCharName으로 캐릭터이름(당나귀) 얻기
		strncpy( stFollowerInfo.Name, clServer->clWorld->clOnlineText.Get( pCharHead->uiCharName ), ON_ID_LENGTH );
		CHAR	szCharName[256];
		ZeroMemory( szCharName, 256 );
		CopyMemory( szCharName, stFollowerInfo.Name, ON_ID_LENGTH );

		IFollower.InitFollower( uiDonkeySlot, &stFollowerInfo, pCharHead->siNation, &clServer->clWorld->clItem );

		DBIKConsumeDonkeyItem		DBInfo;

		ZeroMemory( &DBInfo, sizeof(DBIKConsumeDonkeyItem) );

		DBInfo.DBAccount		=	stAccountInfo.siDBAccount;
		DBInfo.uiSlot			=	stAccountInfo.uiSlot;
		DBInfo.uiFollowerID		=	uiFollowerID;					// 아이템을 사용한 용병
		DBInfo.uiItemID			=	uiItemID;						// 아이템 ID
		DBInfo.uiDonkeySlot		=	uiDonkeySlot;					// 당나귀가 들어갈 슬롯	
		DBInfo.siKind			=	stFollowerInfo.uiKind;
		clServer->clWorld->clDatabase->writeDBQuery( SO_DB_INFOKIND_CONSUMEDONKEYITEM, LPVOID(&DBInfo), sizeof(DBInfo) );

		// Client에 당나귀를 추가하라고 보낸다.
		SendFieldMsg( ON_ADD_FOLLOWER, SO_SFM_ONLYME, LPVOID(&stFollowerInfo) );

		return TRUE;
	}

	return FALSE;
}


