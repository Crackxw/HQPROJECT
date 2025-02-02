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
#include "OnlineSMS.h"		// actdoll (2004/06/15 10:46) : SMS 辨 等檜攪 蹺陛
#include "OnlinePannel.h"


//----------------------------------------------------------------------------------------------------------------
//	 撲貲	:	頂 Ы溯檜橫曖 嬪纂蒂 爾魚棻.
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
//	 撲貲	:	頂 Ы溯檜橫曖 嬪纂蒂 爾魚棻.
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
//	撲貲	:	頂陛 檜翕п撿 ж朝 寞щ擊 爾頂遽棻.
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
//	撲貲	:	頂陛 檜翕п撿 ж朝 謝ル蒂 爾頂遽棻.
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
//	撲貲	:	
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
//	撲貲	:	頂陛 顫檜ёи 詭衛雖蒂 爾魚棻.
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
//	撲貲	:	頂陛 葆擊縑 菟橫除棻朝 詭衛雖蒂 憮幗縑啪 憲溥遽棻.
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
BOOL	OnlineClient::SendGotoVillage(UI16 uiGotoVillageCode)
{
	OnReqGotoVillageMsg		GotoVillageMsg;

	GotoVillageMsg.dwMsgType		=	ON_REQUEST_ENTER_VILLAGE;
	GotoVillageMsg.uiVillageCode	=	uiGotoVillageCode;	
	
	return Write(&GotoVillageMsg, sizeof(OnReqGotoVillageMsg));	
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
//	撲貲	:	葆擊擊 釭除棻.
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
BOOL	OnlineClient::SendGoOutVillage()
{
	OnReqGoOutVillageMsg		ReqGoOutVillageMsg;

	ReqGoOutVillageMsg.dwMsgType	=	ON_REQUEST_LEAVE_VILLAGE;	

	return Write(&ReqGoOutVillageMsg, sizeof(OnReqGoOutVillageMsg));
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
//	撲貲	:	嬴檜蠱曖 葬蝶お蒂 殖塭堅 蹂掘и棻.
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
BOOL	OnlineClient::SendRequestItemList()
{
	OnReqItemListInMarketMsg	ReqItemListInMarketMsg;

	ReqItemListInMarketMsg.dwMsgType	=	ON_REQUEST_LIST_ITEM;	

	return Write(&ReqItemListInMarketMsg, sizeof(OnReqItemListInMarketMsg));
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
//	撲貲	:	嬴檜蠱擊 骯棻堅 憮幗縑 蹂羶и棻.
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
BOOL	OnlineClient::SendBuyItem(OnReqBuyItemMsg* lpBuyItem)
{
	return Write(lpBuyItem, sizeof(OnReqBuyItemMsg));	
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
//	撲貲	:	嬴檜蠱擊 っ棻堅 憮幗縑 蹂羶и棻.
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
//	撲貲	:	棻艇 Ы溯檜橫諦 瞪癱蒂 и棻堅 憮幗縑啪 憲溥遽棻.
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
BOOL	OnlineClient::SendBattle(SI32 siID)
{
	OnReqBattleMsg		BattleMsg;

	BattleMsg.dwMsgType	=	ON_REQUEST_BATTLE;
	BattleMsg.uiCharID	=	UI16(siID);

	return Write(&BattleMsg, sizeof(BattleMsg));	
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	檣漸饜葬頂縑憮曖 頂 嬴檜蠱曖 嬪纂蒂 憮幗縑啪 憲溥遽棻.
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
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


//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	嬴檜蠱擊 幗萼棻.
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
BOOL	OnlineClient::SendThrowItem(OnReqThrowItemMsg* lpOnReqThrowItemMsg)
{
	return Write(lpOnReqThrowItemMsg, sizeof(OnReqThrowItemMsg));	
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	階縑 雲橫螳 氈朝 嬴檜蠱擊 輿歷棻.
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
BOOL	OnlineClient::SendPickUpItem(OnReqPickUpItemMsg* lpOnlineTempOnReqPickUpItemMsg)
{
	return Write(lpOnlineTempOnReqPickUpItemMsg, sizeof(OnReqPickUpItemMsg));	
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	棻衛 螞塭檣戲煎 給嬴陛溥堅 и棻.
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
BOOL	OnlineClient::SendReturnOnline(BOOL bWin)
{
	OnReqReturnOnlineMsg	ReturnOnlineMsg;	

//	DWriteStringToFile("SendReturnOnline() 詭撮雖 爾鹵");
	ReturnOnlineMsg.dwMsgType	=	ON_REQUEST_RETURNONLINE;
	ReturnOnlineMsg.dwBattleResult = bWin;

	return Write(&ReturnOnlineMsg, sizeof(OnReqReturnOnlineMsg));
}



//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	議葛攪曖 薑爾蒂 殖塭堅 撲薑и棻.
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
BOOL	OnlineClient::SendRequestPlayerInfo(UI16 uiCharNum, UI16 *puiIDIn)
{
	CHAR						cLocalBuffer[ON_MAX_LOCALBUFFER];
	OnReqPlayerInfoMsg			*pReqPlayerInfoMsg;
	UI16						*puiIDOut;
	SI32						siLength;

//	SetMessage("議葛攪 薑爾 蹂羶");

	pReqPlayerInfoMsg				=	(OnReqPlayerInfoMsg*)cLocalBuffer;

	pReqPlayerInfoMsg->dwMsgType		=	ON_REQUEST_PLAYER_INFO;
	pReqPlayerInfoMsg->uiCharNum		=	uiCharNum;
	puiIDOut							=	(UI16*)((CHAR*)pReqPlayerInfoMsg + sizeof(OnReqPlayerInfoMsg));
	siLength							=	sizeof(OnReqPlayerInfoMsg) + (sizeof(UI16) * pReqPlayerInfoMsg->uiCharNum);
	
	memcpy(puiIDOut, puiIDIn, sizeof(UI16) * pReqPlayerInfoMsg->uiCharNum);

	return Write(pReqPlayerInfoMsg, siLength);
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	葆擊, 衛濰縑 菟橫除棻.
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
BOOL	OnlineClient::SendRequestEnterMarket()
{
	OnReqEnterMarketMsg	EnterMarketMsg;

	EnterMarketMsg.dwMsgType	=	ON_REQUEST_ENTER_MARKET;
	
	return Write(&EnterMarketMsg, sizeof(EnterMarketMsg));
}
	
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	葆擊, 衛濰縑憮 釭除棻.
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
BOOL	OnlineClient::SendRequestLeaveMarket()
{	
	OnReqLeaveMarketMsg	LeaveMarketMsg;

	LeaveMarketMsg.dwMsgType	=	ON_REQUEST_LEAVE_MARKET;

	return Write(&LeaveMarketMsg, sizeof(LeaveMarketMsg));
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	瞪濰 薑爾蒂 蹂羶
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
BOOL	OnlineClient::SendRequestBankInfo()
{
	OnReqBankInfoMsg	TempOnReqBankInfoMsg;

	TempOnReqBankInfoMsg.dwMsgType	=	ON_REQUEST_BANK_INFO;	

	return Write( &TempOnReqBankInfoMsg, sizeof( OnReqBankInfoMsg ) );

}
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	啗謝 偃撲 蹂羶
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
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

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	鱔濰 葬蝶お 蹂羶
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
BOOL	OnlineClient::SendRequestMySavingAccount(void)
{	
	OnReqBankBankBookListMsg			TempOnReqBankBankBookList;

	TempOnReqBankBankBookList.dwMsgType = ON_REQUEST_BANK_BANKBOOK_LIST;

	return Write(&TempOnReqBankBankBookList, sizeof(OnReqBankBankBookListMsg));
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	殮旎
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
BOOL	OnlineClient::SendRequestSavingAccount(OnAccountNumber AccountNumber, MONEY Money)
{
	OnReqBankDepositMsg		TempOnReqBankDepositMsg;

	TempOnReqBankDepositMsg.dwMsgType     = ON_REQUEST_BANK_DEPOSIT;
	TempOnReqBankDepositMsg.AccountNumber = AccountNumber;
	TempOnReqBankDepositMsg.siMoney       = Money;

	return Write(&TempOnReqBankDepositMsg, sizeof(OnReqBankDepositMsg));
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	轎旎
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
BOOL	OnlineClient::SendRequestDrawingAccount(OnAccountNumber AccountNumber, MONEY Money)
{
	OnReqBankDepositMsg		TempOnReqBankDepositMsg;

	TempOnReqBankDepositMsg.dwMsgType     = ON_REQUEST_BANK_DRAWING;
	TempOnReqBankDepositMsg.AccountNumber = AccountNumber;
	TempOnReqBankDepositMsg.siMoney       = Money;

	return Write(&TempOnReqBankDepositMsg, sizeof(OnReqBankDepositMsg));
}
	
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	葆擊, 瞪濰縑憮 氬爾渠轎
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
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

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	葆擊, 瞪濰縑憮 氬爾僭 葬蝶お蒂 嫡朝棻.
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
BOOL	OnlineClient::SendCollateralListMsg()
{
	OnReqCollateralListMsg		TempOnReqCollateralListMsg;

	TempOnReqCollateralListMsg.dwMsgType	=	ON_REQUEST_BANK_COLLATERALLIST;

	return Write( &TempOnReqCollateralListMsg, sizeof( OnReqCollateralListMsg ) );
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	癱濠旎 薑爾
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
BOOL OnlineClient::SendReqAuditVillageInfo()
{
	OnReqAuditVillageList		TempReqAuditVillageList;

	TempReqAuditVillageList.dwMsgType = ON_REQUEST_AUDIT_VILLAGE_LIST;

	return Write(  &TempReqAuditVillageList,sizeof(OnReqAuditVillageList));
}
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	葆擊, 瞪濰縑憮 氬爾蒂 瓊朝棻.
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
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

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	Message 瞪歎
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
BOOL	OnlineClient::SendMessage(char* lpMsg, SI16 Size)
{
	return Write(lpMsg, Size);
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	葆擊, 輿薄縑 菟橫除棻.
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
BOOL	OnlineClient::SendRequestEnterInn()
{
	OnReqEnterInnMsg	TempOnReqEnterInnMsg;

	TempOnReqEnterInnMsg.dwMsgType		= 	ON_REQUEST_ENTER_INN;

	return Write(&TempOnReqEnterInnMsg, sizeof(OnReqEnterInnMsg));
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	葆擊, 輿薄縑憮 釭除棻.
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
BOOL	OnlineClient::SendRequestLeaveInn()
{
	ReqLeaveInnMsg		TempReqLeaveInnMsg;

	TempReqLeaveInnMsg.dwMsgType		=	ON_REQUEST_LEAVE_INN;

	return Write(&TempReqLeaveInnMsg, sizeof(ReqLeaveInnMsg));
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	葆擊, 輿薄縑 渠�亳Й� 菟橫除棻.
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
BOOL	OnlineClient::SendRequestEnterInnVWR()
{
	OnReqEnterWaitRoomMsg	TempOnReqEnterWaitRoomMsg;

	TempOnReqEnterWaitRoomMsg.dwMsgType		= 	ON_REQUEST_ENTER_VWR;

	return Write(&TempOnReqEnterWaitRoomMsg, sizeof(OnReqEnterWaitRoomMsg));
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	葆擊, 輿薄縑 渠�亳Ч□� 渠�食瞈� 霤罹и棻.
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
BOOL	OnlineClient::SendRequestJoinVilInnChatRoom(UI16 ChattingRoomID)
{
	OnReqJoinVilInnChatRoomMsg	ReqJoinVilInnChatRoomMsg;

	ReqJoinVilInnChatRoomMsg.dwMsgType	  = ON_REQUEST_VIL_INN_JOIN_CHAT_ROOM;
	ReqJoinVilInnChatRoomMsg.uiChatRoomID = ChattingRoomID;

	return Write(&ReqJoinVilInnChatRoomMsg, sizeof(OnReqJoinVilInnChatRoomMsg));
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	葆擊, 輿薄縑 渠�亳Ч□� 渠�食瞈□� 釭除棻.
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
BOOL	OnlineClient::SendRequestLeaveVilInnChatRoom()
{
	OnReqLeaveVilInnChatRoomMsg	ReqLeaveVilInnChatRoomMsg;

	ReqLeaveVilInnChatRoomMsg.dwMsgType	=	ON_REQUEST_VIL_INN_LEAVE_CHAT_ROOM;

	return Write(&ReqLeaveVilInnChatRoomMsg, sizeof(ReqLeaveVilInnChatRoomMsg));
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	葆擊, 輿薄縑 渠�亳Ч□� 瓣た寞 儅撩
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
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

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	葆擊, 輿薄縑 啪衛っ縑 菟橫馬
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
BOOL	OnlineClient::SendRequestEnterInnNoticeBoard(DWORD Kind)
{
	OnReqEnterNoticeBoardMsg	TempOnReqEnterNoticeBoardMsg;

	TempOnReqEnterNoticeBoardMsg.dwMsgType			=	ON_REQUEST_ENTER_NOTICEBOARD;
	
	return Write(&TempOnReqEnterNoticeBoardMsg, sizeof(OnReqEnterNoticeBoardMsg));
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	啗衛っ縑憮 旋擊 雖遴棻.
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
BOOL	OnlineClient::SendRequestDeleteNotice( DWORD dwPage, UI16 uiIndex )
{
	OnReqDeleteNoticeMsg		TempOnReqDeleteNoticeMsg;

	TempOnReqDeleteNoticeMsg.dwMsgType		=	ON_REQUEST_DELETE_NOTICE;
	TempOnReqDeleteNoticeMsg.dwPageNum		=	dwPage;
	TempOnReqDeleteNoticeMsg.uiNoticeIndex	=	uiIndex;

	return Write( &TempOnReqDeleteNoticeMsg, sizeof( OnReqDeleteNoticeMsg ) );	
}


//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	啪衛っ縑憮曖 匐儀
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
BOOL	OnlineClient::SendRequestSearchNotice( UI08	uiType, char *pMsg )
{
/*
	UI08						TempBuffer[1024];
	OnReqSearchNoticeMsg		TempOnReqSearchNoticeMsg;

	TempOnReqSearchNoticeMsg.dwMsgType		=	ON_REQUEST_SEARCH_NOTICE;
	TempOnReqSearchNoticeMsg.uiSearchKind	=	uiType;
	TempOnReqSearchNoticeMsg.uiStringLength	=	strlen( pMsg );


	memcpy( TempBuffer, &TempOnReqSearchNoticeMsg, sizeof( 	OnReqSearchNoticeMsg ) );			 // 試盪 掘褻羹蒂
	memcpy( &TempBuffer[ sizeof( 	OnReqSearchNoticeMsg ) ], pMsg, strlen( pMsg ) );			 // 棻擠縑 頂辨擊 稱罹憮 爾頂遽棻.

	return Write( TempBuffer, ( sizeof( OnReqSearchNoticeMsg ) + strlen( pMsg ) ) );
*/
	return FALSE;
}



//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	啪衛っ縑 旋擊 蹂羶 и棻.
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
BOOL	OnlineClient::SendRequestNoticesListMsg( UI16 uiPageID )
{
	OnReqNoticesListMsg			TempOnReqNoticesListMsg;

	TempOnReqNoticesListMsg.dwMsgType	=	ON_REQUEST_NOTICEBOARD_NOTICELIST;
	TempOnReqNoticesListMsg.uiPage		=	uiPageID;

	return Write( &TempOnReqNoticesListMsg, sizeof( OnReqNoticesListMsg ) );
}


//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	啪衛っ擊 檗朝棻.
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
BOOL	OnlineClient::SendRequestViewNotice( UI16	uiIndex )
{
	OnReqViewNoticeMsg		TempOnReqViewNoticeMsg;

	TempOnReqViewNoticeMsg.dwMsgType		=	ON_REQUEST_VIEW_NOTICE;
	TempOnReqViewNoticeMsg.uiNoticeIndex	=	uiIndex;

	return Write( &TempOnReqViewNoticeMsg, sizeof( OnReqViewNoticeMsg ) );
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	頂陛 Portal縑 菟橫除棻朝 詭衛雖蒂 憮幗縑啪 憲溥遽棻.
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
BOOL	OnlineClient::SendGotoPortal(SI16 siID)
{
	OnReqMoveMapMsg				TempOnReqMoveMapMsg;

	TempOnReqMoveMapMsg.dwMsgType  = ON_REQUEST_MOVEMAP;
	TempOnReqMoveMapMsg.uiPortalID = siID;

	return Write(&TempOnReqMoveMapMsg, sizeof(OnReqMoveMapMsg));
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	�こ羹瓵� 菟橫除棻.
BOOL	OnlineClient::SendRequestBarrackEnter(void)
{
	OnReqBarrackGetList		TempOnReqBarrackGetList;

	TempOnReqBarrackGetList.dwMsgType = ON_REQUEST_BARRACK_GETSOLDERLIST;

	return Write(&TempOnReqBarrackGetList, sizeof(OnReqBarrackGetList));
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	�こ羹瓵□� 辨煽擊 堅辨и棻.
BOOL	OnlineClient::SendRequestBarrackBuySolder(SI32 Code, BYTE* lpName)
{
	OnReqBarrackBuySolder	TempOnReqBarrackBuySolder;

	ZeroMemory(&TempOnReqBarrackBuySolder, sizeof(OnReqBarrackBuySolder));
	TempOnReqBarrackBuySolder.dwMsgType     = ON_REQUEST_BARRACK_BUYSOLDER;
	TempOnReqBarrackBuySolder.dwBuySolderID = Code;
	strcpy((char*)TempOnReqBarrackBuySolder.szName, (char*)lpName);

	return Write(&TempOnReqBarrackBuySolder, sizeof(OnReqBarrackBuySolder));
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	�こ羹� п堅縑 菟橫除棻.
BOOL	OnlineClient::SendRequestBarrackDismissalEnter(void)
{
	OnReqBarrackGetDismissalList		TempOnReqBarrackGetDismissalList;

	TempOnReqBarrackGetDismissalList.dwMsgType = ON_REQUEST_BARRACK_GETDISMISSALSOLDERLIST;

	return Write(&TempOnReqBarrackGetDismissalList, sizeof(OnReqBarrackGetDismissalList));
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	�こ羹瓵□� 辨煽擊 п堅и棻.
BOOL	OnlineClient::SendRequestBarrackDismissalSolder(SI32 Slot)
{
	OnReqBarrackDismissalSolder	TempOnReqBarrackDismissalSolder;

	ZeroMemory(&TempOnReqBarrackDismissalSolder, sizeof(OnReqBarrackDismissalSolder));
	TempOnReqBarrackDismissalSolder.dwMsgType = ON_REQUEST_BARRACK_DISMISSALSOLDER;
	TempOnReqBarrackDismissalSolder.dwSlot    = Slot;

	return Write(&TempOnReqBarrackDismissalSolder, sizeof(OnReqBarrackDismissalSolder));
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	濰熱曄撩模縑 菟橫除棻.
BOOL	OnlineClient::SendRequestGBarrackEnter(void)
{
	OnReqGBarrackGetList		TempOnReqGBarrackGetList;

	ZeroMemory(&TempOnReqGBarrackGetList, sizeof(OnReqGBarrackGetList));
	TempOnReqGBarrackGetList.dwMsgType = ON_REQUEST_GBARRACK_GETSOLDERLIST;
	
	return Write(&TempOnReqGBarrackGetList, sizeof(OnReqGBarrackGetList));
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	濰熱曄撩模縑憮 濰熱擊 堅辨и棻.
BOOL	OnlineClient::SendRequestGBarrackBuySolder(SI32 Code, BYTE* lpName)
{
	OnReqGBarrackBuySolder	TempOnReqGBarrackBuySolder;

	ZeroMemory(&TempOnReqGBarrackBuySolder, sizeof(OnReqGBarrackBuySolder));
	TempOnReqGBarrackBuySolder.dwMsgType     = ON_REQUEST_GBARRACK_BUYSOLDER;
	TempOnReqGBarrackBuySolder.dwBuySolderID = Code;
	strcpy((char*)TempOnReqGBarrackBuySolder.szName, (char*)lpName);

	return Write(&TempOnReqGBarrackBuySolder, sizeof(OnReqGBarrackBuySolder));
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	濰熱曄撩模 瞪霜縑 菟橫除棻.
BOOL	OnlineClient::SendRequestGBarrackChangeJobEnter(void)
{
	OnReqGBarrackGetChangeJobList		TempOnReqGBarrackGetChangeJobList;

	ZeroMemory(&TempOnReqGBarrackGetChangeJobList, sizeof(OnReqGBarrackGetChangeJobList));
	TempOnReqGBarrackGetChangeJobList.dwMsgType = ON_REQUEST_GBARRACK_GETCHANGEJOBSOLDERLIST;
	
	return Write(&TempOnReqGBarrackGetChangeJobList, sizeof(OnReqGBarrackGetChangeJobList));
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	濰熱曄撩模縑憮 濰熱擊 瞪霜и棻.
BOOL	OnlineClient::SendRequestGBarrackChangeJobSolder(SI32 Slot, SI32 Kind)
{
	OnReqGBarrackChangeJobSolder	TempOnReqGBarrackChangeJobSolder;

	ZeroMemory(&TempOnReqGBarrackChangeJobSolder, sizeof(OnReqGBarrackChangeJobSolder));
	TempOnReqGBarrackChangeJobSolder.dwMsgType       = ON_REQUEST_GBARRACK_CHANGEJOBSOLDER;
	TempOnReqGBarrackChangeJobSolder.dwChangeJobSlot = Slot;
	TempOnReqGBarrackChangeJobSolder.dwChangeJobKind = Kind;

	return Write(&TempOnReqGBarrackChangeJobSolder, sizeof(OnReqGBarrackChangeJobSolder));
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	濰熱曄撩模 п堅縑 菟橫除棻.
BOOL	OnlineClient::SendRequestGBarrackDismissalEnter(void)
{
	OnReqGBarrackGetDismissalList		TempOnReqGBarrackGetDismissalList;

	ZeroMemory(&TempOnReqGBarrackGetDismissalList, sizeof(OnReqGBarrackGetDismissalList));
	TempOnReqGBarrackGetDismissalList.dwMsgType = ON_REQUEST_GBARRACK_GETDISMISSALSOLDERLIST;
	
	return Write(&TempOnReqGBarrackGetDismissalList, sizeof(OnReqGBarrackGetDismissalList));
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	濰熱曄撩模縑憮 濰熱擊 п堅и棻.
BOOL	OnlineClient::SendRequestGBarrackDismissalSolder(SI32 Slot)
{
	OnReqGBarrackDismissalSolder	TempOnReqGBarrackDismissalSolder;

	ZeroMemory(&TempOnReqGBarrackDismissalSolder, sizeof(OnReqGBarrackDismissalSolder));
	TempOnReqGBarrackDismissalSolder.dwMsgType = ON_REQUEST_GBARRACK_DISMISSALSOLDER;
	TempOnReqGBarrackDismissalSolder.dwSlot    = Slot;

	return Write(&TempOnReqGBarrackDismissalSolder, sizeof(OnReqGBarrackDismissalSolder));
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	2離 瞪霜 濰熱曄撩模縑 菟橫除棻.(幗が擊 援艇棻.)
BOOL	OnlineClient::SendRequestGBarrackGeneralExEnter(void)
{
	OnReqGBarrackGetList	TempOnReqGBarrackGetList;

	ZeroMemory(&TempOnReqGBarrackGetList,sizeof(OnReqGBarrackGetList));
	TempOnReqGBarrackGetList.dwMsgType = ON_REQUEST_GBARRACK_GET_GENERAL_LIST;

	return Write(&TempOnReqGBarrackGetList, sizeof(OnReqGBarrackGetList));;
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	2離 瞪霜 濰熱曄撩模縑憮 2離 濰熱蒂 堅辨и棻.
BOOL	OnlineClient::SendRequestGBarrackBuyGeneralEx(SI32 Code, BYTE* lpName)
{
	OnReqGBarrackBuySolder	TempOnReqGBarrackBuySolder;

	ZeroMemory(&TempOnReqGBarrackBuySolder, sizeof(OnReqGBarrackBuySolder));
	TempOnReqGBarrackBuySolder.dwMsgType     = ON_REQUEST_GBARRACK_BUY_GENERAL;
	TempOnReqGBarrackBuySolder.dwBuySolderID = Code;
	strcpy((char*)TempOnReqGBarrackBuySolder.szName, (char*)lpName);

	return Write(&TempOnReqGBarrackBuySolder, sizeof(OnReqGBarrackBuySolder));
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//  撲貲	:	2離 瞪霜 濰熱曄撩模縑憮 瞪霜縑 菟橫除棻.
BOOL	OnlineClient::SendRequestGBarrackChangeGeneralExEnter(void)
{
	OnReqGBarrackGetChangeJobList		TempOnReqGBarrackGetChangeJobList;

	ZeroMemory(&TempOnReqGBarrackGetChangeJobList, sizeof(OnReqGBarrackGetChangeJobList));
	TempOnReqGBarrackGetChangeJobList.dwMsgType = ON_REQUEST_GBARRACK_GET_CHANGE_GENERAL_LIST;
	
	return Write(&TempOnReqGBarrackGetChangeJobList, sizeof(OnReqGBarrackGetChangeJobList));
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//  撲貲	:	2離 瞪霜 濰熱曄撩模縑憮 瞪霜 濰熱蒂 堅辨и棻.
BOOL	OnlineClient::SendRequestGBarrackBuyChangeGeneralEx(SI32 Slot, SI32 Kind)
{
	OnReqGBarrackChangeJobSolder	TempOnReqGBarrackChangeJobSolder;

	ZeroMemory(&TempOnReqGBarrackChangeJobSolder, sizeof(OnReqGBarrackChangeJobSolder));
	TempOnReqGBarrackChangeJobSolder.dwMsgType       = ON_REQUEST_GBARRACK_CHANGE_GENERAL;
	TempOnReqGBarrackChangeJobSolder.dwChangeJobSlot = Slot;
	TempOnReqGBarrackChangeJobSolder.dwChangeJobKind = Kind;

	return Write(&TempOnReqGBarrackChangeJobSolder, sizeof(OnReqGBarrackChangeJobSolder));
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
// 2離 瞪霜 濰熱曄撩模縑憮 п堅縑 菟橫除棻.
BOOL	OnlineClient::SendRequestGBarrackDismissalGeneralExEnter(void)
{
	OnReqGBarrackGetDismissalList		TempOnReqGBarrackGetDismissalList;

	ZeroMemory(&TempOnReqGBarrackGetDismissalList, sizeof(OnReqGBarrackGetDismissalList));
	TempOnReqGBarrackGetDismissalList.dwMsgType = ON_REQUEST_GBARRACK_GETDISMISSAL_GENERAL_LIST;
	
	return Write(&TempOnReqGBarrackGetDismissalList, sizeof(OnReqGBarrackGetDismissalList));
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
// 2離 瞪霜 濰熱曄撩模縑憮 濰熱蒂 п堅и棻.
BOOL	OnlineClient::SendRequestGBarrackSellDismissalGeneralEx(SI32 Slot)
{
	OnReqGBarrackDismissalSolder	TempOnReqGBarrackDismissalSolder;

	ZeroMemory(&TempOnReqGBarrackDismissalSolder, sizeof(OnReqGBarrackDismissalSolder));
	TempOnReqGBarrackDismissalSolder.dwMsgType = ON_REQUEST_GBARRACK_DISMISSAL_GENERAL;
	TempOnReqGBarrackDismissalSolder.dwSlot    = Slot;

	return Write(&TempOnReqGBarrackDismissalSolder, sizeof(OnReqGBarrackDismissalSolder));
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	跨蝶攪曄撩模縑 菟橫除棻.
BOOL	OnlineClient::SendRequestMBarrackEnter(void)
{
	OnReqMBarrackGetList		TempOnReqMBarrackGetList;

	TempOnReqMBarrackGetList.dwMsgType = ON_REQUEST_MBARRACK_GETMONSTERLIST;

	return Write(&TempOnReqMBarrackGetList, sizeof(OnReqMBarrackGetList));
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	跨蝶攪曄撩模縑憮 辨煽擊 堅辨и棻.
BOOL	OnlineClient::SendRequestMBarrackBuySolder(SI32 Code, BYTE* lpName)
{
	OnReqMBarrackBuyMonster		TempOnReqMBarrackBuyMonster;

	ZeroMemory(&TempOnReqMBarrackBuyMonster, sizeof(OnReqMBarrackBuyMonster));
	TempOnReqMBarrackBuyMonster.dwMsgType      = ON_REQUEST_MBARRACK_BUYMONSTER;
	TempOnReqMBarrackBuyMonster.dwBuyMonsterID = Code;
	strcpy((char*)TempOnReqMBarrackBuyMonster.szName, (char*)lpName);

	return Write(&TempOnReqMBarrackBuyMonster, sizeof(OnReqMBarrackBuyMonster));
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	跨蝶攪曄撩模 п堅縑 菟橫除棻.
BOOL	OnlineClient::SendRequestMBarrackDismissalEnter(void)
{
	OnReqMBarrackGetDismissList		TempOnReqMBarrackGetDismissList;

	TempOnReqMBarrackGetDismissList.dwMsgType = ON_REQUEST_MBARRACK_GETDISMISSMONSTERLIST;

	return Write(&TempOnReqMBarrackGetDismissList, sizeof(OnReqMBarrackGetDismissList));
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	跨蝶攪曄撩模縑憮 辨煽擊 п堅и棻.
BOOL	OnlineClient::SendRequestMBarrackDismissalSolder(SI32 Slot)
{
	OnReqMBarrackDismissMonster		TempOnReqMBarrackDismissMonster;

	ZeroMemory(&TempOnReqMBarrackDismissMonster, sizeof(OnReqMBarrackDismissMonster));
	TempOnReqMBarrackDismissMonster.dwMsgType = ON_REQUEST_MBARRACK_DISMISSMONSTER;
	TempOnReqMBarrackDismissMonster.dwSlot    = Slot;

	return Write(&TempOnReqMBarrackDismissMonster, sizeof(OnReqMBarrackDismissMonster));
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	曖錳縑憮 纂猿 辨煽曖 List蒂 橢橫螞棻.
BOOL	OnlineClient::SendRequestHospitalGetCureList(void)
{
	OnReqHospitalGetCureList		TempOnReqHospitalGetCureList;

	TempOnReqHospitalGetCureList.dwMsgType = ON_REQUEST_HOSPITAL_GETCURELIST;

	return Write(&TempOnReqHospitalGetCureList, sizeof(OnReqHospitalGetCureList));
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	曖錳縑憮 辨煽擊 纂猿и棻.
BOOL	OnlineClient::SendRequestHospitalCureSolder(UI08 Slot)
{
	OnReqHospitalCureSolder		TempOnReqHospitalCureSolder;

	ZeroMemory(&TempOnReqHospitalCureSolder, sizeof(OnReqHospitalCureSolder));
	TempOnReqHospitalCureSolder.dwMsgType        = ON_REQUEST_HOSPITAL_CURESOLDER;
	TempOnReqHospitalCureSolder.dwCureSolderSlot = Slot;

	return Write(&TempOnReqHospitalCureSolder, sizeof(OnReqHospitalCureSolder));
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	曖錳縑憮 賅萇 辨煽擊 纂猿и棻.
BOOL	OnlineClient::SendRequestHospitalCureAllSolder(void)
{
	OnReqHospitalCureAllSolder		TempOnReqHospitalCureAllSolder;

	ZeroMemory(&TempOnReqHospitalCureAllSolder, sizeof(OnReqHospitalCureAllSolder));
	TempOnReqHospitalCureAllSolder.dwMsgType = ON_REQUEST_HOSPITAL_CUREALLSOLDER;

	return Write(&TempOnReqHospitalCureAllSolder, sizeof(OnReqHospitalCureAllSolder));
}


//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	濰睡, 識 啗謝蒂 蹂掘и棻.
BOOL	OnlineClient::SendAuditAccountList()
{
	OnReqAuditAccountListMsg		TempOnReqAuditAccountListMsg;

	TempOnReqAuditAccountListMsg.dwMsgType	=	ON_REQUEST_AUDIT_ACCOUNT;

	return Write( &TempOnReqAuditAccountListMsg, sizeof( OnReqAuditAccountListMsg ) );
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	濰睡縑憮 睡翕骯 葬蝶お蒂 蹂掘 и棻.
BOOL	OnlineClient::SendAuditOwnListMsg(void)
{
	OnReqAuditOwnListMsg		TempOnReqAuditOwnListMsg;

	TempOnReqAuditOwnListMsg.dwMsgType	=	ON_REQUEST_AUDIT_OWN;

	return Write( &TempOnReqAuditOwnListMsg, sizeof( OnReqAuditOwnListMsg ) );

}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	濰睡縑憮 望萄 葬蝶お蒂 蹂掘и棻.
BOOL	OnlineClient::SendAuditGuildMemberList()
{
	OnReqAuditGuildMemberInfoMsg	TempOnAuditGuildMemberInfoMsg;

	TempOnAuditGuildMemberInfoMsg.dwMsgType = ON_REQUEST_AUDIT_GUILD_INFO;

	return Write(&TempOnAuditGuildMemberInfoMsg,sizeof(OnReqAuditGuildMemberInfoMsg));
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	睡舒縑 菟橫除棻.
BOOL	OnlineClient::SendRequestWharfEnter(void)
{
	OnReqWharf_Enter			TempOnReqWharf_Enter;

	TempOnReqWharf_Enter.dwMsgType = ON_REQUEST_WHARF_ENTER;
	return Write(&TempOnReqWharf_Enter, sizeof(OnReqWharf_Enter));
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	睡舒縑憮 ル蒂 骯棻.
BOOL	OnlineClient::SendRequestWharfBuyTicket(SI32 TicketKind)
{
	OnReqWharf_BuyTicket			TempOnReqWharf_BuyTicket;

	TempOnReqWharf_BuyTicket.dwMsgType    = ON_REQUEST_WHARF_BUYTICKET;
	TempOnReqWharf_BuyTicket.siTicketKind = TicketKind;
	return Write(&TempOnReqWharf_BuyTicket, sizeof(OnReqWharf_BuyTicket));
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	睡舒縑憮 釭除棻.
BOOL	OnlineClient::SendRequestWharfExit(void)
{
	OnReqWharf_Exit			TempOnReqWharf_Exit;

	TempOnReqWharf_Exit.dwMsgType    = ON_REQUEST_WHARF_EXIT;
	return Write(&TempOnReqWharf_Exit, sizeof(OnReqWharf_Exit));
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	睡舒渠晦褒縑 菟橫除棻.
BOOL	OnlineClient::SendRequestWharfWaitRoomEnter(void)
{
	OnReqWharfWaitRoom_Enter			TempOnReqWharfWaitRoom_Enter;

	TempOnReqWharfWaitRoom_Enter.dwMsgType    = ON_REQUEST_WHARFWAITROOM_ENTER;
	return Write(&TempOnReqWharfWaitRoom_Enter, sizeof(OnReqWharfWaitRoom_Enter));
}
	
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	睡舒渠晦褒縑憮 釭除棻.
BOOL	OnlineClient::SendRequestWharfWaitRoomExit(void)
{
	OnReqWharfWaitRoom_Exit			TempOnReqWharfWaitRoom_Exit;

	TempOnReqWharfWaitRoom_Exit.dwMsgType    = ON_REQUEST_WHARFWAITROOM_EXIT;
	return Write(&TempOnReqWharfWaitRoom_Exit, sizeof(OnReqWharfWaitRoom_Enter));
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	堯濰 薑爾蒂 蹂羶擊 и棻.
BOOL	OnlineClient::SendRequestFramInfo(UI08 uiPlantID)
{
	OnReqFarmInfoMsg				TempOnReqFarmInfoMsg;

	TempOnReqFarmInfoMsg.dwMsgType	=	ON_REQUEST_FARM_INFO;
	TempOnReqFarmInfoMsg.uiPlantID	=	uiPlantID;

	return Write( &TempOnReqFarmInfoMsg, sizeof( OnReqFarmInfoMsg ) );
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	儅骯衛撲 - 勒僭擊 骯棻
BOOL	OnlineClient::SendRequestMakeBuy(UI08 uiPlantID, SI16 siSlot)
{
	OnReqFarmBuyMsg					TempOnReqFarmBuyMsg;

	TempOnReqFarmBuyMsg.dwMsgType	=	ON_REQUEST_FARM_BUY;
	TempOnReqFarmBuyMsg.uiFarmIndex	=	(UI08)siSlot;
	TempOnReqFarmBuyMsg.uiPlantID	=	uiPlantID;

	return Write( &TempOnReqFarmBuyMsg, sizeof( OnReqFarmBuyMsg ) );
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	儅骯衛撲 - 勒僭擊 勒撲
BOOL	OnlineClient::SendRequestBuildStruct( SI16 siPay, SI16 siSlot, CHAR *pBuildKind )
{
	OnReqFarmBuildStructMsg			TempOnReqFarmBuildStructMsg;

	TempOnReqFarmBuildStructMsg.dwMsgType	=	ON_REQUEST_FARM_BUILDSTRUCT;		
	TempOnReqFarmBuildStructMsg.uiSlotIndex	=	(UI08)siSlot;
	TempOnReqFarmBuildStructMsg.siPay		=	siPay;

	strcpy( TempOnReqFarmBuildStructMsg.Code, pBuildKind );
	
	return Write( &TempOnReqFarmBuildStructMsg, sizeof( OnReqFarmBuildStructMsg ) );
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	儅骯衛撲 - 蝸煜曖 薑爾蒂 蹂羶и棻.
BOOL	OnlineClient::SendRequestSlotInfo( UI08 uiPlantID, SI16 siSlot )
{
	OnReqFarmSlotInfoMsg		TempOnReqFarmSlotInfoMsg;

	TempOnReqFarmSlotInfoMsg.dwMsgType		=	ON_REQUEST_FARM_SLOTINFO;
	TempOnReqFarmSlotInfoMsg.uiFarmIndex	=	(UI08)siSlot;
	TempOnReqFarmSlotInfoMsg.uiPlantID		=	uiPlantID;

	return Write( &TempOnReqFarmSlotInfoMsg, sizeof( OnReqFarmSlotInfoMsg ) );
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	儅骯衛撲 - 儅骯 鏃模
BOOL	OnlineClient::SendRequseCancel( UI08 uiPlantID, UI08 uiFarmIndex, UI08 uiSlotIndex )
{
	OnReqPlantCancelPlanMsg		TempOnReqPlantCancelPlanMsg;

	TempOnReqPlantCancelPlanMsg.dwMsgType	=	ON_REQUEST_PLANT_CANCEL_PLAN;
	TempOnReqPlantCancelPlanMsg.uiPlantID	=	uiPlantID;
	TempOnReqPlantCancelPlanMsg.uiFarmIndex	=	uiFarmIndex;
	TempOnReqPlantCancelPlanMsg.uiSlotIndex	=	uiSlotIndex;

	return Write( &TempOnReqPlantCancelPlanMsg, sizeof( OnReqPlantCancelPlanMsg ) );
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	儅骯衛撲 - 勒僭擊 機斜溯檜萄 и棻.
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
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	儅骯衛撲 - 璽堅曖 薑爾蒂 蹂羶и棻.
BOOL	OnlineClient::SendRequestStorageInfo( UI08 uiPlantID, UI08 uiPartID, UI08 uiSlotIndex )
{
	OnReqStorageInfoMsg			TempOnReqStorageInfoMsg;	

	TempOnReqStorageInfoMsg.dwMsgType	=	ON_REQUEST_STORAGE_INFO;
	TempOnReqStorageInfoMsg.uiPlantID	=	uiPlantID;
	TempOnReqStorageInfoMsg.uiPartID	=	uiPartID;
	TempOnReqStorageInfoMsg.uiSlotIndex	=	uiSlotIndex;

	return Write( &TempOnReqStorageInfoMsg, sizeof( OnReqStorageInfoMsg ) );

}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	儅鼻衛撲 - 勒僭檜 橫雯 嬴檜蠱擊 虜萄陛?
BOOL	OnlineClient::SendRequestWorkInfo( UI08 uiPlantID, SI16 siFarm, UI08 uiSlot )
{
	OnReqFarmWorkInfoMsg				TempOnReqFarmWorkInfoMsg;
	
	TempOnReqFarmWorkInfoMsg.dwMsgType		=	ON_REQUEST_FARM_WORKINFO;	
	TempOnReqFarmWorkInfoMsg.uiPlantID		=	uiPlantID;
	TempOnReqFarmWorkInfoMsg.uiFarmIndex	=	(UI08)siFarm;
	TempOnReqFarmWorkInfoMsg.uiSlotIndex	=	uiSlot;

	return Write( &TempOnReqFarmWorkInfoMsg, sizeof( OnReqFarmWorkInfoMsg ) );
}
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	儅骯衛撲 - 璽堅曖 嬴檜蠱擊 厥朝棻.
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

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
// 儅骯衛撲 - 嬴檜蠱擊 儅骯 и棻堅  
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


//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
// 儅骯衛撲 - 儅骯衛撲 - 喻翕擊 и棻.
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


//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
// 儅骯衛撲 - 蝸煜擊 機旋 и棻.
BOOL	OnlineClient::SendRequseUpgradeSlot( UI08 uiSlot, SI16 siPay )
{
	OnReqFarmUpgradeSlotMsg		TempOnReqFarmUpgradeSlotMsg;

	TempOnReqFarmUpgradeSlotMsg.dwMsgType	=	ON_REQUEST_FARM_UPGRADESLOT;
	TempOnReqFarmUpgradeSlotMsg.siPay		=	siPay;
	TempOnReqFarmUpgradeSlotMsg.uiSlotIndex	=	uiSlot;

	return Write( &TempOnReqFarmUpgradeSlotMsg, sizeof( OnReqFarmUpgradeSlotMsg ) );
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
// 儅骯衛撲 - 嬴檜蠱擊 陛雖堅 除棻.
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

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
// 儅骯衛撲 - 蝸煜擊 �挫敻� и棻.
BOOL	OnlineClient::SendRequesExtension( UI08 uiPlantID, UI08 uiFarmIndex )
{
	OnReqFarmExpandSlotMsg			TempOnReqFarmExpandSlotMsg;

	TempOnReqFarmExpandSlotMsg.dwMsgType	=	ON_REQUEST_FARM_EXPANDSLOT;
	TempOnReqFarmExpandSlotMsg.uiPlantID	=	uiPlantID;
	TempOnReqFarmExpandSlotMsg.uiFarmIndex	=	uiFarmIndex;

	return Write( &TempOnReqFarmExpandSlotMsg, sizeof( OnReqFarmExpandSlotMsg ) );
}


//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
// 儅骯衛撲 - 蝸煜縑 氈朝 勒僭擊 薯剪
BOOL	OnlineClient::SendRequesDestroyStrcut( UI08 uiPlantID, UI08 uiFarmIndex, UI08 uiSlotIndex )
{
	OnReqFarmDestroyStructMsg		TempOnReqFarmDestroyStructMsg;

	TempOnReqFarmDestroyStructMsg.dwMsgType		=	ON_REQUEST_FARM_DESTROYSTRUCT;
	TempOnReqFarmDestroyStructMsg.uiPlantID		=	uiPlantID;
	TempOnReqFarmDestroyStructMsg.uiFarmIndex	=	uiFarmIndex;
	TempOnReqFarmDestroyStructMsg.uiSlotIndex	=	uiSlotIndex;

	return Write( &TempOnReqFarmDestroyStructMsg, sizeof( OnReqFarmDestroyStructMsg ) );
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
// 儅骯衛撲 - 橾ж朝 夠縑 紫掘 氈朝 憲嬴 獄棻.
BOOL	OnlineClient::SendRequseFarmWorkStartMsg( UI08 uiSlot )
{
	OnReqFarmWorkStartMsg		TempOnReqFarmWorkStartMsg;

	TempOnReqFarmWorkStartMsg.dwMsgType		=	ON_REQUEST_FARM_WORKSTART;
	TempOnReqFarmWorkStartMsg.uiSlotIndex	=	uiSlot;

	return Write( &TempOnReqFarmWorkStartMsg, sizeof( OnReqFarmWorkStartMsg ) );
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
// 儅骯衛撲 - 歜旎擊 滲唳
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


//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
// 儅骯衛撲 - 欽雖縑憮 釭除棻堅 爾頂遽棻.
BOOL	OnlineClient::SendRequestLeaveStructure()
{
	OnReqLeaveStructureMsg	TempOnReqLeaveStructureMsg;

	TempOnReqLeaveStructureMsg.dwMsgType	=	ON_REQUEST_LEAVE_STRUCTURE;

	return Write( &TempOnReqLeaveStructureMsg, sizeof( OnReqLeaveStructureMsg ) );
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
// 儅骯衛撲 - 奢濰縑憮 釭除棻堅 爾頂遽棻.
BOOL	OnlineClient::SendRequestLeaveFactory()
{
	OnReqLeaveFactoryMsg	TempOnReqLeaveFactoryMsg;

	TempOnReqLeaveFactoryMsg.dwMsgType	=	ON_REQUEST_LEAVE_FACTORY;

	return Write( &TempOnReqLeaveFactoryMsg, sizeof( OnReqLeaveFactoryMsg ) );
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	嬴嬴蠱 雜辨
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

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	嬴檜蠱 慇棻
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

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	嬴檜蠱 餌辨
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

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	雜辨ж堅 氈朝 嬴檜蠱擊 嫡朝棻.
BOOL	OnlineClient::SendRequestItemInfo()
{
	OnReqWearItemInfoMsg			TempOnReqWearItemInfoMsg;

	TempOnReqWearItemInfoMsg.dwMsgType	=	ON_REQUEST_WEARITEM_INFO;

	return Write( &TempOnReqWearItemInfoMsg, sizeof( OnReqWearItemInfoMsg ) );
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	婦羶 菟橫陛晦
BOOL	OnlineClient::SendRequestGovernment_Main_Enter(void)
{
	OnReqEnterGovernment			TempOnReqEnterGovernment;

	TempOnReqEnterGovernment.dwMsgType	=	ON_REQUEST_GOVERNMENT_MAIN_ENTER;

	return Write(&TempOnReqEnterGovernment, sizeof(OnReqEnterGovernment));
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	婦羶 釭陛晦
BOOL	OnlineClient::SendRequestGovernment_Main_Leave(void)
{
	OnReqLeaveGovernment			TempOnReqLeaveGovernment;

	TempOnReqLeaveGovernment.dwMsgType	=	ON_REQUEST_GOVERNMENT_MAIN_LEAVE;

	return Write(&TempOnReqLeaveGovernment, sizeof(OnReqLeaveGovernment));
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	婦羶, 癱濠 菟橫陛晦
BOOL	OnlineClient::SendRequestGovernment_Invenstment_Enter(SI16 Type)
{
	OnReqGovernmentEnterInvestment		TempOnReqGovernmentEnterInvestment;

	TempOnReqGovernmentEnterInvestment.dwMsgType	=	ON_REQUEST_GOVERNMENT_INVESTMENT_ENTER;
	TempOnReqGovernmentEnterInvestment.m_Type       =   Type;

	return Write(&TempOnReqGovernmentEnterInvestment, sizeof(OnReqGovernmentEnterInvestment));
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	婦羶, 癱濠 釭陛晦
BOOL	OnlineClient::SendRequestGovernment_Invenstment_Leave(void)
{
	OnReqGovernmentLeaveInvestment		TempOnReqGovernmentLeaveInvestment;

	TempOnReqGovernmentLeaveInvestment.dwMsgType	=	ON_REQUEST_GOVERNMENT_INVESTMENT_LEAVE;

	return Write(&TempOnReqGovernmentLeaveInvestment, sizeof(OnReqGovernmentLeaveInvestment));
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	婦羶, 癱濠 List橢晦
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

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	婦羶, 癱濠 ж晦
BOOL	OnlineClient::SendRequestGovernment_Invenstment_SetInvestment(SI16 Type, MONEY InvestmentMoney_InPocket, MONEY InvestmentMoney_InBank/* = 0*/)
{
	OnReqGovernmentSetInvestment			TempOnReqGovernmentSetInvestment;

	TempOnReqGovernmentSetInvestment.dwMsgType				= ON_REQUEST_GOVERNMENT_INVESTMENT_SETINVESTMENT;
//	TempOnReqGovernmentSetInvestment.m_Type					= Type;
	TempOnReqGovernmentSetInvestment.m_InvestmentMoney		= InvestmentMoney_InPocket;	// 癱濠ж溥朝 絲(模雖旎)
	TempOnReqGovernmentSetInvestment.m_InvestmentSaveMoney	= InvestmentMoney_InBank;	// 癱濠ж溥朝 絲(瞪濰)

	// 葆擊縑 癱濠ж朝 識 癱濠旎擎 InvestmentMoney_InPocket + InvestmentMoney_InBank 檜貊,
	// 餌辨濠陛 ァ驕 癱濠 掏и擊檜辨ж罹 癱濠ж朝 唳縑虜 瞪濰曖 絲(InvestmentMoney_InBank)檜 餌辨脹棻.

	return Write(&TempOnReqGovernmentSetInvestment, sizeof(OnReqGovernmentSetInvestment));
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	葆擊 模偃僥 檗晦
BOOL	OnlineClient::SendRequestVillageDescript_Read(UI16 uiVillageCode)
{
	OnReqVillageDescript			TempOnReqVillageDescript;

	TempOnReqVillageDescript.dwMsgType		 = ON_REQUEST_READ_DESCRIPT;
	TempOnReqVillageDescript.uiVillageCode	 = uiVillageCode;

	return Write(&TempOnReqVillageDescript, sizeof(OnReqVillageDescript));
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	葆擊 模偃僥 噙晦
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

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	葆擊 薑爾 蹂羶
BOOL	OnlineClient::SendRequestVillageData(UI16 uiVillageCode)
{
	OnReqVillageGetNation			TempOnReqVillageGetNation;

	TempOnReqVillageGetNation.dwMsgType		= ON_REQUEST_VILLAGE_NATION;
	TempOnReqVillageGetNation.uiVillageCode	= uiVillageCode;

	return Write(&TempOnReqVillageGetNation, sizeof(OnReqVillageGetNation));
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	檜賅じ夔 餌辨
BOOL	OnlineClient::SendRequestImoticon(UI16 ImoticonID)
{
	OnReqImoticonMsg		TempOnReqImoticonMsg;

	TempOnReqImoticonMsg.dwMsgType    = ON_REQUEST_IMOTICON;
	TempOnReqImoticonMsg.uiImoticonID = ImoticonID;

	return Write(&TempOnReqImoticonMsg, sizeof(TempOnReqImoticonMsg));
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	剪楚 蹂羶
BOOL	OnlineClient::SendRequestAskTrade( UI16 uiID )
{
	OnReqAskTradeMsg		TempOnReqAskTradeMsg;

	TempOnReqAskTradeMsg.dwMsgType	=	ON_REQUEST_TRADE;
	TempOnReqAskTradeMsg.UserID		=	uiID;

	return Write( &TempOnReqAskTradeMsg, sizeof( OnReqAskTradeMsg ) );
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	剪楚 蹂羶 嫡擎 餌塋檜 棻衛 爾頂朝 詭撮雖
BOOL	OnlineClient::SendRequestTrade(BOOL bAbccept)
{
	OnResAskTradeMsg		TempOnResAskTradeMsg;

	TempOnResAskTradeMsg.dwMsgType	= ON_REQUEST_ACCEPT_TRADE;
	TempOnResAskTradeMsg.dwResponse = bAbccept == TRUE ? TRUE : FALSE;

	return Write( &TempOnResAskTradeMsg, sizeof( OnResAskTradeMsg ) );
}


// 撲貲		: だじ 蹂羶.
BOOL	OnlineClient::SendRequestAskParty(UI16	uiID)
{

	OnReqAskPartyMsg		TempOnReqAskPartyMsg;

	TempOnReqAskPartyMsg.dwMsgType	=	ON_REQUEST_PARTY;
	TempOnReqAskPartyMsg.UserID		=	uiID;

	return Write( &TempOnReqAskPartyMsg, sizeof( OnReqAskPartyMsg ) );
}

// 撲貲		: だじ 蹂羶 嫡擎 餌塋檜 棻衛 爾頂朝 詭撮雖.
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



//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	僭勒擊 螢溥 場朝棻.
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

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	剪楚 鏃模
BOOL	OnlineClient::SendRequestCancelTrade()
{
	OnReqCancelTradeMsg		TempOnReqCancelTradeMsg;

	TempOnReqCancelTradeMsg.dwMsgType	=	ON_REQUEST_CANCEL_TRADE;

	return Write( &TempOnReqCancelTradeMsg, sizeof( OnReqCancelTradeMsg ) );
}


//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	剪楚 幗が蒂 揚葷
BOOL	OnlineClient::SendRequestConfirmTrade( BOOL bFlag )
{
	OnReqFinishChooseTradeItemMsg			TempOnReqFinishChooseTradeItemMsg;

	TempOnReqFinishChooseTradeItemMsg.dwMsgType			=	ON_REQUEST_FINISH_CHOOSE_TRADEITEM;
	TempOnReqFinishChooseTradeItemMsg.bFinishChooseItem	=	bFlag;

	return Write( &TempOnReqFinishChooseTradeItemMsg, sizeof( OnReqFinishChooseTradeItemMsg ) );
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	熱塊 幗が擊 揚葷
BOOL	OnlineClient::SendRequestTradeSuccess(BOOL bPush)
{
	OnReqDecideTradeMsg		TempOnReqDecideTradeMsg;

	TempOnReqDecideTradeMsg.dwMsgType		=	ON_REQUEST_DECIDE_TRADE;
	TempOnReqDecideTradeMsg.bDecideTrade	=	bPush;

	return Write( &TempOnReqDecideTradeMsg, sizeof(OnReqDecideTradeMsg) );
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	遴艙濠 貲滄
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
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	爾傘蝶 熱纂 
BOOL	OnlineClient::SendBouns( UI08 uiFollowerID, AbilityKindType Ability )
{
	OnReqIncAbilityByBonusMsg		TempOnReqIncAbilityByBonusMsg;


	TempOnReqIncAbilityByBonusMsg.dwMsgType		=	ON_REQUEST_INC_ABILITY_BYBONUS;
	TempOnReqIncAbilityByBonusMsg.uiFollowerID	=	uiFollowerID;
	TempOnReqIncAbilityByBonusMsg.AbilityKind	=	Ability;

	return Write( &TempOnReqIncAbilityByBonusMsg, sizeof( TempOnReqIncAbilityByBonusMsg ) );
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	謝っ擊 籀擠 偃撲и棻.
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

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	謝っ 偃撲�� 嬴檜蠱 滲唳 塽 蹺陛
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

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	謝っ渠縑憮 釭除棻.
BOOL	OnlineClient::SendLeaveBoothMsg()
{
	OnReqPMCloseMsg				TempOnReqPMCloseMsg;

	TempOnReqPMCloseMsg.dwMsgType	=	ON_REQUEST_PM_CLOSE;

	return Write( &TempOnReqPMCloseMsg, sizeof( OnReqPMCloseMsg ) );
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	謝っ縑 氈朝 嬴檜蠱擊 蹂羶擊 и棻.
BOOL	OnlineClient::SendDisplayBoothItemMsg( UI16 uiBoothID )
{
	OnReqDisplayBoothItemMsg		TempOnReqDisplayBoothItemMsg;

	TempOnReqDisplayBoothItemMsg.dwMsgType	=	ON_REQUEST_DISPLAYBOOTH;
	TempOnReqDisplayBoothItemMsg.uiBoothID	=	uiBoothID;

	return Write( &TempOnReqDisplayBoothItemMsg, sizeof( OnReqDisplayBoothItemMsg ) );
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	謝っ縑 氈朝 嬴檜蠱 薑爾蒂 橢橫螞棻.
BOOL	OnlineClient::SendGetBoothItemInfo(UI16	uiAccount)
{
	OnReqPMItemInfoMsg			TempOnReqPMItemInfoMsg;

	TempOnReqPMItemInfoMsg.dwMsgType	=	ON_REQUEST_PM_ITEMINFO;
	TempOnReqPMItemInfoMsg.uiAccount	=	uiAccount;
//	TempOnReqPMItemInfoMsg.uiSlotID		=	uiSlotID;

	return Write( &TempOnReqPMItemInfoMsg, sizeof( OnReqPMItemInfoMsg ) );
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	謝っ僭勒擊 掘殮ж棻.
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

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	葆擊 薑爾蒂 殖塭堅 蹂羶
BOOL	OnlineClient::SendRequestGovernment_VillageInfo(void)
{
	OnReqGovernmentVillageInfo		TempOnReqGovernmentVillageInfo;

	TempOnReqGovernmentVillageInfo.dwMsgType = ON_REQUEST_GOVERNMENT_VILLAGEINFO;
	return Write( &TempOnReqGovernmentVillageInfo, sizeof( TempOnReqGovernmentVillageInfo ));
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	棻艇 鼻欽薑爾蒂 橢橫褥
BOOL	OnlineClient::SendRequestGovernment_GetDiffGuild(void)
{
	OnReqGuildListMsg				TempOnReqGuildListMsg;

	TempOnReqGuildListMsg.dwMsgType = ON_REQUEST_GUILD_LIST;
	return Write(&TempOnReqGuildListMsg, sizeof(TempOnReqGuildListMsg));
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	鼻欽縑 陛殮й溥朝 Ы溯檜橫 薑爾蒂 橢橫褥
BOOL	OnlineClient::SendRequestGovernment_GetJoinGuild(SI32 PageNum)
{
	OnReqGuildWaitingJoinMembersListMsg				TempOnReqGuildWaitingJoinMembersListMsg;

	TempOnReqGuildWaitingJoinMembersListMsg.dwMsgType	=	ON_REQUEST_GUILD_WAITING_JOIN_MEMBERS_LIST;
	TempOnReqGuildWaitingJoinMembersListMsg.dwPage      =   PageNum; 

	return Write(&TempOnReqGuildWaitingJoinMembersListMsg, sizeof(TempOnReqGuildWaitingJoinMembersListMsg));
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	鼻欽縑憮 п堅й溥朝 Ы溯檜橫 薑爾蒂 橢橫褥
BOOL	OnlineClient::SendRequestGovernment_GetDismissalGuild(SI32 PageNum, UI32 uiGuildID)
{
	OnReqGuildMembersListMsg				TempOnReqGuildMembersListMsg;

	TempOnReqGuildMembersListMsg.dwMsgType	=	ON_REQUEST_GUILD_MEMBERS_LIST;
	TempOnReqGuildMembersListMsg.dwPage     =   PageNum;
	TempOnReqGuildMembersListMsg.uiGuidlID  =   uiGuildID;

	return Write(&TempOnReqGuildMembersListMsg, sizeof(TempOnReqGuildMembersListMsg));
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	鼻欽擊 虜蛟
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

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	鼻欽擊 п羹
BOOL	OnlineClient::SendRequestGovernment_DeleteGuild(void)
{
	OnReqDeleteGuildMsg				TempOnReqDeleteGuildMsg;

	TempOnReqDeleteGuildMsg.dwMsgType	=	ON_REQUEST_DELETE_GUILD;

	return Write(&TempOnReqDeleteGuildMsg, sizeof(TempOnReqDeleteGuildMsg));
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	鼻欽 陛殮/驍黴
BOOL OnlineClient::SendRequestGovernment_GuildJoinLeave(void)
{
	OnReqJoinLeaveGuildMsg				TempOnReqJoinLeaveGuildMsg;

	TempOnReqJoinLeaveGuildMsg.dwMsgType	=	ON_REQUEST_JOIN_LEAVE_GUILD;

	return Write(&TempOnReqJoinLeaveGuildMsg, sizeof(TempOnReqJoinLeaveGuildMsg));
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	鼻欽 陛殮擊 ъ塊л
BOOL OnlineClient::SendRequestGovernment_JoinGuildPlayer(char* lpID)
{
	OnReqJoinGuildOkNoMsg		TempOnReqJoinGuildOkNoMsg;

	ZeroMemory(&TempOnReqJoinGuildOkNoMsg, sizeof(TempOnReqJoinGuildOkNoMsg));
	TempOnReqJoinGuildOkNoMsg.dwMsgType	 = ON_REQUEST_JOINGUILD_OKNO;
	TempOnReqJoinGuildOkNoMsg.dwResponse = ON_RET_OK;
	strncpy((char*)TempOnReqJoinGuildOkNoMsg.szID, lpID, ON_ID_LENGTH);

	return Write(&TempOnReqJoinGuildOkNoMsg, sizeof(TempOnReqJoinGuildOkNoMsg));
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	鼻欽 陛殮擊 剪睡л
BOOL OnlineClient::SendRequestGovernment_JoinCancelGuildPlayer(char* lpID)
{
	OnReqJoinGuildOkNoMsg		TempOnReqJoinGuildOkNoMsg;

	ZeroMemory(&TempOnReqJoinGuildOkNoMsg, sizeof(TempOnReqJoinGuildOkNoMsg));
	TempOnReqJoinGuildOkNoMsg.dwMsgType	 = ON_REQUEST_JOINGUILD_OKNO;
	TempOnReqJoinGuildOkNoMsg.dwResponse = ON_RET_NO;
	strncpy((char*)TempOnReqJoinGuildOkNoMsg.szID, lpID, ON_ID_LENGTH);

	return Write(&TempOnReqJoinGuildOkNoMsg, sizeof(TempOnReqJoinGuildOkNoMsg));
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	鼻欽 驍黴蒂 衛霽
BOOL OnlineClient::SendRequestGovernment_DismissalGuildPlayer(char* lpID)
{
	OnReqDismissalMsg		TempOnReqDismissalMsg;

	ZeroMemory(&TempOnReqDismissalMsg, sizeof(TempOnReqDismissalMsg));
	TempOnReqDismissalMsg.dwMsgType	=	ON_REQUEST_GUILD_DISMISSAL;
	strncpy((char*)TempOnReqDismissalMsg.szID, lpID, ON_ID_LENGTH);

	return Write(&TempOnReqDismissalMsg, sizeof(TempOnReqDismissalMsg));
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	鼻欽 翕裊, 瞳渠 撲薑
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
// 奢撩瞪 �俸�薯曖 詭衛雖 爾頂晦
BOOL OnlineClient::SendRequestGovernment_OfferPeace(CHAR *szGuildID, CHAR *szMessage)
{
	OnReqGuildFriendlyMessageOffer	TempOnReqGuildFriendlyMessageOffer;

	ZeroMemory(&TempOnReqGuildFriendlyMessageOffer, sizeof(OnReqGuildFriendlyMessageOffer));

	TempOnReqGuildFriendlyMessageOffer.dwMsgType  = ON_REQUEST_GUILD_FRIELDLY_MESSAGE_OFFER;
	strncpy((char*)TempOnReqGuildFriendlyMessageOffer.szGuildID, szGuildID, ON_GUILDNAME_LENGTH);	// 瞳渠鼻欽 檜葷
	strncpy((char*)TempOnReqGuildFriendlyMessageOffer.szMessage, szMessage, 80);					// �俸�薯曖 詭衛雖

	return Write(&TempOnReqGuildFriendlyMessageOffer, sizeof(TempOnReqGuildFriendlyMessageOffer));
}

// robypark 2004/11/25 16:16
// 奢撩瞪 �俸�薯曖 熱塊 詭衛雖 爾頂晦
BOOL OnlineClient::SendRequestGovernment_AcceptOfferedPeace(CHAR *szGuildID)
{
	OnReqGuildFriendlyMessageAccept	TempOnReqGuildFriendlyMessageAccept;

	ZeroMemory(&TempOnReqGuildFriendlyMessageAccept, sizeof(OnReqGuildFriendlyMessageAccept));

	TempOnReqGuildFriendlyMessageAccept.dwMsgType  = ON_REQUEST_GUILD_FRIENDLY_MESSAGE_ACCEPT;
	strncpy((char*)TempOnReqGuildFriendlyMessageAccept.szGuildID, szGuildID, ON_GUILDNAME_LENGTH);	// 瞳渠鼻欽 檜葷

	return Write(&TempOnReqGuildFriendlyMessageAccept, sizeof(TempOnReqGuildFriendlyMessageAccept));
}

// robypark 2004/11/25 16:16
// 奢撩瞪 �俸�薯曖 剪瞰 詭衛雖 爾頂晦
BOOL OnlineClient::SendRequestGovernment_DeclineOfferedPeace(CHAR *szGuildID)
{
	OnReqGuildFreindlyMessageCancel	TempOnReqGuildFreindlyMessageCancel;

	ZeroMemory(&TempOnReqGuildFreindlyMessageCancel, sizeof(OnReqGuildFreindlyMessageCancel));

	TempOnReqGuildFreindlyMessageCancel.dwMsgType  = ON_REQUEST_GUILD_FRIENDLY_MESSAGE_CANCEL;
	strncpy((char*)TempOnReqGuildFreindlyMessageCancel.szGuildID, szGuildID, ON_GUILDNAME_LENGTH);	// 瞳渠鼻欽 檜葷

	return Write(&TempOnReqGuildFreindlyMessageCancel, sizeof(TempOnReqGuildFreindlyMessageCancel));
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	鼻欽 葆擊葬蝶お 蹂羶
BOOL OnlineClient::SendRequestGovernment_GetGuildVillageList(char* lpGuildID)
{
	OnReqGuildDetailInfoMsg		TempOnReqGuildDetailInfoMsg;

	ZeroMemory(&TempOnReqGuildDetailInfoMsg, sizeof(OnReqGuildDetailInfoMsg));
	TempOnReqGuildDetailInfoMsg.dwMsgType  = ON_REQUEST_GUILD_DETAIL_INFO;
	strncpy((char*)TempOnReqGuildDetailInfoMsg.szGuildID, lpGuildID, ON_GUILDNAME_LENGTH);

	return Write(&TempOnReqGuildDetailInfoMsg, sizeof(TempOnReqGuildDetailInfoMsg));
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	葆擊曖 寞橫溘縑 癱濠�じ� 氈朝 譆渠絲擊 橢橫螞棻.
BOOL OnlineClient::SendRequestGovernment_GetMaxVillageDefenceInvestment(void)
{
	OnReqMaxVillageDefenceInvestment		TempOnReqMaxVillageDefenceInvestment;

	TempOnReqMaxVillageDefenceInvestment.dwMsgType = ON_REQUEST_MAXVILLAGEDEFENCEINVESTMENT;

	return Write(&TempOnReqMaxVillageDefenceInvestment, sizeof(TempOnReqMaxVillageDefenceInvestment));
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	葆擊曖 寞橫溘縑 癱濠и棻.
BOOL OnlineClient::SendRequestGovernment_SetVillageDefenceInvestment(MONEY DefenceMoney)
{
	OnReqSetVillageDefenceInvestment		TempOnReqSetVillageDefenceInvestment;

	TempOnReqSetVillageDefenceInvestment.dwMsgType       = ON_REQUEST_SETVILLAGEDEFENCEINVESTMENT;
	TempOnReqSetVillageDefenceInvestment.InvestmentMoney = DefenceMoney;

	return Write(&TempOnReqSetVillageDefenceInvestment, sizeof(TempOnReqSetVillageDefenceInvestment));
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	辨煽曖 檜葷擊 滲唳
BOOL OnlineClient::SendRequestSetRename( CHAR *pRename, UI08 uiFollower )
{
	OnReqSetNameMsg			TempOnReqSetNameMsg;

	TempOnReqSetNameMsg.dwMsgType		=	ON_REQUEST_AUDIT_SETNAME;
	TempOnReqSetNameMsg.uiFollowerNum	=	uiFollower;
	strncpy( TempOnReqSetNameMsg.cName, pRename, ON_ID_LENGTH );

	return Write( &TempOnReqSetNameMsg, sizeof( OnReqSetNameMsg ) );
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	⑷營 TickCount蒂 瞪歎 
BOOL OnlineClient::SendGameTickCount( DWORD TickCount, UI16 uiType )
{
	OnHeartBeatMsg		TempOnHeartBeatMsg;

	TempOnHeartBeatMsg.dwMsgType	= ON_HEARTBEAT;
	TempOnHeartBeatMsg.dwTimer		= TickCount;
	TempOnHeartBeatMsg.uiType		= uiType;

	return Write( &TempOnHeartBeatMsg, sizeof( OnHeartBeatMsg ) );
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	葆擊擊 奢問и棻.(斬剪葬)
BOOL	OnlineClient::SendShortDistanceAttackVillage( UI16 VillageCode, SI16 siAttackType )
{
	OnReqAttackVillageMsg		TempOnReqAttackVillageMsg;

	TempOnReqAttackVillageMsg.dwMsgtype     = ON_REQUEST_ATTACK_VILLAGE;
	TempOnReqAttackVillageMsg.uiVillageCode = VillageCode;
	TempOnReqAttackVillageMsg.siAttackType	= siAttackType;/*ON_ATTACK_TYPE_NORMAL;*/

	return Write(&TempOnReqAttackVillageMsg, sizeof(OnReqAttackVillageMsg));
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	葆擊擊 奢問и棻.(錳剪葬)
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

// 奢撩瞪 熱撩 檣攪む檜蝶煎 菟橫陛啊棻堅 蹂羶
BOOL	OnlineClient::SendEnterVillageDefenceInterface(UI16 uiVillageCode)
{
	OnReqWarVillageDefence			TempOnReqWarVillageDefence;

	TempOnReqWarVillageDefence.dwMsgType		=	ON_REQUEST_WAR_VILLAGE_DEFENCE;
	TempOnReqWarVillageDefence.uiVillageCode	=	uiVillageCode;

	return Write(&TempOnReqWarVillageDefence, sizeof(OnReqWarVillageDefence));
}

/*
// 奢撩瞪 熱撩 檣攪む檜蝶 鼻縑憮 葆擊縑 菟橫陛啊棻堅 蹂羶
BOOL	OnlineClient::SendEnterVillageFromVillageDefenceInterface(UI16 uiVillageCode)
{
	OnReqWarVillageIn			TempOnReqWarVillageIn;
	
	TempOnReqWarVillageIn.dwMsgType			=	ON_REQUEST_WAR_VILLAGE_IN;
	TempOnReqWarVillageIn.uiVillageCode		=	uiVillageCode;

	return Write(&TempOnReqWarVillageIn, sizeof(OnReqWarVillageIn));
}
*/

// 奢撩瞪 熱撩 檣攪む檜蝶 鼻縑憮 釭陛啊棻堅 蹂羶
BOOL	OnlineClient::SendLeaveVillageDefenceInterface(UI16 uiVillageCode)
{
	OnReqWarVillageIn			TempOnReqWarVillageIn;
	
	TempOnReqWarVillageIn.dwMsgType			=	ON_REQUEST_VILLAGE_DEFENCE_STOP;
	TempOnReqWarVillageIn.uiVillageCode		=	uiVillageCode;

	return Write(&TempOnReqWarVillageIn, sizeof(OnReqWarVillageIn));
}

// 奢撩瞪
// 葆擊曖 奢問 顫殮擊 滲唳檜 陛棟и雖 蹂羶ж晦
BOOL	OnlineClient::SendCanChangeVillageAttackType(UI16 VillageCode)
{
	OnReqVillageAttackTypeCheck		TempOnReqVillageAttackTypeCheck;

	TempOnReqVillageAttackTypeCheck.dwMsgType		=	ON_REQUEST_VILLAGE_ATTACK_TYPE_CHECK;
	TempOnReqVillageAttackTypeCheck.uiVillageCode	=	VillageCode;

	return Write(&TempOnReqVillageAttackTypeCheck, sizeof(OnReqVillageAttackTypeCheck));
}

// 奢撩瞪
// 葆擊 晦葷 奢問 衛紫 蹂羶ж晦
BOOL	OnlineClient::SendVillagePlayerOilAttack(UI16  VillageCode)
{
	OnReqVillagePlayerOilAttack		TempOnReqVillagePlayerOilAttack;

	TempOnReqVillagePlayerOilAttack.dwMsgType		=	ON_REQUEST_VILLAGE_PLAYER_OIL_ATTACK;
	TempOnReqVillagePlayerOilAttack.uiVillageCode	=	VillageCode;

	return Write(&TempOnReqVillagePlayerOilAttack, sizeof(OnReqVillagePlayerOilAttack));
}

// 奢撩瞪
// 葆擊 給 奢問 衛紫 辨羶ж晦
BOOL	OnlineClient::SendVillagePlayerStoneAttack(UI16  VillageCode)
{
	OnReqVillagePlayerStoneAttack		TempOnReqVillagePlayerStoneAttack;

	TempOnReqVillagePlayerStoneAttack.dwMsgType		=	ON_REQUEST_VILLAGE_PLAYER_STONE_ATTACK;
	TempOnReqVillagePlayerStoneAttack.uiVillageCode	=	VillageCode;

	return Write(&TempOnReqVillagePlayerStoneAttack, sizeof(OnReqVillagePlayerStoneAttack));
}

// 奢撩瞪
// 葆擊 寞橫溘 щ鼻 蹂羶ж晦
BOOL	OnlineClient::SendVillageDefenceUp(UI16 VillageCode)
{
	OnReqPlayerVillageDefenceUp		TempOnReqPlayerVillageDefenceUp;

	TempOnReqPlayerVillageDefenceUp.dwMsgType		=	ON_REQUEST_VILLAGE_DEFENCE;
	TempOnReqPlayerVillageDefenceUp.uiVillageCode	=	VillageCode;

	return Write(&TempOnReqPlayerVillageDefenceUp, sizeof(OnReqPlayerVillageDefenceUp));
}

// robypark 2005/1/28 18:3
// 奢撩瞪 嶸棉戲煎 滲褐ж啊棻堅 憮幗縑 蹂羶
BOOL	OnlineClient::SendTransformToSiegeUnit(SI16 siChangeKindIdx)
{
	OnReqChangeUnit		TempOnReqChangeUnit;

	TempOnReqChangeUnit.dwMsgType		=	ON_REQUEST_CHANGE_WAR;
	TempOnReqChangeUnit.siChangeKind	=	siChangeKindIdx;		// 滲褐ж溥朝 煽婁曖 寡翮 檣策蝶(蕨: 匐煽 0, ...)

	return Write(&TempOnReqChangeUnit, sizeof(OnReqChangeUnit));
}

// robypark 2005/1/29 14:13
// 奢撩瞪 爾晝熱溯陛 葆釭 10蟾葆棻 濠翕 �蛹� 蹂羶
BOOL	OnlineClient::SendReuqestChargeMP(void)
{
	OnMsg			TempOnMsg;

	TempOnMsg.dwMsgType		=	ON_REQUEST_CHARGE_MP;

	return Write(&TempOnMsg, sizeof(OnMsg));
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	奢撩瞪衛 議葛攪陛 議葛攪蒂 奢問и棻朝 詭撮雖蒂 憲溥遽棻.
BOOL	OnlineClient::SendFieldAttackReady(UI16	uiUniqueID,SI16 siAttackCharX,SI16 siAttackCharY)
{
	OnReqFieldAttackRdyMsg		TempOnReqFieldAttackRdyMsg;

	TempOnReqFieldAttackRdyMsg.dwMsgType		= ON_REQUEST_FIELDATTACK_READY;
	TempOnReqFieldAttackRdyMsg.uiUniqueID		= uiUniqueID;
//	TempOnReqFieldAttackRdyMsg.siAttackCharX	= siAttackCharX;
//	TempOnReqFieldAttackRdyMsg.siAttackCharY	= siAttackCharY;
	return Write(&TempOnReqFieldAttackRdyMsg,sizeof(OnReqFieldAttackRdyMsg));
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	奢撩瞪衛 議葛攪陛 餌辨и 鼠晦縑 蜃懊棻.
BOOL	OnlineClient::SendFieldAttackAction(UI16 uiEnemyCharID, SI16 siAttackType)
{
	OnReqFieldAttackActionMsg	TempOnReqFieldAttackActionMsg;

	TempOnReqFieldAttackActionMsg.dwMsgType		= ON_REQUEST_FIELDATTACK_ACTION;
	TempOnReqFieldAttackActionMsg.uiUniqueID	= uiEnemyCharID;
	//Gscprotocol 霤褻 樓撩縑 評塭 siAttackType擊 薑и棻.
	TempOnReqFieldAttackActionMsg.siAttackType	= siAttackType;/*ON_ATTACK_TYPE_NORMAL;*/


	return Write(&TempOnReqFieldAttackActionMsg,sizeof(OnReqFieldAttackActionMsg));
}
 
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	羹溘檜 棻腎憮 避歷棻.
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

//葆擊 寰縑憮 鼻欽 爾晝ヶ曖 偎熱蒂 棺萼棻.(渠寞.ч熱虜 陛棟)
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
	TempOnReqDecreaseGuildSupply.siSupplyQuantity	=	siSupply;			// в蹂и 榆
	TempOnReqDecreaseGuildSupply.siVillageCode		=	siVillageCode;		// 鼻欽 葆擊

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
// 奢撩瞪 葆擊 熱綠煽 熱 蹂羶ж晦
BOOL	OnlineClient::SendVillageGarrisonCount(SI16 siVillageCode)
{
	OnReqGetVillageDefenceSoldier	TempOnReqGetVillageDefenceSoldier;

	TempOnReqGetVillageDefenceSoldier.dwMsgType		=	ON_REQUEST_GET_VILLAGE_DEFENCE_SOLDIER;
	TempOnReqGetVillageDefenceSoldier.siVillageCode	=	siVillageCode;

	return Write(&TempOnReqGetVillageDefenceSoldier, sizeof(OnReqGetVillageDefenceSoldier));
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	Change Guild Flag
BOOL	OnlineClient::SendChangeGuildFlag(SI32 siGuildFlag)
{
	OnReqGuildChangeFlag		TempOnReqGuildChangeFlag;

	TempOnReqGuildChangeFlag.dwMsgType = ON_REQUEST_CHANGE_GUILD_FLAG;
	TempOnReqGuildChangeFlag.siFlag    = siGuildFlag;
	return Write(&TempOnReqGuildChangeFlag, sizeof(TempOnReqGuildChangeFlag));
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	餌辨腋朝 梓嫦 橢晦
BOOL	OnlineClient::SendUsedFlag(void)
{
	OnReqUsedFlagList		TempOnReqUsedFlagList;

	TempOnReqUsedFlagList.dwMsgType = ON_REQUEST_USED_FLAG_LIST;
	return Write(&TempOnReqUsedFlagList, sizeof(TempOnReqUsedFlagList));
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	葆擊曖 熱櫛 鱔啗蒂 橢橫螞棻.
BOOL	OnlineClient::SendRequestGovernment_GetVillageProfitStatistics(void)
{
	OnReqProfitStatistics		TempOnReqProfitStatistics;

	TempOnReqProfitStatistics.dwMsgType = ON_REQUEST_PROFIT_STATISTICS;
	return Write(&TempOnReqProfitStatistics, sizeof(TempOnReqProfitStatistics));
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	癱濠旎擊 檣轎и棻.
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

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
// 葆擊 啪衛っ縑憮 啪衛僭 頂辨擊 滲唳
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

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
// NPC 摹鷗
BOOL	OnlineClient::SendRequestQuestSelectNPC(SI16 siNPCCode, SI32 siBookCount)
{
	OnReqQuestSelectNPC		TempOnReqQuestSelectNPC;

	ZeroMemory(&TempOnReqQuestSelectNPC, sizeof(TempOnReqQuestSelectNPC));
	TempOnReqQuestSelectNPC.dwMsgType   = ON_REQUEST_QUEST_SELECTNPC;
	TempOnReqQuestSelectNPC.siNPCCode   = siNPCCode;
	TempOnReqQuestSelectNPC.siBookCount = siBookCount;

	return Write(&TempOnReqQuestSelectNPC, sizeof(OnReqQuestSelectNPC));
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
// Quest 熱塊
BOOL	OnlineClient::SendRequestQuestAcceptQuest(SI32 siQuestCode)
{
	OnReqQuestAcceptQuest		TempOnReqQuestAcceptQuest;

	ZeroMemory(&TempOnReqQuestAcceptQuest, sizeof(TempOnReqQuestAcceptQuest));
	TempOnReqQuestAcceptQuest.dwMsgType   = ON_REQUEST_QUEST_ACCEPTQUEST;
	TempOnReqQuestAcceptQuest.m_QuestCode = siQuestCode;

	return Write(&TempOnReqQuestAcceptQuest, sizeof(TempOnReqQuestAcceptQuest));
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
// 霞ч醞檣 Quest 鏃模
BOOL	OnlineClient::SendRequestQuestCancelQuest(SI32 siQuestCode)
{
	OnReqQuestCancelQuest		TempOnReqQuestCancelQuest;

	ZeroMemory(&TempOnReqQuestCancelQuest, sizeof(TempOnReqQuestCancelQuest));
	TempOnReqQuestCancelQuest.dwMsgType   = ON_REQUEST_QUEST_CANCELQUEST;
	TempOnReqQuestCancelQuest.m_QuestCode = siQuestCode;

	return Write(&TempOnReqQuestCancelQuest, sizeof(TempOnReqQuestCancelQuest));
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
// 霞ч醞檣 Quest List 蹂羶
BOOL	OnlineClient::SendRequestQuestGetPlayQuestList(void)
{
	OnReqQuestGetPlayQuest		TempOnReqQuestGetPlayQuest;

	ZeroMemory(&TempOnReqQuestGetPlayQuest, sizeof(TempOnReqQuestGetPlayQuest));
	TempOnReqQuestGetPlayQuest.dwMsgType   = ON_REQUEST_QUEST_GETPLAYQUEST;

	return Write(&TempOnReqQuestGetPlayQuest, sizeof(TempOnReqQuestGetPlayQuest));
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
// 擎ч啗謝 熱蒂 橢橫褥
BOOL	OnlineClient::SendRequestQuestGetBankBookCount(void)
{
	OnReqBankBankBookListCountMsg	TempOnReqBankBankBookListCountMsg;

	ZeroMemory(&TempOnReqBankBankBookListCountMsg, sizeof(OnReqBankBankBookListCountMsg));
	TempOnReqBankBankBookListCountMsg.dwMsgType   = ON_REQUEST_BANK_BANKBOOK_LISTCOUNT;

	return Write(&TempOnReqBankBankBookListCountMsg, sizeof(OnReqBankBankBookListCountMsg));
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
// Ы溯檜橫曖 薑爾蒂 蹂羶и棻.
BOOL	OnlineClient::SendPlayerDetailInfo(UI16 uiID)
{
	OnReqCharDetailInfoMsg		TempOnReqCharDetailInfoMsg;

	TempOnReqCharDetailInfoMsg.dwMsgType = ON_REQUEST_CHAR_DETAILINFO;
	TempOnReqCharDetailInfoMsg.uiAccount = uiID;

	return Write(&TempOnReqCharDetailInfoMsg, sizeof(OnReqCharDetailInfoMsg));	
}

/*
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
// 蝶鑒餌辨擊 蹂羶и棻.
BOOL	OnlineClient::SendRequestUseSkill(SI16 siSkillKind, SI16 siWhatDo)
{
	SKILL::sRequestUse		TempsRequestUse;

	TempsRequestUse.skillKind	= siSkillKind;
	TempsRequestUse.whatDo		= siWhatDo;

	return Write(&TempsRequestUse, sizeof(SKILL::sRequestUse));	
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
// 蝶鑒蛔晝擊 螢萼棻.
BOOL	OnlineClient::SendRequestLevelUp(SI16 siSkillKind)
{
	SKILL::sRequestLevelUp		TempsRequestLevelUp;

	TempsRequestLevelUp.skillKind	= siSkillKind;

	return Write(&TempsRequestLevelUp, sizeof(SKILL::sRequestLevelUp));	
}
*/

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
// 雖羲鬚餌蒂 蹂羶и棻.
BOOL	OnlineClient::SendRequestProbeArea()
{
	BURIED::sRequestProbeArea		TempsRequestProbeArea;
	TempsRequestProbeArea.command	=	BURIED::REQUEST_PROBE_AREA;

	return Write(&TempsRequestProbeArea, sizeof(BURIED::sRequestProbeArea));	
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
// 葆擊薑爾蒂 蹂羶и棻.
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

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
// 掘殮и 嬴檜蠱 葬蝶お蒂 蹂羶и棻.(輿虞)
BOOL	OnlineClient::SendRequestInnItemList()
{
	OnReqItemListMsg		TempOnReqItemListMsg;

	ZeroMemory(&TempOnReqItemListMsg, sizeof(OnReqItemListMsg));
	TempOnReqItemListMsg.dwMsgType     = ON_REQUEST_ITEMLIST;

	return Write(&TempOnReqItemListMsg, sizeof(OnReqItemListMsg));
}
	
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
// 掘殮и 嬴檜蠱擊 檣漸饜葬縑 厥朝棻.(輿虞)
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

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
// 嬴檜蠱擊 輿虞 檣漸饜葬縑 厥朝棻.(輿虞)
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

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
// 鼻欽薑爾蒂 蹂羶и棻.
BOOL	OnlineClient::SendRequestGuildInfo(CHAR *pszName)
{
	OnReqPersonalInfo		TempOnReqPersonalInfo;

	ZeroMemory(&TempOnReqPersonalInfo, sizeof(OnReqPersonalInfo));
	TempOnReqPersonalInfo.dwMsgType			= ON_REQUEST_PERSONAL_INFO;
	strncpy(TempOnReqPersonalInfo.szID, pszName, ON_GUILDNAME_LENGTH);

	return Write(&TempOnReqPersonalInfo, sizeof(OnReqPersonalInfo));
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
// 評塭棻棲朝 辨煽擊 餉薯и棻. 
BOOL	OnlineClient::SendRequestDeleteVMercenary(void)
{
	OnRequest_VMercenary_Delete		TempOnRequest_VMercenary_Delete;

	ZeroMemory(&TempOnRequest_VMercenary_Delete, sizeof(OnRequest_VMercenary_Delete));
	TempOnRequest_VMercenary_Delete.dwMsgType       = ON_REQUEST_VMERCENARY_DELETE;

	return Write(&TempOnRequest_VMercenary_Delete, sizeof(OnRequest_VMercenary_Delete));
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
// 嬴檜蠱 薯溼擊 蹂羶и棻.
BOOL	OnlineClient::SendRequestRefineItem(UI16 uiFollowerSlot, UI16 uiItemID)
{
	OnRequestRefineItem		TempOnRequestRefineItem;

	ZeroMemory(&TempOnRequestRefineItem, sizeof(OnRequestRefineItem));
	TempOnRequestRefineItem.dwMsgType		= ON_REQUEST_REFINE_ITEM;
	TempOnRequestRefineItem.uiFollowerSlot	= uiFollowerSlot;
	TempOnRequestRefineItem.uiItemID		= uiItemID;

	return Write(&TempOnRequestRefineItem, sizeof(OnRequestRefineItem));
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
// 鼻欽偃撲 綠辨擊 蹂羶и棻.
BOOL	OnlineClient::SendRequestGuildCreateCost()
{
	OnRequestGuildCreateCost		TempOnRequestGuildCreateCost;

	ZeroMemory(&TempOnRequestGuildCreateCost, sizeof(OnRequestGuildCreateCost));
	TempOnRequestGuildCreateCost.dwMsgType		= ON_REQUEST_GUILD_CREATE_COST;

	return Write(&TempOnRequestGuildCreateCost, sizeof(OnRequestGuildCreateCost));
}



/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
/// 煎斜檣婦溼 濛機и剪

//----------------------------------------------------------------------------------------------------------------
//	撲貲	:	贗塭檜樹お曖 薑爾蒂 爾魚棻.
//----------------------------------------------------------------------------------------------------------------
BOOL	OnlineClient::SendClientKind()
{
	FRONT_USER::sRequestCheckVersion		ClientInfoMsg;

	ClientInfoMsg.command	=	FRONT_USER::REQUEST_CHECK_VERSION;
	ClientInfoMsg.version	=	ON_CLINETVERSION;	
	
	return	Write(&ClientInfoMsg, sizeof(FRONT_USER::sRequestCheckVersion));	
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	憲葡餌о擊 蹂羶и棻.
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
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
//	撲貲	:	摹鷗и 憮幗 謙盟蒂 爾魚棻.
//----------------------------------------------------------------------------------------------------------------
BOOL	OnlineClient::SendWorldID(BYTE BWorldID)
{
	FRONT_USER::sRequestSelectWorld		SelectWorldMsg;

	SelectWorldMsg.command	=	FRONT_USER::REQUEST_SELECT_WORLD;
	SelectWorldMsg.world_id	=	BWorldID;
	
	return	Write(&SelectWorldMsg, sizeof(FRONT_USER::sRequestSelectWorld));	
}

//----------------------------------------------------------------------------------------------------------------
//	撲貲	:	煎斜檣曖 薑爾蒂 爾魚棻.
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

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
// 議葛攪 薑爾蒂 蹂羶и棻.
BOOL	OnlineClient::SendRequestCharInfo()
{
	LOGIN::sRequestCharsInfo		TempRequestCharsInfoMsg;

	TempRequestCharsInfoMsg.command		=	LOGIN::REQUEST_CHARS_INFO;

	return Write(&TempRequestCharsInfoMsg, sizeof(LOGIN::sRequestCharsInfo), TRUE);	
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	議葛攪蒂 餉薯и棻.
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
BOOL	OnlineClient::SendDeleteCharacter(UI08 uiSlot)
{
	LOGIN::sRequestDelChar		TempRequestDelChar;

	TempRequestDelChar.command	=	LOGIN::REQUEST_DEL_CAHR;
	TempRequestDelChar.delSlot	=	uiSlot;

	return Write(&TempRequestDelChar, sizeof(LOGIN::sRequestDelChar), TRUE);
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	議葛攪蒂 儅撩и棻堅 憮幗縑 憲溥遽棻.
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
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
//	撲貲	:	啪歜憮幗縑 煎斜檣и棻.
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
//	撲貲	:	渠ч熱, ч翕渠濰 蛔曖 霜晝擊 歜貲и棻.
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
//	撲貲	:	渠ч熱, ч翕渠濰 蛔曖 霜晝縑憮 п歜и棻.
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
//	撲貲	:	煎斜嬴醒擊 憮幗縑 憲萼棻.
//----------------------------------------------------------------------------------------------------------------
BOOL	OnlineClient::SendLogOut()
{
	OnReqGameServerLogOutMsg	TempOnReqGameServerLogOutMsg;
	ZeroMemory(&TempOnReqGameServerLogOutMsg, sizeof(OnReqGameServerLogOutMsg));
	TempOnReqGameServerLogOutMsg.dwMsgType = ON_REQUEST_LOGOUT;

	return Write(&TempOnReqGameServerLogOutMsg, sizeof(OnReqGameServerLogOutMsg));
}

//----------------------------------------------------------------------------------------------------------------
//	撲貲	:	耀掘葬蝶お 蹂羶
//----------------------------------------------------------------------------------------------------------------
BOOL	OnlineClient::SendReqFriendList()
{
	OnReqAuditFriendList	TempOnReqAuditFriendList;
	ZeroMemory(&TempOnReqAuditFriendList, sizeof(OnReqAuditFriendList));
	TempOnReqAuditFriendList.dwMsgType = ON_REQUEST_AUDIT_FRIEND_LIST;

	return Write(&TempOnReqAuditFriendList, sizeof(OnReqAuditFriendList));
}

//----------------------------------------------------------------------------------------------------------------
//	撲貲	:	耀掘 蹺陛
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
//	撲貲	:	耀掘 餉薯
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
//	撲貲	:	賅萇 耀掘 餉薯
//----------------------------------------------------------------------------------------------------------------
BOOL	OnlineClient::SendReqDeleteAllFriend()
{
	OnReqAuditFriendRemoveAll	TempOnReqAuditFriendRemoveAll;
	ZeroMemory(&TempOnReqAuditFriendRemoveAll, sizeof(OnReqAuditFriendRemoveAll));
	TempOnReqAuditFriendRemoveAll.dwMsgType = ON_REQUEST_AUDIT_FRIEND_REMOVE_ALL;

	return Write(&TempOnReqAuditFriendRemoveAll, sizeof(OnReqAuditFriendRemoveAll));
}

//----------------------------------------------------------------------------------------------------------------
//	撲貲	:	熱褐剪睡 撲薑
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
// 奢撩瞪 嶸棉 爾晝 熱溯縑啪 爾晝ヶ擊 蹂羶и棻.
// UI16 uiUniqueID_SupplyWagon:	奢撩瞪 爾晝熱溯 嶸棉曖 Unique ID
// SI16 ssNeedsSupplyGoodsAmount: в蹂и 爾晝ヶ 曄
BOOL	OnlineClient::SendSiegeWarfareSupplyGoodsFromSupplyWagon(UI16 uiUniqueID_SupplyWagon, SI16 ssNeedsSupplyGoodsAmount)
{
	OnReqGetSupplyAtGuildUnit	SDataOnReqGetSupplyAtGuildUnit;

	ZeroMemory(&SDataOnReqGetSupplyAtGuildUnit, sizeof(OnReqGetSupplyAtGuildUnit));

	SDataOnReqGetSupplyAtGuildUnit.dwMsgType		= ON_REQUEST_GET_SUPPLY_AT_GUILD_UNIT;
	SDataOnReqGetSupplyAtGuildUnit.siRequireSupply	= ssNeedsSupplyGoodsAmount;				// в蹂и 榆
	SDataOnReqGetSupplyAtGuildUnit.uiTargetID		= uiUniqueID_SupplyWagon;				// 爾晝熱溯 UniqueID

	return Write(&SDataOnReqGetSupplyAtGuildUnit, sizeof(SDataOnReqGetSupplyAtGuildUnit));
}

// robypark 2004/12/2 10:48
// 奢撩瞪 謙猿 �醴� 薄滄и 葆擊 葬蝶お蒂 蹂羶и棻.
BOOL	OnlineClient::SendRequest_guild_villageattack_result(void)
{
	OnReqVillageAttackResult		TempOnReqVillageAttackResult;

	ZeroMemory(&TempOnReqVillageAttackResult, sizeof(OnReqVillageAttackResult));

	TempOnReqVillageAttackResult.dwMsgType		= ON_REQUEST_GUILD_VILLAGEATTACK_RESULT;

	return Write(&TempOnReqVillageAttackResult, sizeof(TempOnReqVillageAttackResult));
}

// robypark 2004/12/2 13:41
// 奢撩瞪 謙猿 �醴� 葆擊縑 渠и ァ癱 掏и擊 か薑 議葛攪縑 輿晦 蹂羶
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
// 餌辨濠陛 菟橫陛 氈朝 葆擊縑 ァ驕 癱濠 掏и檜 氈朝雖 蹂羶и棻.
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
// 奢撩瞪 醞縑 橾奩 嶸棉 陝 措陛曖 か薑 嬪纂(蟾爾濠 熱溼濰)煎 錶Щ 蹂羶ж晦
// 檜 蹂羶 擬港擎 晦襄曖 遴艙濠 貲滄橫檣 /w(錶Щ)曖 匙擊 餌辨и棻.(ON_RESPONSE_WARP)
BOOL	OnlineClient::SendRequestSiegeWarfareWarp(UI16 uiUniqueID)
{
	OnReqWarWarp					TempOnReqWarWarp;

	TempOnReqWarWarp.dwMsgType		=	ON_REQUEST_WAR_WARP;
	TempOnReqWarWarp.uiUniqueID		=	uiUniqueID;				// 錶Щп憮 爾頂朝 橾奩嶸棉 嬴檜蛤.

	return Write(&TempOnReqWarWarp, sizeof(TempOnReqWarWarp));
}

// robypark 2005/1/5 17:4
// 奢撩瞪 嶸棉檣 鼻鷓縑憮 橾奩 嶸棉戲煎 給嬴陛啊棻堅 憮幗縑 蹂羶
BOOL	OnlineClient::SendRequestNormalCharacter(UI16 uiUniqueID)
{
	OnReqNormalChar					TempOnReqNormalChar;

	TempOnReqNormalChar.dwMsgType	=	ON_REQUEST_NORMAL_CHAR;
	TempOnReqNormalChar.uiUniqueID	=	uiUniqueID;

	return Write(&TempOnReqNormalChar, sizeof(TempOnReqNormalChar));
}