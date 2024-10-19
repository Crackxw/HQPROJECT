#include "SOMain.h"
#include "SOWorld.h"
#include "SOPlayer.h"
#include <stdlib.h>

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	맵을 이동하는 메시지를 받았다.
//	수정 일자	:	2002-04-29 오후 4:40:02 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
VOID	SOPlayer::RecvMoveMap()
{
	OnReqMoveMapMsg				*pReqMoveMapMsg;
	OnResMoveMapMsg				*pResMoveMapMsg;
	OnResMoveMapOkMsg			*pResMoveMapOkMsg;
	BOOL						bSuccess	=	FALSE;		
	POINT						ptPos;	
	CHAR						cSendData[ON_MAX_LOCALBUFFER];	
	UI16						uiAroundCharInfo[ON_STATUS_AREA_XSIZE * ON_STATUS_AREA_YSIZE];
	OnlineReceiveFieldItemInfo	AroundFieldItemInfo[ON_STATUS_AREA_XSIZE * ON_STATUS_AREA_YSIZE];
	CHAR						*pSendPlayerInfo;
	UI32						uiPlayersInfoMsgLength;
	OnlineReceiveFieldItemInfo	*pSendFieldItemInfo;
	SI32						siSendSize;		
	PortalHeader				*pPortalHeader;

	// 맵의 이동 메시지
	pReqMoveMapMsg	=	(OnReqMoveMapMsg*)cMsg;

	pResMoveMapMsg					=	(OnResMoveMapMsg*)cSendData;
	pResMoveMapMsg->dwMsgType		=	ON_RESPONSE_MOVEMAP;

	// 전투중이라면 맵을 이동할 수 없다.
	if( GetBattle() != NULL )																goto	END;

	// 거래중이라면 맵을 이동할 수 없다.
	if( IsTrading() == TRUE )																goto	END;


	// 포탈을 사용할 수 있는지 알아온다.
	if(pMyWorld->Portal.CanUsePortal(pReqMoveMapMsg->uiPortalID) == FALSE)					goto	END;	

	// Potal의 Header를 얻어온다.
	if((pPortalHeader = pMyWorld->Portal.GetPortal(pReqMoveMapMsg->uiPortalID)) == NULL)	goto	END;	

	// 레벨이나 신용도가 되는지 검사한다.	
	if((pPortalHeader->siNeedLV > IFollower.GetFollowerParameterLevel(0)) && (pPortalHeader->siNeedCredit > GetTradeGrade()))							goto	END;
	
	// 시간대가 유효한지 알아온다.
	if(pPortalHeader->siStartTime != 0 && pPortalHeader->siEndTime != 0)
		if(((pMyWorld->RealWorldTime.uiHour >= pPortalHeader->siStartTime) && (pMyWorld->RealWorldTime.uiHour < pPortalHeader->siEndTime)) == FALSE)	goto	END;	

	// 맵이 유효한지 판단한다.
	if(pMyWorld->clISOMap.IsValidMapID(pPortalHeader->siPortalMoveMapIndex) == FALSE)	goto	END;	
	

	// 기준 좌표를 토대로 실제 캐릭터를 설정할 위치를 찾고 
	// 그 위치를 캐릭터에게 보내준다.
	ptPos	=	pMyWorld->clISOMap.SearchMap(pPortalHeader->siPortalMoveMapIndex, 
											pPortalHeader->siPortalMoveMapXPosition, 
											pPortalHeader->siPortalMoveMapYPosition, 
											ON_MAP_SI_CHAR, uiMoveableTileInfo);	
	
	// 워프하는 곳이 제대로 되었는지 판단한다.
	if(ptPos.x == -1 || ptPos.y == -1)	goto	END;	

	// 저잣거리일 경우 pVillage를 지정해 준다. 저잣거리가 아니면 NULL : 성재가 추가.
	stAccountInfo.pVillage	= pMyWorld->GetVillageByCode( pMyWorld->clISOMap.GetVillageCode( pPortalHeader->siPortalMoveMapIndex ) );	
	if( stAccountInfo.pVillage != NULL )
	{	
		// 저잣거리에 들어갔다.
		
		// 마을에도 들어간다.
		EnterVillage( TRUE );		
	}
	else
	{
		// 저잣거리가 아니다.
		// 저잣거리에 있었다면 저잣거리에서 빠져나오게 해준다.
		if( CHECKSTATUS( siStatus, ON_PLAYER_STATUS_INVILLAGE ) == TRUE )
		{
			// 마을에서 나간다.
			LeaveVillage( TRUE );
		}	
	}
	
	// 포탈 이용시 돈이 필요하다면 돈을 뺀다.
	// 돈이 없다면 실패이다.
	if( pPortalHeader->i64PortalMoney > 0 )	
		if(DecreaseMoney(pPortalHeader->i64PortalMoney,FALSE) == FALSE)	goto	END;

	DBIKUsePortal DBInfo;

	DBInfo.siDBAccount = stAccountInfo.siDBAccount;
	DBInfo.uiDBSlot = stAccountInfo.uiSlot;
    DBInfo.siPortalNum = pReqMoveMapMsg->uiPortalID;
	DBInfo.moMoney = pPortalHeader->i64PortalMoney;

	pMyWorld->clDatabase->writeDBQuery(SO_DB_INFOKIND_USEPORTAL,&DBInfo,sizeof(DBInfo));

	// 이제부터는 실질적으로 다른 맵으로 이동 시킨다.			

	// 메시지를 캐스트 한다.
	pResMoveMapOkMsg				=	(OnResMoveMapOkMsg*)pResMoveMapMsg;
	pResMoveMapOkMsg->dwResponse	=	ON_RET_OK;
	pResMoveMapOkMsg->mnMoney		=	siMoney;

	// 맵에서 ID를 삭제한다.
	if(DelIDOnMap( 9 ) == FALSE)
		writeInfoToFile("DelIDOnMap Failed.txt", "6 [%u] [%u]", uiAccount, siStatus);
//		pMyWorld->clServer->WriteInfo("..\\DelIDOnMap Failed.txt", "6 [%u] [%u]", uiAccount, siStatus);
	
	ChangeMap(pPortalHeader->siPortalMoveMapIndex, SI16(ptPos.x), SI16(ptPos.y));

	Quest_AddQuestObject_PortalUse(pPortalHeader->siPortalMoveMapIndex);
//	writeInfoToFile( "Portal_Test.txt", "MapIndex = %d, MoveMapIndex = %d\n", pPortalHeader->siPortalMapIndex , pPortalHeader->siPortalMoveMapIndex );

	// 현재 위치의 섹터에서 ID를 설정한다.
	if(SetIDOnMap( 10 ) == FALSE)
		writeInfoToFile("SetIDOnMap Failed.txt", "7");
//		pMyWorld->clServer->WriteInfo("..\\SetIDOnMap Failed.txt", "7");
	

	// 주위에 캐릭터와 아이템의 정보를 얻어온다.
	pResMoveMapOkMsg->uiAroundCharNum	=	pMyWorld->clISOMap.GetNearSectorPlayerList(uiMapID, siX, siY, uiAroundCharInfo, ON_MAP_CHARID_FIELD);
	pResMoveMapOkMsg->uiAroundItemNum	=	pMyWorld->clISOMap.GetNearSectorItemList(uiMapID, siX, siY, AroundFieldItemInfo);		


	pSendPlayerInfo			=	(CHAR*)pResMoveMapOkMsg + sizeof(OnResMoveMapOkMsg);
	uiPlayersInfoMsgLength	=	MakeOnReceivedPlayerInfoPacket(pSendPlayerInfo, pResMoveMapOkMsg->uiAroundCharNum, uiAroundCharInfo, &pResMoveMapOkMsg->uiAroundCharNum, 0);
	
	pSendFieldItemInfo		=	(OnlineReceiveFieldItemInfo*)((CHAR*)pSendPlayerInfo + uiPlayersInfoMsgLength);			
	memcpy(pSendFieldItemInfo, AroundFieldItemInfo, sizeof(OnlineReceiveFieldItemInfo) * pResMoveMapOkMsg->uiAroundItemNum);		

	// 보내야 하는 사이즈를 결정한다.
	siSendSize	=	sizeof(OnResMoveMapOkMsg) + uiPlayersInfoMsgLength + 
					(sizeof(OnlineReceiveFieldItemInfo) * pResMoveMapOkMsg->uiAroundItemNum);

	if(siSendSize >= ON_MAX_LOCALBUFFER)
	{
		printf("siSendSize >= ON_MAX_LOCALBUFFER. message size : [%d]\n", siSendSize);
	}

	// 전송한다
	Write(pResMoveMapOkMsg, siSendSize);

	// 교역창을 닫는다.
	CloseTrade();	

	// 포탈을 사용했다고 알려준다.
	pMyWorld->Portal.UsePortal(pReqMoveMapMsg->uiPortalID);

	bSuccess = TRUE;

END:

	// 실패했을 경우 실패했다고 메시지를 보내준다.
	if(bSuccess == FALSE)
	{
		pResMoveMapMsg->dwResponse	=	ON_RET_NO;
		siSendSize					=	sizeof(OnResMoveMapMsg);	

		Write(pResMoveMapMsg, siSendSize);
	}
}
