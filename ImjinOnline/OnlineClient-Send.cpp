#include <GSL.h>
#include "OnlineWorld.h"
#include "OnlineGameMsg.h"	
#include "OnlineVersion.h"
#include <DebugTool.h>
#include "OnlineMyData.h"
#include "_SkillDefine.h"
#include "_BuriedItemDefine.h"
#include "MiniGame1.h"
#include "OnlineClient.h"
#include "OnlineSMS.h"		// actdoll (2004/06/15 10:46) : SMS 용 데이터 추가
#include "OnlinePannel.h"


//----------------------------------------------------------------------------------------------------------------
//	 설명	:	내 플레이어의 위치를 보낸다.
//----------------------------------------------------------------------------------------------------------------
BOOL	OnlineClient::SendPlayerPos()
{
	OnReqMyCharMoveMsg	CharMoveMsg;	
	SI16				siXPos, siYPos;	
	
	pMyOnlineWorld->pIOnlineChar->GetPos(pMyOnlineWorld->pMyData->GetMyCharUniqueID(), &siXPos, &siYPos);
	CharMoveMsg.dwMsgType	=	ON_REQUEST_POSITION;
	CharMoveMsg.uiX			=	UI16(siXPos);
	CharMoveMsg.uiY			=	UI16(siYPos);
	CharMoveMsg.bWarUnit	=	pMyOnlineWorld->pIOnlineChar->IsSiegeWarfareUnit(pMyOnlineWorld->pMyData->GetMyCharUniqueID());

	return	Write(&CharMoveMsg, sizeof(OnReqMyCharMoveMsg));	
}

