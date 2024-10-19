#include	"SOMain.h"
#include	"SOWorld.h"
#include	"SOBaseChar.h"
#include	"OnlineGameMsg.h"

#include "..\\XCommon\\XPacketCmd.h"
#include "XGateServerMgr.h"

//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	�ش��ϴ������αٿ� �ִ� �ٸ��÷��̾�� �޼����� ������.
//	bSend�� TRUE�� �ڱ��ڽſ��Ե� �޼����� ������ FALSE�� ������ �ʴ´�.
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
BOOL	BaseChar::SendFieldMsg(SI32 siMsg, SI32 siSendInfo, LPVOID pParam1, LPVOID pParam2, LPVOID pParam3, LPVOID pParam4, LPVOID pParam5, LPVOID pParam6)
{		
	static UI16		uiCharUniIDs[ON_STATUS_AREA_XSIZE * ON_STATUS_AREA_YSIZE];	
	static CHAR		cLocalBuffer[ON_MAX_LOCALBUFFER];	
	SI32			siSendCharNum = 0;
	SI16			siLength;	
	BOOL			bRet = TRUE;						// �� �Լ����� �޽����� �ٷ�°�?	
	BOOL			bSendMe = TRUE;		
	SI32			i;

	switch(siMsg)
	{	
	case ON_RESPONSE_POSITION:
		{
			OnCharMoveMsg			*pCharMoveMsg;	
			UI16					*pMovedChars;	
			UI16					uiMovedCharNum;	
			UI16					*puiInfo;
			BOOL					bOverWeight;
			BaseChar				*pBaseChar;		
			SOPlayer				*pPlayer;

			pMovedChars						=	(UI16*)pParam1;
			uiMovedCharNum					=	UI16(pParam2);
				
			pCharMoveMsg					=	(OnCharMoveMsg*)cLocalBuffer;			
			pCharMoveMsg->dwMsgType			=	ON_RESPONSE_POSITION;
			pCharMoveMsg->uiMovedCharNum	=	uiMovedCharNum;

			puiInfo							=	(UI16*)((CHAR*)pCharMoveMsg + sizeof(OnCharMoveMsg));
			for(i = 0; i < uiMovedCharNum; i++)
			{
				if(ISMONSTER(pMovedChars[i]) == TRUE)		pBaseChar	=	(BaseChar*)pMyWorld->clMonster.GetMonster(pMovedChars[i]);				
				else if(ISPLAYER(pMovedChars[i]) == TRUE)
				{
					pPlayer		=	pMyWorld->GetPlayer( pMovedChars[i] );
					pBaseChar	=	(BaseChar*)pPlayer;
				}
				else if(ISSHIP(pMovedChars[i]) == TRUE)		pBaseChar	=	(BaseChar*)pMyWorld->clSOShip.GetShip(pMovedChars[i]);

				// �÷��̾��� ��쿡�� ��ȿ�� ���� ������ ������ �������� 0���� ������.
				if( ISPLAYER(pMovedChars[i]) == TRUE )
				{
					bOverWeight		=	pPlayer->IsOverWeight();
				}
				else
				{
					bOverWeight		=	FALSE;
				}

				puiInfo[i * 4 + 0]		=	pBaseChar->uiAccount;
				puiInfo[i * 4 + 1]		=	UI16(pBaseChar->siX);
				puiInfo[i * 4 + 2]		=	UI16(pBaseChar->siY);
				puiInfo[i * 4 + 3]		=	UI16(bOverWeight);		// ���� ����
			}
			
			siLength						=	sizeof(OnCharMoveMsg) + (sizeof(UI16) * uiMovedCharNum * 4);			
		}
		break;
	case ON_ADDCHAR:		
		{		
			OnAddCharMsg				*pAddCharMsg;
			CHAR						*pTempBuffer;			
			UI16						uiAddCharAccount[1];			

			pAddCharMsg					=	(OnAddCharMsg*)cLocalBuffer;
			pTempBuffer					=	(CHAR*)((CHAR*)pAddCharMsg + sizeof(OnAddCharMsg));					
			
			pAddCharMsg->dwMsgType		=	ON_ADDCHAR;									
			uiAddCharAccount[0]			=	uiAccount;

			siLength					=	sizeof(OnAddCharMsg);
			siLength					+=	MakeOnReceivedPlayerInfoPacket( pTempBuffer, 1, uiAddCharAccount, NULL, 4 );			

			if(siSendInfo == SO_SFM_CUSTOM)
			{
				siSendCharNum				=	SI32(pParam1);
				memcpy(uiCharUniIDs, pParam2, sizeof(UI16) * siSendCharNum);			
			}			
		}
		break;		
	case ON_DELCHAR:
		{
			OnDelCharMsg	*pDelCharMsg;


			pDelCharMsg					=	(OnDelCharMsg*)cLocalBuffer;
			pDelCharMsg->dwMsgType		=	ON_DELCHAR;
			pDelCharMsg->uiCharID		=	uiAccount;			

			siLength					=	sizeof(OnDelCharMsg);

			if(siSendInfo == SO_SFM_CUSTOM)
			{
				siSendCharNum				=	SI32(pParam1);
				memcpy(uiCharUniIDs, pParam2, sizeof(UI16) * siSendCharNum);			
			}			
		}
		break;
	case ON_RESPONSE_BATTLEPLAYER:
		{
			OnResBattlePlayer*		lpOnResBattlePlayer;

			lpOnResBattlePlayer = (OnResBattlePlayer*)cLocalBuffer;
			lpOnResBattlePlayer->dwMsgType  = ON_RESPONSE_BATTLEPLAYER;
			lpOnResBattlePlayer->uiUniqueID = uiAccount;
			lpOnResBattlePlayer->fFlag      = UI08(pParam1);

			siLength					=	sizeof(OnResBattlePlayer);
		}
		break;
	case ON_RESPONSE_PLAYER_INFO:
		{		
			OnResPlayerInfoMsg			*pPlayerInfoMsg;									
			CHAR						*pTempBuffer;
			UI16						uiCharNum;
			UI16						*puiCharIDArray;
			UI32						uiGuildID;
			SI32						siClassInGuild;
			

			pPlayerInfoMsg				=	(OnResPlayerInfoMsg*)cLocalBuffer;
			uiCharNum					=	(UI16)(pParam1);
			puiCharIDArray				=	(UI16*)pParam2;
			pTempBuffer					=	(CHAR*)((CHAR*)pPlayerInfoMsg + sizeof(OnResPlayerInfoMsg));
			uiGuildID					=	UI32(pParam3);
			siClassInGuild				=	SI32(pParam4);

			pPlayerInfoMsg->dwMsgType	=	ON_RESPONSE_PLAYER_INFO;			
			pPlayerInfoMsg->uiCharNum	=	uiCharNum;

			siLength					=	sizeof(OnResPlayerInfoMsg);

			siLength					+=	MakeOnReceivedPlayerInfoPacket(pTempBuffer, pPlayerInfoMsg->uiCharNum, puiCharIDArray, &pPlayerInfoMsg->uiCharNum, 5 );			
		}
		break;
	case ON_RESPONSE_BATTLE:
		{			
			OnResBattleMsg		*pBattleMsg;

			pBattleMsg				=	(OnResBattleMsg*)cLocalBuffer;
			pBattleMsg->dwMsgType	=	ON_RESPONSE_BATTLE;
			pBattleMsg->dwResponse	=	DWORD(pParam1);

			// ���� �޽���.(ON_RET_BATTLE_WAIT, ON_RET_BATTLE_NO);
			siLength						=	sizeof(OnResBattleMsg);												
		}
		break;		
	case ON_RESPONSE_WARP:
		{
			OnResWarpMsg				*pWarpMsg;
			OnResWarpOkMsg				*pWarpOkMsg;
			UI16						*puiCharUniIDs;	
			OnlineReceiveFieldItemInfo	*pRecvFItemInfoIn, *pRecvFItemInfoOut;
			CHAR						*pReceivedPlayerInfo;
			UI32						uiPlayersInfoMsgLength;

			pWarpMsg				=	(OnResWarpMsg*)cLocalBuffer;
			pWarpMsg->dwMsgType		=	ON_RESPONSE_WARP;
			pWarpMsg->dwResponse	=	DWORD(pParam1);		
			

			switch(pWarpMsg->dwResponse)
			{
			case ON_RET_OK:
				pWarpOkMsg				=	(OnResWarpOkMsg*)pWarpMsg;				

				pWarpOkMsg->uiMapID			=	uiMapID;							
				pWarpOkMsg->uiX				=	UI16(siX);							
				pWarpOkMsg->uiY				=	UI16(siY);							
				pWarpOkMsg->uiPlayerNum		=	((UI16 *)pParam2)[0];				
				pWarpOkMsg->uiFieldItemNum	=	((UI16 *)pParam2)[1];				
				puiCharUniIDs				=	(UI16 *)pParam3;					
				pRecvFItemInfoIn			=	(OnlineReceiveFieldItemInfo*)pParam4;

				// �����͸� �������ش�.			
				pReceivedPlayerInfo	=	(CHAR*)((CHAR*)pWarpOkMsg + sizeof(OnResWarpOkMsg));
				
				uiPlayersInfoMsgLength	=	MakeOnReceivedPlayerInfoPacket(pReceivedPlayerInfo, 
																			pWarpOkMsg->uiPlayerNum, 
																			puiCharUniIDs, &pWarpOkMsg->uiPlayerNum, 6 );				

				// �� ������ ������ ����.			
				pRecvFItemInfoOut	=	(OnlineReceiveFieldItemInfo*)((CHAR*)pReceivedPlayerInfo + uiPlayersInfoMsgLength);

				if(pRecvFItemInfoIn != NULL)
					memcpy(pRecvFItemInfoOut, pRecvFItemInfoIn, sizeof(OnlineReceiveFieldItemInfo) * pWarpOkMsg->uiFieldItemNum);

				siLength			=	sizeof(OnResWarpOkMsg) + uiPlayersInfoMsgLength + 
										(sizeof(OnlineReceiveFieldItemInfo) * pWarpOkMsg->uiFieldItemNum);
				break;
			case ON_RET_NO:
				siLength			=	sizeof(OnResWarpMsg);
				break;
			}

			// �޽����� ���� ������� �̹� �����Ǿ� �ִ�.
			switch(siSendInfo)
			{
			case SO_SFM_CUSTOM:
				siSendCharNum		=	SI32(pParam5);
				memcpy(uiCharUniIDs, (UI16*)pParam6, siSendCharNum * sizeof(UI16));
				break;
			}
		}
		break;
	case ON_OFFICIAL_ANNOUNCEMENT:
		{
			CHAR		*pInLocalBuffer;
				
			// ��������.		
			pInLocalBuffer		=	(CHAR*)pParam1;
			siLength			=	SI32(pParam2);			

			memcpy(cLocalBuffer, pInLocalBuffer, siLength);
		}
		break;
	default:
		bRet	=	FALSE;
		break;
	}

	if(bRet == TRUE)
		SendMsg(siSendInfo, cLocalBuffer, siLength, siSendCharNum, uiCharUniIDs, bSendMe);

	return bRet;
}



