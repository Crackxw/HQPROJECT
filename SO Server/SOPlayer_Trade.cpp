#include "SOMain.h"
#include "SOWorld.h"
#include "SOBaseChar.h"
#include "OnlineProtocol.h"
#include "OnlineCommonStruct.h"
#include "OnlineGameMsg.h"


//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	Trade를 하자고 요청한다. 
//	수정 일자	:	2002-07-04 오전 10:28:41 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
VOID	SOPlayer::RecvAskTrade()
{
	SOPlayer				*pTargetPlayer;
	OnReqAskTradeMsg		*pAskTradeMsg;
	BOOL					bSuccess  = FALSE;

	pAskTradeMsg	=	(OnReqAskTradeMsg*) cMsg;	

	// 현재 내가 Trade가 가능한지 알아온다.
	if(CanTrade() == FALSE)													goto	END;

	// Trade를 하고자하는 플레이어가 유효한지 검사한다.
	if((pTargetPlayer = pMyWorld->GetPlayer(pAskTradeMsg->UserID)) == NULL)	goto	END;		

	// Target 플레이어가 Trade가 가능한지 알아온다.
	if(pTargetPlayer->CanTrade() == FALSE)									goto	END;
	
	// 타 유저에게 파티 신청중이거나 
	// 다른 유저에 파티 신청을 받았다면 이를 취소한다.
	CloseRequestingJoinParty();

	bSuccess	=	TRUE;	

END:
	if(bSuccess == TRUE)
	{
		
		clTrade.SetState(TRADE_STATE_REQUESTING, LPARAM(pTargetPlayer->uiAccount));
		pTargetPlayer->clTrade.SetState(TRADE_STATE_REQUESTED, LPARAM(uiAccount));

		// 성공적으로 Trade를 하고자하는 플레이어에게 Trade를 원한다는 메시지를 보냈다.
		SendFieldMsg(ON_RESPONSE_TRADE, SO_SFM_ONLYME, LPVOID(ON_RET_OK));

		// Trade를 원하는 플레이어에게도 Trade를 원한다는 메시지를 보내준다.
		pTargetPlayer->SendFieldMsg(ON_RECEIVE_ASK_TRADE, SO_SFM_ONLYME, LPVOID(uiAccount), LPVOID(szID));
	}
	else
	{
		// Trade를 하고자하는 플레이어와 Trade를 하는것이 불가능하다.
		SendFieldMsg(ON_RESPONSE_TRADE, SO_SFM_ONLYME, LPVOID(ON_RET_NO));
	}
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	Trade를 하자고 요청한것에 대한 상대방의 응답을 받았다.
//	수정 일자	:	2002-07-04 오후 1:04:06 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
VOID	SOPlayer::RecvAcceptTrade()
{
	BOOL					bSuccess = FALSE;		
	SOPlayer				*pTargetPlayer;
	OnReqAcceptTradeMsg		*pAcceptTradeMsg;	
	
	pAcceptTradeMsg		=	(OnReqAcceptTradeMsg*)cMsg;

	// 현재 다른 플레이어가 나에게 교역을 요청했는지 알아온다.
	if(IsWaitingAcceptTrade() == FALSE)													goto	END;
	
	// 나와 교역하려는 플레이어의 객체를 얻어온다.
	if((pTargetPlayer =	pMyWorld->GetPlayer(clTrade.GetTradePlayerAccount())) == NULL)	goto	END;
	
	// 나와 교역하려는 플레이어 역시 교역을 요청하고 있는지 알아온다.
	if(pTargetPlayer->IsWaitingAcceptTrade(uiAccount) == FALSE)							goto	END;

	if(pAcceptTradeMsg->bAccept == TRUE)
	{
		// 교역을 할 수 있도록 설정한다.		
		AcceptedTrade( pTargetPlayer->uiGuildID, pTargetPlayer->siClassInGuild, pTargetPlayer->IFollower.GetFollowerParameterLevel( 0 ) );
		pTargetPlayer->AcceptedTrade( uiGuildID, siClassInGuild, IFollower.GetFollowerParameterLevel( 0 ) );
	}
	else
	{
		// 교역을 하지 않는다고 설정한다.	
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


//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	현재 진행중인 교역을 취소한다.
//	수정 일자	:	2002-07-04 오후 1:23:34 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
VOID	SOPlayer::RecvCancelTrade()
{	
	BOOL				bSuccess  = FALSE;	
	SOPlayer			*pTargetPlayer;	

	
	// 현재 내가 교역을 하고 있는지 알아온다.
	if(IsAcceptedTrading() == FALSE)														goto	END;

	// 나와 교역하려는 플레이어의 객체를 얻어온다.
	if((pTargetPlayer =	pMyWorld->GetPlayer(clTrade.GetTradePlayerAccount())) == NULL)		goto	END;

	// 대상 플레이어 역시 나와 교역중인지 알아온다.
	if(pTargetPlayer->IsAcceptedTrading(uiAccount) == FALSE)							goto	END;

	// 두 플레이어의 교역이 취소되었다.
	CancelTrade();	
	pTargetPlayer->CancelTrade();


	bSuccess	=	TRUE;

END:
	if(bSuccess == FALSE)
	{
		// 교역을 취소하는데 문제가 생겼다면 교역을 할 수 없다고 알려준다.
		SendFieldMsg(ON_RESPONSE_CANCEL_TRADE, SO_SFM_ONLYME, LPVOID(ON_RET_NO));
	}

	return;
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	Trade시 아이템 선택을 모두 끝냈다.
//	수정 일자	:	2002-07-06 오후 6:34:08 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
VOID	SOPlayer::RecvFinishChooseTradeItem()
{
	BOOL							bSuccess = FALSE;
	SOPlayer						*pTargetPlayer;	
	OnReqFinishChooseTradeItemMsg	*pFinishChooseTradeItemMsg;
	
	pFinishChooseTradeItemMsg	=	(OnReqFinishChooseTradeItemMsg*)cMsg;

	// 현재 내가 교역을 하고 있는지 알아온다.
	if(IsAcceptedTrading() == FALSE && IsChoseTradeItem() == FALSE)													goto	END;

	// 나와 교역하려는 플레이어의 객체를 얻어온다.
	if((pTargetPlayer =	pMyWorld->GetPlayer(clTrade.GetTradePlayerAccount())) == NULL)								goto	END;

	// 대상 플레이어 역시 나와 교역중인지 알아온다.
	if(pTargetPlayer->IsAcceptedTrading(uiAccount) == FALSE && pTargetPlayer->IsChoseTradeItem(uiAccount) == FALSE)	goto	END;

	if(pFinishChooseTradeItemMsg->bFinishChooseItem == TRUE)
	{
		// 교역할 아이템을 선택했다.
		if(ChooseTradeItem() == FALSE)																					goto	END;
	}
	else
	{
		// 고역할 아이템을 선택한 것을 취소한다.
		if(CancelChooseTradeItem() == FALSE)																			goto	END;
	}
	
	bSuccess	=	TRUE;
END:
	if(bSuccess == TRUE)
	{
		// 두 클라이언트에게 보내준다.
		SendFieldMsg(ON_RESPONSE_FINISH_CHOOSE_TRADEITEM, SO_SFM_ONLYME, LPVOID(ON_RET_OK), LPVOID(TRUE), LPVOID(pFinishChooseTradeItemMsg->bFinishChooseItem));
		pTargetPlayer->SendFieldMsg(ON_RESPONSE_FINISH_CHOOSE_TRADEITEM, SO_SFM_ONLYME, LPVOID(ON_RET_OK), LPVOID(FALSE), LPVOID(pFinishChooseTradeItemMsg->bFinishChooseItem));
	}
	else
	{
		// 교역할 아이템을 선택하는데 실패했다.
		SendFieldMsg(ON_RESPONSE_FINISH_CHOOSE_TRADEITEM, SO_SFM_ONLYME, LPVOID(ON_RET_NO));
	}
}


//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	Trade시 교역을 한다는 것을 결정한다.
//	수정 일자	:	2002-07-07 오전 1:05:27 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
VOID	SOPlayer::RecvDecideTrade()
{
	BOOL					bTradeSuccess	=	FALSE;
	DWORD					dwFailReason	=	ON_RET_NO;
	OnReqDecideTradeMsg		*pDecideTradeMsg;
	SOPlayer				*pTargetPlayer	=	NULL;		
	BOOL					bCancelTrade	=	FALSE;	
	BOOL					bInvalidMsg		=	FALSE;
	
	pDecideTradeMsg		=	(OnReqDecideTradeMsg*)cMsg;	

	// 현재 거래할 아이템들을 모두 선택했는지 알아온다.
	if(IsChoseTradeItem() == FALSE)															{	bInvalidMsg	=	TRUE;	goto	END; }
	
	// 나와 교역하려는 플레이어의 객체를 얻어온다.
	if((pTargetPlayer =	pMyWorld->GetPlayer(clTrade.GetTradePlayerAccount())) == NULL)		{	bCancelTrade =  TRUE;	goto	END; }

	// 나와 교역하려는 플레이어 역시 아이템을 모두 선택했는지 알아온다.
	if(pTargetPlayer->IsChoseTradeItem(uiAccount) == FALSE)									{	bInvalidMsg	=	TRUE;	goto	END; }

	if(pDecideTradeMsg->bDecideTrade == TRUE)
	{
		if(pTargetPlayer->IsDecideTrading() == TRUE)
		{			
			// 양쪽이 거래를 한다고 결정했다.

			// 우선 한쪽이라도 장부가 있는지 검사한다.
			if( ( clTrade.DoesExistTradeGoods( TRADE_GOODS_TRADE_BOOK ) == TRUE ) || 
				( pTargetPlayer->clTrade.DoesExistTradeGoods( TRADE_GOODS_TRADE_BOOK )  == TRUE ) )
			{
				// 양쪽에 장부를 전송중이라고 설정한다.
				clTrade.SetState( TRADE_STATE_SENDING_TRADE_BOOK );
				pTargetPlayer->clTrade.SetState( TRADE_STATE_SENDING_TRADE_BOOK );
				
				// 양쪽에 장부를 전송하라고 알려준다.
				SendFieldMsg( ON_RESPONSE_DECIDE_TRADE, SO_SFM_ONLYME, LPVOID( ON_RET_SEND_TRADE_BOOK ) );
				pTargetPlayer->SendFieldMsg( ON_RESPONSE_DECIDE_TRADE, SO_SFM_ONLYME, LPVOID( ON_RET_SEND_TRADE_BOOK ) );				
			}
			else
			{
				// 거래를 하도록 한다.
				if( Trade( pTargetPlayer ) == FALSE )										{	bCancelTrade = TRUE;	goto	END; }
			}
		}		
		else
		{	
			DecideTrade();
			// 거래 요청 정보를 송신, 자신/타인 setting
			SendFieldMsg( ON_RESPONSE_DECIDE_TRADE, SO_SFM_ONLYME, LPVOID( ON_RET_OK ), LPVOID(TRUE) );			
			pTargetPlayer->SendFieldMsg ( ON_RESPONSE_DECIDE_TRADE, SO_SFM_ONLYME, LPVOID( ON_RET_OK ), LPVOID(FALSE));
		}
	}
	else
	{
		// 이 부분은 아직 게임에서 사용하지 않는 부분이다.

		// 아이템 교역하려는 것을 취소했다.	

		// 아이템 교역하려는 것을 취소한다.
		if(CancelDecideTrading() == FALSE)													{	bCancelTrade = TRUE;	goto	END; }
		
		// 나에게는 명령이 제대로 접수되었다고 알려준다.
		SendFieldMsg( ON_RESPONSE_DECIDE_TRADE, SO_SFM_ONLYME, LPVOID( ON_RET_OK ) );

		// 교역 결정에 대해서 취소한다고 상대방에게 알려준다.
		pTargetPlayer->SendFieldMsg( ON_RESPONSE_DECIDE_TRADE, SO_SFM_ONLYME, LPVOID( ON_RET_CANCEL_DECIDE_TRADE ) );
	}		


END:

	if( bInvalidMsg	== TRUE )
	{
		SendFieldMsg( ON_RESPONSE_DECIDE_TRADE, SO_SFM_ONLYME, LPVOID( ON_RET_NO ) );
	}

	if( bCancelTrade == TRUE )
	{
		// 교역을 취소해야 한다.

		// 양쪽에게 교역을 취소하라고 알려준다.
		CancelTrade();	
		if( pTargetPlayer != NULL ) pTargetPlayer->CancelTrade();
	}
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	Trade시 교역중인 아이템을 수정한다.
//	수정 일자	:	2002-07-07 오전 5:15:07 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
VOID	SOPlayer::RecvChangeTradeGoods()
{
	OnReqChangeTradeGoods				*pChangeTradeGoods;
	BOOL								bSuccess = FALSE;
	SOPlayer							*pTargetPlayer;
	CHAR								cFollowerBuffer[10240];
	DWORD								dwFollowerDataSize	=	0;
	OnIndustrialComplexTradeInfo		IndustrialComplexTradeInfo;

	pChangeTradeGoods	=	(OnReqChangeTradeGoods*)cMsg;

	// 나와 교역하려는 플레이어의 객체를 얻어온다.
	if((pTargetPlayer =	pMyWorld->GetPlayer(clTrade.GetTradePlayerAccount())) == NULL)														goto	END;

	// 나와 교역하려는 플레이어가 내가 주려는 품목을 받을 수 있는지 알아온다.	
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


//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	Trade시 장부 거래가 완료되었다.
//	수정 일자	:	2002-09-25 오전 11:06:44 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
VOID	SOPlayer::RecvCompleteTradeBook()
{	
	OnReqCompleteTradeBoookMsg	*pCompleteTradeBoookMsg;
	SOPlayer					*pTargetPlayer;
	BOOL						bSuccess = FALSE;	

	pCompleteTradeBoookMsg		=	( OnReqCompleteTradeBoookMsg* )cMsg;

	// 내가 장부를 거래중인지 검사한다.
	if( clTrade.IsSendingTradeBook() == FALSE )														goto	END;

	// 나와 교역하려는 플레이어의 객체를 얻어온다.
	if( ( pTargetPlayer = pMyWorld->GetPlayer( clTrade.GetTradePlayerAccount() ) ) == NULL )		goto	END;

	// 나와 교역하려는 플레이어 역시 장부를 거래중인지 검사한다.
	if( ( pTargetPlayer->clTrade.IsSendingTradeBook() == FALSE ) && 
		( pTargetPlayer->clTrade.DoesCompleteSendingTradeBook() == FALSE ) )						goto	END;	

	switch( pCompleteTradeBoookMsg->dwResponse )
	{
	case ON_RET_OK:
		// 장부를 성공적으로 거래 완료했다.
		
		// 상대방 역시 장부 거래를 완료했는지 검사한다.
		if( pTargetPlayer->clTrade.DoesCompleteSendingTradeBook() == TRUE )
		{
			// 상대방 역시 장부 거래를 완료했다.

			// 교역을 한다.
			if( Trade( pTargetPlayer ) == FALSE )
			{
				// 두 플레이어의 교역을 취소 시킨다.				
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
		// 장부를 거래하는데 실패했다.		

		// 두 플레이어의 교역을 취소 시킨다.
		goto	CANCEL_TRADE;
	}

	bSuccess	=	TRUE;

END:
	if( bSuccess == FALSE )			SendFieldMsg( ON_RESPONSE_COMPLETE_TRADE_BOOK, SO_SFM_ONLYME, LPVOID( ON_RET_NO ) ); 		

	return;

CANCEL_TRADE:

	// 두 플레이어의 교역을 취소시킨다.
	CancelTrade();
	pTargetPlayer->CancelTrade();

	return;
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	교역을 수용했다.
//	수정 일자	:	2002-07-06 오후 2:14:32 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
BOOL	SOPlayer::AcceptedTrade( UI32 uiOppGuildID, SI32 siOppClassInGuild, SI32 siOppLevel )
{
	CHAR	*pszOppGuildID;
	CHAR	szOppGuildID[ ON_GUILDNAME_LENGTH + 1 ];	

	// 고역을 수용한다.
	if(clTrade.AcceptedTrade() == FALSE)	return	FALSE;	

	// 플레이어에게 교역을 수용하라고 보내준다.
	ZeroMemory( szOppGuildID, sizeof( szOppGuildID ) );
	if( uiOppGuildID != 0 )
	{	
		if( ( pszOppGuildID = pMyWorld->clIGuild.GetGuildName( uiOppGuildID ) ) != NULL )		
			strncpy( szOppGuildID, pszOppGuildID, ON_ID_LENGTH );		
	}

	SendFieldMsg( ON_RESPONSE_ACCEPT_TRADE, SO_SFM_ONLYME, LPVOID( ON_RET_OK ), LPVOID( siOppLevel ), LPVOID( szOppGuildID ), LPVOID( siOppClassInGuild ) );

	return	TRUE;
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	교역을 수용하지 않았다.
//	수정 일자	:	2002-07-06 오후 3:15:10 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
BOOL	SOPlayer::NotAcceptedTrade()
{
	// 고역을 수용한다.
	if(clTrade.NotAcceptedTrade() == FALSE)	return	FALSE;
	
	// 플레이어에게 교역을 못한다고 알려준다.
	SendFieldMsg(ON_RESPONSE_ACCEPT_TRADE, SO_SFM_ONLYME, LPVOID(ON_RET_NO));

	return	TRUE;
}


//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	현재 진행중인 교역을 취소한다.
//	수정 일자	:	2002-07-06 오후 2:37:49 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
BOOL	SOPlayer::CancelTrade()
{
	// 교역을 취소한다.
	if(clTrade.CancelTrade() == FALSE)	return	FALSE;
	
	// 플레이어게 교역이 취소되었다고 보내준다.
	SendFieldMsg(ON_RESPONSE_CANCEL_TRADE, SO_SFM_ONLYME, LPVOID(ON_RET_OK));

	return	TRUE;
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	현재 다른 플레이어가 교역을 수락해서 교역을 하고 있는지 알아온다.
//	수정 일자	:	2002-07-06 오후 2:48:53 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
BOOL	SOPlayer::IsAcceptedTrading(UI16 uiAccount)
{
	return	 clTrade.IsAcceptedTrading(uiAccount);
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	상대방이 교역을 승낙하는 것을 대기하는지 알아온다.
//	수정 일자	:	2002-07-06 오후 3:10:07 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
BOOL	SOPlayer::IsWaitingAcceptTrade(UI16 uiAccount)
{
	return	clTrade.IsWaitingAcceptTrade(uiAccount);
}


//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	교역할 아이템을 선택했다.
//	수정 일자	:	2002-07-06 오후 6:41:43 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
BOOL	SOPlayer::ChooseTradeItem()
{
	return	clTrade.ChooseTradeItem();
}


//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	교역하려고 했던 것을 취소한다.
//	수정 일자	:	2002-07-07 오전 2:06:28 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
BOOL	SOPlayer::CancelDecideTrading()
{
	return	clTrade.CancelDecideTrading();
}


//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	교역을 하려고 결정을 했는지 알아온다.
//	수정 일자	:	2002-07-07 오전 2:18:10 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
BOOL	SOPlayer::IsDecideTrading(UI16 uiAccount)
{
	return	clTrade.IsDecideTrading();
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	교역 물품을 추가한다.
//	수정 일자	:	2002-09-30 오후 3:45:07 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
BOOL	SOPlayer::AddTradeGoods(UI08 uiSlotID, TradeGoodsKind Kind, LPARAM lParam1, LPARAM lParam2, OnFollowerInfo *pTradeFollowerInfo,
								DWORD *pdwFollowerDataSize, OnIndustrialComplexTradeInfo *pIndustrialComplexTradeInfo )
{
	UI16							uiFollowerID;
	UI16							uiItemID, uiQuantity;
	MyItemData						*pItemInfo;
	SOVillage_IndustrialComplex		*pIndustrialComplex;


	switch( Kind ) 
	{
	case TRADE_GOODS_ITEM :						// 아이템을 교역한다.		
		

		// 용병을 알아낸다.
		uiFollowerID	=	UI16(lParam1);	

		// 아이템의 ID를 알아낸다.
		uiItemID		=	HIWORD(lParam2);

		// 현재 거래중인 아이템인지 검사한다.
		// 거래중인 아이템을 다른 개수로 하여 다시 올리수는 없다.
		if( clTrade.IsTradingItem( uiFollowerID, uiItemID ) == TRUE )					return	FALSE;

		// 아이템의 수량을 알아낸다.
		uiQuantity		=	LOWORD(lParam2);

		// 해당 용병에게 이 아이템이 수량만큼 있는지 알아낸다.
		if((pItemInfo  = IFollower.GetItemByID(uiFollowerID, uiItemID)) == NULL)		return	FALSE;

		// 아이템의 수량이 되는지 체크한다.
		if(pItemInfo->uiQuantity < uiQuantity)											return	FALSE;

		break;


	case TRADE_GOODS_FOLLOWER :					// 용병을 교역한다.

		// 용병을 알아낸다.
		uiFollowerID	=	UI16(lParam1);		

		// 교역하려는 용병의 정보를 얻어온다.
		//if(IFollower.GetFollowerTradeInfo(uiFollowerID, pTradeFollowerInfo) == FALSE)	return	FALSE;
		if( (*pdwFollowerDataSize = GetFollowerInfo( uiFollowerID, pTradeFollowerInfo )) == 0 )		return FALSE;

		break;


	case TRADE_GOODS_MONEY : 					// 돈을 교역한다.		

		// 돈이 충분히 있는지 검사한다.
		if(UI32(lParam1) > siMoney )													return	FALSE;

		break;

	case TRADE_GOODS_REALESTATE :				// 부동산을 교역한다.

		// 내가 부동산을 가지고 있는지 검사한다.
		if( ( pIndustrialComplex = pMyWorld->GetIndustrialComplex( this ) ) == NULL )	return	FALSE;
		
		pIndustrialComplex->GetTradeInfo( pIndustrialComplexTradeInfo );	
		
		break;

	default:
		return	FALSE;

	}

	return	clTrade.AddTradeGoods(uiSlotID, Kind, lParam1, lParam2);
	
}


//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	상대방이 주려고 하는 물품들을 모두 받을 수 있는지 체크한다.
//	수정 일자	:	2002-07-07 오전 6:23:43 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
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

	// 나와 교역하려는 플레이어의 객체를 얻어온다.
	if((pTargetPlayer =	pMyWorld->GetPlayer(clTrade.GetTradePlayerAccount())) == NULL)	return	FALSE;

	// 상대방이 주려고 하는 물품들을 얻어온다.
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
			uiFollowerNum++;										// 총 용병의 수
			if( pTargetPlayer->IsMonster(uiFollowerID) == TRUE )	// 총 몬스터의 수
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

		// 아이템을 추가 시킬 수 있는지 알아온다.
		// 지금은 오로지 주인공 캐릭터만 받는다.
		if(CanAddItem(0, uiItemID, uiItemQuantity, uiItemNum) == FALSE)										return	FALSE;
		break;


	case TRADE_GOODS_FOLLOWER:
	{
		uiFollowerID					=	UI16(lParam1);		

		// 내가 최대 가질 수 있는 용병보다 초과하지 않는지 알아온다.
		if(uiFollowerNum >= GetMaxFollowerNum())															return	FALSE;

		// 만약 넘겨주려는 캐릭터가 주인공 캐릭터라면 실패한다.
		if(uiFollowerID == 0)																				return	FALSE;

		// 넘겨주려는 캐릭터가 유효한지 알아온다.		
		if(pTargetPlayer->IFollower.IsValidFollower(uiFollowerID) == FALSE)									return	FALSE;
		
		// 상대방이 장수를 주려고 하는지 알아온다.
		if(pTargetPlayer->IsGeneral(uiFollowerID) == TRUE)					 								return	FALSE;

		// 상대방이 몬스터를 주려고 하는 경우 이미 갖고 있는 몬스터수와 거래창에 올라온 몬스터수의 합은 ON_MAXMONSTER보다 작아야 한다.
		if( pTargetPlayer->IsMonster(uiFollowerID) == TRUE )
		{
			if( GetMonsterNum() + uiMonsterNum >= ON_MAXMONSTER )											return	FALSE;
		}

		uiFollowerNum++;	

		// 용병을 추가 시킬 수 있는지 알아온다.
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

		// 내가 가질 수 있는 돈보다 많은 돈을 주려고 하는지 알아온다.
		if( GetMaxMoneyToGet() < UI32( lParam1 ) )															return	FALSE;

		break;

	case TRADE_GOODS_REALESTATE:

		// 이미 내가 부동산을 가지고 있는지 검사한다.
		if( pMyWorld->IsIndustrialComplexOwner( this ) == TRUE )											return	FALSE;
		
		//신용도가 30 미만 이라면 사고 팔 수 없다. - 상민  2003. 2. 27- 
		if ( GetTradeGrade() < 30 )
		{
			return FALSE;
		}


		break;

	}

	return	TRUE;
}


//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	상대방과 교역을 한다.
//	수정 일자	:	2002-09-30 오후 3:18:08 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
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

	// 값을 초기화 한다.
	*pdwTradeItemDataSize	=	0;
	*pdwFollowerDataSize	=	0;
	*puiFollowerNum			=	0;		
	*puiTradeItemNum		=	0;

	// 상대방이 주려고 했던 품목들을 모두 받아온다.	
	pstTradeGoods	=	pTargetPlayer->clTrade.GetTradeGoods();
	
	// 돈을 받는다.
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
		case TRADE_GOODS_ITEM :					// 아이템이다.			

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

		case TRADE_GOODS_FOLLOWER :				// 용병이다.

			uiFollowerID	=	UI16(pstTradeGoods[i].lParam1);

			// 용병을 받는다.			
			if( ( dwFollowerDataSize = pTargetPlayer->TradeFollower( this, uiFollowerID, pFollowerInfo, &uiOppFollowerID ) ) != 0 )
			{
				// 성공적으로 용병을 받았다.								
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
	// 내가 주려고 했던 것들을 모두 없앤다.
	
	// 내가 주려고 했던 품목들을 모두 받아온다.	
	pstTradeGoods	=	clTrade.GetTradeGoods();

	// 돈을 없앤다.
	uiTradeMoney	=	clTrade.GetTradeMoney();
	DecreaseMoney(uiTradeMoney);
	
	for(i = 0; i < MAX_TRADE_WINDOW; i++)
	{	
		switch( pstTradeGoods[i].Kind )
		{
		case TRADE_GOODS_ITEM :				// 아이템이다.
			
			uiFollowerID	=	UI16(pstTradeGoods[i].lParam1);
			uiItemID		=	HIWORD(pstTradeGoods[i].lParam2);
			uiItemQuantity	=	LOWORD(pstTradeGoods[i].lParam2);
			
			DelItem(uiFollowerID, uiItemID, uiItemQuantity);

			break;

		case TRADE_GOODS_FOLLOWER :			// 용병이다.

			// 용병은 상대방이 나의 Follower를 가져가면서 없어진다.

			break;
		}
	}		
*/
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	pTradePlayer에게 uiFollowerID의 용병을 준다.
//	수정 일자	:	2002-07-07 오전 11:45:11 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
DWORD	SOPlayer::TradeFollower(SOPlayer *pTradePlayer, UI16 uiFollowerID, OnFollowerInfo *pFollowerInfo, UI08 *puiOppFollowerID)
{
	DWORD				dwFollowerDataSize;
	SI32				siNation;
	//DBIKGiveFollower	DBGiveFollower;
	SI16				siEmptyFollowerSlot;

	// 주인공이면 못준다.
	if(uiFollowerID == 0)		return	0;

	// 용병의 정보를 얻어오는데 성공해야한다.
	if((dwFollowerDataSize = GetFollowerInfo(uiFollowerID, pFollowerInfo)) == 0)		return	0;

	// 용병의 국가를 얻어온다.
	siNation	=	IFollower.GetFollowerParameterNation(uiFollowerID);

	// 우선 내 용병은 삭제한다. 
	if(IFollower.DeleteFollower(uiFollowerID) == FALSE)									return	0;
	
	// 용병을 받는 플레이어가 용병을 어디에 추가시켜야 하는지 알아온다.
	if((siEmptyFollowerSlot = pTradePlayer->IFollower.FindEmptyFollowerSlot()) == -1)	return	0;

	// 상대방에게 내 용병을 추가시켜준다.
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


//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	교역창을 닫는다.
//	수정 일자	:	2002-07-08 오후 8:08:38 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
VOID	SOPlayer::CloseTrade()
{	
	SOPlayer		*pTargetPlayer;		

	// 현재 교역중이 아니라면 그냥 나간다.
	if(IsTrading() == FALSE)															return;

	// 나와 교역하려는 플레이어의 객체를 얻어온다.
	if((pTargetPlayer =	pMyWorld->GetPlayer(clTrade.GetTradePlayerAccount())) == NULL)	return;

	// 상대방이 교역에 응답하기를 기다리고 있는 상태와
	// 교역중에 있을때는 메시지를 달리해준다.
	if(pTargetPlayer->IsWaitingAcceptTrade(uiAccount) == TRUE)		pTargetPlayer->SendFieldMsg(ON_RESPONSE_ACCEPT_TRADE, SO_SFM_ONLYME, LPVOID(ON_RET_NO));
	if(pTargetPlayer->IsAcceptedTrading(uiAccount) == TRUE)			pTargetPlayer->SendFieldMsg(ON_RESPONSE_CANCEL_TRADE, SO_SFM_ONLYME, LPVOID(ON_RET_NO));
	
	// 나와 상대방의 교역창을 초기화 한다.
	clTrade.Init();
	pTargetPlayer->clTrade.Init();
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	내가 교역하려는 품목이 아직 유효한지 검사한다
//	수정 일자	:	2002-09-23 오후 12:19:38 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
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

			// 아이템을 받는다.
			uiFollowerID	=	UI16(pstTradeGoods[i].lParam1);
			uiItemID		=	HIWORD(pstTradeGoods[i].lParam2);
			uiItemQuantity	=	LOWORD(pstTradeGoods[i].lParam2);
			
			if( ( pMyItemData =	IFollower.GetItemByID( uiFollowerID, uiItemID ) ) == NULL)	return	FALSE;			
			
			// 수량을 검사한다.
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

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	교역중인 품목에 대해서 거래를 하라고 알려준다.
//	수정 일자	:	2002-09-24 오후 8:09:18 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
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

	// 나와 상대방이 교역하려는 품목들이 모두 유효한지 알아온다.
	if(IsValidTradeGoods() == FALSE)																		goto	END;
	if(pTargetPlayer->IsValidTradeGoods() == FALSE)															goto	END;							

	// 상대방 역시 아이템을 교역할 것을 결정했다.
	TradeGoods(pTargetPlayer, &dwFollowerDataSize1, (OnFollowerInfo*)cFollowerBuffer1, &uiFollowerNum1, 
					&dwTradeItemDataSize1, uiTradeItem1, &uiTradeItemNum1, szTrade2DBArg );
	pTargetPlayer->TradeGoods(this, &dwFollowerDataSize2, (OnFollowerInfo*)cFollowerBuffer2, &uiFollowerNum2,
					&dwTradeItemDataSize2, uiTradeItem2, &uiTradeItemNum2, szTrade1DBArg );
	
	// 다시 교역을 할 수 있는 상태로 설정한다.
	clTrade.Init();
	pTargetPlayer->clTrade.Init();
	
	// DB 명령문을 만든다.
	sprintf( szAccountDBArg, "%d, %d, %d, %d",	stAccountInfo.siDBAccount, stAccountInfo.uiSlot, 
													pTargetPlayer->stAccountInfo.siDBAccount, pTargetPlayer->stAccountInfo.uiSlot );	

	// DB에 저장한다.
	ZeroMemory( szCompletedDBArg, sizeof( szCompletedDBArg ) );
	sprintf( szCompletedDBArg, "%s, %s, %s", szAccountDBArg, szTrade1DBArg, szTrade2DBArg );
	pMyWorld->clDatabase->writeDBQuery( SO_DB_INFOKIND_TRADE, LPVOID( szCompletedDBArg ), strlen( szCompletedDBArg ) + 1 );

	bSuccess	=	TRUE;

END:

	if( bSuccess == TRUE)
	{
		// 교역이 성립되었다.
		SendFieldMsg( ON_COMPLETE_TRADE, SO_SFM_ONLYME, LPVOID( dwFollowerDataSize1 ), LPVOID( cFollowerBuffer1 ), LPVOID( uiFollowerNum1 ),
										LPVOID( dwTradeItemDataSize1 ), LPVOID( uiTradeItem1 ), LPVOID( uiTradeItemNum1 ) );
		pTargetPlayer->SendFieldMsg( ON_COMPLETE_TRADE , SO_SFM_ONLYME, LPVOID( dwFollowerDataSize2 ), 	LPVOID( cFollowerBuffer2 ), LPVOID( uiFollowerNum2 ),
										LPVOID( dwTradeItemDataSize2 ), LPVOID( uiTradeItem2 ), LPVOID( uiTradeItemNum2 ) );	
	}
	else
	{
		// 교역이 실패했다. 
		// 교역을 취소 시킨다.

		CancelTrade();
		pTargetPlayer->CancelTrade();
	}
	
	return	bSuccess;
}
	
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	아이템을 소모한다.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
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
			// 포만감은 그냥오른다.
			if(pItemHeader->m_siHealth != 0)
			{
				if(pItemHeader->m_siGroupCode &  ON_ITEM_ALL_RECOVER )
					ChangeSatiety( static_cast<SI32>((float)ON_MAX_HEALTH * ((float)pItemHeader->m_siHealth / 100.0)) );
				else
					ChangeSatiety(pItemHeader->m_siHealth);
			}
			
			// 전체적용아이템이 아니면 
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

				// 특수 효과가 있는 아이템이다.
				if (pItemHeader->m_siGroupCode & ON_ITEM_SPECIAL)
				{

					switch(pItemHeader->m_siSpecial)
					{
					case ON_ITEM_SPECIAL_INITIALIZE:
						{							
							OnlineFollower *pFollower = IFollower.GetFollower(uiFollowerID);
							
							SI32 siBonusDelta;
							
							if (pFollower == NULL) return FALSE;
							
							// 여기서 uiFollowerID 는 몇번째 용병이냐를 뜻하고
							// uiCharID 는 용병 고유의 아이디이다.
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
							// 0번용병이 쓰면 실패
							if(uiFollowerID == 0)
							{
								return FALSE;
							}
							// 이미 이용병에 적용돼어 있으면 실패
							if(siVMercenarySlot == uiFollowerID)
							{
								return FALSE;
							}

							// 용병 보임 설정
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


							// 0번용병이 아니면 실패
							if(uiFollowerID != 0)
							{
								return FALSE;
							}

							OnlineFollower *pFollower = IFollower.GetFollower(uiFollowerID);

							// 입고있는 장비를 얻어온다.
							UI16 puiWearItems[WEAR_ITEM_NUM];
							pFollower->GetWearItems(puiWearItems);

							// 무기를 장비하고있으면 실패
							if (puiWearItems[ON_WEAR_WEAPON_ITEM] != 0)							
							{
								return FALSE;
							}

							switch (pFollower->uiCharID)
							{
								// 조선남
							case 67:
								pFollower->uiCharID = 323;
								uiKind = pFollower->uiCharID;
								break;
								// 조선녀
							case 323:
								pFollower->uiCharID = 67;
								uiKind = pFollower->uiCharID;
								
								break;
								// 일본남
							case 8771:
								pFollower->uiCharID = 9027;
								uiKind = pFollower->uiCharID;
								
								break;
								// 일본녀
							case 9027:
								pFollower->uiCharID = 8771;
								uiKind = pFollower->uiCharID;

								break;
								// 대만남
							case 10307:
								pFollower->uiCharID = 10563;
								uiKind = pFollower->uiCharID;
								break;
								// 대만녀
							case 10563:
								pFollower->uiCharID = 10307;
								uiKind = pFollower->uiCharID;
								break;
								// 중국남
							case 12867:
								pFollower->uiCharID = 13123;
								uiKind = pFollower->uiCharID;
								break;
								// 중국녀
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
							
							// 용병 보임 설정
							

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
							//공성을 할수 있는 날인가 체크.
							if(pMyWorld->clIGuild.GetGuildWarCheck(uiGuildID) == FALSE)
							{
								return FALSE;
							}
							OnlineFollower *pFollower = IFollower.GetFollower(uiFollowerID);
							
							//공성유닛->공성유닛 으로 변한 경우라면 저장해 놓은 PrevKind를 다시 저장.
							//그렇지 않은 경우라면 일반 유닛의 PrevKind를 저장.
							if(pMyWorld->m_FieldBattleParser.IsFieldAttackUnit(uiKind) == TRUE)
								uiPrevKind = GetPrevKind();								
							else
								uiPrevKind = uiKind;

							//Character 와 Mercenary 의 캐릭터 Kind둘다 변경한다.
							//서버에선 둘의 Kind만 변경해주고 DB에 저장할때는 CharactorWar테이블에 저장한다.
							uiKind				= pItemHeader->m_pImperium->m_uiKind;
							pFollower->uiCharID = pItemHeader->m_pImperium->m_uiKind;
							
							//uiQuantity : 대장의 경우 2장의 소집권을 사용한다.
							//             1장일 경우 일반유닛 2장일 경우 대장유닛.
							//대장일경우엔 대장이라는 표시를 보내고 체력을 두배로 만든다.
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
							ImperumUse.uiItemID		   = uiItemID; //Item ID로 가지 않고 Index로 갈듯.
							ImperumUse.uiQuantity	   = uiQuantity;
							ImperumUse.bCaptain		   = bCaptain;
							ImperumUse.siHp			   = siHp;
							ImperumUse.siSupply		   = 0;        // 일반 유닛은 보급 게이지를 0으로 한다.
*/
							if(uiQuantity == 1)
								ImperumUse.bCaptain		   = FALSE;
							else 
								ImperumUse.bCaptain		   = TRUE;
							
							SetFieldHp(siHp);			//체력   설정.

							//pMyWorld->m_FieldBattleParser.
							
							SetPrevKind(uiPrevKind);								//Prev Kind 설정.
							SetCaptainUnit(bCaptain);								//대장유닛인지 설정.

							//일반 유저들은 보급을 설정한다.
							//일반 공성유닛의 보급량은 서버에서 관리하지 않는다.
							if(pMyWorld->m_FieldBattleParser.GetUnitKind(pItemHeader->m_pImperium->m_uiKind) != FIELD_BATTLE_SUPPLY)
							{
	//							SetFieldSupply((SI32)pItemHeader->m_pImperium->m_siSupply);   //보급량 설정.
	//							InitFieldSupplyCharge(0);
							}
							//보급수레는 보급충전량을 설정한다.
							else if(pMyWorld->m_FieldBattleParser.GetUnitKind(pItemHeader->m_pImperium->m_uiKind) == FIELD_BATTLE_SUPPLY)
							{
	//							InitFieldSupplyCharge((SI32)pItemHeader->m_pImperium->m_siSupply); // 보급충전량 설정.
//								ImperumUse.siSupply = (SI16)pItemHeader->m_pImperium->m_siSupply; // 디비에 보급충전 게이지를 저장.
							}

							pMyWorld->clDatabase->writeDBQuery(SO_DB_INFOKIND_IMPERIUM_USE, LPVOID(&ImperumUse), sizeof(DBIKImperumUse));
							bSendDeleteQuery = FALSE;

						}
						break;
					case ON_ITEM_SPECIAL_CHANGENATION:
						{
							// 0번용병이 아니면 실패
							if(uiFollowerID != 0)
							{
								return FALSE;
							}

							OnlineFollower *pFollower = IFollower.GetFollower(uiFollowerID);

							

							// 입고있는 장비를 얻어온다.
							UI16 puiWearItems[WEAR_ITEM_NUM];
							pFollower->GetWearItems(puiWearItems);

							// 무기를 장비하고있으면 실패
							if (puiWearItems[ON_WEAR_WEAPON_ITEM] != 0)							
							{
								return FALSE;
							}

							// 국적이 변하려는 국가와 같으면 실패
							if (pFollower->Nation == pItemHeader->m_siSpecialParam) return FALSE;


							// 캐릭 kind 변경
							switch(pItemHeader->m_siSpecialParam)
							{
							// 조선귀화
							case 1:
								switch (pFollower->uiCharID)
								{
									// 남자
								case 67:
								case 8771:
								case 10307:
								case 12867:
									// 조선남으로 변신
									pFollower->uiCharID = 67;
									uiKind = pFollower->uiCharID;
								
									
									break;
									// 여자.
								case 323:
								case 9027:
								case 10563:
								case 13123:
									pFollower->uiCharID = 323;
									uiKind = pFollower->uiCharID;
								
									break;
								}
								
								break;
							// 일본귀화
							case 10:
								switch (pFollower->uiCharID)
								{
									// 남자
								case 67:
								case 8771:
								case 10307:
								case 12867:

									pFollower->uiCharID = 8771;
									uiKind = pFollower->uiCharID;
								
									break;
									// 여자.
								case 323:
								case 9027:
								case 10563:
								case 13123:

									pFollower->uiCharID = 9027;
									uiKind = pFollower->uiCharID;
								
									break;
								}
								
								break;
							// 대만귀화
							case 1000:
								switch (pFollower->uiCharID)
								{
									// 남자
								case 67:
								case 8771:
								case 10307:
								case 12867:

									pFollower->uiCharID = 10307;
									uiKind = pFollower->uiCharID;
								
									break;
									// 여자.
								case 323:
								case 9027:
								case 10563:
								case 13123:

									pFollower->uiCharID = 10563;
									uiKind = pFollower->uiCharID;
								
									break;
								}
								
								break;
							// 중국귀화
							case 100:
								switch (pFollower->uiCharID)
								{
									// 남자
								case 67:
								case 8771:
								case 10307:
								case 12867:
									pFollower->uiCharID = 12867;
									uiKind = pFollower->uiCharID;								
									break;
									// 여자.
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

							// 국적변경
							pFollower->Nation = pItemHeader->m_siSpecialParam;

							// 퀘스트 삭제

							Quest_AllClear();

							
							// 변신 메시지 날림

							if (IsInVillage()) {
								SendFieldMsg(ON_CHANGE_CHARACTERCODE, SO_SFM_ONLYME, LPVOID(this),LPVOID(pFollower->uiCharID));
							}
							else {
								SendFieldMsg(ON_CHANGE_CHARACTERCODE, SO_SFM_MEANDNEARCHAR, LPVOID(this),LPVOID(pFollower->uiCharID));
							}

							
							
							// 변신 db쿼리날림
							

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
					case ON_ITEM_SPECIAL_TACTICS:		// 손자 병법 사용으로 최대 고용 장수 증가 최대 5명 까지 증가 가능함.
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
					case ON_ITEM_SPECIAL_DONKEY:		// 당나귀를 사용하면 용병 슬롯에 당나귀가 한마리 들어간다.
						{
							if( !OnUseDonkeyItem(uiFollowerID, uiItemID) )		return FALSE;
							bSendDeleteQuery	=	FALSE;
						}
						break;

					case ON_ITEM_SPECIAL_MAGICSTONERED:
					case ON_ITEM_SPECIAL_MAGICSTONEBLUE:
					case ON_ITEM_SPECIAL_MAGICSTONEBLACK:
						{
							if( IFollower.CanWeaponBeUpgraded(uiFollowerID) )					// 업그레이드 가능한 무기인지 검사한다.
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
								// 해당 용병이 착용중인 아이템들을 얻어온다.
								IFollower.GetWearItems( uiFollowerID, puiWearItems );
								// 해당 마석으로 착용중인 무기 아이템을 가공한 결과를 얻는다.
								stItemUpgradeResultInfo		=	pMyWorld->m_ItemUpgradeParser.GetUpgradeResult( puiWearItems[ON_WEAR_WEAPON_ITEM], siMagicStoneKind );

								// 가공 결과 가공 불가능이면 그냥 리턴시킨다. (마석은 소모되지 않는다.)
								if( stItemUpgradeResultInfo->siResult == ITEM_UPGRADE_IMPOSSIBLE )			return FALSE;
								
								if( stItemUpgradeResultInfo->siResult == ITEM_UPGRADE_SUCCESS )
								{
									// 아이템을 업그레이드 해주고 DB에 성공 쿼리를 날려준다.
									IFollower.UpgradeWeapon( uiFollowerID, stItemUpgradeResultInfo->uiItemID );		// 아이템 업그레이드

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
									// DB에 실패 쿼리만 날려준다.
									IFollower.UpgradeWeapon( uiFollowerID, stItemUpgradeResultInfo->uiItemID );		// 아이템 업그레이드
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
									// 아이템이 파괴되었으므로 삭제하고 DB에 파괴되었다는 쿼리를 날려준다.
									IFollower.BrakeWeapon( uiFollowerID );											// 아이템 파괴

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
				}	// end of if(특수효과가 있는 아이템이다)
			}	// end of if(전체 적용 아이템이 아니다)
			// 전체적용아이템이면
			else {
				
				SI32 siHpGain = pItemHeader->m_siHpGain;
				SI32 siMpGain = pItemHeader->m_siMpGain;
				
				SI32 siMaxFollower = ON_MAX_FOLLOWER_NUM;
				
				for (int i = 0; i < siMaxFollower; i++)
				{
					// 존재하는 부하이면 
					if (IFollower.IsValidFollower(i))
					{					
						// parameterbox를 구해서
						pParaBox	= &IFollower.GetFollower(i)->ParametaBox;
						if(pParaBox)
						{
							// 부활아이템이아니면 죽은용병은 치료하지않는다.
							if ( !(pItemHeader->m_siGroupCode & ON_ITEM_REVIVE) )
							{
								if (pParaBox->IP.GetLife() <= 0) continue;								
							}
							

							// 전체 회복 | 100%로 회복
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
								// 체력 마력 회복!
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





