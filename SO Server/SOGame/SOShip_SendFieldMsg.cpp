#include "SOMain.h"
#include "SOWorld.h"
#include "OnlineGameMsg.h"
#include "SOShip.h"

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명	:	실질적으로 메시지를 Send 한다.(배에서는 주로 배에 탑승한 사람에게만 보내준다.)
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
BOOL	cltSOShip::SendFieldMsg(SI32 siMsg, SI32 siSendInfo, LPVOID pParam1, LPVOID pParam2, LPVOID pParam3, LPVOID pParam4, LPVOID pParam5, LPVOID pParam6)
{
	BOOL						bRet	=	TRUE;	
	CHAR						cLocalBuffer[ON_MAX_LOCALBUFFER];	
	SI16						siLength;		
	CHAR						*pSendPlayerInfo;	
	UI32						uiPlayersInfoMsgLength;	

	if(BaseChar::SendFieldMsg(siMsg, siSendInfo, pParam1, pParam2, pParam3, pParam4, pParam5, pParam6) == TRUE)
		return TRUE;

	switch(siMsg)
	{
	case ON_BOARDSHIP:
		{
			OnBoardShipMsg				*pBoardShipMsg;			
			OnlineReceiveFieldItemInfo	*pSendFieldItemInfo;
			UI16						*puiAroundCharInfo;
			OnlineReceiveFieldItemInfo	*pAroundFieldItemInfo;

			// 배에 탑승한다.
			pBoardShipMsg	=	(OnBoardShipMsg*)cLocalBuffer;
		
			pBoardShipMsg->dwMsgType	=	ON_BOARDSHIP;	
			pBoardShipMsg->uiAccount	=	uiAccount;			

			pBoardShipMsg->uiAroundCharNum			=	UI16(pParam1);
			pBoardShipMsg->uiAroundItemNum			=	UI16(pParam2);	

			puiAroundCharInfo			=	(UI16*)pParam3;			
			pAroundFieldItemInfo		=	(OnlineReceiveFieldItemInfo*)pParam4;
			
			// 플레이어 정보를 설정한다.
			pSendPlayerInfo				=	(CHAR*)((CHAR*)pBoardShipMsg + sizeof(OnBoardShipMsg));
			uiPlayersInfoMsgLength		=	MakeOnReceivedPlayerInfoPacket(pSendPlayerInfo, pBoardShipMsg->uiAroundCharNum, puiAroundCharInfo, 
																		&pBoardShipMsg->uiAroundCharNum, 7 );
			
			// 아이템 정보를 설정한다.
			pSendFieldItemInfo			=	(OnlineReceiveFieldItemInfo*)((CHAR*)pSendPlayerInfo + uiPlayersInfoMsgLength);			
			memcpy(pSendFieldItemInfo, pAroundFieldItemInfo, sizeof(OnlineReceiveFieldItemInfo) * pBoardShipMsg->uiAroundItemNum);			

			// 보내야 하는 사이즈를 결정한다.
			siLength					=	sizeof(OnBoardShipMsg) + uiPlayersInfoMsgLength + 
											(sizeof(OnlineReceiveFieldItemInfo) * pBoardShipMsg->uiAroundItemNum);			

		}
		break;
	default:
		bRet	=	FALSE;
		break;
	}

	if(bRet == TRUE)
	{
		// 항상 배에 타있는 모든 사람에게 메시지를 보낸다.
		SendMsg(siSendInfo, cLocalBuffer, siLength, CustomerID.m_NowUseDataCount, CustomerID.m_Data, FALSE);
	}


	return bRet;
}