#include "SOMain.h"
#include "SODefine.h"
#include "SOWorld.h"
#include "SOPlayer.h"
#include "OnlineCommonStruct.h"

//����������������������������������������������������������������������������
// �������� ������.
//
//����������������������������������������������������������������������������
VOID	SOPlayer::RecvLeaveVillage()
{	
	UI16							uiCharFItemInfo[2];			// 0���� ĳ���� ��, 1���� ������ ��,
	UI16							uiCharUniIDs[ON_STATUS_AREA_XSIZE * ON_STATUS_AREA_YSIZE];	
	OnlineReceiveFieldItemInfo		FieldItemInfo[ON_STATUS_AREA_XSIZE * ON_STATUS_AREA_YSIZE];
	POINT							ptPlayerPos;	
	BOOL							bSuccess = FALSE;
	
	// ���� ������ �ִ°�?
	if( stAccountInfo.pVillage == NULL )								goto	END;

	// �켱 �ʿ��� ���� ��ǥ�� ã�´�. 
	// ������ ��ġ�� �������� �ʿ� ���� ��ǥ�� ���´�.
	ptPlayerPos		=	pMyWorld->clISOMap.SearchMap( stAccountInfo.pVillage->uiMapID,	
													stAccountInfo.pVillage->siVillageX, 
													stAccountInfo.pVillage->siVillageY, 
													ON_MAP_SI_CHAR, uiMoveableTileInfo );		

	// �������� ���� ���� ã�µ� �����ߴ��� �˻��Ѵ�.
	if( ( ptPlayerPos.x == -1 ) && ( ptPlayerPos.y == -1 ) )			goto	END;


	// ������ ������ ��ǥ�� �����Ѵ�.
	if( ChangeMap( stAccountInfo.pVillage->uiMapID, SI16( ptPlayerPos.x ), SI16( ptPlayerPos.y ) ) == FALSE )	goto	END;

	
	// �ʿ� ĳ���� ID�� �����Ѵ�.
	if( SetIDOnMap( 11 ) == FALSE )			
	{
		// �ʿ� ĳ���͸� �����ϴµ� �����ߴ�.

//		pMyWorld->clServer->WriteInfo("..\\SetIDOnMap Failed.txt", "8 [ %u ], [ %d ], [ %d ]", uiMapID, siX, siY );
		writeInfoToFile("SetIDOnMap Failed.txt", "8 [ %u ], [ %d ], [ %d ]", uiMapID, siX, siY );
		goto	END;
	}

	// ������ ���� ���´�.
	LeaveVillage();
	
	bSuccess	=	TRUE;

END:

	if( bSuccess == TRUE )
	{
		// �������� ���������� ���� �� �ִ�.

		uiCharFItemInfo[0]		=	pMyWorld->clISOMap.GetNearSectorPlayerList(uiMapID, siX, siY, uiCharUniIDs, ON_MAP_CHARID_FIELD);
		uiCharFItemInfo[1]		=	pMyWorld->clISOMap.GetNearSectorItemList(uiMapID, siX, siY, FieldItemInfo);		
		

		// ������ �� ������ ĳ���Ϳ� �������� ������ �����ش�.
		SendFieldMsg( ON_RESPONSE_LEAVE_VILLAGE, SO_SFM_ONLYME, LPVOID( ON_RET_LEAVE_VILLAGE_OK ), LPVOID( uiCharFItemInfo ), 
						LPVOID( uiCharUniIDs ), LPVOID( FieldItemInfo ) );
	}
	else
	{
		// �������� �����µ� �����ߴ�.

		SendFieldMsg(ON_RESPONSE_LEAVE_VILLAGE, SO_SFM_ONLYME, LPVOID(ON_RET_LEAVE_VILLAGE_NO));

	}


}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	�������� �������� ����Ʈ�� �䱸�Ѵ�.
//	���� ����	:	2002-10-01 ���� 9:49:03 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
VOID	SOPlayer::RecvListItem()
{
	SI32					siItemNum;
	OnlineReceivedItemInfo	ItemsInfo[ ON_VILLAGE_MAX_ITEM ];
	
	// �������� �Ǹ��ϴ� �������� ����� ���´�.
	siItemNum	=	stAccountInfo.pVillage->m_pMarket->GetSellItemList( ItemsInfo );			

	SendFieldMsg( ON_RESPONSE_LIST_ITEM, SO_SFM_ONLYME, LPVOID( siItemNum ), LPVOID( ItemsInfo ) );
}

//----------------------------------------------------------------------------------------------------------------
//	����		:	�������� ���.
//	���� ����	:	2002-02-27 ���� 2:03:04 - ������
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
//	����		:	�������� �Ǵ�.
//	���� ����	:	2002-03-02 ���� 11:02:18 - ������
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

	// ������ ���� �� �����۵��� �䱸�� ������ŭ �ִ��� �˻��Ѵ�.	
	pPlayerItem		=	IFollower.GetItem(pReqSellItemMsg->uiFollowerID, pReqSellItemMsg->uiItemID, pReqSellItemMsg->uiPosInInventory);

	// �� �뺴���Դ� �� �κ��丮 ��ġ�� �� �������� ����.
	if(pPlayerItem == NULL)		goto	END;

	// �ȷ��� �ϴ� ������ ���� ������ �ִ� �������� ���ų� ������ �˻��Ѵ�.
	if(pPlayerItem->uiQuantity < pReqSellItemMsg->uiItemQuantity)	goto	END;

	// �ѹ��� �� �� �ִ� ������ �����Ѵ�.
	if( pReqSellItemMsg->uiItemQuantity > ON_MARKET_MAXQUANTITY )	goto	END;


	// �ȷ��� �������� �� ���忡�� ����ϴ� ��ǰ���� �˻��Ѵ�.	
	if( stAccountInfo.pVillage->m_pMarket->FindItem( pReqSellItemMsg->uiItemID ) != NULL )
	{
		// ����ϴ� ��ǰ�̴�.

		// �÷��̾�� �� ������ ��ܰ��� ���踦 �����Ѵ�.			

		// ��� ������� �˾ƿ´�.
		if((stAccountInfo.pVillage->uiGuildID != 0) && (uiGuildID != 0) && (siClassInGuild != pMyWorld->clIGuildClass.GetJWJCode()))
		{
			if(stAccountInfo.pVillage->uiGuildID == uiGuildID)							
				siGuildRelation		=	ON_GUILD_RELATION_MINE;		// �� ������ ��ܿ��̴�.
			else if(pMyWorld->clIGuild.IsHostilityRelation(uiGuildID, stAccountInfo.pVillage->uiGuildID) == TRUE)	
				siGuildRelation		=	ON_GUILD_RELATION_ENEMY;	// �������� ������ ����̴�.
		}

		// �������� ������ ����� ���� �� �ִ�.		
		stAccountInfo.pVillage->m_pMarket->GetSellItemPrice( pReqSellItemMsg->uiItemID, siGuildRelation, &uiRealItemsPrice );
		uiRealItemsPrice	*=	pReqSellItemMsg->uiItemQuantity;

		// ������ �������� ���������ش�.	
		stAccountInfo.pVillage->m_pMarket->IncreaseItemQuantity( pReqSellItemMsg->uiItemID, pReqSellItemMsg->uiItemQuantity );
	}	
	else
	{
		// ������� �ʴ� ��ǰ�̴�.

		// �������� ������ ����� ���� ���Ѵ�.
		uiRealItemsPrice	=	pMyWorld->clItem.GetItem(pReqSellItemMsg->uiItemID)->m_siDBPrice  * pReqSellItemMsg->uiItemQuantity / 3;

		// ������ �������� ������������ �ʴ´�.(�ƹ��ϵ� ���� �ʴ´�.)
	}

	
	// �������� ���ش�.
	if(DelItem(pReqSellItemMsg->uiFollowerID, pReqSellItemMsg->uiItemID, pReqSellItemMsg->uiItemQuantity, pReqSellItemMsg->uiPosInInventory) == NULL)
		goto	END;


	// ���� ���� ���������ش�.
	IncreaseMoney(uiRealItemsPrice);
	// ������ ���� �ں��� �ø���.
	stAccountInfo.pVillage->IncreaseCapital(MONEY(uiRealItemsPrice*ON_CAPITAL_RATE_TRADE),ON_PROFIT_TRADE);

	// ���� �ſ뵵�� ���������ش�.
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
//	����	:	�������� �߰��Ҷ� ���� ���ѿ� �ɸ����� Ȯ���Ѵ�.
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
//	����	:	�����ʰ��� �뺴�� �ϳ��� �����Ͽ� �ȱ� ���� �����̸� TRUE�� �����Ѵ�.
//----------------------------------------------------------------------------------------------------------------
BOOL SOPlayer::IsOverWeight()
{
	return IFollower.IsOverWeight();
}


//----------------------------------------------------------------------------------------------------------------
//	����	:	������ ������ ���´�.
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
			// ���� ���࿡�� �÷��̾��� �ܾ��� ���´�.
//			pMyWorld->clDatabase->GetDBInfo(SO_DB_INFOKIND_BANKBALANCE, LPVOID(&DBBankInfo), LPVOID(&DBBankInfo));
//			siBalance	=	DBBankInfo.siBankCapital;

			// ����, ���� �ܾ�, �� ���� �����ش�.

			SendFieldMsg(ON_RESPONSE_BANK_INFO, SO_SFM_ONLYME, LPVOID(stAccountInfo.pVillage->pstBankData));
						//LPVOID(siBalance),
						//LPVOID(uiMoney));
		}	
		*/
	}
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	���࿡ ������ �Ѵ�.
//	���� ����	:	2002-10-18 ���� 4:27:42 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
VOID SOPlayer::RecvBankDeposit()
{
	OnReqBankDepositMsg		*pReqBankDepositMsg;
	BOOL					bSuccess  = FALSE;

	pReqBankDepositMsg	=	(OnReqBankDepositMsg*)cMsg;
	
	// ���� ���� �ȿ� �ִ��� �˻��Ѵ�.
	if( IsInVillage() == FALSE )																				goto	END;	

	// �Ա� �Ϸ��� ���� ��ȿ���� �˻��Ѵ�.
	if( ( pReqBankDepositMsg->siMoney <= 0 ) || ( EnoughMoney( pReqBankDepositMsg->siMoney ) == FALSE ) )		goto	END;

	bSuccess	=	stAccountInfo.pVillage->BankDeposit( this, pReqBankDepositMsg );

END:

	if( bSuccess == FALSE )
	{
		SendFieldMsg( ON_RESPONSE_BANK_DEPOSIT, SO_SFM_ONLYME, LPVOID( ON_RET_BANK_NO ) );	
	}

	/*
	// ���� �Ϸ��� �ݾ��� ���� ������ �ִ� ������ ���ų� �۾ƾ� �Ѵ�.
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
//	����	:	���࿡�� ����� �Ѵ�.
//----------------------------------------------------------------------------------------------------------------
VOID SOPlayer::RecvBankDrawing()
{
	OnReqBankDrawingMsg		*pReqBankDrawingMsg;	
	BOOL					bSuccess  = FALSE;
	
	pReqBankDrawingMsg	=	(OnReqBankDrawingMsg*)cMsg;

	// ���� ���� �ȿ� �ִ��� �˻��Ѵ�.
	if( IsInVillage() == FALSE )										goto	END;	

	// ��� �Ϸ��� ���� ��ȿ���� �˻��Ѵ�.
	if( pReqBankDrawingMsg->siMoney <= 0 )								goto	END;
	
	// �ش� �÷��̾ ������ �� �ִ� �ݾ��� �ʰ��Ǵ��� �˻��Ѵ�.
	if( GetMaxMoneyToGet() < pReqBankDrawingMsg->siMoney )				goto	END;

	bSuccess	=	stAccountInfo.pVillage->BankDrawing( this, pReqBankDrawingMsg );

END:
	
	if( bSuccess == FALSE )
	{
		SendFieldMsg( ON_RESPONSE_BANK_DRAWING, SO_SFM_ONLYME, LPVOID( ON_RET_BANK_NO ) );
	}

	/*
	//DB���� ���� ���������´�.	
	pReqBankDrawingMsg	=	(OnReqBankDrawingMsg*)cMsg;
	
	stAccountInfo.pVillage->BankDrawing(this,pReqBankDrawingMsg);

*/

}

