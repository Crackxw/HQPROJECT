#include "SOMain.h"
#include "SOWorld.h"
#include "SOPlayer.h"
#include <stdlib.h>

//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	���� �̵��ϴ� �޽����� �޾Ҵ�.
//	���� ����	:	2002-04-29 ���� 4:40:02 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������
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

	// ���� �̵� �޽���
	pReqMoveMapMsg	=	(OnReqMoveMapMsg*)cMsg;

	pResMoveMapMsg					=	(OnResMoveMapMsg*)cSendData;
	pResMoveMapMsg->dwMsgType		=	ON_RESPONSE_MOVEMAP;

	// �������̶�� ���� �̵��� �� ����.
	if( GetBattle() != NULL )																goto	END;

	// �ŷ����̶�� ���� �̵��� �� ����.
	if( IsTrading() == TRUE )																goto	END;


	// ��Ż�� ����� �� �ִ��� �˾ƿ´�.
	if(pMyWorld->Portal.CanUsePortal(pReqMoveMapMsg->uiPortalID) == FALSE)					goto	END;	

	// Potal�� Header�� ���´�.
	if((pPortalHeader = pMyWorld->Portal.GetPortal(pReqMoveMapMsg->uiPortalID)) == NULL)	goto	END;	

	// �����̳� �ſ뵵�� �Ǵ��� �˻��Ѵ�.	
	if((pPortalHeader->siNeedLV > IFollower.GetFollowerParameterLevel(0)) && (pPortalHeader->siNeedCredit > GetTradeGrade()))							goto	END;
	
	// �ð��밡 ��ȿ���� �˾ƿ´�.
	if(pPortalHeader->siStartTime != 0 && pPortalHeader->siEndTime != 0)
		if(((pMyWorld->RealWorldTime.uiHour >= pPortalHeader->siStartTime) && (pMyWorld->RealWorldTime.uiHour < pPortalHeader->siEndTime)) == FALSE)	goto	END;	

	// ���� ��ȿ���� �Ǵ��Ѵ�.
	if(pMyWorld->clISOMap.IsValidMapID(pPortalHeader->siPortalMoveMapIndex) == FALSE)	goto	END;	
	

	// ���� ��ǥ�� ���� ���� ĳ���͸� ������ ��ġ�� ã�� 
	// �� ��ġ�� ĳ���Ϳ��� �����ش�.
	ptPos	=	pMyWorld->clISOMap.SearchMap(pPortalHeader->siPortalMoveMapIndex, 
											pPortalHeader->siPortalMoveMapXPosition, 
											pPortalHeader->siPortalMoveMapYPosition, 
											ON_MAP_SI_CHAR, uiMoveableTileInfo);	
	
	// �����ϴ� ���� ����� �Ǿ����� �Ǵ��Ѵ�.
	if(ptPos.x == -1 || ptPos.y == -1)	goto	END;	

	// ����Ÿ��� ��� pVillage�� ������ �ش�. ����Ÿ��� �ƴϸ� NULL : ���簡 �߰�.
	stAccountInfo.pVillage	= pMyWorld->GetVillageByCode( pMyWorld->clISOMap.GetVillageCode( pPortalHeader->siPortalMoveMapIndex ) );	
	if( stAccountInfo.pVillage != NULL )
	{	
		// ����Ÿ��� ����.
		
		// �������� ����.
		EnterVillage( TRUE );		
	}
	else
	{
		// ����Ÿ��� �ƴϴ�.
		// ����Ÿ��� �־��ٸ� ����Ÿ����� ���������� ���ش�.
		if( CHECKSTATUS( siStatus, ON_PLAYER_STATUS_INVILLAGE ) == TRUE )
		{
			// �������� ������.
			LeaveVillage( TRUE );
		}	
	}
	
	// ��Ż �̿�� ���� �ʿ��ϴٸ� ���� ����.
	// ���� ���ٸ� �����̴�.
	if( pPortalHeader->i64PortalMoney > 0 )	
		if(DecreaseMoney(pPortalHeader->i64PortalMoney,FALSE) == FALSE)	goto	END;

	DBIKUsePortal DBInfo;

	DBInfo.siDBAccount = stAccountInfo.siDBAccount;
	DBInfo.uiDBSlot = stAccountInfo.uiSlot;
    DBInfo.siPortalNum = pReqMoveMapMsg->uiPortalID;
	DBInfo.moMoney = pPortalHeader->i64PortalMoney;

	pMyWorld->clDatabase->writeDBQuery(SO_DB_INFOKIND_USEPORTAL,&DBInfo,sizeof(DBInfo));

	// �������ʹ� ���������� �ٸ� ������ �̵� ��Ų��.			

	// �޽����� ĳ��Ʈ �Ѵ�.
	pResMoveMapOkMsg				=	(OnResMoveMapOkMsg*)pResMoveMapMsg;
	pResMoveMapOkMsg->dwResponse	=	ON_RET_OK;
	pResMoveMapOkMsg->mnMoney		=	siMoney;

	// �ʿ��� ID�� �����Ѵ�.
	if(DelIDOnMap( 9 ) == FALSE)
		writeInfoToFile("DelIDOnMap Failed.txt", "6 [%u] [%u]", uiAccount, siStatus);
