#include "SOMain.h"
#include "SOWorld.h"
#include "SOPlayer.h"
#include "ParametaBox.h"


//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
// 플레이어를 이동시킨다.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━


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

	if(uiBoardShipAccount != 0)																						goto	END;		// 현재 배에 탑승되어 있는 상태라면 이동하지 못한다.
	if(GetBattle() != NULL)																							goto	END;		// 현재 전투 대기중이면 이동하지 못한다.

	if(pMyWorld->clISOMap.MoveChar( this, pMyCharMoveMsg->uiX, pMyCharMoveMsg->uiY, pMyCharMoveMsg->bWarUnit,uiMoveableTileInfo) == FALSE)	goto	END;		// 캐릭터를 이동 시킨다.

	// 이동에 성공하였다.
	bSuccess	=	TRUE;

END:

	if(bSuccess == FALSE)
	{
		// 그 위치에는 캐릭터를 설정할 수 없다.
		// 원래 위치로 이동하라고 메시지를 보내준다.
		SendFieldMsg( ON_RESPONSE_POSITION, SO_SFM_ONLYME, LPVOID( &uiAccount ), LPVOID( 1 ) );
	}	
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명	:	워프 한다는 메시지를 받았다.			
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
BOOL	SOPlayer::RecvWarp()
{
	BOOL			bSuccess		=	FALSE;
	OnReqWarpMsg	*pReqWarpMsg;
	strWarpInfo		*pstWarpInfo;

	pReqWarpMsg	=	(OnReqWarpMsg*)cMsg;

	pstWarpInfo	=	pMyWorld->m_clWarpParser.GetWarpInfo( pReqWarpMsg->uiWarpCode );

	// 워프할 캐릭터의 아이디가 자신의 아이디인 경우
	bSuccess	=	Warp( pstWarpInfo->uiMapIndex, pstWarpInfo->uiX, pstWarpInfo->uiY );

	if( bSuccess )		SendFieldMsg( ON_RESPONSE_WARP, SO_SFM_ONLYME, LPVOID(ON_RET_OK) );
	else				SendFieldMsg( ON_RESPONSE_WARP, SO_SFM_ONLYME, LPVOID(ON_RET_NO) );

	return	bSuccess;
}


//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명	:	워프를 하고 주위의 데이타를 받는다.
//				인자로 들어오는 siX, siY는 기준이 되는 위치이지
//				반드시 그 위치로 워프가 되는 것은 아니다.
//				그 위치가 유효한 위치가 아닐 경우에는 그 주위의 위치로 워프 될 수도 있다.
//				워프에 성공 했을 경우에만 이 함수내에서 해당 플레이어에게 워프 메시지를 보내고
//				워프에 실패 했을 경우에는 이 함수를 호출 하는 부분에서 메시지를 보내준다.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
BOOL	SOPlayer::Warp(UI16 uimapid, SI32 six, SI32 siy)
{
	UI16							uiCharFItemInfo[2];			// 0번은 캐릭터 수, 1번은 아이템 수,
	UI16							uiCharUniIDs[ON_STATUS_AREA_XSIZE * ON_STATUS_AREA_YSIZE];	
	OnlineReceiveFieldItemInfo		FieldItemInfo[ON_STATUS_AREA_XSIZE * ON_STATUS_AREA_YSIZE];
	BOOL							bWarpSuccess = FALSE;
	UI16							uiPrevMapID;
	SI32							siPrevX, siPrevY;	
	POINT							ptWarpXY;

	// 현재 반드시 필드 상에 있어야 한다.
	if( CHECKSTATUS( siStatus, ON_PLAYER_STATUS_INFIELD ) == FALSE )	return	FALSE;
	//if(siStatus	!= ON_PLAYER_STATUS_INFIELD)				return FALSE;

	// 워프할 맵의 반드시 유효해야 한다.
	if(pMyWorld->clISOMap.IsValidMapID(uimapid) == FALSE)	return	FALSE;

	// 워프할 곳을 찾는다.
	ptWarpXY	=	pMyWorld->clISOMap.SearchMap(uimapid, six, siy, ON_MAP_SI_CHAR, uiMoveableTileInfo);

	// 워프할 곳에 워프할 자리를 찾지 못해서 실패했다.				
	if((ptWarpXY.x == -1) && (ptWarpXY.y == -1))	return FALSE;

	// 현재 맵의 ID를 저장한다.
	uiPrevMapID		=	uiMapID;
	siPrevX			=	siX;
	siPrevY			=	siY;	
	
	// 맵의 현재 위치에서 내 ID를 지운다.
	if( DelIDOnMap( 4 ) == FALSE)
		writeInfoToFile("DelIDOnMap Failed.txt", "4 [%u]", uiAccount);
//		pMyWorld->clServer->WriteInfo("..\\DelIDOnMap Failed.txt", "4 [%u]", uiAccount);

	// 우선 맵을 변경한다.
	// 변경하려는 맵이 없으면 FALSE를 리턴한다.
	// 사용자는 ChangeMap을 반드시 호출해주어야 한다.
	if(ChangeMap(uimapid, SI16(ptWarpXY.x), SI16(ptWarpXY.y)) == TRUE)			
	{		
		// 맵의 새로운 위치에 내 ID를 설정한다.
		if(SetIDOnMap( 5 ) == FALSE)
			writeInfoToFile("SetIDOnMap Failed.txt", "6");		
//			pMyWorld->clServer->WriteInfo("..\\SetIDOnMap Failed.txt", "6");		
		
		// 내 주위에 있는 캐릭터의 ID와 아이템을 얻어온다.
		uiCharFItemInfo[0]		=	pMyWorld->clISOMap.GetNearSectorPlayerList(uiMapID, siX, siY, uiCharUniIDs, ON_MAP_CHARID_FIELD);
		uiCharFItemInfo[1]		=	pMyWorld->clISOMap.GetNearSectorItemList(uiMapID, siX, siY, FieldItemInfo);				

		// 우선 나에게 내 주위의 캐릭터와 아이템의 정보를 보내준다.
		SendFieldMsg(ON_RESPONSE_WARP, SO_SFM_ONLYME, 
						LPVOID(ON_RET_OK),
						LPVOID(uiCharFItemInfo), 
						LPVOID(uiCharUniIDs), 
						LPVOID(FieldItemInfo));		

		bWarpSuccess	=	TRUE;	
	}

	if(bWarpSuccess == FALSE)
	{
		// 새로운 지점으로 워프하는데 실패했다면
		// 원래 자리로 다시 워프한다.
		BaseChar::Warp(uiPrevMapID, siPrevX, siPrevY);
	}

	return bWarpSuccess;
}

