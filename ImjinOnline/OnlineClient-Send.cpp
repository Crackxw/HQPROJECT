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
#include "OnlineSMS.h"		// actdoll (2004/06/15 10:46) : SMS �� ������ �߰�
#include "OnlinePannel.h"


//----------------------------------------------------------------------------------------------------------------
//	 ����	:	�� �÷��̾��� ��ġ�� ������.
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
//	 ����	:	�� �÷��̾��� ��ġ�� ������.
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
//	����	:	���� �̵��ؾ� �ϴ� ������ �����ش�.
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
//	����	:	���� �̵��ؾ� �ϴ� ��ǥ�� �����ش�.
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
//	����	:	
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
//	����	:	���� Ÿ������ �޽����� ������.
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
//	����	:	���� ������ ���ٴ� �޽����� �������� �˷��ش�.
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
BOOL	OnlineClient::SendGotoVillage(UI16 uiGotoVillageCode)
{
	OnReqGotoVillageMsg		GotoVillageMsg;

	GotoVillageMsg.dwMsgType		=	ON_REQUEST_ENTER_VILLAGE;
	GotoVillageMsg.uiVillageCode	=	uiGotoVillageCode;	
	
	return Write(&GotoVillageMsg, sizeof(OnReqGotoVillageMsg));	
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
//	����	:	������ ������.
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
BOOL	OnlineClient::SendGoOutVillage()
{
	OnReqGoOutVillageMsg		ReqGoOutVillageMsg;

	ReqGoOutVillageMsg.dwMsgType	=	ON_REQUEST_LEAVE_VILLAGE;	

	return Write(&ReqGoOutVillageMsg, sizeof(OnReqGoOutVillageMsg));
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
//	����	:	�������� ����Ʈ�� �޶�� �䱸�Ѵ�.
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
BOOL	OnlineClient::SendRequestItemList()
{
	OnReqItemListInMarketMsg	ReqItemListInMarketMsg;

	ReqItemListInMarketMsg.dwMsgType	=	ON_REQUEST_LIST_ITEM;	

	return Write(&ReqItemListInMarketMsg, sizeof(OnReqItemListInMarketMsg));
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
//	����	:	�������� ��ٰ� ������ ��û�Ѵ�.
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
BOOL	OnlineClient::SendBuyItem(OnReqBuyItemMsg* lpBuyItem)
{
	return Write(lpBuyItem, sizeof(OnReqBuyItemMsg));	
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
//	����	:	�������� �Ǵٰ� ������ ��û�Ѵ�.
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
//	����	:	�ٸ� �÷��̾�� ������ �Ѵٰ� �������� �˷��ش�.
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
BOOL	OnlineClient::SendBattle(SI32 siID)
{
	OnReqBattleMsg		BattleMsg;

	BattleMsg.dwMsgType	=	ON_REQUEST_BATTLE;
	BattleMsg.uiCharID	=	UI16(siID);

	return Write(&BattleMsg, sizeof(BattleMsg));	
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	�κ��丮�������� �� �������� ��ġ�� �������� �˷��ش�.
//��������������������������������������������������������������������������������������������������������������������������������������������������������������
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


//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	�������� ������.
//��������������������������������������������������������������������������������������������������������������������������������������������������������������
BOOL	OnlineClient::SendThrowItem(OnReqThrowItemMsg* lpOnReqThrowItemMsg)
{
	return Write(lpOnReqThrowItemMsg, sizeof(OnReqThrowItemMsg));	
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	���� ������ �ִ� �������� �־���.
//��������������������������������������������������������������������������������������������������������������������������������������������������������������
BOOL	OnlineClient::SendPickUpItem(OnReqPickUpItemMsg* lpOnlineTempOnReqPickUpItemMsg)
{
	return Write(lpOnlineTempOnReqPickUpItemMsg, sizeof(OnReqPickUpItemMsg));	
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	�ٽ� �¶������� ���ư����� �Ѵ�.
//��������������������������������������������������������������������������������������������������������������������������������������������������������������
BOOL	OnlineClient::SendReturnOnline(BOOL bWin)
{
	OnReqReturnOnlineMsg	ReturnOnlineMsg;	

//	DWriteStringToFile("SendReturnOnline() �޼��� ����");
	ReturnOnlineMsg.dwMsgType	=	ON_REQUEST_RETURNONLINE;
	ReturnOnlineMsg.dwBattleResult = bWin;

	return Write(&ReturnOnlineMsg, sizeof(OnReqReturnOnlineMsg));
}



//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	ĳ������ ������ �޶�� �����Ѵ�.
//��������������������������������������������������������������������������������������������������������������������������������������������������������������
BOOL	OnlineClient::SendRequestPlayerInfo(UI16 uiCharNum, UI16 *puiIDIn)
{
	CHAR						cLocalBuffer[ON_MAX_LOCALBUFFER];
	OnReqPlayerInfoMsg			*pReqPlayerInfoMsg;
	UI16						*puiIDOut;
	SI32						siLength;

//	SetMessage("ĳ���� ���� ��û");

	pReqPlayerInfoMsg				=	(OnReqPlayerInfoMsg*)cLocalBuffer;

	pReqPlayerInfoMsg->dwMsgType		=	ON_REQUEST_PLAYER_INFO;
	pReqPlayerInfoMsg->uiCharNum		=	uiCharNum;
	puiIDOut							=	(UI16*)((CHAR*)pReqPlayerInfoMsg + sizeof(OnReqPlayerInfoMsg));
	siLength							=	sizeof(OnReqPlayerInfoMsg) + (sizeof(UI16) * pReqPlayerInfoMsg->uiCharNum);
	
	memcpy(puiIDOut, puiIDIn, sizeof(UI16) * pReqPlayerInfoMsg->uiCharNum);

	return Write(pReqPlayerInfoMsg, siLength);
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	����, ���忡 ����.
//��������������������������������������������������������������������������������������������������������������������������������������������������������������
BOOL	OnlineClient::SendRequestEnterMarket()
{
	OnReqEnterMarketMsg	EnterMarketMsg;

	EnterMarketMsg.dwMsgType	=	ON_REQUEST_ENTER_MARKET;
	
	return Write(&EnterMarketMsg, sizeof(EnterMarketMsg));
}
	
//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	����, ���忡�� ������.
//��������������������������������������������������������������������������������������������������������������������������������������������������������������
BOOL	OnlineClient::SendRequestLeaveMarket()
{	
	OnReqLeaveMarketMsg	LeaveMarketMsg;

	LeaveMarketMsg.dwMsgType	=	ON_REQUEST_LEAVE_MARKET;

	return Write(&LeaveMarketMsg, sizeof(LeaveMarketMsg));
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	���� ������ ��û
//��������������������������������������������������������������������������������������������������������������������������������������������������������������
BOOL	OnlineClient::SendRequestBankInfo()
{
	OnReqBankInfoMsg	TempOnReqBankInfoMsg;

	TempOnReqBankInfoMsg.dwMsgType	=	ON_REQUEST_BANK_INFO;	

	return Write( &TempOnReqBankInfoMsg, sizeof( OnReqBankInfoMsg ) );

}
//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	���� ���� ��û
//��������������������������������������������������������������������������������������������������������������������������������������������������������������
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

//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	���� ����Ʈ ��û
//��������������������������������������������������������������������������������������������������������������������������������������������������������������
BOOL	OnlineClient::SendRequestMySavingAccount(void)
{	
	OnReqBankBankBookListMsg			TempOnReqBankBankBookList;

	TempOnReqBankBankBookList.dwMsgType = ON_REQUEST_BANK_BANKBOOK_LIST;

	return Write(&TempOnReqBankBankBookList, sizeof(OnReqBankBankBookListMsg));
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	�Ա�
//��������������������������������������������������������������������������������������������������������������������������������������������������������������
BOOL	OnlineClient::SendRequestSavingAccount(OnAccountNumber AccountNumber, MONEY Money)
{
	OnReqBankDepositMsg		TempOnReqBankDepositMsg;

	TempOnReqBankDepositMsg.dwMsgType     = ON_REQUEST_BANK_DEPOSIT;
	TempOnReqBankDepositMsg.AccountNumber = AccountNumber;
	TempOnReqBankDepositMsg.siMoney       = Money;

	return Write(&TempOnReqBankDepositMsg, sizeof(OnReqBankDepositMsg));
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	���
//��������������������������������������������������������������������������������������������������������������������������������������������������������������
BOOL	OnlineClient::SendRequestDrawingAccount(OnAccountNumber AccountNumber, MONEY Money)
{
	OnReqBankDepositMsg		TempOnReqBankDepositMsg;

	TempOnReqBankDepositMsg.dwMsgType     = ON_REQUEST_BANK_DRAWING;
	TempOnReqBankDepositMsg.AccountNumber = AccountNumber;
	TempOnReqBankDepositMsg.siMoney       = Money;

	return Write(&TempOnReqBankDepositMsg, sizeof(OnReqBankDepositMsg));
}
	
//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	����, ���忡�� �㺸����
//��������������������������������������������������������������������������������������������������������������������������������������������������������������
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

//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	����, ���忡�� �㺸�� ����Ʈ�� �޴´�.
//��������������������������������������������������������������������������������������������������������������������������������������������������������������
BOOL	OnlineClient::SendCollateralListMsg()
{
	OnReqCollateralListMsg		TempOnReqCollateralListMsg;

	TempOnReqCollateralListMsg.dwMsgType	=	ON_REQUEST_BANK_COLLATERALLIST;

	return Write( &TempOnReqCollateralListMsg, sizeof( OnReqCollateralListMsg ) );
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	���ڱ� ����
//��������������������������������������������������������������������������������������������������������������������������������������������������������������
BOOL OnlineClient::SendReqAuditVillageInfo()
{
	OnReqAuditVillageList		TempReqAuditVillageList;

	TempReqAuditVillageList.dwMsgType = ON_REQUEST_AUDIT_VILLAGE_LIST;

	return Write(  &TempReqAuditVillageList,sizeof(OnReqAuditVillageList));
}
//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	����, ���忡�� �㺸�� ã�´�.
//��������������������������������������������������������������������������������������������������������������������������������������������������������������
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

//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	Message ����
//��������������������������������������������������������������������������������������������������������������������������������������������������������������
BOOL	OnlineClient::SendMessage(char* lpMsg, SI16 Size)
{
	return Write(lpMsg, Size);
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	����, ������ ����.
//��������������������������������������������������������������������������������������������������������������������������������������������������������������
BOOL	OnlineClient::SendRequestEnterInn()
{
	OnReqEnterInnMsg	TempOnReqEnterInnMsg;

	TempOnReqEnterInnMsg.dwMsgType		= 	ON_REQUEST_ENTER_INN;

	return Write(&TempOnReqEnterInnMsg, sizeof(OnReqEnterInnMsg));
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	����, �������� ������.
//��������������������������������������������������������������������������������������������������������������������������������������������������������������
BOOL	OnlineClient::SendRequestLeaveInn()
{
	ReqLeaveInnMsg		TempReqLeaveInnMsg;

	TempReqLeaveInnMsg.dwMsgType		=	ON_REQUEST_LEAVE_INN;

	return Write(&TempReqLeaveInnMsg, sizeof(ReqLeaveInnMsg));
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	����, ������ ��ȭ�Ƿ� ����.
//��������������������������������������������������������������������������������������������������������������������������������������������������������������
BOOL	OnlineClient::SendRequestEnterInnVWR()
{
	OnReqEnterWaitRoomMsg	TempOnReqEnterWaitRoomMsg;

	TempOnReqEnterWaitRoomMsg.dwMsgType		= 	ON_REQUEST_ENTER_VWR;

	return Write(&TempOnReqEnterWaitRoomMsg, sizeof(OnReqEnterWaitRoomMsg));
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	����, ������ ��ȭ�ǿ��� ��ȭ�濡 �����Ѵ�.
//��������������������������������������������������������������������������������������������������������������������������������������������������������������
BOOL	OnlineClient::SendRequestJoinVilInnChatRoom(UI16 ChattingRoomID)
{
	OnReqJoinVilInnChatRoomMsg	ReqJoinVilInnChatRoomMsg;

	ReqJoinVilInnChatRoomMsg.dwMsgType	  = ON_REQUEST_VIL_INN_JOIN_CHAT_ROOM;
	ReqJoinVilInnChatRoomMsg.uiChatRoomID = ChattingRoomID;

	return Write(&ReqJoinVilInnChatRoomMsg, sizeof(OnReqJoinVilInnChatRoomMsg));
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	����, ������ ��ȭ�ǿ��� ��ȭ�濡�� ������.
//��������������������������������������������������������������������������������������������������������������������������������������������������������������
BOOL	OnlineClient::SendRequestLeaveVilInnChatRoom()
{
	OnReqLeaveVilInnChatRoomMsg	ReqLeaveVilInnChatRoomMsg;

	ReqLeaveVilInnChatRoomMsg.dwMsgType	=	ON_REQUEST_VIL_INN_LEAVE_CHAT_ROOM;

	return Write(&ReqLeaveVilInnChatRoomMsg, sizeof(ReqLeaveVilInnChatRoomMsg));
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	����, ������ ��ȭ�ǿ��� ä�ù� ����
//��������������������������������������������������������������������������������������������������������������������������������������������������������������
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

//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	����, ������ �Խ��ǿ� ��
//��������������������������������������������������������������������������������������������������������������������������������������������������������������
BOOL	OnlineClient::SendRequestEnterInnNoticeBoard(DWORD Kind)
{
	OnReqEnterNoticeBoardMsg	TempOnReqEnterNoticeBoardMsg;

	TempOnReqEnterNoticeBoardMsg.dwMsgType			=	ON_REQUEST_ENTER_NOTICEBOARD;
	
	return Write(&TempOnReqEnterNoticeBoardMsg, sizeof(OnReqEnterNoticeBoardMsg));
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	����ǿ��� ���� �����.
//��������������������������������������������������������������������������������������������������������������������������������������������������������������
BOOL	OnlineClient::SendRequestDeleteNotice( DWORD dwPage, UI16 uiIndex )
{
	OnReqDeleteNoticeMsg		TempOnReqDeleteNoticeMsg;

	TempOnReqDeleteNoticeMsg.dwMsgType		=	ON_REQUEST_DELETE_NOTICE;
	TempOnReqDeleteNoticeMsg.dwPageNum		=	dwPage;
	TempOnReqDeleteNoticeMsg.uiNoticeIndex	=	uiIndex;

	return Write( &TempOnReqDeleteNoticeMsg, sizeof( OnReqDeleteNoticeMsg ) );	
}


//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	�Խ��ǿ����� �˻�
//��������������������������������������������������������������������������������������������������������������������������������������������������������������
BOOL	OnlineClient::SendRequestSearchNotice( UI08	uiType, char *pMsg )
{
/*
	UI08						TempBuffer[1024];
	OnReqSearchNoticeMsg		TempOnReqSearchNoticeMsg;

	TempOnReqSearchNoticeMsg.dwMsgType		=	ON_REQUEST_SEARCH_NOTICE;
	TempOnReqSearchNoticeMsg.uiSearchKind	=	uiType;
	TempOnReqSearchNoticeMsg.uiStringLength	=	strlen( pMsg );


	memcpy( TempBuffer, &TempOnReqSearchNoticeMsg, sizeof( 	OnReqSearchNoticeMsg ) );			 // ���� ����ü��
	memcpy( &TempBuffer[ sizeof( 	OnReqSearchNoticeMsg ) ], pMsg, strlen( pMsg ) );			 // ������ ������ �ٿ��� �����ش�.

	return Write( TempBuffer, ( sizeof( OnReqSearchNoticeMsg ) + strlen( pMsg ) ) );
*/
	return FALSE;
}



//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	�Խ��ǿ� ���� ��û �Ѵ�.
//��������������������������������������������������������������������������������������������������������������������������������������������������������������
BOOL	OnlineClient::SendRequestNoticesListMsg( UI16 uiPageID )
{
	OnReqNoticesListMsg			TempOnReqNoticesListMsg;

	TempOnReqNoticesListMsg.dwMsgType	=	ON_REQUEST_NOTICEBOARD_NOTICELIST;
	TempOnReqNoticesListMsg.uiPage		=	uiPageID;

	return Write( &TempOnReqNoticesListMsg, sizeof( OnReqNoticesListMsg ) );
}


//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	�Խ����� �д´�.
//��������������������������������������������������������������������������������������������������������������������������������������������������������������
BOOL	OnlineClient::SendRequestViewNotice( UI16	uiIndex )
{
	OnReqViewNoticeMsg		TempOnReqViewNoticeMsg;

	TempOnReqViewNoticeMsg.dwMsgType		=	ON_REQUEST_VIEW_NOTICE;
	TempOnReqViewNoticeMsg.uiNoticeIndex	=	uiIndex;

	return Write( &TempOnReqViewNoticeMsg, sizeof( OnReqViewNoticeMsg ) );
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	���� Portal�� ���ٴ� �޽����� �������� �˷��ش�.
//��������������������������������������������������������������������������������������������������������������������������������������������������������������
BOOL	OnlineClient::SendGotoPortal(SI16 siID)
{
	OnReqMoveMapMsg				TempOnReqMoveMapMsg;

	TempOnReqMoveMapMsg.dwMsgType  = ON_REQUEST_MOVEMAP;
	TempOnReqMoveMapMsg.uiPortalID = siID;

	return Write(&TempOnReqMoveMapMsg, sizeof(OnReqMoveMapMsg));
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	�Ʒüҿ� ����.
BOOL	OnlineClient::SendRequestBarrackEnter(void)
{
	OnReqBarrackGetList		TempOnReqBarrackGetList;

	TempOnReqBarrackGetList.dwMsgType = ON_REQUEST_BARRACK_GETSOLDERLIST;

	return Write(&TempOnReqBarrackGetList, sizeof(OnReqBarrackGetList));
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	�Ʒüҿ��� �뺴�� ����Ѵ�.
BOOL	OnlineClient::SendRequestBarrackBuySolder(SI32 Code, BYTE* lpName)
{
	OnReqBarrackBuySolder	TempOnReqBarrackBuySolder;

	ZeroMemory(&TempOnReqBarrackBuySolder, sizeof(OnReqBarrackBuySolder));
	TempOnReqBarrackBuySolder.dwMsgType     = ON_REQUEST_BARRACK_BUYSOLDER;
	TempOnReqBarrackBuySolder.dwBuySolderID = Code;
	strcpy((char*)TempOnReqBarrackBuySolder.szName, (char*)lpName);

	return Write(&TempOnReqBarrackBuySolder, sizeof(OnReqBarrackBuySolder));
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	�Ʒü� �ذ� ����.
BOOL	OnlineClient::SendRequestBarrackDismissalEnter(void)
{
	OnReqBarrackGetDismissalList		TempOnReqBarrackGetDismissalList;

	TempOnReqBarrackGetDismissalList.dwMsgType = ON_REQUEST_BARRACK_GETDISMISSALSOLDERLIST;

	return Write(&TempOnReqBarrackGetDismissalList, sizeof(OnReqBarrackGetDismissalList));
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	�Ʒüҿ��� �뺴�� �ذ��Ѵ�.
BOOL	OnlineClient::SendRequestBarrackDismissalSolder(SI32 Slot)
{
	OnReqBarrackDismissalSolder	TempOnReqBarrackDismissalSolder;

	ZeroMemory(&TempOnReqBarrackDismissalSolder, sizeof(OnReqBarrackDismissalSolder));
	TempOnReqBarrackDismissalSolder.dwMsgType = ON_REQUEST_BARRACK_DISMISSALSOLDER;
	TempOnReqBarrackDismissalSolder.dwSlot    = Slot;

	return Write(&TempOnReqBarrackDismissalSolder, sizeof(OnReqBarrackDismissalSolder));
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	����缺�ҿ� ����.
BOOL	OnlineClient::SendRequestGBarrackEnter(void)
{
	OnReqGBarrackGetList		TempOnReqGBarrackGetList;

	ZeroMemory(&TempOnReqGBarrackGetList, sizeof(OnReqGBarrackGetList));
	TempOnReqGBarrackGetList.dwMsgType = ON_REQUEST_GBARRACK_GETSOLDERLIST;
	
	return Write(&TempOnReqGBarrackGetList, sizeof(OnReqGBarrackGetList));
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	����缺�ҿ��� ����� ����Ѵ�.
BOOL	OnlineClient::SendRequestGBarrackBuySolder(SI32 Code, BYTE* lpName)
{
	OnReqGBarrackBuySolder	TempOnReqGBarrackBuySolder;

	ZeroMemory(&TempOnReqGBarrackBuySolder, sizeof(OnReqGBarrackBuySolder));
	TempOnReqGBarrackBuySolder.dwMsgType     = ON_REQUEST_GBARRACK_BUYSOLDER;
	TempOnReqGBarrackBuySolder.dwBuySolderID = Code;
	strcpy((char*)TempOnReqGBarrackBuySolder.szName, (char*)lpName);

	return Write(&TempOnReqGBarrackBuySolder, sizeof(OnReqGBarrackBuySolder));
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	����缺�� ������ ����.
BOOL	OnlineClient::SendRequestGBarrackChangeJobEnter(void)
{
	OnReqGBarrackGetChangeJobList		TempOnReqGBarrackGetChangeJobList;

	ZeroMemory(&TempOnReqGBarrackGetChangeJobList, sizeof(OnReqGBarrackGetChangeJobList));
	TempOnReqGBarrackGetChangeJobList.dwMsgType = ON_REQUEST_GBARRACK_GETCHANGEJOBSOLDERLIST;
	
	return Write(&TempOnReqGBarrackGetChangeJobList, sizeof(OnReqGBarrackGetChangeJobList));
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	����缺�ҿ��� ����� �����Ѵ�.
BOOL	OnlineClient::SendRequestGBarrackChangeJobSolder(SI32 Slot, SI32 Kind)
{
	OnReqGBarrackChangeJobSolder	TempOnReqGBarrackChangeJobSolder;

	ZeroMemory(&TempOnReqGBarrackChangeJobSolder, sizeof(OnReqGBarrackChangeJobSolder));
	TempOnReqGBarrackChangeJobSolder.dwMsgType       = ON_REQUEST_GBARRACK_CHANGEJOBSOLDER;
	TempOnReqGBarrackChangeJobSolder.dwChangeJobSlot = Slot;
	TempOnReqGBarrackChangeJobSolder.dwChangeJobKind = Kind;

	return Write(&TempOnReqGBarrackChangeJobSolder, sizeof(OnReqGBarrackChangeJobSolder));
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	����缺�� �ذ� ����.
BOOL	OnlineClient::SendRequestGBarrackDismissalEnter(void)
{
	OnReqGBarrackGetDismissalList		TempOnReqGBarrackGetDismissalList;

	ZeroMemory(&TempOnReqGBarrackGetDismissalList, sizeof(OnReqGBarrackGetDismissalList));
	TempOnReqGBarrackGetDismissalList.dwMsgType = ON_REQUEST_GBARRACK_GETDISMISSALSOLDERLIST;
	
	return Write(&TempOnReqGBarrackGetDismissalList, sizeof(OnReqGBarrackGetDismissalList));
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	����缺�ҿ��� ����� �ذ��Ѵ�.
BOOL	OnlineClient::SendRequestGBarrackDismissalSolder(SI32 Slot)
{
	OnReqGBarrackDismissalSolder	TempOnReqGBarrackDismissalSolder;

	ZeroMemory(&TempOnReqGBarrackDismissalSolder, sizeof(OnReqGBarrackDismissalSolder));
	TempOnReqGBarrackDismissalSolder.dwMsgType = ON_REQUEST_GBARRACK_DISMISSALSOLDER;
	TempOnReqGBarrackDismissalSolder.dwSlot    = Slot;

	return Write(&TempOnReqGBarrackDismissalSolder, sizeof(OnReqGBarrackDismissalSolder));
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	2�� ���� ����缺�ҿ� ����.(��ư�� ������.)
BOOL	OnlineClient::SendRequestGBarrackGeneralExEnter(void)
{
	OnReqGBarrackGetList	TempOnReqGBarrackGetList;

	ZeroMemory(&TempOnReqGBarrackGetList,sizeof(OnReqGBarrackGetList));
	TempOnReqGBarrackGetList.dwMsgType = ON_REQUEST_GBARRACK_GET_GENERAL_LIST;

	return Write(&TempOnReqGBarrackGetList, sizeof(OnReqGBarrackGetList));;
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	2�� ���� ����缺�ҿ��� 2�� ����� ����Ѵ�.
BOOL	OnlineClient::SendRequestGBarrackBuyGeneralEx(SI32 Code, BYTE* lpName)
{
	OnReqGBarrackBuySolder	TempOnReqGBarrackBuySolder;

	ZeroMemory(&TempOnReqGBarrackBuySolder, sizeof(OnReqGBarrackBuySolder));
	TempOnReqGBarrackBuySolder.dwMsgType     = ON_REQUEST_GBARRACK_BUY_GENERAL;
	TempOnReqGBarrackBuySolder.dwBuySolderID = Code;
	strcpy((char*)TempOnReqGBarrackBuySolder.szName, (char*)lpName);

	return Write(&TempOnReqGBarrackBuySolder, sizeof(OnReqGBarrackBuySolder));
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//  ����	:	2�� ���� ����缺�ҿ��� ������ ����.
BOOL	OnlineClient::SendRequestGBarrackChangeGeneralExEnter(void)
{
	OnReqGBarrackGetChangeJobList		TempOnReqGBarrackGetChangeJobList;

	ZeroMemory(&TempOnReqGBarrackGetChangeJobList, sizeof(OnReqGBarrackGetChangeJobList));
	TempOnReqGBarrackGetChangeJobList.dwMsgType = ON_REQUEST_GBARRACK_GET_CHANGE_GENERAL_LIST;
	
	return Write(&TempOnReqGBarrackGetChangeJobList, sizeof(OnReqGBarrackGetChangeJobList));
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//  ����	:	2�� ���� ����缺�ҿ��� ���� ����� ����Ѵ�.
BOOL	OnlineClient::SendRequestGBarrackBuyChangeGeneralEx(SI32 Slot, SI32 Kind)
{
	OnReqGBarrackChangeJobSolder	TempOnReqGBarrackChangeJobSolder;

	ZeroMemory(&TempOnReqGBarrackChangeJobSolder, sizeof(OnReqGBarrackChangeJobSolder));
	TempOnReqGBarrackChangeJobSolder.dwMsgType       = ON_REQUEST_GBARRACK_CHANGE_GENERAL;
	TempOnReqGBarrackChangeJobSolder.dwChangeJobSlot = Slot;
	TempOnReqGBarrackChangeJobSolder.dwChangeJobKind = Kind;

	return Write(&TempOnReqGBarrackChangeJobSolder, sizeof(OnReqGBarrackChangeJobSolder));
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������
// 2�� ���� ����缺�ҿ��� �ذ� ����.
BOOL	OnlineClient::SendRequestGBarrackDismissalGeneralExEnter(void)
{
	OnReqGBarrackGetDismissalList		TempOnReqGBarrackGetDismissalList;

	ZeroMemory(&TempOnReqGBarrackGetDismissalList, sizeof(OnReqGBarrackGetDismissalList));
	TempOnReqGBarrackGetDismissalList.dwMsgType = ON_REQUEST_GBARRACK_GETDISMISSAL_GENERAL_LIST;
	
	return Write(&TempOnReqGBarrackGetDismissalList, sizeof(OnReqGBarrackGetDismissalList));
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������
// 2�� ���� ����缺�ҿ��� ����� �ذ��Ѵ�.
BOOL	OnlineClient::SendRequestGBarrackSellDismissalGeneralEx(SI32 Slot)
{
	OnReqGBarrackDismissalSolder	TempOnReqGBarrackDismissalSolder;

	ZeroMemory(&TempOnReqGBarrackDismissalSolder, sizeof(OnReqGBarrackDismissalSolder));
	TempOnReqGBarrackDismissalSolder.dwMsgType = ON_REQUEST_GBARRACK_DISMISSAL_GENERAL;
	TempOnReqGBarrackDismissalSolder.dwSlot    = Slot;

	return Write(&TempOnReqGBarrackDismissalSolder, sizeof(OnReqGBarrackDismissalSolder));
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	���;缺�ҿ� ����.
BOOL	OnlineClient::SendRequestMBarrackEnter(void)
{
	OnReqMBarrackGetList		TempOnReqMBarrackGetList;

	TempOnReqMBarrackGetList.dwMsgType = ON_REQUEST_MBARRACK_GETMONSTERLIST;

	return Write(&TempOnReqMBarrackGetList, sizeof(OnReqMBarrackGetList));
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	���;缺�ҿ��� �뺴�� ����Ѵ�.
BOOL	OnlineClient::SendRequestMBarrackBuySolder(SI32 Code, BYTE* lpName)
{
	OnReqMBarrackBuyMonster		TempOnReqMBarrackBuyMonster;

	ZeroMemory(&TempOnReqMBarrackBuyMonster, sizeof(OnReqMBarrackBuyMonster));
	TempOnReqMBarrackBuyMonster.dwMsgType      = ON_REQUEST_MBARRACK_BUYMONSTER;
	TempOnReqMBarrackBuyMonster.dwBuyMonsterID = Code;
	strcpy((char*)TempOnReqMBarrackBuyMonster.szName, (char*)lpName);

	return Write(&TempOnReqMBarrackBuyMonster, sizeof(OnReqMBarrackBuyMonster));
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	���;缺�� �ذ� ����.
BOOL	OnlineClient::SendRequestMBarrackDismissalEnter(void)
{
	OnReqMBarrackGetDismissList		TempOnReqMBarrackGetDismissList;

	TempOnReqMBarrackGetDismissList.dwMsgType = ON_REQUEST_MBARRACK_GETDISMISSMONSTERLIST;

	return Write(&TempOnReqMBarrackGetDismissList, sizeof(OnReqMBarrackGetDismissList));
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	���;缺�ҿ��� �뺴�� �ذ��Ѵ�.
BOOL	OnlineClient::SendRequestMBarrackDismissalSolder(SI32 Slot)
{
	OnReqMBarrackDismissMonster		TempOnReqMBarrackDismissMonster;

	ZeroMemory(&TempOnReqMBarrackDismissMonster, sizeof(OnReqMBarrackDismissMonster));
	TempOnReqMBarrackDismissMonster.dwMsgType = ON_REQUEST_MBARRACK_DISMISSMONSTER;
	TempOnReqMBarrackDismissMonster.dwSlot    = Slot;

	return Write(&TempOnReqMBarrackDismissMonster, sizeof(OnReqMBarrackDismissMonster));
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	�ǿ����� ġ�� �뺴�� List�� ���´�.
BOOL	OnlineClient::SendRequestHospitalGetCureList(void)
{
	OnReqHospitalGetCureList		TempOnReqHospitalGetCureList;

	TempOnReqHospitalGetCureList.dwMsgType = ON_REQUEST_HOSPITAL_GETCURELIST;

	return Write(&TempOnReqHospitalGetCureList, sizeof(OnReqHospitalGetCureList));
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	�ǿ����� �뺴�� ġ���Ѵ�.
BOOL	OnlineClient::SendRequestHospitalCureSolder(UI08 Slot)
{
	OnReqHospitalCureSolder		TempOnReqHospitalCureSolder;

	ZeroMemory(&TempOnReqHospitalCureSolder, sizeof(OnReqHospitalCureSolder));
	TempOnReqHospitalCureSolder.dwMsgType        = ON_REQUEST_HOSPITAL_CURESOLDER;
	TempOnReqHospitalCureSolder.dwCureSolderSlot = Slot;

	return Write(&TempOnReqHospitalCureSolder, sizeof(OnReqHospitalCureSolder));
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	�ǿ����� ��� �뺴�� ġ���Ѵ�.
BOOL	OnlineClient::SendRequestHospitalCureAllSolder(void)
{
	OnReqHospitalCureAllSolder		TempOnReqHospitalCureAllSolder;

	ZeroMemory(&TempOnReqHospitalCureAllSolder, sizeof(OnReqHospitalCureAllSolder));
	TempOnReqHospitalCureAllSolder.dwMsgType = ON_REQUEST_HOSPITAL_CUREALLSOLDER;

	return Write(&TempOnReqHospitalCureAllSolder, sizeof(OnReqHospitalCureAllSolder));
}


//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	���, �� ���¸� �䱸�Ѵ�.
BOOL	OnlineClient::SendAuditAccountList()
{
	OnReqAuditAccountListMsg		TempOnReqAuditAccountListMsg;

	TempOnReqAuditAccountListMsg.dwMsgType	=	ON_REQUEST_AUDIT_ACCOUNT;

	return Write( &TempOnReqAuditAccountListMsg, sizeof( OnReqAuditAccountListMsg ) );
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	��ο��� �ε��� ����Ʈ�� �䱸 �Ѵ�.
BOOL	OnlineClient::SendAuditOwnListMsg(void)
{
	OnReqAuditOwnListMsg		TempOnReqAuditOwnListMsg;

	TempOnReqAuditOwnListMsg.dwMsgType	=	ON_REQUEST_AUDIT_OWN;

	return Write( &TempOnReqAuditOwnListMsg, sizeof( OnReqAuditOwnListMsg ) );

}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	��ο��� ��� ����Ʈ�� �䱸�Ѵ�.
BOOL	OnlineClient::SendAuditGuildMemberList()
{
	OnReqAuditGuildMemberInfoMsg	TempOnAuditGuildMemberInfoMsg;

	TempOnAuditGuildMemberInfoMsg.dwMsgType = ON_REQUEST_AUDIT_GUILD_INFO;

	return Write(&TempOnAuditGuildMemberInfoMsg,sizeof(OnReqAuditGuildMemberInfoMsg));
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	�εο� ����.
BOOL	OnlineClient::SendRequestWharfEnter(void)
{
	OnReqWharf_Enter			TempOnReqWharf_Enter;

	TempOnReqWharf_Enter.dwMsgType = ON_REQUEST_WHARF_ENTER;
	return Write(&TempOnReqWharf_Enter, sizeof(OnReqWharf_Enter));
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	�εο��� ǥ�� ���.
BOOL	OnlineClient::SendRequestWharfBuyTicket(SI32 TicketKind)
{
	OnReqWharf_BuyTicket			TempOnReqWharf_BuyTicket;

	TempOnReqWharf_BuyTicket.dwMsgType    = ON_REQUEST_WHARF_BUYTICKET;
	TempOnReqWharf_BuyTicket.siTicketKind = TicketKind;
	return Write(&TempOnReqWharf_BuyTicket, sizeof(OnReqWharf_BuyTicket));
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	�εο��� ������.
BOOL	OnlineClient::SendRequestWharfExit(void)
{
	OnReqWharf_Exit			TempOnReqWharf_Exit;

	TempOnReqWharf_Exit.dwMsgType    = ON_REQUEST_WHARF_EXIT;
	return Write(&TempOnReqWharf_Exit, sizeof(OnReqWharf_Exit));
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	�εδ��ǿ� ����.
BOOL	OnlineClient::SendRequestWharfWaitRoomEnter(void)
{
	OnReqWharfWaitRoom_Enter			TempOnReqWharfWaitRoom_Enter;

	TempOnReqWharfWaitRoom_Enter.dwMsgType    = ON_REQUEST_WHARFWAITROOM_ENTER;
	return Write(&TempOnReqWharfWaitRoom_Enter, sizeof(OnReqWharfWaitRoom_Enter));
}
	
//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	�εδ��ǿ��� ������.
BOOL	OnlineClient::SendRequestWharfWaitRoomExit(void)
{
	OnReqWharfWaitRoom_Exit			TempOnReqWharfWaitRoom_Exit;

	TempOnReqWharfWaitRoom_Exit.dwMsgType    = ON_REQUEST_WHARFWAITROOM_EXIT;
	return Write(&TempOnReqWharfWaitRoom_Exit, sizeof(OnReqWharfWaitRoom_Enter));
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	���� ������ ��û�� �Ѵ�.
BOOL	OnlineClient::SendRequestFramInfo(UI08 uiPlantID)
{
	OnReqFarmInfoMsg				TempOnReqFarmInfoMsg;

	TempOnReqFarmInfoMsg.dwMsgType	=	ON_REQUEST_FARM_INFO;
	TempOnReqFarmInfoMsg.uiPlantID	=	uiPlantID;

	return Write( &TempOnReqFarmInfoMsg, sizeof( OnReqFarmInfoMsg ) );
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	����ü� - �ǹ��� ���
BOOL	OnlineClient::SendRequestMakeBuy(UI08 uiPlantID, SI16 siSlot)
{
	OnReqFarmBuyMsg					TempOnReqFarmBuyMsg;

	TempOnReqFarmBuyMsg.dwMsgType	=	ON_REQUEST_FARM_BUY;
	TempOnReqFarmBuyMsg.uiFarmIndex	=	(UI08)siSlot;
	TempOnReqFarmBuyMsg.uiPlantID	=	uiPlantID;

	return Write( &TempOnReqFarmBuyMsg, sizeof( OnReqFarmBuyMsg ) );
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	����ü� - �ǹ��� �Ǽ�
BOOL	OnlineClient::SendRequestBuildStruct( SI16 siPay, SI16 siSlot, CHAR *pBuildKind )
{
	OnReqFarmBuildStructMsg			TempOnReqFarmBuildStructMsg;

	TempOnReqFarmBuildStructMsg.dwMsgType	=	ON_REQUEST_FARM_BUILDSTRUCT;		
	TempOnReqFarmBuildStructMsg.uiSlotIndex	=	(UI08)siSlot;
	TempOnReqFarmBuildStructMsg.siPay		=	siPay;

	strcpy( TempOnReqFarmBuildStructMsg.Code, pBuildKind );
	
	return Write( &TempOnReqFarmBuildStructMsg, sizeof( OnReqFarmBuildStructMsg ) );
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	����ü� - ������ ������ ��û�Ѵ�.
BOOL	OnlineClient::SendRequestSlotInfo( UI08 uiPlantID, SI16 siSlot )
{
	OnReqFarmSlotInfoMsg		TempOnReqFarmSlotInfoMsg;

	TempOnReqFarmSlotInfoMsg.dwMsgType		=	ON_REQUEST_FARM_SLOTINFO;
	TempOnReqFarmSlotInfoMsg.uiFarmIndex	=	(UI08)siSlot;
	TempOnReqFarmSlotInfoMsg.uiPlantID		=	uiPlantID;

	return Write( &TempOnReqFarmSlotInfoMsg, sizeof( OnReqFarmSlotInfoMsg ) );
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	����ü� - ���� ���
BOOL	OnlineClient::SendRequseCancel( UI08 uiPlantID, UI08 uiFarmIndex, UI08 uiSlotIndex )
{
	OnReqPlantCancelPlanMsg		TempOnReqPlantCancelPlanMsg;

	TempOnReqPlantCancelPlanMsg.dwMsgType	=	ON_REQUEST_PLANT_CANCEL_PLAN;
	TempOnReqPlantCancelPlanMsg.uiPlantID	=	uiPlantID;
	TempOnReqPlantCancelPlanMsg.uiFarmIndex	=	uiFarmIndex;
	TempOnReqPlantCancelPlanMsg.uiSlotIndex	=	uiSlotIndex;

	return Write( &TempOnReqPlantCancelPlanMsg, sizeof( OnReqPlantCancelPlanMsg ) );
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	����ü� - �ǹ��� ���׷��̵� �Ѵ�.
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
//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	����ü� - â���� ������ ��û�Ѵ�.
BOOL	OnlineClient::SendRequestStorageInfo( UI08 uiPlantID, UI08 uiPartID, UI08 uiSlotIndex )
{
	OnReqStorageInfoMsg			TempOnReqStorageInfoMsg;	

	TempOnReqStorageInfoMsg.dwMsgType	=	ON_REQUEST_STORAGE_INFO;
	TempOnReqStorageInfoMsg.uiPlantID	=	uiPlantID;
	TempOnReqStorageInfoMsg.uiPartID	=	uiPartID;
	TempOnReqStorageInfoMsg.uiSlotIndex	=	uiSlotIndex;

	return Write( &TempOnReqStorageInfoMsg, sizeof( OnReqStorageInfoMsg ) );

}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	����ü� - �ǹ��� � �������� ���尡?
BOOL	OnlineClient::SendRequestWorkInfo( UI08 uiPlantID, SI16 siFarm, UI08 uiSlot )
{
	OnReqFarmWorkInfoMsg				TempOnReqFarmWorkInfoMsg;
	
	TempOnReqFarmWorkInfoMsg.dwMsgType		=	ON_REQUEST_FARM_WORKINFO;	
	TempOnReqFarmWorkInfoMsg.uiPlantID		=	uiPlantID;
	TempOnReqFarmWorkInfoMsg.uiFarmIndex	=	(UI08)siFarm;
	TempOnReqFarmWorkInfoMsg.uiSlotIndex	=	uiSlot;

	return Write( &TempOnReqFarmWorkInfoMsg, sizeof( OnReqFarmWorkInfoMsg ) );
}
//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	����ü� - â���� �������� �ִ´�.
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

//��������������������������������������������������������������������������������������������������������������������������������������������������������������
// ����ü� - �������� ���� �Ѵٰ�  
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


//��������������������������������������������������������������������������������������������������������������������������������������������������������������
// ����ü� - ����ü� - �뵿�� �Ѵ�.
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


//��������������������������������������������������������������������������������������������������������������������������������������������������������������
// ����ü� - ������ ���� �Ѵ�.
BOOL	OnlineClient::SendRequseUpgradeSlot( UI08 uiSlot, SI16 siPay )
{
	OnReqFarmUpgradeSlotMsg		TempOnReqFarmUpgradeSlotMsg;

	TempOnReqFarmUpgradeSlotMsg.dwMsgType	=	ON_REQUEST_FARM_UPGRADESLOT;
	TempOnReqFarmUpgradeSlotMsg.siPay		=	siPay;
	TempOnReqFarmUpgradeSlotMsg.uiSlotIndex	=	uiSlot;

	return Write( &TempOnReqFarmUpgradeSlotMsg, sizeof( OnReqFarmUpgradeSlotMsg ) );
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������
// ����ü� - �������� ������ ����.
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

//��������������������������������������������������������������������������������������������������������������������������������������������������������������
// ����ü� - ������ Ȯ���� �Ѵ�.
BOOL	OnlineClient::SendRequesExtension( UI08 uiPlantID, UI08 uiFarmIndex )
{
	OnReqFarmExpandSlotMsg			TempOnReqFarmExpandSlotMsg;

	TempOnReqFarmExpandSlotMsg.dwMsgType	=	ON_REQUEST_FARM_EXPANDSLOT;
	TempOnReqFarmExpandSlotMsg.uiPlantID	=	uiPlantID;
	TempOnReqFarmExpandSlotMsg.uiFarmIndex	=	uiFarmIndex;

	return Write( &TempOnReqFarmExpandSlotMsg, sizeof( OnReqFarmExpandSlotMsg ) );
}


//��������������������������������������������������������������������������������������������������������������������������������������������������������������
// ����ü� - ���Կ� �ִ� �ǹ��� ����
BOOL	OnlineClient::SendRequesDestroyStrcut( UI08 uiPlantID, UI08 uiFarmIndex, UI08 uiSlotIndex )
{
	OnReqFarmDestroyStructMsg		TempOnReqFarmDestroyStructMsg;

	TempOnReqFarmDestroyStructMsg.dwMsgType		=	ON_REQUEST_FARM_DESTROYSTRUCT;
	TempOnReqFarmDestroyStructMsg.uiPlantID		=	uiPlantID;
	TempOnReqFarmDestroyStructMsg.uiFarmIndex	=	uiFarmIndex;
	TempOnReqFarmDestroyStructMsg.uiSlotIndex	=	uiSlotIndex;

	return Write( &TempOnReqFarmDestroyStructMsg, sizeof( OnReqFarmDestroyStructMsg ) );
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������
// ����ü� - ���ϴ� ���� ���� �ִ� �˾� ����.
BOOL	OnlineClient::SendRequseFarmWorkStartMsg( UI08 uiSlot )
{
	OnReqFarmWorkStartMsg		TempOnReqFarmWorkStartMsg;

	TempOnReqFarmWorkStartMsg.dwMsgType		=	ON_REQUEST_FARM_WORKSTART;
	TempOnReqFarmWorkStartMsg.uiSlotIndex	=	uiSlot;

	return Write( &TempOnReqFarmWorkStartMsg, sizeof( OnReqFarmWorkStartMsg ) );
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������
// ����ü� - �ӱ��� ����
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


//��������������������������������������������������������������������������������������������������������������������������������������������������������������
// ����ü� - �������� �����ٰ� �����ش�.
BOOL	OnlineClient::SendRequestLeaveStructure()
{
	OnReqLeaveStructureMsg	TempOnReqLeaveStructureMsg;

	TempOnReqLeaveStructureMsg.dwMsgType	=	ON_REQUEST_LEAVE_STRUCTURE;

	return Write( &TempOnReqLeaveStructureMsg, sizeof( OnReqLeaveStructureMsg ) );
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������
// ����ü� - ���忡�� �����ٰ� �����ش�.
BOOL	OnlineClient::SendRequestLeaveFactory()
{
	OnReqLeaveFactoryMsg	TempOnReqLeaveFactoryMsg;

	TempOnReqLeaveFactoryMsg.dwMsgType	=	ON_REQUEST_LEAVE_FACTORY;

	return Write( &TempOnReqLeaveFactoryMsg, sizeof( OnReqLeaveFactoryMsg ) );
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	�ƾ��� ����
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

//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	������ ����
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

//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	������ ���
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

//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	�����ϰ� �ִ� �������� �޴´�.
BOOL	OnlineClient::SendRequestItemInfo()
{
	OnReqWearItemInfoMsg			TempOnReqWearItemInfoMsg;

	TempOnReqWearItemInfoMsg.dwMsgType	=	ON_REQUEST_WEARITEM_INFO;

	return Write( &TempOnReqWearItemInfoMsg, sizeof( OnReqWearItemInfoMsg ) );
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	��û ����
BOOL	OnlineClient::SendRequestGovernment_Main_Enter(void)
{
	OnReqEnterGovernment			TempOnReqEnterGovernment;

	TempOnReqEnterGovernment.dwMsgType	=	ON_REQUEST_GOVERNMENT_MAIN_ENTER;

	return Write(&TempOnReqEnterGovernment, sizeof(OnReqEnterGovernment));
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	��û ������
BOOL	OnlineClient::SendRequestGovernment_Main_Leave(void)
{
	OnReqLeaveGovernment			TempOnReqLeaveGovernment;

	TempOnReqLeaveGovernment.dwMsgType	=	ON_REQUEST_GOVERNMENT_MAIN_LEAVE;

	return Write(&TempOnReqLeaveGovernment, sizeof(OnReqLeaveGovernment));
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	��û, ���� ����
BOOL	OnlineClient::SendRequestGovernment_Invenstment_Enter(SI16 Type)
{
	OnReqGovernmentEnterInvestment		TempOnReqGovernmentEnterInvestment;

	TempOnReqGovernmentEnterInvestment.dwMsgType	=	ON_REQUEST_GOVERNMENT_INVESTMENT_ENTER;
	TempOnReqGovernmentEnterInvestment.m_Type       =   Type;

	return Write(&TempOnReqGovernmentEnterInvestment, sizeof(OnReqGovernmentEnterInvestment));
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	��û, ���� ������
BOOL	OnlineClient::SendRequestGovernment_Invenstment_Leave(void)
{
	OnReqGovernmentLeaveInvestment		TempOnReqGovernmentLeaveInvestment;

	TempOnReqGovernmentLeaveInvestment.dwMsgType	=	ON_REQUEST_GOVERNMENT_INVESTMENT_LEAVE;

	return Write(&TempOnReqGovernmentLeaveInvestment, sizeof(OnReqGovernmentLeaveInvestment));
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	��û, ���� List���
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

//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	��û, ���� �ϱ�
BOOL	OnlineClient::SendRequestGovernment_Invenstment_SetInvestment(SI16 Type, MONEY InvestmentMoney_InPocket, MONEY InvestmentMoney_InBank/* = 0*/)
{
	OnReqGovernmentSetInvestment			TempOnReqGovernmentSetInvestment;

	TempOnReqGovernmentSetInvestment.dwMsgType				= ON_REQUEST_GOVERNMENT_INVESTMENT_SETINVESTMENT;
//	TempOnReqGovernmentSetInvestment.m_Type					= Type;
	TempOnReqGovernmentSetInvestment.m_InvestmentMoney		= InvestmentMoney_InPocket;	// �����Ϸ��� ��(������)
	TempOnReqGovernmentSetInvestment.m_InvestmentSaveMoney	= InvestmentMoney_InBank;	// �����Ϸ��� ��(����)

	// ������ �����ϴ� �� ���ڱ��� InvestmentMoney_InPocket + InvestmentMoney_InBank �̸�,
	// ����ڰ� ��ź ���� �������̿��Ͽ� �����ϴ� �濡�� ������ ��(InvestmentMoney_InBank)�� ���ȴ�.

	return Write(&TempOnReqGovernmentSetInvestment, sizeof(OnReqGovernmentSetInvestment));
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	���� �Ұ��� �б�
BOOL	OnlineClient::SendRequestVillageDescript_Read(UI16 uiVillageCode)
{
	OnReqVillageDescript			TempOnReqVillageDescript;

	TempOnReqVillageDescript.dwMsgType		 = ON_REQUEST_READ_DESCRIPT;
	TempOnReqVillageDescript.uiVillageCode	 = uiVillageCode;

	return Write(&TempOnReqVillageDescript, sizeof(OnReqVillageDescript));
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	���� �Ұ��� ����
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

//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	���� ���� ��û
BOOL	OnlineClient::SendRequestVillageData(UI16 uiVillageCode)
{
	OnReqVillageGetNation			TempOnReqVillageGetNation;

	TempOnReqVillageGetNation.dwMsgType		= ON_REQUEST_VILLAGE_NATION;
	TempOnReqVillageGetNation.uiVillageCode	= uiVillageCode;

	return Write(&TempOnReqVillageGetNation, sizeof(OnReqVillageGetNation));
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	�̸�Ƽ�� ���
BOOL	OnlineClient::SendRequestImoticon(UI16 ImoticonID)
{
	OnReqImoticonMsg		TempOnReqImoticonMsg;

	TempOnReqImoticonMsg.dwMsgType    = ON_REQUEST_IMOTICON;
	TempOnReqImoticonMsg.uiImoticonID = ImoticonID;

	return Write(&TempOnReqImoticonMsg, sizeof(TempOnReqImoticonMsg));
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	�ŷ� ��û
BOOL	OnlineClient::SendRequestAskTrade( UI16 uiID )
{
	OnReqAskTradeMsg		TempOnReqAskTradeMsg;

	TempOnReqAskTradeMsg.dwMsgType	=	ON_REQUEST_TRADE;
	TempOnReqAskTradeMsg.UserID		=	uiID;

	return Write( &TempOnReqAskTradeMsg, sizeof( OnReqAskTradeMsg ) );
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	�ŷ� ��û ���� ����� �ٽ� ������ �޼���
BOOL	OnlineClient::SendRequestTrade(BOOL bAbccept)
{
	OnResAskTradeMsg		TempOnResAskTradeMsg;

	TempOnResAskTradeMsg.dwMsgType	= ON_REQUEST_ACCEPT_TRADE;
	TempOnResAskTradeMsg.dwResponse = bAbccept == TRUE ? TRUE : FALSE;

	return Write( &TempOnResAskTradeMsg, sizeof( OnResAskTradeMsg ) );
}


// ����		: ��Ƽ ��û.
BOOL	OnlineClient::SendRequestAskParty(UI16	uiID)
{

	OnReqAskPartyMsg		TempOnReqAskPartyMsg;

	TempOnReqAskPartyMsg.dwMsgType	=	ON_REQUEST_PARTY;
	TempOnReqAskPartyMsg.UserID		=	uiID;

	return Write( &TempOnReqAskPartyMsg, sizeof( OnReqAskPartyMsg ) );
}

// ����		: ��Ƽ ��û ���� ����� �ٽ� ������ �޼���.
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



//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	������ �÷� ���´�.
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

//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	�ŷ� ���
BOOL	OnlineClient::SendRequestCancelTrade()
{
	OnReqCancelTradeMsg		TempOnReqCancelTradeMsg;

	TempOnReqCancelTradeMsg.dwMsgType	=	ON_REQUEST_CANCEL_TRADE;

	return Write( &TempOnReqCancelTradeMsg, sizeof( OnReqCancelTradeMsg ) );
}


//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	�ŷ� ��ư�� ����
BOOL	OnlineClient::SendRequestConfirmTrade( BOOL bFlag )
{
	OnReqFinishChooseTradeItemMsg			TempOnReqFinishChooseTradeItemMsg;

	TempOnReqFinishChooseTradeItemMsg.dwMsgType			=	ON_REQUEST_FINISH_CHOOSE_TRADEITEM;
	TempOnReqFinishChooseTradeItemMsg.bFinishChooseItem	=	bFlag;

	return Write( &TempOnReqFinishChooseTradeItemMsg, sizeof( OnReqFinishChooseTradeItemMsg ) );
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	���� ��ư�� ����
BOOL	OnlineClient::SendRequestTradeSuccess(BOOL bPush)
{
	OnReqDecideTradeMsg		TempOnReqDecideTradeMsg;

	TempOnReqDecideTradeMsg.dwMsgType		=	ON_REQUEST_DECIDE_TRADE;
	TempOnReqDecideTradeMsg.bDecideTrade	=	bPush;

	return Write( &TempOnReqDecideTradeMsg, sizeof(OnReqDecideTradeMsg) );
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	��� ���
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
//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	���ʽ� ��ġ 
BOOL	OnlineClient::SendBouns( UI08 uiFollowerID, AbilityKindType Ability )
{
	OnReqIncAbilityByBonusMsg		TempOnReqIncAbilityByBonusMsg;


	TempOnReqIncAbilityByBonusMsg.dwMsgType		=	ON_REQUEST_INC_ABILITY_BYBONUS;
	TempOnReqIncAbilityByBonusMsg.uiFollowerID	=	uiFollowerID;
	TempOnReqIncAbilityByBonusMsg.AbilityKind	=	Ability;

	return Write( &TempOnReqIncAbilityByBonusMsg, sizeof( TempOnReqIncAbilityByBonusMsg ) );
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	������ ó�� �����Ѵ�.
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

//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	���� ������ ������ ���� �� �߰�
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

//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	���Ǵ뿡�� ������.
BOOL	OnlineClient::SendLeaveBoothMsg()
{
	OnReqPMCloseMsg				TempOnReqPMCloseMsg;

	TempOnReqPMCloseMsg.dwMsgType	=	ON_REQUEST_PM_CLOSE;

	return Write( &TempOnReqPMCloseMsg, sizeof( OnReqPMCloseMsg ) );
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	���ǿ� �ִ� �������� ��û�� �Ѵ�.
BOOL	OnlineClient::SendDisplayBoothItemMsg( UI16 uiBoothID )
{
	OnReqDisplayBoothItemMsg		TempOnReqDisplayBoothItemMsg;

	TempOnReqDisplayBoothItemMsg.dwMsgType	=	ON_REQUEST_DISPLAYBOOTH;
	TempOnReqDisplayBoothItemMsg.uiBoothID	=	uiBoothID;

	return Write( &TempOnReqDisplayBoothItemMsg, sizeof( OnReqDisplayBoothItemMsg ) );
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	���ǿ� �ִ� ������ ������ ���´�.
BOOL	OnlineClient::SendGetBoothItemInfo(UI16	uiAccount)
{
	OnReqPMItemInfoMsg			TempOnReqPMItemInfoMsg;

	TempOnReqPMItemInfoMsg.dwMsgType	=	ON_REQUEST_PM_ITEMINFO;
	TempOnReqPMItemInfoMsg.uiAccount	=	uiAccount;
//	TempOnReqPMItemInfoMsg.uiSlotID		=	uiSlotID;

	return Write( &TempOnReqPMItemInfoMsg, sizeof( OnReqPMItemInfoMsg ) );
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	���ǹ����� �����ϴ�.
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

//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	���� ������ �޶�� ��û
BOOL	OnlineClient::SendRequestGovernment_VillageInfo(void)
{
	OnReqGovernmentVillageInfo		TempOnReqGovernmentVillageInfo;

	TempOnReqGovernmentVillageInfo.dwMsgType = ON_REQUEST_GOVERNMENT_VILLAGEINFO;
	return Write( &TempOnReqGovernmentVillageInfo, sizeof( TempOnReqGovernmentVillageInfo ));
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	�ٸ� ��������� ����
BOOL	OnlineClient::SendRequestGovernment_GetDiffGuild(void)
{
	OnReqGuildListMsg				TempOnReqGuildListMsg;

	TempOnReqGuildListMsg.dwMsgType = ON_REQUEST_GUILD_LIST;
	return Write(&TempOnReqGuildListMsg, sizeof(TempOnReqGuildListMsg));
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	��ܿ� �����ҷ��� �÷��̾� ������ ����
BOOL	OnlineClient::SendRequestGovernment_GetJoinGuild(SI32 PageNum)
{
	OnReqGuildWaitingJoinMembersListMsg				TempOnReqGuildWaitingJoinMembersListMsg;

	TempOnReqGuildWaitingJoinMembersListMsg.dwMsgType	=	ON_REQUEST_GUILD_WAITING_JOIN_MEMBERS_LIST;
	TempOnReqGuildWaitingJoinMembersListMsg.dwPage      =   PageNum; 

	return Write(&TempOnReqGuildWaitingJoinMembersListMsg, sizeof(TempOnReqGuildWaitingJoinMembersListMsg));
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	��ܿ��� �ذ��ҷ��� �÷��̾� ������ ����
BOOL	OnlineClient::SendRequestGovernment_GetDismissalGuild(SI32 PageNum, UI32 uiGuildID)
{
	OnReqGuildMembersListMsg				TempOnReqGuildMembersListMsg;

	TempOnReqGuildMembersListMsg.dwMsgType	=	ON_REQUEST_GUILD_MEMBERS_LIST;
	TempOnReqGuildMembersListMsg.dwPage     =   PageNum;
	TempOnReqGuildMembersListMsg.uiGuidlID  =   uiGuildID;

	return Write(&TempOnReqGuildMembersListMsg, sizeof(TempOnReqGuildMembersListMsg));
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	����� ����
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

//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	����� ��ü
BOOL	OnlineClient::SendRequestGovernment_DeleteGuild(void)
{
	OnReqDeleteGuildMsg				TempOnReqDeleteGuildMsg;

	TempOnReqDeleteGuildMsg.dwMsgType	=	ON_REQUEST_DELETE_GUILD;

	return Write(&TempOnReqDeleteGuildMsg, sizeof(TempOnReqDeleteGuildMsg));
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	��� ����/Ż��
BOOL OnlineClient::SendRequestGovernment_GuildJoinLeave(void)
{
	OnReqJoinLeaveGuildMsg				TempOnReqJoinLeaveGuildMsg;

	TempOnReqJoinLeaveGuildMsg.dwMsgType	=	ON_REQUEST_JOIN_LEAVE_GUILD;

	return Write(&TempOnReqJoinLeaveGuildMsg, sizeof(TempOnReqJoinLeaveGuildMsg));
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	��� ������ �����
BOOL OnlineClient::SendRequestGovernment_JoinGuildPlayer(char* lpID)
{
	OnReqJoinGuildOkNoMsg		TempOnReqJoinGuildOkNoMsg;

	ZeroMemory(&TempOnReqJoinGuildOkNoMsg, sizeof(TempOnReqJoinGuildOkNoMsg));
	TempOnReqJoinGuildOkNoMsg.dwMsgType	 = ON_REQUEST_JOINGUILD_OKNO;
	TempOnReqJoinGuildOkNoMsg.dwResponse = ON_RET_OK;
	strncpy((char*)TempOnReqJoinGuildOkNoMsg.szID, lpID, ON_ID_LENGTH);

	return Write(&TempOnReqJoinGuildOkNoMsg, sizeof(TempOnReqJoinGuildOkNoMsg));
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	��� ������ �ź���
BOOL OnlineClient::SendRequestGovernment_JoinCancelGuildPlayer(char* lpID)
{
	OnReqJoinGuildOkNoMsg		TempOnReqJoinGuildOkNoMsg;

	ZeroMemory(&TempOnReqJoinGuildOkNoMsg, sizeof(TempOnReqJoinGuildOkNoMsg));
	TempOnReqJoinGuildOkNoMsg.dwMsgType	 = ON_REQUEST_JOINGUILD_OKNO;
	TempOnReqJoinGuildOkNoMsg.dwResponse = ON_RET_NO;
	strncpy((char*)TempOnReqJoinGuildOkNoMsg.szID, lpID, ON_ID_LENGTH);

	return Write(&TempOnReqJoinGuildOkNoMsg, sizeof(TempOnReqJoinGuildOkNoMsg));
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	��� Ż�� ��Ŵ
BOOL OnlineClient::SendRequestGovernment_DismissalGuildPlayer(char* lpID)
{
	OnReqDismissalMsg		TempOnReqDismissalMsg;

	ZeroMemory(&TempOnReqDismissalMsg, sizeof(TempOnReqDismissalMsg));
	TempOnReqDismissalMsg.dwMsgType	=	ON_REQUEST_GUILD_DISMISSAL;
	strncpy((char*)TempOnReqDismissalMsg.szID, lpID, ON_ID_LENGTH);

	return Write(&TempOnReqDismissalMsg, sizeof(TempOnReqDismissalMsg));
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	��� ����, ���� ����
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
// ������ ȭģ���� �޽��� ������
BOOL OnlineClient::SendRequestGovernment_OfferPeace(CHAR *szGuildID, CHAR *szMessage)
{
	OnReqGuildFriendlyMessageOffer	TempOnReqGuildFriendlyMessageOffer;

	ZeroMemory(&TempOnReqGuildFriendlyMessageOffer, sizeof(OnReqGuildFriendlyMessageOffer));

	TempOnReqGuildFriendlyMessageOffer.dwMsgType  = ON_REQUEST_GUILD_FRIELDLY_MESSAGE_OFFER;
	strncpy((char*)TempOnReqGuildFriendlyMessageOffer.szGuildID, szGuildID, ON_GUILDNAME_LENGTH);	// ������ �̸�
	strncpy((char*)TempOnReqGuildFriendlyMessageOffer.szMessage, szMessage, 80);					// ȭģ���� �޽���

	return Write(&TempOnReqGuildFriendlyMessageOffer, sizeof(TempOnReqGuildFriendlyMessageOffer));
}

// robypark 2004/11/25 16:16
// ������ ȭģ���� ���� �޽��� ������
BOOL OnlineClient::SendRequestGovernment_AcceptOfferedPeace(CHAR *szGuildID)
{
	OnReqGuildFriendlyMessageAccept	TempOnReqGuildFriendlyMessageAccept;

	ZeroMemory(&TempOnReqGuildFriendlyMessageAccept, sizeof(OnReqGuildFriendlyMessageAccept));

	TempOnReqGuildFriendlyMessageAccept.dwMsgType  = ON_REQUEST_GUILD_FRIENDLY_MESSAGE_ACCEPT;
	strncpy((char*)TempOnReqGuildFriendlyMessageAccept.szGuildID, szGuildID, ON_GUILDNAME_LENGTH);	// ������ �̸�

	return Write(&TempOnReqGuildFriendlyMessageAccept, sizeof(TempOnReqGuildFriendlyMessageAccept));
}

// robypark 2004/11/25 16:16
// ������ ȭģ���� ���� �޽��� ������
BOOL OnlineClient::SendRequestGovernment_DeclineOfferedPeace(CHAR *szGuildID)
{
	OnReqGuildFreindlyMessageCancel	TempOnReqGuildFreindlyMessageCancel;

	ZeroMemory(&TempOnReqGuildFreindlyMessageCancel, sizeof(OnReqGuildFreindlyMessageCancel));

	TempOnReqGuildFreindlyMessageCancel.dwMsgType  = ON_REQUEST_GUILD_FRIENDLY_MESSAGE_CANCEL;
	strncpy((char*)TempOnReqGuildFreindlyMessageCancel.szGuildID, szGuildID, ON_GUILDNAME_LENGTH);	// ������ �̸�

	return Write(&TempOnReqGuildFreindlyMessageCancel, sizeof(TempOnReqGuildFreindlyMessageCancel));
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	��� ��������Ʈ ��û
BOOL OnlineClient::SendRequestGovernment_GetGuildVillageList(char* lpGuildID)
{
	OnReqGuildDetailInfoMsg		TempOnReqGuildDetailInfoMsg;

	ZeroMemory(&TempOnReqGuildDetailInfoMsg, sizeof(OnReqGuildDetailInfoMsg));
	TempOnReqGuildDetailInfoMsg.dwMsgType  = ON_REQUEST_GUILD_DETAIL_INFO;
	strncpy((char*)TempOnReqGuildDetailInfoMsg.szGuildID, lpGuildID, ON_GUILDNAME_LENGTH);

	return Write(&TempOnReqGuildDetailInfoMsg, sizeof(TempOnReqGuildDetailInfoMsg));
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	������ ���¿� ���ډƼ� �ִ� �ִ뵷�� ���´�.
BOOL OnlineClient::SendRequestGovernment_GetMaxVillageDefenceInvestment(void)
{
	OnReqMaxVillageDefenceInvestment		TempOnReqMaxVillageDefenceInvestment;

	TempOnReqMaxVillageDefenceInvestment.dwMsgType = ON_REQUEST_MAXVILLAGEDEFENCEINVESTMENT;

	return Write(&TempOnReqMaxVillageDefenceInvestment, sizeof(TempOnReqMaxVillageDefenceInvestment));
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	������ ���¿� �����Ѵ�.
BOOL OnlineClient::SendRequestGovernment_SetVillageDefenceInvestment(MONEY DefenceMoney)
{
	OnReqSetVillageDefenceInvestment		TempOnReqSetVillageDefenceInvestment;

	TempOnReqSetVillageDefenceInvestment.dwMsgType       = ON_REQUEST_SETVILLAGEDEFENCEINVESTMENT;
	TempOnReqSetVillageDefenceInvestment.InvestmentMoney = DefenceMoney;

	return Write(&TempOnReqSetVillageDefenceInvestment, sizeof(TempOnReqSetVillageDefenceInvestment));
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	�뺴�� �̸��� ����
BOOL OnlineClient::SendRequestSetRename( CHAR *pRename, UI08 uiFollower )
{
	OnReqSetNameMsg			TempOnReqSetNameMsg;

	TempOnReqSetNameMsg.dwMsgType		=	ON_REQUEST_AUDIT_SETNAME;
	TempOnReqSetNameMsg.uiFollowerNum	=	uiFollower;
	strncpy( TempOnReqSetNameMsg.cName, pRename, ON_ID_LENGTH );

	return Write( &TempOnReqSetNameMsg, sizeof( OnReqSetNameMsg ) );
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	���� TickCount�� ���� 
BOOL OnlineClient::SendGameTickCount( DWORD TickCount, UI16 uiType )
{
	OnHeartBeatMsg		TempOnHeartBeatMsg;

	TempOnHeartBeatMsg.dwMsgType	= ON_HEARTBEAT;
	TempOnHeartBeatMsg.dwTimer		= TickCount;
	TempOnHeartBeatMsg.uiType		= uiType;

	return Write( &TempOnHeartBeatMsg, sizeof( OnHeartBeatMsg ) );
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	������ �����Ѵ�.(�ٰŸ�)
BOOL	OnlineClient::SendShortDistanceAttackVillage( UI16 VillageCode, SI16 siAttackType )
{
	OnReqAttackVillageMsg		TempOnReqAttackVillageMsg;

	TempOnReqAttackVillageMsg.dwMsgtype     = ON_REQUEST_ATTACK_VILLAGE;
	TempOnReqAttackVillageMsg.uiVillageCode = VillageCode;
	TempOnReqAttackVillageMsg.siAttackType	= siAttackType;/*ON_ATTACK_TYPE_NORMAL;*/

	return Write(&TempOnReqAttackVillageMsg, sizeof(OnReqAttackVillageMsg));
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	������ �����Ѵ�.(���Ÿ�)
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

// ������ ���� �������̽��� ���ڴٰ� ��û
BOOL	OnlineClient::SendEnterVillageDefenceInterface(UI16 uiVillageCode)
{
	OnReqWarVillageDefence			TempOnReqWarVillageDefence;

	TempOnReqWarVillageDefence.dwMsgType		=	ON_REQUEST_WAR_VILLAGE_DEFENCE;
	TempOnReqWarVillageDefence.uiVillageCode	=	uiVillageCode;

	return Write(&TempOnReqWarVillageDefence, sizeof(OnReqWarVillageDefence));
}

/*
// ������ ���� �������̽� �󿡼� ������ ���ڴٰ� ��û
BOOL	OnlineClient::SendEnterVillageFromVillageDefenceInterface(UI16 uiVillageCode)
{
	OnReqWarVillageIn			TempOnReqWarVillageIn;
	
	TempOnReqWarVillageIn.dwMsgType			=	ON_REQUEST_WAR_VILLAGE_IN;
	TempOnReqWarVillageIn.uiVillageCode		=	uiVillageCode;

	return Write(&TempOnReqWarVillageIn, sizeof(OnReqWarVillageIn));
}
*/

// ������ ���� �������̽� �󿡼� �����ڴٰ� ��û
BOOL	OnlineClient::SendLeaveVillageDefenceInterface(UI16 uiVillageCode)
{
	OnReqWarVillageIn			TempOnReqWarVillageIn;
	
	TempOnReqWarVillageIn.dwMsgType			=	ON_REQUEST_VILLAGE_DEFENCE_STOP;
	TempOnReqWarVillageIn.uiVillageCode		=	uiVillageCode;

	return Write(&TempOnReqWarVillageIn, sizeof(OnReqWarVillageIn));
}

// ������
// ������ ���� Ÿ���� ������ �������� ��û�ϱ�
BOOL	OnlineClient::SendCanChangeVillageAttackType(UI16 VillageCode)
{
	OnReqVillageAttackTypeCheck		TempOnReqVillageAttackTypeCheck;

	TempOnReqVillageAttackTypeCheck.dwMsgType		=	ON_REQUEST_VILLAGE_ATTACK_TYPE_CHECK;
	TempOnReqVillageAttackTypeCheck.uiVillageCode	=	VillageCode;

	return Write(&TempOnReqVillageAttackTypeCheck, sizeof(OnReqVillageAttackTypeCheck));
}

// ������
// ���� �⸧ ���� �õ� ��û�ϱ�
BOOL	OnlineClient::SendVillagePlayerOilAttack(UI16  VillageCode)
{
	OnReqVillagePlayerOilAttack		TempOnReqVillagePlayerOilAttack;

	TempOnReqVillagePlayerOilAttack.dwMsgType		=	ON_REQUEST_VILLAGE_PLAYER_OIL_ATTACK;
	TempOnReqVillagePlayerOilAttack.uiVillageCode	=	VillageCode;

	return Write(&TempOnReqVillagePlayerOilAttack, sizeof(OnReqVillagePlayerOilAttack));
}

// ������
// ���� �� ���� �õ� ��û�ϱ�
BOOL	OnlineClient::SendVillagePlayerStoneAttack(UI16  VillageCode)
{
	OnReqVillagePlayerStoneAttack		TempOnReqVillagePlayerStoneAttack;

	TempOnReqVillagePlayerStoneAttack.dwMsgType		=	ON_REQUEST_VILLAGE_PLAYER_STONE_ATTACK;
	TempOnReqVillagePlayerStoneAttack.uiVillageCode	=	VillageCode;

	return Write(&TempOnReqVillagePlayerStoneAttack, sizeof(OnReqVillagePlayerStoneAttack));
}

// ������
// ���� ���� ��� ��û�ϱ�
BOOL	OnlineClient::SendVillageDefenceUp(UI16 VillageCode)
{
	OnReqPlayerVillageDefenceUp		TempOnReqPlayerVillageDefenceUp;

	TempOnReqPlayerVillageDefenceUp.dwMsgType		=	ON_REQUEST_VILLAGE_DEFENCE;
	TempOnReqPlayerVillageDefenceUp.uiVillageCode	=	VillageCode;

	return Write(&TempOnReqPlayerVillageDefenceUp, sizeof(OnReqPlayerVillageDefenceUp));
}

// robypark 2005/1/28 18:3
// ������ �������� �����ϰڴٰ� ������ ��û
BOOL	OnlineClient::SendTransformToSiegeUnit(SI16 siChangeKindIdx)
{
	OnReqChangeUnit		TempOnReqChangeUnit;

	TempOnReqChangeUnit.dwMsgType		=	ON_REQUEST_CHANGE_WAR;
	TempOnReqChangeUnit.siChangeKind	=	siChangeKindIdx;		// �����Ϸ��� ������ �迭 �ε���(��: �˺� 0, ...)

	return Write(&TempOnReqChangeUnit, sizeof(OnReqChangeUnit));
}

// robypark 2005/1/29 14:13
// ������ ���޼����� ���� 10�ʸ��� �ڵ� ȸ�� ��û
BOOL	OnlineClient::SendReuqestChargeMP(void)
{
	OnMsg			TempOnMsg;

	TempOnMsg.dwMsgType		=	ON_REQUEST_CHARGE_MP;

	return Write(&TempOnMsg, sizeof(OnMsg));
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	�������� ĳ���Ͱ� ĳ���͸� �����Ѵٴ� �޼����� �˷��ش�.
BOOL	OnlineClient::SendFieldAttackReady(UI16	uiUniqueID,SI16 siAttackCharX,SI16 siAttackCharY)
{
	OnReqFieldAttackRdyMsg		TempOnReqFieldAttackRdyMsg;

	TempOnReqFieldAttackRdyMsg.dwMsgType		= ON_REQUEST_FIELDATTACK_READY;
	TempOnReqFieldAttackRdyMsg.uiUniqueID		= uiUniqueID;
//	TempOnReqFieldAttackRdyMsg.siAttackCharX	= siAttackCharX;
//	TempOnReqFieldAttackRdyMsg.siAttackCharY	= siAttackCharY;
	return Write(&TempOnReqFieldAttackRdyMsg,sizeof(OnReqFieldAttackRdyMsg));
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	�������� ĳ���Ͱ� ����� ���⿡ �¾Ҵ�.
BOOL	OnlineClient::SendFieldAttackAction(UI16 uiEnemyCharID, SI16 siAttackType)
{
	OnReqFieldAttackActionMsg	TempOnReqFieldAttackActionMsg;

	TempOnReqFieldAttackActionMsg.dwMsgType		= ON_REQUEST_FIELDATTACK_ACTION;
	TempOnReqFieldAttackActionMsg.uiUniqueID	= uiEnemyCharID;
	//Gscprotocol ���� �Ӽ��� ���� siAttackType�� ���Ѵ�.
	TempOnReqFieldAttackActionMsg.siAttackType	= siAttackType;/*ON_ATTACK_TYPE_NORMAL;*/


	return Write(&TempOnReqFieldAttackActionMsg,sizeof(OnReqFieldAttackActionMsg));
}
 
//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	ü���� �ٵǼ� �׾���.
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

//���� �ȿ��� ��� ����ǰ�� ������ �ø���.(���.����� ����)
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
	TempOnReqDecreaseGuildSupply.siSupplyQuantity	=	siSupply;			// �ʿ��� ��
	TempOnReqDecreaseGuildSupply.siVillageCode		=	siVillageCode;		// ��� ����

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
// ������ ���� ���� �� ��û�ϱ�
BOOL	OnlineClient::SendVillageGarrisonCount(SI16 siVillageCode)
{
	OnReqGetVillageDefenceSoldier	TempOnReqGetVillageDefenceSoldier;

	TempOnReqGetVillageDefenceSoldier.dwMsgType		=	ON_REQUEST_GET_VILLAGE_DEFENCE_SOLDIER;
	TempOnReqGetVillageDefenceSoldier.siVillageCode	=	siVillageCode;

	return Write(&TempOnReqGetVillageDefenceSoldier, sizeof(OnReqGetVillageDefenceSoldier));
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	Change Guild Flag
BOOL	OnlineClient::SendChangeGuildFlag(SI32 siGuildFlag)
{
	OnReqGuildChangeFlag		TempOnReqGuildChangeFlag;

	TempOnReqGuildChangeFlag.dwMsgType = ON_REQUEST_CHANGE_GUILD_FLAG;
	TempOnReqGuildChangeFlag.siFlag    = siGuildFlag;
	return Write(&TempOnReqGuildChangeFlag, sizeof(TempOnReqGuildChangeFlag));
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	���Ŵ� ��� ���
BOOL	OnlineClient::SendUsedFlag(void)
{
	OnReqUsedFlagList		TempOnReqUsedFlagList;

	TempOnReqUsedFlagList.dwMsgType = ON_REQUEST_USED_FLAG_LIST;
	return Write(&TempOnReqUsedFlagList, sizeof(TempOnReqUsedFlagList));
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	������ ���� ��踦 ���´�.
BOOL	OnlineClient::SendRequestGovernment_GetVillageProfitStatistics(void)
{
	OnReqProfitStatistics		TempOnReqProfitStatistics;

	TempOnReqProfitStatistics.dwMsgType = ON_REQUEST_PROFIT_STATISTICS;
	return Write(&TempOnReqProfitStatistics, sizeof(TempOnReqProfitStatistics));
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	���ڱ��� �����Ѵ�.
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

//��������������������������������������������������������������������������������������������������������������������������������������������������������������
// ���� �Խ��ǿ��� �Խù� ������ ����
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

//��������������������������������������������������������������������������������������������������������������������������������������������������������������
// NPC ����
BOOL	OnlineClient::SendRequestQuestSelectNPC(SI16 siNPCCode, SI32 siBookCount)
{
	OnReqQuestSelectNPC		TempOnReqQuestSelectNPC;

	ZeroMemory(&TempOnReqQuestSelectNPC, sizeof(TempOnReqQuestSelectNPC));
	TempOnReqQuestSelectNPC.dwMsgType   = ON_REQUEST_QUEST_SELECTNPC;
	TempOnReqQuestSelectNPC.siNPCCode   = siNPCCode;
	TempOnReqQuestSelectNPC.siBookCount = siBookCount;

	return Write(&TempOnReqQuestSelectNPC, sizeof(OnReqQuestSelectNPC));
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������
// Quest ����
BOOL	OnlineClient::SendRequestQuestAcceptQuest(SI32 siQuestCode)
{
	OnReqQuestAcceptQuest		TempOnReqQuestAcceptQuest;

	ZeroMemory(&TempOnReqQuestAcceptQuest, sizeof(TempOnReqQuestAcceptQuest));
	TempOnReqQuestAcceptQuest.dwMsgType   = ON_REQUEST_QUEST_ACCEPTQUEST;
	TempOnReqQuestAcceptQuest.m_QuestCode = siQuestCode;

	return Write(&TempOnReqQuestAcceptQuest, sizeof(TempOnReqQuestAcceptQuest));
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������
// �������� Quest ���
BOOL	OnlineClient::SendRequestQuestCancelQuest(SI32 siQuestCode)
{
	OnReqQuestCancelQuest		TempOnReqQuestCancelQuest;

	ZeroMemory(&TempOnReqQuestCancelQuest, sizeof(TempOnReqQuestCancelQuest));
	TempOnReqQuestCancelQuest.dwMsgType   = ON_REQUEST_QUEST_CANCELQUEST;
	TempOnReqQuestCancelQuest.m_QuestCode = siQuestCode;

	return Write(&TempOnReqQuestCancelQuest, sizeof(TempOnReqQuestCancelQuest));
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������
// �������� Quest List ��û
BOOL	OnlineClient::SendRequestQuestGetPlayQuestList(void)
{
	OnReqQuestGetPlayQuest		TempOnReqQuestGetPlayQuest;

	ZeroMemory(&TempOnReqQuestGetPlayQuest, sizeof(TempOnReqQuestGetPlayQuest));
	TempOnReqQuestGetPlayQuest.dwMsgType   = ON_REQUEST_QUEST_GETPLAYQUEST;

	return Write(&TempOnReqQuestGetPlayQuest, sizeof(TempOnReqQuestGetPlayQuest));
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������
// ������� ���� ����
BOOL	OnlineClient::SendRequestQuestGetBankBookCount(void)
{
	OnReqBankBankBookListCountMsg	TempOnReqBankBankBookListCountMsg;

	ZeroMemory(&TempOnReqBankBankBookListCountMsg, sizeof(OnReqBankBankBookListCountMsg));
	TempOnReqBankBankBookListCountMsg.dwMsgType   = ON_REQUEST_BANK_BANKBOOK_LISTCOUNT;

	return Write(&TempOnReqBankBankBookListCountMsg, sizeof(OnReqBankBankBookListCountMsg));
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������
// �÷��̾��� ������ ��û�Ѵ�.
BOOL	OnlineClient::SendPlayerDetailInfo(UI16 uiID)
{
	OnReqCharDetailInfoMsg		TempOnReqCharDetailInfoMsg;

	TempOnReqCharDetailInfoMsg.dwMsgType = ON_REQUEST_CHAR_DETAILINFO;
	TempOnReqCharDetailInfoMsg.uiAccount = uiID;

	return Write(&TempOnReqCharDetailInfoMsg, sizeof(OnReqCharDetailInfoMsg));	
}

/*
//��������������������������������������������������������������������������������������������������������������������������������������������������������������
// ��ų����� ��û�Ѵ�.
BOOL	OnlineClient::SendRequestUseSkill(SI16 siSkillKind, SI16 siWhatDo)
{
	SKILL::sRequestUse		TempsRequestUse;

	TempsRequestUse.skillKind	= siSkillKind;
	TempsRequestUse.whatDo		= siWhatDo;

	return Write(&TempsRequestUse, sizeof(SKILL::sRequestUse));	
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������
// ��ų����� �ø���.
BOOL	OnlineClient::SendRequestLevelUp(SI16 siSkillKind)
{
	SKILL::sRequestLevelUp		TempsRequestLevelUp;

	TempsRequestLevelUp.skillKind	= siSkillKind;

	return Write(&TempsRequestLevelUp, sizeof(SKILL::sRequestLevelUp));	
}
*/

//��������������������������������������������������������������������������������������������������������������������������������������������������������������
// ����Ž�縦 ��û�Ѵ�.
BOOL	OnlineClient::SendRequestProbeArea()
{
	BURIED::sRequestProbeArea		TempsRequestProbeArea;
	TempsRequestProbeArea.command	=	BURIED::REQUEST_PROBE_AREA;

	return Write(&TempsRequestProbeArea, sizeof(BURIED::sRequestProbeArea));	
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������
// ���������� ��û�Ѵ�.
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

//��������������������������������������������������������������������������������������������������������������������������������������������������������������
// ������ ������ ����Ʈ�� ��û�Ѵ�.(�ָ�)
BOOL	OnlineClient::SendRequestInnItemList()
{
	OnReqItemListMsg		TempOnReqItemListMsg;

	ZeroMemory(&TempOnReqItemListMsg, sizeof(OnReqItemListMsg));
	TempOnReqItemListMsg.dwMsgType     = ON_REQUEST_ITEMLIST;

	return Write(&TempOnReqItemListMsg, sizeof(OnReqItemListMsg));
}
	
//��������������������������������������������������������������������������������������������������������������������������������������������������������������
// ������ �������� �κ��丮�� �ִ´�.(�ָ�)
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

//��������������������������������������������������������������������������������������������������������������������������������������������������������������
// �������� �ָ� �κ��丮�� �ִ´�.(�ָ�)
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

//��������������������������������������������������������������������������������������������������������������������������������������������������������������
// ��������� ��û�Ѵ�.
BOOL	OnlineClient::SendRequestGuildInfo(CHAR *pszName)
{
	OnReqPersonalInfo		TempOnReqPersonalInfo;

	ZeroMemory(&TempOnReqPersonalInfo, sizeof(OnReqPersonalInfo));
	TempOnReqPersonalInfo.dwMsgType			= ON_REQUEST_PERSONAL_INFO;
	strncpy(TempOnReqPersonalInfo.szID, pszName, ON_GUILDNAME_LENGTH);

	return Write(&TempOnReqPersonalInfo, sizeof(OnReqPersonalInfo));
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������
// ����ٴϴ� �뺴�� �����Ѵ�. 
BOOL	OnlineClient::SendRequestDeleteVMercenary(void)
{
	OnRequest_VMercenary_Delete		TempOnRequest_VMercenary_Delete;

	ZeroMemory(&TempOnRequest_VMercenary_Delete, sizeof(OnRequest_VMercenary_Delete));
	TempOnRequest_VMercenary_Delete.dwMsgType       = ON_REQUEST_VMERCENARY_DELETE;

	return Write(&TempOnRequest_VMercenary_Delete, sizeof(OnRequest_VMercenary_Delete));
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������
// ������ ������ ��û�Ѵ�.
BOOL	OnlineClient::SendRequestRefineItem(UI16 uiFollowerSlot, UI16 uiItemID)
{
	OnRequestRefineItem		TempOnRequestRefineItem;

	ZeroMemory(&TempOnRequestRefineItem, sizeof(OnRequestRefineItem));
	TempOnRequestRefineItem.dwMsgType		= ON_REQUEST_REFINE_ITEM;
	TempOnRequestRefineItem.uiFollowerSlot	= uiFollowerSlot;
	TempOnRequestRefineItem.uiItemID		= uiItemID;

	return Write(&TempOnRequestRefineItem, sizeof(OnRequestRefineItem));
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������
// ��ܰ��� ����� ��û�Ѵ�.
BOOL	OnlineClient::SendRequestGuildCreateCost()
{
	OnRequestGuildCreateCost		TempOnRequestGuildCreateCost;

	ZeroMemory(&TempOnRequestGuildCreateCost, sizeof(OnRequestGuildCreateCost));
	TempOnRequestGuildCreateCost.dwMsgType		= ON_REQUEST_GUILD_CREATE_COST;

	return Write(&TempOnRequestGuildCreateCost, sizeof(OnRequestGuildCreateCost));
}



/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
/// �α��ΰ��� �۾��Ѱ�

//----------------------------------------------------------------------------------------------------------------
//	����	:	Ŭ���̾�Ʈ�� ������ ������.
//----------------------------------------------------------------------------------------------------------------
BOOL	OnlineClient::SendClientKind()
{
	FRONT_USER::sRequestCheckVersion		ClientInfoMsg;

	ClientInfoMsg.command	=	FRONT_USER::REQUEST_CHECK_VERSION;
	ClientInfoMsg.version	=	ON_CLINETVERSION;	
	
	return	Write(&ClientInfoMsg, sizeof(FRONT_USER::sRequestCheckVersion));	
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	�˸������� ��û�Ѵ�.
//��������������������������������������������������������������������������������������������������������������������������������������������������������������
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
//	����	:	������ ���� ������ ������.
//----------------------------------------------------------------------------------------------------------------
BOOL	OnlineClient::SendWorldID(BYTE BWorldID)
{
	FRONT_USER::sRequestSelectWorld		SelectWorldMsg;

	SelectWorldMsg.command	=	FRONT_USER::REQUEST_SELECT_WORLD;
	SelectWorldMsg.world_id	=	BWorldID;
	
	return	Write(&SelectWorldMsg, sizeof(FRONT_USER::sRequestSelectWorld));	
}

//----------------------------------------------------------------------------------------------------------------
//	����	:	�α����� ������ ������.
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

//��������������������������������������������������������������������������������������������������������������������������������������������������������������
// ĳ���� ������ ��û�Ѵ�.
BOOL	OnlineClient::SendRequestCharInfo()
{
	LOGIN::sRequestCharsInfo		TempRequestCharsInfoMsg;

	TempRequestCharsInfoMsg.command		=	LOGIN::REQUEST_CHARS_INFO;

	return Write(&TempRequestCharsInfoMsg, sizeof(LOGIN::sRequestCharsInfo), TRUE);	
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	ĳ���͸� �����Ѵ�.
//��������������������������������������������������������������������������������������������������������������������������������������������������������������
BOOL	OnlineClient::SendDeleteCharacter(UI08 uiSlot)
{
	LOGIN::sRequestDelChar		TempRequestDelChar;

	TempRequestDelChar.command	=	LOGIN::REQUEST_DEL_CAHR;
	TempRequestDelChar.delSlot	=	uiSlot;

	return Write(&TempRequestDelChar, sizeof(LOGIN::sRequestDelChar), TRUE);
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	ĳ���͸� �����Ѵٰ� ������ �˷��ش�.
//��������������������������������������������������������������������������������������������������������������������������������������������������������������
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
//	����	:	���Ӽ����� �α����Ѵ�.
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
//	����	:	�����, �ൿ���� ���� ������ �Ӹ��Ѵ�.
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
//	����	:	�����, �ൿ���� ���� ���޿��� �����Ѵ�.
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
//	����	:	�α׾ƿ��� ������ �˸���.
//----------------------------------------------------------------------------------------------------------------
BOOL	OnlineClient::SendLogOut()
{
	OnReqGameServerLogOutMsg	TempOnReqGameServerLogOutMsg;
	ZeroMemory(&TempOnReqGameServerLogOutMsg, sizeof(OnReqGameServerLogOutMsg));
	TempOnReqGameServerLogOutMsg.dwMsgType = ON_REQUEST_LOGOUT;

	return Write(&TempOnReqGameServerLogOutMsg, sizeof(OnReqGameServerLogOutMsg));
}

//----------------------------------------------------------------------------------------------------------------
//	����	:	ģ������Ʈ ��û
//----------------------------------------------------------------------------------------------------------------
BOOL	OnlineClient::SendReqFriendList()
{
	OnReqAuditFriendList	TempOnReqAuditFriendList;
	ZeroMemory(&TempOnReqAuditFriendList, sizeof(OnReqAuditFriendList));
	TempOnReqAuditFriendList.dwMsgType = ON_REQUEST_AUDIT_FRIEND_LIST;

	return Write(&TempOnReqAuditFriendList, sizeof(OnReqAuditFriendList));
}

//----------------------------------------------------------------------------------------------------------------
//	����	:	ģ�� �߰�
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
//	����	:	ģ�� ����
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
//	����	:	��� ģ�� ����
//----------------------------------------------------------------------------------------------------------------
BOOL	OnlineClient::SendReqDeleteAllFriend()
{
	OnReqAuditFriendRemoveAll	TempOnReqAuditFriendRemoveAll;
	ZeroMemory(&TempOnReqAuditFriendRemoveAll, sizeof(OnReqAuditFriendRemoveAll));
	TempOnReqAuditFriendRemoveAll.dwMsgType = ON_REQUEST_AUDIT_FRIEND_REMOVE_ALL;

	return Write(&TempOnReqAuditFriendRemoveAll, sizeof(OnReqAuditFriendRemoveAll));
}

//----------------------------------------------------------------------------------------------------------------
//	����	:	���Űź� ����
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
// ������ ���� ���� �������� ����ǰ�� ��û�Ѵ�.
// UI16 uiUniqueID_SupplyWagon:	������ ���޼��� ������ Unique ID
// SI16 ssNeedsSupplyGoodsAmount: �ʿ��� ����ǰ ��
BOOL	OnlineClient::SendSiegeWarfareSupplyGoodsFromSupplyWagon(UI16 uiUniqueID_SupplyWagon, SI16 ssNeedsSupplyGoodsAmount)
{
	OnReqGetSupplyAtGuildUnit	SDataOnReqGetSupplyAtGuildUnit;

	ZeroMemory(&SDataOnReqGetSupplyAtGuildUnit, sizeof(OnReqGetSupplyAtGuildUnit));

	SDataOnReqGetSupplyAtGuildUnit.dwMsgType		= ON_REQUEST_GET_SUPPLY_AT_GUILD_UNIT;
	SDataOnReqGetSupplyAtGuildUnit.siRequireSupply	= ssNeedsSupplyGoodsAmount;				// �ʿ��� ��
	SDataOnReqGetSupplyAtGuildUnit.uiTargetID		= uiUniqueID_SupplyWagon;				// ���޼��� UniqueID

	return Write(&SDataOnReqGetSupplyAtGuildUnit, sizeof(SDataOnReqGetSupplyAtGuildUnit));
}

// robypark 2004/12/2 10:48
// ������ ���� �Ŀ� ������ ���� ����Ʈ�� ��û�Ѵ�.
BOOL	OnlineClient::SendRequest_guild_villageattack_result(void)
{
	OnReqVillageAttackResult		TempOnReqVillageAttackResult;

	ZeroMemory(&TempOnReqVillageAttackResult, sizeof(OnReqVillageAttackResult));

	TempOnReqVillageAttackResult.dwMsgType		= ON_REQUEST_GUILD_VILLAGEATTACK_RESULT;

	return Write(&TempOnReqVillageAttackResult, sizeof(TempOnReqVillageAttackResult));
}

// robypark 2004/12/2 13:41
// ������ ���� �Ŀ� ������ ���� ���� ������ Ư�� ĳ���Ϳ� �ֱ� ��û
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
// ����ڰ� �� �ִ� ������ ��ź ���� ������ �ִ��� ��û�Ѵ�.
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
// ������ �߿� �Ϲ� ���� �� ������ Ư�� ��ġ(�ʺ��� ������)�� ���� ��û�ϱ�
// �� ��û ������ ������ ��� ��ɾ��� /w(����)�� ���� ����Ѵ�.(ON_RESPONSE_WARP)
BOOL	OnlineClient::SendRequestSiegeWarfareWarp(UI16 uiUniqueID)
{
	OnReqWarWarp					TempOnReqWarWarp;

	TempOnReqWarWarp.dwMsgType		=	ON_REQUEST_WAR_WARP;
	TempOnReqWarWarp.uiUniqueID		=	uiUniqueID;				// �����ؼ� ������ �Ϲ����� ���̵�.

	return Write(&TempOnReqWarWarp, sizeof(TempOnReqWarWarp));
}

// robypark 2005/1/5 17:4
// ������ ������ ���¿��� �Ϲ� �������� ���ư��ڴٰ� ������ ��û
BOOL	OnlineClient::SendRequestNormalCharacter(UI16 uiUniqueID)
{
	OnReqNormalChar					TempOnReqNormalChar;

	TempOnReqNormalChar.dwMsgType	=	ON_REQUEST_NORMAL_CHAR;
	TempOnReqNormalChar.uiUniqueID	=	uiUniqueID;

	return Write(&TempOnReqNormalChar, sizeof(TempOnReqNormalChar));
}