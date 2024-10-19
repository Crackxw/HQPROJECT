#include "SOMain.h"
#include "SOWorld.h"
#include "SOPlayer.h"
#include "ParametaBox.h"


//����������������������������������������������������������������������������
// �÷��̾ �̵���Ų��.
//����������������������������������������������������������������������������


void printsectorlist( strSector **pSector )
{
	int i;
	for( i = 0; i < 9; i++ ) {
		if( pSector[ i ] != NULL ) {
			printf( "[%d,%d]", pSector[ i ]->uiX, pSector[ i ]->uiY );
		}
	}
}

VOID SOPlayer::RecvPosition()
{
	OnReqMyCharMoveMsg	*pMyCharMoveMsg;			
	BOOL				bSuccess  = FALSE;

	pMyCharMoveMsg		=	(OnReqMyCharMoveMsg*)cMsg;

	if(uiBoardShipAccount != 0)																						goto	END;		// ���� �迡 ž�µǾ� �ִ� ���¶�� �̵����� ���Ѵ�.
	if(GetBattle() != NULL)																							goto	END;		// ���� ���� ������̸� �̵����� ���Ѵ�.

	if(pMyWorld->clISOMap.MoveChar( this, pMyCharMoveMsg->uiX, pMyCharMoveMsg->uiY, pMyCharMoveMsg->bWarUnit,uiMoveableTileInfo) == FALSE)	goto	END;		// ĳ���͸� �̵� ��Ų��.

	// �̵��� �����Ͽ���.
	bSuccess	=	TRUE;

END:

	if(bSuccess == FALSE)
	{
		// �� ��ġ���� ĳ���͸� ������ �� ����.
		// ���� ��ġ�� �̵��϶�� �޽����� �����ش�.
		SendFieldMsg( ON_RESPONSE_POSITION, SO_SFM_ONLYME, LPVOID( &uiAccount ), LPVOID( 1 ) );
	}	
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	���� �Ѵٴ� �޽����� �޾Ҵ�.			
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
BOOL	SOPlayer::RecvWarp()
{
	BOOL			bSuccess		=	FALSE;
	OnReqWarpMsg	*pReqWarpMsg;
	strWarpInfo		*pstWarpInfo;

	pReqWarpMsg	=	(OnReqWarpMsg*)cMsg;

	pstWarpInfo	=	pMyWorld->m_clWarpParser.GetWarpInfo( pReqWarpMsg->uiWarpCode );

	// ������ ĳ������ ���̵� �ڽ��� ���̵��� ���
	bSuccess	=	Warp( pstWarpInfo->uiMapIndex, pstWarpInfo->uiX, pstWarpInfo->uiY );

	if( bSuccess )		SendFieldMsg( ON_RESPONSE_WARP, SO_SFM_ONLYME, LPVOID(ON_RET_OK) );
	else				SendFieldMsg( ON_RESPONSE_WARP, SO_SFM_ONLYME, LPVOID(ON_RET_NO) );

	return	bSuccess;
}


//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	������ �ϰ� ������ ����Ÿ�� �޴´�.
//				���ڷ� ������ siX, siY�� ������ �Ǵ� ��ġ����
//				�ݵ�� �� ��ġ�� ������ �Ǵ� ���� �ƴϴ�.
//				�� ��ġ�� ��ȿ�� ��ġ�� �ƴ� ��쿡�� �� ������ ��ġ�� ���� �� ���� �ִ�.
//				������ ���� ���� ��쿡�� �� �Լ������� �ش� �÷��̾�� ���� �޽����� ������
//				������ ���� ���� ��쿡�� �� �Լ��� ȣ�� �ϴ� �κп��� �޽����� �����ش�.
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
BOOL	SOPlayer::Warp(UI16 uimapid, SI32 six, SI32 siy)
{
	UI16							uiCharFItemInfo[2];			// 0���� ĳ���� ��, 1���� ������ ��,
	UI16							uiCharUniIDs[ON_STATUS_AREA_XSIZE * ON_STATUS_AREA_YSIZE];	
	OnlineReceiveFieldItemInfo		FieldItemInfo[ON_STATUS_AREA_XSIZE * ON_STATUS_AREA_YSIZE];
	BOOL							bWarpSuccess = FALSE;
	UI16							uiPrevMapID;
	SI32							siPrevX, siPrevY;	
	POINT							ptWarpXY;

	// ���� �ݵ�� �ʵ� �� �־�� �Ѵ�.
	if( CHECKSTATUS( siStatus, ON_PLAYER_STATUS_INFIELD ) == FALSE )	return	FALSE;
	//if(siStatus	!= ON_PLAYER_STATUS_INFIELD)				return FALSE;

	// ������ ���� �ݵ�� ��ȿ�ؾ� �Ѵ�.
	if(pMyWorld->clISOMap.IsValidMapID(uimapid) == FALSE)	return	FALSE;

	// ������ ���� ã�´�.
	ptWarpXY	=	pMyWorld->clISOMap.SearchMap(uimapid, six, siy, ON_MAP_SI_CHAR, uiMoveableTileInfo);

	// ������ ���� ������ �ڸ��� ã�� ���ؼ� �����ߴ�.				
	if((ptWarpXY.x == -1) && (ptWarpXY.y == -1))	return FALSE;

	// ���� ���� ID�� �����Ѵ�.
	uiPrevMapID		=	uiMapID;
	siPrevX			=	siX;
	siPrevY			=	siY;	
	
	// ���� ���� ��ġ���� �� ID�� �����.
	if( DelIDOnMap( 4 ) == FALSE)
		writeInfoToFile("DelIDOnMap Failed.txt", "4 [%u]", uiAccount);
//		pMyWorld->clServer->WriteInfo("..\\DelIDOnMap Failed.txt", "4 [%u]", uiAccount);

	// �켱 ���� �����Ѵ�.
	// �����Ϸ��� ���� ������ FALSE�� �����Ѵ�.
	// ����ڴ� ChangeMap�� �ݵ�� ȣ�����־�� �Ѵ�.
	if(ChangeMap(uimapid, SI16(ptWarpXY.x), SI16(ptWarpXY.y)) == TRUE)			
	{		
		// ���� ���ο� ��ġ�� �� ID�� �����Ѵ�.
		if(SetIDOnMap( 5 ) == FALSE)
			writeInfoToFile("SetIDOnMap Failed.txt", "6");		
//			pMyWorld->clServer->WriteInfo("..\\SetIDOnMap Failed.txt", "6");		
		
		// �� ������ �ִ� ĳ������ ID�� �������� ���´�.
		uiCharFItemInfo[0]		=	pMyWorld->clISOMap.GetNearSectorPlayerList(uiMapID, siX, siY, uiCharUniIDs, ON_MAP_CHARID_FIELD);
		uiCharFItemInfo[1]		=	pMyWorld->clISOMap.GetNearSectorItemList(uiMapID, siX, siY, FieldItemInfo);				

		// �켱 ������ �� ������ ĳ���Ϳ� �������� ������ �����ش�.
		SendFieldMsg(ON_RESPONSE_WARP, SO_SFM_ONLYME, 
						LPVOID(ON_RET_OK),
						LPVOID(uiCharFItemInfo), 
						LPVOID(uiCharUniIDs), 
						LPVOID(FieldItemInfo));		

		bWarpSuccess	=	TRUE;	
	}

	if(bWarpSuccess == FALSE)
	{
		// ���ο� �������� �����ϴµ� �����ߴٸ�
		// ���� �ڸ��� �ٽ� �����Ѵ�.
		BaseChar::Warp(uiPrevMapID, siPrevX, siPrevY);
	}

	return bWarpSuccess;
}

//----------------------------------------------------------------------------------------------------------------
//	����	:	Ư�� �÷��̾��� ������ ��û�Ѵ�
//----------------------------------------------------------------------------------------------------------------
VOID	SOPlayer::RecvPlayerInfo()
{
	OnReqPlayerInfoMsg		*pReqPlayerInfoMsg;
	UI16					*puiCharsID;
	UI16					uiCharIDs[20];					// �ѹ��� 20�� �̻��� �������� �ʴ´�.
	UI16					uiCharNum;
	SI32					i;

	// ������ �����ش�.
	pReqPlayerInfoMsg		=	(OnReqPlayerInfoMsg*)cMsg;	
	puiCharsID				=	(UI16*)((CHAR*)pReqPlayerInfoMsg + sizeof(OnReqPlayerInfoMsg));
	
	ZeroMemory(uiCharIDs, sizeof(uiCharIDs));
	
	// ���� ��ȿ�� ĳ���͸� �����Ѵ�.
	for(uiCharNum = 0, i = 0; i < pReqPlayerInfoMsg->uiCharNum; i++)
	{
		if(IsValidID(puiCharsID[i]) == TRUE)
		{			
			uiCharIDs[uiCharNum]	=	puiCharsID[i];
			uiCharNum++;

			// ����� 20������ �����ϴ�.
			if(uiCharNum >= 20)
				break;
		}
	}

	// �޽����� �����ش�.
	SendFieldMsg(ON_RESPONSE_PLAYER_INFO, SO_SFM_ONLYME, LPVOID(uiCharNum), LPVOID(uiCharIDs), LPVOID(uiGuildID));	
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	ĳ������ �� ������ ��û�Ѵٴ� �޽����� �޾Ҵ�.
//	���� ����	:	2002-08-15 ���� 3:05:51 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
VOID	SOPlayer::RecvCharDetailInfo()
{
	OnReqCharDetailInfoMsg		*pReqCharDetailInfoMsg;
	SOPlayer					*pPlayer;

	pReqCharDetailInfoMsg		=	(OnReqCharDetailInfoMsg*)cMsg;

	if(ISPLAYER(pReqCharDetailInfoMsg->uiAccount) == TRUE)
	{
		if((pPlayer = pMyWorld->GetPlayer(pReqCharDetailInfoMsg->uiAccount)) != NULL)
		{
			// �÷��̾ ��ȿ�ϴ�.
			// ĳ������ �� ������ �����ش�.
			SendFieldMsg(ON_RESPONSE_CHAR_DETAILINFO, SO_SFM_ONLYME, LPVOID(ON_RET_OK), LPVOID(pPlayer));
		}
		else
		{
			// �÷��̾ ��ȿ���� �ʴ�.
			SendFieldMsg(ON_RESPONSE_CHAR_DETAILINFO, SO_SFM_ONLYME, LPVOID(ON_RET_NO));
		}		
	}
	else if(ISMONSTER(pReqCharDetailInfoMsg->uiAccount) == TRUE)
	{
	}
	else if(ISSHIP(pReqCharDetailInfoMsg->uiAccount) == TRUE)
	{
	}
}

//����������������������������������������������������������������������������
//	����	:	������ ���ٴ� �޽����� �޾Ҵ�.
//����������������������������������������������������������������������������
VOID SOPlayer::RecvEnterVillage()
{
	OnReqGotoVillageMsg		*pReqGotoVillageMsg;	
	DWORD					dwResponse = ON_RET_NO;
	UI32					uiVillageGuildID;
	cltGuild*				pclGuild;

	pclGuild = pMyWorld->clIGuild.GetGuildByID(uiGuildID);
	pReqGotoVillageMsg	=	(OnReqGotoVillageMsg*)cMsg;
		
	// ���� ���� ���� ������̰ų� �����߿� ������ ���� ���Ѵ�.
	if( GetBattle() != NULL )			goto	END;	

	// ���� ���� �迡 ž�����̸� ���� ���Ѵ�.
	if( uiBoardShipAccount != 0)		goto	END;

	// �켱 ��ȿ�� �������� �˾ƿ´�.
	stAccountInfo.pVillage	=	pMyWorld->GetVillageByCode(pReqGotoVillageMsg->uiVillageCode);				// ������ ã�´�.
	if(stAccountInfo.pVillage == NULL)	goto	END;

	if( (stAccountInfo.pVillage->uiMapID != uiMapID) ||
		( (abs(stAccountInfo.pVillage->siVillageX - siX) > 10) && (abs(stAccountInfo.pVillage->siVillageY - siY) > 10) ) )
	{
		writeInfoToFile( "WarpHacking.txt", "DBAccount(%d)  szID(%s)  Player MapIndex(%d) X(%d) Y(%d) Try to Enter village(%s)",
						stAccountInfo.siDBAccount, szID, uiMapID, siX, siY, stAccountInfo.pVillage->GetVillageName() );
		goto	END;
	}

	// ������ Guild ID�� ���Ѵ�.
	uiVillageGuildID		=	stAccountInfo.pVillage->GetGuildNID();

	// �� ������ ����̸� �� �� ����.
	if((uiGuildID != 0) && (uiVillageGuildID != 0) && (siClassInGuild != pMyWorld->clIGuildClass.GetJWJCode()))
	{
		if(pMyWorld->clIGuild.IsEnemyRelationWar(uiGuildID,uiVillageGuildID) == TRUE || pMyWorld->clIGuild.IsEnemyGuildBoth(uiGuildID, uiVillageGuildID) == TRUE)
		{
			dwResponse	=	ON_RET_ENEMY_GUILD_VILLAGE;
			goto	END;
		}
	}

	
	// �ʿ��� �� ID�� �����.
	// �ݵ�� ������ �������� ���־�� �Ѵ�.
	if(DelIDOnMap( 6 ) == FALSE)
		writeInfoToFile("DelIDOnMap Failed.txt", "5 [%u] [%d] [%d] [%d]", uiAccount, uiMapID, siX, siY);
//		pMyWorld->clServer->WriteInfo("..\\DelIDOnMap Failed.txt", "5 [%u] [%d] [%d] [%d]", uiAccount, uiMapID, siX, siY);

	// ������ ����.
	EnterVillage();	

	dwResponse	=	ON_RET_OK;
END:
	// ������ �� �� ����.
	switch(dwResponse)
	{
	case ON_RET_OK:
		// ���� �÷��̾�� �켱 �˷��ش�.
		SendFieldMsg(ON_RESPONSE_ENTER_VILLAGE, SO_SFM_ONLYNEARCHAR, LPVOID(dwResponse));	
			
		// �׸��� �����Ե� �˷��ش�.		
		SendFieldMsg(ON_RESPONSE_ENTER_VILLAGE, SO_SFM_ONLYME, LPVOID(dwResponse), LPVOID(stAccountInfo.pVillage));
		break;
	case ON_RET_NO:
	case ON_RET_ENEMY_GUILD_VILLAGE:		
		SendFieldMsg(ON_RESPONSE_ENTER_VILLAGE, SO_SFM_ONLYME, LPVOID(dwResponse));				
		break;
	}
	
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	������ ����
//	���� ����	:	2002-09-28 ���� 11:35:00 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������
BOOL	SOPlayer::EnterVillage( BOOL bIsVillageField )
{
	if(stAccountInfo.pVillage->EnterVillage( this ) == TRUE)
	{
		// ������ �� ���·� siStatus�� �����Ѵ�.
		//siStatus							=	ON_PLAYER_STATUS_INVILLAGE;		
		if( bIsVillageField == TRUE)
		{
			// ����Ÿ��̴�.
			ADDSTATUS( siStatus, ON_PLAYER_STATUS_INVILLAGE );
		}
		else
		{
			// ����Ÿ��� �ƴϴ�.
			SETSTATUS( siStatus, ON_PLAYER_STATUS_INVILLAGE );
		}			
		
		stAccountInfo.uiStatusInVillage		=	INVILLAGE_STATUS_NONE;	

		// ������ �������� 5���� �����ٸ� DB�� �������ش�.
		if( (int)(pMyWorld->dwtimeGetTime - m_latestSavedPosTime) > 300000 ) savePosToDB();		

		// ����â�� �ݴ´�.
		CloseTrade();

		// Ÿ �������� ��Ƽ ��û���̰ų� 
		// �ٸ� ������ ��Ƽ ��û�� �޾Ҵٸ� �̸� ����Ѵ�.
		CloseRequestingJoinParty();

		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	�������� ������.
//	���� ����	:	2002-09-28 ���� 11:38:37 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������
VOID SOPlayer::LeaveVillage( BOOL bIsVillageField )
{	
	// �ʵ忡 �ִ� ���·� siStatus�� �����Ѵ�.
	//siStatus							=	ON_PLAYER_STATUS_INFIELD;	
	if( bIsVillageField == TRUE)
	{
		// ����Ÿ��̴�.
		if( CHECKSTATUS( siStatus, ON_PLAYER_STATUS_INVILLAGE ) == TRUE)
		{
			REMOVESTATUS( siStatus, ON_PLAYER_STATUS_INVILLAGE );
		}
	}
	else
	{
		// ����Ÿ��� �ƴϴ�.
		SETSTATUS( siStatus, ON_PLAYER_STATUS_INFIELD );
	}			
	
	stAccountInfo.pVillage				=	NULL;	
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	�������� �����ٴ� �޽����� �޾Ҵ�.
//	���� ����	:	2002-11-19 ���� 9:21:47 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
VOID	SOPlayer::RecvThrowItem()
{
	OnReqThrowItemMsg		*pReqThrowItemMsg;
	BOOL					bSuccess = FALSE;
	POINT					ptThrowXY;
	CHAR					szDBArg[ 1024 ];

	pReqThrowItemMsg	=	(OnReqThrowItemMsg*)cMsg;

	//pReqThrowItemMsg->

	// �������� ���� �� �ִ� �ڸ��� �ִ��� �Ǵ��Ѵ�.
	// ---- 20050216 - ������ ����
	ptThrowXY	=	pMyWorld->clISOMap.SearchMap( uiMapID, siX, siY, ON_MAP_SI_ITEM, pReqThrowItemMsg->uiItemID, pReqThrowItemMsg->uiQuantity );
	// ----
//	ptThrowXY	=	pMyWorld->clISOMap.SearchMap( uiMapID, siX, siY, ON_MAP_SI_ITEM, pReqThrowItemMsg->uiItemID );

	// �������� ���� �� �ִ� ��ǥ�� ã�� ���ߴ�.
	if( ptThrowXY.x == -1 && ptThrowXY.y == -1 )	goto	END;

	// ���� �� ���ϰ� �ִ���, �� ���ϰ� �� �������� ������ �ִ����� �����غ��� �ȴ�.
	// ���� ���� �����ߴٸ� �������� �����µ� �����Ѵ�.
	if( IFollower.DelItem( pReqThrowItemMsg->uiFollowerID, pReqThrowItemMsg->uiItemID,
							pReqThrowItemMsg->uiQuantity ) == ONLINEFOLLOWER_DELITEMRETURNTYPE_FALSE )	goto	END;	
	
	// �ʿ� �� �������� �����Ѵ�.
	pMyWorld->clISOMap.SetItem( uiMapID, ptThrowXY.x, ptThrowXY.y, pReqThrowItemMsg->uiItemID, pReqThrowItemMsg->uiQuantity, FALSE );

	// DB�� �����Ѵ�.
	ZeroMemory( szDBArg, sizeof( szDBArg ) );
	sprintf( szDBArg, "%d, %d, %d, %d, %d, %d, %u, %u", stAccountInfo.siDBAccount, stAccountInfo.uiSlot, 
														pReqThrowItemMsg->uiFollowerID, uiMapID, ptThrowXY.x, ptThrowXY.y,
														pReqThrowItemMsg->uiItemID, pReqThrowItemMsg->uiQuantity );
	pMyWorld->clDatabase->writeDBQuery( SO_DB_INFOKIND_DROPITEM, LPVOID( szDBArg ), strlen( szDBArg) + 1 );			
	

	bSuccess	=	TRUE;

END:

	if( bSuccess == TRUE )
	{			
		
		
		
		// ������ �÷��̾�� ����� �ʵ��� ������ ������ ������.
		
		OnChangeFieldItemMsg Msg;
		
		Msg.dwMsgType = ON_CHANGE_FIELDITEM;
		Msg.siX = ptThrowXY.x;
		Msg.siY = ptThrowXY.y;
		//Msg.uiItemID = pReqThrowItemMsg->uiItemID;
		//Msg.uiItemQuantity = pReqThrowItemMsg->uiQuantity;

		// ����: ���� �ʿ� �ִ� �����۰� �������� ���� �����Ƿ� ���� ������ ���´�.
		pMyWorld->clISOMap.GetItemID( uiMapID, ptThrowXY.x, ptThrowXY.y, &Msg.uiItemID, &Msg.uiItemQuantity );

		
		UI16 uiCharUniIDs[ON_STATUS_AREA_XSIZE * ON_STATUS_AREA_YSIZE];
		
		
		SI32 siSendCharNum;
		
		siSendCharNum	=	pMyWorld->clISOMap.GetNearSectorPlayerList(uiMapID, Msg.siX, Msg.siY, uiCharUniIDs, ON_MAP_CHARID_ALLPLAYER);
		
		
		
		SendFieldMsg( ON_CHANGE_FIELDITEM, SO_SFM_CUSTOM, LPVOID( siSendCharNum ), LPVOID( uiCharUniIDs ), 
			LPVOID( &Msg), NULL );

		// �������� �����µ� �����ߴ�.
		
		SendFieldMsg( ON_RESPONSE_THROWITEM, SO_SFM_ONLYME, LPVOID( ON_RET_OK ) );	
		
		
			/*		SendFieldMsg( ON_CHANGE_FIELDITEM, SO_SFM_MEANDNEARCHAR, LPVOID( ptThrowXY.x ), LPVOID( ptThrowXY.y ), 
		LPVOID( pReqThrowItemMsg->uiItemID ), LPVOID( pReqThrowItemMsg->uiQuantity ) );		*/
	}
	else
	{
		// �������� �����µ� �����ߴ�.
		SendFieldMsg( ON_RESPONSE_THROWITEM, SO_SFM_ONLYME, LPVOID( ON_RET_NO ) );
	}
}


//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	�������� �ݴ´�.
//	���� ����	:	2002-11-19 ���� 10:54:18 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
VOID	SOPlayer::RecvPickUpItem()
{
	OnReqPickUpItemMsg		*pReqPickUpItemMsg;
	BOOL					bSuccess = FALSE;
	UI16					uiItemID, uiItemQuantity;
	MyItemData				ItemData;
	CHAR					szDBArg[ 1024 ];


	pReqPickUpItemMsg	=	(OnReqPickUpItemMsg*)cMsg;

	if( pMyWorld->clISOMap.GetItemID( uiMapID, pReqPickUpItemMsg->uiX, pReqPickUpItemMsg->uiY, &uiItemID, &uiItemQuantity ) == FALSE )		goto	END;
	if( ( uiItemID != pReqPickUpItemMsg->uiItemID ) || ( uiItemQuantity < pReqPickUpItemMsg->uiQuantity ) )									goto	END;	
	if( IFollower.CanAddItem( pReqPickUpItemMsg->uiFollowerID, pReqPickUpItemMsg->uiItemID, 
							pReqPickUpItemMsg->uiQuantity, pReqPickUpItemMsg->uiPosInInventory) == FALSE )									goto	END;
	

	// �������� �κ��丮�� �߰� ��Ų��.
	ItemData.uiID				=	pReqPickUpItemMsg->uiItemID;
	ItemData.uiQuantity			=	pReqPickUpItemMsg->uiQuantity;
	ItemData.siPosInInventory	=	SI16( pReqPickUpItemMsg->uiPosInInventory );
	IFollower.AddItem( pReqPickUpItemMsg->uiFollowerID, &ItemData );

	// �������� ������ ������Ʈ �Ѵ�.
	uiItemQuantity				=	uiItemQuantity - pReqPickUpItemMsg->uiQuantity;
	
	// �켱 �ʿ��� �������� ���ش�.
	pMyWorld->clISOMap.DelItem( this, uiMapID, pReqPickUpItemMsg->uiX, pReqPickUpItemMsg->uiY, FALSE );

	// ���� �������� ���� �ִٸ� ���� �������� �ٽ� �������ش�.
	if( uiItemQuantity != 0 )
	{
		// �������� �Ϻθ� ��������.
		// �ʿ� ������ �������� ������ ������Ʈ�Ѵ�.
		pMyWorld->clISOMap.SetItem( uiMapID, pReqPickUpItemMsg->uiX, pReqPickUpItemMsg->uiY, pReqPickUpItemMsg->uiItemID, uiItemQuantity, FALSE );
	}				

	// DB�� �����Ѵ�.
	ZeroMemory( szDBArg, sizeof( szDBArg ) );
	sprintf( szDBArg, "%d, %d, %d, %d, %d, %d, %d, %u, %u", stAccountInfo.siDBAccount, stAccountInfo.uiSlot, 
															pReqPickUpItemMsg->uiFollowerID, ItemData.siPosInInventory, 
															uiMapID, pReqPickUpItemMsg->uiX, pReqPickUpItemMsg->uiY,
															pReqPickUpItemMsg->uiItemID, pReqPickUpItemMsg->uiQuantity );
	pMyWorld->clDatabase->writeDBQuery( SO_DB_INFOKIND_PICKUPITEM, LPVOID( szDBArg ), strlen( szDBArg) + 1 );			

	// �������� �ݴµ� �����ߴ�.
	bSuccess	=	TRUE;

END:

	if(bSuccess == TRUE)
	{
		
		// ������ �÷��̾�� ����� �ʵ��� ������ ������ ������.
		
		OnChangeFieldItemMsg Msg;
		
		Msg.dwMsgType = ON_CHANGE_FIELDITEM;
		Msg.siX = pReqPickUpItemMsg->uiX;
		Msg.siY = pReqPickUpItemMsg->uiY;

		if (uiItemQuantity	> 0) {
			Msg.uiItemID = pReqPickUpItemMsg->uiItemID;
			Msg.uiItemQuantity = uiItemQuantity;

		}
		else
		{
			Msg.uiItemID = 0;
			Msg.uiItemQuantity = 0;
		}
		
		UI16 uiCharUniIDs[ON_STATUS_AREA_XSIZE * ON_STATUS_AREA_YSIZE];
		
		
		SI32 siSendCharNum;
		
		siSendCharNum	=	pMyWorld->clISOMap.GetNearSectorPlayerList(uiMapID, Msg.siX, Msg.siY, uiCharUniIDs, ON_MAP_CHARID_ALLPLAYER);
		
		
		
		SendFieldMsg( ON_CHANGE_FIELDITEM, SO_SFM_CUSTOM, LPVOID( siSendCharNum ), LPVOID( uiCharUniIDs ), 
			LPVOID( &Msg), NULL );

		// �������� �ݴµ� �����ߴ�.
		
		SendFieldMsg( ON_RESPONSE_PICKUPITEM, SO_SFM_ONLYME, LPVOID( ON_RET_OK ) );



		
/*		SendFieldMsg( ON_CHANGE_FIELDITEM, SO_SFM_MEANDNEARCHAR, LPVOID( pReqPickUpItemMsg->uiX ), LPVOID( pReqPickUpItemMsg->uiY ), 
						LPVOID( uiItemID ), LPVOID( uiItemQuantity ) );		*/
	}
	else
	{
		// �������� �ݴµ� �����ߴ�.
		SendFieldMsg( ON_RESPONSE_PICKUPITEM, SO_SFM_ONLYME, LPVOID( ON_RET_NO ) );
	}
}





//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	��û �޾����� �������� �����Ѵ�.
//	���� ����	:	2002-07-13 ���� 2:34:46 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
VOID	SOPlayer::RecvWearItem()
{
	OnReqWearItemMsg			*pWearItemMsg;	
	BOOL						bResult = FALSE;

	pWearItemMsg	=	(OnReqWearItemMsg*)	cMsg;

	// �ŷ����϶��� �������� ������ �� ������ �Ѵ�.
	if( IsTrading() == TRUE )						goto	END;

	// �ŷ� ���� ���� �������� ������ �� ������ �Ѵ�.
	bResult = WearItem( pWearItemMsg->uiFollowerID, pWearItemMsg->uiWearLoc, pWearItemMsg->uiItemID, pWearItemMsg->uiPosInventory );	

END:

	if( bResult == TRUE )
	{
		SendFieldMsg( ON_RESPONSE_WEARITEM, SO_SFM_ONLYME, LPVOID( ON_RET_WEARITEM_OK ) );

		if(pWearItemMsg->uiFollowerID == 0)			//	���ΰ�ĳ���� ������ �ٲ�����.
		{
			if((pWearItemMsg->uiWearLoc == ON_WEAR_HELMET_ITEM) ||
			   (pWearItemMsg->uiWearLoc == ON_WEAR_ARMOR_ITEM)  ||
			   (pWearItemMsg->uiWearLoc == ON_WEAR_WEAPON_ITEM) ||
			   (pWearItemMsg->uiWearLoc == ON_WEAR_GUARDIAN_ITEM))
			{
				
					SendFieldMsg( ON_CHANGE_CLOTHES, SO_SFM_ONLYNEARCHAR, LPVOID( this ) );
			}
		}
	}
	else
	{
		SendFieldMsg( ON_RESPONSE_WEARITEM, SO_SFM_ONLYME, LPVOID( ON_RET_WEARITEM_NO ) );
	}

}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	�������� �����Ѵ�.
//	���� ����	:	2002-07-13 ���� 2:32:09 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
BOOL	SOPlayer::WearItem(UI08 uiFollowerID, UI08 uiWearLoc, UI16 uiItemID, SI16 siPosInInventory)
{
	DBIKWearItem				stWearItem;
	MyItemData					ItemData;
	SI08						siPutOnItemErrorCode;
	
	// �������� ������ �� �ִ��� �˾ƿ´�.
	if(IFollower.CanPutOnItem(uiFollowerID, uiWearLoc, uiItemID, siPosInInventory, GetTradeGrade(), siPutOnItemErrorCode) == FALSE)		return	FALSE;

	if( IFollower.PutOnItem(uiFollowerID, uiWearLoc, uiItemID, siPosInInventory, &ItemData) == FALSE )
	{
//		pMyWorld->clServer->WriteInfo( "WearItemError.txt", "DBAccount(%d)  Slot(%d)  uiFollowerID(%d)  uiWearLoc(%d)  uiItemID(%d)  siPosInInventory(%d)  ItemData.siPosInInventory(%d)  ItemData.uiID(%d)  ItemData.uiQuantity(%d)",
		writeInfoToFile( "WearItemError.txt", "DBAccount(%d)  Slot(%d)  uiFollowerID(%d)  uiWearLoc(%d)  uiItemID(%d)  siPosInInventory(%d)  ItemData.siPosInInventory(%d)  ItemData.uiID(%d)  ItemData.uiQuantity(%d)",
										stAccountInfo.siDBAccount,
										stAccountInfo.uiSlot,
										uiFollowerID,
										uiWearLoc,
										uiItemID,
										siPosInInventory,
										ItemData.siPosInInventory,
										ItemData.uiID,
										ItemData.uiQuantity );
		return	FALSE;
	}

	// DB�� �����Ѵ�.
	stWearItem.siDBAccount			=	stAccountInfo.siDBAccount;
	stWearItem.uiSlot				=	stAccountInfo.uiSlot;
	stWearItem.uiFollowerID			=	uiFollowerID;
	stWearItem.uiWearLoc			=	uiWearLoc;
	stWearItem.uiItemID				=	uiItemID;						// !!! ItemData.uiID�� ����ؼ��� �ȵȴ�.
	stWearItem.siItemPosInInventory	=	ItemData.siPosInInventory;
	stWearItem.uiQuantity			=	ItemData.uiQuantity;
	stWearItem.siLimitTime			=	pMyWorld->clItem.GetItemUsingDay(uiItemID);

	
	if(stWearItem.siLimitTime)
	{
		SI16	siNowYear,siNowMon,siNowDay;
		SI16	siEndYear = 0,siEndMon = 0,siEndDay = 0;
		char szTemp[10];

		siNowYear = pMyWorld->RealWorldTime.uiYear;
		siNowMon  = pMyWorld->RealWorldTime.uiMonth;
		siNowDay  = pMyWorld->RealWorldTime.uiDay;

		GetEndDate(stWearItem.siLimitTime,siNowYear,siNowMon,siNowDay,siEndYear,siEndMon,siEndDay);

		sprintf(szTemp,"%04d%02d%02d",siNowYear,siNowMon,siNowDay);
		stWearItem.siStartTime			=	atoi(szTemp);	//���� �ð��� �����Ѵ�.
		sprintf(szTemp,"%04d%02d%02d",siEndYear,siEndMon,siEndDay);
		stWearItem.siEndTime			=	atoi(szTemp);	//���� �ð��� �����Ѵ�.



		m_stTimerItemInfo[uiFollowerID][uiWearLoc].bDeleteTimerItem = FALSE;
		m_stTimerItemInfo[uiFollowerID][uiWearLoc].siItemID			= uiItemID;
		m_stTimerItemInfo[uiFollowerID][uiWearLoc].siLoc			= uiWearLoc;
		m_stTimerItemInfo[uiFollowerID][uiWearLoc].siMercenarySlot	= uiFollowerID;
		m_stTimerItemInfo[uiFollowerID][uiWearLoc].siRestTime		= pMyWorld->clItem.GetItemUsingDay(uiItemID);



	}
	else
	{
		stWearItem.siStartTime			=	0;
		stWearItem.siEndTime			=	0;
	}
	
	pMyWorld->clDatabase->writeDBQuery(SO_DB_INFOKIND_CHANGE_WEAR_INFO, &stWearItem, sizeof(DBIKWearItem));		

	return	TRUE;
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	��û�� �޾����� ������ �������� ��ü�Ѵ�.
//	���� ����	:	2002-07-13 ���� 2:39:24 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
VOID	SOPlayer::RecvStripItem()
{	
	OnReqStripItemMsg			*pWearItemMsg;	
	BOOL						bResult = FALSE;
	
	pWearItemMsg	=	(OnReqStripItemMsg*)	cMsg;

	// �ŷ����϶��� �������� ���� �� ������ �Ѵ�.
	if( IsTrading() == TRUE )						goto	END;

	bResult	=	StripItem(pWearItemMsg->uiFollowerID,pWearItemMsg->uiWearLoc,pWearItemMsg->uiItemID,pWearItemMsg->uiPosInventory);
	
END:

	if( bResult == TRUE )
	{
		SendFieldMsg(ON_RESPONSE_STRIPITEM, SO_SFM_ONLYME, LPVOID(ON_RET_WEARITEM_OK));
		
		if(pWearItemMsg->uiFollowerID == 0)			//	���ΰ�ĳ���� ������ �ٲ�����.
			SendFieldMsg(ON_CHANGE_CLOTHES, SO_SFM_ONLYNEARCHAR, LPVOID(this));
	}
	else
	{

		SendFieldMsg(ON_RESPONSE_STRIPITEM, SO_SFM_ONLYME, LPVOID(ON_RET_WEARITEM_NO));
	}
}


//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	������ �������� ��ü�Ѵ�.
//	���� ����	:	2002-07-13 ���� 12:35:31 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
BOOL	SOPlayer::StripItem(UI08 uiFollowerID, UI08 uiWearLoc, UI16 uiItemID, SI16 siPosInventory)
{
	DBIKWearItem				stWearItem;
	MyItemData					ItemData;
	
	if( siPosInventory < 0 )
		writeInfoToFile("StripPosError.txt", "StripItem 1 :: [%u], [%u], [%d], [%d]", uiFollowerID, uiWearLoc, 
																						uiItemID, siPosInventory );
//		pMyWorld->clServer->WriteInfo("..\\StripPosError.txt", "StripItem 1 :: [%u], [%u], [%d], [%d]", uiFollowerID, uiWearLoc, 

	// �������� ���� �� �ִ��� �˾ƿ´�.
	if(IFollower.CanTakeOffItem(uiFollowerID, uiWearLoc, uiItemID, siPosInventory) == FALSE	)				return	FALSE;
	
	// �������� ���´�.
	if( IFollower.TakeOffItem(uiFollowerID, uiWearLoc, uiItemID, siPosInventory, &ItemData) == FALSE )
	{
//		pMyWorld->clServer->WriteInfo( "StripItemError.txt", "DBAccount(%d)  Slot(%d)  uiFollowerID(%d)  uiWearLoc(%d)  uiItemID(%d)  siPosInventory(%d)  ItemData.siPosInInventory(%d)  ItemData.uiID(%d)  ItemData.uiQuantity(%d)",
		writeInfoToFile( "StripItemError.txt", "DBAccount(%d)  Slot(%d)  uiFollowerID(%d)  uiWearLoc(%d)  uiItemID(%d)  siPosInventory(%d)  ItemData.siPosInInventory(%d)  ItemData.uiID(%d)  ItemData.uiQuantity(%d)",
										stAccountInfo.siDBAccount,
										stAccountInfo.uiSlot,
										uiFollowerID,
										uiWearLoc,
										uiItemID,
										siPosInventory,
										ItemData.siPosInInventory,
										ItemData.uiID,
										ItemData.uiQuantity );
		return FALSE;
	}
	
	// DB�� �����Ѵ�.
	stWearItem.siDBAccount			=	stAccountInfo.siDBAccount;
	stWearItem.uiSlot				=	stAccountInfo.uiSlot;
	stWearItem.uiFollowerID			=	uiFollowerID;
	stWearItem.uiWearLoc			=	uiWearLoc;
	stWearItem.uiItemID				=	0;								// !!! �ݵ�� 0�� �־��־���Ѵ�.
	stWearItem.siItemPosInInventory	=	ItemData.siPosInInventory;
	stWearItem.uiQuantity			=	ItemData.uiQuantity;
	stWearItem.siLimitTime			=	pMyWorld->clItem.GetItemUsingDay(uiItemID);

	stWearItem.siStartTime			=	0;
	stWearItem.siEndTime			=	0;

//		pMyWorld->clServer->WriteInfo("..\\StripPosError.txt", 

	if( stWearItem.siItemPosInInventory < 0 )
		writeInfoToFile("StripPosError.txt", 
										"StripItem 2 :: [%d], [%u], [%u], [%u], [%u], [%d], [%u]", 
										stWearItem.siDBAccount,
										stWearItem.uiSlot,
										stWearItem.uiFollowerID,
										stWearItem.uiWearLoc,
										stWearItem.uiItemID,
										stWearItem.siItemPosInInventory,
										stWearItem.uiQuantity );						


	pMyWorld->clDatabase->writeDBQuery(SO_DB_INFOKIND_CHANGE_WEAR_INFO, &stWearItem, sizeof(DBIKWearItem));	
	
	return	TRUE;
}

BOOL	SOPlayer::DisAppearItem( UI08 uiFollowerID, UI08 uiWearLoc, UI16 uiItemID, SI16 siPosInventory)
{
	DBIKWearItem				stWearItem;
	MyItemData					ItemData;
	
	// �������� ���´�.
	IFollower.DisAppearItem(uiFollowerID, uiWearLoc, uiItemID, siPosInventory, &ItemData);	

	// DB�� �����Ѵ�.
	stWearItem.siDBAccount			=	stAccountInfo.siDBAccount;
	stWearItem.uiSlot				=	stAccountInfo.uiSlot;
	stWearItem.uiFollowerID			=	uiFollowerID;
	stWearItem.uiWearLoc			=	uiWearLoc;
	stWearItem.uiItemID				=	0;								// !!! �ݵ�� 0�� �־��־���Ѵ�.
	stWearItem.siItemPosInInventory	=	0;
	stWearItem.uiQuantity			=	0;
	stWearItem.siLimitTime			=	pMyWorld->clItem.GetItemUsingDay(uiItemID);

	stWearItem.siStartTime			=	0;
	stWearItem.siEndTime			=	0;

	pMyWorld->clDatabase->writeDBQuery(SO_DB_INFOKIND_CHANGE_WEAR_INFO, &stWearItem, sizeof(DBIKWearItem));	
	
	return	TRUE;

}


VOID	SOPlayer::RecvVillageNation()
{
	OnReqVillageGetNation		*pVillageGetNation;
	SOVillage					*pVillage	=	NULL;

	pVillageGetNation		=	(OnReqVillageGetNation*)	cMsg;
	pVillage				=	pMyWorld->GetVillageByCode(pVillageGetNation->uiVillageCode);

	if(pVillage != NULL)
		SendFieldMsg(ON_RESPONSE_VILLAGE_NATION, SO_SFM_ONLYME, LPVOID(ON_RET_OK), LPVOID(pVillage));
	else
	{
		SendFieldMsg(ON_RESPONSE_VILLAGE_NATION, SO_SFM_ONLYME, LPVOID(ON_RET_NO));		
	}
}

VOID	SOPlayer::RecvReadDescript()
{
	OnReqVillageDescript		*pVillageDescript;
	SOVillage					*pVillage= NULL;

	pVillageDescript		=	(OnReqVillageDescript*)	cMsg;
	pVillage				=	pMyWorld->GetVillageByCode(pVillageDescript->uiVillageCode);

	if(pVillage)
		SendFieldMsg(ON_RESPONSE_READ_DESCRIPT, SO_SFM_ONLYME, LPVOID(pVillageDescript->uiVillageCode), LPVOID(pVillage->szDescription), LPVOID(pVillage->GetGuildSID()));
}

VOID	SOPlayer::RecvImoticon()
{
	OnReqImoticonMsg			*pImoticonMsg;

	pImoticonMsg			=	(OnReqImoticonMsg*) cMsg;
	SendFieldMsg(ON_RESPONSE_IMOTICON, SO_SFM_ONLYNEARCHAR, LPVOID(this->uiAccount),LPVOID(pImoticonMsg->uiImoticonID));
}

VOID	SOPlayer::RecvSpendItem()
{
	OnReqSpendItemMsg			*pSpendItemMsg;
	
	pSpendItemMsg		=	(OnReqSpendItemMsg*) cMsg;
	if (SpendItem(pSpendItemMsg->uiFollowerID, pSpendItemMsg->uiItemID,pSpendItemMsg->uiPosInventory,pSpendItemMsg->uiQuantity))
	{
		SendFieldMsg(ON_RESPONSE_SPENDITEM, SO_SFM_ONLYME, LPVOID(ON_RET_OK),NULL);
	}
	else
	{
		SendFieldMsg(ON_RESPONSE_SPENDITEM, SO_SFM_ONLYME, LPVOID(ON_RET_NO),NULL);

	}
};

void	SOPlayer::recvProbeArea()
{
	BURIED::ITEMINFO*	pItem;
	BURIED::ITEMINFO*	foundItems[20];

	memset(foundItems, 0, sizeof(foundItems));

	int				  numFoundItem = 0;


	// 0. ���� ��ġ���� �߰��Ҽ� �ִ� ������ ����� ��� ���� �´� 
	//----------------------------------------------------------------------------------
	while( pItem = clServer->clWorld->m_buriedItemParser.findItem (uiMapID) )
	{	
		
		foundItems[numFoundItem++] = pItem;		
	
		if( numFoundItem >= 20 ) break;
	}

	
	
	// 1. �߰��� �������� ���� ��� ����߿��� �ϳ��� ���ϰ� �κ��丮�� �־��ش�.
	//    ������ �߰��� �����ߴٸ� �޽����� �����ش� 
	//----------------------------------------------------------------------------------		
	if( numFoundItem > 0 )
	{
		int s = rand() % numFoundItem; // ����� ������ ������ 


		
		if( rand() % 1000 > foundItems[ s ]->findProbability )	return; 
		if( IsOverWeight() )									return;


		for(int i=0; i < ON_MAX_FOLLOWER_NUM; ++i)		
		{			
			if( AddItem( i, foundItems[ s ]->itemCode, 1 ) )
			{
				BURIED::sResponseProbeArea packet;
				
				packet.command   = BURIED::RESPONSE_PROBE_AREA;
				packet.foundItem = foundItems[ s ]->itemCode;

				Write(&packet, sizeof(packet));
				return;
			}
		}
	}
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	NPC�� ����������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
VOID	SOPlayer::RecvReqQuestSelectNPC()
{
	OnReqQuestSelectNPC*			lpOnReqQuestSelectNPC;
	SI16							TempSlot;
	SI16							TempQuest;

	lpOnReqQuestSelectNPC = (OnReqQuestSelectNPC*)cMsg;

	stQuestInfo.m_QuestData_BookCount = lpOnReqQuestSelectNPC->siBookCount;

	// �� NPC�� ���õ� Quest�� �ϰ� ������
	TempSlot = Quest_FindPlayQuestNPC(lpOnReqQuestSelectNPC->siNPCCode);
	Quest_AddQuestObject_Npc(lpOnReqQuestSelectNPC->siNPCCode);

	if(TempSlot != -1)
	{
		// ������, ����Ʈ�� �Ϸ��Ҽ� �ִ��� �˻�
		if(Quest_CheckCompleteQuest(TempSlot))
		{
			if(stQuestInfo.m_sPlayingQuestHeader[TempSlot]->m_siQuestEvent == QUEST_EVENT_PLAY)
			{
				//�ӽ� �ڵ�.-_-; �ϴ� ���ƺ���.
				if(pMyWorld->m_bQuestEventUpdate == TRUE) 
				{
					SendFieldMsg(ON_RESPONSE_QUEST_SELECTNPC, SO_SFM_ONLYME, LPVOID(ON_RET_QUEST_SELECTNPC_PLAYINGQUEST), LPVOID(stQuestInfo.m_sPlayingQuest[TempSlot].m_QuestCode));
				}
				else
					Quest_CheckEventItem(TempSlot);
			}
			else
				Quest_EndQuest(TempSlot);
			// �Ϸ��϶�, End
			
		}
		else
		{
			// ���� �� �Ϸ��϶�, Playing
			SendFieldMsg(ON_RESPONSE_QUEST_SELECTNPC, SO_SFM_ONLYME, LPVOID(ON_RET_QUEST_SELECTNPC_PLAYINGQUEST), LPVOID(stQuestInfo.m_sPlayingQuest[TempSlot].m_QuestCode));
		}
	}
	else
	{
		// ������, �� NPC�� ���õ� ���� ���� ���� Quest�� �˻�
		TempQuest = Quest_FindNextQuest(lpOnReqQuestSelectNPC->siNPCCode);
		if(TempQuest > 0)
		{
			// ���� ���� Quest�� �������, �� Quest�� �� ����(Need, Skip Test)�� �Ŵ��� �˻�
			if(Quest_CheckPlayQuestAbility(TempQuest) == TRUE)
			{
				if(Quest_CheckSkipQuestAbility(TempQuest) == FALSE)
				{
					// �� ������ �Ŵ� ���, Start
					SendFieldMsg(ON_RESPONSE_QUEST_SELECTNPC, SO_SFM_ONLYME, LPVOID(ON_RET_QUEST_SELECTNPC_STARTQUEST), LPVOID(TempQuest));
				}
				else
				{
					// Skip�� �Ŵ� ���
					SendFieldMsg(ON_RESPONSE_QUEST_SELECTNPC, SO_SFM_ONLYME, LPVOID(ON_RET_QUEST_SELECTNPC_ENDAFTERQUEST), LPVOID(TempQuest));
				}
			}
			else
			{
				// �� ������ ���Ŵ� ���, StartBefore
				SendFieldMsg(ON_RESPONSE_QUEST_SELECTNPC, SO_SFM_ONLYME, LPVOID(ON_RET_QUEST_SELECTNPC_STARTBEFOREQUEST), LPVOID(TempQuest));
			}
		}
		else
		{
			// ���� ���� Quest�� �������, ���������� �� NPC Quest�� ã�´�.
			TempQuest = Quest_FindLastPlayQuest(lpOnReqQuestSelectNPC->siNPCCode);
			if(TempQuest > 0)
			{
				// ã�����, EndAfter
				SendFieldMsg(ON_RESPONSE_QUEST_SELECTNPC, SO_SFM_ONLYME, LPVOID(ON_RET_QUEST_SELECTNPC_ENDAFTERQUEST), LPVOID(TempQuest));
			}
			else
			{
				// ã�� ���Ұ��, Error
				SendFieldMsg(ON_RESPONSE_QUEST_SELECTNPC, SO_SFM_ONLYME, LPVOID(ON_RET_QUEST_SELECTNPC_ERROR));
			}
		}
	}
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	Quest�� ����������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
VOID	SOPlayer::RecvReqQuestAcceptQuest()
{
	OnReqQuestAcceptQuest*			lpOnReqQuestAcceptQuest;
	SI16							TempSlot;
	DBIKQuestSetPlayQuestData		TempDBIKQuestSetPlayQuestData;

	lpOnReqQuestAcceptQuest	=	(OnReqQuestAcceptQuest*)cMsg;
	
	// ������ �� ����Ʈ���� �˻�
	if(Quest_FindCompletedQuest(lpOnReqQuestAcceptQuest->m_QuestCode) == FALSE)
	{
		// ������ ������ ������, ���� Quest�� �� �ɷ��� �Ŵ��� �˻�
		if(Quest_CheckPlayQuestAbility(lpOnReqQuestAcceptQuest->m_QuestCode))
		{
			// �ɷ��� �Ÿ�, Skip ������ �Ŵ��� �˻�
			if(Quest_CheckSkipQuestAbility(lpOnReqQuestAcceptQuest->m_QuestCode) == FALSE)
			{
				// Skip�� �ȵɶ�, �� Quest Slot�� ã�´�.
				TempSlot = Quest_FindEmptyQuest();
				if(TempSlot != -1)
				{
					ZeroMemory(&stQuestInfo.m_sPlayingQuest[TempSlot], sizeof(SPlayingQuest));
					stQuestInfo.m_sPlayingQuest[TempSlot].m_QuestCode      = lpOnReqQuestAcceptQuest->m_QuestCode;
					stQuestInfo.m_sPlayingQuestHeader[TempSlot]            = pMyWorld->clQuestParser.GetQuest(stQuestInfo.m_sPlayingQuest[TempSlot].m_QuestCode);

					TempDBIKQuestSetPlayQuestData.DBAccount   = stAccountInfo.siDBAccount;
					TempDBIKQuestSetPlayQuestData.uiSlot      = stAccountInfo.uiSlot;
					TempDBIKQuestSetPlayQuestData.QuestIndex  = TempSlot;
					CopyMemory(&TempDBIKQuestSetPlayQuestData.QuestData, &stQuestInfo.m_sPlayingQuest[TempSlot], sizeof(SPlayingQuest));
					pMyWorld->clDatabase->writeDBQuery(SO_DB_INFOKIND_QUEST_SETPLAYQUESTDATA, LPVOID(&TempDBIKQuestSetPlayQuestData), sizeof(TempDBIKQuestSetPlayQuestData));
					SendFieldMsg(ON_RESPONSE_QUEST_ACCEPTQUEST, SO_SFM_ONLYME, LPVOID(ON_RET_OK), LPVOID(lpOnReqQuestAcceptQuest->m_QuestCode));
				}
				else
				{
					SendFieldMsg(ON_RESPONSE_QUEST_ACCEPTQUEST, SO_SFM_ONLYME, LPVOID(ON_RET_NO), LPVOID(lpOnReqQuestAcceptQuest->m_QuestCode));
				}
			}
			else
			{
				// Skip ���ɽ�
				SendFieldMsg(ON_RESPONSE_QUEST_ACCEPTQUEST, SO_SFM_ONLYME, LPVOID(ON_RET_NO), LPVOID(lpOnReqQuestAcceptQuest->m_QuestCode));
			}
		}
		else
		{
			// �ɷ��� �ȵŸ�
			SendFieldMsg(ON_RESPONSE_QUEST_ACCEPTQUEST, SO_SFM_ONLYME, LPVOID(ON_RET_NO), LPVOID(lpOnReqQuestAcceptQuest->m_QuestCode));
		}
	}
	else
	{
		// ������ �� ����Ʈ �϶�
		SendFieldMsg(ON_RESPONSE_QUEST_ACCEPTQUEST, SO_SFM_ONLYME, LPVOID(ON_RET_NO), LPVOID(lpOnReqQuestAcceptQuest->m_QuestCode));
	}
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	�������� Quest�� ���������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
VOID	SOPlayer::RecvReqQuestCancelQuest()
{
	OnReqQuestCancelQuest*			lpOnReqQuestCancelQuest;
	SI16							TempSlot;
	DBIKQuestCancelQuest			TempDBIKQuestCancelQuest;

	lpOnReqQuestCancelQuest	=	(OnReqQuestCancelQuest*)cMsg;

	// �� ����Ʈ�� ���� ���������� �˻�
	TempSlot = Quest_FindQuest(lpOnReqQuestCancelQuest->m_QuestCode);
	if(TempSlot != -1)
	{
		// �������϶� Quest ���
		TempDBIKQuestCancelQuest.DBAccount  = stAccountInfo.siDBAccount;
		TempDBIKQuestCancelQuest.uiSlot     = stAccountInfo.uiSlot;
		TempDBIKQuestCancelQuest.QuestIndex = TempSlot;
		pMyWorld->clDatabase->writeDBQuery(SO_DB_INFOKIND_QUEST_CANCELQUEST, LPVOID(&TempDBIKQuestCancelQuest), sizeof(TempDBIKQuestCancelQuest));
		ZeroMemory(&stQuestInfo.m_sPlayingQuest[TempSlot], sizeof(SPlayingQuest));
		stQuestInfo.m_sPlayingQuestHeader[TempSlot] = NULL;

		SendFieldMsg(ON_RESPONSE_QUEST_CANCELQUEST, SO_SFM_ONLYME, LPVOID(ON_RET_OK), LPVOID(lpOnReqQuestCancelQuest->m_QuestCode));
	}
	else
	{
		// �������� �ƴҶ�
		SendFieldMsg(ON_RESPONSE_QUEST_CANCELQUEST, SO_SFM_ONLYME, LPVOID(ON_RET_NO), LPVOID(lpOnReqQuestCancelQuest->m_QuestCode));
	}
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	�������� Quest List ��û
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
VOID	SOPlayer::RecvReqQuestGetPlayQuest()
{
	// Quest Data ����
	SendFieldMsg(ON_RESPONSE_QUEST_GETPLAYQUEST, SO_SFM_ONLYME, LPVOID(NULL), LPVOID(ON_QUSETDATA_MAXPLAYINGQUEST), LPVOID(ON_QUSETDATA_MAXCOMLETEDQUEST), LPVOID(stQuestInfo.m_sPlayingQuest), LPVOID(stQuestInfo.m_siCompletedQuest));
}


//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	�� NPC�� ���õȾ� Play�ϰ� �ִ� Quest�� ã�´�.
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
SI16	SOPlayer::Quest_FindPlayQuestNPC(SI16 NPCCode)
{
	SI32	TempCount;

	for(TempCount = 0; TempCount < ON_QUSETDATA_MAXPLAYINGQUEST; TempCount++)
	{
		if(stQuestInfo.m_sPlayingQuest[TempCount].m_QuestCode > 0)
		{
			if(stQuestInfo.m_sPlayingQuestHeader[TempCount]->siNPCCode == NPCCode)
			{
				return TempCount;
			}
		}
	}

	return -1;
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	�� Quest�� �Ϸ�ɼ� �ִ��� �˻�
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
BOOL	SOPlayer::Quest_CheckCompleteQuest(SI16 QuestSlot)
{
	SI32		TempCount;
	SI32		TempCount1;
	SI32		TempCount2;
	SI16		TempSI16;

	if(stQuestInfo.m_sPlayingQuest[QuestSlot].m_QuestCode <= 0)
	{
		return FALSE;
	}

	for(TempCount = 0; TempCount < ON_QUSETDATA_MAXQUESTENDREQOBJECT; TempCount++)
	{
		switch(stQuestInfo.m_sPlayingQuestHeader[QuestSlot]->sQuestEndReqObject[TempCount].m_QuestObjectType)
		{
			case QUESTOBJECTTYPE_ITEM:
				if(IFollower.CheckItem(stQuestInfo.m_sPlayingQuestHeader[QuestSlot]->sQuestEndReqObject[TempCount].m_QuestObjectCode, stQuestInfo.m_sPlayingQuestHeader[QuestSlot]->sQuestEndReqObject[TempCount].m_QuestObjectQuantity) == FALSE)
				{
					return FALSE;
				}
				break;

			case QUESTOBJECTTYPE_WORK:
				if(stQuestInfo.m_sPlayingQuest[QuestSlot].m_QuestObject[TempCount] < stQuestInfo.m_sPlayingQuestHeader[QuestSlot]->sQuestEndReqObject[TempCount].m_QuestObjectQuantity)
				{
					return FALSE;
				}
				break;

			case QUESTOBJECTTYPE_BANKBOOK:
				if(m_MoneySaving < stQuestInfo.m_sPlayingQuestHeader[QuestSlot]->sQuestEndReqObject[TempCount].m_QuestObjectQuantity)
				{
					return FALSE;
				}
				break;

			case QUESTOBJECTTYPE_BOOK:
				if(stQuestInfo.m_QuestData_BookCount < stQuestInfo.m_sPlayingQuestHeader[QuestSlot]->sQuestEndReqObject[TempCount].m_QuestObjectQuantity)
				{
					return FALSE;
				}
				break;

			case QUESTOBJECTTYPE_MERCENARY:
				TempCount2 = 0;
				for(TempCount1 = 1; TempCount1 < ON_MAX_FOLLOWER_NUM; TempCount1++)
				{
					if(IFollower.IsValidFollower(TempCount1))
					{
						TempCount2++;
					}
				}
				if(TempCount2 < stQuestInfo.m_sPlayingQuestHeader[QuestSlot]->sQuestEndReqObject[TempCount].m_QuestObjectQuantity)
				{
					return FALSE;
				}
				break;

			case QUESTOBJECTTYPE_MONSTER:
				if(stQuestInfo.m_sPlayingQuest[QuestSlot].m_QuestObject[TempCount] < stQuestInfo.m_sPlayingQuestHeader[QuestSlot]->sQuestEndReqObject[TempCount].m_QuestObjectQuantity)
				{
					return FALSE;
				}
				break;

			case QUESTOBJECTTYPE_SATIETY:
				if(m_satiety < stQuestInfo.m_sPlayingQuestHeader[QuestSlot]->sQuestEndReqObject[TempCount].m_QuestObjectQuantity)
				{
					return FALSE;
				}
				break;
			case QUESTOBJECTTYPE_HEALTH:
				if( IFollower.GetFollowerParameterLife(0) < IFollower.GetFollowerParameterMaxLife(0))
				{
					return FALSE;
				}
				break;
			case QUESTOBJECTTYPE_PORTAL:
				if( stQuestInfo.m_sPlayingQuest[QuestSlot].m_QuestObject[TempCount] < stQuestInfo.m_sPlayingQuestHeader[QuestSlot]->sQuestEndReqObject[TempCount].m_QuestObjectQuantity)
				{
					return FALSE;
				}
				break;
			case QUESTOBJECTTYPE_LEVEL:
				if(IFollower.GetFollowerParameterLevel(0) < stQuestInfo.m_sPlayingQuestHeader[QuestSlot]->sQuestEndReqObject[TempCount].m_QuestObjectQuantity)
				{
					return FALSE;
				}
				break;
			case QUESTOBJECTTYPE_NPC:
				if(stQuestInfo.m_sPlayingQuest[QuestSlot].m_QuestObject[TempCount] < stQuestInfo.m_sPlayingQuestHeader[QuestSlot]->sQuestEndReqObject[TempCount].m_QuestObjectQuantity)
				{
					return FALSE;
				}
				break;

			case QUESTOBJECTTYPE_QUEST:
				TempSI16 = stQuestInfo.m_sPlayingQuestHeader[QuestSlot]->sQuestEndReqObject[TempCount].m_QuestObjectQuantity;
				for(TempCount1 = 0; TempCount1 < ON_QUSETDATA_MAXCOMLETEDQUEST; TempCount1++)
				{
					if(stQuestInfo.m_siCompletedQuest[TempCount1] == TempSI16)
					{
						goto GOTO_COMPLETIONQUEST;
					}
				}
				return FALSE;
GOTO_COMPLETIONQUEST:
				break;

			case QUESTOBJECTTYPE_MONEY:
				if(siMoney < stQuestInfo.m_sPlayingQuestHeader[QuestSlot]->sQuestEndReqObject[TempCount].m_QuestObjectQuantity)
				{
					return FALSE;
				}
				break;
		}
	}

	return TRUE;
}

/*VOID	SOPlayer::Quest_CheckEventItem(SI16 siQuestSlot)
{
	//DBIKPlayerDBInfo		PlayerInfo;

	//���� �ϰ� ������ ������� ������.

	DBIKQuestEventItem		stQuestEventItem;
	SI16					siEventItemID;



	stQuestEventItem.DBAccount		=	stAccountInfo.siDBAccount;
	stQuestEventItem.uiSlot			=	stAccountInfo.uiSlot;

	stQuestEventItem.siQuestCode	=	stQuestInfo.m_sPlayingQuestHeader[siQuestSlot]->siQuestCode;
	stQuestEventItem.siEventItemID	=	siEventItemID;

	{
		pMyWorld->clDatabase->GetDBInfo(SO_DB_INFO_GETEVENTITEM,);
	}

}*/
VOID	SOPlayer::Quest_CheckEventItem(SI16 siQuestSlot)
{
	SI16 siEventItemID;		 //DB�� ���̺� �ִ� ���̵�.
	stQuestEventItem* pstQuestEventItem;
	
	SI16 siQuestCode = stQuestInfo.m_sPlayingQuestHeader[siQuestSlot]->siQuestCode;

	SI16 siTempEventItemID = rand()%pMyWorld->m_QuestEventItemMgr->GetEventQuest(siQuestCode)->m_siEventQuestItemIDCount;//rand()%pMyWorld->m_siEventItemCount;

	//pstQuestEventItem = pMyWorld->m_QuestEventItemParser.GetItemName(siTempEventItemID);
	pstQuestEventItem = pMyWorld->m_QuestEventItemMgr->GetEventQuest(siQuestCode)->GetItemName(siTempEventItemID);

	if(pstQuestEventItem->siDropPrecent >= (rand()%30000 + 1))
	{
		siEventItemID = pstQuestEventItem->siItemNum;
	}
	else
	{
		siEventItemID = pstQuestEventItem->siNextItemNum;
	}

	//if(pMyWorld->m_QuestEventItemParser.IsLogUseItem(siEventItemID) == TRUE)
	/*
		if(pMyWorld->m_QuestEventItemInfo[siEventItemID].siPreItemCount > 0)
			pMyWorld->m_QuestEventItemInfo[siEventItemID].siPreItemCount--;
		else
			//siEventItemID = pMyWorld->m_QuestEventItemParser.GetNextItemID(siEventItemID);
			siEventItemID = pMyWorld->m_QuestEventItemMgr->GetEventQuest(siQuestCode)->GetNextItemID(siEventItemID);
	*/

	if(pMyWorld->m_QuestEventItemMgr->GetEventQuest(siQuestCode)->IsLogUseItem(siEventItemID) == TRUE)
	{
		for(int i = 0 ; i < pMyWorld->clQuestParser.m_siEventQuestCount ; i++)
		{
			if(pMyWorld->m_pstQuestEventItemInfoMgr[i].siQuestID == siQuestCode)
			{
				if(pMyWorld->m_pstQuestEventItemInfoMgr[i].stQuestEventItemInfo[siEventItemID].siPreItemCount > 0)
					pMyWorld->m_pstQuestEventItemInfoMgr[i].stQuestEventItemInfo[siEventItemID].siPreItemCount--;
				else
					siEventItemID = pMyWorld->m_QuestEventItemMgr->GetEventQuest(siQuestCode)->GetItemName(siEventItemID)->siNextItemNum;
			}
		}
	}
	Quest_SetEventItem(siEventItemID,siQuestSlot);

}

VOID	SOPlayer::Quest_SetEventItem(SI16 siEventItemID,SI16 QuestSlot)
{
	char szTemp[256];
	//SI16 siGameItemID;
	stQuestEventItem* pstQuestEventItem;

	SI16 siQuestCode = stQuestInfo.m_sPlayingQuestHeader[QuestSlot]->siQuestCode;

	ZeroMemory(szTemp,sizeof(szTemp));
	
	//pstQuestEventItem = pMyWorld->m_QuestEventItemParser.GetItemName(siEventItemID);
	pstQuestEventItem = pMyWorld->m_QuestEventItemMgr->GetEventQuest(siQuestCode)->GetItemName(siEventItemID);


	//if(pMyWorld->m_QuestEventItemParser.IsLogUseItem(siEventItemID) == TRUE)
	if(pMyWorld->m_QuestEventItemMgr->GetEventQuest(siQuestCode)->IsLogUseItem(siEventItemID) == TRUE)
	{
		if(pstQuestEventItem->bDeliveryItem == FALSE)
		{
			stQuestInfo.m_sPlayingQuestHeader[QuestSlot]->sQuestPay[0].m_QuestPayType = QUESTPAYTYPE_EVENT_GAMEITEM;
			stQuestInfo.m_sPlayingQuestHeader[QuestSlot]->sQuestPay[0].m_QuestPayEventItemID = (SI32)siEventItemID;
			stQuestInfo.m_sPlayingQuestHeader[QuestSlot]->sQuestPay[0].m_QuestPayQuantity = 1;
			
			stQuestInfo.m_sPlayingQuestHeader[QuestSlot]->sQuestPay[0].m_QuestPayCode = atoi(pstQuestEventItem->szItemName);
		}
		else
		{
			stQuestInfo.m_sPlayingQuestHeader[QuestSlot]->sQuestPay[0].m_QuestPayType = QUESTPAYTYPE_EVENT_ITEM;
			stQuestInfo.m_sPlayingQuestHeader[QuestSlot]->sQuestPay[0].m_QuestPayEventItemID = siEventItemID;
		}
	}
	else
	{
		stQuestInfo.m_sPlayingQuestHeader[QuestSlot]->sQuestPay[0].m_QuestPayCode = atoi(pstQuestEventItem->szItemName);
		stQuestInfo.m_sPlayingQuestHeader[QuestSlot]->sQuestPay[0].m_QuestPayType = QUESTPAYTYPE_ITEM;
		stQuestInfo.m_sPlayingQuestHeader[QuestSlot]->sQuestPay[0].m_QuestPayQuantity = 1;
	}
	Quest_EndQuest(QuestSlot);
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	Quest�� �����Ѵ�.(DB�� �����ϰ�, ������ ������, Client�� �޼��� ���۱��� �Ѵ�.)
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
VOID	SOPlayer::Quest_EndQuest(SI16 QuestSlot)
{
	DBIKQuestCompleteQuest		TempDBIKQuestCompleteQuest;
	SI32						TempCount;
	SI32						TempCount1;
	SI32						TempItemSlot;
	SQuestPayData				TempPayData[5];
	OnlineFollower*				lpOnlineFollower;
	MyItemData*					lpMyItemData;
	SI16						TempDelItemCount;
	MercenaryItemData			TempMercenaryItemData[36];
	CItemHeader*				lpCItemHeader;
	MONEY						TempDeleteMoney;
	DBIKQuestPayMoney			TempDBIKQuestPayMoney;
	DBIKQuestPayItem			TempDBIKQuestPayItem;
	DBIKQuestPayCredit			TempDBIKQuestPayCredit;
	DBIKQuestPayEXP				TempDBIKQuestPayEXP;
	DBIKQuestPayEventItem		TempDBIKQuestEventItem;
	DBIKQuestRequest_Item		TempDBIKQuestRequest_Item;
	DBIKQuestRequest_Money		TempDBIKQuestRequest_Money;
	stQuestEventItem*			pstQuestEventItem;

	SI16 siQuestCode = stQuestInfo.m_sPlayingQuestHeader[QuestSlot]->siQuestCode;

	// DB�� �����Ѵ�.	

	//if(stQuestInfo.m_sPlayingQuestHeader[QuestSlot]->siQuestCode != DOKDO_QUEST_CODE1 && 
	//  stQuestInfo.m_sPlayingQuestHeader[QuestSlot]->siQuestCode != DOKDO_QUEST_CODE2)
	if(stQuestInfo.m_sPlayingQuestHeader[QuestSlot]->m_siQuestEvent != QUEST_EVENT_PLAY)
	{	
		TempDBIKQuestCompleteQuest.DBAccount  = stAccountInfo.siDBAccount;
		TempDBIKQuestCompleteQuest.uiSlot     = stAccountInfo.uiSlot;
		TempDBIKQuestCompleteQuest.QuestIndex = QuestSlot;
		pMyWorld->clDatabase->writeDBQuery(SO_DB_INFOKIND_QUEST_COMPLETEQUEST, LPVOID(&TempDBIKQuestCompleteQuest), sizeof(TempDBIKQuestCompleteQuest));
	}

	// ������ �޴´�.
	ZeroMemory(TempPayData, sizeof(SQuestPayData) * ON_QUSETDATA_MAXQUESTPAY);
	for(TempCount = 0; TempCount < ON_QUSETDATA_MAXQUESTPAY; TempCount++)
	{
//		m_sPlayingQuestHeader
		TempPayData[TempCount].m_QuestPayType = stQuestInfo.m_sPlayingQuestHeader[QuestSlot]->sQuestPay[TempCount].m_QuestPayType;
		switch(stQuestInfo.m_sPlayingQuestHeader[QuestSlot]->sQuestPay[TempCount].m_QuestPayType)
		{
			case QUESTPAYTYPE_MONEY:
				IncreaseMoney(stQuestInfo.m_sPlayingQuestHeader[QuestSlot]->sQuestPay[TempCount].m_QuestPayQuantity, FALSE);
				TempPayData[TempCount].m_QuestPayData_Money.m_AddMoney = stQuestInfo.m_sPlayingQuestHeader[QuestSlot]->sQuestPay[TempCount].m_QuestPayQuantity;
				
				TempDBIKQuestPayMoney.siDBAccount = stAccountInfo.siDBAccount;
				TempDBIKQuestPayMoney.uiDBSlot    = stAccountInfo.uiSlot;
				TempDBIKQuestPayMoney.uiQuestCode = stQuestInfo.m_sPlayingQuestHeader[QuestSlot]->siQuestCode;
				TempDBIKQuestPayMoney.siMoney     = stQuestInfo.m_sPlayingQuestHeader[QuestSlot]->sQuestPay[TempCount].m_QuestPayQuantity;
				pMyWorld->clDatabase->writeDBQuery(SO_DB_INFOKIND_QUEST_PAY_MONEY, LPVOID(&TempDBIKQuestPayMoney), sizeof(TempDBIKQuestPayMoney));				
				break;

			case QUESTPAYTYPE_CREDIT:
				IncreaseTradeCredit(stQuestInfo.m_sPlayingQuestHeader[QuestSlot]->sQuestPay[TempCount].m_QuestPayQuantity, FALSE);
				TempPayData[TempCount].m_QuestPayData_Credit.m_AddCredit = stQuestInfo.m_sPlayingQuestHeader[QuestSlot]->sQuestPay[TempCount].m_QuestPayQuantity;

				TempDBIKQuestPayCredit.siDBAccount = stAccountInfo.siDBAccount;
				TempDBIKQuestPayCredit.uiDBSlot    = stAccountInfo.uiSlot;
				TempDBIKQuestPayCredit.uiQuestCode = stQuestInfo.m_sPlayingQuestHeader[QuestSlot]->siQuestCode;
				TempDBIKQuestPayCredit.siCredit    = stQuestInfo.m_sPlayingQuestHeader[QuestSlot]->sQuestPay[TempCount].m_QuestPayQuantity;
				pMyWorld->clDatabase->writeDBQuery(SO_DB_INFOKIND_QUEST_PAY_CREDIT, LPVOID(&TempDBIKQuestPayCredit), sizeof(TempDBIKQuestPayCredit));				
				break;

			case QUESTPAYTYPE_EXP:
				lpOnlineFollower = IFollower.GetFollower(0);
				lpOnlineFollower->ParametaBox.IP.IncreaseExp(stQuestInfo.m_sPlayingQuestHeader[QuestSlot]->sQuestPay[TempCount].m_QuestPayQuantity);
				TempPayData[TempCount].m_QuestPayData_EXP.m_AddHeroEXP = stQuestInfo.m_sPlayingQuestHeader[QuestSlot]->sQuestPay[TempCount].m_QuestPayQuantity;

				TempDBIKQuestPayEXP.siDBAccount = stAccountInfo.siDBAccount;
				TempDBIKQuestPayEXP.uiDBSlot    = stAccountInfo.uiSlot;
				TempDBIKQuestPayEXP.uiQuestCode = stQuestInfo.m_sPlayingQuestHeader[QuestSlot]->siQuestCode;
				TempDBIKQuestPayEXP.siEXP       = stQuestInfo.m_sPlayingQuestHeader[QuestSlot]->sQuestPay[TempCount].m_QuestPayQuantity;
				pMyWorld->clDatabase->writeDBQuery(SO_DB_INFOKIND_QUEST_PAY_EXP, LPVOID(&TempDBIKQuestPayEXP), sizeof(TempDBIKQuestPayEXP));				
				break;

			case QUESTPAYTYPE_EVENT_ITEM:	
				{
					SI16 siEventItemID = stQuestInfo.m_sPlayingQuestHeader[QuestSlot]->sQuestPay[TempCount].m_QuestPayEventItemID;
					//pstQuestEventItem = pMyWorld->m_QuestEventItemParser.GetItemName(siEventItemID);
					pstQuestEventItem = pMyWorld->m_QuestEventItemMgr->GetEventQuest(siQuestCode)->GetItemName(siEventItemID);
					
					strcpy(TempPayData[TempCount].m_QuestPayData_Event.szEventName,(char*)pstQuestEventItem->szItemName);

					TempDBIKQuestEventItem.siDBAccount	= stAccountInfo.siDBAccount;
					TempDBIKQuestEventItem.uiDBSlot		= stAccountInfo.uiSlot;

					TempDBIKQuestEventItem.uiMercenarySlot	=	0;
					TempDBIKQuestEventItem.uiPosition		=	0;

					TempDBIKQuestEventItem.uiQuestCode		= (UI32)stQuestInfo.m_sPlayingQuestHeader[QuestSlot]->siQuestCode;
					TempDBIKQuestEventItem.uiEventItemID	= (UI32)siEventItemID;
					TempDBIKQuestEventItem.uiDelivery		= (UI32)pstQuestEventItem->bDeliveryItem;
					TempDBIKQuestEventItem.uiQuantity		= 1;//stQuestInfo.m_sPlayingQuestHeader[QuestSlot]->sQuestPay[TempCount].m_QuestPayQuantity;
					TempDBIKQuestEventItem.uiItemID			= 0;//(UI32)atoi(pstQuestEventItem->szItemName);

					sprintf(TempDBIKQuestEventItem.szdate,"%d-%d-%d",pMyWorld->RealWorldTime.uiYear,pMyWorld->RealWorldTime.uiMonth,pMyWorld->RealWorldTime.uiDay);

					pMyWorld->clDatabase->writeDBQuery(SO_DB_INFOKIND_QUEST_PAY_EVENTITEM, LPVOID(&TempDBIKQuestEventItem), sizeof(TempDBIKQuestEventItem));
				}
				break;
			case QUESTPAYTYPE_EVENT_GAMEITEM:
				{
					SI16 siEventItemID = stQuestInfo.m_sPlayingQuestHeader[QuestSlot]->sQuestPay[TempCount].m_QuestPayEventItemID;
					//pstQuestEventItem = pMyWorld->m_QuestEventItemParser.GetItemName(siEventItemID);
					pstQuestEventItem = pMyWorld->m_QuestEventItemMgr->GetEventQuest(siQuestCode)->GetItemName(siEventItemID);

					TempPayData[TempCount].m_QuestPayType = QUESTPAYTYPE_NONE;
					for(TempCount1 = 0; TempCount1 < ON_MAX_FOLLOWER_NUM; TempCount1++)
					{
						if(IFollower.IsValidFollower(TempCount1))
						{
							// ���� �������� �ִ��� �˻�
							lpMyItemData = IFollower.GetItemByID(TempCount1, stQuestInfo.m_sPlayingQuestHeader[QuestSlot]->sQuestPay[TempCount].m_QuestPayCode);
							if(lpMyItemData)
							{
								// ������ �� �����ۿ� �߰�
								if(AddItemNoQuery(TempCount1, stQuestInfo.m_sPlayingQuestHeader[QuestSlot]->sQuestPay[TempCount].m_QuestPayCode, stQuestInfo.m_sPlayingQuestHeader[QuestSlot]->sQuestPay[TempCount].m_QuestPayQuantity, lpMyItemData->siPosInInventory) != NULL)
								{
									TempPayData[TempCount].m_QuestPayType                                     = QUESTPAYTYPE_ITEM;
									TempPayData[TempCount].m_QuestPayData_Item.m_FollowerSlot                 = TempCount1;
									TempPayData[TempCount].m_QuestPayData_Item.m_AddItemData.uiID             = stQuestInfo.m_sPlayingQuestHeader[QuestSlot]->sQuestPay[TempCount].m_QuestPayCode;
									TempPayData[TempCount].m_QuestPayData_Item.m_AddItemData.siPosInInventory = lpMyItemData->siPosInInventory;
									TempPayData[TempCount].m_QuestPayData_Item.m_AddItemData.uiQuantity       = 1;

									TempDBIKQuestEventItem.siDBAccount		=	stAccountInfo.siDBAccount;
									TempDBIKQuestEventItem.uiDBSlot			=	stAccountInfo.uiSlot;

									TempDBIKQuestEventItem.uiMercenarySlot	=	TempCount1;
									TempDBIKQuestEventItem.uiPosition		=	lpMyItemData->siPosInInventory;

									TempDBIKQuestEventItem.uiQuestCode		= (UI32)stQuestInfo.m_sPlayingQuestHeader[QuestSlot]->siQuestCode;
									TempDBIKQuestEventItem.uiEventItemID	= (UI32)siEventItemID;
									TempDBIKQuestEventItem.uiDelivery		= (UI32)pstQuestEventItem->bDeliveryItem;
									TempDBIKQuestEventItem.uiQuantity		= 1;
									TempDBIKQuestEventItem.uiItemID			= stQuestInfo.m_sPlayingQuestHeader[QuestSlot]->sQuestPay[0].m_QuestPayCode;//(UI32)atoi(pstQuestEventItem->szItemName);

									sprintf(TempDBIKQuestEventItem.szdate,"%d-%d-%d",pMyWorld->RealWorldTime.uiYear,pMyWorld->RealWorldTime.uiMonth,pMyWorld->RealWorldTime.uiDay);

									pMyWorld->clDatabase->writeDBQuery(SO_DB_INFOKIND_QUEST_PAY_EVENTITEM, LPVOID(&TempDBIKQuestEventItem), sizeof(TempDBIKQuestEventItem));
									break;
								}
							}
							else
							{
								// ������ ��ĭ�� �ִ��� �˻�
								TempItemSlot = IFollower.FindEmptyItemBuffer(TempCount1);
								if((TempItemSlot != -1) && (TempItemSlot != 0xffff))
								{
									// ��ĭ�� ������ �߰�
									if(AddItemNoQuery(TempCount1, stQuestInfo.m_sPlayingQuestHeader[QuestSlot]->sQuestPay[TempCount].m_QuestPayCode, stQuestInfo.m_sPlayingQuestHeader[QuestSlot]->sQuestPay[TempCount].m_QuestPayQuantity, TempItemSlot) != NULL)
									{
										TempPayData[TempCount].m_QuestPayType                                     = QUESTPAYTYPE_ITEM;
										TempPayData[TempCount].m_QuestPayData_Item.m_FollowerSlot                 = TempCount1;
										TempPayData[TempCount].m_QuestPayData_Item.m_AddItemData.uiID             = stQuestInfo.m_sPlayingQuestHeader[QuestSlot]->sQuestPay[TempCount].m_QuestPayCode;
										TempPayData[TempCount].m_QuestPayData_Item.m_AddItemData.siPosInInventory = TempItemSlot;
										TempPayData[TempCount].m_QuestPayData_Item.m_AddItemData.uiQuantity       = 1;

										TempDBIKQuestEventItem.siDBAccount		=	stAccountInfo.siDBAccount;
										TempDBIKQuestEventItem.uiDBSlot			=	stAccountInfo.uiSlot;

										TempDBIKQuestEventItem.uiMercenarySlot	=	TempCount1;
										TempDBIKQuestEventItem.uiPosition		=	TempItemSlot;//lpMyItemData->siPosInInventory;

										TempDBIKQuestEventItem.uiQuestCode		= (UI32)stQuestInfo.m_sPlayingQuestHeader[QuestSlot]->siQuestCode;
										TempDBIKQuestEventItem.uiEventItemID	= (UI32)siEventItemID;
										TempDBIKQuestEventItem.uiDelivery		= (UI32)pstQuestEventItem->bDeliveryItem;
										TempDBIKQuestEventItem.uiQuantity		= 1;
										TempDBIKQuestEventItem.uiItemID			= stQuestInfo.m_sPlayingQuestHeader[QuestSlot]->sQuestPay[0].m_QuestPayCode;//(UI32)atoi(pstQuestEventItem->szItemName);

										sprintf(TempDBIKQuestEventItem.szdate,"%d-%d-%d",pMyWorld->RealWorldTime.uiYear,pMyWorld->RealWorldTime.uiMonth,pMyWorld->RealWorldTime.uiDay);

										pMyWorld->clDatabase->writeDBQuery(SO_DB_INFOKIND_QUEST_PAY_EVENTITEM, LPVOID(&TempDBIKQuestEventItem), sizeof(TempDBIKQuestEventItem));
										break;
									}
								}
							}
						}
					}			
				}
				break;

			case QUESTPAYTYPE_ITEM:
				TempPayData[TempCount].m_QuestPayType = QUESTPAYTYPE_NONE;
				for(TempCount1 = 0; TempCount1 < ON_MAX_FOLLOWER_NUM; TempCount1++)
				{
					if(IFollower.IsValidFollower(TempCount1))
					{
						// ���� �������� �ִ��� �˻�
						lpMyItemData = IFollower.GetItemByID(TempCount1, stQuestInfo.m_sPlayingQuestHeader[QuestSlot]->sQuestPay[TempCount].m_QuestPayCode);
						if(lpMyItemData)
						{
							// ������ �� �����ۿ� �߰�
							if(AddItemNoQuery(TempCount1, stQuestInfo.m_sPlayingQuestHeader[QuestSlot]->sQuestPay[TempCount].m_QuestPayCode, stQuestInfo.m_sPlayingQuestHeader[QuestSlot]->sQuestPay[TempCount].m_QuestPayQuantity, lpMyItemData->siPosInInventory) != NULL)
							{
								TempPayData[TempCount].m_QuestPayType                                     = QUESTPAYTYPE_ITEM;
								TempPayData[TempCount].m_QuestPayData_Item.m_FollowerSlot                 = TempCount1;
								TempPayData[TempCount].m_QuestPayData_Item.m_AddItemData.uiID             = stQuestInfo.m_sPlayingQuestHeader[QuestSlot]->sQuestPay[TempCount].m_QuestPayCode;
								TempPayData[TempCount].m_QuestPayData_Item.m_AddItemData.siPosInInventory = lpMyItemData->siPosInInventory;
								TempPayData[TempCount].m_QuestPayData_Item.m_AddItemData.uiQuantity       = stQuestInfo.m_sPlayingQuestHeader[QuestSlot]->sQuestPay[TempCount].m_QuestPayQuantity;

								TempDBIKQuestPayItem.siDBAccount     = stAccountInfo.siDBAccount;
								TempDBIKQuestPayItem.uiDBSlot        = stAccountInfo.uiSlot;
								TempDBIKQuestPayItem.uiQuestCode     = stQuestInfo.m_sPlayingQuestHeader[QuestSlot]->siQuestCode;
								TempDBIKQuestPayItem.uiMercenarySlot = TempCount1;
								TempDBIKQuestPayItem.uiPosition      = lpMyItemData->siPosInInventory;
								TempDBIKQuestPayItem.siItemID        = stQuestInfo.m_sPlayingQuestHeader[QuestSlot]->sQuestPay[TempCount].m_QuestPayCode;
								TempDBIKQuestPayItem.siQuantity      = stQuestInfo.m_sPlayingQuestHeader[QuestSlot]->sQuestPay[TempCount].m_QuestPayQuantity;
								pMyWorld->clDatabase->writeDBQuery(SO_DB_INFOKIND_QUEST_PAY_ITEM, LPVOID(&TempDBIKQuestPayItem), sizeof(TempDBIKQuestPayItem));				
								break;
							}
						}
						else
						{
							// ������ ��ĭ�� �ִ��� �˻�
							TempItemSlot = IFollower.FindEmptyItemBuffer(TempCount1);
							if((TempItemSlot != -1) && (TempItemSlot != 0xffff))
							{
								// ��ĭ�� ������ �߰�
								if(AddItemNoQuery(TempCount1, stQuestInfo.m_sPlayingQuestHeader[QuestSlot]->sQuestPay[TempCount].m_QuestPayCode, stQuestInfo.m_sPlayingQuestHeader[QuestSlot]->sQuestPay[TempCount].m_QuestPayQuantity, TempItemSlot) != NULL)
								{
									TempPayData[TempCount].m_QuestPayType                                     = QUESTPAYTYPE_ITEM;
									TempPayData[TempCount].m_QuestPayData_Item.m_FollowerSlot                 = TempCount1;
									TempPayData[TempCount].m_QuestPayData_Item.m_AddItemData.uiID             = stQuestInfo.m_sPlayingQuestHeader[QuestSlot]->sQuestPay[TempCount].m_QuestPayCode;
									TempPayData[TempCount].m_QuestPayData_Item.m_AddItemData.siPosInInventory = TempItemSlot;
									TempPayData[TempCount].m_QuestPayData_Item.m_AddItemData.uiQuantity       = stQuestInfo.m_sPlayingQuestHeader[QuestSlot]->sQuestPay[TempCount].m_QuestPayQuantity;

									TempDBIKQuestPayItem.siDBAccount     = stAccountInfo.siDBAccount;
									TempDBIKQuestPayItem.uiDBSlot        = stAccountInfo.uiSlot;
									TempDBIKQuestPayItem.uiQuestCode     = stQuestInfo.m_sPlayingQuestHeader[QuestSlot]->siQuestCode;
									TempDBIKQuestPayItem.uiMercenarySlot = TempCount1;
									TempDBIKQuestPayItem.uiPosition      = TempItemSlot;
									TempDBIKQuestPayItem.siItemID        = stQuestInfo.m_sPlayingQuestHeader[QuestSlot]->sQuestPay[TempCount].m_QuestPayCode;
									TempDBIKQuestPayItem.siQuantity      = stQuestInfo.m_sPlayingQuestHeader[QuestSlot]->sQuestPay[TempCount].m_QuestPayQuantity;
									pMyWorld->clDatabase->writeDBQuery(SO_DB_INFOKIND_QUEST_PAY_ITEM, LPVOID(&TempDBIKQuestPayItem), sizeof(TempDBIKQuestPayItem));				
									break;
								}
							}
						}
					}
				}
				break;
		}
	}

	// ���� Quest�� �ʿ��� Item�� �־�����, �� �������� �����Ѵ�.
	TempDelItemCount = 0;
	TempDeleteMoney  = 0;
	for(TempCount = 0; TempCount < ON_QUSETDATA_MAXQUESTENDREQOBJECT; TempCount++)
	{
		switch(stQuestInfo.m_sPlayingQuestHeader[QuestSlot]->sQuestEndReqObject[TempCount].m_QuestObjectType)
		{
			case QUESTOBJECTTYPE_ITEM:
				{
					lpCItemHeader = pMyWorld->clItem.GetItem(stQuestInfo.m_sPlayingQuestHeader[QuestSlot]->sQuestEndReqObject[TempCount].m_QuestObjectCode);

					if(lpCItemHeader)
					{

						if(stQuestInfo.m_sPlayingQuestHeader[QuestSlot]->m_QuestItemCollect == 1)
						{

							FindDeleteItemNoQuery(
								stQuestInfo.m_sPlayingQuestHeader[QuestSlot]->sQuestEndReqObject[TempCount].m_QuestObjectCode
								, stQuestInfo.m_sPlayingQuestHeader[QuestSlot]->sQuestEndReqObject[TempCount].m_QuestObjectQuantity
								, &TempDelItemCount, (MercenaryItemData*)TempMercenaryItemData);

							for(TempCount1 = 0; TempCount1 < TempDelItemCount; TempCount1++)
							{
								TempDBIKQuestRequest_Item.siDBAccount     = stAccountInfo.siDBAccount;
								TempDBIKQuestRequest_Item.uiDBSlot        = stAccountInfo.uiSlot;
								TempDBIKQuestRequest_Item.uiQuestCode     = stQuestInfo.m_sPlayingQuestHeader[QuestSlot]->siQuestCode;
								TempDBIKQuestRequest_Item.siItemID        = TempMercenaryItemData[TempCount1].siInventoryItemInfo.uiID;
								TempDBIKQuestRequest_Item.uiMercenarySlot = TempMercenaryItemData[TempCount1].siSlot;
								TempDBIKQuestRequest_Item.uiPosition      = TempMercenaryItemData[TempCount1].siInventoryItemInfo.siPosInInventory;
								TempDBIKQuestRequest_Item.siQuantity      = TempMercenaryItemData[TempCount1].siInventoryItemInfo.uiQuantity;
								pMyWorld->clDatabase->writeDBQuery(SO_DB_INFOKIND_QUEST_REQUEST_ITEM, LPVOID(&TempDBIKQuestRequest_Item), sizeof(TempDBIKQuestRequest_Item));				
							}
						}
					}
				}
				break;

			case QUESTOBJECTTYPE_MONEY:
				{
					TempDeleteMoney += stQuestInfo.m_sPlayingQuestHeader[QuestSlot]->sQuestEndReqObject[TempCount].m_QuestObjectQuantity;
					DecreaseMoney(stQuestInfo.m_sPlayingQuestHeader[QuestSlot]->sQuestEndReqObject[TempCount].m_QuestObjectQuantity, FALSE);

					TempDBIKQuestRequest_Money.siDBAccount = stAccountInfo.siDBAccount;
					TempDBIKQuestRequest_Money.uiDBSlot    = stAccountInfo.uiSlot;
					TempDBIKQuestRequest_Money.uiQuestCode = stQuestInfo.m_sPlayingQuestHeader[QuestSlot]->siQuestCode;
					TempDBIKQuestRequest_Money.siMoney     = stQuestInfo.m_sPlayingQuestHeader[QuestSlot]->sQuestEndReqObject[TempCount].m_QuestObjectQuantity;
					pMyWorld->clDatabase->writeDBQuery(SO_DB_INFOKIND_QUEST_REQUEST_MONEY, LPVOID(&TempDBIKQuestRequest_Money), sizeof(TempDBIKQuestRequest_Money));				
				}
				break;
		}
	}

	// Client�� �˸���.
	SendFieldMsg(ON_RESPONSE_QUEST_SELECTNPC, SO_SFM_ONLYME, 
		LPVOID(ON_RET_QUEST_SELECTNPC_ENDQUEST), 
		LPVOID(stQuestInfo.m_sPlayingQuest[QuestSlot].m_QuestCode), LPVOID(TempPayData), LPVOID(TempDelItemCount), LPVOID(TempMercenaryItemData), LPVOID(&TempDeleteMoney));

/*	// �ش� ������ ���� ����Ʈ�� �Ϸ��Ͽ��ٴ� ���� ��� �������� �˷��ش�.
	if(stQuestInfo.m_sPlayingQuest[QuestSlot].m_QuestCode == DOKDO_QUEST_CODE2)
	{
		++pMyWorld->m_uiDokdoQuestCompletedUserCount;

		DBQuestCompletedCount sDBQuestCompletedCount;
		memset(&sDBQuestCompletedCount, 0, sizeof(DBQuestCompletedCount));		

		sDBQuestCompletedCount.siDBAccount				= stAccountInfo.siDBAccount;
		sDBQuestCompletedCount.uiDBSlot					= stAccountInfo.uiSlot;
		sDBQuestCompletedCount.uiQuestCode				= DOKDO_QUEST_CODE2;
		sDBQuestCompletedCount.nQuestCompletedCount		= pMyWorld->m_uiDokdoQuestCompletedUserCount;
		pMyWorld->clDatabase->writeDBQuery(SO_DB_INFO_DOKDO_QUEST_COMPLETED_SAVE, 
			LPVOID(&sDBQuestCompletedCount), sizeof(sDBQuestCompletedCount));			

		SendFieldMsg(ON_RESPONSE_QUEST_DOKDO_COMPLETED_USER_INFO, SO_SFM_ALLCHAR, 
			LPVOID(logininfo->charBaseInfo.name), LPVOID(pMyWorld->m_uiDokdoQuestCompletedUserCount));
	}
*/
//	if(stQuestInfo.m_sPlayingQuestHeader[QuestSlot]->siQuestCode != DOKDO_QUEST_CODE1 && 
//	   stQuestInfo.m_sPlayingQuestHeader[QuestSlot]->siQuestCode != DOKDO_QUEST_CODE2)
	if(stQuestInfo.m_sPlayingQuestHeader[QuestSlot]->m_siQuestEvent != QUEST_EVENT_PLAY)
	{
		// Data�� �ʱ�ȭ �Ѵ�.
		for(TempCount = 0; TempCount < ON_QUSETDATA_MAXCOMLETEDQUEST; TempCount++)
		{
			if(stQuestInfo.m_siCompletedQuest[TempCount] == 0 )
			{
				stQuestInfo.m_siCompletedQuest[TempCount]       = stQuestInfo.m_sPlayingQuest[QuestSlot].m_QuestCode;
				stQuestInfo.m_siCompletedQuestHeader[TempCount] = pMyWorld->clQuestParser.GetQuest(stQuestInfo.m_siCompletedQuest[TempCount]);
				break;
			}
		}
	}

	ZeroMemory(&stQuestInfo.m_sPlayingQuest[QuestSlot], sizeof(SPlayingQuest));
	stQuestInfo.m_sPlayingQuestHeader[QuestSlot] = NULL;
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	�� NPC���Լ� �����Ҽ� �ִ� ���� Quest�� ã�´�.
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
SI16	SOPlayer::Quest_FindNextQuest(SI16 NPCCode)
{
	SI32				TempCount;
	SI32				TempSmallCode;
	SI32				TempMinCode;
	SI32				TempSlot;

	if(NPCCode >= pMyWorld->clQuestParser.m_NPCSortData_MaxCount) return 0;

	// ������ �ּ� �ڵ带 ã�´�.
	TempMinCode = 0;
	for(TempCount = 0; TempCount < ON_QUSETDATA_MAXCOMLETEDQUEST; TempCount++)
	{
		if(stQuestInfo.m_siCompletedQuest[TempCount] > 0)
		{
			if(stQuestInfo.m_siCompletedQuestHeader[TempCount]->siNPCCode == NPCCode)
			{
				if(stQuestInfo.m_siCompletedQuest[TempCount] > TempMinCode)
				{
					TempMinCode = stQuestInfo.m_siCompletedQuest[TempCount];
				}
			}
		}
	}

	// ������ �Ҽ� �ִ� Quest�� ã�´�.
	TempSmallCode = 10000000;
	for(TempCount = 0; TempCount < pMyWorld->clQuestParser.m_NPCSortData_DataCount[NPCCode]; TempCount++)
	{
		TempSlot = pMyWorld->clQuestParser.m_NPCSortData_Data[NPCCode][TempCount];
		if(pMyWorld->clQuestParser.m_lpQuestData[TempSlot].siQuestCode > TempMinCode)
		{
			if(Quest_CheckSkipQuestAbility(pMyWorld->clQuestParser.m_lpQuestData[TempSlot].siQuestCode) == FALSE)
			{
				if(pMyWorld->clQuestParser.m_lpQuestData[TempSlot].siQuestCode < TempSmallCode)
				{
					TempSmallCode = pMyWorld->clQuestParser.m_lpQuestData[TempSlot].siQuestCode;
				}			
			}
		}		
	}

	if(TempSmallCode == 10000000) TempSmallCode = 0;

	return TempSmallCode;
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	Quest�� Start�Ҽ� �ִ��� �˻�
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
BOOL	SOPlayer::Quest_CheckPlayQuestAbility(SI16 QuestCode)
{
	SI32				TempCount;
	SI32				TempCount1;
	SQuestHeader*		lpSQuestHeader;
	SI16				TempSI16;
	BOOL				fEmptyBuffer;

	// ���� ���������� �˻�
	for(TempCount = 0; TempCount < ON_QUSETDATA_MAXPLAYINGQUEST; TempCount++)
	{
		if(stQuestInfo.m_sPlayingQuest[TempCount].m_QuestCode == QuestCode)
		{
			return FALSE;
		}
	}

	// �� Slot�� �ִ��� �˻�
	fEmptyBuffer = FALSE;
	for(TempCount = 0; TempCount < ON_QUSETDATA_MAXPLAYINGQUEST; TempCount++)
	{
		if(stQuestInfo.m_sPlayingQuest[TempCount].m_QuestCode <= 0)
		{
			fEmptyBuffer = TRUE;
			break;
		}
	}
	if(fEmptyBuffer == FALSE) return FALSE;

	// ������ �´��� �˻�
	lpSQuestHeader = pMyWorld->clQuestParser.GetQuest(QuestCode);
	if(lpSQuestHeader == NULL) return FALSE;
	for(TempCount = 0; TempCount < ON_QUSETDATA_MAXQUESTREQCONDITIONAL; TempCount++)
	{
		switch(lpSQuestHeader->sQuestReqConditional[TempCount].m_QuestConditionalType)
		{
			case QUESTCONDITIONALTYPE_CREDIT:
				if(uiTradeCredit < lpSQuestHeader->sQuestReqConditional[TempCount].m_QuestConditionalQuantity)
				{
					return FALSE;
				}
				break;

			case QUESTCONDITIONALTYPE_LEVEL:
				if(IFollower.GetFollowerParameterLevel(0) < lpSQuestHeader->sQuestReqConditional[TempCount].m_QuestConditionalQuantity)
				{
					return FALSE;
				}
				break;

			case QUESTCONDITIONALTYPE_BATTLE:
				if(GetFightingPower() < lpSQuestHeader->sQuestReqConditional[TempCount].m_QuestConditionalQuantity)
				{
					return FALSE;
				}
				break;

			case QUESTCONDITIONALTYPE_COMPLETIONQUEST:
				TempSI16 = lpSQuestHeader->sQuestReqConditional[TempCount].m_QuestConditionalQuantity;
				for(TempCount1 = 0; TempCount1 < ON_QUSETDATA_MAXCOMLETEDQUEST; TempCount1++)
				{
					if(stQuestInfo.m_siCompletedQuest[TempCount1] == TempSI16)
					{
						goto GOTO_COMPLETIONQUEST;
					}
				}
				return FALSE;
GOTO_COMPLETIONQUEST:
				break;

			case QUESTCONDITIONALTYPE_ITEM:
				if(IFollower.CheckItem(lpSQuestHeader->sQuestReqConditional[TempCount].m_QuestConditionalCode, lpSQuestHeader->sQuestReqConditional[TempCount].m_QuestConditionalQuantity) == FALSE)
				{
					return FALSE;
				}
				break;
		}
	}

	return TRUE;
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	Quest�� Skip�Ҽ� �ִ��� �˻�
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
BOOL	SOPlayer::Quest_CheckSkipQuestAbility(SI16 QuestCode)
{
	SI32				TempCount;
	SI32				TempCount1;
	SQuestHeader*		lpSQuestHeader;
	SI16				TempSI16;

	// ������ �´��� �˻�
	lpSQuestHeader = pMyWorld->clQuestParser.GetQuest(QuestCode);
	if(lpSQuestHeader == NULL) return FALSE;
	for(TempCount = 0; TempCount < ON_QUSETDATA_MAXQUESTSKIPCONDITIONAL; TempCount++)
	{
		switch(lpSQuestHeader->sQuestSkipConditional[TempCount].m_QuestConditionalType)
		{
			case QUESTCONDITIONALTYPE_CREDIT:
				if(uiTradeCredit < lpSQuestHeader->sQuestSkipConditional[TempCount].m_QuestConditionalQuantity)
				{
					return FALSE;
				}
				break;

			case QUESTCONDITIONALTYPE_LEVEL:
				if(IFollower.GetFollowerParameterLevel(0) < lpSQuestHeader->sQuestSkipConditional[TempCount].m_QuestConditionalQuantity)
				{
					return FALSE;
				}
				break;

			case QUESTCONDITIONALTYPE_BATTLE:
				if(GetFightingPower() < lpSQuestHeader->sQuestSkipConditional[TempCount].m_QuestConditionalQuantity)
				{
					return FALSE;
				}
				break;

			case QUESTCONDITIONALTYPE_COMPLETIONQUEST:
				TempSI16 = lpSQuestHeader->sQuestSkipConditional[TempCount].m_QuestConditionalQuantity;
				for(TempCount1 = 0; TempCount1 < ON_QUSETDATA_MAXCOMLETEDQUEST; TempCount1++)
				{
					if(stQuestInfo.m_siCompletedQuest[TempCount1] == TempSI16)
					{
						goto GOTO_COMPLETIONQUEST;
					}
				}
				return FALSE;
GOTO_COMPLETIONQUEST:
				break;

			case QUESTCONDITIONALTYPE_ITEM:
				if(IFollower.CheckItem(lpSQuestHeader->sQuestSkipConditional[TempCount].m_QuestConditionalCode, lpSQuestHeader->sQuestSkipConditional[TempCount].m_QuestConditionalQuantity) == FALSE)
				{
					return FALSE;
				}
				break;
		}
	}

	return TRUE;
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	�� NPC���Լ� ���������� Play�� Quest�˻�
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
SI16	SOPlayer::Quest_FindLastPlayQuest(SI16 NPCCode)
{
	SI16		TempLastQuest;
	SI16		TempCount;

	TempLastQuest = 0;
	for(TempCount = 0; TempCount < ON_QUSETDATA_MAXCOMLETEDQUEST; TempCount++)
	{
		if(stQuestInfo.m_siCompletedQuest[TempCount] > 0)
		{
			if(stQuestInfo.m_siCompletedQuestHeader[TempCount]->siNPCCode == NPCCode)
			{
				if(stQuestInfo.m_siCompletedQuest[TempCount] > TempLastQuest)
				{
					TempLastQuest = stQuestInfo.m_siCompletedQuest[TempCount];
				}
			}
		}
	}

	return TempLastQuest;
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	������ ������ �ִ� Quest���� �˻�
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
BOOL	SOPlayer::Quest_FindCompletedQuest(SI16 QuestCode)
{
	SI32	TempCount;

	for(TempCount = 0; TempCount < ON_QUSETDATA_MAXCOMLETEDQUEST; TempCount++)
	{
		if(stQuestInfo.m_siCompletedQuest[TempCount] == QuestCode)
		{
			return TRUE;
		}
	}

	return FALSE;
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	�� Quest Slot�� ã��
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
SI16	SOPlayer::Quest_FindEmptyQuest(void)
{
	SI16		TempCount;

	for(TempCount = 0; TempCount < ON_QUSETDATA_MAXPLAYINGQUEST; TempCount++)
	{
		if(stQuestInfo.m_sPlayingQuest[TempCount].m_QuestCode == 0)
		{
			return TempCount;
		}
	}

	return -1;
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	�������� Quest�� ã��
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
SI16	SOPlayer::Quest_FindQuest(SI16 QuestCode)
{
	SI16		TempCount;

	if(QuestCode <= 0) return -1;

	for(TempCount = 0; TempCount < ON_QUSETDATA_MAXPLAYINGQUEST; TempCount++)
	{
		if(stQuestInfo.m_sPlayingQuest[TempCount].m_QuestCode == QuestCode)
		{
			return TempCount;
		}
	}

	return -1;
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	��� Quest �ʱ�ȭ, ����ϰ�
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
VOID	SOPlayer::Quest_AllClear(void)
{
	ZeroMemory(stQuestInfo.m_siCompletedQuest,       sizeof(SI16) * ON_QUSETDATA_MAXCOMLETEDQUEST);
	ZeroMemory(stQuestInfo.m_siCompletedQuestHeader, sizeof(SQuestHeader*) * ON_QUSETDATA_MAXCOMLETEDQUEST);
	
	ZeroMemory(stQuestInfo.m_sPlayingQuest,       sizeof(SPlayingQuest) * ON_QUSETDATA_MAXPLAYINGQUEST);
	ZeroMemory(stQuestInfo.m_sPlayingQuestHeader, sizeof(SQuestHeader*) * ON_QUSETDATA_MAXPLAYINGQUEST);

	SendFieldMsg(ON_RESPONSE_QUEST_ALLCLEARQUEST, SO_SFM_ONLYME);
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	���� ���� Data
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
VOID	SOPlayer::Quest_AddQuestObject_KillMonster(SI32 MonsterCode, SI32 Count)
{
	SI32							TempCount;
	SI32							TempCount1;
	DBIKQuestSetPlayQuestData		TempDBIKQuestSetPlayQuestData;

	TempDBIKQuestSetPlayQuestData.DBAccount  = stAccountInfo.siDBAccount;
	TempDBIKQuestSetPlayQuestData.uiSlot     = stAccountInfo.uiSlot;
	for(TempCount = 0; TempCount < ON_QUSETDATA_MAXPLAYINGQUEST; TempCount++)
	{
		if(stQuestInfo.m_sPlayingQuest[TempCount].m_QuestCode > 0)
		{
			for(TempCount1 = 0; TempCount1 < ON_QUSETDATA_MAXQUESTENDREQOBJECT; TempCount1++)
			{
				switch(stQuestInfo.m_sPlayingQuestHeader[TempCount]->sQuestEndReqObject[TempCount1].m_QuestObjectType)
				{
					case QUESTOBJECTTYPE_MONSTER:
						if(stQuestInfo.m_sPlayingQuestHeader[TempCount]->sQuestEndReqObject[TempCount1].m_QuestObjectCode == MonsterCode)
						{
							stQuestInfo.m_sPlayingQuest[TempCount].m_QuestObject[TempCount1] += Count;

							TempDBIKQuestSetPlayQuestData.QuestIndex  = TempCount;
							CopyMemory(&TempDBIKQuestSetPlayQuestData.QuestData, &stQuestInfo.m_sPlayingQuest[TempCount], sizeof(SPlayingQuest));
							pMyWorld->clDatabase->writeDBQuery(SO_DB_INFOKIND_QUEST_SETPLAYQUESTDATA, LPVOID(&TempDBIKQuestSetPlayQuestData), sizeof(TempDBIKQuestSetPlayQuestData));
							
							SendFieldMsg(ON_RESPONSE_QUEST_GETPLAYQUESTDATA, SO_SFM_ONLYME, LPVOID(&stQuestInfo.m_sPlayingQuest[TempCount]));
						}
						break;
				}
			}
		}
	}
}

VOID	SOPlayer::Quest_AddQuestObject_PortalUse(SI32 siMapIndex)
{
	SI32							TempCount;
	SI32							TempCount1;
	DBIKQuestSetPlayQuestData		TempDBIKQuestSetPlayQuestData;

	TempDBIKQuestSetPlayQuestData.DBAccount  = stAccountInfo.siDBAccount;
	TempDBIKQuestSetPlayQuestData.uiSlot     = stAccountInfo.uiSlot;

	for(TempCount = 0; TempCount < ON_QUSETDATA_MAXPLAYINGQUEST; TempCount++)
	{
		if(stQuestInfo.m_sPlayingQuest[TempCount].m_QuestCode > 0)
		{
			for(TempCount1 = 0; TempCount1 < ON_QUSETDATA_MAXQUESTENDREQOBJECT; TempCount1++)
			{
				switch(stQuestInfo.m_sPlayingQuestHeader[TempCount]->sQuestEndReqObject[TempCount1].m_QuestObjectType)
				{
					case QUESTOBJECTTYPE_PORTAL:
						if(stQuestInfo.m_sPlayingQuestHeader[TempCount]->sQuestEndReqObject[TempCount1].m_QuestObjectCode == siMapIndex)
						{
							stQuestInfo.m_sPlayingQuest[TempCount].m_QuestObject[TempCount1] = 1;

							TempDBIKQuestSetPlayQuestData.QuestIndex  = TempCount;
							CopyMemory(&TempDBIKQuestSetPlayQuestData.QuestData, &stQuestInfo.m_sPlayingQuest[TempCount], sizeof(SPlayingQuest));
							pMyWorld->clDatabase->writeDBQuery(SO_DB_INFOKIND_QUEST_SETPLAYQUESTDATA, LPVOID(&TempDBIKQuestSetPlayQuestData), sizeof(TempDBIKQuestSetPlayQuestData));
							
							SendFieldMsg(ON_RESPONSE_QUEST_GETPLAYQUESTDATA, SO_SFM_ONLYME, LPVOID(&stQuestInfo.m_sPlayingQuest[TempCount]));
						}
						break;
				}
			}
		}
	}
}

VOID	SOPlayer::Quest_AddQuestObject_Npc(SI32 siMapIndex)
{
	SI32							TempCount;
	SI32							TempCount1;
	DBIKQuestSetPlayQuestData		TempDBIKQuestSetPlayQuestData;

	TempDBIKQuestSetPlayQuestData.DBAccount  = stAccountInfo.siDBAccount;
	TempDBIKQuestSetPlayQuestData.uiSlot     = stAccountInfo.uiSlot;

	for(TempCount = 0; TempCount < ON_QUSETDATA_MAXPLAYINGQUEST; TempCount++)
	{
		if(stQuestInfo.m_sPlayingQuest[TempCount].m_QuestCode > 0)
		{
			for(TempCount1 = 0; TempCount1 < ON_QUSETDATA_MAXQUESTENDREQOBJECT; TempCount1++)
			{
				switch(stQuestInfo.m_sPlayingQuestHeader[TempCount]->sQuestEndReqObject[TempCount1].m_QuestObjectType)
				{
					case QUESTOBJECTTYPE_NPC:
						if(stQuestInfo.m_sPlayingQuestHeader[TempCount]->sQuestEndReqObject[TempCount1].m_QuestObjectCode == siMapIndex)
						{
							stQuestInfo.m_sPlayingQuest[TempCount].m_QuestObject[TempCount1] = 1;

							TempDBIKQuestSetPlayQuestData.QuestIndex  = TempCount;
							CopyMemory(&TempDBIKQuestSetPlayQuestData.QuestData, &stQuestInfo.m_sPlayingQuest[TempCount], sizeof(SPlayingQuest));
							pMyWorld->clDatabase->writeDBQuery(SO_DB_INFOKIND_QUEST_SETPLAYQUESTDATA, LPVOID(&TempDBIKQuestSetPlayQuestData), sizeof(TempDBIKQuestSetPlayQuestData));
							
							SendFieldMsg(ON_RESPONSE_QUEST_GETPLAYQUESTDATA, SO_SFM_ONLYME, LPVOID(&stQuestInfo.m_sPlayingQuest[TempCount]));
						}
						break;
				}
			}
		}
	}
}
 
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	�ڽ��� ����
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
VOID	SOPlayer::Quest_AddQuestObject_Work(SI32 siWork)
{
	SI32							TempCount;
	SI32							TempCount1;
	DBIKQuestSetPlayQuestData		TempDBIKQuestSetPlayQuestData;

	TempDBIKQuestSetPlayQuestData.DBAccount  = stAccountInfo.siDBAccount;
	TempDBIKQuestSetPlayQuestData.uiSlot     = stAccountInfo.uiSlot;
	for(TempCount = 0; TempCount < ON_QUSETDATA_MAXPLAYINGQUEST; TempCount++)
	{
		if(stQuestInfo.m_sPlayingQuest[TempCount].m_QuestCode > 0)
		{
			for(TempCount1 = 0; TempCount1 < ON_QUSETDATA_MAXQUESTENDREQOBJECT; TempCount1++)
			{
				switch(stQuestInfo.m_sPlayingQuestHeader[TempCount]->sQuestEndReqObject[TempCount1].m_QuestObjectType)
				{
					case QUESTOBJECTTYPE_WORK:
						stQuestInfo.m_sPlayingQuest[TempCount].m_QuestObject[TempCount1] += siWork;

						TempDBIKQuestSetPlayQuestData.QuestIndex  = TempCount;
						CopyMemory(&TempDBIKQuestSetPlayQuestData.QuestData, &stQuestInfo.m_sPlayingQuest[TempCount], sizeof(SPlayingQuest));
						pMyWorld->clDatabase->writeDBQuery(SO_DB_INFOKIND_QUEST_SETPLAYQUESTDATA, LPVOID(&TempDBIKQuestSetPlayQuestData), sizeof(TempDBIKQuestSetPlayQuestData));

						SendFieldMsg(ON_RESPONSE_QUEST_GETPLAYQUESTDATA, SO_SFM_ONLYME, LPVOID(&stQuestInfo.m_sPlayingQuest[TempCount]));
						break;
				}
			}
		}
	}
}