/*
//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	���� �Խ��ǿ� ���ٴ� �޽����� �޾Ҵ�.
//��������������������������������������������������������������������������������������������������������������������������������������������������������������
VOID	SOPlayer::RecvEnterNoticeBoard()
{
	if(stAccountInfo.pVillage != NULL)
	{
		// ��� ���� ����� �޽����� Village Ŭ�������� �����ش�.		
		stAccountInfo.pVillage->EnterNoticeBoard(this);				
	}
	else
	{		
		// �Խ��ǿ� �� ���� ����.
		SendFieldMsg(ON_RESPONSE_ENTER_NOTICEBOARD, SO_SFM_ONLYME, LPVOID(ON_RET_INN_NO));
	}
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	������ ���ٴ� �޽����� �޾Ҵ�.
//��������������������������������������������������������������������������������������������������������������������������������������������������������������
VOID	SOPlayer::RecvEnterInn()
{
	// �������� ������ ����.
	SendFieldMsg(ON_RESPONSE_ENTER_INN, SO_SFM_ONLYME);

	// ������ ���ٰ� �����Ѵ�.
	//stAccountInfo.uiStatusInVillage			=	INVILLAGE_STATUS_INN;
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	�������� �����ٴ� �޽����� �޾Ҵ�.
//��������������������������������������������������������������������������������������������������������������������������������������������������������������
VOID	SOPlayer::RecvLeaveInn()
{
	BOOL					bSuccess = FALSE;	

	if(stAccountInfo.pVillage != NULL)
	{
		// ������ ����� �޽����� Village Ŭ�������� �����ش�.
		bSuccess	=	stAccountInfo.pVillage->LeaveInn(this);

		if(bSuccess == TRUE)
		{
			stAccountInfo.uiStatusInVillage	=	INVILLAGE_STATUS_NONE;			
		}
	}	

	if(bSuccess == FALSE)
	{
		// ��ȭ�濡�� ���� �� ����.
		SendFieldMsg(ON_RESPONSE_LEAVE_INN, SO_SFM_ONLYME, LPVOID(ON_RET_INN_NO));
	}
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	���� ä�� ���ǿ� ���ٴ� �޽����� �޾Ҵ�.
//��������������������������������������������������������������������������������������������������������������������������������������������������������������
VOID	SOPlayer::RecvEnterWaitRoom()
{
	BOOL					bSuccess = FALSE;	

	if(stAccountInfo.pVillage != NULL)	
	{
		// ��� ���� ����� �޽����� Village Ŭ�������� �����ش�.		
		bSuccess	=	stAccountInfo.pVillage->EnterWaitRoom(this);				
	}	

	if(bSuccess == FALSE)
	{
		// ��ȭ�濡 �� �� ����.
		SendFieldMsg(ON_RESPONSE_ENTER_VWR, SO_SFM_ONLYME,	LPVOID(ON_RET_INN_NO));
	}
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	�������� ��ȭ���� ����ٴ� �޽����� �޾Ҵ�.
//��������������������������������������������������������������������������������������������������������������������������������������������������������������
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

		// ��ȭ���� �����.		
		// ���������� ��ȭ���� ������ٸ� �� ��ȭ�濡 ������ ����.
		// ���� �޽����� Village Ŭ�������� �����ش�.
		bSuccess	=	stAccountInfo.pVillage->CreateChatRoomInInn(this, szChatRoomName);				
	}

	if(bSuccess == FALSE)
	{
		// ä�ù��� ����� ����.
		SendFieldMsg(ON_RESPONSE_INN_CREATECHATROOM, SO_SFM_ONLYME,
						LPVOID(ON_RET_INN_NO));
	}
}

//----------------------------------------------------------------------------------------------------------------
//	����	:	���� ��ȭ�濡 ���ٴ� �޽����� �޾Ҵ�.
//----------------------------------------------------------------------------------------------------------------
VOID	SOPlayer::RecvJoinInnChatRoom()
{
	BOOL						bSuccess = FALSE;
	OnReqJoinVilInnChatRoomMsg	*pJoinVilInnChatRoomMsg;

	pJoinVilInnChatRoomMsg	=	(OnReqJoinVilInnChatRoomMsg*)cMsg;


	if(stAccountInfo.pVillage != NULL)	
	{
		// ��� ���� ����� �޽����� Village Ŭ�������� �����ش�.		
		bSuccess	=	stAccountInfo.pVillage->JoinChatRoomInInn(pJoinVilInnChatRoomMsg->uiChatRoomID, this);				
	}	

	if(bSuccess == FALSE)
	{
		// ��ȭ�濡 �� �� ����.
		SendFieldMsg(ON_RESPONSE_VIL_INN_JOIN_CHAT_ROOM, SO_SFM_ONLYME,
						LPVOID(ON_RET_INN_NO));
	}
}

//----------------------------------------------------------------------------------------------------------------
//	����	:	�÷��̾ ���� ä�ù濡�� ������.
//----------------------------------------------------------------------------------------------------------------
VOID	SOPlayer::RecvLeaveInnChatRoom()
{
	BOOL	bSuccess = FALSE;
	if(stAccountInfo.pVillage != NULL)
	{
		// ������ ����� �޽����� Village Ŭ�������� �����ش�.
		bSuccess	=	stAccountInfo.pVillage->LeaveChatRoomInInn(this);
	}	

	if(bSuccess == FALSE)
	{
		// ��ȭ�濡�� ���� �� ����.
		SendFieldMsg(ON_RESPONSE_VIL_INN_LEAVE_CHAT_ROOM, SO_SFM_ONLYME,
								LPVOID(ON_RET_INN_NO));
	}
}
*/

//----------------------------------------------------------------------------------------------------------------
//	����	:	���� ���忡 ����.
//----------------------------------------------------------------------------------------------------------------
VOID	SOPlayer::RecvEnterMarket()
{
	BOOL	bSuccess = FALSE;

	if(stAccountInfo.pVillage != NULL)	
	{
		// ��� ���� ����� �޽����� Village Ŭ�������� �����ش�.		
		bSuccess	=	stAccountInfo.pVillage->m_pMarket->EnterPlayer( this );				

		if(bSuccess == TRUE)
		{
			// ���忡 �� �����̴�.
			stAccountInfo.uiStatusInVillage	=	INVILLAGE_STATUS_MARKET;

			SendFieldMsg( ON_RESPONSE_ENTER_MARKET, SO_SFM_ONLYME, LPVOID( ON_RET_MARKET_OK ) );
		}
	}	

	if(bSuccess == FALSE)
	{
		// ��ȭ�濡 �� �� ����.
		SendFieldMsg(ON_RESPONSE_ENTER_MARKET, SO_SFM_ONLYME, LPVOID(ON_RET_MARKET_NO));
	}
}

//----------------------------------------------------------------------------------------------------------------
//	����	:	���� ���忡�� ������.
//----------------------------------------------------------------------------------------------------------------
VOID	SOPlayer::RecvLeaveMarket()
{
	BOOL	bSuccess = FALSE;	

	if(stAccountInfo.pVillage != NULL)
	{
		// ������ ����� �޽����� Village Ŭ�������� �����ش�.
		bSuccess	=	stAccountInfo.pVillage->m_pMarket->LeavePlayer( this );

		if(bSuccess == TRUE)
		{
			stAccountInfo.uiStatusInVillage	=	INVILLAGE_STATUS_NONE;			

			SendFieldMsg(ON_RESPONSE_LEAVE_MARKET, SO_SFM_ONLYME, LPVOID(ON_RET_MARKET_OK));
		}
	}	

	if(bSuccess == FALSE)
	{
		// ��ȭ�濡�� ���� �� ����.
		SendFieldMsg(ON_RESPONSE_LEAVE_MARKET, SO_SFM_ONLYME, LPVOID(ON_RET_INN_NO));
	}
}



//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	���ο� ����(����)�� ����ٴ� �޽����� �޾Ҵ�.
//	���� ����	:	2002-10-18 ���� 3:46:18 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������
VOID	SOPlayer::RecvOpenSavingAccount()
{	
	OnReqBankOpenSavingAccountMsg	*pOpenSavingAccountMsg;	
	DWORD							dwResponse = ON_RET_BANK_NO;

	pOpenSavingAccountMsg	=	(OnReqBankOpenSavingAccountMsg*)cMsg;

	// ���� ���� �ȿ� �ִ��� �˻��Ѵ�.
	if( IsInVillage() == FALSE )																											goto	END;

	// ����ó��.....( ���� �������� �ʴ� �κ��̴�. )
	if( pOpenSavingAccountMsg->uiKind == 4 )																								goto	END;

	// �ſ� �����̶�� ���� �ѵ� Ȯ��..
	if( pOpenSavingAccountMsg->uiKind == ON_BANK_LOAN_TYPE ) 
	{	
		// ������ �� �ִ� �ѵ����� �˻��Ѵ�.
		if( pOpenSavingAccountMsg->siFirstDepositMoney > uiMaxLoan )									 									goto	END;				
		if( pMyWorld->clDatabase->GetDBInfo( SO_DB_INFOKIND_CHECKLOAN, LPVOID( this ), NULL ) == FALSE)										goto	END;		
		if( pOpenSavingAccountMsg->siFirstDepositMoney > stPropertyData.siTotalProperty )													goto	END;
	}

	// ó�� �Ա� �Ϸ��� ���� ��ȿ���� �˻��Ѵ�.
	if( ( pOpenSavingAccountMsg->siFirstDepositMoney <= 0 ) || ( EnoughMoney( pOpenSavingAccountMsg->siFirstDepositMoney ) == FALSE ) )		goto	END;
		
	// �ִ� ���� �� �ִ� ������ ������ �ʰ��ߴ��� �˻��Ѵ�.
	if( GetBankBootNum() >= ON_MAX_BANKBOOK_NUM )																							goto	END;

	dwResponse	=	stAccountInfo.pVillage->OpenSavingAccount( this, pOpenSavingAccountMsg, NULL );

END:

	if( dwResponse != ON_RET_OK )
	{
		// ���¸� ����µ� �����ߴ�.
		SendFieldMsg( ON_RESPONSE_BANK_OPENSAVINGACCOUNT, SO_SFM_ONLYME, LPVOID( dwResponse ) );
		
	}


	/*
	OnReqBankOpenSavingAccountMsg*	pOpenSavingAccountMsg;	
	SI32							siRet	=	ON_RET_BANK_OK;

	 pOpenSavingAccountMsg	=	(OnReqBankOpenSavingAccountMsg*)cMsg;


	 if( pOpenSavingAccountMsg->uiKind  == 3 ) // �ſ� �����̶�� ���� �ѵ� Ȯ��..
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
				 // ������ ��û�� ������ ���࿡�� �����Ѵ�.
				 // ������¸� ���������� ������ ���࿡�� ���� ����ڿ��� �޽����� �����ش�.
				 siRet	=	stAccountInfo.pVillage->OpenSavingAccount(this, pOpenSavingAccountMsg,NULL);
			 }
		 }
	 }

	if(siRet != ON_RET_BANK_OK)
	{		
		// ������¸� ���� �� ����.
		SendFieldMsg(ON_RESPONSE_BANK_OPENSAVINGACCOUNT, SO_SFM_ONLYME, LPVOID(siRet));
	}
	*/
}