//		pMyWorld->clServer->WriteInfo("..\\DelIDOnMap Failed.txt", "6 [%u] [%u]", uiAccount, siStatus);
	
	ChangeMap(pPortalHeader->siPortalMoveMapIndex, SI16(ptPos.x), SI16(ptPos.y));

	Quest_AddQuestObject_PortalUse(pPortalHeader->siPortalMoveMapIndex);
//	writeInfoToFile( "Portal_Test.txt", "MapIndex = %d, MoveMapIndex = %d\n", pPortalHeader->siPortalMapIndex , pPortalHeader->siPortalMoveMapIndex );

	// ���� ��ġ�� ���Ϳ��� ID�� �����Ѵ�.
	if(SetIDOnMap( 10 ) == FALSE)
		writeInfoToFile("SetIDOnMap Failed.txt", "7");
//		pMyWorld->clServer->WriteInfo("..\\SetIDOnMap Failed.txt", "7");
	

	// ������ ĳ���Ϳ� �������� ������ ���´�.
	pResMoveMapOkMsg->uiAroundCharNum	=	pMyWorld->clISOMap.GetNearSectorPlayerList(uiMapID, siX, siY, uiAroundCharInfo, ON_MAP_CHARID_FIELD);
	pResMoveMapOkMsg->uiAroundItemNum	=	pMyWorld->clISOMap.GetNearSectorItemList(uiMapID, siX, siY, AroundFieldItemInfo);		


	pSendPlayerInfo			=	(CHAR*)pResMoveMapOkMsg + sizeof(OnResMoveMapOkMsg);
	uiPlayersInfoMsgLength	=	MakeOnReceivedPlayerInfoPacket(pSendPlayerInfo, pResMoveMapOkMsg->uiAroundCharNum, uiAroundCharInfo, &pResMoveMapOkMsg->uiAroundCharNum, 0);
	
	pSendFieldItemInfo		=	(OnlineReceiveFieldItemInfo*)((CHAR*)pSendPlayerInfo + uiPlayersInfoMsgLength);			
	memcpy(pSendFieldItemInfo, AroundFieldItemInfo, sizeof(OnlineReceiveFieldItemInfo) * pResMoveMapOkMsg->uiAroundItemNum);		

	// ������ �ϴ� ����� �����Ѵ�.
	siSendSize	=	sizeof(OnResMoveMapOkMsg) + uiPlayersInfoMsgLength + 
					(sizeof(OnlineReceiveFieldItemInfo) * pResMoveMapOkMsg->uiAroundItemNum);

	if(siSendSize >= ON_MAX_LOCALBUFFER)
	{
		printf("siSendSize >= ON_MAX_LOCALBUFFER. message size : [%d]\n", siSendSize);
	}

	// �����Ѵ�
	Write(pResMoveMapOkMsg, siSendSize);

	// ����â�� �ݴ´�.
	CloseTrade();	

	// ��Ż�� ����ߴٰ� �˷��ش�.
	pMyWorld->Portal.UsePortal(pReqMoveMapMsg->uiPortalID);

	bSuccess = TRUE;

END:

	// �������� ��� �����ߴٰ� �޽����� �����ش�.
	if(bSuccess == FALSE)
	{
		pResMoveMapMsg->dwResponse	=	ON_RET_NO;
		siSendSize					=	sizeof(OnResMoveMapMsg);	

		Write(pResMoveMapMsg, siSendSize);
	}
}