//----------------------------------------------------------------------------------------------------------------
//	 설명	:	내 플레이어의 위치를 보낸다.
//----------------------------------------------------------------------------------------------------------------
BOOL	OnlineClient::SendPlayerPos(SI16 siXPos, SI16 siYPos, BOOL bIsSiegeWarfareUnit)
{
	OnReqMyCharMoveMsg	CharMoveMsg;	
	
	CharMoveMsg.dwMsgType	=	ON_REQUEST_POSITION;
	CharMoveMsg.uiX			=	UI16(siXPos);
	CharMoveMsg.uiY			=	UI16(siYPos);
	CharMoveMsg.bWarUnit	=	bIsSiegeWarfareUnit;

	return	Write(&CharMoveMsg, sizeof(OnReqMyCharMoveMsg));	
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
//	설명	:	내가 이동해야 하는 방향을 보내준다.
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
BOOL	OnlineClient::SendMovingDirection()
{
	/*
	CHAR	cDirection;
	CHAR	cPack[2];

	switch(pMyOnlineWorld->pIOnlineChar->GetMovingDirection(pMyOnlineWorld->pMyData->GetMyCharUniqueID()))
	{
	case ON_SOUTH:		
		cDirection	=	ON_MOVE_S;
		break;
	case ON_WEST | ON_SOUTH:
		cDirection	=	ON_MOVE_SW;		
		break;
	case ON_WEST:
		cDirection	=	ON_MOVE_W;
		break;
	case ON_WEST | ON_NORTH:
		cDirection	=	ON_MOVE_NW;
		break;
	case ON_NORTH:
		cDirection	=	ON_MOVE_N;			
		break;
	case ON_EAST | ON_NORTH:
		cDirection	=	ON_MOVE_NE;
		break;
	case ON_EAST:
		cDirection	=	ON_MOVE_E;
		break;
	case ON_EAST | ON_SOUTH:
		cDirection	=	ON_MOVE_SE;
		break;		
	}

	cPack[0]	=	ON_REQUEST_MOVE;
	cPack[1]	=	cDirection;

	return Write(cPack, 2);
	*/
	return FALSE;
}
/*
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
//	설명	:	내가 이동해야 하는 좌표를 보내준다.
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
BOOL	OnlineClient::SendMovingPos()
{
	SI16	siX;
	SI16	siY;
	SI32	siDirection;
	CHAR	cPack[5];

	pMyOnlineWorld->pIOnlineChar->GetPos(pMyOnlineWorld->pMyData->GetMyCharUniqueID(), &siX, &siY);
	siDirection	=	pMyOnlineWorld->pIOnlineChar->GetMovingDirection(pMyOnlineWorld->pMyData->GetMyCharUniqueID());
	
	if(siDirection & ON_EAST)
		siX++;
	else if(siDirection & ON_WEST)
		siX--;

	if(siDirection & ON_NORTH)
		siY--;
	else if(siDirection & ON_SOUTH)
		siY++;	

	cPack[0]	=	(BYTE)ON_REQUEST_POSITION;
	cPack[1]	=	(BYTE)LOBYTE(siX);
	cPack[2]	=	(BYTE)HIBYTE(siX);
	cPack[3]	=	(BYTE)LOBYTE(siY);
	cPack[4]	=	(BYTE)HIBYTE(siY);
	
	return	Write(cPack, 5);
}
*/
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
//	설명	:	
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
BOOL	OnlineClient::SendRequestPlayerPos(UI16 uiPlayer)
{
	/*
	CHAR	cPack[3];

	cPack[0]	=	ON_REQUEST_PLAYER_POSITION;
	cPack[1]	=	LOBYTE(uiPlayer);
	cPack[2]	=	HIBYTE(uiPlayer);

	return Write(cPack, 3);
	*/
	return FALSE;
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
//	설명	:	내가 타이핑한 메시지를 보낸다.
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
BOOL	OnlineClient::SendChatting(CHAR *pChatMessage)
{
	/*
	CHAR				cLocalBuffer[ON_MAX_LOCALBUFFER];
	OnReqChattingMsg	*pChattingMsg;
	CHAR				*pChatString;
	SI32				siSendSize;
	SI32				siStrlen;

	siStrlen		=	strlen(pChatMessage);

	pChattingMsg	=	(OnReqChattingMsg*)cLocalBuffer;
	pChatString		=	(CHAR*)((CHAR*)pChattingMsg + sizeof(OnReqChattingMsg));
	siSendSize		=	sizeof(OnReqChattingMsg) + siStrlen;
	
	pChattingMsg->dwMsgType		=	ON_REQUEST_MSG;
	pChattingMsg->dwChatLength	=	siStrlen;	

	strncpy(pChatString, pChatMessage, siStrlen);
	
	return Write(pChattingMsg, siSendSize);	
	*/
	return FALSE;
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
//	설명	:	내가 마을에 들어간다는 메시지를 서버에게 알려준다.
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
BOOL	OnlineClient::SendGotoVillage(UI16 uiGotoVillageCode)
{
	OnReqGotoVillageMsg		GotoVillageMsg;

	GotoVillageMsg.dwMsgType		=	ON_REQUEST_ENTER_VILLAGE;
	GotoVillageMsg.uiVillageCode	=	uiGotoVillageCode;	
	
	return Write(&GotoVillageMsg, sizeof(OnReqGotoVillageMsg));	
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
//	설명	:	마을을 나간다.
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
BOOL	OnlineClient::SendGoOutVillage()
{
	OnReqGoOutVillageMsg		ReqGoOutVillageMsg;

	ReqGoOutVillageMsg.dwMsgType	=	ON_REQUEST_LEAVE_VILLAGE;	

	return Write(&ReqGoOutVillageMsg, sizeof(OnReqGoOutVillageMsg));
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
//	설명	:	아이템의 리스트를 달라고 요구한다.
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
BOOL	OnlineClient::SendRequestItemList()
{
	OnReqItemListInMarketMsg	ReqItemListInMarketMsg;

	ReqItemListInMarketMsg.dwMsgType	=	ON_REQUEST_LIST_ITEM;	

	return Write(&ReqItemListInMarketMsg, sizeof(OnReqItemListInMarketMsg));
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
//	설명	:	아이템을 산다고 서버에 요청한다.
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
BOOL	OnlineClient::SendBuyItem(OnReqBuyItemMsg* lpBuyItem)
{
	return Write(lpBuyItem, sizeof(OnReqBuyItemMsg));	
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
//	설명	:	아이템을 판다고 서버에 요청한다.
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
BOOL	OnlineClient::SendSellItem(UI08 uiFollower, UI16 uiID, UI16 uiItemQuantity)
{
	OnReqSellItemMsg	ReqSellItemMsg;

	ReqSellItemMsg.dwMsgType		= ON_REQUEST_SELL_ITEM;
	ReqSellItemMsg.uiFollowerID     = uiFollower;
	ReqSellItemMsg.uiItemID			= uiID;
	ReqSellItemMsg.uiItemQuantity	= uiItemQuantity;

	return Write(&ReqSellItemMsg, sizeof(OnReqSellItemMsg));
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
//	설명	:	다른 플레이어와 전투를 한다고 서버에게 알려준다.
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
BOOL	OnlineClient::SendBattle(SI32 siID)
{
	OnReqBattleMsg		BattleMsg;

	BattleMsg.dwMsgType	=	ON_REQUEST_BATTLE;
	BattleMsg.uiCharID	=	UI16(siID);

	return Write(&BattleMsg, sizeof(BattleMsg));	
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명	:	인벤토리내에서의 내 아이템의 위치를 서버에게 알려준다.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
BOOL	OnlineClient::SendItemPosInInventory(UI08 uiFollower, UI08 uiReceiverID, UI16 uiID, UI16 siNewPos, SI16 siQuantity )
{
	OnReqShareItemMsg	msg;

	msg.dwMsgType           = ON_REQUEST_MOVEITEM;
	msg.uiFollowerID        = uiFollower;
	msg.uiReceiverID		= uiReceiverID;
	msg.uiItemID            = uiID;
	msg.uiNewPosInInventory = siNewPos;
	msg.uiItemQuantity		= siQuantity;

	return Write((void*)&msg, sizeof(msg));
}


//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명	:	아이템을 버린다.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
BOOL	OnlineClient::SendThrowItem(OnReqThrowItemMsg* lpOnReqThrowItemMsg)
{
	return Write(lpOnReqThrowItemMsg, sizeof(OnReqThrowItemMsg));	
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명	:	땅에 떨어져 있는 아이템을 주었다.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
BOOL	OnlineClient::SendPickUpItem(OnReqPickUpItemMsg* lpOnlineTempOnReqPickUpItemMsg)
{
	return Write(lpOnlineTempOnReqPickUpItemMsg, sizeof(OnReqPickUpItemMsg));	
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명	:	다시 온라인으로 돌아가려고 한다.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
BOOL	OnlineClient::SendReturnOnline(BOOL bWin)
{
	OnReqReturnOnlineMsg	ReturnOnlineMsg;	

//	DWriteStringToFile("SendReturnOnline() 메세지 보냄");
	ReturnOnlineMsg.dwMsgType	=	ON_REQUEST_RETURNONLINE;
	ReturnOnlineMsg.dwBattleResult = bWin;

	return Write(&ReturnOnlineMsg, sizeof(OnReqReturnOnlineMsg));
}



//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명	:	캐릭터의 정보를 달라고 설정한다.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
BOOL	OnlineClient::SendRequestPlayerInfo(UI16 uiCharNum, UI16 *puiIDIn)
{
	CHAR						cLocalBuffer[ON_MAX_LOCALBUFFER];
	OnReqPlayerInfoMsg			*pReqPlayerInfoMsg;
	UI16						*puiIDOut;
	SI32						siLength;

//	SetMessage("캐릭터 정보 요청");

	pReqPlayerInfoMsg				=	(OnReqPlayerInfoMsg*)cLocalBuffer;

	pReqPlayerInfoMsg->dwMsgType		=	ON_REQUEST_PLAYER_INFO;
	pReqPlayerInfoMsg->uiCharNum		=	uiCharNum;
	puiIDOut							=	(UI16*)((CHAR*)pReqPlayerInfoMsg + sizeof(OnReqPlayerInfoMsg));
	siLength							=	sizeof(OnReqPlayerInfoMsg) + (sizeof(UI16) * pReqPlayerInfoMsg->uiCharNum);
	
	memcpy(puiIDOut, puiIDIn, sizeof(UI16) * pReqPlayerInfoMsg->uiCharNum);

	return Write(pReqPlayerInfoMsg, siLength);
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명	:	마을, 시장에 들어간다.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
BOOL	OnlineClient::SendRequestEnterMarket()
{
	OnReqEnterMarketMsg	EnterMarketMsg;

	EnterMarketMsg.dwMsgType	=	ON_REQUEST_ENTER_MARKET;
	
	return Write(&EnterMarketMsg, sizeof(EnterMarketMsg));
}
	
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명	:	마을, 시장에서 나간다.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
BOOL	OnlineClient::SendRequestLeaveMarket()
{	
	OnReqLeaveMarketMsg	LeaveMarketMsg;

	LeaveMarketMsg.dwMsgType	=	ON_REQUEST_LEAVE_MARKET;

	return Write(&LeaveMarketMsg, sizeof(LeaveMarketMsg));
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명	:	전장 정보를 요청
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
BOOL	OnlineClient::SendRequestBankInfo()
{
	OnReqBankInfoMsg	TempOnReqBankInfoMsg;

	TempOnReqBankInfoMsg.dwMsgType	=	ON_REQUEST_BANK_INFO;	

	return Write( &TempOnReqBankInfoMsg, sizeof( OnReqBankInfoMsg ) );

}
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명	:	계좌 개설 요청
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
BOOL	OnlineClient::SendRequestOpenSavingAccount(UI16 Kind, MONEY SavingMoney, BOOL fSavingTime, UI32 SavingTime)
{	
	OnReqBankOpenSavingAccountMsg	TempOnReqBankOpenSavingAccountMsg;


	TempOnReqBankOpenSavingAccountMsg.dwMsgType	          = ON_REQUEST_BANK_OPENSAVINGACCOUNT;
	TempOnReqBankOpenSavingAccountMsg.uiKind  		   	  = Kind;
	TempOnReqBankOpenSavingAccountMsg.siFirstDepositMoney = SavingMoney;


	if( fSavingTime == FALSE )		TempOnReqBankOpenSavingAccountMsg.uiExpireMonth	= 0;
	else							TempOnReqBankOpenSavingAccountMsg.uiExpireMonth = SavingTime;
	

	return Write(&TempOnReqBankOpenSavingAccountMsg, sizeof(OnReqBankOpenSavingAccountMsg));
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명	:	통장 리스트 요청
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
BOOL	OnlineClient::SendRequestMySavingAccount(void)
{	
	OnReqBankBankBookListMsg			TempOnReqBankBankBookList;

	TempOnReqBankBankBookList.dwMsgType = ON_REQUEST_BANK_BANKBOOK_LIST;

	return Write(&TempOnReqBankBankBookList, sizeof(OnReqBankBankBookListMsg));
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명	:	입금
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
BOOL	OnlineClient::SendRequestSavingAccount(OnAccountNumber AccountNumber, MONEY Money)
{
	OnReqBankDepositMsg		TempOnReqBankDepositMsg;

	TempOnReqBankDepositMsg.dwMsgType     = ON_REQUEST_BANK_DEPOSIT;
	TempOnReqBankDepositMsg.AccountNumber = AccountNumber;
	TempOnReqBankDepositMsg.siMoney       = Money;

	return Write(&TempOnReqBankDepositMsg, sizeof(OnReqBankDepositMsg));
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명	:	출금
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
BOOL	OnlineClient::SendRequestDrawingAccount(OnAccountNumber AccountNumber, MONEY Money)
{
	OnReqBankDepositMsg		TempOnReqBankDepositMsg;

	TempOnReqBankDepositMsg.dwMsgType     = ON_REQUEST_BANK_DRAWING;
	TempOnReqBankDepositMsg.AccountNumber = AccountNumber;
	TempOnReqBankDepositMsg.siMoney       = Money;

	return Write(&TempOnReqBankDepositMsg, sizeof(OnReqBankDepositMsg));
}
	
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명	:	마을, 전장에서 담보대출
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
BOOL	OnlineClient::SendCollateralLoanMsg( UI16 uiMonth, UI32 uiMoney, DWORD dwItemNum, UI16 *pID, UI16 *pCount, SI16 *pPos )
{
	OnReqCollateralLoanMsg		TempOnReqCollateralLoanMsg;
	UI08						Temp[60000];

	SI32	siSize;
	UI08	uiTemp[10];

	ZeroMemory( uiTemp, sizeof( UI08 ) * 10 );
	

	TempOnReqCollateralLoanMsg.dwMsgType	=	ON_REQUEST_BANK_COLLATERAL;
	TempOnReqCollateralLoanMsg.uiMonth		=	uiMonth;
	TempOnReqCollateralLoanMsg.siMoney		=	uiMoney;
	TempOnReqCollateralLoanMsg.dwItemNum	=	dwItemNum;
	siSize									=	sizeof(OnReqCollateralLoanMsg);


	memcpy( Temp, &TempOnReqCollateralLoanMsg, siSize );	
	memcpy( &Temp[siSize], uiTemp, sizeof( UI08 ) * dwItemNum );


	siSize		=	sizeof(OnReqCollateralLoanMsg) + sizeof( UI08 ) * dwItemNum;

	memcpy( &Temp[siSize], pID, sizeof( UI16 ) * dwItemNum );
	siSize		+=	sizeof( UI16 ) * dwItemNum;


	memcpy( &Temp[siSize], pCount, sizeof( UI16 ) * dwItemNum );
	siSize		+=	sizeof( UI16 ) * dwItemNum;

	memcpy( &Temp[siSize], pPos, sizeof( SI16 ) * dwItemNum );
	siSize		+= sizeof( SI16 ) * dwItemNum;	

	return	Write( Temp, siSize );
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명	:	마을, 전장에서 담보물 리스트를 받는다.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
BOOL	OnlineClient::SendCollateralListMsg()
{
	OnReqCollateralListMsg		TempOnReqCollateralListMsg;

	TempOnReqCollateralListMsg.dwMsgType	=	ON_REQUEST_BANK_COLLATERALLIST;

	return Write( &TempOnReqCollateralListMsg, sizeof( OnReqCollateralListMsg ) );
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명	:	투자금 정보
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
BOOL OnlineClient::SendReqAuditVillageInfo()
{
	OnReqAuditVillageList		TempReqAuditVillageList;

	TempReqAuditVillageList.dwMsgType = ON_REQUEST_AUDIT_VILLAGE_LIST;

	return Write(  &TempReqAuditVillageList,sizeof(OnReqAuditVillageList));
}
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명	:	마을, 전장에서 담보를 찾는다.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
BOOL	OnlineClient::SendTakeCollateral( SI16 siNum, UI08 *puiFollowerID, UI16 *pItemID, UI16 *pCount, SI16 *pPos )
{
	OnReqTakeCollateralMsg		TempOnReqTakeCollateralMsg;
	UI08		Temp[60000];
	SI16		siSize;

	TempOnReqTakeCollateralMsg.dwMsgType = ON_REQUEST_BANK_TAKECOLLATERAL;
	TempOnReqTakeCollateralMsg.dwItemNum = siNum;

	siSize = sizeof( OnReqTakeCollateralMsg );
	memcpy( Temp, &TempOnReqTakeCollateralMsg, siSize );

	
	memcpy( &Temp[siSize], puiFollowerID, sizeof( UI08 ) * siNum );
	siSize += sizeof( UI08 ) * siNum;

	memcpy( &Temp[siSize], pItemID, sizeof( UI16 ) * siNum );
	siSize += sizeof( UI16 ) * siNum;

	memcpy( &Temp[siSize], pCount, sizeof( UI16 ) * siNum );
	siSize += sizeof( UI16 ) * siNum;

	memcpy( &Temp[siSize], pPos, sizeof( SI16 ) * siNum );
	siSize += sizeof( SI16 ) * siNum;


	return Write( Temp, siSize );
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명	:	Message 전송
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
BOOL	OnlineClient::SendMessage(char* lpMsg, SI16 Size)
{
	return Write(lpMsg, Size);
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명	:	마을, 주점에 들어간다.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
BOOL	OnlineClient::SendRequestEnterInn()
{
	OnReqEnterInnMsg	TempOnReqEnterInnMsg;

	TempOnReqEnterInnMsg.dwMsgType		= 	ON_REQUEST_ENTER_INN;

	return Write(&TempOnReqEnterInnMsg, sizeof(OnReqEnterInnMsg));
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명	:	마을, 주점에서 나간다.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
BOOL	OnlineClient::SendRequestLeaveInn()
{
	ReqLeaveInnMsg		TempReqLeaveInnMsg;

	TempReqLeaveInnMsg.dwMsgType		=	ON_REQUEST_LEAVE_INN;

	return Write(&TempReqLeaveInnMsg, sizeof(ReqLeaveInnMsg));
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명	:	마을, 주점에 대화실로 들어간다.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
BOOL	OnlineClient::SendRequestEnterInnVWR()
{
	OnReqEnterWaitRoomMsg	TempOnReqEnterWaitRoomMsg;

	TempOnReqEnterWaitRoomMsg.dwMsgType		= 	ON_REQUEST_ENTER_VWR;

	return Write(&TempOnReqEnterWaitRoomMsg, sizeof(OnReqEnterWaitRoomMsg));
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명	:	마을, 주점에 대화실에서 대화방에 참여한다.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
BOOL	OnlineClient::SendRequestJoinVilInnChatRoom(UI16 ChattingRoomID)
{
	OnReqJoinVilInnChatRoomMsg	ReqJoinVilInnChatRoomMsg;

	ReqJoinVilInnChatRoomMsg.dwMsgType	  = ON_REQUEST_VIL_INN_JOIN_CHAT_ROOM;
	ReqJoinVilInnChatRoomMsg.uiChatRoomID = ChattingRoomID;

	return Write(&ReqJoinVilInnChatRoomMsg, sizeof(OnReqJoinVilInnChatRoomMsg));
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명	:	마을, 주점에 대화실에서 대화방에서 나간다.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
BOOL	OnlineClient::SendRequestLeaveVilInnChatRoom()
{
	OnReqLeaveVilInnChatRoomMsg	ReqLeaveVilInnChatRoomMsg;

	ReqLeaveVilInnChatRoomMsg.dwMsgType	=	ON_REQUEST_VIL_INN_LEAVE_CHAT_ROOM;

	return Write(&ReqLeaveVilInnChatRoomMsg, sizeof(ReqLeaveVilInnChatRoomMsg));
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명	:	마을, 주점에 대화실에서 채팅방 생성
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
BOOL	OnlineClient::SendRequestINNCreateChattingRoom(char* lpRoomName)
{
	BYTE						TempBuffer[1024];
	OnReqCreateChatRoomMsg*		TemplpOnReqCreateChatRoomMsg;
	SI16						siSize;
	
	siSize                                       = sizeof(OnReqCreateChatRoomMsg) + strlen(lpRoomName);
	TemplpOnReqCreateChatRoomMsg                 = (OnReqCreateChatRoomMsg*)TempBuffer;
	TemplpOnReqCreateChatRoomMsg->dwMsgType      = ON_REQUEST_INN_CREATECHATROOM;
	TemplpOnReqCreateChatRoomMsg->uiStringLength = strlen(lpRoomName);
	strcpy((char*)&TempBuffer[sizeof(OnReqCreateChatRoomMsg)], (char*)lpRoomName);

	return Write(TempBuffer, siSize);
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명	:	마을, 주점에 게시판에 들어감
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
BOOL	OnlineClient::SendRequestEnterInnNoticeBoard(DWORD Kind)
{
	OnReqEnterNoticeBoardMsg	TempOnReqEnterNoticeBoardMsg;

	TempOnReqEnterNoticeBoardMsg.dwMsgType			=	ON_REQUEST_ENTER_NOTICEBOARD;
	
	return Write(&TempOnReqEnterNoticeBoardMsg, sizeof(OnReqEnterNoticeBoardMsg));
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명	:	계시판에서 글을 지운다.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
BOOL	OnlineClient::SendRequestDeleteNotice( DWORD dwPage, UI16 uiIndex )
{
	OnReqDeleteNoticeMsg		TempOnReqDeleteNoticeMsg;

	TempOnReqDeleteNoticeMsg.dwMsgType		=	ON_REQUEST_DELETE_NOTICE;
	TempOnReqDeleteNoticeMsg.dwPageNum		=	dwPage;
	TempOnReqDeleteNoticeMsg.uiNoticeIndex	=	uiIndex;

	return Write( &TempOnReqDeleteNoticeMsg, sizeof( OnReqDeleteNoticeMsg ) );	
}


//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명	:	게시판에서의 검색
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
BOOL	OnlineClient::SendRequestSearchNotice( UI08	uiType, char *pMsg )
{
/*
	UI08						TempBuffer[1024];
	OnReqSearchNoticeMsg		TempOnReqSearchNoticeMsg;

	TempOnReqSearchNoticeMsg.dwMsgType		=	ON_REQUEST_SEARCH_NOTICE;
	TempOnReqSearchNoticeMsg.uiSearchKind	=	uiType;
	TempOnReqSearchNoticeMsg.uiStringLength	=	strlen( pMsg );


	memcpy( TempBuffer, &TempOnReqSearchNoticeMsg, sizeof( 	OnReqSearchNoticeMsg ) );			 // 먼저 구조체를
	memcpy( &TempBuffer[ sizeof( 	OnReqSearchNoticeMsg ) ], pMsg, strlen( pMsg ) );			 // 다음에 내용을 붙여서 보내준다.

	return Write( TempBuffer, ( sizeof( OnReqSearchNoticeMsg ) + strlen( pMsg ) ) );
*/
	return FALSE;
}



//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명	:	게시판에 글을 요청 한다.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
BOOL	OnlineClient::SendRequestNoticesListMsg( UI16 uiPageID )
{
	OnReqNoticesListMsg			TempOnReqNoticesListMsg;

	TempOnReqNoticesListMsg.dwMsgType	=	ON_REQUEST_NOTICEBOARD_NOTICELIST;
	TempOnReqNoticesListMsg.uiPage		=	uiPageID;

	return Write( &TempOnReqNoticesListMsg, sizeof( OnReqNoticesListMsg ) );
}


//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명	:	게시판을 읽는다.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
BOOL	OnlineClient::SendRequestViewNotice( UI16	uiIndex )
{
	OnReqViewNoticeMsg		TempOnReqViewNoticeMsg;

	TempOnReqViewNoticeMsg.dwMsgType		=	ON_REQUEST_VIEW_NOTICE;
	TempOnReqViewNoticeMsg.uiNoticeIndex	=	uiIndex;

	return Write( &TempOnReqViewNoticeMsg, sizeof( OnReqViewNoticeMsg ) );
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명	:	내가 Portal에 들어간다는 메시지를 서버에게 알려준다.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
BOOL	OnlineClient::SendGotoPortal(SI16 siID)
{
	OnReqMoveMapMsg				TempOnReqMoveMapMsg;

	TempOnReqMoveMapMsg.dwMsgType  = ON_REQUEST_MOVEMAP;
	TempOnReqMoveMapMsg.uiPortalID = siID;

	return Write(&TempOnReqMoveMapMsg, sizeof(OnReqMoveMapMsg));
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명	:	훈련소에 들어간다.
BOOL	OnlineClient::SendRequestBarrackEnter(void)
{
	OnReqBarrackGetList		TempOnReqBarrackGetList;

	TempOnReqBarrackGetList.dwMsgType = ON_REQUEST_BARRACK_GETSOLDERLIST;

	return Write(&TempOnReqBarrackGetList, sizeof(OnReqBarrackGetList));
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명	:	훈련소에서 용병을 고용한다.
BOOL	OnlineClient::SendRequestBarrackBuySolder(SI32 Code, BYTE* lpName)
{
	OnReqBarrackBuySolder	TempOnReqBarrackBuySolder;

	ZeroMemory(&TempOnReqBarrackBuySolder, sizeof(OnReqBarrackBuySolder));
	TempOnReqBarrackBuySolder.dwMsgType     = ON_REQUEST_BARRACK_BUYSOLDER;
	TempOnReqBarrackBuySolder.dwBuySolderID = Code;
	strcpy((char*)TempOnReqBarrackBuySolder.szName, (char*)lpName);

	return Write(&TempOnReqBarrackBuySolder, sizeof(OnReqBarrackBuySolder));
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명	:	훈련소 해고에 들어간다.
BOOL	OnlineClient::SendRequestBarrackDismissalEnter(void)
{
	OnReqBarrackGetDismissalList		TempOnReqBarrackGetDismissalList;

	TempOnReqBarrackGetDismissalList.dwMsgType = ON_REQUEST_BARRACK_GETDISMISSALSOLDERLIST;

	return Write(&TempOnReqBarrackGetDismissalList, sizeof(OnReqBarrackGetDismissalList));
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명	:	훈련소에서 용병을 해고한다.
BOOL	OnlineClient::SendRequestBarrackDismissalSolder(SI32 Slot)
{
	OnReqBarrackDismissalSolder	TempOnReqBarrackDismissalSolder;

	ZeroMemory(&TempOnReqBarrackDismissalSolder, sizeof(OnReqBarrackDismissalSolder));
	TempOnReqBarrackDismissalSolder.dwMsgType = ON_REQUEST_BARRACK_DISMISSALSOLDER;
	TempOnReqBarrackDismissalSolder.dwSlot    = Slot;

	return Write(&TempOnReqBarrackDismissalSolder, sizeof(OnReqBarrackDismissalSolder));
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명	:	장수양성소에 들어간다.
BOOL	OnlineClient::SendRequestGBarrackEnter(void)
{
	OnReqGBarrackGetList		TempOnReqGBarrackGetList;

	ZeroMemory(&TempOnReqGBarrackGetList, sizeof(OnReqGBarrackGetList));
	TempOnReqGBarrackGetList.dwMsgType = ON_REQUEST_GBARRACK_GETSOLDERLIST;
	
	return Write(&TempOnReqGBarrackGetList, sizeof(OnReqGBarrackGetList));
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명	:	장수양성소에서 장수을 고용한다.
BOOL	OnlineClient::SendRequestGBarrackBuySolder(SI32 Code, BYTE* lpName)
{
	OnReqGBarrackBuySolder	TempOnReqGBarrackBuySolder;

	ZeroMemory(&TempOnReqGBarrackBuySolder, sizeof(OnReqGBarrackBuySolder));
	TempOnReqGBarrackBuySolder.dwMsgType     = ON_REQUEST_GBARRACK_BUYSOLDER;
	TempOnReqGBarrackBuySolder.dwBuySolderID = Code;
	strcpy((char*)TempOnReqGBarrackBuySolder.szName, (char*)lpName);

	return Write(&TempOnReqGBarrackBuySolder, sizeof(OnReqGBarrackBuySolder));
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명	:	장수양성소 전직에 들어간다.
BOOL	OnlineClient::SendRequestGBarrackChangeJobEnter(void)
{
	OnReqGBarrackGetChangeJobList		TempOnReqGBarrackGetChangeJobList;

	ZeroMemory(&TempOnReqGBarrackGetChangeJobList, sizeof(OnReqGBarrackGetChangeJobList));
	TempOnReqGBarrackGetChangeJobList.dwMsgType = ON_REQUEST_GBARRACK_GETCHANGEJOBSOLDERLIST;
	
	return Write(&TempOnReqGBarrackGetChangeJobList, sizeof(OnReqGBarrackGetChangeJobList));
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명	:	장수양성소에서 장수을 전직한다.
BOOL	OnlineClient::SendRequestGBarrackChangeJobSolder(SI32 Slot, SI32 Kind)
{
	OnReqGBarrackChangeJobSolder	TempOnReqGBarrackChangeJobSolder;

	ZeroMemory(&TempOnReqGBarrackChangeJobSolder, sizeof(OnReqGBarrackChangeJobSolder));
	TempOnReqGBarrackChangeJobSolder.dwMsgType       = ON_REQUEST_GBARRACK_CHANGEJOBSOLDER;
	TempOnReqGBarrackChangeJobSolder.dwChangeJobSlot = Slot;
	TempOnReqGBarrackChangeJobSolder.dwChangeJobKind = Kind;

	return Write(&TempOnReqGBarrackChangeJobSolder, sizeof(OnReqGBarrackChangeJobSolder));
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명	:	장수양성소 해고에 들어간다.
BOOL	OnlineClient::SendRequestGBarrackDismissalEnter(void)
{
	OnReqGBarrackGetDismissalList		TempOnReqGBarrackGetDismissalList;

	ZeroMemory(&TempOnReqGBarrackGetDismissalList, sizeof(OnReqGBarrackGetDismissalList));
	TempOnReqGBarrackGetDismissalList.dwMsgType = ON_REQUEST_GBARRACK_GETDISMISSALSOLDERLIST;
	
	return Write(&TempOnReqGBarrackGetDismissalList, sizeof(OnReqGBarrackGetDismissalList));
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명	:	장수양성소에서 장수을 해고한다.
BOOL	OnlineClient::SendRequestGBarrackDismissalSolder(SI32 Slot)
{
	OnReqGBarrackDismissalSolder	TempOnReqGBarrackDismissalSolder;

	ZeroMemory(&TempOnReqGBarrackDismissalSolder, sizeof(OnReqGBarrackDismissalSolder));
	TempOnReqGBarrackDismissalSolder.dwMsgType = ON_REQUEST_GBARRACK_DISMISSALSOLDER;
	TempOnReqGBarrackDismissalSolder.dwSlot    = Slot;

	return Write(&TempOnReqGBarrackDismissalSolder, sizeof(OnReqGBarrackDismissalSolder));
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명	:	2차 전직 장수양성소에 들어간다.(버튼을 누른다.)
BOOL	OnlineClient::SendRequestGBarrackGeneralExEnter(void)
{
	OnReqGBarrackGetList	TempOnReqGBarrackGetList;

	ZeroMemory(&TempOnReqGBarrackGetList,sizeof(OnReqGBarrackGetList));
	TempOnReqGBarrackGetList.dwMsgType = ON_REQUEST_GBARRACK_GET_GENERAL_LIST;

	return Write(&TempOnReqGBarrackGetList, sizeof(OnReqGBarrackGetList));;
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명	:	2차 전직 장수양성소에서 2차 장수를 고용한다.
BOOL	OnlineClient::SendRequestGBarrackBuyGeneralEx(SI32 Code, BYTE* lpName)
{
	OnReqGBarrackBuySolder	TempOnReqGBarrackBuySolder;

	ZeroMemory(&TempOnReqGBarrackBuySolder, sizeof(OnReqGBarrackBuySolder));
	TempOnReqGBarrackBuySolder.dwMsgType     = ON_REQUEST_GBARRACK_BUY_GENERAL;
	TempOnReqGBarrackBuySolder.dwBuySolderID = Code;
	strcpy((char*)TempOnReqGBarrackBuySolder.szName, (char*)lpName);

	return Write(&TempOnReqGBarrackBuySolder, sizeof(OnReqGBarrackBuySolder));
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//  설명	:	2차 전직 장수양성소에서 전직에 들어간다.
BOOL	OnlineClient::SendRequestGBarrackChangeGeneralExEnter(void)
{
	OnReqGBarrackGetChangeJobList		TempOnReqGBarrackGetChangeJobList;

	ZeroMemory(&TempOnReqGBarrackGetChangeJobList, sizeof(OnReqGBarrackGetChangeJobList));
	TempOnReqGBarrackGetChangeJobList.dwMsgType = ON_REQUEST_GBARRACK_GET_CHANGE_GENERAL_LIST;
	
	return Write(&TempOnReqGBarrackGetChangeJobList, sizeof(OnReqGBarrackGetChangeJobList));
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//  설명	:	2차 전직 장수양성소에서 전직 장수를 고용한다.
BOOL	OnlineClient::SendRequestGBarrackBuyChangeGeneralEx(SI32 Slot, SI32 Kind)
{
	OnReqGBarrackChangeJobSolder	TempOnReqGBarrackChangeJobSolder;

	ZeroMemory(&TempOnReqGBarrackChangeJobSolder, sizeof(OnReqGBarrackChangeJobSolder));
	TempOnReqGBarrackChangeJobSolder.dwMsgType       = ON_REQUEST_GBARRACK_CHANGE_GENERAL;
	TempOnReqGBarrackChangeJobSolder.dwChangeJobSlot = Slot;
	TempOnReqGBarrackChangeJobSolder.dwChangeJobKind = Kind;

	return Write(&TempOnReqGBarrackChangeJobSolder, sizeof(OnReqGBarrackChangeJobSolder));
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
// 2차 전직 장수양성소에서 해고에 들어간다.
BOOL	OnlineClient::SendRequestGBarrackDismissalGeneralExEnter(void)
{
	OnReqGBarrackGetDismissalList		TempOnReqGBarrackGetDismissalList;

	ZeroMemory(&TempOnReqGBarrackGetDismissalList, sizeof(OnReqGBarrackGetDismissalList));
	TempOnReqGBarrackGetDismissalList.dwMsgType = ON_REQUEST_GBARRACK_GETDISMISSAL_GENERAL_LIST;
	
	return Write(&TempOnReqGBarrackGetDismissalList, sizeof(OnReqGBarrackGetDismissalList));
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
// 2차 전직 장수양성소에서 장수를 해고한다.
BOOL	OnlineClient::SendRequestGBarrackSellDismissalGeneralEx(SI32 Slot)
{
	OnReqGBarrackDismissalSolder	TempOnReqGBarrackDismissalSolder;

	ZeroMemory(&TempOnReqGBarrackDismissalSolder, sizeof(OnReqGBarrackDismissalSolder));
	TempOnReqGBarrackDismissalSolder.dwMsgType = ON_REQUEST_GBARRACK_DISMISSAL_GENERAL;
	TempOnReqGBarrackDismissalSolder.dwSlot    = Slot;

	return Write(&TempOnReqGBarrackDismissalSolder, sizeof(OnReqGBarrackDismissalSolder));
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명	:	몬스터양성소에 들어간다.
BOOL	OnlineClient::SendRequestMBarrackEnter(void)
{
	OnReqMBarrackGetList		TempOnReqMBarrackGetList;

	TempOnReqMBarrackGetList.dwMsgType = ON_REQUEST_MBARRACK_GETMONSTERLIST;

	return Write(&TempOnReqMBarrackGetList, sizeof(OnReqMBarrackGetList));
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명	:	몬스터양성소에서 용병을 고용한다.
BOOL	OnlineClient::SendRequestMBarrackBuySolder(SI32 Code, BYTE* lpName)
{
	OnReqMBarrackBuyMonster		TempOnReqMBarrackBuyMonster;

	ZeroMemory(&TempOnReqMBarrackBuyMonster, sizeof(OnReqMBarrackBuyMonster));
	TempOnReqMBarrackBuyMonster.dwMsgType      = ON_REQUEST_MBARRACK_BUYMONSTER;
	TempOnReqMBarrackBuyMonster.dwBuyMonsterID = Code;
	strcpy((char*)TempOnReqMBarrackBuyMonster.szName, (char*)lpName);

	return Write(&TempOnReqMBarrackBuyMonster, sizeof(OnReqMBarrackBuyMonster));
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명	:	몬스터양성소 해고에 들어간다.
BOOL	OnlineClient::SendRequestMBarrackDismissalEnter(void)
{
	OnReqMBarrackGetDismissList		TempOnReqMBarrackGetDismissList;

	TempOnReqMBarrackGetDismissList.dwMsgType = ON_REQUEST_MBARRACK_GETDISMISSMONSTERLIST;

	return Write(&TempOnReqMBarrackGetDismissList, sizeof(OnReqMBarrackGetDismissList));
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명	:	몬스터양성소에서 용병을 해고한다.
BOOL	OnlineClient::SendRequestMBarrackDismissalSolder(SI32 Slot)
{
	OnReqMBarrackDismissMonster		TempOnReqMBarrackDismissMonster;

	ZeroMemory(&TempOnReqMBarrackDismissMonster, sizeof(OnReqMBarrackDismissMonster));
	TempOnReqMBarrackDismissMonster.dwMsgType = ON_REQUEST_MBARRACK_DISMISSMONSTER;
	TempOnReqMBarrackDismissMonster.dwSlot    = Slot;

	return Write(&TempOnReqMBarrackDismissMonster, sizeof(OnReqMBarrackDismissMonster));
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명	:	의원에서 치료 용병의 List를 얻어온다.
BOOL	OnlineClient::SendRequestHospitalGetCureList(void)
{
	OnReqHospitalGetCureList		TempOnReqHospitalGetCureList;

	TempOnReqHospitalGetCureList.dwMsgType = ON_REQUEST_HOSPITAL_GETCURELIST;

	return Write(&TempOnReqHospitalGetCureList, sizeof(OnReqHospitalGetCureList));
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명	:	의원에서 용병을 치료한다.
BOOL	OnlineClient::SendRequestHospitalCureSolder(UI08 Slot)
{
	OnReqHospitalCureSolder		TempOnReqHospitalCureSolder;

	ZeroMemory(&TempOnReqHospitalCureSolder, sizeof(OnReqHospitalCureSolder));
	TempOnReqHospitalCureSolder.dwMsgType        = ON_REQUEST_HOSPITAL_CURESOLDER;
	TempOnReqHospitalCureSolder.dwCureSolderSlot = Slot;

	return Write(&TempOnReqHospitalCureSolder, sizeof(OnReqHospitalCureSolder));
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명	:	의원에서 모든 용병을 치료한다.
BOOL	OnlineClient::SendRequestHospitalCureAllSolder(void)
{
	OnReqHospitalCureAllSolder		TempOnReqHospitalCureAllSolder;

	ZeroMemory(&TempOnReqHospitalCureAllSolder, sizeof(OnReqHospitalCureAllSolder));
	TempOnReqHospitalCureAllSolder.dwMsgType = ON_REQUEST_HOSPITAL_CUREALLSOLDER;

	return Write(&TempOnReqHospitalCureAllSolder, sizeof(OnReqHospitalCureAllSolder));
}


//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명	:	장부, 총 계좌를 요구한다.
BOOL	OnlineClient::SendAuditAccountList()
{
	OnReqAuditAccountListMsg		TempOnReqAuditAccountListMsg;

	TempOnReqAuditAccountListMsg.dwMsgType	=	ON_REQUEST_AUDIT_ACCOUNT;

	return Write( &TempOnReqAuditAccountListMsg, sizeof( OnReqAuditAccountListMsg ) );
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명	:	장부에서 부동산 리스트를 요구 한다.
BOOL	OnlineClient::SendAuditOwnListMsg(void)
{
	OnReqAuditOwnListMsg		TempOnReqAuditOwnListMsg;

	TempOnReqAuditOwnListMsg.dwMsgType	=	ON_REQUEST_AUDIT_OWN;

	return Write( &TempOnReqAuditOwnListMsg, sizeof( OnReqAuditOwnListMsg ) );

}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명	:	장부에서 길드 리스트를 요구한다.
BOOL	OnlineClient::SendAuditGuildMemberList()
{
	OnReqAuditGuildMemberInfoMsg	TempOnAuditGuildMemberInfoMsg;

	TempOnAuditGuildMemberInfoMsg.dwMsgType = ON_REQUEST_AUDIT_GUILD_INFO;

	return Write(&TempOnAuditGuildMemberInfoMsg,sizeof(OnReqAuditGuildMemberInfoMsg));
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명	:	부두에 들어간다.
BOOL	OnlineClient::SendRequestWharfEnter(void)
{
	OnReqWharf_Enter			TempOnReqWharf_Enter;

	TempOnReqWharf_Enter.dwMsgType = ON_REQUEST_WHARF_ENTER;
	return Write(&TempOnReqWharf_Enter, sizeof(OnReqWharf_Enter));
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명	:	부두에서 표를 산다.
BOOL	OnlineClient::SendRequestWharfBuyTicket(SI32 TicketKind)
{
	OnReqWharf_BuyTicket			TempOnReqWharf_BuyTicket;

	TempOnReqWharf_BuyTicket.dwMsgType    = ON_REQUEST_WHARF_BUYTICKET;
	TempOnReqWharf_BuyTicket.siTicketKind = TicketKind;
	return Write(&TempOnReqWharf_BuyTicket, sizeof(OnReqWharf_BuyTicket));
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명	:	부두에서 나간다.
BOOL	OnlineClient::SendRequestWharfExit(void)
{
	OnReqWharf_Exit			TempOnReqWharf_Exit;

	TempOnReqWharf_Exit.dwMsgType    = ON_REQUEST_WHARF_EXIT;
	return Write(&TempOnReqWharf_Exit, sizeof(OnReqWharf_Exit));
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명	:	부두대기실에 들어간다.
BOOL	OnlineClient::SendRequestWharfWaitRoomEnter(void)
{
	OnReqWharfWaitRoom_Enter			TempOnReqWharfWaitRoom_Enter;

	TempOnReqWharfWaitRoom_Enter.dwMsgType    = ON_REQUEST_WHARFWAITROOM_ENTER;
	return Write(&TempOnReqWharfWaitRoom_Enter, sizeof(OnReqWharfWaitRoom_Enter));
}
	
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명	:	부두대기실에서 나간다.
BOOL	OnlineClient::SendRequestWharfWaitRoomExit(void)
{
	OnReqWharfWaitRoom_Exit			TempOnReqWharfWaitRoom_Exit;

	TempOnReqWharfWaitRoom_Exit.dwMsgType    = ON_REQUEST_WHARFWAITROOM_EXIT;
	return Write(&TempOnReqWharfWaitRoom_Exit, sizeof(OnReqWharfWaitRoom_Enter));
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명	:	농장 정보를 요청을 한다.
BOOL	OnlineClient::SendRequestFramInfo(UI08 uiPlantID)
{
	OnReqFarmInfoMsg				TempOnReqFarmInfoMsg;

	TempOnReqFarmInfoMsg.dwMsgType	=	ON_REQUEST_FARM_INFO;
	TempOnReqFarmInfoMsg.uiPlantID	=	uiPlantID;

	return Write( &TempOnReqFarmInfoMsg, sizeof( OnReqFarmInfoMsg ) );
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명	:	생산시설 - 건물을 산다
BOOL	OnlineClient::SendRequestMakeBuy(UI08 uiPlantID, SI16 siSlot)
{
	OnReqFarmBuyMsg					TempOnReqFarmBuyMsg;

	TempOnReqFarmBuyMsg.dwMsgType	=	ON_REQUEST_FARM_BUY;
	TempOnReqFarmBuyMsg.uiFarmIndex	=	(UI08)siSlot;
	TempOnReqFarmBuyMsg.uiPlantID	=	uiPlantID;

	return Write( &TempOnReqFarmBuyMsg, sizeof( OnReqFarmBuyMsg ) );
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명	:	생산시설 - 건물을 건설
BOOL	OnlineClient::SendRequestBuildStruct( SI16 siPay, SI16 siSlot, CHAR *pBuildKind )
{
	OnReqFarmBuildStructMsg			TempOnReqFarmBuildStructMsg;

	TempOnReqFarmBuildStructMsg.dwMsgType	=	ON_REQUEST_FARM_BUILDSTRUCT;		
	TempOnReqFarmBuildStructMsg.uiSlotIndex	=	(UI08)siSlot;
	TempOnReqFarmBuildStructMsg.siPay		=	siPay;

	strcpy( TempOnReqFarmBuildStructMsg.Code, pBuildKind );
	
	return Write( &TempOnReqFarmBuildStructMsg, sizeof( OnReqFarmBuildStructMsg ) );
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명	:	생산시설 - 슬롯의 정보를 요청한다.
BOOL	OnlineClient::SendRequestSlotInfo( UI08 uiPlantID, SI16 siSlot )
{
	OnReqFarmSlotInfoMsg		TempOnReqFarmSlotInfoMsg;

	TempOnReqFarmSlotInfoMsg.dwMsgType		=	ON_REQUEST_FARM_SLOTINFO;
	TempOnReqFarmSlotInfoMsg.uiFarmIndex	=	(UI08)siSlot;
	TempOnReqFarmSlotInfoMsg.uiPlantID		=	uiPlantID;

	return Write( &TempOnReqFarmSlotInfoMsg, sizeof( OnReqFarmSlotInfoMsg ) );
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명	:	생산시설 - 생산 취소
BOOL	OnlineClient::SendRequseCancel( UI08 uiPlantID, UI08 uiFarmIndex, UI08 uiSlotIndex )
{
	OnReqPlantCancelPlanMsg		TempOnReqPlantCancelPlanMsg;

	TempOnReqPlantCancelPlanMsg.dwMsgType	=	ON_REQUEST_PLANT_CANCEL_PLAN;
	TempOnReqPlantCancelPlanMsg.uiPlantID	=	uiPlantID;
	TempOnReqPlantCancelPlanMsg.uiFarmIndex	=	uiFarmIndex;
	TempOnReqPlantCancelPlanMsg.uiSlotIndex	=	uiSlotIndex;

	return Write( &TempOnReqPlantCancelPlanMsg, sizeof( OnReqPlantCancelPlanMsg ) );
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명	:	생산시설 - 건물을 업그레이드 한다.
BOOL	OnlineClient::SendRequestSetEquipment( UI08 uiPlantID, SI16 siFarm, SI16 siSlot, UI08 uiUpIndex )
{
	OnReqFarmSetEquipmentMsg		TempOnReqFarmSetEquipmentMsg;

	TempOnReqFarmSetEquipmentMsg.dwMsgType		=	ON_REQUEST_FARM_SETEQUIPMENT;
	TempOnReqFarmSetEquipmentMsg.uiPlantID		=	uiPlantID;
	TempOnReqFarmSetEquipmentMsg.uiFarmIndex	=	(UI08)siFarm;
	TempOnReqFarmSetEquipmentMsg.uiSlotIndex	=	(UI08)siSlot;
	TempOnReqFarmSetEquipmentMsg.uiUpIndex		=	uiUpIndex;

	return Write( &TempOnReqFarmSetEquipmentMsg, sizeof( OnReqFarmSetEquipmentMsg ) );

}
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명	:	생산시설 - 창고의 정보를 요청한다.
BOOL	OnlineClient::SendRequestStorageInfo( UI08 uiPlantID, UI08 uiPartID, UI08 uiSlotIndex )
{
	OnReqStorageInfoMsg			TempOnReqStorageInfoMsg;	

	TempOnReqStorageInfoMsg.dwMsgType	=	ON_REQUEST_STORAGE_INFO;
	TempOnReqStorageInfoMsg.uiPlantID	=	uiPlantID;
	TempOnReqStorageInfoMsg.uiPartID	=	uiPartID;
	TempOnReqStorageInfoMsg.uiSlotIndex	=	uiSlotIndex;

	return Write( &TempOnReqStorageInfoMsg, sizeof( OnReqStorageInfoMsg ) );

}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명	:	생상시설 - 건물이 어떤 아이템을 만드가?
BOOL	OnlineClient::SendRequestWorkInfo( UI08 uiPlantID, SI16 siFarm, UI08 uiSlot )
{
	OnReqFarmWorkInfoMsg				TempOnReqFarmWorkInfoMsg;
	
	TempOnReqFarmWorkInfoMsg.dwMsgType		=	ON_REQUEST_FARM_WORKINFO;	
	TempOnReqFarmWorkInfoMsg.uiPlantID		=	uiPlantID;
	TempOnReqFarmWorkInfoMsg.uiFarmIndex	=	(UI08)siFarm;
	TempOnReqFarmWorkInfoMsg.uiSlotIndex	=	uiSlot;

	return Write( &TempOnReqFarmWorkInfoMsg, sizeof( OnReqFarmWorkInfoMsg ) );
}
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명	:	생산시설 - 창고의 아이템을 넣는다.
BOOL	OnlineClient::SendRequestStorageInPut( UI08 uiPlantID, UI08 uiPartID, UI08 uiSlotIndex, UI16 uiQunatity, UI08 uiFollowerID, SI16 siPosInInventory, UI16 uiItemCode )
{
	OnReqStorageInputItemMsg		TempOnReqStorageInputItemMsg;

	TempOnReqStorageInputItemMsg.dwMsgType			=	ON_REQUEST_STORAGE_INPUTITEM;
	TempOnReqStorageInputItemMsg.siPosInInventory	=	siPosInInventory;
	TempOnReqStorageInputItemMsg.uiFollowerID		=	uiFollowerID;
	TempOnReqStorageInputItemMsg.uiItemCode			=	uiItemCode;
	TempOnReqStorageInputItemMsg.uiPartID			=	uiPartID;
	TempOnReqStorageInputItemMsg.uiPlantID			=	uiPlantID;
	TempOnReqStorageInputItemMsg.uiQunatity			=	uiQunatity;
	TempOnReqStorageInputItemMsg.uiSlotIndex		=	uiSlotIndex;

	return Write( &TempOnReqStorageInputItemMsg, sizeof( OnReqStorageInputItemMsg ) );
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
// 생산시설 - 아이템을 생산 한다고  
BOOL	OnlineClient::SendRequestMakeItem( UI08 uiPlantID, UI08 uiFarmIndex, UI08 uiSlotIndex, UI16 uiItemCode, UI32 uiQuantity, MONEY uiPay )
{
	OnReqFarmItemProductMsg				TempOnReqFarmItemProductMsg;

	TempOnReqFarmItemProductMsg.dwMsgType	=	ON_REQUEST_FARM_ITEMPRODUCT;
	TempOnReqFarmItemProductMsg.uiPlantID	=	uiPlantID;
	TempOnReqFarmItemProductMsg.uiFarmIndex	=	uiFarmIndex;
	TempOnReqFarmItemProductMsg.uiSlotIndex	=	uiSlotIndex;
	TempOnReqFarmItemProductMsg.uiItemCode	=	uiItemCode;
	TempOnReqFarmItemProductMsg.uiQuantity	=	uiQuantity;
	TempOnReqFarmItemProductMsg.uiPay		=	(SI16)uiPay;	

	return Write( &TempOnReqFarmItemProductMsg, sizeof( OnReqFarmItemProductMsg ) );
}


//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
// 생산시설 - 생산시설 - 노동을 한다.
BOOL	OnlineClient::SendRequestWork( UI08 uiSlotIndex, UI08 uiGameType, SI32 siLabor, SI08 siStep )
{
	OnReqFarmWorkMsg				TempOnReqFarmWorkMsg;

	TempOnReqFarmWorkMsg.dwMsgType		=	ON_REQUEST_FARM_WORK;
	TempOnReqFarmWorkMsg.uiSlotIndex	=	uiSlotIndex;
	TempOnReqFarmWorkMsg.uiGameType		=	uiGameType;
	TempOnReqFarmWorkMsg.siLabor		=	siLabor;
	TempOnReqFarmWorkMsg.siStep			=	siStep;

	if(TempOnReqFarmWorkMsg.siLabor != GetMiniGameWork())
	{	
		SendFarmWorkHack(GetMiniGameType()*10 + 4, siStep, TempOnReqFarmWorkMsg.siLabor, GetMiniGameWork());
		return FALSE;
	}

	if(pMyOnlineWorld->pOnlinePannel->m_bHomeKey)
	{
		pMyOnlineWorld->pOnlinePannel->m_bHomeKey = FALSE;

		if(rand()%2 != 0)
		{
			pMyOnlineWorld->pOnlineMsgBoxError->SetMsg(ON_MSGBOX_TYPE_OK, "", pMyOnlineWorld->pOnlineText->Get(2283));
			return FALSE;
		}
	}
	
	if(pMyOnlineWorld->IsRunningProcess("AutoPlay.exe"))
	{
		if(rand()%6 != 0)
		{
			pMyOnlineWorld->pOnlineMsgBoxError->SetMsg(ON_MSGBOX_TYPE_OK, "", pMyOnlineWorld->pOnlineText->Get(2283));
			return FALSE;
		}
	}


	return Write( &TempOnReqFarmWorkMsg, sizeof( OnReqFarmWorkMsg ));
}

BOOL	OnlineClient::SendFarmWorkHack( UI08 uiType, SI08 siStep, SI32 siPoint1, SI32 siPoint2 )
{
	OnFarmWorkHack				TempOnFarmWorkHack;

	TempOnFarmWorkHack.dwMsgType	=	ON_REQUEST_FARM_WORK_HACK;
	TempOnFarmWorkHack.uiType		=	uiType;
	TempOnFarmWorkHack.siStep		=	siStep;
	TempOnFarmWorkHack.siPoint1		=	siPoint1;
	TempOnFarmWorkHack.siPoint2		=	siPoint2;

	return Write( &TempOnFarmWorkHack, sizeof( OnFarmWorkHack ));
}

BOOL OnlineClient::SendMacroUsingUserInfo(const char* szMacroProgName)
{
	OnReqMacroUsingUserInfo	TempOnMacroUsingUserInfo;
	memset(&TempOnMacroUsingUserInfo, 0, sizeof(OnReqMacroUsingUserInfo));

	TempOnMacroUsingUserInfo.dwMsgType = ON_REQUEST_MACRO_USING_USER_INFO;
	strcpy(TempOnMacroUsingUserInfo.szMacroProgName, szMacroProgName);

	return Write(&TempOnMacroUsingUserInfo, sizeof(OnReqMacroUsingUserInfo));
}

BOOL OnlineClient::SendGameGuardUserInfo(DWORD dwType, const char* szMsg)
{
	OnReqGameGuardUserInfo	TempOnReqGameGuardUserInfo;
	memset(&TempOnReqGameGuardUserInfo, 0, sizeof(TempOnReqGameGuardUserInfo));

	TempOnReqGameGuardUserInfo.dwMsgType = ON_USER_INFO_GAMEGUARD;
	TempOnReqGameGuardUserInfo.dwGameGuardMsgType = dwType;

	strncpy(TempOnReqGameGuardUserInfo.szMemo, szMsg, sizeof(TempOnReqGameGuardUserInfo.szMemo));
	TempOnReqGameGuardUserInfo.szMemo[sizeof(TempOnReqGameGuardUserInfo.szMemo) - 1] = '\0';

	return Write(&TempOnReqGameGuardUserInfo, sizeof(TempOnReqGameGuardUserInfo));
}


//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
// 생산시설 - 슬롯을 업글 한다.
BOOL	OnlineClient::SendRequseUpgradeSlot( UI08 uiSlot, SI16 siPay )
{
	OnReqFarmUpgradeSlotMsg		TempOnReqFarmUpgradeSlotMsg;

	TempOnReqFarmUpgradeSlotMsg.dwMsgType	=	ON_REQUEST_FARM_UPGRADESLOT;
	TempOnReqFarmUpgradeSlotMsg.siPay		=	siPay;
	TempOnReqFarmUpgradeSlotMsg.uiSlotIndex	=	uiSlot;

	return Write( &TempOnReqFarmUpgradeSlotMsg, sizeof( OnReqFarmUpgradeSlotMsg ) );
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
// 생산시설 - 아이템을 가지고 간다.
BOOL	OnlineClient::SendRequestOutPut( UI08 uiPlantID,UI08 uiPartID,UI08 uiSlotIndex,UI08 uiFollowerID,UI16 uiItemCode, UI32 uiQunatity, SI16	siPosInInventory )
{
	OnReqStorageOutputItemMsg		TempOnReqStorageOutputItemMsg;

	TempOnReqStorageOutputItemMsg.dwMsgType			=	ON_REQUEST_STORAGE_OUTPUTITEM;
	TempOnReqStorageOutputItemMsg.uiPlantID			=	uiPlantID;
	TempOnReqStorageOutputItemMsg.uiPartID			=	uiPartID;
	TempOnReqStorageOutputItemMsg.uiSlotIndex		=	uiSlotIndex;
	TempOnReqStorageOutputItemMsg.uiFollowerID		=	uiFollowerID;	
	TempOnReqStorageOutputItemMsg.uiItemCode		=	uiItemCode;
	TempOnReqStorageOutputItemMsg.uiQunatity		=	uiQunatity;
	TempOnReqStorageOutputItemMsg.siPosInInventory	=	siPosInInventory;

	return Write( &TempOnReqStorageOutputItemMsg, sizeof( OnReqStorageOutputItemMsg ) );
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
// 생산시설 - 슬롯을 확장을 한다.
BOOL	OnlineClient::SendRequesExtension( UI08 uiPlantID, UI08 uiFarmIndex )
{
	OnReqFarmExpandSlotMsg			TempOnReqFarmExpandSlotMsg;

	TempOnReqFarmExpandSlotMsg.dwMsgType	=	ON_REQUEST_FARM_EXPANDSLOT;
	TempOnReqFarmExpandSlotMsg.uiPlantID	=	uiPlantID;
	TempOnReqFarmExpandSlotMsg.uiFarmIndex	=	uiFarmIndex;

	return Write( &TempOnReqFarmExpandSlotMsg, sizeof( OnReqFarmExpandSlotMsg ) );
}


//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
// 생산시설 - 슬롯에 있는 건물을 제거
BOOL	OnlineClient::SendRequesDestroyStrcut( UI08 uiPlantID, UI08 uiFarmIndex, UI08 uiSlotIndex )
{
	OnReqFarmDestroyStructMsg		TempOnReqFarmDestroyStructMsg;

	TempOnReqFarmDestroyStructMsg.dwMsgType		=	ON_REQUEST_FARM_DESTROYSTRUCT;
	TempOnReqFarmDestroyStructMsg.uiPlantID		=	uiPlantID;
	TempOnReqFarmDestroyStructMsg.uiFarmIndex	=	uiFarmIndex;
	TempOnReqFarmDestroyStructMsg.uiSlotIndex	=	uiSlotIndex;

	return Write( &TempOnReqFarmDestroyStructMsg, sizeof( OnReqFarmDestroyStructMsg ) );
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
// 생산시설 - 일하는 곳에 도구 있는 알아 본다.
BOOL	OnlineClient::SendRequseFarmWorkStartMsg( UI08 uiSlot )
{
	OnReqFarmWorkStartMsg		TempOnReqFarmWorkStartMsg;

	TempOnReqFarmWorkStartMsg.dwMsgType		=	ON_REQUEST_FARM_WORKSTART;
	TempOnReqFarmWorkStartMsg.uiSlotIndex	=	uiSlot;

	return Write( &TempOnReqFarmWorkStartMsg, sizeof( OnReqFarmWorkStartMsg ) );
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
// 생산시설 - 임금을 변경
BOOL	OnlineClient::SendRequseChangePay( UI08 uiPlantID, UI08 uiFarmIndex, UI08 uiSlotIndex, SI16 siPay )
{
	OnReqPlantChangePayMsg	TempOnReqPlantChangePayMsg;

	TempOnReqPlantChangePayMsg.dwMsgType	=	ON_REQUEST_PLANT_CHANGE_PAY;
	TempOnReqPlantChangePayMsg.uiPlantID	=	uiPlantID;
	TempOnReqPlantChangePayMsg.uiFarmIndex	=	uiFarmIndex;
	TempOnReqPlantChangePayMsg.uiSlotIndex	=	uiSlotIndex;
	TempOnReqPlantChangePayMsg.siPay		=	siPay;

	return Write( &TempOnReqPlantChangePayMsg, sizeof( OnReqPlantChangePayMsg ) );
}


//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
// 생산시설 - 단지에서 나간다고 보내준다.
BOOL	OnlineClient::SendRequestLeaveStructure()
{
	OnReqLeaveStructureMsg	TempOnReqLeaveStructureMsg;

	TempOnReqLeaveStructureMsg.dwMsgType	=	ON_REQUEST_LEAVE_STRUCTURE;

	return Write( &TempOnReqLeaveStructureMsg, sizeof( OnReqLeaveStructureMsg ) );
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
// 생산시설 - 공장에서 나간다고 보내준다.
BOOL	OnlineClient::SendRequestLeaveFactory()
{
	OnReqLeaveFactoryMsg	TempOnReqLeaveFactoryMsg;

	TempOnReqLeaveFactoryMsg.dwMsgType	=	ON_REQUEST_LEAVE_FACTORY;

	return Write( &TempOnReqLeaveFactoryMsg, sizeof( OnReqLeaveFactoryMsg ) );
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명	:	아아템 착용
BOOL	OnlineClient::SendRequestAttachItem( UI08 uiFollower, SI16 siPos, UI32 uiID, SI16 siGroup )
{
	if( siGroup == ON_WEAR_NO )		return FALSE;

	OnReqWearItemMsg				TempOnReqWearItemMsg;

	TempOnReqWearItemMsg.dwMsgType		=	ON_REQUEST_WEARITEM;
	TempOnReqWearItemMsg.uiFollowerID	=	uiFollower;
	TempOnReqWearItemMsg.uiItemID		=	uiID;
	TempOnReqWearItemMsg.uiPosInventory	=	siPos;
	TempOnReqWearItemMsg.uiWearLoc		=	(UI08)siGroup;;

	return Write( &TempOnReqWearItemMsg, sizeof( OnReqWearItemMsg ) );
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명	:	아이템 벗다
BOOL	OnlineClient::SendRequestStripItem( UI08 uiFollower, SI16 siPos, UI32 uiID, SI16 siGroup )
{
	if( siGroup == ON_WEAR_NO )		return FALSE;

	OnReqStripItemMsg			TempOnReqStripItemMsg;
	
	TempOnReqStripItemMsg.dwMsgType			=	ON_REQUEST_STRIPITEM;
	TempOnReqStripItemMsg.uiFollowerID		=	uiFollower;
	TempOnReqStripItemMsg.uiItemID			=	uiID;
	TempOnReqStripItemMsg.uiPosInventory	=	siPos;
	TempOnReqStripItemMsg.uiWearLoc			=	(UI08)siGroup;

	return Write( &TempOnReqStripItemMsg, sizeof( OnReqStripItemMsg ) );
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명	:	아이템 사용
BOOL	OnlineClient::SendRequestUseItem( UI08 uiFollower, SI16 siPos, UI32 uiID, UI08 uiNum )
{
	OnReqSpendItemMsg			TempOnReqSpendItemMsg;

	TempOnReqSpendItemMsg.dwMsgType			=	ON_REQUEST_SPENDITEM;
	TempOnReqSpendItemMsg.uiFollowerID		=	uiFollower;
	TempOnReqSpendItemMsg.uiItemID			=	(UI16)uiID;
	TempOnReqSpendItemMsg.uiPosInventory	=	siPos;	
	TempOnReqSpendItemMsg.uiQuantity		=	uiNum;

	return Write( &TempOnReqSpendItemMsg, sizeof( OnReqSpendItemMsg ) );
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명	:	착용하고 있는 아이템을 받는다.
BOOL	OnlineClient::SendRequestItemInfo()
{
	OnReqWearItemInfoMsg			TempOnReqWearItemInfoMsg;

	TempOnReqWearItemInfoMsg.dwMsgType	=	ON_REQUEST_WEARITEM_INFO;

	return Write( &TempOnReqWearItemInfoMsg, sizeof( OnReqWearItemInfoMsg ) );
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명	:	관청 들어가기
BOOL	OnlineClient::SendRequestGovernment_Main_Enter(void)
{
	OnReqEnterGovernment			TempOnReqEnterGovernment;

	TempOnReqEnterGovernment.dwMsgType	=	ON_REQUEST_GOVERNMENT_MAIN_ENTER;

	return Write(&TempOnReqEnterGovernment, sizeof(OnReqEnterGovernment));
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명	:	관청 나가기
BOOL	OnlineClient::SendRequestGovernment_Main_Leave(void)
{
	OnReqLeaveGovernment			TempOnReqLeaveGovernment;

	TempOnReqLeaveGovernment.dwMsgType	=	ON_REQUEST_GOVERNMENT_MAIN_LEAVE;

	return Write(&TempOnReqLeaveGovernment, sizeof(OnReqLeaveGovernment));
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명	:	관청, 투자 들어가기
BOOL	OnlineClient::SendRequestGovernment_Invenstment_Enter(SI16 Type)
{
	OnReqGovernmentEnterInvestment		TempOnReqGovernmentEnterInvestment;

	TempOnReqGovernmentEnterInvestment.dwMsgType	=	ON_REQUEST_GOVERNMENT_INVESTMENT_ENTER;
	TempOnReqGovernmentEnterInvestment.m_Type       =   Type;

	return Write(&TempOnReqGovernmentEnterInvestment, sizeof(OnReqGovernmentEnterInvestment));
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명	:	관청, 투자 나가기
BOOL	OnlineClient::SendRequestGovernment_Invenstment_Leave(void)
{
	OnReqGovernmentLeaveInvestment		TempOnReqGovernmentLeaveInvestment;

	TempOnReqGovernmentLeaveInvestment.dwMsgType	=	ON_REQUEST_GOVERNMENT_INVESTMENT_LEAVE;

	return Write(&TempOnReqGovernmentLeaveInvestment, sizeof(OnReqGovernmentLeaveInvestment));
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명	:	관청, 투자 List얻기
BOOL	OnlineClient::SendRequestGovernment_Invenstment_GetList(SI16 VillageList)
{
	OnReqGovernmentGetInvestmentList			TempOnReqGovernmentGetInvestmentList;

	TempOnReqGovernmentGetInvestmentList.dwMsgType	=	ON_REQUEST_GOVERNMENT_INVESTMENT_GETLIST;
//	TempOnReqGovernmentGetInvestmentList.m_Type     =   Type; 
	TempOnReqGovernmentGetInvestmentList.siVillageCode	= VillageList;

	return Write(&TempOnReqGovernmentGetInvestmentList, sizeof(OnReqGovernmentGetInvestmentList));
}

BOOL	OnlineClient::SendRequest_Investment_Gains_GetList(int code)
{
	OnReqAuditProfitStatistics					TempOnReqAuditProfitStatistics;

	TempOnReqAuditProfitStatistics.dwMsgType =  ON_REQUEST_AUDIT_PROFIT_STATISTICS;
	
	TempOnReqAuditProfitStatistics.uiVillageCode	= code; 

	return Write(&TempOnReqAuditProfitStatistics, sizeof(OnReqAuditProfitStatistics));

}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명	:	관청, 투자 하기
BOOL	OnlineClient::SendRequestGovernment_Invenstment_SetInvestment(SI16 Type, MONEY InvestmentMoney_InPocket, MONEY InvestmentMoney_InBank/* = 0*/)
{
	OnReqGovernmentSetInvestment			TempOnReqGovernmentSetInvestment;

	TempOnReqGovernmentSetInvestment.dwMsgType				= ON_REQUEST_GOVERNMENT_INVESTMENT_SETINVESTMENT;
//	TempOnReqGovernmentSetInvestment.m_Type					= Type;
	TempOnReqGovernmentSetInvestment.m_InvestmentMoney		= InvestmentMoney_InPocket;	// 투자하려는 돈(소지금)
	TempOnReqGovernmentSetInvestment.m_InvestmentSaveMoney	= InvestmentMoney_InBank;	// 투자하려는 돈(전장)

	// 마을에 투자하는 총 투자금은 InvestmentMoney_InPocket + InvestmentMoney_InBank 이며,
	// 사용자가 폭탄 투자 권한을이용하여 투자하는 경에만 전장의 돈(InvestmentMoney_InBank)이 사용된다.

	return Write(&TempOnReqGovernmentSetInvestment, sizeof(OnReqGovernmentSetInvestment));
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명	:	마을 소개문 읽기
BOOL	OnlineClient::SendRequestVillageDescript_Read(UI16 uiVillageCode)
{
	OnReqVillageDescript			TempOnReqVillageDescript;

	TempOnReqVillageDescript.dwMsgType		 = ON_REQUEST_READ_DESCRIPT;
	TempOnReqVillageDescript.uiVillageCode	 = uiVillageCode;

	return Write(&TempOnReqVillageDescript, sizeof(OnReqVillageDescript));
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명	:	마을 소개문 쓰기
BOOL	OnlineClient::SendRequestVillageDescript_Write(char* lpText)
{
	BYTE						TempBuffer[2048];
	OnReqWriteDescript*			TempOnReqWriteDescript;
	SI32						TempSize;
	char*						TemplpBuffer;

	TempOnReqWriteDescript = (OnReqWriteDescript*)TempBuffer;
	TemplpBuffer           = (char*)(((char*)TempOnReqWriteDescript) + sizeof(OnReqWriteDescript));

	TempOnReqWriteDescript->dwMsgType = ON_REQUEST_WRITE_DESCRIPT;
	TempOnReqWriteDescript->uiLength  = strlen(lpText) + 1;
	strcpy(TemplpBuffer, lpText);
	TemplpBuffer[TempOnReqWriteDescript->uiLength - 1] = NULL;

	TempSize = TempOnReqWriteDescript->uiLength + sizeof(OnReqWriteDescript);

	return Write(TempOnReqWriteDescript, TempSize);
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명	:	마을 정보 요청
BOOL	OnlineClient::SendRequestVillageData(UI16 uiVillageCode)
{
	OnReqVillageGetNation			TempOnReqVillageGetNation;

	TempOnReqVillageGetNation.dwMsgType		= ON_REQUEST_VILLAGE_NATION;
	TempOnReqVillageGetNation.uiVillageCode	= uiVillageCode;

	return Write(&TempOnReqVillageGetNation, sizeof(OnReqVillageGetNation));
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명	:	이모티콘 사용
BOOL	OnlineClient::SendRequestImoticon(UI16 ImoticonID)
{
	OnReqImoticonMsg		TempOnReqImoticonMsg;

	TempOnReqImoticonMsg.dwMsgType    = ON_REQUEST_IMOTICON;
	TempOnReqImoticonMsg.uiImoticonID = ImoticonID;

	return Write(&TempOnReqImoticonMsg, sizeof(TempOnReqImoticonMsg));
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명	:	거래 요청
BOOL	OnlineClient::SendRequestAskTrade( UI16 uiID )
{
	OnReqAskTradeMsg		TempOnReqAskTradeMsg;

	TempOnReqAskTradeMsg.dwMsgType	=	ON_REQUEST_TRADE;
	TempOnReqAskTradeMsg.UserID		=	uiID;

	return Write( &TempOnReqAskTradeMsg, sizeof( OnReqAskTradeMsg ) );
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명	:	거래 요청 받은 사람이 다시 보내는 메세지
BOOL	OnlineClient::SendRequestTrade(BOOL bAbccept)
{
	OnResAskTradeMsg		TempOnResAskTradeMsg;

	TempOnResAskTradeMsg.dwMsgType	= ON_REQUEST_ACCEPT_TRADE;
	TempOnResAskTradeMsg.dwResponse = bAbccept == TRUE ? TRUE : FALSE;

	return Write( &TempOnResAskTradeMsg, sizeof( OnResAskTradeMsg ) );
}


// 설명		: 파티 요청.
BOOL	OnlineClient::SendRequestAskParty(UI16	uiID)
{

	OnReqAskPartyMsg		TempOnReqAskPartyMsg;

	TempOnReqAskPartyMsg.dwMsgType	=	ON_REQUEST_PARTY;
	TempOnReqAskPartyMsg.UserID		=	uiID;

	return Write( &TempOnReqAskPartyMsg, sizeof( OnReqAskPartyMsg ) );
}

// 설명		: 파티 요청 받은 사람이 다시 보내는 메세지.
BOOL	OnlineClient::SendRequestParty(DWORD dwAccept)
{
	OnResAskPartyMsg	  TempOnResAskPartyMsg;

	TempOnResAskPartyMsg.dwMsgType	= ON_REQUEST_ACCEPT_PARTY;
	TempOnResAskPartyMsg.dwResponse = dwAccept;

	return Write( &TempOnResAskPartyMsg,sizeof(OnResAskPartyMsg));
}

BOOL	OnlineClient::SendRequestQuitParty()
{
	OnResponseMsg		pOnResPartyQuitMsg;

	pOnResPartyQuitMsg.dwMsgType	= ON_REQUEST_LEAVE_PARTY;

	return Write( &pOnResPartyQuitMsg,sizeof(OnResponseMsg));
}


BOOL	OnlineClient::SendRequestPartyCancel()
{
	OnResponseMsg		pOnResPartyCancel;

	pOnResPartyCancel.dwMsgType = ON_REQUEST_CANCEL_REQUESTING_PARTY;

	return Write( &pOnResPartyCancel,sizeof(OnResponseMsg));
}



//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명	:	물건을 올려 놓는다.
BOOL	OnlineClient::SendRequestPreTrade( UI08 uiSlot, UI16 uiID ,UI16 uiItemNum, MONEY Money, UI16 uiFollower, BOOL bProperty)
{
	OnReqChangeTradeGoods	TempOnReqChangeTradeGoods;

	TempOnReqChangeTradeGoods.dwMsgType	= ON_REQUEST_CHANGE_TRADE_GOODS;	
	TempOnReqChangeTradeGoods.uiSlot	= uiSlot;

	if(uiID)	
	{
		TempOnReqChangeTradeGoods.Kind 	  =	TRADE_GOODS_ITEM;		
		TempOnReqChangeTradeGoods.lParam1 = uiFollower;
		TempOnReqChangeTradeGoods.lParam2 = MAKELPARAM(uiItemNum, uiID);
	}
	else if(Money)
	{
		TempOnReqChangeTradeGoods.Kind 	  =	TRADE_GOODS_MONEY;
		TempOnReqChangeTradeGoods.lParam1 = (LPARAM)Money;
		TempOnReqChangeTradeGoods.lParam2 = 0;
	}
	else if(uiFollower)
	{
		TempOnReqChangeTradeGoods.Kind 	  =	TRADE_GOODS_FOLLOWER;
		TempOnReqChangeTradeGoods.lParam1 = uiFollower;
		TempOnReqChangeTradeGoods.lParam2 = 0;
	}
	else if(bProperty)
	{
		TempOnReqChangeTradeGoods.Kind 	  =	TRADE_GOODS_REALESTATE;
		TempOnReqChangeTradeGoods.lParam1 = 0;
		TempOnReqChangeTradeGoods.lParam2 = 0;
	}

	return Write( &TempOnReqChangeTradeGoods, sizeof( OnReqChangeTradeGoods ) );
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명	:	거래 취소
BOOL	OnlineClient::SendRequestCancelTrade()
{
	OnReqCancelTradeMsg		TempOnReqCancelTradeMsg;

	TempOnReqCancelTradeMsg.dwMsgType	=	ON_REQUEST_CANCEL_TRADE;

	return Write( &TempOnReqCancelTradeMsg, sizeof( OnReqCancelTradeMsg ) );
}


//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명	:	거래 버튼를 눌름
BOOL	OnlineClient::SendRequestConfirmTrade( BOOL bFlag )
{
	OnReqFinishChooseTradeItemMsg			TempOnReqFinishChooseTradeItemMsg;

	TempOnReqFinishChooseTradeItemMsg.dwMsgType			=	ON_REQUEST_FINISH_CHOOSE_TRADEITEM;
	TempOnReqFinishChooseTradeItemMsg.bFinishChooseItem	=	bFlag;

	return Write( &TempOnReqFinishChooseTradeItemMsg, sizeof( OnReqFinishChooseTradeItemMsg ) );
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명	:	수락 버튼을 눌름
BOOL	OnlineClient::SendRequestTradeSuccess(BOOL bPush)
{
	OnReqDecideTradeMsg		TempOnReqDecideTradeMsg;

	TempOnReqDecideTradeMsg.dwMsgType		=	ON_REQUEST_DECIDE_TRADE;
	TempOnReqDecideTradeMsg.bDecideTrade	=	bPush;

	return Write( &TempOnReqDecideTradeMsg, sizeof(OnReqDecideTradeMsg) );
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명	:	운영자 명령
BOOL	OnlineClient::SendGMMessage(char* lpChatting)
{
	BYTE					TempBuffer[1024];
	char*					TempChatting;
	OnCommandMsg*			TemplpOnCommandMsg;

	TemplpOnCommandMsg = (OnCommandMsg*)TempBuffer;
	TempChatting         = (char*)(TempBuffer + sizeof(OnCommandMsg));
	TemplpOnCommandMsg->dwMsgType      = ON_COMMAND;
	TemplpOnCommandMsg->siStringLength = strlen(lpChatting);
	strcpy(TempChatting, lpChatting);

	return Write(TempBuffer, sizeof(OnCommandMsg) + TemplpOnCommandMsg->siStringLength);	
}
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명	:	보너스 수치 
BOOL	OnlineClient::SendBouns( UI08 uiFollowerID, AbilityKindType Ability )
{
	OnReqIncAbilityByBonusMsg		TempOnReqIncAbilityByBonusMsg;


	TempOnReqIncAbilityByBonusMsg.dwMsgType		=	ON_REQUEST_INC_ABILITY_BYBONUS;
	TempOnReqIncAbilityByBonusMsg.uiFollowerID	=	uiFollowerID;
	TempOnReqIncAbilityByBonusMsg.AbilityKind	=	Ability;

	return Write( &TempOnReqIncAbilityByBonusMsg, sizeof( TempOnReqIncAbilityByBonusMsg ) );
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명	:	좌판을 처음 개설한다.
BOOL	OnlineClient::SendOpenBoothMsg(UI16 uiItemNum, OnBoothItemForSell *pBoothItemForSell)
{
	OnReqPMOpenMsg					TempOnReqPMOpenMsg;
	CHAR							szTempBuffer[1024];
	SI16							siSize, uiLength;

	TempOnReqPMOpenMsg.dwMsgType		=	ON_REQUEST_PM_OPEN;
	TempOnReqPMOpenMsg.uiItemNum		=	uiItemNum;

	siSize	=	sizeof( OnReqPMOpenMsg );
	ZeroMemory( szTempBuffer, 1024 );
	memcpy( szTempBuffer, &TempOnReqPMOpenMsg, siSize );
	uiLength = sizeof( OnBoothItemForSell ) * uiItemNum;
	memcpy( &szTempBuffer[ siSize ], pBoothItemForSell, uiLength );
	siSize	+=	uiLength;

	return Write( szTempBuffer, siSize );
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명	:	좌판 개설후 아이템 변경 및 추가
BOOL	OnlineClient::SendChangeBoothItemMsg(UI16 uiItemNum, OnBoothItemForSell *pBoothItemForSell)
{
	OnReqChangeBoothItemMsg			TempOnReqChangeBoothItem;
	CHAR							szTempBuffer[1024];
	SI16							siSize, uiLength;

	TempOnReqChangeBoothItem.dwMsgType		=	ON_REQUEST_PM_CHANGEITEM;
	TempOnReqChangeBoothItem.uiItemNum		=	uiItemNum;

	siSize	=	sizeof( OnReqPMOpenMsg );
	ZeroMemory( szTempBuffer, 1024 );
	memcpy( szTempBuffer, &TempOnReqChangeBoothItem, siSize );
	uiLength = sizeof( OnBoothItemForSell ) * uiItemNum;
	memcpy( &szTempBuffer[ siSize ], pBoothItemForSell, uiLength );
	siSize	+=	uiLength;

	return Write( szTempBuffer, siSize );
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명	:	좌판대에서 나간다.
BOOL	OnlineClient::SendLeaveBoothMsg()
{
	OnReqPMCloseMsg				TempOnReqPMCloseMsg;

	TempOnReqPMCloseMsg.dwMsgType	=	ON_REQUEST_PM_CLOSE;

	return Write( &TempOnReqPMCloseMsg, sizeof( OnReqPMCloseMsg ) );
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명	:	좌판에 있는 아이템을 요청을 한다.
BOOL	OnlineClient::SendDisplayBoothItemMsg( UI16 uiBoothID )
{
	OnReqDisplayBoothItemMsg		TempOnReqDisplayBoothItemMsg;

	TempOnReqDisplayBoothItemMsg.dwMsgType	=	ON_REQUEST_DISPLAYBOOTH;
	TempOnReqDisplayBoothItemMsg.uiBoothID	=	uiBoothID;

	return Write( &TempOnReqDisplayBoothItemMsg, sizeof( OnReqDisplayBoothItemMsg ) );
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명	:	좌판에 있는 아이템 정보를 얻어온다.
BOOL	OnlineClient::SendGetBoothItemInfo(UI16	uiAccount)
{
	OnReqPMItemInfoMsg			TempOnReqPMItemInfoMsg;

	TempOnReqPMItemInfoMsg.dwMsgType	=	ON_REQUEST_PM_ITEMINFO;
	TempOnReqPMItemInfoMsg.uiAccount	=	uiAccount;
//	TempOnReqPMItemInfoMsg.uiSlotID		=	uiSlotID;

	return Write( &TempOnReqPMItemInfoMsg, sizeof( OnReqPMItemInfoMsg ) );
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명	:	좌판물건을 구입하다.
BOOL	OnlineClient::SendBuyBoothItem(UI16 uiAccount, UI16 uiQuantity, UI08 uiSlotID, UI08 uiInvenPos, UI16 uiItemID, UI08 uiFollowerID)
{
	OnReqPMBuyItemMsg			TempOnReqPMBuyItemMsg;

	TempOnReqPMBuyItemMsg.dwMsgType		=	ON_REQUEST_PM_BUYITEM;
	TempOnReqPMBuyItemMsg.uiAccount		=	uiAccount;
	TempOnReqPMBuyItemMsg.uiQuantity	=	uiQuantity;
	TempOnReqPMBuyItemMsg.uiSlotID		=	uiSlotID;
	TempOnReqPMBuyItemMsg.uiItemID		=	uiItemID;
	TempOnReqPMBuyItemMsg.uiInvenPos	=	uiInvenPos;
	TempOnReqPMBuyItemMsg.uiFollowerID	=	uiFollowerID;

	return Write( &TempOnReqPMBuyItemMsg, sizeof( OnReqPMBuyItemMsg ) );
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명	:	마을 정보를 달라고 요청
BOOL	OnlineClient::SendRequestGovernment_VillageInfo(void)
{
	OnReqGovernmentVillageInfo		TempOnReqGovernmentVillageInfo;

	TempOnReqGovernmentVillageInfo.dwMsgType = ON_REQUEST_GOVERNMENT_VILLAGEINFO;
	return Write( &TempOnReqGovernmentVillageInfo, sizeof( TempOnReqGovernmentVillageInfo ));
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명	:	다른 상단정보를 얻어옴
BOOL	OnlineClient::SendRequestGovernment_GetDiffGuild(void)
{
	OnReqGuildListMsg				TempOnReqGuildListMsg;

	TempOnReqGuildListMsg.dwMsgType = ON_REQUEST_GUILD_LIST;
	return Write(&TempOnReqGuildListMsg, sizeof(TempOnReqGuildListMsg));
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명	:	상단에 가입할려는 플레이어 정보를 얻어옴
BOOL	OnlineClient::SendRequestGovernment_GetJoinGuild(SI32 PageNum)
{
	OnReqGuildWaitingJoinMembersListMsg				TempOnReqGuildWaitingJoinMembersListMsg;

	TempOnReqGuildWaitingJoinMembersListMsg.dwMsgType	=	ON_REQUEST_GUILD_WAITING_JOIN_MEMBERS_LIST;
	TempOnReqGuildWaitingJoinMembersListMsg.dwPage      =   PageNum; 

	return Write(&TempOnReqGuildWaitingJoinMembersListMsg, sizeof(TempOnReqGuildWaitingJoinMembersListMsg));
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명	:	상단에서 해고할려는 플레이어 정보를 얻어옴
BOOL	OnlineClient::SendRequestGovernment_GetDismissalGuild(SI32 PageNum, UI32 uiGuildID)
{
	OnReqGuildMembersListMsg				TempOnReqGuildMembersListMsg;

	TempOnReqGuildMembersListMsg.dwMsgType	=	ON_REQUEST_GUILD_MEMBERS_LIST;
	TempOnReqGuildMembersListMsg.dwPage     =   PageNum;
	TempOnReqGuildMembersListMsg.uiGuidlID  =   uiGuildID;

	return Write(&TempOnReqGuildMembersListMsg, sizeof(TempOnReqGuildMembersListMsg));
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명	:	상단을 만듬
BOOL	OnlineClient::SendRequestGovernment_MakeGuild(char* lpName)
{
	char								TempBufer[256];
	char*								TempBufer1;
	OnReqCreateGuildMsg*				lpOnReqCreateGuildMsg;

	ZeroMemory(TempBufer, 256);
	lpOnReqCreateGuildMsg = (OnReqCreateGuildMsg*)TempBufer;
	TempBufer1            = TempBufer + sizeof(OnReqCreateGuildMsg);

	lpOnReqCreateGuildMsg->dwMsgType	     = ON_REQUEST_CREATE_GUILD;
	lpOnReqCreateGuildMsg->uiGuildNameLength = strlen(lpName);
	strncpy(TempBufer1, lpName, ON_GUILDNAME_LENGTH);

	return Write(lpOnReqCreateGuildMsg, sizeof(OnReqCreateGuildMsg) + lpOnReqCreateGuildMsg->uiGuildNameLength);
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명	:	상단을 해체
BOOL	OnlineClient::SendRequestGovernment_DeleteGuild(void)
{
	OnReqDeleteGuildMsg				TempOnReqDeleteGuildMsg;

	TempOnReqDeleteGuildMsg.dwMsgType	=	ON_REQUEST_DELETE_GUILD;

	return Write(&TempOnReqDeleteGuildMsg, sizeof(TempOnReqDeleteGuildMsg));
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명	:	상단 가입/탈퇴
BOOL OnlineClient::SendRequestGovernment_GuildJoinLeave(void)
{
	OnReqJoinLeaveGuildMsg				TempOnReqJoinLeaveGuildMsg;

	TempOnReqJoinLeaveGuildMsg.dwMsgType	=	ON_REQUEST_JOIN_LEAVE_GUILD;

	return Write(&TempOnReqJoinLeaveGuildMsg, sizeof(TempOnReqJoinLeaveGuildMsg));
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명	:	상단 가입을 허락함
BOOL OnlineClient::SendRequestGovernment_JoinGuildPlayer(char* lpID)
{
	OnReqJoinGuildOkNoMsg		TempOnReqJoinGuildOkNoMsg;

	ZeroMemory(&TempOnReqJoinGuildOkNoMsg, sizeof(TempOnReqJoinGuildOkNoMsg));
	TempOnReqJoinGuildOkNoMsg.dwMsgType	 = ON_REQUEST_JOINGUILD_OKNO;
	TempOnReqJoinGuildOkNoMsg.dwResponse = ON_RET_OK;
	strncpy((char*)TempOnReqJoinGuildOkNoMsg.szID, lpID, ON_ID_LENGTH);

	return Write(&TempOnReqJoinGuildOkNoMsg, sizeof(TempOnReqJoinGuildOkNoMsg));
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명	:	상단 가입을 거부함
BOOL OnlineClient::SendRequestGovernment_JoinCancelGuildPlayer(char* lpID)
{
	OnReqJoinGuildOkNoMsg		TempOnReqJoinGuildOkNoMsg;

	ZeroMemory(&TempOnReqJoinGuildOkNoMsg, sizeof(TempOnReqJoinGuildOkNoMsg));
	TempOnReqJoinGuildOkNoMsg.dwMsgType	 = ON_REQUEST_JOINGUILD_OKNO;
	TempOnReqJoinGuildOkNoMsg.dwResponse = ON_RET_NO;
	strncpy((char*)TempOnReqJoinGuildOkNoMsg.szID, lpID, ON_ID_LENGTH);

	return Write(&TempOnReqJoinGuildOkNoMsg, sizeof(TempOnReqJoinGuildOkNoMsg));
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명	:	상단 탈퇴를 시킴
BOOL OnlineClient::SendRequestGovernment_DismissalGuildPlayer(char* lpID)
{
	OnReqDismissalMsg		TempOnReqDismissalMsg;

	ZeroMemory(&TempOnReqDismissalMsg, sizeof(TempOnReqDismissalMsg));
	TempOnReqDismissalMsg.dwMsgType	=	ON_REQUEST_GUILD_DISMISSAL;
	strncpy((char*)TempOnReqDismissalMsg.szID, lpID, ON_ID_LENGTH);

	return Write(&TempOnReqDismissalMsg, sizeof(TempOnReqDismissalMsg));
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명	:	상단 동맹, 적대 설정
BOOL OnlineClient::SendRequestGovernment_SetRelation(char* lpGuildID, SI32 RelationType)
{
	OnReqGuildSetRelationMsg	TempOnReqGuildSetRelationMsg;

	ZeroMemory(&TempOnReqGuildSetRelationMsg, sizeof(OnReqGuildSetRelationMsg));
	TempOnReqGuildSetRelationMsg.dwMsgType  = ON_REQUEST_GUILD_SET_RELATION;
	TempOnReqGuildSetRelationMsg.dwResponse = RelationType;
	strncpy((char*)TempOnReqGuildSetRelationMsg.szGuildID, lpGuildID, ON_GUILDNAME_LENGTH);

	return Write(&TempOnReqGuildSetRelationMsg, sizeof(TempOnReqGuildSetRelationMsg));
}

// robypark 2004/11/25 15:51
// 공성전 화친제의 메시지 보내기
BOOL OnlineClient::SendRequestGovernment_OfferPeace(CHAR *szGuildID, CHAR *szMessage)
{
	OnReqGuildFriendlyMessageOffer	TempOnReqGuildFriendlyMessageOffer;

	ZeroMemory(&TempOnReqGuildFriendlyMessageOffer, sizeof(OnReqGuildFriendlyMessageOffer));

	TempOnReqGuildFriendlyMessageOffer.dwMsgType  = ON_REQUEST_GUILD_FRIELDLY_MESSAGE_OFFER;
	strncpy((char*)TempOnReqGuildFriendlyMessageOffer.szGuildID, szGuildID, ON_GUILDNAME_LENGTH);	// 적대상단 이름
	strncpy((char*)TempOnReqGuildFriendlyMessageOffer.szMessage, szMessage, 80);					// 화친제의 메시지

	return Write(&TempOnReqGuildFriendlyMessageOffer, sizeof(TempOnReqGuildFriendlyMessageOffer));
}

// robypark 2004/11/25 16:16
// 공성전 화친제의 수락 메시지 보내기
BOOL OnlineClient::SendRequestGovernment_AcceptOfferedPeace(CHAR *szGuildID)
{
	OnReqGuildFriendlyMessageAccept	TempOnReqGuildFriendlyMessageAccept;

	ZeroMemory(&TempOnReqGuildFriendlyMessageAccept, sizeof(OnReqGuildFriendlyMessageAccept));

	TempOnReqGuildFriendlyMessageAccept.dwMsgType  = ON_REQUEST_GUILD_FRIENDLY_MESSAGE_ACCEPT;
	strncpy((char*)TempOnReqGuildFriendlyMessageAccept.szGuildID, szGuildID, ON_GUILDNAME_LENGTH);	// 적대상단 이름

	return Write(&TempOnReqGuildFriendlyMessageAccept, sizeof(TempOnReqGuildFriendlyMessageAccept));
}

// robypark 2004/11/25 16:16
// 공성전 화친제의 거절 메시지 보내기
BOOL OnlineClient::SendRequestGovernment_DeclineOfferedPeace(CHAR *szGuildID)
{
	OnReqGuildFreindlyMessageCancel	TempOnReqGuildFreindlyMessageCancel;

	ZeroMemory(&TempOnReqGuildFreindlyMessageCancel, sizeof(OnReqGuildFreindlyMessageCancel));

	TempOnReqGuildFreindlyMessageCancel.dwMsgType  = ON_REQUEST_GUILD_FRIENDLY_MESSAGE_CANCEL;
	strncpy((char*)TempOnReqGuildFreindlyMessageCancel.szGuildID, szGuildID, ON_GUILDNAME_LENGTH);	// 적대상단 이름

	return Write(&TempOnReqGuildFreindlyMessageCancel, sizeof(TempOnReqGuildFreindlyMessageCancel));
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명	:	상단 마을리스트 요청
BOOL OnlineClient::SendRequestGovernment_GetGuildVillageList(char* lpGuildID)
{
	OnReqGuildDetailInfoMsg		TempOnReqGuildDetailInfoMsg;

	ZeroMemory(&TempOnReqGuildDetailInfoMsg, sizeof(OnReqGuildDetailInfoMsg));
	TempOnReqGuildDetailInfoMsg.dwMsgType  = ON_REQUEST_GUILD_DETAIL_INFO;
	strncpy((char*)TempOnReqGuildDetailInfoMsg.szGuildID, lpGuildID, ON_GUILDNAME_LENGTH);

	return Write(&TempOnReqGuildDetailInfoMsg, sizeof(TempOnReqGuildDetailInfoMsg));
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명	:	마을의 방어력에 투자됄수 있는 최대돈을 얻어온다.
BOOL OnlineClient::SendRequestGovernment_GetMaxVillageDefenceInvestment(void)
{
	OnReqMaxVillageDefenceInvestment		TempOnReqMaxVillageDefenceInvestment;

	TempOnReqMaxVillageDefenceInvestment.dwMsgType = ON_REQUEST_MAXVILLAGEDEFENCEINVESTMENT;

	return Write(&TempOnReqMaxVillageDefenceInvestment, sizeof(TempOnReqMaxVillageDefenceInvestment));
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명	:	마을의 방어력에 투자한다.
BOOL OnlineClient::SendRequestGovernment_SetVillageDefenceInvestment(MONEY DefenceMoney)
{
	OnReqSetVillageDefenceInvestment		TempOnReqSetVillageDefenceInvestment;

	TempOnReqSetVillageDefenceInvestment.dwMsgType       = ON_REQUEST_SETVILLAGEDEFENCEINVESTMENT;
	TempOnReqSetVillageDefenceInvestment.InvestmentMoney = DefenceMoney;

	return Write(&TempOnReqSetVillageDefenceInvestment, sizeof(TempOnReqSetVillageDefenceInvestment));
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명	:	용병의 이름을 변경
BOOL OnlineClient::SendRequestSetRename( CHAR *pRename, UI08 uiFollower )
{
	OnReqSetNameMsg			TempOnReqSetNameMsg;

	TempOnReqSetNameMsg.dwMsgType		=	ON_REQUEST_AUDIT_SETNAME;
	TempOnReqSetNameMsg.uiFollowerNum	=	uiFollower;
	strncpy( TempOnReqSetNameMsg.cName, pRename, ON_ID_LENGTH );

	return Write( &TempOnReqSetNameMsg, sizeof( OnReqSetNameMsg ) );
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명	:	현재 TickCount를 전송 
BOOL OnlineClient::SendGameTickCount( DWORD TickCount, UI16 uiType )
{
	OnHeartBeatMsg		TempOnHeartBeatMsg;

	TempOnHeartBeatMsg.dwMsgType	= ON_HEARTBEAT;
	TempOnHeartBeatMsg.dwTimer		= TickCount;
	TempOnHeartBeatMsg.uiType		= uiType;

	return Write( &TempOnHeartBeatMsg, sizeof( OnHeartBeatMsg ) );
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명	:	마을을 공격한다.(근거리)
BOOL	OnlineClient::SendShortDistanceAttackVillage( UI16 VillageCode, SI16 siAttackType )
{
	OnReqAttackVillageMsg		TempOnReqAttackVillageMsg;

	TempOnReqAttackVillageMsg.dwMsgtype     = ON_REQUEST_ATTACK_VILLAGE;
	TempOnReqAttackVillageMsg.uiVillageCode = VillageCode;
	TempOnReqAttackVillageMsg.siAttackType	= siAttackType;/*ON_ATTACK_TYPE_NORMAL;*/

	return Write(&TempOnReqAttackVillageMsg, sizeof(OnReqAttackVillageMsg));
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명	:	마을을 공격한다.(원거리)
BOOL	OnlineClient::SendLongDistanceAttackVillage( UI16 VillageCode ,SI16 siAttackCharX, SI16 siAttackCharY)
{
	OnReqLongAttackVillageRdyMsg	TempOnReqLongAttackVillageRdyMsg;

	TempOnReqLongAttackVillageRdyMsg.dwMsgType		=	ON_REQUEST_LONGDISTANCE_VILLAGEATTACK_READY;
	TempOnReqLongAttackVillageRdyMsg.uiVillageCode	=	VillageCode;
//	TempOnReqLongAttackVillageRdyMsg.siAttackCharX	=	siAttackCharX;
//	TempOnReqLongAttackVillageRdyMsg.siAttackCharY	=	siAttackCharY;

	return Write(&TempOnReqLongAttackVillageRdyMsg, sizeof(OnReqLongAttackVillageRdyMsg));
}

BOOL	OnlineClient::SendLongDistanceAttackVillageAction(UI16 VillageCode, SI16 siAttackType)
{	
	OnReqLongAttackVillageActionMsg		TempOnReqLongAttackVillageActionMsg;
	
	TempOnReqLongAttackVillageActionMsg.dwMsgType		=	ON_REQUEST_LONGDISTANCE_VILLAGEATTACK_ACTION;
	TempOnReqLongAttackVillageActionMsg.uiVillageCode	=	VillageCode;
	TempOnReqLongAttackVillageActionMsg.siAttackType	=	siAttackType;/*ON_ATTACK_TYPE_NORMAL;*/

	return Write(&TempOnReqLongAttackVillageActionMsg, sizeof(OnReqLongAttackVillageActionMsg));
}

// 공성전 수성 인터페이스로 들어가겠다고 요청
BOOL	OnlineClient::SendEnterVillageDefenceInterface(UI16 uiVillageCode)
{
	OnReqWarVillageDefence			TempOnReqWarVillageDefence;

	TempOnReqWarVillageDefence.dwMsgType		=	ON_REQUEST_WAR_VILLAGE_DEFENCE;
	TempOnReqWarVillageDefence.uiVillageCode	=	uiVillageCode;

	return Write(&TempOnReqWarVillageDefence, sizeof(OnReqWarVillageDefence));
}

/*
// 공성전 수성 인터페이스 상에서 마을에 들어가겠다고 요청
BOOL	OnlineClient::SendEnterVillageFromVillageDefenceInterface(UI16 uiVillageCode)
{
	OnReqWarVillageIn			TempOnReqWarVillageIn;
	
	TempOnReqWarVillageIn.dwMsgType			=	ON_REQUEST_WAR_VILLAGE_IN;
	TempOnReqWarVillageIn.uiVillageCode		=	uiVillageCode;

	return Write(&TempOnReqWarVillageIn, sizeof(OnReqWarVillageIn));
}
*/

// 공성전 수성 인터페이스 상에서 나가겠다고 요청
BOOL	OnlineClient::SendLeaveVillageDefenceInterface(UI16 uiVillageCode)
{
	OnReqWarVillageIn			TempOnReqWarVillageIn;
	
	TempOnReqWarVillageIn.dwMsgType			=	ON_REQUEST_VILLAGE_DEFENCE_STOP;
	TempOnReqWarVillageIn.uiVillageCode		=	uiVillageCode;

	return Write(&TempOnReqWarVillageIn, sizeof(OnReqWarVillageIn));
}

// 공성전
// 마을의 공격 타입을 변경이 가능한지 요청하기
BOOL	OnlineClient::SendCanChangeVillageAttackType(UI16 VillageCode)
{
	OnReqVillageAttackTypeCheck		TempOnReqVillageAttackTypeCheck;

	TempOnReqVillageAttackTypeCheck.dwMsgType		=	ON_REQUEST_VILLAGE_ATTACK_TYPE_CHECK;
	TempOnReqVillageAttackTypeCheck.uiVillageCode	=	VillageCode;

	return Write(&TempOnReqVillageAttackTypeCheck, sizeof(OnReqVillageAttackTypeCheck));
}

// 공성전
// 마을 기름 공격 시도 요청하기
BOOL	OnlineClient::SendVillagePlayerOilAttack(UI16  VillageCode)
{
	OnReqVillagePlayerOilAttack		TempOnReqVillagePlayerOilAttack;

	TempOnReqVillagePlayerOilAttack.dwMsgType		=	ON_REQUEST_VILLAGE_PLAYER_OIL_ATTACK;
	TempOnReqVillagePlayerOilAttack.uiVillageCode	=	VillageCode;

	return Write(&TempOnReqVillagePlayerOilAttack, sizeof(OnReqVillagePlayerOilAttack));
}

// 공성전
// 마을 돌 공격 시도 용청하기
BOOL	OnlineClient::SendVillagePlayerStoneAttack(UI16  VillageCode)
{
	OnReqVillagePlayerStoneAttack		TempOnReqVillagePlayerStoneAttack;

	TempOnReqVillagePlayerStoneAttack.dwMsgType		=	ON_REQUEST_VILLAGE_PLAYER_STONE_ATTACK;
	TempOnReqVillagePlayerStoneAttack.uiVillageCode	=	VillageCode;

	return Write(&TempOnReqVillagePlayerStoneAttack, sizeof(OnReqVillagePlayerStoneAttack));
}

// 공성전
// 마을 방어력 향상 요청하기
BOOL	OnlineClient::SendVillageDefenceUp(UI16 VillageCode)
{
	OnReqPlayerVillageDefenceUp		TempOnReqPlayerVillageDefenceUp;

	TempOnReqPlayerVillageDefenceUp.dwMsgType		=	ON_REQUEST_VILLAGE_DEFENCE;
	TempOnReqPlayerVillageDefenceUp.uiVillageCode	=	VillageCode;

	return Write(&TempOnReqPlayerVillageDefenceUp, sizeof(OnReqPlayerVillageDefenceUp));
}

// robypark 2005/1/28 18:3
// 공성전 유닛으로 변신하겠다고 서버에 요청
BOOL	OnlineClient::SendTransformToSiegeUnit(SI16 siChangeKindIdx)
{
	OnReqChangeUnit		TempOnReqChangeUnit;

	TempOnReqChangeUnit.dwMsgType		=	ON_REQUEST_CHANGE_WAR;
	TempOnReqChangeUnit.siChangeKind	=	siChangeKindIdx;		// 변신하려는 병과의 배열 인덱스(예: 검병 0, ...)

	return Write(&TempOnReqChangeUnit, sizeof(OnReqChangeUnit));
}

// robypark 2005/1/29 14:13
// 공성전 보급수레가 마나 10초마다 자동 회복 요청
BOOL	OnlineClient::SendReuqestChargeMP(void)
{
	OnMsg			TempOnMsg;

	TempOnMsg.dwMsgType		=	ON_REQUEST_CHARGE_MP;

	return Write(&TempOnMsg, sizeof(OnMsg));
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명	:	공성전시 캐릭터가 캐릭터를 공격한다는 메세지를 알려준다.
BOOL	OnlineClient::SendFieldAttackReady(UI16	uiUniqueID,SI16 siAttackCharX,SI16 siAttackCharY)
{
	OnReqFieldAttackRdyMsg		TempOnReqFieldAttackRdyMsg;

	TempOnReqFieldAttackRdyMsg.dwMsgType		= ON_REQUEST_FIELDATTACK_READY;
	TempOnReqFieldAttackRdyMsg.uiUniqueID		= uiUniqueID;
//	TempOnReqFieldAttackRdyMsg.siAttackCharX	= siAttackCharX;
//	TempOnReqFieldAttackRdyMsg.siAttackCharY	= siAttackCharY;
	return Write(&TempOnReqFieldAttackRdyMsg,sizeof(OnReqFieldAttackRdyMsg));
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명	:	공성전시 캐릭터가 사용한 무기에 맞았다.
BOOL	OnlineClient::SendFieldAttackAction(UI16 uiEnemyCharID, SI16 siAttackType)
{
	OnReqFieldAttackActionMsg	TempOnReqFieldAttackActionMsg;

	TempOnReqFieldAttackActionMsg.dwMsgType		= ON_REQUEST_FIELDATTACK_ACTION;
	TempOnReqFieldAttackActionMsg.uiUniqueID	= uiEnemyCharID;
	//Gscprotocol 참조 속성에 따라 siAttackType을 정한다.
	TempOnReqFieldAttackActionMsg.siAttackType	= siAttackType;/*ON_ATTACK_TYPE_NORMAL;*/


	return Write(&TempOnReqFieldAttackActionMsg,sizeof(OnReqFieldAttackActionMsg));
}
 
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명	:	체력이 다되서 죽었다.
BOOL	OnlineClient::SendFieldAttackDead(UI16	uiUniqueID)
{
	OnReqFieldAttackDeadMsg	TempOnReqFieldAttackDeadMsg;

	TempOnReqFieldAttackDeadMsg.dwMsgType	= ON_REQUEST_FIELDATTACK_DEAD;
	TempOnReqFieldAttackDeadMsg.uiShooterID	= uiUniqueID;

	return Write(&TempOnReqFieldAttackDeadMsg,sizeof(OnReqFieldAttackDeadMsg));
}

BOOL	OnlineClient::SendIncreaseGuildUnit(SI16 siKind,SI16 siCount, SI16 siVillageCode)
{
	OnReqIncreaseGuildUnit			TempOnReqIncreaseGuildUnit;
	
	TempOnReqIncreaseGuildUnit.dwMsgType			=	ON_REQUEST_INCREASE_GUILD_UNIT;
	TempOnReqIncreaseGuildUnit.siKind				=	siKind;
	TempOnReqIncreaseGuildUnit.siCount				=	siCount;
	TempOnReqIncreaseGuildUnit.uiVillageCode		=	siVillageCode;

	return Write(&TempOnReqIncreaseGuildUnit, sizeof(OnReqIncreaseGuildUnit));
}

BOOL	OnlineClient::SendDecreaseGuildUnit(SI16 siKind,SI16 siCount)
{
	OnReqDecreaseGuildUnit			TempOnReqDecreaseGuildUnit;

	TempOnReqDecreaseGuildUnit.dwMsgType			=	ON_REQUEST_DECREASE_GUILD_UNIT;
	TempOnReqDecreaseGuildUnit.siKind				=	siKind;
	TempOnReqDecreaseGuildUnit.siCount				=	siCount;

	return Write(&TempOnReqDecreaseGuildUnit, sizeof(OnReqDecreaseGuildUnit));
}

BOOL	OnlineClient::SendGetGuildUnitList(UI32 uiGuildID)
{
	OnReqGetGuildUnitList			TempOnReqGetGuildUnitList;

	TempOnReqGetGuildUnitList.dwMsgType				=	ON_REQUEST_LIST_GUILD_UNIT;
	TempOnReqGetGuildUnitList.uiGuildID				=	uiGuildID;

	return Write(&TempOnReqGetGuildUnitList, sizeof(OnReqGetGuildUnitList));
}

//마을 안에서 상단 보급품의 갯수를 늘린다.(대방.행수만 가능)
BOOL	OnlineClient::SendIncreaseGuildSupply(SI32 siSupply,SI16 siVillageCode)
{
	OnReqIncreaseGuildSupply	TempOnReqIncreaseGuildSupply;

	TempOnReqIncreaseGuildSupply.dwMsgType			=	ON_REQUEST_INCREASE_GUILD_SUPPLY;
	TempOnReqIncreaseGuildSupply.siSupplyQuantity	=	siSupply;
	TempOnReqIncreaseGuildSupply.siVillageCode		=	siVillageCode;

	return Write(&TempOnReqIncreaseGuildSupply, sizeof(OnReqIncreaseGuildSupply));
}

BOOL	OnlineClient::SendDecreaseGuildSupply(SI32 siSupply,SI16 siVillageCode)
{
	OnReqDecreaseGuildSupply	TempOnReqDecreaseGuildSupply;

	TempOnReqDecreaseGuildSupply.dwMsgType			=	ON_REQUEST_DECREASE_GUILD_SUPPLY;
	TempOnReqDecreaseGuildSupply.siSupplyQuantity	=	siSupply;			// 필요한 량
	TempOnReqDecreaseGuildSupply.siVillageCode		=	siVillageCode;		// 상단 마을

	return Write(&TempOnReqDecreaseGuildSupply, sizeof(OnReqDecreaseGuildSupply));
}

BOOL	OnlineClient::SendGetGuildSupplyList(SI16 siVillageCode)
{
	OnReqGetGuildSupplyList		TempOnReqGetGuildSupplyList;

	TempOnReqGetGuildSupplyList.dwMsgType			=	ON_REQUEST_LIST_GUILD_SUPPLY;
	TempOnReqGetGuildSupplyList.siVillageCode		=	siVillageCode;

	return Write(&TempOnReqGetGuildSupplyList, sizeof(OnReqGetGuildSupplyList));
}

BOOL	OnlineClient::SendIncreaseVillageDefenceSoldier(SI16 siDefenceSoldier, SI16 siVillageCode)
{
	OnReqVillageDefenceSoldier	TempOnReqVillageDefenceSoldier;

	TempOnReqVillageDefenceSoldier.dwMsgType				=	ON_REQUEST_INCREASE_VILLAGE_DEFENCE_SOLDIER;
	TempOnReqVillageDefenceSoldier.siDefenceSoldierCount	=	siDefenceSoldier;
	TempOnReqVillageDefenceSoldier.siVillageCode			=	siVillageCode;

	return Write(&TempOnReqVillageDefenceSoldier, sizeof(OnReqVillageDefenceSoldier));
}

// robypark 2004/11/19 13:34
// 공성전 마을 수비병 수 요청하기
BOOL	OnlineClient::SendVillageGarrisonCount(SI16 siVillageCode)
{
	OnReqGetVillageDefenceSoldier	TempOnReqGetVillageDefenceSoldier;

	TempOnReqGetVillageDefenceSoldier.dwMsgType		=	ON_REQUEST_GET_VILLAGE_DEFENCE_SOLDIER;
	TempOnReqGetVillageDefenceSoldier.siVillageCode	=	siVillageCode;

	return Write(&TempOnReqGetVillageDefenceSoldier, sizeof(OnReqGetVillageDefenceSoldier));
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명	:	Change Guild Flag
BOOL	OnlineClient::SendChangeGuildFlag(SI32 siGuildFlag)
{
	OnReqGuildChangeFlag		TempOnReqGuildChangeFlag;

	TempOnReqGuildChangeFlag.dwMsgType = ON_REQUEST_CHANGE_GUILD_FLAG;
	TempOnReqGuildChangeFlag.siFlag    = siGuildFlag;
	return Write(&TempOnReqGuildChangeFlag, sizeof(TempOnReqGuildChangeFlag));
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명	:	사용돼는 깃발 얻기
BOOL	OnlineClient::SendUsedFlag(void)
{
	OnReqUsedFlagList		TempOnReqUsedFlagList;

	TempOnReqUsedFlagList.dwMsgType = ON_REQUEST_USED_FLAG_LIST;
	return Write(&TempOnReqUsedFlagList, sizeof(TempOnReqUsedFlagList));
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명	:	마을의 수익 통계를 얻어온다.
BOOL	OnlineClient::SendRequestGovernment_GetVillageProfitStatistics(void)
{
	OnReqProfitStatistics		TempOnReqProfitStatistics;

	TempOnReqProfitStatistics.dwMsgType = ON_REQUEST_PROFIT_STATISTICS;
	return Write(&TempOnReqProfitStatistics, sizeof(TempOnReqProfitStatistics));
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명	:	투자금을 인출한다.
BOOL	OnlineClient::SendRequestGovernment_InvestmentGetOut(MONEY GetOutMoney)
{
	OnReqGovernmentInvestmentGetOut		TempOnReqGovernmentInvestmentGetOut;

	TempOnReqGovernmentInvestmentGetOut.dwMsgType   = ON_REQUEST_GOVERNMENT_INVESTMENT_GETOUT;
	TempOnReqGovernmentInvestmentGetOut.GetOutMoney = GetOutMoney;
	return Write(&TempOnReqGovernmentInvestmentGetOut, sizeof(OnReqGovernmentInvestmentGetOut));
}

BOOL	OnlineClient::SendRPIInMinimap(CHAR *pszName)
{
	OnReqPIInMiniMapMsg		ReqPIInMiniMapMsg;

	ZeroMemory(&ReqPIInMiniMapMsg, sizeof(ReqPIInMiniMapMsg));

	ReqPIInMiniMapMsg.dwMsgType	=	ON_REQUEST_PI_INMINIMAP;
	strncpy(ReqPIInMiniMapMsg.szName, pszName, sizeof(ReqPIInMiniMapMsg.szName));	

	return Write(&ReqPIInMiniMapMsg, sizeof(OnReqPIInMiniMapMsg));
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
// 마을 게시판에서 게시물 내용을 변경
BOOL	OnlineClient::SendRequestModifyNotic(DWORD dwNoticeNum, UI08 uiTitleLength, UI16 uiNoticeLength, const char* pTitle, const char* pNotice )
{
	OnReqModifyNoticeMsg		TempOnReqModifyNoticeMsg;
	CHAR						szTempBuffer[1024];
	SI16						siSize;

	TempOnReqModifyNoticeMsg.dwMsgType				=	ON_REQUEST_MODIFY_NOTICE;
	TempOnReqModifyNoticeMsg.dwNoticeNum			=	dwNoticeNum;
	TempOnReqModifyNoticeMsg.uiTitleStringLength	=	uiTitleLength;
	TempOnReqModifyNoticeMsg.uiNoticeStringLength	=	uiNoticeLength;


	siSize	=	sizeof( OnReqModifyNoticeMsg );

	memcpy( szTempBuffer, &TempOnReqModifyNoticeMsg, siSize );
	memcpy( &szTempBuffer[ siSize ], pTitle, uiTitleLength );

	siSize += uiTitleLength;
	memcpy( &szTempBuffer[ siSize ], pNotice, uiNoticeLength );	

	siSize += uiNoticeLength;

	return Write( szTempBuffer, siSize );
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
// NPC 선택
BOOL	OnlineClient::SendRequestQuestSelectNPC(SI16 siNPCCode, SI32 siBookCount)
{
	OnReqQuestSelectNPC		TempOnReqQuestSelectNPC;

	ZeroMemory(&TempOnReqQuestSelectNPC, sizeof(TempOnReqQuestSelectNPC));
	TempOnReqQuestSelectNPC.dwMsgType   = ON_REQUEST_QUEST_SELECTNPC;
	TempOnReqQuestSelectNPC.siNPCCode   = siNPCCode;
	TempOnReqQuestSelectNPC.siBookCount = siBookCount;

	return Write(&TempOnReqQuestSelectNPC, sizeof(OnReqQuestSelectNPC));
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
// Quest 수락
BOOL	OnlineClient::SendRequestQuestAcceptQuest(SI32 siQuestCode)
{
	OnReqQuestAcceptQuest		TempOnReqQuestAcceptQuest;

	ZeroMemory(&TempOnReqQuestAcceptQuest, sizeof(TempOnReqQuestAcceptQuest));
	TempOnReqQuestAcceptQuest.dwMsgType   = ON_REQUEST_QUEST_ACCEPTQUEST;
	TempOnReqQuestAcceptQuest.m_QuestCode = siQuestCode;

	return Write(&TempOnReqQuestAcceptQuest, sizeof(TempOnReqQuestAcceptQuest));
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
// 진행중인 Quest 취소
BOOL	OnlineClient::SendRequestQuestCancelQuest(SI32 siQuestCode)
{
	OnReqQuestCancelQuest		TempOnReqQuestCancelQuest;

	ZeroMemory(&TempOnReqQuestCancelQuest, sizeof(TempOnReqQuestCancelQuest));
	TempOnReqQuestCancelQuest.dwMsgType   = ON_REQUEST_QUEST_CANCELQUEST;
	TempOnReqQuestCancelQuest.m_QuestCode = siQuestCode;

	return Write(&TempOnReqQuestCancelQuest, sizeof(TempOnReqQuestCancelQuest));
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
// 진행중인 Quest List 요청
BOOL	OnlineClient::SendRequestQuestGetPlayQuestList(void)
{
	OnReqQuestGetPlayQuest		TempOnReqQuestGetPlayQuest;

	ZeroMemory(&TempOnReqQuestGetPlayQuest, sizeof(TempOnReqQuestGetPlayQuest));
	TempOnReqQuestGetPlayQuest.dwMsgType   = ON_REQUEST_QUEST_GETPLAYQUEST;

	return Write(&TempOnReqQuestGetPlayQuest, sizeof(TempOnReqQuestGetPlayQuest));
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
// 은행계좌 수를 얻어옴
BOOL	OnlineClient::SendRequestQuestGetBankBookCount(void)
{
	OnReqBankBankBookListCountMsg	TempOnReqBankBankBookListCountMsg;

	ZeroMemory(&TempOnReqBankBankBookListCountMsg, sizeof(OnReqBankBankBookListCountMsg));
	TempOnReqBankBankBookListCountMsg.dwMsgType   = ON_REQUEST_BANK_BANKBOOK_LISTCOUNT;

	return Write(&TempOnReqBankBankBookListCountMsg, sizeof(OnReqBankBankBookListCountMsg));
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
// 플레이어의 정보를 요청한다.
BOOL	OnlineClient::SendPlayerDetailInfo(UI16 uiID)
{
	OnReqCharDetailInfoMsg		TempOnReqCharDetailInfoMsg;

	TempOnReqCharDetailInfoMsg.dwMsgType = ON_REQUEST_CHAR_DETAILINFO;
	TempOnReqCharDetailInfoMsg.uiAccount = uiID;

	return Write(&TempOnReqCharDetailInfoMsg, sizeof(OnReqCharDetailInfoMsg));	
}

/*
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
// 스킬사용을 요청한다.
BOOL	OnlineClient::SendRequestUseSkill(SI16 siSkillKind, SI16 siWhatDo)
{
	SKILL::sRequestUse		TempsRequestUse;

	TempsRequestUse.skillKind	= siSkillKind;
	TempsRequestUse.whatDo		= siWhatDo;

	return Write(&TempsRequestUse, sizeof(SKILL::sRequestUse));	
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
// 스킬등급을 올린다.
BOOL	OnlineClient::SendRequestLevelUp(SI16 siSkillKind)
{
	SKILL::sRequestLevelUp		TempsRequestLevelUp;

	TempsRequestLevelUp.skillKind	= siSkillKind;

	return Write(&TempsRequestLevelUp, sizeof(SKILL::sRequestLevelUp));	
}
*/

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
// 지역탐사를 요청한다.
BOOL	OnlineClient::SendRequestProbeArea()
{
	BURIED::sRequestProbeArea		TempsRequestProbeArea;
	TempsRequestProbeArea.command	=	BURIED::REQUEST_PROBE_AREA;

	return Write(&TempsRequestProbeArea, sizeof(BURIED::sRequestProbeArea));	
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
// 마을정보를 요청한다.
BOOL	OnlineClient::SendRequestVillageInfo(SI16 siVillageSeries)
{
	OnReqVillageInfoMsg		TempOnReqVillageInfoMsg;

	ZeroMemory(&TempOnReqVillageInfoMsg, sizeof(OnReqVillageInfoMsg));
	TempOnReqVillageInfoMsg.dwMsgType     = ON_REQUEST_VILLAGE_INFO;
	TempOnReqVillageInfoMsg.uiVillageCode = (UI16)siVillageSeries;

	return Write(&TempOnReqVillageInfoMsg, sizeof(OnReqVillageInfoMsg));
}

BOOL	OnlineClient::SendRequestBankSave(MONEY SavingMoney)
{
	OnReqBankSaveMsg    savemsg;
	savemsg.dwMsgType = ON_REQUEST_BANK_SAVE;
	savemsg.moMoney   = SavingMoney;

	return Write(&savemsg, sizeof(savemsg));
}
																	
BOOL	OnlineClient::SendRequestBankWithdraw(MONEY DrawMoney)
{
	OnReqBankSaveMsg    drawmsg;
	drawmsg.dwMsgType = ON_REQUEST_BANK_WITHDRAW;
	drawmsg.moMoney   = DrawMoney;

	return Write(&drawmsg, sizeof(drawmsg));
}

BOOL	OnlineClient::SendDummyPacket(void *packet,int size)
{

	return TRUE;
	
//	return Write( packet, size );
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
// 구입한 아이템 리스트를 요청한다.(주막)
BOOL	OnlineClient::SendRequestInnItemList()
{
	OnReqItemListMsg		TempOnReqItemListMsg;

	ZeroMemory(&TempOnReqItemListMsg, sizeof(OnReqItemListMsg));
	TempOnReqItemListMsg.dwMsgType     = ON_REQUEST_ITEMLIST;

	return Write(&TempOnReqItemListMsg, sizeof(OnReqItemListMsg));
}
	
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
// 구입한 아이템을 인벤토리에 넣는다.(주막)
BOOL	OnlineClient::SendMoveInnItemToInven(UI08 uiFollowerID, UI16 uiItemID, UI16 uiQuantity)
{
	OnReqTakeOutItemMsg		TempOnReqTakeOutItemMsg;

	ZeroMemory(&TempOnReqTakeOutItemMsg, sizeof(OnReqTakeOutItemMsg));
	TempOnReqTakeOutItemMsg.dwMsgType		= ON_REQUEST_TAKEOUTITEM;
	TempOnReqTakeOutItemMsg.uiFollowerID    = uiFollowerID;
	TempOnReqTakeOutItemMsg.uiItemID		= uiItemID;
	TempOnReqTakeOutItemMsg.uiQuantity		= uiQuantity;

	return Write(&TempOnReqTakeOutItemMsg, sizeof(OnReqTakeOutItemMsg));
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
// 아이템을 주막 인벤토리에 넣는다.(주막)
BOOL	OnlineClient::SendMoveInvenItemToInn(UI08 uiFollowerID, UI16 uiItemID, UI16 uiQuantity)
{
	OnReqSaveItemMsg		TempOnReqSaveItemMsg;

	ZeroMemory(&TempOnReqSaveItemMsg, sizeof(OnReqSaveItemMsg));
	TempOnReqSaveItemMsg.dwMsgType			= ON_REQUEST_SAVEITEM;
	TempOnReqSaveItemMsg.uiFollowerID		= uiFollowerID;
	TempOnReqSaveItemMsg.uiItemID			= uiItemID;
	TempOnReqSaveItemMsg.uiQuantity			= uiQuantity;

	return Write(&TempOnReqSaveItemMsg, sizeof(OnReqSaveItemMsg));
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
// 상단정보를 요청한다.
BOOL	OnlineClient::SendRequestGuildInfo(CHAR *pszName)
{
	OnReqPersonalInfo		TempOnReqPersonalInfo;

	ZeroMemory(&TempOnReqPersonalInfo, sizeof(OnReqPersonalInfo));
	TempOnReqPersonalInfo.dwMsgType			= ON_REQUEST_PERSONAL_INFO;
	strncpy(TempOnReqPersonalInfo.szID, pszName, ON_GUILDNAME_LENGTH);

	return Write(&TempOnReqPersonalInfo, sizeof(OnReqPersonalInfo));
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
// 따라다니는 용병을 삭제한다. 
BOOL	OnlineClient::SendRequestDeleteVMercenary(void)
{
	OnRequest_VMercenary_Delete		TempOnRequest_VMercenary_Delete;

	ZeroMemory(&TempOnRequest_VMercenary_Delete, sizeof(OnRequest_VMercenary_Delete));
	TempOnRequest_VMercenary_Delete.dwMsgType       = ON_REQUEST_VMERCENARY_DELETE;

	return Write(&TempOnRequest_VMercenary_Delete, sizeof(OnRequest_VMercenary_Delete));
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
// 아이템 제련을 요청한다.
BOOL	OnlineClient::SendRequestRefineItem(UI16 uiFollowerSlot, UI16 uiItemID)
{
	OnRequestRefineItem		TempOnRequestRefineItem;

	ZeroMemory(&TempOnRequestRefineItem, sizeof(OnRequestRefineItem));
	TempOnRequestRefineItem.dwMsgType		= ON_REQUEST_REFINE_ITEM;
	TempOnRequestRefineItem.uiFollowerSlot	= uiFollowerSlot;
	TempOnRequestRefineItem.uiItemID		= uiItemID;

	return Write(&TempOnRequestRefineItem, sizeof(OnRequestRefineItem));
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
// 상단개설 비용을 요청한다.
BOOL	OnlineClient::SendRequestGuildCreateCost()
{
	OnRequestGuildCreateCost		TempOnRequestGuildCreateCost;

	ZeroMemory(&TempOnRequestGuildCreateCost, sizeof(OnRequestGuildCreateCost));
	TempOnRequestGuildCreateCost.dwMsgType		= ON_REQUEST_GUILD_CREATE_COST;

	return Write(&TempOnRequestGuildCreateCost, sizeof(OnRequestGuildCreateCost));
}



/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
/// 로그인관련 작업한거

//----------------------------------------------------------------------------------------------------------------
//	설명	:	클라이언트의 정보를 보낸다.
//----------------------------------------------------------------------------------------------------------------
BOOL	OnlineClient::SendClientKind()
{
	FRONT_USER::sRequestCheckVersion		ClientInfoMsg;

	ClientInfoMsg.command	=	FRONT_USER::REQUEST_CHECK_VERSION;
	ClientInfoMsg.version	=	ON_CLINETVERSION;	
	
	return	Write(&ClientInfoMsg, sizeof(FRONT_USER::sRequestCheckVersion));	
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명	:	알림사항을 요청한다.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
BOOL	OnlineClient::SendAdminMsg(void)
{
	FRONT_USER::sRequestNotice		ReqAdminMsg;

	ReqAdminMsg.command		=	FRONT_USER::REQUEST_NOTICE;

	return Write(&ReqAdminMsg, sizeof(FRONT_USER::sRequestNotice));
}

BOOL	OnlineClient::SendRequestWorldInfo(void)
{
	FRONT_USER::sRequestWorldsInfo		TempRequestWorldsInfo;

	TempRequestWorldsInfo.command	=	FRONT_USER::REQUEST_WORLDS_INFO;

	return Write(&TempRequestWorldsInfo, sizeof(FRONT_USER::sRequestWorldsInfo));	
}

//----------------------------------------------------------------------------------------------------------------
//	설명	:	선택한 서버 종류를 보낸다.
//----------------------------------------------------------------------------------------------------------------
BOOL	OnlineClient::SendWorldID(BYTE BWorldID)
{
	FRONT_USER::sRequestSelectWorld		SelectWorldMsg;

	SelectWorldMsg.command	=	FRONT_USER::REQUEST_SELECT_WORLD;
	SelectWorldMsg.world_id	=	BWorldID;
	
	return	Write(&SelectWorldMsg, sizeof(FRONT_USER::sRequestSelectWorld));	
}

//----------------------------------------------------------------------------------------------------------------
//	설명	:	로그인의 정보를 보낸다.
//----------------------------------------------------------------------------------------------------------------
BOOL	OnlineClient::SendAccountLogIn(CHAR *pszID, CHAR *pszPW)
{
	LOGIN::sRequestLogin	TempRequestLoginMsg;


//	FILE *fp = fopen( "login.txt", "a+" );
//	fprintf( fp, "ID[%s] PW[%s]\n", pszID, pszPW );
//	fclose( fp );


	ZeroMemory(&TempRequestLoginMsg, sizeof(LOGIN::sRequestLogin));
	TempRequestLoginMsg.command		=	LOGIN::REQUEST_LOGIN;
	strncpy(TempRequestLoginMsg.id, pszID, LOGIN::ID_MAX_LENGTH);
	strncpy(TempRequestLoginMsg.password, pszPW, LOGIN::PW_MAX_LENGTH);

	return	Write(&TempRequestLoginMsg, sizeof(LOGIN::sRequestLogin), TRUE);	
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
// 캐릭터 정보를 요청한다.
BOOL	OnlineClient::SendRequestCharInfo()
{
	LOGIN::sRequestCharsInfo		TempRequestCharsInfoMsg;

	TempRequestCharsInfoMsg.command		=	LOGIN::REQUEST_CHARS_INFO;

	return Write(&TempRequestCharsInfoMsg, sizeof(LOGIN::sRequestCharsInfo), TRUE);	
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명	:	캐릭터를 삭제한다.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
BOOL	OnlineClient::SendDeleteCharacter(UI08 uiSlot)
{
	LOGIN::sRequestDelChar		TempRequestDelChar;

	TempRequestDelChar.command	=	LOGIN::REQUEST_DEL_CAHR;
	TempRequestDelChar.delSlot	=	uiSlot;

	return Write(&TempRequestDelChar, sizeof(LOGIN::sRequestDelChar), TRUE);
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명	:	캐릭터를 생성한다고 서버에 알려준다.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
BOOL	OnlineClient::SendCreateNewCharacter(CHAR *pszID, SI32 uiKind, UI08 uiStr, UI08 uiDex, UI08 uiVit, UI08 uiInt)
{
	LOGIN::sRequestNewChar		TempReqNewCharMsg;

	ZeroMemory( &TempReqNewCharMsg, sizeof( LOGIN::sRequestNewChar ) );

	TempReqNewCharMsg.command	=	LOGIN::REQUEST_NEW_CHAR;
	TempReqNewCharMsg.kind		=	uiKind;
	TempReqNewCharMsg.Str		=	(SI16)uiStr;
	TempReqNewCharMsg.Dex		=	(SI16)uiDex;
	TempReqNewCharMsg.Vit		=	(SI16)uiVit;
	TempReqNewCharMsg.Int		=	(SI16)uiInt;
	strncpy( TempReqNewCharMsg.name, pszID, ON_ID_LENGTH );

	return Write(&TempReqNewCharMsg, sizeof(LOGIN::sRequestNewChar), TRUE);
}

//----------------------------------------------------------------------------------------------------------------
//	설명	:	게임서버에 로그인한다.
//----------------------------------------------------------------------------------------------------------------
BOOL	OnlineClient::SendGameServerLogIn()
{
	OnReqGameServerLogInMsg		GameServerLogInMsg;
	memset(&GameServerLogInMsg, 0 ,sizeof(OnReqGameServerLogInMsg));

	GameServerLogInMsg.dwMsgType	=	ON_REQUEST_LOGIN;
//	pMyOnlineWorld->pMyData->GetMyAccountGUID(GameServerLogInMsg.Guid);	
	GameServerLogInMsg.uiCharSlot	=	UI08(pMyOnlineWorld->pMyData->GetSelectMyCharacterSlot());

	return	Write(&GameServerLogInMsg, sizeof(OnReqGameServerLogInMsg), TRUE);	
}

//----------------------------------------------------------------------------------------------------------------
//	설명	:	대행수, 행동대장 등의 직급을 임명한다.
//----------------------------------------------------------------------------------------------------------------
BOOL OnlineClient::SendRequestGovernment_AppointClass(char* lpID, SI32 ClassCode)
{
	OnRequestGuildPromotion		TempOnRequestGuildPromotion;

	ZeroMemory(&TempOnRequestGuildPromotion, sizeof(OnRequestGuildPromotion));
	TempOnRequestGuildPromotion.dwMsgType	=	ON_REQUEST_GUILD_PROMOTION;
	strncpy((char*)TempOnRequestGuildPromotion.szCharID, lpID, ON_ID_LENGTH);
	TempOnRequestGuildPromotion.siClassCode	=	ClassCode;

	return Write(&TempOnRequestGuildPromotion, sizeof(OnRequestGuildPromotion));
}

//----------------------------------------------------------------------------------------------------------------
//	설명	:	대행수, 행동대장 등의 직급에서 해임한다.
//----------------------------------------------------------------------------------------------------------------
BOOL OnlineClient::SendRequestGovernment_DismissClass(char* lpID)
{
	OnRequestGuildDemotion		TempOnRequestGuildDemotion;

	ZeroMemory(&TempOnRequestGuildDemotion, sizeof(OnRequestGuildDemotion));
	TempOnRequestGuildDemotion.dwMsgType	=	ON_REQUEST_GUILD_DEMOTION;
	strncpy((char*)TempOnRequestGuildDemotion.szCharID, lpID, ON_ID_LENGTH);

	return Write(&TempOnRequestGuildDemotion, sizeof(OnRequestGuildDemotion));
}

//----------------------------------------------------------------------------------------------------------------
//	설명	:	로그아웃을 서버에 알린다.
//----------------------------------------------------------------------------------------------------------------
BOOL	OnlineClient::SendLogOut()
{
	OnReqGameServerLogOutMsg	TempOnReqGameServerLogOutMsg;
	ZeroMemory(&TempOnReqGameServerLogOutMsg, sizeof(OnReqGameServerLogOutMsg));
	TempOnReqGameServerLogOutMsg.dwMsgType = ON_REQUEST_LOGOUT;

	return Write(&TempOnReqGameServerLogOutMsg, sizeof(OnReqGameServerLogOutMsg));
}

//----------------------------------------------------------------------------------------------------------------
//	설명	:	친구리스트 요청
//----------------------------------------------------------------------------------------------------------------
BOOL	OnlineClient::SendReqFriendList()
{
	OnReqAuditFriendList	TempOnReqAuditFriendList;
	ZeroMemory(&TempOnReqAuditFriendList, sizeof(OnReqAuditFriendList));
	TempOnReqAuditFriendList.dwMsgType = ON_REQUEST_AUDIT_FRIEND_LIST;

	return Write(&TempOnReqAuditFriendList, sizeof(OnReqAuditFriendList));
}

//----------------------------------------------------------------------------------------------------------------
//	설명	:	친구 추가
//----------------------------------------------------------------------------------------------------------------
BOOL	OnlineClient::SendReqAddFriend(char* lpID)
{
	OnReqAuditFriendAddOne	TempOnReqAuditFriendAddOne;
	ZeroMemory(&TempOnReqAuditFriendAddOne, sizeof(OnReqAuditFriendAddOne));
	TempOnReqAuditFriendAddOne.dwMsgType = ON_REQUEST_AUDIT_FRIEND_ADD_ONE;
	memcpy(TempOnReqAuditFriendAddOne.szCharID, lpID, ON_ID_LENGTH);

	return Write(&TempOnReqAuditFriendAddOne, sizeof(OnReqAuditFriendAddOne));
}

//----------------------------------------------------------------------------------------------------------------
//	설명	:	친구 삭제
//----------------------------------------------------------------------------------------------------------------
BOOL	OnlineClient::SendReqDeleteFriend(char* lpID, UI16 FriendNum)
{
	CHAR szTempBuffer[1024];
	SI16 siSize, uiLength;
	OnReqAuditFriendRemoveSome	TempOnReqAuditFriendRemoveSome;
	ZeroMemory(&TempOnReqAuditFriendRemoveSome, sizeof(OnReqAuditFriendRemoveSome));

	TempOnReqAuditFriendRemoveSome.dwMsgType = ON_REQUEST_AUDIT_FRIEND_REMOVE_SOME;
	TempOnReqAuditFriendRemoveSome.uiCount	= FriendNum;

	siSize = sizeof( OnReqAuditFriendRemoveSome );
	ZeroMemory( szTempBuffer, 1024 );
	memcpy( szTempBuffer, &TempOnReqAuditFriendRemoveSome, siSize );
	uiLength = ON_ID_LENGTH * FriendNum;
	memcpy( &szTempBuffer[siSize], lpID, uiLength );
	siSize += uiLength;

	return Write( szTempBuffer, siSize );
}

//----------------------------------------------------------------------------------------------------------------
//	설명	:	모든 친구 삭제
//----------------------------------------------------------------------------------------------------------------
BOOL	OnlineClient::SendReqDeleteAllFriend()
{
	OnReqAuditFriendRemoveAll	TempOnReqAuditFriendRemoveAll;
	ZeroMemory(&TempOnReqAuditFriendRemoveAll, sizeof(OnReqAuditFriendRemoveAll));
	TempOnReqAuditFriendRemoveAll.dwMsgType = ON_REQUEST_AUDIT_FRIEND_REMOVE_ALL;

	return Write(&TempOnReqAuditFriendRemoveAll, sizeof(OnReqAuditFriendRemoveAll));
}

//----------------------------------------------------------------------------------------------------------------
//	설명	:	수신거부 설정
//----------------------------------------------------------------------------------------------------------------
BOOL	OnlineClient::SendReqSetDenialFriend(char* lpID, BOOL denial)
{
	OnReqAuditFriendSetDenial	TempOnReqAuditFriendSetDenial;
	ZeroMemory(&TempOnReqAuditFriendSetDenial, sizeof(OnReqAuditFriendSetDenial));

	TempOnReqAuditFriendSetDenial.dwMsgType = ON_REQUEST_AUDIT_FRIEND_SET_DENIAL;
	strncpy((char*)TempOnReqAuditFriendSetDenial.szCharID, lpID, ON_ID_LENGTH);
	TempOnReqAuditFriendSetDenial.bDenial = denial;

	return Write(&TempOnReqAuditFriendSetDenial, sizeof(OnReqAuditFriendSetDenial));
}

BOOL	OnlineClient::SendAttackMode(BOOL b)
{
	OnReqSetAttackMode	tempReqSetAttckMode;

	tempReqSetAttckMode.dwMsgType	= ON_REQUEST_SET_ATTACK_MODE;
	tempReqSetAttckMode.bAttackMode	= b;

	return Write(&tempReqSetAttckMode, sizeof(OnReqSetAttackMode));
}

// robypark 2004/11/9 16:53
// 공성전 유닛 보급 수레에게 보급품을 요청한다.
// UI16 uiUniqueID_SupplyWagon:	공성전 보급수레 유닛의 Unique ID
// SI16 ssNeedsSupplyGoodsAmount: 필요한 보급품 양
BOOL	OnlineClient::SendSiegeWarfareSupplyGoodsFromSupplyWagon(UI16 uiUniqueID_SupplyWagon, SI16 ssNeedsSupplyGoodsAmount)
{
	OnReqGetSupplyAtGuildUnit	SDataOnReqGetSupplyAtGuildUnit;

	ZeroMemory(&SDataOnReqGetSupplyAtGuildUnit, sizeof(OnReqGetSupplyAtGuildUnit));

	SDataOnReqGetSupplyAtGuildUnit.dwMsgType		= ON_REQUEST_GET_SUPPLY_AT_GUILD_UNIT;
	SDataOnReqGetSupplyAtGuildUnit.siRequireSupply	= ssNeedsSupplyGoodsAmount;				// 필요한 량
	SDataOnReqGetSupplyAtGuildUnit.uiTargetID		= uiUniqueID_SupplyWagon;				// 보급수레 UniqueID

	return Write(&SDataOnReqGetSupplyAtGuildUnit, sizeof(SDataOnReqGetSupplyAtGuildUnit));
}

// robypark 2004/12/2 10:48
// 공성전 종료 후에 점령한 마을 리스트를 요청한다.
BOOL	OnlineClient::SendRequest_guild_villageattack_result(void)
{
	OnReqVillageAttackResult		TempOnReqVillageAttackResult;

	ZeroMemory(&TempOnReqVillageAttackResult, sizeof(OnReqVillageAttackResult));

	TempOnReqVillageAttackResult.dwMsgType		= ON_REQUEST_GUILD_VILLAGEATTACK_RESULT;

	return Write(&TempOnReqVillageAttackResult, sizeof(TempOnReqVillageAttackResult));
}

// robypark 2004/12/2 13:41
// 공성전 종료 후에 마을에 대한 폭투 권한을 특정 캐릭터에 주기 요청
BOOL	OnlineClient::SendRequest_guild_choice_player(UI16 uiVillageCode, char *pszCharname)
{
	OnReqGuildChoicePlayer			TempOnReqGuildChoicePlayer;

	ZeroMemory(&TempOnReqGuildChoicePlayer, sizeof(OnReqGuildChoicePlayer));

	TempOnReqGuildChoicePlayer.dwMsgType		=	ON_REQUEST_GUILD_CHOICE_PLAYER;
	TempOnReqGuildChoicePlayer.siVillageCode	=	uiVillageCode;
	strncpy(TempOnReqGuildChoicePlayer.szUserName, pszCharname, ON_ID_LENGTH);

	return Write(&TempOnReqGuildChoicePlayer, sizeof(TempOnReqGuildChoicePlayer));
}

// robypark 2004/12/2 14:26
// 사용자가 들어가 있는 마을에 폭탄 투자 권한이 있는지 요청한다.
BOOL	OnlineClient::SendRequest_village_investment_power(UI16 uiVillageCode, char *pszCharname)
{
	OnReqInvestmentPower			TempOnReqInvestmentPower;

	ZeroMemory(&TempOnReqInvestmentPower, sizeof(OnReqInvestmentPower));

	TempOnReqInvestmentPower.dwMsgType		=	ON_REQUEST_VILLAGE_INVESTMENT_POWER;
	TempOnReqInvestmentPower.siVillageCode	=	uiVillageCode;
	strncpy(TempOnReqInvestmentPower.szUserName, pszCharname, ON_ID_LENGTH);

	return Write(&TempOnReqInvestmentPower, sizeof(TempOnReqInvestmentPower));
}

// robypark 2004/12/21 16:30
// 공성전 중에 일반 유닛 각 국가의 특정 위치(초보자 수련장)로 워프 요청하기
// 이 요청 응답은 기존의 운영자 명령어인 /w(워프)의 것을 사용한다.(ON_RESPONSE_WARP)
BOOL	OnlineClient::SendRequestSiegeWarfareWarp(UI16 uiUniqueID)
{
	OnReqWarWarp					TempOnReqWarWarp;

	TempOnReqWarWarp.dwMsgType		=	ON_REQUEST_WAR_WARP;
	TempOnReqWarWarp.uiUniqueID		=	uiUniqueID;				// 워프해서 보내는 일반유닛 아이디.

	return Write(&TempOnReqWarWarp, sizeof(TempOnReqWarWarp));
}

// robypark 2005/1/5 17:4
// 공성전 유닛인 상태에서 일반 유닛으로 돌아가겠다고 서버에 요청
BOOL	OnlineClient::SendRequestNormalCharacter(UI16 uiUniqueID)
{
	OnReqNormalChar					TempOnReqNormalChar;

	TempOnReqNormalChar.dwMsgType	=	ON_REQUEST_NORMAL_CHAR;
	TempOnReqNormalChar.uiUniqueID	=	uiUniqueID;

	return Write(&TempOnReqNormalChar, sizeof(TempOnReqNormalChar));
}