//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	���� ����Ʈ�� �޶�� �䱸�� �޾Ҵ�.
//��������������������������������������������������������������������������������������������������������������������������������������������������������������
VOID	SOPlayer::RecvBankbookList()
{
	DBIKBankBookList			BankBookList;
	DBIKVillageBankBookInfo		VillageBankBookInfo;
	BOOL						bGetBankBookListSuccess = TRUE;

	// �켱 �� ĳ���Ͱ� ������ ��� ������ �� �־�� �Ѵ�.
	if(stAccountInfo.pVillage != NULL)
	{
		// DB���� ������ ������ ���´�.	
		VillageBankBookInfo.pPlayer			=	this;
		VillageBankBookInfo.uiVillageCode	=	stAccountInfo.pVillage->uiVillageCode;
		VillageBankBookInfo.uiVillageCode	=	stAccountInfo.pVillage->uiVillageCode;

		memset(&BankBookList, 0, sizeof(BankBookList));
		if(pMyWorld->clDatabase->GetDBInfo(SO_DB_INFOKIND_MYBANKBOOK, LPVOID(&VillageBankBookInfo), LPVOID(&BankBookList)) == TRUE)
		{
			// ������ ������ ���������� ���Դ�.			
			SendFieldMsg(ON_RESPONSE_BANK_BANKBOOK_LIST, SO_SFM_ONLYME, LPVOID(ON_RET_BANK_OK), LPVOID(&BankBookList));
			
		}
		else
			bGetBankBookListSuccess	=	FALSE;
	}
	else
		bGetBankBookListSuccess	=	FALSE;

	if(bGetBankBookListSuccess == FALSE)
	{
		// ������ ������ �����µ� �����ߴ�.
		SendFieldMsg(ON_RESPONSE_BANK_BANKBOOK_LIST, SO_SFM_ONLYME, LPVOID(ON_RET_BANK_NO));
	}
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	���� ����Ʈ�� ���� �޶�� �䱸�� �޾Ҵ�.
//��������������������������������������������������������������������������������������������������������������������������������������������������������������
VOID	SOPlayer::RecvBankbookListCount()
{
	MONEY		Temp;

	Temp = m_MoneySaving;
	SendFieldMsg(ON_RESPONSE_BANK_BANKBOOK_LISTCOUNT, SO_SFM_ONLYME, LPVOID(ON_RET_OK), LPVOID(&Temp));
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	�㺸�� �ñ�ٴ� �䱸�� �޾Ҵ�.
//��������������������������������������������������������������������������������������������������������������������������������������������������������������
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

	// �㺸 ���忡 ���� ������ ���´�.
	pEconomyHeader = pMyWorld->clOnlineEconomy.GetHeader(3);

	if(stAccountInfo.pVillage != NULL && pEconomyHeader != NULL)
	{
		stVillageCollateral.pPlayer			=	this;
		stVillageCollateral.uiVillageCode	=	stAccountInfo.pVillage->uiVillageCode;
		stVillageCollateral.uiVillageCode	=	stAccountInfo.pVillage->uiVillageCode;
		stVillageCollateral.siMoney			=	pCollateralLoan->siMoney;
		stVillageCollateral.uiMonth			=	pCollateralLoan->uiMonth;
		stVillageCollateral.pCollateralList	=	&stCollateralList;

		// ���� ���� �ѵ��� �Ѿ����� Ȯ���Ѵ�.
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
			
			// ���� �׼� �ѵ��� �Ѿ����� Ȯ���Ѵ�.
			for(int i=0	;	i < (SI32)pCollateralLoan->dwItemNum ; i++)
			{
				if( stCollateralList.uiNum[i] > 1000 )
				{
					siRet = ON_RET_BANK_NO;
					break;
				}
				MaxLoan		+=	(pMyWorld->clItem.GetItem(stCollateralList.uiITemCode[i])->m_siDBPrice) * ( stCollateralList.uiNum[i] ) * 7 / 10;
			}			
			
			if(stVillageCollateral.siMoney > MaxLoan )   //1000 ��� �����ѵ��� �;� �Ѵ�.
			{				
				siRet = ON_RET_BANK_NO;
			}
		}
		//���� db�� �����Ѵ�. DB���� ���� ������ �ִ����� Ȯ�� 

		if(siRet == ON_RET_BANK_OK && stAccountInfo.pVillage != NULL)
		{	
			// DB�� �㺸�� ���� �⺻ ������ �ְ� Ȯ���Ѵ�.
			pMyWorld->clDatabase->GetDBInfo(SO_DB_INFOKIND_COLLATERAL_1, LPVOID(&stVillageCollateral), LPVOID(&siRet));				
			if(siRet == 1)
			{
				// 4�� ����(�㺸 ����)�� ����� �������� �����Ѵ�.				
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
					SendFieldMsg(ON_RESPONSE_BANK_COLLATERAL, SO_SFM_ONLYME, LPVOID(siRet),LPVOID(&stAccountNum));  // pParam2�� ���� ��ȣ.
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


//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	�㺸 ����Ʈ�� �޶�� �䱸�� �޾Ҵ�.
//��������������������������������������������������������������������������������������������������������������������������������������������������������������
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
			// ������ ������ ���������� ���Դ�.			
			SendFieldMsg(ON_RESPONSE_BANK_COLLATERALLIST, SO_SFM_ONLYME, LPVOID(ON_RET_BANK_OK), LPVOID(&stCollateralList));
		}
		else
			bGetCollateralListSuccess	=	FALSE;
	}
	else
		bGetCollateralListSuccess	=	FALSE;

	if(bGetCollateralListSuccess == FALSE)
	{		
		// �㺸 ������ �����µ� �����ߴ�.
		SendFieldMsg(ON_RESPONSE_BANK_COLLATERALLIST, SO_SFM_ONLYME, LPVOID(ON_RET_BANK_NO));
	}
}


//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	�㺸�� ���� �޶�� �䱸�� �޾Ҵ�.
//��������������������������������������������������������������������������������������������������������������������������������������������������������������
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
		// ������ ������ ���������� ���Դ�.			
		SendFieldMsg(ON_RESPONSE_BANK_TAKECOLLATERAL, SO_SFM_ONLYME, LPVOID(ON_RET_BANK_OK));		
	}
	if(siResult == ON_RET_BANK_NO)
	{
		
		SendFieldMsg(ON_RESPONSE_BANK_TAKECOLLATERAL, SO_SFM_ONLYME, LPVOID(ON_RET_BANK_NO));
	}
}

/*
//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	�Խù��� ����Ѵ�.
//��������������������������������������������������������������������������������������������������������������������������������������������������������������
VOID	SOPlayer::RecvRegistNotice()
{
	BOOL					bSuccessRegistNotice  = FALSE;
	CHAR					szTitle[4096], szNotice[10240];
	CHAR					*pszTitle;
	CHAR					*pszNotice;
	OnReqRegistNoticeMsg	*pRegistNoticeMsg;
	
	// ���� ����ϴµ� �����ߴٰ� �˷��ش�.
	SendFieldMsg(ON_RESPONSE_REGIST_NOTICE, SO_SFM_ONLYME, LPVOID(ON_RET_INN_NO));
	return;

	pRegistNoticeMsg	=	(OnReqRegistNoticeMsg*)cMsg;

	if(stAccountInfo.pVillage != NULL)
	{
		pszTitle		=	(CHAR*)pRegistNoticeMsg + sizeof(OnReqRegistNoticeMsg);
		pszNotice		=	(CHAR*)pszTitle + pRegistNoticeMsg->uiTitleStringLength;

		// Title�� ����.
		strncpy(szTitle, pszTitle, pRegistNoticeMsg->uiTitleStringLength);
		szTitle[pRegistNoticeMsg->uiTitleStringLength]		=	NULL;

		// Notice�� ����.
		strncpy(szNotice, pszNotice, pRegistNoticeMsg->uiNoticeStringLength);
		szNotice[pRegistNoticeMsg->uiNoticeStringLength]	=	NULL;

		// �� �� ���� ���ڰ� �ִ� �� Ȯ���Ѵ�.
		strtok(szTitle, ON_FORBIDDEN_CHAR);
		strtok(szNotice, ON_FORBIDDEN_CHAR);
		if(szTitle[0]	==	'\'')	szTitle[0]	=	20;
		if(szNotice[0]	==	'\'')	szNotice[0]	=	20;
		
		bSuccessRegistNotice	=	stAccountInfo.pVillage->RegistNotice(this, szTitle, szNotice);
	}
	
	if(bSuccessRegistNotice == FALSE)
	{
		// ���� ����ϴµ� �����ߴٰ� �˷��ش�.
		SendFieldMsg(ON_RESPONSE_REGIST_NOTICE, SO_SFM_ONLYME, LPVOID(ON_RET_INN_NO));
	}

}


//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	�Խ����� ���� �����Ѵ�.
//��������������������������������������������������������������������������������������������������������������������������������������������������������������
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
		// ���� ����ϴµ� �����ߴٰ� �˷��ش�.
		SendFieldMsg(ON_RESPONSE_DELETE_NOTICE, SO_SFM_ONLYME, LPVOID(ON_RET_INN_NO));
	}
}
		

//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	�Խù��� ����Ʈ�� ���´�.
//��������������������������������������������������������������������������������������������������������������������������������������������������������������
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
		// ���� ����Ʈ�� �����µ� �����ߴ�.
		SendFieldMsg(ON_RESPONSE_NOTICEBOARD_NOTICELIST, SO_SFM_ONLYME, LPVOID(ON_RET_INN_NO));
	}
}



//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	�Խù��� ����.
//��������������������������������������������������������������������������������������������������������������������������������������������������������������
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
		// ���� �����µ� �����ߴ�.
		SendFieldMsg(ON_RESPONSE_VIEW_NOTICE, SO_SFM_ONLYME, LPVOID(ON_RET_INN_NO));
	}
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	�Խù��� �����Ѵ�.
//	���� ����	:	2002-04-15 ���� 2:30:17 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
VOID	SOPlayer::RecvModifyNotice()
{
	BOOL					bSuccess	=	FALSE;
	OnReqModifyNoticeMsg	*pModifyNoticeMsg;
	CHAR					*pszNoticeTitleString;
	CHAR					*pszNoticeContentString;
	CHAR					szNoticeTitle[256];
	CHAR					szNoticeContent[4096];

	// ���� ����ϴµ� �����ߴٰ� �˷��ش�.
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
		// ���� �����µ� �����ߴ�.
		SendFieldMsg(ON_RESPONSE_MODIFY_NOTICE, SO_SFM_ONLYME, LPVOID(ON_RET_INN_NO));
	}

}	
*/