//----------------------------------------------------------------------------------------------------------------
//	설명	:	특정 플레이어의 정보를 요청한다
//----------------------------------------------------------------------------------------------------------------
VOID	SOPlayer::RecvPlayerInfo()
{
	OnReqPlayerInfoMsg		*pReqPlayerInfoMsg;
	UI16					*puiCharsID;
	UI16					uiCharIDs[20];					// 한번에 20명 이상은 보내주지 않는다.
	UI16					uiCharNum;
	SI32					i;

	// 정보를 보내준다.
	pReqPlayerInfoMsg		=	(OnReqPlayerInfoMsg*)cMsg;	
	puiCharsID				=	(UI16*)((CHAR*)pReqPlayerInfoMsg + sizeof(OnReqPlayerInfoMsg));
	
	ZeroMemory(uiCharIDs, sizeof(uiCharIDs));
	
	// 현재 유효한 캐릭터만 추출한다.
	for(uiCharNum = 0, i = 0; i < pReqPlayerInfoMsg->uiCharNum; i++)
	{
		if(IsValidID(puiCharsID[i]) == TRUE)
		{			
			uiCharIDs[uiCharNum]	=	puiCharsID[i];
			uiCharNum++;

			// 현재는 20명으로 제한하다.
			if(uiCharNum >= 20)
				break;
		}
	}

	// 메시지를 보내준다.
	SendFieldMsg(ON_RESPONSE_PLAYER_INFO, SO_SFM_ONLYME, LPVOID(uiCharNum), LPVOID(uiCharIDs), LPVOID(uiGuildID));	
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	캐릭터의 상세 정보를 요청한다는 메시지를 받았다.
//	수정 일자	:	2002-08-15 오후 3:05:51 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
VOID	SOPlayer::RecvCharDetailInfo()
{
	OnReqCharDetailInfoMsg		*pReqCharDetailInfoMsg;
	SOPlayer					*pPlayer;

	pReqCharDetailInfoMsg		=	(OnReqCharDetailInfoMsg*)cMsg;

	if(ISPLAYER(pReqCharDetailInfoMsg->uiAccount) == TRUE)
	{
		if((pPlayer = pMyWorld->GetPlayer(pReqCharDetailInfoMsg->uiAccount)) != NULL)
		{
			// 플레이어가 유효하다.
			// 캐릭터의 상세 정보를 보내준다.
			SendFieldMsg(ON_RESPONSE_CHAR_DETAILINFO, SO_SFM_ONLYME, LPVOID(ON_RET_OK), LPVOID(pPlayer));
		}
		else
		{
			// 플레이어가 유효하지 않다.
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

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명	:	마을에 들어간다는 메시지를 받았다.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
VOID SOPlayer::RecvEnterVillage()
{
	OnReqGotoVillageMsg		*pReqGotoVillageMsg;	
	DWORD					dwResponse = ON_RET_NO;
	UI32					uiVillageGuildID;
	cltGuild*				pclGuild;

	pclGuild = pMyWorld->clIGuild.GetGuildByID(uiGuildID);
	pReqGotoVillageMsg	=	(OnReqGotoVillageMsg*)cMsg;
		
	// 현재 내가 전투 대기중이거나 전투중에 있으면 들어가지 못한다.
	if( GetBattle() != NULL )			goto	END;	

	// 현재 내가 배에 탑승중이면 들어가지 못한다.
	if( uiBoardShipAccount != 0)		goto	END;

	// 우선 유효한 마을인지 알아온다.
	stAccountInfo.pVillage	=	pMyWorld->GetVillageByCode(pReqGotoVillageMsg->uiVillageCode);				// 마을을 찾는다.
	if(stAccountInfo.pVillage == NULL)	goto	END;

	if( (stAccountInfo.pVillage->uiMapID != uiMapID) ||
		( (abs(stAccountInfo.pVillage->siVillageX - siX) > 10) && (abs(stAccountInfo.pVillage->siVillageY - siY) > 10) ) )
	{
		writeInfoToFile( "WarpHacking.txt", "DBAccount(%d)  szID(%s)  Player MapIndex(%d) X(%d) Y(%d) Try to Enter village(%s)",
						stAccountInfo.siDBAccount, szID, uiMapID, siX, siY, stAccountInfo.pVillage->GetVillageName() );
		goto	END;
	}

	// 마을의 Guild ID를 구한다.
	uiVillageGuildID		=	stAccountInfo.pVillage->GetGuildNID();

	// 적 마을의 상단이면 들어갈 수 없다.
	if((uiGuildID != 0) && (uiVillageGuildID != 0) && (siClassInGuild != pMyWorld->clIGuildClass.GetJWJCode()))
	{
		if(pMyWorld->clIGuild.IsEnemyRelationWar(uiGuildID,uiVillageGuildID) == TRUE || pMyWorld->clIGuild.IsEnemyGuildBoth(uiGuildID, uiVillageGuildID) == TRUE)
		{
			dwResponse	=	ON_RET_ENEMY_GUILD_VILLAGE;
			goto	END;
		}
	}

	
	// 맵에서 내 ID를 지운다.
	// 반드시 마을에 들어가기전에 해주어야 한다.
	if(DelIDOnMap( 6 ) == FALSE)
		writeInfoToFile("DelIDOnMap Failed.txt", "5 [%u] [%d] [%d] [%d]", uiAccount, uiMapID, siX, siY);
//		pMyWorld->clServer->WriteInfo("..\\DelIDOnMap Failed.txt", "5 [%u] [%d] [%d] [%d]", uiAccount, uiMapID, siX, siY);

	// 마을에 들어간다.
	EnterVillage();	

	dwResponse	=	ON_RET_OK;
END:
	// 마을에 들어갈 수 없다.
	switch(dwResponse)
	{
	case ON_RET_OK:
		// 주위 플레이어에게 우선 알려준다.
		SendFieldMsg(ON_RESPONSE_ENTER_VILLAGE, SO_SFM_ONLYNEARCHAR, LPVOID(dwResponse));	
			
		// 그리고 나에게도 알려준다.		
		SendFieldMsg(ON_RESPONSE_ENTER_VILLAGE, SO_SFM_ONLYME, LPVOID(dwResponse), LPVOID(stAccountInfo.pVillage));
		break;
	case ON_RET_NO:
	case ON_RET_ENEMY_GUILD_VILLAGE:		
		SendFieldMsg(ON_RESPONSE_ENTER_VILLAGE, SO_SFM_ONLYME, LPVOID(dwResponse));				
		break;
	}
	
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	마을에 들어간다
//	수정 일자	:	2002-09-28 오전 11:35:00 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
BOOL	SOPlayer::EnterVillage( BOOL bIsVillageField )
{
	if(stAccountInfo.pVillage->EnterVillage( this ) == TRUE)
	{
		// 마을에 들어간 상태로 siStatus를 설정한다.
		//siStatus							=	ON_PLAYER_STATUS_INVILLAGE;		
		if( bIsVillageField == TRUE)
		{
			// 저잣거리이다.
			ADDSTATUS( siStatus, ON_PLAYER_STATUS_INVILLAGE );
		}
		else
		{
			// 자잣거리가 아니다.
			SETSTATUS( siStatus, ON_PLAYER_STATUS_INVILLAGE );
		}			
		
		stAccountInfo.uiStatusInVillage		=	INVILLAGE_STATUS_NONE;	

		// 이전에 저장한지 5분이 지났다면 DB에 설정해준다.
		if( (int)(pMyWorld->dwtimeGetTime - m_latestSavedPosTime) > 300000 ) savePosToDB();		

		// 교역창을 닫는다.
		CloseTrade();

		// 타 유저에게 파티 신청중이거나 
		// 다른 유저에 파티 신청을 받았다면 이를 취소한다.
		CloseRequestingJoinParty();

		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	마을에서 떠난다.
//	수정 일자	:	2002-09-28 오전 11:38:37 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
VOID SOPlayer::LeaveVillage( BOOL bIsVillageField )
{	
	// 필드에 있는 상태로 siStatus를 설정한다.
	//siStatus							=	ON_PLAYER_STATUS_INFIELD;	
	if( bIsVillageField == TRUE)
	{
		// 저잣거리이다.
		if( CHECKSTATUS( siStatus, ON_PLAYER_STATUS_INVILLAGE ) == TRUE)
		{
			REMOVESTATUS( siStatus, ON_PLAYER_STATUS_INVILLAGE );
		}
	}
	else
	{
		// 자잣거리가 아니다.
		SETSTATUS( siStatus, ON_PLAYER_STATUS_INFIELD );
	}			
	
	stAccountInfo.pVillage				=	NULL;	
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	아이템을 버린다는 메시지를 받았다.
//	수정 일자	:	2002-11-19 오전 9:21:47 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
VOID	SOPlayer::RecvThrowItem()
{
	OnReqThrowItemMsg		*pReqThrowItemMsg;
	BOOL					bSuccess = FALSE;
	POINT					ptThrowXY;
	CHAR					szDBArg[ 1024 ];

	pReqThrowItemMsg	=	(OnReqThrowItemMsg*)cMsg;

	//pReqThrowItemMsg->

	// 아이템을 버릴 수 있는 자리가 있는지 판단한다.
	// ---- 20050216 - 윤영대 수정
	ptThrowXY	=	pMyWorld->clISOMap.SearchMap( uiMapID, siX, siY, ON_MAP_SI_ITEM, pReqThrowItemMsg->uiItemID, pReqThrowItemMsg->uiQuantity );
	// ----
//	ptThrowXY	=	pMyWorld->clISOMap.SearchMap( uiMapID, siX, siY, ON_MAP_SI_ITEM, pReqThrowItemMsg->uiItemID );

	// 아이템을 심을 수 있는 좌표를 찾지 못했다.
	if( ptThrowXY.x == -1 && ptThrowXY.y == -1 )	goto	END;

	// 실제 그 부하가 있는지, 그 부하가 그 아이템을 가지고 있는지는 삭제해보면 안다.
	// 만약 삭제 성공했다면 아이템을 버리는데 성공한다.
	if( IFollower.DelItem( pReqThrowItemMsg->uiFollowerID, pReqThrowItemMsg->uiItemID,
							pReqThrowItemMsg->uiQuantity ) == ONLINEFOLLOWER_DELITEMRETURNTYPE_FALSE )	goto	END;	
	
	// 맵에 그 아이템을 설정한다.
	pMyWorld->clISOMap.SetItem( uiMapID, ptThrowXY.x, ptThrowXY.y, pReqThrowItemMsg->uiItemID, pReqThrowItemMsg->uiQuantity, FALSE );

	// DB에 저장한다.
	ZeroMemory( szDBArg, sizeof( szDBArg ) );
	sprintf( szDBArg, "%d, %d, %d, %d, %d, %d, %u, %u", stAccountInfo.siDBAccount, stAccountInfo.uiSlot, 
														pReqThrowItemMsg->uiFollowerID, uiMapID, ptThrowXY.x, ptThrowXY.y,
														pReqThrowItemMsg->uiItemID, pReqThrowItemMsg->uiQuantity );
	pMyWorld->clDatabase->writeDBQuery( SO_DB_INFOKIND_DROPITEM, LPVOID( szDBArg ), strlen( szDBArg) + 1 );			
	

	bSuccess	=	TRUE;

END:

	if( bSuccess == TRUE )
	{			
		
		
		
		// 주위의 플레이어에게 변경된 필드의 아이템 정보를 보낸다.
		
		OnChangeFieldItemMsg Msg;
		
		Msg.dwMsgType = ON_CHANGE_FIELDITEM;
		Msg.siX = ptThrowXY.x;
		Msg.siY = ptThrowXY.y;
		//Msg.uiItemID = pReqThrowItemMsg->uiItemID;
		//Msg.uiItemQuantity = pReqThrowItemMsg->uiQuantity;

		// 주의: 원래 맵에 있던 아이템과 합쳐졌을 수도 있으므로 새로 수량을 얻어온다.
		pMyWorld->clISOMap.GetItemID( uiMapID, ptThrowXY.x, ptThrowXY.y, &Msg.uiItemID, &Msg.uiItemQuantity );

		
		UI16 uiCharUniIDs[ON_STATUS_AREA_XSIZE * ON_STATUS_AREA_YSIZE];
		
		
		SI32 siSendCharNum;
		
		siSendCharNum	=	pMyWorld->clISOMap.GetNearSectorPlayerList(uiMapID, Msg.siX, Msg.siY, uiCharUniIDs, ON_MAP_CHARID_ALLPLAYER);
		
		
		
		SendFieldMsg( ON_CHANGE_FIELDITEM, SO_SFM_CUSTOM, LPVOID( siSendCharNum ), LPVOID( uiCharUniIDs ), 
			LPVOID( &Msg), NULL );

		// 아이템을 버리는데 성공했다.
		
		SendFieldMsg( ON_RESPONSE_THROWITEM, SO_SFM_ONLYME, LPVOID( ON_RET_OK ) );	
		
		
			/*		SendFieldMsg( ON_CHANGE_FIELDITEM, SO_SFM_MEANDNEARCHAR, LPVOID( ptThrowXY.x ), LPVOID( ptThrowXY.y ), 
		LPVOID( pReqThrowItemMsg->uiItemID ), LPVOID( pReqThrowItemMsg->uiQuantity ) );		*/
	}
	else
	{
		// 아이템을 버리는데 실패했다.
		SendFieldMsg( ON_RESPONSE_THROWITEM, SO_SFM_ONLYME, LPVOID( ON_RET_NO ) );
	}
}


//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	아이템을 줍는다.
//	수정 일자	:	2002-11-19 오전 10:54:18 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
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
	

	// 아이템을 인벤토리에 추가 시킨다.
	ItemData.uiID				=	pReqPickUpItemMsg->uiItemID;
	ItemData.uiQuantity			=	pReqPickUpItemMsg->uiQuantity;
	ItemData.siPosInInventory	=	SI16( pReqPickUpItemMsg->uiPosInInventory );
	IFollower.AddItem( pReqPickUpItemMsg->uiFollowerID, &ItemData );

	// 아이템의 정보를 업데이트 한다.
	uiItemQuantity				=	uiItemQuantity - pReqPickUpItemMsg->uiQuantity;
	
	// 우선 맵에서 아이템을 없앤다.
	pMyWorld->clISOMap.DelItem( this, uiMapID, pReqPickUpItemMsg->uiX, pReqPickUpItemMsg->uiY, FALSE );

	// 아직 아이템이 남아 있다면 남은 아이템을 다시 설정해준다.
	if( uiItemQuantity != 0 )
	{
		// 아이템을 일부만 가져갔다.
		// 맵에 떨어진 아이템의 정보를 업데이트한다.
		pMyWorld->clISOMap.SetItem( uiMapID, pReqPickUpItemMsg->uiX, pReqPickUpItemMsg->uiY, pReqPickUpItemMsg->uiItemID, uiItemQuantity, FALSE );
	}				

	// DB에 저장한다.
	ZeroMemory( szDBArg, sizeof( szDBArg ) );
	sprintf( szDBArg, "%d, %d, %d, %d, %d, %d, %d, %u, %u", stAccountInfo.siDBAccount, stAccountInfo.uiSlot, 
															pReqPickUpItemMsg->uiFollowerID, ItemData.siPosInInventory, 
															uiMapID, pReqPickUpItemMsg->uiX, pReqPickUpItemMsg->uiY,
															pReqPickUpItemMsg->uiItemID, pReqPickUpItemMsg->uiQuantity );
	pMyWorld->clDatabase->writeDBQuery( SO_DB_INFOKIND_PICKUPITEM, LPVOID( szDBArg ), strlen( szDBArg) + 1 );			

	// 아이템을 줍는데 성공했다.
	bSuccess	=	TRUE;

END:

	if(bSuccess == TRUE)
	{
		
		// 주위의 플레이어에게 변경된 필드의 아이템 정보를 보낸다.
		
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

		// 아이템을 줍는데 성공했다.
		
		SendFieldMsg( ON_RESPONSE_PICKUPITEM, SO_SFM_ONLYME, LPVOID( ON_RET_OK ) );



		
/*		SendFieldMsg( ON_CHANGE_FIELDITEM, SO_SFM_MEANDNEARCHAR, LPVOID( pReqPickUpItemMsg->uiX ), LPVOID( pReqPickUpItemMsg->uiY ), 
						LPVOID( uiItemID ), LPVOID( uiItemQuantity ) );		*/
	}
	else
	{
		// 아이템을 줍는데 실패했다.
		SendFieldMsg( ON_RESPONSE_PICKUPITEM, SO_SFM_ONLYME, LPVOID( ON_RET_NO ) );
	}
}





//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	요청 받았을때 아이템을 착용한다.
//	수정 일자	:	2002-07-13 오후 2:34:46 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
VOID	SOPlayer::RecvWearItem()
{
	OnReqWearItemMsg			*pWearItemMsg;	
	BOOL						bResult = FALSE;

	pWearItemMsg	=	(OnReqWearItemMsg*)	cMsg;

	// 거래중일때는 아이템을 착용할 수 없도록 한다.
	if( IsTrading() == TRUE )						goto	END;

	// 거래 중일 때는 아이템을 착용할 수 없도록 한다.
	bResult = WearItem( pWearItemMsg->uiFollowerID, pWearItemMsg->uiWearLoc, pWearItemMsg->uiItemID, pWearItemMsg->uiPosInventory );	

END:

	if( bResult == TRUE )
	{
		SendFieldMsg( ON_RESPONSE_WEARITEM, SO_SFM_ONLYME, LPVOID( ON_RET_WEARITEM_OK ) );

		if(pWearItemMsg->uiFollowerID == 0)			//	주인공캐릭이 복장을 바꿨을때.
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

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	아이템을 착용한다.
//	수정 일자	:	2002-07-13 오후 2:32:09 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
BOOL	SOPlayer::WearItem(UI08 uiFollowerID, UI08 uiWearLoc, UI16 uiItemID, SI16 siPosInInventory)
{
	DBIKWearItem				stWearItem;
	MyItemData					ItemData;
	SI08						siPutOnItemErrorCode;
	
	// 아이템을 착용할 수 있는지 알아온다.
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

	// DB에 저장한다.
	stWearItem.siDBAccount			=	stAccountInfo.siDBAccount;
	stWearItem.uiSlot				=	stAccountInfo.uiSlot;
	stWearItem.uiFollowerID			=	uiFollowerID;
	stWearItem.uiWearLoc			=	uiWearLoc;
	stWearItem.uiItemID				=	uiItemID;						// !!! ItemData.uiID를 사용해서는 안된다.
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
		stWearItem.siStartTime			=	atoi(szTemp);	//현재 시간을 저장한다.
		sprintf(szTemp,"%04d%02d%02d",siEndYear,siEndMon,siEndDay);
		stWearItem.siEndTime			=	atoi(szTemp);	//현재 시간을 저장한다.



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

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	요청을 받았을때 착용한 아이템을 해체한다.
//	수정 일자	:	2002-07-13 오후 2:39:24 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
VOID	SOPlayer::RecvStripItem()
{	
	OnReqStripItemMsg			*pWearItemMsg;	
	BOOL						bResult = FALSE;
	
	pWearItemMsg	=	(OnReqStripItemMsg*)	cMsg;

	// 거래중일때는 아이템을 벗을 수 없도록 한다.
	if( IsTrading() == TRUE )						goto	END;

	bResult	=	StripItem(pWearItemMsg->uiFollowerID,pWearItemMsg->uiWearLoc,pWearItemMsg->uiItemID,pWearItemMsg->uiPosInventory);
	
END:

	if( bResult == TRUE )
	{
		SendFieldMsg(ON_RESPONSE_STRIPITEM, SO_SFM_ONLYME, LPVOID(ON_RET_WEARITEM_OK));
		
		if(pWearItemMsg->uiFollowerID == 0)			//	주인공캐릭이 복장을 바꿨을때.
			SendFieldMsg(ON_CHANGE_CLOTHES, SO_SFM_ONLYNEARCHAR, LPVOID(this));
	}
	else
	{

		SendFieldMsg(ON_RESPONSE_STRIPITEM, SO_SFM_ONLYME, LPVOID(ON_RET_WEARITEM_NO));
	}
}


//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	착용한 아이템을 해체한다.
//	수정 일자	:	2002-07-13 오후 12:35:31 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
BOOL	SOPlayer::StripItem(UI08 uiFollowerID, UI08 uiWearLoc, UI16 uiItemID, SI16 siPosInventory)
{
	DBIKWearItem				stWearItem;
	MyItemData					ItemData;
	
	if( siPosInventory < 0 )
		writeInfoToFile("StripPosError.txt", "StripItem 1 :: [%u], [%u], [%d], [%d]", uiFollowerID, uiWearLoc, 
																						uiItemID, siPosInventory );
//		pMyWorld->clServer->WriteInfo("..\\StripPosError.txt", "StripItem 1 :: [%u], [%u], [%d], [%d]", uiFollowerID, uiWearLoc, 

	// 아이템을 벗을 수 있는지 알아온다.
	if(IFollower.CanTakeOffItem(uiFollowerID, uiWearLoc, uiItemID, siPosInventory) == FALSE	)				return	FALSE;
	
	// 아이템을 벗는다.
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
	
	// DB에 저장한다.
	stWearItem.siDBAccount			=	stAccountInfo.siDBAccount;
	stWearItem.uiSlot				=	stAccountInfo.uiSlot;
	stWearItem.uiFollowerID			=	uiFollowerID;
	stWearItem.uiWearLoc			=	uiWearLoc;
	stWearItem.uiItemID				=	0;								// !!! 반드시 0을 넣어주어야한다.
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
	
	// 아이템을 벗는다.
	IFollower.DisAppearItem(uiFollowerID, uiWearLoc, uiItemID, siPosInventory, &ItemData);	

	// DB에 저장한다.
	stWearItem.siDBAccount			=	stAccountInfo.siDBAccount;
	stWearItem.uiSlot				=	stAccountInfo.uiSlot;
	stWearItem.uiFollowerID			=	uiFollowerID;
	stWearItem.uiWearLoc			=	uiWearLoc;
	stWearItem.uiItemID				=	0;								// !!! 반드시 0을 넣어주어야한다.
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


	// 0. 현재 위치에서 발견할수 있는 아이템 목록을 모두 가져 온다 
	//----------------------------------------------------------------------------------
	while( pItem = clServer->clWorld->m_buriedItemParser.findItem (uiMapID) )
	{	
		
		foundItems[numFoundItem++] = pItem;		
	
		if( numFoundItem >= 20 ) break;
	}

	
	
	// 1. 발견한 아이템이 있을 경우 목록중에서 하나를 택하고 인벤토리에 넣어준다.
	//    아이템 추가에 성공했다면 메시지를 보내준다 
	//----------------------------------------------------------------------------------		
	if( numFoundItem > 0 )
	{
		int s = rand() % numFoundItem; // 목록중 선택한 아이템 


		
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

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	NPC를 선택했을때
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
VOID	SOPlayer::RecvReqQuestSelectNPC()
{
	OnReqQuestSelectNPC*			lpOnReqQuestSelectNPC;
	SI16							TempSlot;
	SI16							TempQuest;

	lpOnReqQuestSelectNPC = (OnReqQuestSelectNPC*)cMsg;

	stQuestInfo.m_QuestData_BookCount = lpOnReqQuestSelectNPC->siBookCount;

	// 이 NPC에 관련된 Quest를 하고 있으면
	TempSlot = Quest_FindPlayQuestNPC(lpOnReqQuestSelectNPC->siNPCCode);
	Quest_AddQuestObject_Npc(lpOnReqQuestSelectNPC->siNPCCode);

	if(TempSlot != -1)
	{
		// 있으면, 퀘스트를 완료할수 있는지 검사
		if(Quest_CheckCompleteQuest(TempSlot))
		{
			if(stQuestInfo.m_sPlayingQuestHeader[TempSlot]->m_siQuestEvent == QUEST_EVENT_PLAY)
			{
				//임시 코드.-_-; 일단 막아보자.
				if(pMyWorld->m_bQuestEventUpdate == TRUE) 
				{
					SendFieldMsg(ON_RESPONSE_QUEST_SELECTNPC, SO_SFM_ONLYME, LPVOID(ON_RET_QUEST_SELECTNPC_PLAYINGQUEST), LPVOID(stQuestInfo.m_sPlayingQuest[TempSlot].m_QuestCode));
				}
				else
					Quest_CheckEventItem(TempSlot);
			}
			else
				Quest_EndQuest(TempSlot);
			// 완료일때, End
			
		}
		else
		{
			// 아직 미 완료일때, Playing
			SendFieldMsg(ON_RESPONSE_QUEST_SELECTNPC, SO_SFM_ONLYME, LPVOID(ON_RET_QUEST_SELECTNPC_PLAYINGQUEST), LPVOID(stQuestInfo.m_sPlayingQuest[TempSlot].m_QuestCode));
		}
	}
	else
	{
		// 없으면, 이 NPC에 관련된 아직 하지 못한 Quest를 검사
		TempQuest = Quest_FindNextQuest(lpOnReqQuestSelectNPC->siNPCCode);
		if(TempQuest > 0)
		{
			// 하지 못한 Quest가 있을경우, 그 Quest를 할 조건(Need, Skip Test)이 돼는지 검사
			if(Quest_CheckPlayQuestAbility(TempQuest) == TRUE)
			{
				if(Quest_CheckSkipQuestAbility(TempQuest) == FALSE)
				{
					// 할 조건이 돼는 경우, Start
					SendFieldMsg(ON_RESPONSE_QUEST_SELECTNPC, SO_SFM_ONLYME, LPVOID(ON_RET_QUEST_SELECTNPC_STARTQUEST), LPVOID(TempQuest));
				}
				else
				{
					// Skip이 돼는 경우
					SendFieldMsg(ON_RESPONSE_QUEST_SELECTNPC, SO_SFM_ONLYME, LPVOID(ON_RET_QUEST_SELECTNPC_ENDAFTERQUEST), LPVOID(TempQuest));
				}
			}
			else
			{
				// 할 조건이 못돼는 경우, StartBefore
				SendFieldMsg(ON_RESPONSE_QUEST_SELECTNPC, SO_SFM_ONLYME, LPVOID(ON_RET_QUEST_SELECTNPC_STARTBEFOREQUEST), LPVOID(TempQuest));
			}
		}
		else
		{
			// 하지 못한 Quest가 없을경우, 마지막으로 이 NPC Quest를 찾는다.
			TempQuest = Quest_FindLastPlayQuest(lpOnReqQuestSelectNPC->siNPCCode);
			if(TempQuest > 0)
			{
				// 찾을경우, EndAfter
				SendFieldMsg(ON_RESPONSE_QUEST_SELECTNPC, SO_SFM_ONLYME, LPVOID(ON_RET_QUEST_SELECTNPC_ENDAFTERQUEST), LPVOID(TempQuest));
			}
			else
			{
				// 찾지 못할경우, Error
				SendFieldMsg(ON_RESPONSE_QUEST_SELECTNPC, SO_SFM_ONLYME, LPVOID(ON_RET_QUEST_SELECTNPC_ERROR));
			}
		}
	}
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	Quest를 수락했을때
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
VOID	SOPlayer::RecvReqQuestAcceptQuest()
{
	OnReqQuestAcceptQuest*			lpOnReqQuestAcceptQuest;
	SI16							TempSlot;
	DBIKQuestSetPlayQuestData		TempDBIKQuestSetPlayQuestData;

	lpOnReqQuestAcceptQuest	=	(OnReqQuestAcceptQuest*)cMsg;
	
	// 예전에 한 퀘스트인지 검사
	if(Quest_FindCompletedQuest(lpOnReqQuestAcceptQuest->m_QuestCode) == FALSE)
	{
		// 예전에 한적이 없을때, 현재 Quest를 할 능력이 돼는지 검사
		if(Quest_CheckPlayQuestAbility(lpOnReqQuestAcceptQuest->m_QuestCode))
		{
			// 능력이 돼면, Skip 조건이 돼는지 검사
			if(Quest_CheckSkipQuestAbility(lpOnReqQuestAcceptQuest->m_QuestCode) == FALSE)
			{
				// Skip이 안될때, 빈 Quest Slot을 찾는다.
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
				// Skip 가능시
				SendFieldMsg(ON_RESPONSE_QUEST_ACCEPTQUEST, SO_SFM_ONLYME, LPVOID(ON_RET_NO), LPVOID(lpOnReqQuestAcceptQuest->m_QuestCode));
			}
		}
		else
		{
			// 능력이 안돼면
			SendFieldMsg(ON_RESPONSE_QUEST_ACCEPTQUEST, SO_SFM_ONLYME, LPVOID(ON_RET_NO), LPVOID(lpOnReqQuestAcceptQuest->m_QuestCode));
		}
	}
	else
	{
		// 예전에 한 퀘스트 일때
		SendFieldMsg(ON_RESPONSE_QUEST_ACCEPTQUEST, SO_SFM_ONLYME, LPVOID(ON_RET_NO), LPVOID(lpOnReqQuestAcceptQuest->m_QuestCode));
	}
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	진행중인 Quest를 취소했을때
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
VOID	SOPlayer::RecvReqQuestCancelQuest()
{
	OnReqQuestCancelQuest*			lpOnReqQuestCancelQuest;
	SI16							TempSlot;
	DBIKQuestCancelQuest			TempDBIKQuestCancelQuest;

	lpOnReqQuestCancelQuest	=	(OnReqQuestCancelQuest*)cMsg;

	// 그 퀘스트가 현재 진행중인지 검사
	TempSlot = Quest_FindQuest(lpOnReqQuestCancelQuest->m_QuestCode);
	if(TempSlot != -1)
	{
		// 진행중일때 Quest 취소
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
		// 진행중이 아닐때
		SendFieldMsg(ON_RESPONSE_QUEST_CANCELQUEST, SO_SFM_ONLYME, LPVOID(ON_RET_NO), LPVOID(lpOnReqQuestCancelQuest->m_QuestCode));
	}
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	진행중인 Quest List 요청
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
VOID	SOPlayer::RecvReqQuestGetPlayQuest()
{
	// Quest Data 보냄
	SendFieldMsg(ON_RESPONSE_QUEST_GETPLAYQUEST, SO_SFM_ONLYME, LPVOID(NULL), LPVOID(ON_QUSETDATA_MAXPLAYINGQUEST), LPVOID(ON_QUSETDATA_MAXCOMLETEDQUEST), LPVOID(stQuestInfo.m_sPlayingQuest), LPVOID(stQuestInfo.m_siCompletedQuest));
}


//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	이 NPC에 관련된어 Play하고 있는 Quest를 찾는다.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
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

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	이 Quest가 완료될수 있는지 검사
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
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

	//랜덤 하게 돌려서 어떤선물이 나오면.

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
	SI16 siEventItemID;		 //DB의 테이블에 있는 아이디.
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

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	Quest를 종료한다.(DB에 저장하고, 보상을 받으며, Client로 메세지 전송까지 한다.)
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
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

	// DB에 저장한다.	

	//if(stQuestInfo.m_sPlayingQuestHeader[QuestSlot]->siQuestCode != DOKDO_QUEST_CODE1 && 
	//  stQuestInfo.m_sPlayingQuestHeader[QuestSlot]->siQuestCode != DOKDO_QUEST_CODE2)
	if(stQuestInfo.m_sPlayingQuestHeader[QuestSlot]->m_siQuestEvent != QUEST_EVENT_PLAY)
	{	
		TempDBIKQuestCompleteQuest.DBAccount  = stAccountInfo.siDBAccount;
		TempDBIKQuestCompleteQuest.uiSlot     = stAccountInfo.uiSlot;
		TempDBIKQuestCompleteQuest.QuestIndex = QuestSlot;
		pMyWorld->clDatabase->writeDBQuery(SO_DB_INFOKIND_QUEST_COMPLETEQUEST, LPVOID(&TempDBIKQuestCompleteQuest), sizeof(TempDBIKQuestCompleteQuest));
	}

	// 보상을 받는다.
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
							// 같은 아이템이 있는지 검사
							lpMyItemData = IFollower.GetItemByID(TempCount1, stQuestInfo.m_sPlayingQuestHeader[QuestSlot]->sQuestPay[TempCount].m_QuestPayCode);
							if(lpMyItemData)
							{
								// 있으면 그 아이템에 추가
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
								// 없으면 빈칸이 있는지 검사
								TempItemSlot = IFollower.FindEmptyItemBuffer(TempCount1);
								if((TempItemSlot != -1) && (TempItemSlot != 0xffff))
								{
									// 빈칸이 있으면 추가
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
						// 같은 아이템이 있는지 검사
						lpMyItemData = IFollower.GetItemByID(TempCount1, stQuestInfo.m_sPlayingQuestHeader[QuestSlot]->sQuestPay[TempCount].m_QuestPayCode);
						if(lpMyItemData)
						{
							// 있으면 그 아이템에 추가
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
							// 없으면 빈칸이 있는지 검사
							TempItemSlot = IFollower.FindEmptyItemBuffer(TempCount1);
							if((TempItemSlot != -1) && (TempItemSlot != 0xffff))
							{
								// 빈칸이 있으면 추가
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

	// 만약 Quest에 필요한 Item이 있었으면, 그 아이템을 삭제한다.
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

	// Client에 알린다.
	SendFieldMsg(ON_RESPONSE_QUEST_SELECTNPC, SO_SFM_ONLYME, 
		LPVOID(ON_RET_QUEST_SELECTNPC_ENDQUEST), 
		LPVOID(stQuestInfo.m_sPlayingQuest[QuestSlot].m_QuestCode), LPVOID(TempPayData), LPVOID(TempDelItemCount), LPVOID(TempMercenaryItemData), LPVOID(&TempDeleteMoney));

/*	// 해당 유저가 독도 퀘스트가 완료하였다는 것을 모든 유저에게 알려준다.
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
		// Data를 초기화 한다.
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

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	이 NPC에게서 진행할수 있는 다음 Quest를 찾는다.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
SI16	SOPlayer::Quest_FindNextQuest(SI16 NPCCode)
{
	SI32				TempCount;
	SI32				TempSmallCode;
	SI32				TempMinCode;
	SI32				TempSlot;

	if(NPCCode >= pMyWorld->clQuestParser.m_NPCSortData_MaxCount) return 0;

	// 진행한 최소 코드를 찾는다.
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

	// 앞으로 할수 있는 Quest를 찾는다.
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

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	Quest를 Start할수 있는지 검사
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
BOOL	SOPlayer::Quest_CheckPlayQuestAbility(SI16 QuestCode)
{
	SI32				TempCount;
	SI32				TempCount1;
	SQuestHeader*		lpSQuestHeader;
	SI16				TempSI16;
	BOOL				fEmptyBuffer;

	// 현재 진행중인지 검사
	for(TempCount = 0; TempCount < ON_QUSETDATA_MAXPLAYINGQUEST; TempCount++)
	{
		if(stQuestInfo.m_sPlayingQuest[TempCount].m_QuestCode == QuestCode)
		{
			return FALSE;
		}
	}

	// 빈 Slot이 있는지 검사
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

	// 조건이 맞는지 검사
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

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	Quest를 Skip할수 있는지 검사
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
BOOL	SOPlayer::Quest_CheckSkipQuestAbility(SI16 QuestCode)
{
	SI32				TempCount;
	SI32				TempCount1;
	SQuestHeader*		lpSQuestHeader;
	SI16				TempSI16;

	// 조건이 맞는지 검사
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

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	이 NPC에게서 마지막으로 Play한 Quest검사
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
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

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	예전에 한적이 있는 Quest인지 검사
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
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

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	빈 Quest Slot을 찾음
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
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

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	진행중인 Quest를 찾음
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
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

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	모든 Quest 초기화, 깔끔하게
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
VOID	SOPlayer::Quest_AllClear(void)
{
	ZeroMemory(stQuestInfo.m_siCompletedQuest,       sizeof(SI16) * ON_QUSETDATA_MAXCOMLETEDQUEST);
	ZeroMemory(stQuestInfo.m_siCompletedQuestHeader, sizeof(SQuestHeader*) * ON_QUSETDATA_MAXCOMLETEDQUEST);
	
	ZeroMemory(stQuestInfo.m_sPlayingQuest,       sizeof(SPlayingQuest) * ON_QUSETDATA_MAXPLAYINGQUEST);
	ZeroMemory(stQuestInfo.m_sPlayingQuestHeader, sizeof(SQuestHeader*) * ON_QUSETDATA_MAXPLAYINGQUEST);

	SendFieldMsg(ON_RESPONSE_QUEST_ALLCLEARQUEST, SO_SFM_ONLYME);
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	죽인 몬스터 Data
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
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
 
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	자신이 한일
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
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

