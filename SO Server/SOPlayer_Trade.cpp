#include "SOMain.h"
#include "SOWorld.h"
#include "SOBaseChar.h"
#include "OnlineProtocol.h"
#include "OnlineCommonStruct.h"
#include "OnlineGameMsg.h"


//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	Trade�� ���ڰ� ��û�Ѵ�. 
//	���� ����	:	2002-07-04 ���� 10:28:41 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
VOID	SOPlayer::RecvAskTrade()
{
	SOPlayer				*pTargetPlayer;
	OnReqAskTradeMsg		*pAskTradeMsg;
	BOOL					bSuccess  = FALSE;

	pAskTradeMsg	=	(OnReqAskTradeMsg*) cMsg;	

	// ���� ���� Trade�� �������� �˾ƿ´�.
	if(CanTrade() == FALSE)													goto	END;

	// Trade�� �ϰ����ϴ� �÷��̾ ��ȿ���� �˻��Ѵ�.
	if((pTargetPlayer = pMyWorld->GetPlayer(pAskTradeMsg->UserID)) == NULL)	goto	END;		

	// Target �÷��̾ Trade�� �������� �˾ƿ´�.
	if(pTargetPlayer->CanTrade() == FALSE)									goto	END;
	
	// Ÿ �������� ��Ƽ ��û���̰ų� 
	// �ٸ� ������ ��Ƽ ��û�� �޾Ҵٸ� �̸� ����Ѵ�.
	CloseRequestingJoinParty();

	bSuccess	=	TRUE;	

END:
	if(bSuccess == TRUE)
	{
		
		clTrade.SetState(TRADE_STATE_REQUESTING, LPARAM(pTargetPlayer->uiAccount));
		pTargetPlayer->clTrade.SetState(TRADE_STATE_REQUESTED, LPARAM(uiAccount));

		// ���������� Trade�� �ϰ����ϴ� �÷��̾�� Trade�� ���Ѵٴ� �޽����� ���´�.
		SendFieldMsg(ON_RESPONSE_TRADE, SO_SFM_ONLYME, LPVOID(ON_RET_OK));

		// Trade�� ���ϴ� �÷��̾�Ե� Trade�� ���Ѵٴ� �޽����� �����ش�.
		pTargetPlayer->SendFieldMsg(ON_RECEIVE_ASK_TRADE, SO_SFM_ONLYME, LPVOID(uiAccount), LPVOID(szID));
	}
	else
	{
		// Trade�� �ϰ����ϴ� �÷��̾�� Trade�� �ϴ°��� �Ұ����ϴ�.
		SendFieldMsg(ON_RESPONSE_TRADE, SO_SFM_ONLYME, LPVOID(ON_RET_NO));
	}
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	Trade�� ���ڰ� ��û�ѰͿ� ���� ������ ������ �޾Ҵ�.
//	���� ����	:	2002-07-04 ���� 1:04:06 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
VOID	SOPlayer::RecvAcceptTrade()
{
	BOOL					bSuccess = FALSE;		
	SOPlayer				*pTargetPlayer;
	OnReqAcceptTradeMsg		*pAcceptTradeMsg;	
	
	pAcceptTradeMsg		=	(OnReqAcceptTradeMsg*)cMsg;

	// ���� �ٸ� �÷��̾ ������ ������ ��û�ߴ��� �˾ƿ´�.
	if(IsWaitingAcceptTrade() == FALSE)													goto	END;
	
	// ���� �����Ϸ��� �÷��̾��� ��ü�� ���´�.
	if((pTargetPlayer =	pMyWorld->GetPlayer(clTrade.GetTradePlayerAccount())) == NULL)	goto	END;
	
	// ���� �����Ϸ��� �÷��̾� ���� ������ ��û�ϰ� �ִ��� �˾ƿ´�.
	if(pTargetPlayer->IsWaitingAcceptTrade(uiAccount) == FALSE)							goto	END;

	if(pAcceptTradeMsg->bAccept == TRUE)
	{
		// ������ �� �� �ֵ��� �����Ѵ�.		
		AcceptedTrade( pTargetPlayer->uiGuildID, pTargetPlayer->siClassInGuild, pTargetPlayer->IFollower.GetFollowerParameterLevel( 0 ) );
		pTargetPlayer->AcceptedTrade( uiGuildID, siClassInGuild, IFollower.GetFollowerParameterLevel( 0 ) );
	}
	else
	{
		// ������ ���� �ʴ´ٰ� �����Ѵ�.	
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


//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	���� �������� ������ ����Ѵ�.
//	���� ����	:	2002-07-04 ���� 1:23:34 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
VOID	SOPlayer::RecvCancelTrade()
{	
	BOOL				bSuccess  = FALSE;	
	SOPlayer			*pTargetPlayer;	

	
	// ���� ���� ������ �ϰ� �ִ��� �˾ƿ´�.
	if(IsAcceptedTrading() == FALSE)														goto	END;

	// ���� �����Ϸ��� �÷��̾��� ��ü�� ���´�.
	if((pTargetPlayer =	pMyWorld->GetPlayer(clTrade.GetTradePlayerAccount())) == NULL)		goto	END;

	// ��� �÷��̾� ���� ���� ���������� �˾ƿ´�.
	if(pTargetPlayer->IsAcceptedTrading(uiAccount) == FALSE)							goto	END;

	// �� �÷��̾��� ������ ��ҵǾ���.
	CancelTrade();	
	pTargetPlayer->CancelTrade();


	bSuccess	=	TRUE;

END:
	if(bSuccess == FALSE)
	{
		// ������ ����ϴµ� ������ ����ٸ� ������ �� �� ���ٰ� �˷��ش�.
		SendFieldMsg(ON_RESPONSE_CANCEL_TRADE, SO_SFM_ONLYME, LPVOID(ON_RET_NO));
	}

	return;
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	Trade�� ������ ������ ��� ���´�.
//	���� ����	:	2002-07-06 ���� 6:34:08 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
VOID	SOPlayer::RecvFinishChooseTradeItem()
{
	BOOL							bSuccess = FALSE;
	SOPlayer						*pTargetPlayer;	
	OnReqFinishChooseTradeItemMsg	*pFinishChooseTradeItemMsg;
	
	pFinishChooseTradeItemMsg	=	(OnReqFinishChooseTradeItemMsg*)cMsg;

	// ���� ���� ������ �ϰ� �ִ��� �˾ƿ´�.
	if(IsAcceptedTrading() == FALSE && IsChoseTradeItem() == FALSE)													goto	END;

	// ���� �����Ϸ��� �÷��̾��� ��ü�� ���´�.
	if((pTargetPlayer =	pMyWorld->GetPlayer(clTrade.GetTradePlayerAccount())) == NULL)								goto	END;

	// ��� �÷��̾� ���� ���� ���������� �˾ƿ´�.
	if(pTargetPlayer->IsAcceptedTrading(uiAccount) == FALSE && pTargetPlayer->IsChoseTradeItem(uiAccount) == FALSE)	goto	END;

	if(pFinishChooseTradeItemMsg->bFinishChooseItem == TRUE)
	{
		// ������ �������� �����ߴ�.
		if(ChooseTradeItem() == FALSE)																					goto	END;
	}
	else
	{
		// ���� �������� ������ ���� ����Ѵ�.
		if(CancelChooseTradeItem() == FALSE)																			goto	END;
	}
	
	bSuccess	=	TRUE;
END:
	if(bSuccess == TRUE)
	{
		// �� Ŭ���̾�Ʈ���� �����ش�.
		SendFieldMsg(ON_RESPONSE_FINISH_CHOOSE_TRADEITEM, SO_SFM_ONLYME, LPVOID(ON_RET_OK), LPVOID(TRUE), LPVOID(pFinishChooseTradeItemMsg->bFinishChooseItem));
		pTargetPlayer->SendFieldMsg(ON_RESPONSE_FINISH_CHOOSE_TRADEITEM, SO_SFM_ONLYME, LPVOID(ON_RET_OK), LPVOID(FALSE), LPVOID(pFinishChooseTradeItemMsg->bFinishChooseItem));
	}
	else
	{
		// ������ �������� �����ϴµ� �����ߴ�.
		SendFieldMsg(ON_RESPONSE_FINISH_CHOOSE_TRADEITEM, SO_SFM_ONLYME, LPVOID(ON_RET_NO));
	}
}


//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	Trade�� ������ �Ѵٴ� ���� �����Ѵ�.
//	���� ����	:	2002-07-07 ���� 1:05:27 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
VOID	SOPlayer::RecvDecideTrade()
{
	BOOL					bTradeSuccess	=	FALSE;
	DWORD					dwFailReason	=	ON_RET_NO;
	OnReqDecideTradeMsg		*pDecideTradeMsg;
	SOPlayer				*pTargetPlayer	=	NULL;		
	BOOL					bCancelTrade	=	FALSE;	
	BOOL					bInvalidMsg		=	FALSE;
	
	pDecideTradeMsg		=	(OnReqDecideTradeMsg*)cMsg;	

	// ���� �ŷ��� �����۵��� ��� �����ߴ��� �˾ƿ´�.
	if(IsChoseTradeItem() == FALSE)															{	bInvalidMsg	=	TRUE;	goto	END; }
	
	// ���� �����Ϸ��� �÷��̾��� ��ü�� ���´�.
	if((pTargetPlayer =	pMyWorld->GetPlayer(clTrade.GetTradePlayerAccount())) == NULL)		{	bCancelTrade =  TRUE;	goto	END; }

	// ���� �����Ϸ��� �÷��̾� ���� �������� ��� �����ߴ��� �˾ƿ´�.
	if(pTargetPlayer->IsChoseTradeItem(uiAccount) == FALSE)									{	bInvalidMsg	=	TRUE;	goto	END; }

	if(pDecideTradeMsg->bDecideTrade == TRUE)
	{
		if(pTargetPlayer->IsDecideTrading() == TRUE)
		{			
			// ������ �ŷ��� �Ѵٰ� �����ߴ�.

			// �켱 �����̶� ��ΰ� �ִ��� �˻��Ѵ�.
			if( ( clTrade.DoesExistTradeGoods( TRADE_GOODS_TRADE_BOOK ) == TRUE ) || 
				( pTargetPlayer->clTrade.DoesExistTradeGoods( TRADE_GOODS_TRADE_BOOK )  == TRUE ) )
			{
				// ���ʿ� ��θ� �������̶�� �����Ѵ�.
				clTrade.SetState( TRADE_STATE_SENDING_TRADE_BOOK );
				pTargetPlayer->clTrade.SetState( TRADE_STATE_SENDING_TRADE_BOOK );
				
				// ���ʿ� ��θ� �����϶�� �˷��ش�.
				SendFieldMsg( ON_RESPONSE_DECIDE_TRADE, SO_SFM_ONLYME, LPVOID( ON_RET_SEND_TRADE_BOOK ) );
				pTargetPlayer->SendFieldMsg( ON_RESPONSE_DECIDE_TRADE, SO_SFM_ONLYME, LPVOID( ON_RET_SEND_TRADE_BOOK ) );				
			}
			else
			{
				// �ŷ��� �ϵ��� �Ѵ�.
				if( Trade( pTargetPlayer ) == FALSE )										{	bCancelTrade = TRUE;	goto	END; }
			}
		}		
		else
		{	
			DecideTrade();
			// �ŷ� ��û ������ �۽�, �ڽ�/Ÿ�� setting
			SendFieldMsg( ON_RESPONSE_DECIDE_TRADE, SO_SFM_ONLYME, LPVOID( ON_RET_OK ), LPVOID(TRUE) );			
			pTargetPlayer->SendFieldMsg ( ON_RESPONSE_DECIDE_TRADE, SO_SFM_ONLYME, LPVOID( ON_RET_OK ), LPVOID(FALSE));
		}
	}
	else
	{
		// �� �κ��� ���� ���ӿ��� ������� �ʴ� �κ��̴�.

		// ������ �����Ϸ��� ���� ����ߴ�.	

		// ������ �����Ϸ��� ���� ����Ѵ�.
		if(CancelDecideTrading() == FALSE)													{	bCancelTrade = TRUE;	goto	END; }
		
		// �����Դ� ����� ����� �����Ǿ��ٰ� �˷��ش�.
		SendFieldMsg( ON_RESPONSE_DECIDE_TRADE, SO_SFM_ONLYME, LPVOID( ON_RET_OK ) );

		// ���� ������ ���ؼ� ����Ѵٰ� ���濡�� �˷��ش�.
		pTargetPlayer->SendFieldMsg( ON_RESPONSE_DECIDE_TRADE, SO_SFM_ONLYME, LPVOID( ON_RET_CANCEL_DECIDE_TRADE ) );
	}		


END:

	if( bInvalidMsg	== TRUE )
	{
		SendFieldMsg( ON_RESPONSE_DECIDE_TRADE, SO_SFM_ONLYME, LPVOID( ON_RET_NO ) );
	}

	if( bCancelTrade == TRUE )
	{
		// ������ ����ؾ� �Ѵ�.

		// ���ʿ��� ������ ����϶�� �˷��ش�.
		CancelTrade();	
		if( pTargetPlayer != NULL ) pTargetPlayer->CancelTrade();
	}
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	Trade�� �������� �������� �����Ѵ�.
//	���� ����	:	2002-07-07 ���� 5:15:07 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
VOID	SOPlayer::RecvChangeTradeGoods()
{
	OnReqChangeTradeGoods				*pChangeTradeGoods;
	BOOL								bSuccess = FALSE;
	SOPlayer							*pTargetPlayer;
	CHAR								cFollowerBuffer[10240];
	DWORD								dwFollowerDataSize	=	0;
	OnIndustrialComplexTradeInfo		IndustrialComplexTradeInfo;

	pChangeTradeGoods	=	(OnReqChangeTradeGoods*)cMsg;

	// ���� �����Ϸ��� �÷��̾��� ��ü�� ���´�.
	if((pTargetPlayer =	pMyWorld->GetPlayer(clTrade.GetTradePlayerAccount())) == NULL)														goto	END;

	// ���� �����Ϸ��� �÷��̾ ���� �ַ��� ǰ���� ���� �� �ִ��� �˾ƿ´�.	
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


//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	Trade�� ��� �ŷ��� �Ϸ�Ǿ���.
//	���� ����	:	2002-09-25 ���� 11:06:44 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
VOID	SOPlayer::RecvCompleteTradeBook()
{	
	OnReqCompleteTradeBoookMsg	*pCompleteTradeBoookMsg;
	SOPlayer					*pTargetPlayer;
	BOOL						bSuccess = FALSE;	

	pCompleteTradeBoookMsg		=	( OnReqCompleteTradeBoookMsg* )cMsg;

	// ���� ��θ� �ŷ������� �˻��Ѵ�.
	if( clTrade.IsSendingTradeBook() == FALSE )														goto	END;

	// ���� �����Ϸ��� �÷��̾��� ��ü�� ���´�.
	if( ( pTargetPlayer = pMyWorld->GetPlayer( clTrade.GetTradePlayerAccount() ) ) == NULL )		goto	END;

	// ���� �����Ϸ��� �÷��̾� ���� ��θ� �ŷ������� �˻��Ѵ�.
	if( ( pTargetPlayer->clTrade.IsSendingTradeBook() == FALSE ) && 
		( pTargetPlayer->clTrade.DoesCompleteSendingTradeBook() == FALSE ) )						goto	END;	

	switch( pCompleteTradeBoookMsg->dwResponse )
	{
	case ON_RET_OK:
		// ��θ� ���������� �ŷ� �Ϸ��ߴ�.
		
		// ���� ���� ��� �ŷ��� �Ϸ��ߴ��� �˻��Ѵ�.
		if( pTargetPlayer->clTrade.DoesCompleteSendingTradeBook() == TRUE )
		{
			// ���� ���� ��� �ŷ��� �Ϸ��ߴ�.

			// ������ �Ѵ�.
			if( Trade( pTargetPlayer ) == FALSE )
			{
				// �� �÷��̾��� ������ ��� ��Ų��.				
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
		// ��θ� �ŷ��ϴµ� �����ߴ�.		

		// �� �÷��̾��� ������ ��� ��Ų��.
		goto	CANCEL_TRADE;
	}

	bSuccess	=	TRUE;

END:
	if( bSuccess == FALSE )			SendFieldMsg( ON_RESPONSE_COMPLETE_TRADE_BOOK, SO_SFM_ONLYME, LPVOID( ON_RET_NO ) ); 		

	return;

CANCEL_TRADE:

	// �� �÷��̾��� ������ ��ҽ�Ų��.
	CancelTrade();
	pTargetPlayer->CancelTrade();

	return;
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	������ �����ߴ�.
//	���� ����	:	2002-07-06 ���� 2:14:32 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
BOOL	SOPlayer::AcceptedTrade( UI32 uiOppGuildID, SI32 siOppClassInGuild, SI32 siOppLevel )
{
	CHAR	*pszOppGuildID;
	CHAR	szOppGuildID[ ON_GUILDNAME_LENGTH + 1 ];	

	// ���� �����Ѵ�.
	if(clTrade.AcceptedTrade() == FALSE)	return	FALSE;	

	// �÷��̾�� ������ �����϶�� �����ش�.
	ZeroMemory( szOppGuildID, sizeof( szOppGuildID ) );
	if( uiOppGuildID != 0 )
	{	
		if( ( pszOppGuildID = pMyWorld->clIGuild.GetGuildName( uiOppGuildID ) ) != NULL )		
			strncpy( szOppGuildID, pszOppGuildID, ON_ID_LENGTH );		
	}

	SendFieldMsg( ON_RESPONSE_ACCEPT_TRADE, SO_SFM_ONLYME, LPVOID( ON_RET_OK ), LPVOID( siOppLevel ), LPVOID( szOppGuildID ), LPVOID( siOppClassInGuild ) );

	return	TRUE;
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	������ �������� �ʾҴ�.
//	���� ����	:	2002-07-06 ���� 3:15:10 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
BOOL	SOPlayer::NotAcceptedTrade()
{
	// ���� �����Ѵ�.
	if(clTrade.NotAcceptedTrade() == FALSE)	return	FALSE;
	
	// �÷��̾�� ������ ���Ѵٰ� �˷��ش�.
	SendFieldMsg(ON_RESPONSE_ACCEPT_TRADE, SO_SFM_ONLYME, LPVOID(ON_RET_NO));

	return	TRUE;
}


//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	���� �������� ������ ����Ѵ�.
//	���� ����	:	2002-07-06 ���� 2:37:49 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
BOOL	SOPlayer::CancelTrade()
{
	// ������ ����Ѵ�.
	if(clTrade.CancelTrade() == FALSE)	return	FALSE;
	
	// �÷��̾�� ������ ��ҵǾ��ٰ� �����ش�.
	SendFieldMsg(ON_RESPONSE_CANCEL_TRADE, SO_SFM_ONLYME, LPVOID(ON_RET_OK));

	return	TRUE;
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	���� �ٸ� �÷��̾ ������ �����ؼ� ������ �ϰ� �ִ��� �˾ƿ´�.
//	���� ����	:	2002-07-06 ���� 2:48:53 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
BOOL	SOPlayer::IsAcceptedTrading(UI16 uiAccount)
{
	return	 clTrade.IsAcceptedTrading(uiAccount);
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	������ ������ �³��ϴ� ���� ����ϴ��� �˾ƿ´�.
//	���� ����	:	2002-07-06 ���� 3:10:07 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
BOOL	SOPlayer::IsWaitingAcceptTrade(UI16 uiAccount)
{
	return	clTrade.IsWaitingAcceptTrade(uiAccount);
}


//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	������ �������� �����ߴ�.
//	���� ����	:	2002-07-06 ���� 6:41:43 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
BOOL	SOPlayer::ChooseTradeItem()
{
	return	clTrade.ChooseTradeItem();
}


//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	�����Ϸ��� �ߴ� ���� ����Ѵ�.
//	���� ����	:	2002-07-07 ���� 2:06:28 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
BOOL	SOPlayer::CancelDecideTrading()
{
	return	clTrade.CancelDecideTrading();
}


//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	������ �Ϸ��� ������ �ߴ��� �˾ƿ´�.
//	���� ����	:	2002-07-07 ���� 2:18:10 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
BOOL	SOPlayer::IsDecideTrading(UI16 uiAccount)
{
	return	clTrade.IsDecideTrading();
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	���� ��ǰ�� �߰��Ѵ�.
//	���� ����	:	2002-09-30 ���� 3:45:07 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
BOOL	SOPlayer::AddTradeGoods(UI08 uiSlotID, TradeGoodsKind Kind, LPARAM lParam1, LPARAM lParam2, OnFollowerInfo *pTradeFollowerInfo,
								DWORD *pdwFollowerDataSize, OnIndustrialComplexTradeInfo *pIndustrialComplexTradeInfo )
{
	UI16							uiFollowerID;
	UI16							uiItemID, uiQuantity;
	MyItemData						*pItemInfo;
	SOVillage_IndustrialComplex		*pIndustrialComplex;


	switch( Kind ) 
	{
	case TRADE_GOODS_ITEM :						// �������� �����Ѵ�.		
		

		// �뺴�� �˾Ƴ���.
		uiFollowerID	=	UI16(lParam1);	

		// �������� ID�� �˾Ƴ���.
		uiItemID		=	HIWORD(lParam2);

		// ���� �ŷ����� ���������� �˻��Ѵ�.
		// �ŷ����� �������� �ٸ� ������ �Ͽ� �ٽ� �ø����� ����.
		if( clTrade.IsTradingItem( uiFollowerID, uiItemID ) == TRUE )					return	FALSE;

		// �������� ������ �˾Ƴ���.
		uiQuantity		=	LOWORD(lParam2);

		// �ش� �뺴���� �� �������� ������ŭ �ִ��� �˾Ƴ���.
		if((pItemInfo  = IFollower.GetItemByID(uiFollowerID, uiItemID)) == NULL)		return	FALSE;

		// �������� ������ �Ǵ��� üũ�Ѵ�.
		if(pItemInfo->uiQuantity < uiQuantity)											return	FALSE;

		break;


	case TRADE_GOODS_FOLLOWER :					// �뺴�� �����Ѵ�.

		// �뺴�� �˾Ƴ���.
		uiFollowerID	=	UI16(lParam1);		

		// �����Ϸ��� �뺴�� ������ ���´�.
		//if(IFollower.GetFollowerTradeInfo(uiFollowerID, pTradeFollowerInfo) == FALSE)	return	FALSE;
		if( (*pdwFollowerDataSize = GetFollowerInfo( uiFollowerID, pTradeFollowerInfo )) == 0 )		return FALSE;

		break;


	case TRADE_GOODS_MONEY : 					// ���� �����Ѵ�.		

		// ���� ����� �ִ��� �˻��Ѵ�.
		if(UI32(lParam1) > siMoney )													return	FALSE;

		break;

	case TRADE_GOODS_REALESTATE :				// �ε����� �����Ѵ�.

		// ���� �ε����� ������ �ִ��� �˻��Ѵ�.
		if( ( pIndustrialComplex = pMyWorld->GetIndustrialComplex( this ) ) == NULL )	return	FALSE;
		
		pIndustrialComplex->GetTradeInfo( pIndustrialComplexTradeInfo );	
		
		break;

	default:
		return	FALSE;

	}

	return	clTrade.AddTradeGoods(uiSlotID, Kind, lParam1, lParam2);
	
}


//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	������ �ַ��� �ϴ� ��ǰ���� ��� ���� �� �ִ��� üũ�Ѵ�.
//	���� ����	:	2002-07-07 ���� 6:23:43 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
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

	// ���� �����Ϸ��� �÷��̾��� ��ü�� ���´�.
	if((pTargetPlayer =	pMyWorld->GetPlayer(clTrade.GetTradePlayerAccount())) == NULL)	return	FALSE;

	// ������ �ַ��� �ϴ� ��ǰ���� ���´�.
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
			uiFollowerNum++;										// �� �뺴�� ��
			if( pTargetPlayer->IsMonster(uiFollowerID) == TRUE )	// �� ������ ��
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

		// �������� �߰� ��ų �� �ִ��� �˾ƿ´�.
		// ������ ������ ���ΰ� ĳ���͸� �޴´�.
		if(CanAddItem(0, uiItemID, uiItemQuantity, uiItemNum) == FALSE)										return	FALSE;
		break;


	case TRADE_GOODS_FOLLOWER:
	{
		uiFollowerID					=	UI16(lParam1);		

		// ���� �ִ� ���� �� �ִ� �뺴���� �ʰ����� �ʴ��� �˾ƿ´�.
		if(uiFollowerNum >= GetMaxFollowerNum())															return	FALSE;

		// ���� �Ѱ��ַ��� ĳ���Ͱ� ���ΰ� ĳ���Ͷ�� �����Ѵ�.
		if(uiFollowerID == 0)																				return	FALSE;

		// �Ѱ��ַ��� ĳ���Ͱ� ��ȿ���� �˾ƿ´�.		
		if(pTargetPlayer->IFollower.IsValidFollower(uiFollowerID) == FALSE)									return	FALSE;
		
		// ������ ����� �ַ��� �ϴ��� �˾ƿ´�.
		if(pTargetPlayer->IsGeneral(uiFollowerID) == TRUE)					 								return	FALSE;

		// ������ ���͸� �ַ��� �ϴ� ��� �̹� ���� �ִ� ���ͼ��� �ŷ�â�� �ö�� ���ͼ��� ���� ON_MAXMONSTER���� �۾ƾ� �Ѵ�.
		if( pTargetPlayer->IsMonster(uiFollowerID) == TRUE )
		{
			if( GetMonsterNum() + uiMonsterNum >= ON_MAXMONSTER )											return	FALSE;
		}

		uiFollowerNum++;	

		// �뺴�� �߰� ��ų �� �ִ��� �˾ƿ´�.
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

		// ���� ���� �� �ִ� ������ ���� ���� �ַ��� �ϴ��� �˾ƿ´�.
		if( GetMaxMoneyToGet() < UI32( lParam1 ) )															return	FALSE;

		break;

	case TRADE_GOODS_REALESTATE:

		// �̹� ���� �ε����� ������ �ִ��� �˻��Ѵ�.
		if( pMyWorld->IsIndustrialComplexOwner( this ) == TRUE )											return	FALSE;
		
		//�ſ뵵�� 30 �̸� �̶�� ��� �� �� ����. - ���  2003. 2. 27- 
		if ( GetTradeGrade() < 30 )
		{
			return FALSE;
		}


		break;

	}

	return	TRUE;
}


//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	����� ������ �Ѵ�.
//	���� ����	:	2002-09-30 ���� 3:18:08 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
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

	// ���� �ʱ�ȭ �Ѵ�.
	*pdwTradeItemDataSize	=	0;
	*pdwFollowerDataSize	=	0;
	*puiFollowerNum			=	0;		
	*puiTradeItemNum		=	0;

	// ������ �ַ��� �ߴ� ǰ����� ��� �޾ƿ´�.	
	pstTradeGoods	=	pTargetPlayer->clTrade.GetTradeGoods();
	
	// ���� �޴´�.
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
		case TRADE_GOODS_ITEM :					// �������̴�.			

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

		case TRADE_GOODS_FOLLOWER :				// �뺴�̴�.

			uiFollowerID	=	UI16(pstTradeGoods[i].lParam1);

			// �뺴�� �޴´�.			
			if( ( dwFollowerDataSize = pTargetPlayer->TradeFollower( this, uiFollowerID, pFollowerInfo, &uiOppFollowerID ) ) != 0 )
			{
				// ���������� �뺴�� �޾Ҵ�.								
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
	// ���� �ַ��� �ߴ� �͵��� ��� ���ش�.
	
	// ���� �ַ��� �ߴ� ǰ����� ��� �޾ƿ´�.	
	pstTradeGoods	=	clTrade.GetTradeGoods();

	// ���� ���ش�.
	uiTradeMoney	=	clTrade.GetTradeMoney();
	DecreaseMoney(uiTradeMoney);
	
	for(i = 0; i < MAX_TRADE_WINDOW; i++)
	{	
		switch( pstTradeGoods[i].Kind )
		{
		case TRADE_GOODS_ITEM :				// �������̴�.
			
			uiFollowerID	=	UI16(pstTradeGoods[i].lParam1);
			uiItemID		=	HIWORD(pstTradeGoods[i].lParam2);
			uiItemQuantity	=	LOWORD(pstTradeGoods[i].lParam2);
			
			DelItem(uiFollowerID, uiItemID, uiItemQuantity);

			break;

		case TRADE_GOODS_FOLLOWER :			// �뺴�̴�.

			// �뺴�� ������ ���� Follower�� �������鼭 ��������.

			break;
		}
	}		
*/
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	pTradePlayer���� uiFollowerID�� �뺴�� �ش�.
//	���� ����	:	2002-07-07 ���� 11:45:11 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
DWORD	SOPlayer::TradeFollower(SOPlayer *pTradePlayer, UI16 uiFollowerID, OnFollowerInfo *pFollowerInfo, UI08 *puiOppFollowerID)
{
	DWORD				dwFollowerDataSize;
	SI32				siNation;
	//DBIKGiveFollower	DBGiveFollower;
	SI16				siEmptyFollowerSlot;

	// ���ΰ��̸� ���ش�.
	if(uiFollowerID == 0)		return	0;

	// �뺴�� ������ �����µ� �����ؾ��Ѵ�.
	if((dwFollowerDataSize = GetFollowerInfo(uiFollowerID, pFollowerInfo)) == 0)		return	0;

	// �뺴�� ������ ���´�.
	siNation	=	IFollower.GetFollowerParameterNation(uiFollowerID);

	// �켱 �� �뺴�� �����Ѵ�. 
	if(IFollower.DeleteFollower(uiFollowerID) == FALSE)									return	0;
	
	// �뺴�� �޴� �÷��̾ �뺴�� ��� �߰����Ѿ� �ϴ��� �˾ƿ´�.
	if((siEmptyFollowerSlot = pTradePlayer->IFollower.FindEmptyFollowerSlot()) == -1)	return	0;

	// ���濡�� �� �뺴�� �߰������ش�.
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


//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	����â�� �ݴ´�.
//	���� ����	:	2002-07-08 ���� 8:08:38 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
VOID	SOPlayer::CloseTrade()
{	
	SOPlayer		*pTargetPlayer;		

	// ���� �������� �ƴ϶�� �׳� ������.
	if(IsTrading() == FALSE)															return;

	// ���� �����Ϸ��� �÷��̾��� ��ü�� ���´�.
	if((pTargetPlayer =	pMyWorld->GetPlayer(clTrade.GetTradePlayerAccount())) == NULL)	return;

	// ������ ������ �����ϱ⸦ ��ٸ��� �ִ� ���¿�
	// �����߿� �������� �޽����� �޸����ش�.
	if(pTargetPlayer->IsWaitingAcceptTrade(uiAccount) == TRUE)		pTargetPlayer->SendFieldMsg(ON_RESPONSE_ACCEPT_TRADE, SO_SFM_ONLYME, LPVOID(ON_RET_NO));
	if(pTargetPlayer->IsAcceptedTrading(uiAccount) == TRUE)			pTargetPlayer->SendFieldMsg(ON_RESPONSE_CANCEL_TRADE, SO_SFM_ONLYME, LPVOID(ON_RET_NO));
	
	// ���� ������ ����â�� �ʱ�ȭ �Ѵ�.
	clTrade.Init();
	pTargetPlayer->clTrade.Init();
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	���� �����Ϸ��� ǰ���� ���� ��ȿ���� �˻��Ѵ�
//	���� ����	:	2002-09-23 ���� 12:19:38 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
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

			// �������� �޴´�.
			uiFollowerID	=	UI16(pstTradeGoods[i].lParam1);
			uiItemID		=	HIWORD(pstTradeGoods[i].lParam2);
			uiItemQuantity	=	LOWORD(pstTradeGoods[i].lParam2);
			
			if( ( pMyItemData =	IFollower.GetItemByID( uiFollowerID, uiItemID ) ) == NULL)	return	FALSE;			
			
			// ������ �˻��Ѵ�.
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

//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	�������� ǰ�� ���ؼ� �ŷ��� �϶�� �˷��ش�.
//	���� ����	:	2002-09-24 ���� 8:09:18 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
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

	// ���� ������ �����Ϸ��� ǰ����� ��� ��ȿ���� �˾ƿ´�.
	if(IsValidTradeGoods() == FALSE)																		goto	END;
	if(pTargetPlayer->IsValidTradeGoods() == FALSE)															goto	END;							

	// ���� ���� �������� ������ ���� �����ߴ�.
	TradeGoods(pTargetPlayer, &dwFollowerDataSize1, (OnFollowerInfo*)cFollowerBuffer1, &uiFollowerNum1, 
					&dwTradeItemDataSize1, uiTradeItem1, &uiTradeItemNum1, szTrade2DBArg );
	pTargetPlayer->TradeGoods(this, &dwFollowerDataSize2, (OnFollowerInfo*)cFollowerBuffer2, &uiFollowerNum2,
					&dwTradeItemDataSize2, uiTradeItem2, &uiTradeItemNum2, szTrade1DBArg );
	
	// �ٽ� ������ �� �� �ִ� ���·� �����Ѵ�.
	clTrade.Init();
	pTargetPlayer->clTrade.Init();
	
	// DB ��ɹ��� �����.
	sprintf( szAccountDBArg, "%d, %d, %d, %d",	stAccountInfo.siDBAccount, stAccountInfo.uiSlot, 
													pTargetPlayer->stAccountInfo.siDBAccount, pTargetPlayer->stAccountInfo.uiSlot );	

	// DB�� �����Ѵ�.
	ZeroMemory( szCompletedDBArg, sizeof( szCompletedDBArg ) );
	sprintf( szCompletedDBArg, "%s, %s, %s", szAccountDBArg, szTrade1DBArg, szTrade2DBArg );
	pMyWorld->clDatabase->writeDBQuery( SO_DB_INFOKIND_TRADE, LPVOID( szCompletedDBArg ), strlen( szCompletedDBArg ) + 1 );

	bSuccess	=	TRUE;

END:

	if( bSuccess == TRUE)
	{
		// ������ �����Ǿ���.
		SendFieldMsg( ON_COMPLETE_TRADE, SO_SFM_ONLYME, LPVOID( dwFollowerDataSize1 ), LPVOID( cFollowerBuffer1 ), LPVOID( uiFollowerNum1 ),
										LPVOID( dwTradeItemDataSize1 ), LPVOID( uiTradeItem1 ), LPVOID( uiTradeItemNum1 ) );
		pTargetPlayer->SendFieldMsg( ON_COMPLETE_TRADE , SO_SFM_ONLYME, LPVOID( dwFollowerDataSize2 ), 	LPVOID( cFollowerBuffer2 ), LPVOID( uiFollowerNum2 ),
										LPVOID( dwTradeItemDataSize2 ), LPVOID( uiTradeItem2 ), LPVOID( uiTradeItemNum2 ) );	
	}
	else
	{
		// ������ �����ߴ�. 
		// ������ ��� ��Ų��.

		CancelTrade();
		pTargetPlayer->CancelTrade();
	}
	
	return	bSuccess;
}
	
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	�������� �Ҹ��Ѵ�.
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
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
			// �������� �׳ɿ�����.
			if(pItemHeader->m_siHealth != 0)
			{
				if(pItemHeader->m_siGroupCode &  ON_ITEM_ALL_RECOVER )
					ChangeSatiety( static_cast<SI32>((float)ON_MAX_HEALTH * ((float)pItemHeader->m_siHealth / 100.0)) );
				else
					ChangeSatiety(pItemHeader->m_siHealth);
			}
			
			// ��ü����������� �ƴϸ� 
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

				// Ư�� ȿ���� �ִ� �������̴�.
				if (pItemHeader->m_siGroupCode & ON_ITEM_SPECIAL)
				{

					switch(pItemHeader->m_siSpecial)
					{
					case ON_ITEM_SPECIAL_INITIALIZE:
						{							
							OnlineFollower *pFollower = IFollower.GetFollower(uiFollowerID);
							
							SI32 siBonusDelta;
							
							if (pFollower == NULL) return FALSE;
							
							// ���⼭ uiFollowerID �� ���° �뺴�̳ĸ� ���ϰ�
							// uiCharID �� �뺴 ������ ���̵��̴�.
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
							// 0���뺴�� ���� ����
							if(uiFollowerID == 0)
							{
								return FALSE;
							}
							// �̹� �̿뺴�� ����ž� ������ ����
							if(siVMercenarySlot == uiFollowerID)
							{
								return FALSE;
							}

							// �뺴 ���� ����
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


							// 0���뺴�� �ƴϸ� ����
							if(uiFollowerID != 0)
							{
								return FALSE;
							}

							OnlineFollower *pFollower = IFollower.GetFollower(uiFollowerID);

							// �԰��ִ� ��� ���´�.
							UI16 puiWearItems[WEAR_ITEM_NUM];
							pFollower->GetWearItems(puiWearItems);

							// ���⸦ ����ϰ������� ����
							if (puiWearItems[ON_WEAR_WEAPON_ITEM] != 0)							
							{
								return FALSE;
							}

							switch (pFollower->uiCharID)
							{
								// ������
							case 67:
								pFollower->uiCharID = 323;
								uiKind = pFollower->uiCharID;
								break;
								// ������
							case 323:
								pFollower->uiCharID = 67;
								uiKind = pFollower->uiCharID;
								
								break;
								// �Ϻ���
							case 8771:
								pFollower->uiCharID = 9027;
								uiKind = pFollower->uiCharID;
								
								break;
								// �Ϻ���
							case 9027:
								pFollower->uiCharID = 8771;
								uiKind = pFollower->uiCharID;

								break;
								// �븸��
							case 10307:
								pFollower->uiCharID = 10563;
								uiKind = pFollower->uiCharID;
								break;
								// �븸��
							case 10563:
								pFollower->uiCharID = 10307;
								uiKind = pFollower->uiCharID;
								break;
								// �߱���
							case 12867:
								pFollower->uiCharID = 13123;
								uiKind = pFollower->uiCharID;
								break;
								// �߱���
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
							
							// �뺴 ���� ����
							

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
							//������ �Ҽ� �ִ� ���ΰ� üũ.
							if(pMyWorld->clIGuild.GetGuildWarCheck(uiGuildID) == FALSE)
							{
								return FALSE;
							}
							OnlineFollower *pFollower = IFollower.GetFollower(uiFollowerID);
							
							//��������->�������� ���� ���� ����� ������ ���� PrevKind�� �ٽ� ����.
							//�׷��� ���� ����� �Ϲ� ������ PrevKind�� ����.
							if(pMyWorld->m_FieldBattleParser.IsFieldAttackUnit(uiKind) == TRUE)
								uiPrevKind = GetPrevKind();								
							else
								uiPrevKind = uiKind;

							//Character �� Mercenary �� ĳ���� Kind�Ѵ� �����Ѵ�.
							//�������� ���� Kind�� �������ְ� DB�� �����Ҷ��� CharactorWar���̺� �����Ѵ�.
							uiKind				= pItemHeader->m_pImperium->m_uiKind;
							pFollower->uiCharID = pItemHeader->m_pImperium->m_uiKind;
							
							//uiQuantity : ������ ��� 2���� �������� ����Ѵ�.
							//             1���� ��� �Ϲ����� 2���� ��� ��������.
							//�����ϰ�쿣 �����̶�� ǥ�ø� ������ ü���� �ι�� �����.
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
							ImperumUse.uiItemID		   = uiItemID; //Item ID�� ���� �ʰ� Index�� ����.
							ImperumUse.uiQuantity	   = uiQuantity;
							ImperumUse.bCaptain		   = bCaptain;
							ImperumUse.siHp			   = siHp;
							ImperumUse.siSupply		   = 0;        // �Ϲ� ������ ���� �������� 0���� �Ѵ�.
*/
							if(uiQuantity == 1)
								ImperumUse.bCaptain		   = FALSE;
							else 
								ImperumUse.bCaptain		   = TRUE;
							
							SetFieldHp(siHp);			//ü��   ����.

							//pMyWorld->m_FieldBattleParser.
							
							SetPrevKind(uiPrevKind);								//Prev Kind ����.
							SetCaptainUnit(bCaptain);								//������������ ����.

							//�Ϲ� �������� ������ �����Ѵ�.
							//�Ϲ� ���������� ���޷��� �������� �������� �ʴ´�.
							if(pMyWorld->m_FieldBattleParser.GetUnitKind(pItemHeader->m_pImperium->m_uiKind) != FIELD_BATTLE_SUPPLY)
							{
	//							SetFieldSupply((SI32)pItemHeader->m_pImperium->m_siSupply);   //���޷� ����.
	//							InitFieldSupplyCharge(0);
							}
							//���޼����� ������������ �����Ѵ�.
							else if(pMyWorld->m_FieldBattleParser.GetUnitKind(pItemHeader->m_pImperium->m_uiKind) == FIELD_BATTLE_SUPPLY)
							{
	//							InitFieldSupplyCharge((SI32)pItemHeader->m_pImperium->m_siSupply); // ���������� ����.
//								ImperumUse.siSupply = (SI16)pItemHeader->m_pImperium->m_siSupply; // ��� �������� �������� ����.
							}

							pMyWorld->clDatabase->writeDBQuery(SO_DB_INFOKIND_IMPERIUM_USE, LPVOID(&ImperumUse), sizeof(DBIKImperumUse));
							bSendDeleteQuery = FALSE;

						}
						break;
					case ON_ITEM_SPECIAL_CHANGENATION:
						{
							// 0���뺴�� �ƴϸ� ����
							if(uiFollowerID != 0)
							{
								return FALSE;
							}

							OnlineFollower *pFollower = IFollower.GetFollower(uiFollowerID);

							

							// �԰��ִ� ��� ���´�.
							UI16 puiWearItems[WEAR_ITEM_NUM];
							pFollower->GetWearItems(puiWearItems);

							// ���⸦ ����ϰ������� ����
							if (puiWearItems[ON_WEAR_WEAPON_ITEM] != 0)							
							{
								return FALSE;
							}

							// ������ ���Ϸ��� ������ ������ ����
							if (pFollower->Nation == pItemHeader->m_siSpecialParam) return FALSE;


							// ĳ�� kind ����
							switch(pItemHeader->m_siSpecialParam)
							{
							// ������ȭ
							case 1:
								switch (pFollower->uiCharID)
								{
									// ����
								case 67:
								case 8771:
								case 10307:
								case 12867:
									// ���������� ����
									pFollower->uiCharID = 67;
									uiKind = pFollower->uiCharID;
								
									
									break;
									// ����.
								case 323:
								case 9027:
								case 10563:
								case 13123:
									pFollower->uiCharID = 323;
									uiKind = pFollower->uiCharID;
								
									break;
								}
								
								break;
							// �Ϻ���ȭ
							case 10:
								switch (pFollower->uiCharID)
								{
									// ����
								case 67:
								case 8771:
								case 10307:
								case 12867:

									pFollower->uiCharID = 8771;
									uiKind = pFollower->uiCharID;
								
									break;
									// ����.
								case 323:
								case 9027:
								case 10563:
								case 13123:

									pFollower->uiCharID = 9027;
									uiKind = pFollower->uiCharID;
								
									break;
								}
								
								break;
							// �븸��ȭ
							case 1000:
								switch (pFollower->uiCharID)
								{
									// ����
								case 67:
								case 8771:
								case 10307:
								case 12867:

									pFollower->uiCharID = 10307;
									uiKind = pFollower->uiCharID;
								
									break;
									// ����.
								case 323:
								case 9027:
								case 10563:
								case 13123:

									pFollower->uiCharID = 10563;
									uiKind = pFollower->uiCharID;
								
									break;
								}
								
								break;
							// �߱���ȭ
							case 100:
								switch (pFollower->uiCharID)
								{
									// ����
								case 67:
								case 8771:
								case 10307:
								case 12867:
									pFollower->uiCharID = 12867;
									uiKind = pFollower->uiCharID;								
									break;
									// ����.
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

							// ��������
							pFollower->Nation = pItemHeader->m_siSpecialParam;

							// ����Ʈ ����

							Quest_AllClear();

							
							// ���� �޽��� ����

							if (IsInVillage()) {
								SendFieldMsg(ON_CHANGE_CHARACTERCODE, SO_SFM_ONLYME, LPVOID(this),LPVOID(pFollower->uiCharID));
							}
							else {
								SendFieldMsg(ON_CHANGE_CHARACTERCODE, SO_SFM_MEANDNEARCHAR, LPVOID(this),LPVOID(pFollower->uiCharID));
							}

							
							
							// ���� db��������
							

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
					case ON_ITEM_SPECIAL_TACTICS:		// ���� ���� ������� �ִ� ��� ��� ���� �ִ� 5�� ���� ���� ������.
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
					case ON_ITEM_SPECIAL_DONKEY:		// �糪�͸� ����ϸ� �뺴 ���Կ� �糪�Ͱ� �Ѹ��� ����.
						{
							if( !OnUseDonkeyItem(uiFollowerID, uiItemID) )		return FALSE;
							bSendDeleteQuery	=	FALSE;
						}
						break;

					case ON_ITEM_SPECIAL_MAGICSTONERED:
					case ON_ITEM_SPECIAL_MAGICSTONEBLUE:
					case ON_ITEM_SPECIAL_MAGICSTONEBLACK:
						{
							if( IFollower.CanWeaponBeUpgraded(uiFollowerID) )					// ���׷��̵� ������ �������� �˻��Ѵ�.
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
								// �ش� �뺴�� �������� �����۵��� ���´�.
								IFollower.GetWearItems( uiFollowerID, puiWearItems );
								// �ش� �������� �������� ���� �������� ������ ����� ��´�.
								stItemUpgradeResultInfo		=	pMyWorld->m_ItemUpgradeParser.GetUpgradeResult( puiWearItems[ON_WEAR_WEAPON_ITEM], siMagicStoneKind );

								// ���� ��� ���� �Ұ����̸� �׳� ���Ͻ�Ų��. (������ �Ҹ���� �ʴ´�.)
								if( stItemUpgradeResultInfo->siResult == ITEM_UPGRADE_IMPOSSIBLE )			return FALSE;
								
								if( stItemUpgradeResultInfo->siResult == ITEM_UPGRADE_SUCCESS )
								{
									// �������� ���׷��̵� ���ְ� DB�� ���� ������ �����ش�.
									IFollower.UpgradeWeapon( uiFollowerID, stItemUpgradeResultInfo->uiItemID );		// ������ ���׷��̵�

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
									// DB�� ���� ������ �����ش�.
									IFollower.UpgradeWeapon( uiFollowerID, stItemUpgradeResultInfo->uiItemID );		// ������ ���׷��̵�
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
									// �������� �ı��Ǿ����Ƿ� �����ϰ� DB�� �ı��Ǿ��ٴ� ������ �����ش�.
									IFollower.BrakeWeapon( uiFollowerID );											// ������ �ı�

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
				}	// end of if(Ư��ȿ���� �ִ� �������̴�)
			}	// end of if(��ü ���� �������� �ƴϴ�)
			// ��ü����������̸�
			else {
				
				SI32 siHpGain = pItemHeader->m_siHpGain;
				SI32 siMpGain = pItemHeader->m_siMpGain;
				
				SI32 siMaxFollower = ON_MAX_FOLLOWER_NUM;
				
				for (int i = 0; i < siMaxFollower; i++)
				{
					// �����ϴ� �����̸� 
					if (IFollower.IsValidFollower(i))
					{					
						// parameterbox�� ���ؼ�
						pParaBox	= &IFollower.GetFollower(i)->ParametaBox;
						if(pParaBox)
						{
							// ��Ȱ�������̾ƴϸ� �����뺴�� ġ�������ʴ´�.
							if ( !(pItemHeader->m_siGroupCode & ON_ITEM_REVIVE) )
							{
								if (pParaBox->IP.GetLife() <= 0) continue;								
							}
							

							// ��ü ȸ�� | 100%�� ȸ��
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
								// ü�� ���� ȸ��!
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