//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	���������� �޽����� Send �Ѵ�.
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
VOID	BaseChar::SendMsg(SI32 siSendInfo, CHAR *cLocalBuffer, SI16 siLength, SI32 siSendCharNum, UI16 uiCharUniIDs[ON_STATUS_AREA_XSIZE * ON_STATUS_AREA_YSIZE], BOOL bSendMe)
{

	int i;
	
	UI16		uiAccounts[ ON_STATUS_AREA_XSIZE * ON_STATUS_AREA_YSIZE ];

	SOPlayer		*pPlayer;

	if(siLength > ON_MAX_LOCALBUFFER)
	{	
		return;
	}

	switch(siSendInfo)
	{
	case SO_SFM_CUSTOM:
		// ���ڷ� �Ѿ�Դ�.
		break;
	case SO_SFM_ONLYME:		
		// �����Ը� ������.
		siSendCharNum	=	1;
		uiCharUniIDs[0]	=	uiAccount;
		bSendMe			=	TRUE;		
		break;
	case SO_SFM_ONLYNEARCHAR:
	case SO_SFM_MEANDNEARCHAR:		
		// �˻��� ��ġ�� ���� ������ �Ѿ������ �Ǵ��Ѵ�.	

		// ������ ������ �ִ��÷��̾�� ������ �����Ѵ�.		
		siSendCharNum	=	pMyWorld->clISOMap.GetNearSectorPlayerList(uiMapID, siX, siY, uiCharUniIDs, ON_MAP_CHARID_ALLPLAYER);

		if(siSendInfo == SO_SFM_ONLYNEARCHAR)		bSendMe			=	FALSE;		
		else										bSendMe			=	TRUE;		
		break;	
	case SO_SFM_ALLCHAR:
		{

			for( i = 1; i < ON_MAX_CONNECTION+1; ++i )
			{
				if( pMyWorld->pPlayerOnAccount[i] ) pMyWorld->pPlayerOnAccount[i]->Write( cLocalBuffer, siLength );
			}
			
			return;

		}
		break;
	}

	int siSendNum;

	siSendNum = 0;
	for( i = 0; i < siSendCharNum; ++i ) {

		// �÷��̾�Ը� �޽����� ���� ���� �ִ�.
		if( ISPLAYER( uiCharUniIDs[ i ] ) == TRUE) {
			
			pPlayer	= pMyWorld->pPlayerOnAccount[ uiCharUniIDs[ i ] ];

			if( pPlayer && pPlayer->uiAccount ) {
				if( bSendMe == FALSE && pPlayer->siX == siX && pPlayer->siY == siY );
				else uiAccounts[ siSendNum++ ] = uiCharUniIDs[ i ];
			}
		}
	}

	if( siSendNum == 0 ) return;

	// ���Ͱ� �ƴ� ĳ���Ϳ��Ը� ������.
	// �̸� ���� ĳ���ʹ� ���� �Ǿ� �־�߸� �Ѵ�.	

	for( i = 0; i < siSendNum; ++i ) {

		pPlayer	= pMyWorld->pPlayerOnAccount[ uiAccounts[ i ] ];			

		pPlayer->Write( cLocalBuffer, siLength );

	}

}
