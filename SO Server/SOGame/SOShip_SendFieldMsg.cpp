#include "SOMain.h"
#include "SOWorld.h"
#include "OnlineGameMsg.h"
#include "SOShip.h"

//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	���������� �޽����� Send �Ѵ�.(�迡���� �ַ� �迡 ž���� ������Ը� �����ش�.)
//��������������������������������������������������������������������������������������������������������������������������������������������������������������
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

			// �迡 ž���Ѵ�.
			pBoardShipMsg	=	(OnBoardShipMsg*)cLocalBuffer;
		
			pBoardShipMsg->dwMsgType	=	ON_BOARDSHIP;	
			pBoardShipMsg->uiAccount	=	uiAccount;			

			pBoardShipMsg->uiAroundCharNum			=	UI16(pParam1);
			pBoardShipMsg->uiAroundItemNum			=	UI16(pParam2);	

			puiAroundCharInfo			=	(UI16*)pParam3;			
			pAroundFieldItemInfo		=	(OnlineReceiveFieldItemInfo*)pParam4;
			
			// �÷��̾� ������ �����Ѵ�.
			pSendPlayerInfo				=	(CHAR*)((CHAR*)pBoardShipMsg + sizeof(OnBoardShipMsg));
			uiPlayersInfoMsgLength		=	MakeOnReceivedPlayerInfoPacket(pSendPlayerInfo, pBoardShipMsg->uiAroundCharNum, puiAroundCharInfo, 
																		&pBoardShipMsg->uiAroundCharNum, 7 );
			
			// ������ ������ �����Ѵ�.
			pSendFieldItemInfo			=	(OnlineReceiveFieldItemInfo*)((CHAR*)pSendPlayerInfo + uiPlayersInfoMsgLength);			
			memcpy(pSendFieldItemInfo, pAroundFieldItemInfo, sizeof(OnlineReceiveFieldItemInfo) * pBoardShipMsg->uiAroundItemNum);			

			// ������ �ϴ� ����� �����Ѵ�.
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
		// �׻� �迡 Ÿ�ִ� ��� ������� �޽����� ������.
		SendMsg(siSendInfo, cLocalBuffer, siLength, CustomerID.m_NowUseDataCount, CustomerID.m_Data, FALSE);
	}


	return bRet;
}