//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	�뺴 List�� ��û�Ѵ�.
//��������������������������������������������������������������������������������������������������������������������������������������������������������������
VOID	SOPlayer::RecvBarrackGetSolderList()
{
	if(stAccountInfo.pVillage != NULL)
	{

		if( stAccountInfo.pVillage->m_Barrack != NULL )
		{

			// ��� ���� ����� �޽����� Village Ŭ�������� �����ش�.		
			stAccountInfo.pVillage->m_Barrack->BarrackGetSolderList(this);				

		}
		else
		{
			// �뺴 ������ List�� �ټ� ����.
			SendFieldMsg(ON_RESPONSE_BARRACK_GETSOLDERLIST, SO_SFM_ONLYME, LPVOID(ON_RET_NO));
		}

	}
	else
	{		
		// �뺴 ������ List�� �ټ� ����.
		SendFieldMsg(ON_RESPONSE_BARRACK_GETSOLDERLIST, SO_SFM_ONLYME, LPVOID(ON_RET_NO));
	}
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	�뺴�� ����Ѵ�.
//��������������������������������������������������������������������������������������������������������������������������������������������������������������
VOID	SOPlayer::RecvBarrackBuySolder()
{
	OnReqBarrackBuySolder		*pOnReqBarrackBuySolder;

	pOnReqBarrackBuySolder	=	(OnReqBarrackBuySolder*)cMsg;

	if(stAccountInfo.pVillage != NULL)
	{
		if( stAccountInfo.pVillage->m_Barrack != NULL )
		{

			// �뺴 ��뿡 ���� ������ Village Ŭ�������� �����ش�.
			if(stAccountInfo.pVillage->m_Barrack->BarrackBuySolder(this, pOnReqBarrackBuySolder->dwBuySolderID, (BYTE*)pOnReqBarrackBuySolder->szName) == FALSE)
			{
				// ���н� Message �� ������ �ش�. ������������ ���ο��� Message�� ������.
				SendFieldMsg(ON_RESPONSE_BARRACK_BUYSOLDER, SO_SFM_ONLYME, LPVOID(ON_RET_NO));
			}
		}
		else
		{
			// �뺴 �������� �뺴�� ����Ҽ� ����.
			SendFieldMsg(ON_RESPONSE_BARRACK_BUYSOLDER, SO_SFM_ONLYME, LPVOID(ON_RET_NO));
		}

	}
	else
	{		
		// �뺴 �������� �뺴�� ����Ҽ� ����.
		SendFieldMsg(ON_RESPONSE_BARRACK_BUYSOLDER, SO_SFM_ONLYME, LPVOID(ON_RET_NO));
	}
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	�ذ��� �뺴 List�� ��û�Ѵ�.
//��������������������������������������������������������������������������������������������������������������������������������������������������������������
VOID	SOPlayer::RecvBarrackGetDismissalSolderList()
{
	if(stAccountInfo.pVillage != NULL)
	{
		if( stAccountInfo.pVillage->m_Barrack != NULL )
		{

			// ��� ���� ����� �޽����� Village Ŭ�������� �����ش�.		
			stAccountInfo.pVillage->m_Barrack->BarrackGetDismissalSolderList(this);				

		}
		else
		{

			// �뺴 ������ List�� �ټ� ����.
			SendFieldMsg(ON_RESPONSE_BARRACK_GETDISMISSALSOLDERLIST, SO_SFM_ONLYME, LPVOID(ON_RET_NO));

		}
	}
	else
	{		
		// �뺴 ������ List�� �ټ� ����.
		SendFieldMsg(ON_RESPONSE_BARRACK_GETDISMISSALSOLDERLIST, SO_SFM_ONLYME, LPVOID(ON_RET_NO));
	}
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	�뺴�� �ذ��Ѵ�.
//��������������������������������������������������������������������������������������������������������������������������������������������������������������
VOID	SOPlayer::RecvBarrackDismissalSolder()
{
	OnReqBarrackDismissalSolder		*pOnReqBarrackDismissalSolder;

	pOnReqBarrackDismissalSolder	=	(OnReqBarrackDismissalSolder*)cMsg;

	if(stAccountInfo.pVillage != NULL)
	{
		if( stAccountInfo.pVillage->m_Barrack != NULL )
		{

			// �뺴 �ذ� ���� ������ Village Ŭ�������� �����ش�.
			if(stAccountInfo.pVillage->m_Barrack->BarrackDismissalSolder(this, pOnReqBarrackDismissalSolder->dwSlot) == FALSE)
			{
				// ���н� Message �� ������ �ش�. ������������ ���ο��� Message�� ������.
				SendFieldMsg(ON_RESPONSE_BARRACK_DISMISSALSOLDER, SO_SFM_ONLYME, LPVOID(ON_RET_NO));
			}
		}
		else
		{
			// �뺴 �������� �뺴�� �ذ��Ҽ� ����.
			SendFieldMsg(ON_RESPONSE_BARRACK_DISMISSALSOLDER, SO_SFM_ONLYME, LPVOID(ON_RET_NO));
		}
		
	}
	else
	{		
		// �뺴 �������� �뺴�� �ذ��Ҽ� ����.
		SendFieldMsg(ON_RESPONSE_BARRACK_DISMISSALSOLDER, SO_SFM_ONLYME, LPVOID(ON_RET_NO));
	}
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	����뺴 List�� ��û�Ѵ�.
//��������������������������������������������������������������������������������������������������������������������������������������������������������������
VOID	SOPlayer::RecvGBarrackGetSolderList()
{
	if(stAccountInfo.pVillage != NULL)
	{
		if( stAccountInfo.pVillage->m_Barrack != NULL )
		{

			// ��� ���� ����� �޽����� Village Ŭ�������� �����ش�.		
			stAccountInfo.pVillage->m_Barrack->GBarrackGetSolderList(this);				
		}
		else
		{
			// ����뺴 ������ List�� �ټ� ����.
			SendFieldMsg(ON_RESPONSE_GBARRACK_GETSOLDERLIST, SO_SFM_ONLYME, LPVOID(ON_RET_NO));
		}
	}
	else
	{		
		// ����뺴 ������ List�� �ټ� ����.
		SendFieldMsg(ON_RESPONSE_GBARRACK_GETSOLDERLIST, SO_SFM_ONLYME, LPVOID(ON_RET_NO));
	}
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	����뺴�� ����Ѵ�.
//��������������������������������������������������������������������������������������������������������������������������������������������������������������
VOID	SOPlayer::RecvGBarrackBuySolder()
{
	OnReqGBarrackBuySolder		*pOnReqGBarrackBuySolder;

	pOnReqGBarrackBuySolder	=	(OnReqGBarrackBuySolder*)cMsg;

	if(stAccountInfo.pVillage != NULL)
	{
		if( stAccountInfo.pVillage->m_Barrack != NULL )
		{

			// ����뺴 ��뿡 ���� ������ Village Ŭ�������� �����ش�.
			if(stAccountInfo.pVillage->m_Barrack->GBarrackBuySolder(this, pOnReqGBarrackBuySolder->dwBuySolderID, (BYTE*)pOnReqGBarrackBuySolder->szName) == FALSE)
			{
				// ���н� Message �� ������ �ش�. ������������ ���ο��� Message�� ������.
				SendFieldMsg(ON_RESPONSE_GBARRACK_BUYSOLDER, SO_SFM_ONLYME, LPVOID(ON_RET_NO));
			}

		}
		else
		{
			// ����뺴 �������� �뺴�� ����Ҽ� ����.
			SendFieldMsg(ON_RESPONSE_GBARRACK_BUYSOLDER, SO_SFM_ONLYME, LPVOID(ON_RET_NO));
		}
	}
	else
	{		
		// ����뺴 �������� �뺴�� ����Ҽ� ����.
		SendFieldMsg(ON_RESPONSE_GBARRACK_BUYSOLDER, SO_SFM_ONLYME, LPVOID(ON_RET_NO));
	}
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	������ ����뺴 List�� ��û�Ѵ�.
//��������������������������������������������������������������������������������������������������������������������������������������������������������������
VOID	SOPlayer::RecvGBarrackGetChangeJobSolderList()
{
	if(stAccountInfo.pVillage != NULL)
	{
		if( stAccountInfo.pVillage->m_Barrack != NULL )
		{

			// ��� ���� ����� �޽����� Village Ŭ�������� �����ش�.		
			stAccountInfo.pVillage->m_Barrack->GBarrackGetChangeJobSolderList(this);				
		}
		else
		{	
			// ����뺴 �������� List�� �ټ� ����.
			SendFieldMsg(ON_RESPONSE_GBARRACK_GETCHANGEJOBSOLDERLIST, SO_SFM_ONLYME, LPVOID(ON_RET_NO));
		}
	}
	else
	{		
		// ����뺴 �������� List�� �ټ� ����.
		SendFieldMsg(ON_RESPONSE_GBARRACK_GETCHANGEJOBSOLDERLIST, SO_SFM_ONLYME, LPVOID(ON_RET_NO));
	}
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	����뺴�� �����Ѵ�.
//��������������������������������������������������������������������������������������������������������������������������������������������������������������
VOID	SOPlayer::RecvGBarrackChangeJobSolder()
{
	OnReqGBarrackChangeJobSolder		*pOnReqGBarrackChangeJobSolder;

	pOnReqGBarrackChangeJobSolder	=	(OnReqGBarrackChangeJobSolder*)cMsg;

	if(stAccountInfo.pVillage != NULL)
	{
		if( stAccountInfo.pVillage->m_Barrack != NULL )
		{

			// ����뺴 ������ ���� ������ Village Ŭ�������� �����ش�.
			if(stAccountInfo.pVillage->m_Barrack->GBarrackChangeJobSolder(this, pOnReqGBarrackChangeJobSolder->dwChangeJobSlot, pOnReqGBarrackChangeJobSolder->dwChangeJobKind) == FALSE)
			{
				// ���н� Message �� ������ �ش�. ������������ ���ο��� Message�� ������.
				SendFieldMsg(ON_RESPONSE_GBARRACK_CHANGEJOBSOLDER, SO_SFM_ONLYME, LPVOID(ON_RET_NO));
			}
		}
		else
		{
			// ����뺴 �������� �뺴�� �����Ҽ� ����.
			SendFieldMsg(ON_RESPONSE_GBARRACK_CHANGEJOBSOLDER, SO_SFM_ONLYME, LPVOID(ON_RET_NO));
		}
	}
	else
	{		
		// ����뺴 �������� �뺴�� �����Ҽ� ����.
		SendFieldMsg(ON_RESPONSE_GBARRACK_CHANGEJOBSOLDER, SO_SFM_ONLYME, LPVOID(ON_RET_NO));
	}
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	�ذ��� ����뺴 List�� ��û�Ѵ�.
//��������������������������������������������������������������������������������������������������������������������������������������������������������������
VOID	SOPlayer::RecvGBarrackGetDismissalSolderList()
{
	if(stAccountInfo.pVillage != NULL)
	{
		if( stAccountInfo.pVillage->m_Barrack != NULL )
		{

			// ��� ���� ����� �޽����� Village Ŭ�������� �����ش�.		
			stAccountInfo.pVillage->m_Barrack->GBarrackGetDismissalSolderList(this);				
		}
		else
		{
			// ����뺴 ������ List�� �ټ� ����.
			SendFieldMsg(ON_RESPONSE_GBARRACK_GETDISMISSALSOLDERLIST, SO_SFM_ONLYME, LPVOID(ON_RET_NO));
		}
	}
	else
	{		
		// ����뺴 ������ List�� �ټ� ����.
		SendFieldMsg(ON_RESPONSE_GBARRACK_GETDISMISSALSOLDERLIST, SO_SFM_ONLYME, LPVOID(ON_RET_NO));
	}
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	����뺴�� �ذ��Ѵ�.
//��������������������������������������������������������������������������������������������������������������������������������������������������������������
VOID	SOPlayer::RecvGBarrackDismissalSolder()
{
	OnReqGBarrackDismissalSolder		*pOnReqGBarrackDismissalSolder;

	pOnReqGBarrackDismissalSolder	=	(OnReqGBarrackDismissalSolder*)cMsg;

	if(stAccountInfo.pVillage != NULL)
	{
		if( stAccountInfo.pVillage->m_Barrack != NULL )
		{
			// ����뺴 �ذ� ���� ������ Village Ŭ�������� �����ش�.
			if(stAccountInfo.pVillage->m_Barrack->GBarrackDismissalSolder(this, pOnReqGBarrackDismissalSolder->dwSlot) == FALSE)
			{
				// ���н� Message �� ������ �ش�. ������������ ���ο��� Message�� ������.
				SendFieldMsg(ON_RESPONSE_GBARRACK_DISMISSALSOLDER, SO_SFM_ONLYME, LPVOID(ON_RET_NO));
			}
		}
		else
		{
			// ����뺴 �������� �뺴�� �ذ��Ҽ� ����.
			SendFieldMsg(ON_RESPONSE_GBARRACK_DISMISSALSOLDER, SO_SFM_ONLYME, LPVOID(ON_RET_NO));
		}
	}
	else
	{		
		// ����뺴 �������� �뺴�� �ذ��Ҽ� ����.
		SendFieldMsg(ON_RESPONSE_GBARRACK_DISMISSALSOLDER, SO_SFM_ONLYME, LPVOID(ON_RET_NO));
	}
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	2�� ��� ��� List�� ��û
//��������������������������������������������������������������������������������������������������������������������������������������������������������������
VOID	SOPlayer::RecvGBarrackGetGeneralExList()
{
	if(stAccountInfo.pVillage != NULL)
	{
		if( stAccountInfo.pVillage->m_Barrack != NULL )
		{
			// ��� ���� ����� �޽����� Village Ŭ�������� �����ش�.		
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

//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	2�� ��� ��� ��û
//��������������������������������������������������������������������������������������������������������������������������������������������������������������
VOID	SOPlayer::RecvGBarrackBuyGeneralEx()
{
	OnReqGBarrackBuySolder		*pOnReqGBarrackBuySolder;

	pOnReqGBarrackBuySolder	= (OnReqGBarrackBuySolder*)cMsg;

	if(stAccountInfo.pVillage != NULL)
	{
		if( stAccountInfo.pVillage->m_Barrack != NULL )
		{
			// 2�� ��� ��뿡 ���� ������ Village Ŭ�������� �����ش�.
			if(stAccountInfo.pVillage->m_Barrack->GBarrackBuyGeneralEx(this, pOnReqGBarrackBuySolder->dwBuySolderID, (BYTE*)pOnReqGBarrackBuySolder->szName) == FALSE)
			{
				// ���н� Message �� ������ �ش�. ������������ ���ο��� Message�� ������.
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

//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	2�� ��� ���� List ��û 
//��������������������������������������������������������������������������������������������������������������������������������������������������������������
VOID	SOPlayer::RecvGBarrackGetChangeGeneralExList()
{
	if(stAccountInfo.pVillage != NULL)
	{
		if( stAccountInfo.pVillage->m_Barrack != NULL )
		{
			// ��� ���� ����� �޽����� Village Ŭ�������� �����ش�.		
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

//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	2�� ��� ���� ��û
//��������������������������������������������������������������������������������������������������������������������������������������������������������������
VOID	SOPlayer::RecvGBarrackChangeGeneralEx()
{
	OnReqGBarrackChangeJobSolder*		pOnReqGBarrackChangeJobSolder;

	pOnReqGBarrackChangeJobSolder	=	(OnReqGBarrackChangeJobSolder*)cMsg;

	if(stAccountInfo.pVillage != NULL)
	{
		if( stAccountInfo.pVillage->m_Barrack != NULL )
		{

			// ����뺴 ������ ���� ������ Village Ŭ�������� �����ش�.
			if(stAccountInfo.pVillage->m_Barrack->GBarrackChangeGeneralEx(this, pOnReqGBarrackChangeJobSolder->dwChangeJobSlot, pOnReqGBarrackChangeJobSolder->dwChangeJobKind) == FALSE)
			{
				// ���н� Message �� ������ �ش�. ������������ ���ο��� Message�� ������.
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

//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	2�� ��� �ذ� List ��û
//��������������������������������������������������������������������������������������������������������������������������������������������������������������
VOID	SOPlayer::RecvGBarrackGetDismissalGeneralExList()
{
	if(stAccountInfo.pVillage != NULL)
	{
		if( stAccountInfo.pVillage->m_Barrack != NULL )
		{
			// ��� ���� ����� �޽����� Village Ŭ�������� �����ش�.		
			stAccountInfo.pVillage->m_Barrack->GBarrackGetDismissalGeneralExList(this);				
		}
		else
		{
			// ����뺴 ������ List�� �ټ� ����.
			SendFieldMsg(ON_RESPONSE_GBARRACK_GETDISMISSAL_GENERAL_LIST, SO_SFM_ONLYME, LPVOID(ON_RET_NO));
		}
	}
	else
	{		
		// ����뺴 ������ List�� �ټ� ����.
		SendFieldMsg(ON_RESPONSE_GBARRACK_GETDISMISSAL_GENERAL_LIST, SO_SFM_ONLYME, LPVOID(ON_RET_NO));
	}
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	2�� ��� �ذ� ��û
//��������������������������������������������������������������������������������������������������������������������������������������������������������������
VOID	SOPlayer::RecvGBarrackDismissalGeneralEx()
{
	OnReqGBarrackDismissalSolder*			pOnReqGBarrackDismissalSolder;

	pOnReqGBarrackDismissalSolder	=		(OnReqGBarrackDismissalSolder*)cMsg;

	if(stAccountInfo.pVillage != NULL)
	{
		if( stAccountInfo.pVillage->m_Barrack != NULL )
		{
			// ����뺴 �ذ� ���� ������ Village Ŭ�������� �����ش�.
			if(stAccountInfo.pVillage->m_Barrack->GBarrackSellDismissalGeneralEx(this, pOnReqGBarrackDismissalSolder->dwSlot) == FALSE)
			{
				// ���н� Message �� ������ �ش�. ������������ ���ο��� Message�� ������.
				SendFieldMsg(ON_RESPONSE_GBARRACK_DISMISSAL_GENERAL, SO_SFM_ONLYME, LPVOID(ON_RET_NO));
			}
		}
		else
		{
			// ����뺴 �������� �뺴�� �ذ��Ҽ� ����.
			SendFieldMsg(ON_RESPONSE_GBARRACK_DISMISSAL_GENERAL, SO_SFM_ONLYME, LPVOID(ON_RET_NO));
		}
	}
	else
	{		
		// ����뺴 �������� �뺴�� �ذ��Ҽ� ����.
		SendFieldMsg(ON_RESPONSE_GBARRACK_DISMISSAL_GENERAL, SO_SFM_ONLYME, LPVOID(ON_RET_NO));
	}
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	���� List�� ��û�Ѵ�.
//��������������������������������������������������������������������������������������������������������������������������������������������������������������
VOID	SOPlayer::RecvMBarrackGetMonsterList()
{
	if( stAccountInfo.pVillage != NULL )
	{
		if( stAccountInfo.pVillage->m_Barrack != NULL )
		{

			// ��� ���� ����� �޽����� Village Ŭ�������� �����ش�.		
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


//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	���͸� ����Ѵ�.
//��������������������������������������������������������������������������������������������������������������������������������������������������������������
VOID	SOPlayer::RecvMBarrackBuyMonster()
{
	OnReqMBarrackBuyMonster		*pOnReqMBarrackBuyMonster;

	pOnReqMBarrackBuyMonster	=	(OnReqMBarrackBuyMonster*)cMsg;

	if( stAccountInfo.pVillage != NULL )
	{
		if( stAccountInfo.pVillage->m_Barrack != NULL )
		{

			// ���� ��뿡 ���� ������ Village Ŭ�������� �����ش�.
			if( stAccountInfo.pVillage->m_Barrack->MBarrackBuyMonster(this, pOnReqMBarrackBuyMonster->dwBuyMonsterID, (BYTE*)pOnReqMBarrackBuyMonster->szName) == FALSE )
			{
				// ���н� Message �� ������ �ش�. ������������ ���ο��� Message�� ������.
				SendFieldMsg( ON_RESPONSE_MBARRACK_BUYMONSTER, SO_SFM_ONLYME, LPVOID(ON_RET_NO) );
			}
		}
		else
		{
			// ���� �������� ���͸� ����Ҽ� ����.
			SendFieldMsg( ON_RESPONSE_MBARRACK_BUYMONSTER, SO_SFM_ONLYME, LPVOID(ON_RET_NO) );
		}

	}
	else
	{		
		// ���� �������� ���͸� ����Ҽ� ����.
		SendFieldMsg( ON_RESPONSE_MBARRACK_BUYMONSTER, SO_SFM_ONLYME, LPVOID(ON_RET_NO) );
	}
}


//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	�ذ��� ���� List�� ��û�Ѵ�.
//��������������������������������������������������������������������������������������������������������������������������������������������������������������
VOID	SOPlayer::RecvMBarrackGetDismissMonsterList()
{
	if( stAccountInfo.pVillage != NULL )
	{
		if( stAccountInfo.pVillage->m_Barrack != NULL )
		{

			// ��� ���� ����� �޽����� Village Ŭ�������� �����ش�.		
			stAccountInfo.pVillage->m_Barrack->MBarrackGetDismissMonsterList( this );				
		}
		else
		{
			// ���� ������ List�� �ټ� ����.
			SendFieldMsg( ON_RESPONSE_MBARRACK_GETDISMISSMONSTERLIST, SO_SFM_ONLYME, LPVOID(ON_RET_NO) );
		}

	}
	else
	{		
		// ���� ������ List�� �ټ� ����.
		SendFieldMsg( ON_RESPONSE_MBARRACK_GETDISMISSMONSTERLIST, SO_SFM_ONLYME, LPVOID(ON_RET_NO) );
	}
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	���͸� �ذ��Ѵ�.
//��������������������������������������������������������������������������������������������������������������������������������������������������������������
VOID	SOPlayer::RecvMBarrackDismissMonster()
{
	OnReqMBarrackDismissMonster			*pOnReqMBarrackDismissMonster;

	pOnReqMBarrackDismissMonster	=	(OnReqMBarrackDismissMonster*)cMsg;

	if( stAccountInfo.pVillage != NULL )
	{
		if( stAccountInfo.pVillage->m_Barrack != NULL )
		{

			// ���� �ذ� ���� ������ Village Ŭ�������� �����ش�.
			if( stAccountInfo.pVillage->m_Barrack->MBarrackDismissMonster(this, pOnReqMBarrackDismissMonster->dwSlot) == FALSE )
			{
				// ���н� Message �� ������ �ش�. ������������ ���ο��� Message�� ������.
				SendFieldMsg( ON_RESPONSE_MBARRACK_DISMISSMONSTER, SO_SFM_ONLYME, LPVOID(ON_RET_NO) );
			}
		}
		else
		{
			// ���� �������� ���͸� �ذ��Ҽ� ����.
			SendFieldMsg( ON_RESPONSE_MBARRACK_DISMISSMONSTER, SO_SFM_ONLYME, LPVOID(ON_RET_NO) );			
		}
	}
	else
	{		
		// ���� �������� ���͸� �ذ��Ҽ� ����.
		SendFieldMsg( ON_RESPONSE_MBARRACK_DISMISSMONSTER, SO_SFM_ONLYME, LPVOID(ON_RET_NO) );
	}
}


//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	���尳���� ���´�.
//��������������������������������������������������������������������������������������������������������������������������������������������������������������
SI32	SOPlayer::GetBankBootNum()
{
	SI32			siNum;
	if(pMyWorld->clDatabase->GetDBInfo(SO_DB_INFOKIND_BANKBOOK_NUM, this, LPVOID(&siNum)) == FALSE)
		siNum	=	-1;
	return siNum;
}


//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	�ſ� �㺸 ���� �ִ� �ݾ�.
//��������������������������������������������������������������������������������������������������������������������������������������������������������������
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
				//siMaxLoan�� �����ؾ� �Ѵ�.
				SendFieldMsg(ON_RESPONSE_BANK_MAXCREDITLOAN, SO_SFM_ONLYME, LPVOID(ON_RET_BANK_OK), LPVOID(uiMaxLoan),  LPVOID(0));  
				return;
			}
		}
		bResult	=	ON_RET_BANK_NO;
	}
	if( bResult	==	ON_RET_BANK_NO )
		SendFieldMsg(ON_RESPONSE_BANK_MAXCREDITLOAN, SO_SFM_ONLYME, LPVOID(ON_RET_BANK_NO));
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	�뺴 ġ��List�� ��û�Ѵ�.
//��������������������������������������������������������������������������������������������������������������������������������������������������������������
VOID	SOPlayer::RecvHospitalGetCureList()
{
	if(stAccountInfo.pVillage != NULL)
	{
		// ��� ���� ����� �޽����� Village Ŭ�������� �����ش�.		
		stAccountInfo.pVillage->m_Hospital->HospitalGetCureList(this);				
	}
	else
	{		
		// �뺴 ġ����� List�� �ټ� ����.
		SendFieldMsg(ON_RESPONSE_HOSPITAL_GETCURELIST, SO_SFM_ONLYME);
	}
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	�뺴�� ġ���Ѵ�.
//��������������������������������������������������������������������������������������������������������������������������������������������������������������
VOID	SOPlayer::RecvHospitalCureSolder()
{
	OnReqHospitalCureSolder		*pOnReqHospitalCureSolder;

	pOnReqHospitalCureSolder	=	(OnReqHospitalCureSolder*)cMsg;

	if(stAccountInfo.pVillage != NULL)
	{
		// �뺴 ġ�ῡ ���� ������ Village Ŭ�������� �����ش�.
		if(stAccountInfo.pVillage->m_Hospital->HospitalCureSolder(this, pOnReqHospitalCureSolder->dwCureSolderSlot) == FALSE)
		{
			// ���н� Message �� ������ �ش�. ������������ ���ο��� Message�� ������.
			SendFieldMsg(ON_RESPONSE_HOSPITAL_CURESOLDER, SO_SFM_ONLYME, LPVOID(ON_RET_NO));
		}
	}
	else
	{		
		// �뺴 ġ������� �뺴�� ġ���Ҽ� ����.
		SendFieldMsg(ON_RESPONSE_HOSPITAL_CURESOLDER, SO_SFM_ONLYME, LPVOID(ON_RET_NO));
	}
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	��� �뺴�� ġ���Ѵ�.
//��������������������������������������������������������������������������������������������������������������������������������������������������������������
VOID	SOPlayer::RecvHospitalCureAllSolder()
{
	OnReqHospitalCureAllSolder		*pOnReqHospitalCureAllSolder;

	pOnReqHospitalCureAllSolder	=	(OnReqHospitalCureAllSolder*)cMsg;

	if(stAccountInfo.pVillage != NULL)
	{
		// �뺴 ġ�ῡ ���� ������ Village Ŭ�������� �����ش�.
		if(stAccountInfo.pVillage->m_Hospital->HospitalCureAllSolder(this) == FALSE)
		{
			// ���н� Message �� ������ �ش�. ������������ ���ο��� Message�� ������.
			SendFieldMsg(ON_RESPONSE_HOSPITAL_CUREALLSOLDER, SO_SFM_ONLYME, LPVOID(ON_RET_NO));
		}
	}
	else
	{		
		// �뺴 ġ������� �뺴�� ġ���Ҽ� ����.
		SendFieldMsg(ON_RESPONSE_HOSPITAL_CUREALLSOLDER, SO_SFM_ONLYME, LPVOID(ON_RET_NO));
	}
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	�εο� ��
//��������������������������������������������������������������������������������������������������������������������������������������������������������������
VOID	SOPlayer::RecvWharfEnter()
{
	if(stAccountInfo.pVillage != NULL)
	{
		// ��� ���� ����� �޽����� Village Ŭ�������� �����ش�.		
		stAccountInfo.pVillage->m_Wharf->WharfEnter(this);				
	}
	else
	{		
		// �εη� ���� ����.
		SendFieldMsg(ON_RESPONSE_WHARF_ENTER, SO_SFM_ONLYME, LPVOID(ON_RET_NO));
	}
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	ǥ�� ��
//��������������������������������������������������������������������������������������������������������������������������������������������������������������
VOID	SOPlayer::RecvWharfBuyTicket()
{
	OnReqWharf_BuyTicket*			lpOnReqWharf_BuyTicket;

	if(stAccountInfo.pVillage != NULL)
	{
		// ��� ���� ����� �޽����� Village Ŭ�������� �����ش�.		
		lpOnReqWharf_BuyTicket = (OnReqWharf_BuyTicket*)cMsg;

		stAccountInfo.pVillage->m_Wharf->WharfBuyTicket(this, lpOnReqWharf_BuyTicket->siTicketKind);
	}
	else
	{		
		// �εο��� Ticket�� ��� ����.
		SendFieldMsg(ON_RESPONSE_WHARF_BUYTICKET, SO_SFM_ONLYME, LPVOID(ON_RET_NO));
	}
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	�εθ� ����
//��������������������������������������������������������������������������������������������������������������������������������������������������������������
VOID	SOPlayer::RecvWharfExit()
{
	if(stAccountInfo.pVillage != NULL)
	{
		// ��� ���� ����� �޽����� Village Ŭ�������� �����ش�.		
		stAccountInfo.pVillage->m_Wharf->WharfExit(this);				
	}
	else
	{		
		// �εο��� ������ ����.
		SendFieldMsg(ON_RESPONSE_WHARF_EXIT, SO_SFM_ONLYME, LPVOID(ON_RET_NO));
	}
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	�ε� ���ǿ� ��
//��������������������������������������������������������������������������������������������������������������������������������������������������������������
VOID	SOPlayer::RecvWharfWaitRoomEnter()
{
	if(stAccountInfo.pVillage != NULL)
	{
		// ��� ���� ����� �޽����� Village Ŭ�������� �����ش�.		
		stAccountInfo.pVillage->m_Wharf->WharfWaitRoomEnter(this);				
	}
	else
	{		
		// �εδ��ǿ� ���� ����.
		SendFieldMsg(ON_RESPONSE_WHARFWAITROOM_ENTER, SO_SFM_ONLYME, LPVOID(ON_RET_NO));
	}
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	�ε� ������ ����
//��������������������������������������������������������������������������������������������������������������������������������������������������������������
VOID	SOPlayer::RecvWharfWaitRoomExit()
{
	if(stAccountInfo.pVillage != NULL)
	{
		// ��� ���� ����� �޽����� Village Ŭ�������� �����ش�.		
		stAccountInfo.pVillage->m_Wharf->WharfWaitRoomExit(this);				
	}
	else
	{		
		// �εδ��ǿ��� ������ ����.
		SendFieldMsg(ON_RESPONSE_WHARFWAITROOM_EXIT, SO_SFM_ONLYME, LPVOID(ON_RET_NO));
	}
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	DB���� Ticket ������ ����
//��������������������������������������������������������������������������������������������������������������������������������������������������������������
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
	// ������ OK
	SendFieldMsg(ON_RESPONSE_GOVERNMENT_MAIN_ENTER, SO_SFM_ONLYME, LPVOID(ON_RET_OK));
}

VOID	SOPlayer::RecvGovernmentLeave()
{
	// ������ OK
	SendFieldMsg(ON_RESPONSE_GOVERNMENT_MAIN_LEAVE, SO_SFM_ONLYME, LPVOID(ON_RET_OK));
}

VOID	SOPlayer::RecvGovernmentInvestmentEnter()
{
	OnReqGovernmentEnterInvestment*			lpOnReqGovernmentEnterInvestment;

	lpOnReqGovernmentEnterInvestment = (OnReqGovernmentEnterInvestment*)cMsg;
	if(stAccountInfo.pVillage != NULL)
	{
		// ��� ���� ����� �޽����� Village Ŭ�������� �����ش�.		
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
		// ��� ���� ����� �޽����� Village Ŭ�������� �����ش�.		
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

	// siVillageCode�� -1�� ������ �÷��̾ ���� �� �ִ� ������ ������ ������ �����ش�.
	// siVillageCode�� -1�� �ƴ� ���̸� �÷��̾ �� �ִ� ������ �ƴ� siVillageCode�� ������ ������ �����ش�.
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
		// ��� ���� ����� �޽����� Village Ŭ�������� �����ش�.		
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
		// ��� ���� ����� �޽����� Village Ŭ�������� �����ش�.		
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
			printf("�ȳ���-�����ȹ��� ���[%s]-[%s]-[%c]\n",Temp,cTemp,*cTemp);
			return;
		*/
		uiResult = stAccountInfo.pVillage->m_Government->WriteDescription(this, Temp);
	}
	SendFieldMsg(ON_RESPONSE_WRITE_DESCRIPT, SO_SFM_ONLYME, LPVOID(uiResult));
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	��û, �������� ���
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
VOID	SOPlayer::RecvGovernmentVillageInfo()
{
	if(stAccountInfo.pVillage != NULL)
	{
		stAccountInfo.pVillage->GetVillageInfo(this);
	}
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	��û, �����ѵ� ������ ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
VOID	SOPlayer::RecvGovernmentInvestmentGetOut()
{
	OnReqGovernmentInvestmentGetOut*			lpOnReqGovernmentInvestmentGetOut;

	lpOnReqGovernmentInvestmentGetOut = (OnReqGovernmentInvestmentGetOut*)cMsg;
	if(stAccountInfo.pVillage != NULL)
	{
		stAccountInfo.pVillage->m_Government->InvestmentGetOut(this, lpOnReqGovernmentInvestmentGetOut->GetOutMoney);
	}
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	��Ⱓ ������ ���� ��� ����.
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
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

//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	�������¿� �����Ҽ� �ִ� ������ ��´�
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
VOID	SOPlayer::RecvReqMaxVillageDefenceInvestment()
{
	if(stAccountInfo.pVillage != NULL)
	{
		stAccountInfo.pVillage->m_Government->SendVillageReqMaxVillageDefenceInvestment(this);
	}
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	���� ���¿� �����ϱ�
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
VOID	SOPlayer::RecvReqSetVillageDefenceInvestment()
{
	OnReqSetVillageDefenceInvestment*		lpOnReqSetVillageDefenceInvestment;

	lpOnReqSetVillageDefenceInvestment = (OnReqSetVillageDefenceInvestment*)cMsg;
	if(stAccountInfo.pVillage != NULL)
	{
		stAccountInfo.pVillage->m_Government->SendVillageReqSetVillageDefenceInvestment(this, lpOnReqSetVillageDefenceInvestment->InvestmentMoney);
	}
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	��� ������ �ʿ��� ����� �䱸�ϴ� �޽����� �޾Ҵ�.
//	���� ����	:	2003-01-27 ���� 2:18:10 - �����
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
VOID	SOPlayer::RecvReqGuildCreateCost()
{
	MONEY	mnGuildCreateCost	=	pMyWorld->clIGuild.GetGuildCreateCost();
	SendFieldMsg(ON_RESPONSE_GUILD_CREATE_COST, SO_SFM_ONLYME, (LPVOID*)&mnGuildCreateCost );
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	��带 �����Ѵٴ� �޽����� �޾Ҵ�.
//	���� ����	:	2002-02-25 ���� 11:26:10 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
VOID	SOPlayer::RecvCreateGuild()
{

	// DB��ĥ��

	OnReqCreateGuildMsg		*pCreateGuildMsg;
	CHAR					*pGuildName;	
	BOOL					bDBSuccess = FALSE;		
	CHAR					szCharID[ON_ID_LENGTH + 1];
	CHAR					szGuildName[ON_GUILDNAME_LENGTH + 1];
	BOOL					bCreateGuildSuccess = FALSE;
	MONEY					mnGuildCreateCost;

	if(stAccountInfo.pVillage == NULL)														goto	END;	// ������ ���� ������ ����� ������ �� ����.
	if(stAccountInfo.pVillage->uiGuildID !=0)												goto	END;	// ���� ����� ������� �ϴ� ������ �̹� �ٸ� ����� ������ ����� ������� ����.
	// �ڽ��� �ٸ���ܿ� ��ܿ��̸� ����� ���������.
	if (uiGuildID != 0 ) goto END;
	
	// ���� �� ������ �ְ� ���������� ����.
	ZeroMemory(szCharID, sizeof(szCharID));
	strncpy(szCharID, szID, ON_ID_LENGTH);
	if(stAccountInfo.pVillage->m_Government->CheckInvestmentMaxInvestmentorName(szCharID, 0) == FALSE)	goto END;		

	// ��� ������ �ʿ��� �ڱ�
	mnGuildCreateCost = pMyWorld->clIGuild.GetGuildCreateCost();

	// ��� ������ ���� ����� �־�� �Ѵ�.
	if(EnoughMoney(mnGuildCreateCost) == FALSE)												goto	END;

	pCreateGuildMsg	=	(OnReqCreateGuildMsg*)cMsg;	
	pGuildName		=	(CHAR*)((CHAR*)pCreateGuildMsg + sizeof(OnReqCreateGuildMsg));	
	
	ZeroMemory(szGuildName, sizeof(szGuildName));
	strncpy(szGuildName, pGuildName, min(pCreateGuildMsg->uiGuildNameLength, ON_GUILDNAME_LENGTH));

	// ��� �̸��� ��ȿ���� �Ǵ��Ѵ�.
	if(CheckForbiddenName(szGuildName) == FALSE)											goto	END;
	if(pMyWorld->clIGuild.IsUsedName(szGuildName) == TRUE )                                 goto    END;

	

	uiGuildID		=	pMyWorld->clIGuild.CreateGuild(szGuildName, this, stAccountInfo.pVillage);

	
	if(uiGuildID == 0)																		goto	END;
	
	ChangeMoney(-mnGuildCreateCost,FALSE);

	// ���������� ����� �����ߴ�.
	bCreateGuildSuccess	=	TRUE;	

END:
	if(bCreateGuildSuccess == TRUE)
	{	
		// ����� �����ϴµ� �����ߴ�.		
		SendFieldMsg(ON_RESPONSE_CREATE_GUILD, SO_SFM_ONLYME, LPVOID(ON_RET_OK));
	}
	else
	{
		// ����� �����ϴµ� �����ߴ�.
		SendFieldMsg(ON_RESPONSE_CREATE_GUILD, SO_SFM_ONLYME, LPVOID(ON_RET_NO));
	}
}


//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	����� �����Ѵٴ� �޽����� �޾Ҵ�
//	���� ����	:	2002-02-25 ���� 11:26:05 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
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
	//�����ϴ� ���ΰ�
	if(pMyWorld->m_WarTimeParser.IsWarCheck(pMyWorld->pRealTime->tm_wday, pMyWorld->pRealTime->tm_hour))
	{
		if(pMyWorld->IsAttackVillageTimeZone()) //���� �ð����� üũ
		{
			SendFieldMsg(ON_RESPONSE_DELETE_GUILD, SO_SFM_ONLYME, LPVOID(ON_RET_NO));
			return;
		}
	}

	//���밡 1���� �ɸ� ����� ����� ��ü�Ҽ� ����.
	if(pMyWorld->clIGuild.GetGuildWarCheck(uiGuildID)) 
	{  
		SendFieldMsg(ON_RESPONSE_DELETE_GUILD, SO_SFM_ONLYME, LPVOID(ON_RET_NO));
		return;
	}

	// Guild ID�� ��ȿ���� üũ�Ѵ�.
	if(pMyWorld->clIGuild.DeleteGuild(uiGuildID, stAccountInfo.pVillage) == TRUE)
	{
		// ����� �����ϴµ� �����ߴ�.		
		SendFieldMsg(ON_RESPONSE_DELETE_GUILD, SO_SFM_ONLYME, LPVOID(ON_RET_OK));
	}		
	else
	{
		// ����� �����ϴµ� �����ߴ�.
		SendFieldMsg(ON_RESPONSE_DELETE_GUILD, SO_SFM_ONLYME, LPVOID(ON_RET_NO));
	}	
}


//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	��ܿ� �����ϰų� ������.
//	���� ����	:	2002-03-04 ���� 10:41:08 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
VOID	SOPlayer::RecvJoinLeaveGuild()
{
	OnReqJoinLeaveGuildMsg		*pJoinLeaveGuildMsg;	
	BOOL						bDBSuccess = FALSE, bSuccess = FALSE;
	BOOL						bDeleteGuild	=	FALSE;	
	BOOL						bCandidate;
	DWORD						dwResponse = ON_RET_NO;

	pJoinLeaveGuildMsg	=	(OnReqJoinLeaveGuildMsg*)cMsg;

    //������ �ִ³��̸鼭 �����ð����� ��ܰ���, Ż�� �ʵȴ�.
	if(pMyWorld->m_WarTimeParser.IsWarCheck(pMyWorld->pRealTime->tm_wday, pMyWorld->pRealTime->tm_hour))
	if(pMyWorld->m_WarTimeParser.IsWarTime(pMyWorld->pRealTime->tm_hour))
	{
		SendFieldMsg(ON_RESPONSE_JOIN_LEAVE_GUILD, SO_SFM_ONLYME, LPVOID(dwResponse));
		return;
	}

	if(uiGuildID == 0)
	{
		// ������ �ؾ� �Ѵ�.
		dwResponse		=	ON_RET_GLD_JOIN_NO;

		// ���� ������ ��ȿ���� �˾ƿ´�.
		if(stAccountInfo.pVillage != NULL)
		{
			// �� ������ Guild ID�� ���Ѵ�.
			if(stAccountInfo.pVillage->uiGuildID != 0)
			{
				// ��ܿ� �����Ѵ�.
				if(pMyWorld->clIGuild.AddCandidate(stAccountInfo.pVillage->uiGuildID, this) == TRUE)									
					dwResponse			=	ON_RET_GLD_JOIN_OK;												
			}
		}
	}
	else
	{
		// ������ ��ܿ��� ������.
		dwResponse			=	ON_RET_GLD_LEAVE_NO;

		// ���� ������ ��ȿ���� �˾ƿ´�.
		if(stAccountInfo.pVillage != NULL)
		{
			// �� ������ Guild ID�� �ڱ��� Guild ID�� ���ƾ� �Ѵ�.
			// �� �ڱ� ����� �����ϰ� �ִ� �����̾�� �Ѵ�.
			if(stAccountInfo.pVillage->uiGuildID == uiGuildID)
			{
				if(pMyWorld->m_WarTimeParser.IsWarCheck(pMyWorld->pRealTime->tm_wday, pMyWorld->pRealTime->tm_hour)) return;
				if(pMyWorld->m_WarTimeParser.IsWarTime(pMyWorld->pRealTime->tm_hour)) return;
								
				// ������� ����.
				if(siClassInGuild == pMyWorld->clIGuildClass.GetNumericClassCode(SO_GUILD_SC_DAI))
				{
					// ����̴�. 					
					
					///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
					// for debug
					if( uiGuildID == 0 )
					{
//						pMyWorld->clServer->WriteInfo( "..\\data\\DeletedGuild.txt", "Clan Master has left the guild. Guild disorganizes. But the Guild ID is 0" );
						writeInfoToFile( "DeletedGuild.txt", "Clan Master has left the guild. Guild disorganizes. But the Guild ID is 0" );
					}
					///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

					// ����� ���ش�.
					if(pMyWorld->clIGuild.DeleteGuild(uiGuildID,stAccountInfo.pVillage) == TRUE)
					{						
						dwResponse		=	ON_RET_GLD_LEAVE_OK_DELETEGLD;										
					}
				}
				else
				{
					// ����� �ƴϴ�.

					// ��ܿ� �Ѹ��� ���������ٰ� �˷��ش�.
					if(siClassInGuild == pMyWorld->clIGuildClass.GetJWJCode())				bCandidate	=	TRUE;
					else																	bCandidate	=	FALSE;

					if(pMyWorld->clIGuild.DelGuildMember(stAccountInfo.pVillage->uiGuildID, szID, bCandidate, FALSE ,FALSE) == TRUE)					
						dwResponse		=	ON_RET_GLD_LEAVE_OK;										
				}				
			}
		}
	}

	// �����ߴ�.
	SendFieldMsg(ON_RESPONSE_JOIN_LEAVE_GUILD, SO_SFM_ONLYME, LPVOID(dwResponse));	
}


//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	��ܿ� �����ϱ⸦ ��ٸ��� ����ڵ��� ����Ʈ�� ���´�.
//	���� ����	:	2002-02-25 ���� 1:15:50 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
VOID	SOPlayer::RecvReqGuildWaitingJoinMembersList()
{
	cltGuild	*pclGuild;
	BOOL		bSuccess = FALSE;
	OnReqGuildWaitingJoinMembersListMsg		*pGuildWaitingJoinMembersListMsg;

	pGuildWaitingJoinMembersListMsg		=	(OnReqGuildWaitingJoinMembersListMsg*)cMsg;

	// ��ܿ� �ݵ�� �ҼӵǾ� �־�� �Ѵ�.
	if(uiGuildID == 0)	goto	END;

	// ����� �ݵ�� �����Ͽ��� �Ѵ�.
	if((pclGuild = pMyWorld->clIGuild.GetGuildByID(uiGuildID)) == NULL)	goto	END;		

	// �����ߴ�.
	bSuccess	=	TRUE;

END:
	if(bSuccess == TRUE)
	{
		// �������� ����Ʈ�� ���� �� �� �ִ�.
		SendFieldMsg(ON_RESPONSE_GUILD_WAITING_JOIN_MEMBERS_LIST, SO_SFM_ONLYME, LPVOID(ON_RET_OK), LPVOID(pclGuild), 
						LPVOID(pGuildWaitingJoinMembersListMsg->dwPage));
	}
	else
	{
		// �������� ����Ʈ�� ���� �� �� ����
		SendFieldMsg(ON_RESPONSE_GUILD_WAITING_JOIN_MEMBERS_LIST, SO_SFM_ONLYME, LPVOID(ON_RET_NO));
	}
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	��ܿ� ������ ��� �� �ź��Ѵ�.
//	���� ����	:	2002-02-25 ���� 1:46:04 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
VOID	SOPlayer::RecvJoinGuildOkNo()
{
	CHAR						szCharID[1024];
	OnReqJoinGuildOkNoMsg		*pJoinGuildOkNoMsg;	
	DWORD						dwResponse = ON_RET_NO;

	pJoinGuildOkNoMsg		=	(OnReqJoinGuildOkNoMsg*)cMsg;	
	
	ZeroMemory(szCharID, sizeof(szCharID));
	strncpy(szCharID, pJoinGuildOkNoMsg->szID, ON_ID_LENGTH);

	if(pJoinGuildOkNoMsg->dwResponse == ON_RET_OK)			// '�Դ����'��ư�� ������ ���
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
					// ����� �������ش�. (�Դ� ���)
					if(pMyWorld->clIGuild.AddGuildMember(uiGuildID, szCharID) == TRUE)
						dwResponse	=	ON_RET_OK;
					else		
						dwResponse	=	ON_RET_NO_GLD_ADMIT_FULL;
				}
			}
		}		
	}
	else if(pJoinGuildOkNoMsg->dwResponse == ON_RET_NO)		// '�Դ����'��ư�� ������ ���
	{
		// ������ �ź��Ѵ�.
		if(pMyWorld->clIGuild.DelGuildMember(uiGuildID, szCharID, TRUE, TRUE ,TRUE) == TRUE)	dwResponse	=	ON_RET_OK;		
	}

	// �� �޾Ҵٰ� ������ �����ش�.
	SendFieldMsg(ON_RESPONSE_JOINGUILD_OKNO, SO_SFM_ONLYME, LPVOID(dwResponse));
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	����� ���� �� ������ ���´�.	
//	���� ����	:	2002-04-22 ���� 9:35:33 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
VOID	SOPlayer::RecvGuildDetailInfo()
{
	OnReqGuildDetailInfoMsg	*pGuildDetailInfoMsg;
	CHAR					szGuildID[ON_GUILDNAME_LENGTH + 1];	// ����� �̸�
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

//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	��ܿ� ���� ����Ʈ�� ���´�.
//	���� ����	:	2002-02-25 ���� 2:18:10 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
VOID	SOPlayer::RecvReqGuildMembersList()
{
	OnReqGuildMembersListMsg	*pGuildMembersListMsg;
	cltGuild					*pclGuild;
	UI32						uiReqGuild;

	pGuildMembersListMsg	=	(OnReqGuildMembersListMsg*)cMsg;	

	// ���࿡ ��� ID�� 0�̶�� ���� ��ܿ� ������ �䱸�ϴ°��̴�.
	if(pGuildMembersListMsg->uiGuidlID == 0)
		uiReqGuild	=	uiGuildID;
	else
		uiReqGuild	=	pGuildMembersListMsg->uiGuidlID;

	if((pclGuild = pMyWorld->clIGuild.GetGuildByID(uiReqGuild)) != NULL)	
		SendFieldMsg(ON_RESPONSE_GUILD_MEMBERS_LIST, SO_SFM_ONLYME, LPVOID(ON_RET_OK), LPVOID(pclGuild), LPVOID(pGuildMembersListMsg->dwPage));	
	else	
		SendFieldMsg(ON_RESPONSE_GUILD_MEMBERS_LIST, SO_SFM_ONLYME, LPVOID(ON_RET_NO));	
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	��ܿ��� �ذ��Ѵ�.
//	���� ����	:	2002-03-25 ���� 2:05:32 - �����
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
VOID	SOPlayer::RecvGuildDismissal()
{
	CHAR				szCharID[ON_ID_LENGTH + 1];
	OnReqDismissalMsg	*pDismissalMsg;
	cltGuild			*pclGuild;
	BOOL				bSuccess	=	FALSE;

	pDismissalMsg	=	(OnReqDismissalMsg*)cMsg;

	ZeroMemory(szCharID, sizeof(szCharID));
	strncpy(szCharID, pDismissalMsg->szID, ON_ID_LENGTH);

	//���� �ð����� �ذ��Ҽ� ����.
	if(pMyWorld->m_WarTimeParser.IsWarCheck(pMyWorld->pRealTime->tm_wday, pMyWorld->pRealTime->tm_hour)) goto END;
	if(pMyWorld->m_WarTimeParser.IsWarTime(pMyWorld->pRealTime->tm_hour)) goto END;
	// ��� ��ü�� ���´�.
	if( (pclGuild	=	pMyWorld->clIGuild.GetGuildByID(uiGuildID)) == NULL )		goto END;
	
	// �ذ��Ϸ��� ����� ����̶�� ���� ó���Ѵ�.
	if( strcmp(pclGuild->GetDAIID(), szCharID) == 0 )								goto END;

	// ���� ����
	if( pMyWorld->clIGuild.DelGuildMember(uiGuildID, szCharID, FALSE, TRUE ,TRUE) == TRUE )		bSuccess	=	TRUE;


END:
	if( bSuccess )
	{
		// ��ܿ��Ե� �˷��ش�.
		SendFieldMsg(ON_RESPONSE_GUILD_DISMISSAL, SO_SFM_ONLYME, LPVOID(ON_RET_OK));
	}
	else
	{
		SendFieldMsg(ON_RESPONSE_GUILD_DISMISSAL, SO_SFM_ONLYME, LPVOID(ON_RET_NO));
	}
}


//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	��ܿ��� Ư�� �������� �Ӹ��Ѵٴ� �޽����� ó���Ѵ�.
//	���� ����	:	2003-02-11 ���� 1:43 - �����
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
VOID	SOPlayer::RecvReqGuildPromotion()
{
	CHAR						szCharID[ON_ID_LENGTH+1];
	OnRequestGuildPromotion		*pPromotionMsg;
	SI32						siSHNCode	=	pMyWorld->clIGuildClass.GetSHNCode();			// ��ȯ
	SI32						siHNSCode	=	pMyWorld->clIGuildClass.GetHNSCode();			// ���
	SI32						siDHSCode	=	pMyWorld->clIGuildClass.GetDHSCode();			// �����
	SI32						siHNDCode	=	pMyWorld->clIGuildClass.GetHNDCode();			// �ൿ����
	BOOL						bSuccess	=	FALSE;

	pPromotionMsg = (OnRequestGuildPromotion*)cMsg;

	ZeroMemory( szCharID, sizeof(szCharID) );
	strncpy( szCharID, pPromotionMsg->szCharID, ON_ID_LENGTH );

	cltGuild *pGuild = pMyWorld->clIGuild.GetGuildByID( uiGuildID );

	strGuildCommonMemberInfo MemberInfo;

	// ĳ���� �̸��� ����Ͽ� ��� ������ ���´�.
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
		// ������� �Ӹ� ��û
		// ����1: �Ӹ���� ����� �������� ��ȯ �̻��̾�� �Ѵ�.
		// ����2: ���� ����� �ڸ��� ����־�� �Ѵ�.

		// ��ȯ���� ���� ������ �ƴϸ� �Ӹ� ����
		if( pMyWorld->clIGuildClass.ClassComp(MemberInfo.siClassInGuild, siSHNCode) == SO_GUILD_CLASSCOMP_LOW )			goto	END;
		// �̹� ������� ������ �Ӹ� ����
		if( pGuild->m_GuildMemberManager.GetDHSID() != NULL )															goto	END;

		bSuccess	=	pMyWorld->clIGuild.ChangeClass( uiGuildID, szCharID, siDHSCode );
	}
	else if( pPromotionMsg->siClassCode == siHNDCode )
	{
		// �ൿ�������� �Ӹ� ��û
		// ����1: �Ӹ���� ����� �������� ��ȯ�̾�� �Ѵ�.
		// ����2: �ൿ������ ���� ���� ���� 5���� 1���̴�.		����: (n + 4) / 5		n: ����������

		if( MemberInfo.siClassInGuild != siSHNCode )	goto	END;				// ��ȯ�� �ƴϸ� �Ӹ� ����
		// �Ӹ��� �� �ִ� ��ȯ�� ���� �Ѿ�� �Ӹ� ����
		if( pGuild->m_GuildMemberManager.GetTotalHNDNum() >= (UI32)((pGuild->GetOwnerVillageNum() + 4) / 5) )	goto	END;

		bSuccess	=	pMyWorld->clIGuild.ChangeClass( uiGuildID, szCharID, siHNDCode );
	}

END:
	if( bSuccess )
		SendFieldMsg( ON_RESPONSE_GUILD_PROMOTION, SO_SFM_ONLYME, LPVOID(ON_RET_OK) );
	else		
		SendFieldMsg( ON_RESPONSE_GUILD_PROMOTION, SO_SFM_ONLYME, LPVOID(ON_RET_NO) );
}


//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	��ܿ��� Ư�� ���޿��� �����Ѵٴ� �޽����� ó���Ѵ�.
//	���� ����	:	2003-02-11 ���� 1:47 - �����
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
VOID	SOPlayer::RecvReqGuildDemotion()
{
	CHAR						szCharID[ON_ID_LENGTH+1];
	OnRequestGuildDemotion		*pDemotionMsg;
	SI32						siSHNCode	=	pMyWorld->clIGuildClass.GetSHNCode();			// ��ȯ
	SI32						siHNSCode	=	pMyWorld->clIGuildClass.GetHNSCode();			// ���
	SI32						siDHSCode	=	pMyWorld->clIGuildClass.GetDHSCode();			// �����
	SI32						siHNDCode	=	pMyWorld->clIGuildClass.GetHNDCode();			// �ൿ����
	SI32						siDAICode	=	pMyWorld->clIGuildClass.GetDAICode();			// �ൿ����
	BOOL						bSuccess	=	FALSE;
	SI32						siNewClass;

	pDemotionMsg = (OnRequestGuildDemotion*)cMsg;
	ZeroMemory( szCharID, sizeof(szCharID) );
	strncpy( szCharID, pDemotionMsg->szCharID, ON_ID_LENGTH );

	cltGuild *pGuild = pMyWorld->clIGuild.GetGuildByID( uiGuildID );

	strGuildCommonMemberInfo MemberInfo;

	// ĳ���� �̸��� ����Ͽ� ��� ������ ���´�.
	if( pGuild != NULL )
	{
		if( pGuild->m_GuildMemberManager.GetGuildMemberByCharName(szCharID, &MemberInfo) == FALSE )		goto	END;
	}
	else
	{
		goto	END;
	}

	if( MemberInfo.siClassInGuild == siDAICode )		goto END;							// ����̸� ������ �� ����.

	if( MemberInfo.siClassInGuild == siDHSCode )
	{
		// ������� ������ ��� --> ��ȯ�� �� ���� �ְ� ����� �� ���� �ִ�.
		siNewClass	=	pMyWorld->GetHighestGuildClass( szCharID );							// �� ������ �˻��Ͽ� �� ĳ������ �ִ� ������ ���´�.
		bSuccess	=	pMyWorld->clIGuild.ChangeClass( uiGuildID, szCharID, siNewClass );	// �˻��Ͽ� ���� �ִ��������� �ٲ��ش�.
	}
	else if( MemberInfo.siClassInGuild == siHNDCode )
	{
		// �ൿ������ ������ ��� --> ��ȯ�� �ȴ�.
		bSuccess	=	pMyWorld->clIGuild.ChangeClass( uiGuildID, szCharID, siSHNCode );
	}

END:

	if( bSuccess )
		SendFieldMsg( ON_RESPONSE_GUILD_DEMOTION, SO_SFM_ONLYME, LPVOID(ON_RET_OK) );
	else
		SendFieldMsg( ON_RESPONSE_GUILD_DEMOTION, SO_SFM_ONLYME, LPVOID(ON_RET_NO) );
}


//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	����� ����Ʈ�� �䱸�Ѵ�.
//	���� ����	:	2002-02-27 ���� 2:45:30 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
VOID	SOPlayer::RecvGuildList()
{	
	// ����� ������ �����ش�.
	SendFieldMsg(ON_RESPONSE_GUILD_LIST, SO_SFM_ONLYME);
}


//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	����� �����Ѵ�.
//	���� ����	:	2002-02-27 ���� 4:27:53 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
VOID	SOPlayer::SetGuild(UI32 uiguildid)
{
	// ����� �����Ѵ�.
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

//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	������ �����Ѵ�.
//	���� ����	:	2002-02-27 ���� 4:27:53 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
VOID	SOPlayer::SetClassInGuild(SI32 siclassinguild)
{
	// ������ �����Ѵ�.
	siClassInGuild	=	siclassinguild;	
}


//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	������ ���´�.
//	���� ����	:	2003-02-11 ���� 2:173 - �����
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
SI32	SOPlayer::GetClassInGuild()
{
	return siClassInGuild;
}
	
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	����� ����� �����Ѵ�.
//	���� ����	:	2002-04-04 ���� 1:35:28 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
VOID	SOPlayer::RecvGuildChangeFlag()
{

	// DB��ĥ��

	BOOL						bSuccess = FALSE;
	OnReqGuildChangeFlag		*pGuildChangeFlag;

	// ������� üũ�Ѵ�.
	if(uiGuildID == 0)	goto	END;
	if(siClassInGuild != pMyWorld->clIGuildClass.GetNumericClassCode(SO_GUILD_SC_DAI))	goto	END;

	if(EnoughMoney(ON_GUILD_COST_CHANGE_FLAG) == FALSE)				goto	END;

	pGuildChangeFlag	=	(OnReqGuildChangeFlag*)cMsg;

	if(pMyWorld->clIGuild.SetFlag(uiGuildID, pGuildChangeFlag->siFlag) == FALSE)
		goto	END;

	// ���������� ����� �����ϰ� �Ǹ�
	// SetFlag �Լ� ������ �ڵ������� ���� ���ӵǾ� �ִ� ��ܿ����� �޽����� �����ش�.

	ChangeMoney(-ON_GUILD_COST_CHANGE_FLAG,FALSE);

	// DB�� �����Ѵ�.

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

//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	���� ����� ����Ʈ�� ����.
//	���� ����	:	2002-04-04 ���� 2:18:02 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
VOID	SOPlayer::RecvReqUsedFlagList()
{	
	if(uiGuildID != 0)
	{
		// ����� ���� ��쿡�� �Ѱ� �ش�.		
		SendFieldMsg(ON_RESPONSE_USED_FLAG_LIST, SO_SFM_ONLYME, LPVOID(uiGuildID));
	}
}
/*
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	������ �����Ѵٴ� �޽����� �޾Ҵ�.
//	���� ����	:	2002-04-08 ���� 2:31:00 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
VOID	SOPlayer::RecvDefenceVillage()
{
	OnReqRepairVillageMsg	*pRepairVillageMsg;
	BOOL					bSuccess = FALSE;

	pRepairVillageMsg	=	(OnReqRepairVillageMsg*)cMsg;

	if(stAccountInfo.pVillage == NULL)	goto	END;

	// �������� ��쿡�� IncreaseDefence() ������ ���õ� �÷��̾�鿡�� �޽����� �����ش�.
	stAccountInfo.pVillage->RepairDefence(this, pRepairVillageMsg->vdDefence);

	// �����ߴ�.
	bSuccess	=	TRUE;
END:
	if(bSuccess == FALSE)
	{
		// ����
		SendFieldMsg(ON_RESPONSE_REPAIR_VILLAGE, SO_SFM_ONLYME, LPVOID(ON_RET_NO));
	}	
}
*/

//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	�������� Ư�� �ǹ��� ���ٴ� �޽����� �޾Ҵ�.
//	���� ����	:	2002-10-02 ���� 7:16:46 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
VOID	SOPlayer::RecvEnterStructure()
{
	OnReqEnterStructureMsg		*pEnterStructureMsg;
	BOOL						bSuccess	=	FALSE;
	
	pEnterStructureMsg		=	( OnReqEnterStructureMsg* )cMsg;
	
	// ���� ���� �ȿ� �ִ��� �˻��Ѵ�.
	if( IsInVillage() == FALSE )																							goto	END;
	
	// ���� �� �ִ� �ǹ��� �ִ��� �˻��Ѵ�.
	if( IsEnteredStructure() == TRUE )																						goto	END;
	
	// Ư�� �ǹ��� ����.
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

//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	�������� Ư�� �ǹ����� �����ٴ� �޽����� �޾Ҵ�.
//	���� ����	:	2002-10-02 ���� 7:31:20 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
VOID	SOPlayer::RecvLeaveStructure()
{
	BOOL						bSuccess	=	FALSE;
	
	// ���� ���� �ȿ� �ִ��� �˻��Ѵ�.
	if( IsInVillage() == FALSE )																			goto	END;
	
	// ���� �� �ִ� �ǹ��� �ִ��� �˻��Ѵ�.
	if( IsEnteredStructure() == TRUE )																		goto	END;
	
	// Ư�� �ǹ����� ���´�.
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

//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	���� ��� ������ ���忡 ����.
//	���� ����	:	2002-10-11 ���� 4:57:09 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
VOID	SOPlayer::RecvEnterFactory()
{
	OnReqFarmWorkInfoMsg		*pFarmWorkInfoMsg;
	BOOL						bSuccess = FALSE;
	
	pFarmWorkInfoMsg		=	(OnReqFarmWorkInfoMsg*) cMsg;
	
	// ���� ���� �ȿ� �ִ��� �˻��Ѵ�.
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

//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	���� ��� ������ ���忡�� ������.
//	���� ����	:	2002-10-11 ���� 4:56:58 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
VOID	SOPlayer::RecvLeaveFactory()
{
	BOOL	bSuccess = FALSE;

	// ���� ���� �ȿ� �ִ��� �˻��Ѵ�.
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
		// �����̸�,

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

	// ���� �޽��� ���� 
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

	// ���� �޽��� ���� 
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

		// ��ܿ� �Ҽӵ� ����� �ƴϸ�
		if (pPlayer->siClassInGuild == 0) {

			ZeroMemory(resMsg.szGuildName,sizeof(resMsg.szGuildName));	
		}
		else
		{
			// ����� �ݵ�� �����Ͽ��� �Ѵ�.
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

	// �� �뺴 ������ �ִ��� �˻��Ѵ�.
	for( uiDonkeySlot = 0; uiDonkeySlot < ON_MAX_FOLLOWER_NUM; uiDonkeySlot++ )
	{
		if( IFollower.IsValidFollower(uiDonkeySlot) == FALSE )
		{
			bFound	=	TRUE;
			break;
		}
	}

	// �� ������ �ִٸ� �糪�� ĳ������ ������ �����ϰ� DB�� �˷��ְ� Ŭ���̾�Ʈ�� ������.
	if( bFound )
	{
		// ĳ���� ����Ʈ���� �糪�͸� ã�´�.
		for( SI32 i = 0; i < clServer->clWorld->clCharParser.GetCharTotalNum(); i++)
		{
			pCharHead = clServer->clWorld->clCharParser.GetCharHeader(i);
			
			if( pCharHead->uiID == clServer->clWorld->clCharParser.GetID("U85") )	break;
		}

		// Player�� �뺴ĭ�� �糪�͸� �߰��Ѵ�.(DB�� ������� �Ѵ�.)
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
		// OnlineText���� uiCharName���� ĳ�����̸�(�糪��) ���
		strncpy( stFollowerInfo.Name, clServer->clWorld->clOnlineText.Get( pCharHead->uiCharName ), ON_ID_LENGTH );
		CHAR	szCharName[256];
		ZeroMemory( szCharName, 256 );
		CopyMemory( szCharName, stFollowerInfo.Name, ON_ID_LENGTH );

		IFollower.InitFollower( uiDonkeySlot, &stFollowerInfo, pCharHead->siNation, &clServer->clWorld->clItem );

		DBIKConsumeDonkeyItem		DBInfo;

		ZeroMemory( &DBInfo, sizeof(DBIKConsumeDonkeyItem) );

		DBInfo.DBAccount		=	stAccountInfo.siDBAccount;
		DBInfo.uiSlot			=	stAccountInfo.uiSlot;
		DBInfo.uiFollowerID		=	uiFollowerID;					// �������� ����� �뺴
		DBInfo.uiItemID			=	uiItemID;						// ������ ID
		DBInfo.uiDonkeySlot		=	uiDonkeySlot;					// �糪�Ͱ� �� ����	
		DBInfo.siKind			=	stFollowerInfo.uiKind;
		clServer->clWorld->clDatabase->writeDBQuery( SO_DB_INFOKIND_CONSUMEDONKEYITEM, LPVOID(&DBInfo), sizeof(DBInfo) );

		// Client�� �糪�͸� �߰��϶�� ������.
		SendFieldMsg( ON_ADD_FOLLOWER, SO_SFM_ONLYME, LPVOID(&stFollowerInfo) );

		return TRUE;
	}

	return FALSE;
}


