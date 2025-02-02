#include "SOMain.h"
#include "SOWorld.h"
#include "SOPlayer.h"
#include "ParametaBox.h"


//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
// Ы溯檜橫蒂 檜翕衛鑑棻.
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收


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

	if(uiBoardShipAccount != 0)																						goto	END;		// ⑷營 寡縑 鱉蝓腎橫 氈朝 鼻鷓塭賊 檜翕ж雖 跤и棻.
	if(GetBattle() != NULL)																							goto	END;		// ⑷營 瞪癱 渠晦醞檜賊 檜翕ж雖 跤и棻.

	if(pMyWorld->clISOMap.MoveChar( this, pMyCharMoveMsg->uiX, pMyCharMoveMsg->uiY, pMyCharMoveMsg->bWarUnit,uiMoveableTileInfo) == FALSE)	goto	END;		// 議葛攪蒂 檜翕 衛鑑棻.

	// 檜翕縑 撩奢ж艘棻.
	bSuccess	=	TRUE;

END:

	if(bSuccess == FALSE)
	{
		// 斜 嬪纂縑朝 議葛攪蒂 撲薑й 熱 橈棻.
		// 錳楚 嬪纂煎 檜翕ж塭堅 詭衛雖蒂 爾頂遽棻.
		SendFieldMsg( ON_RESPONSE_POSITION, SO_SFM_ONLYME, LPVOID( &uiAccount ), LPVOID( 1 ) );
	}	
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	錶Щ и棻朝 詭衛雖蒂 嫡懊棻.			
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
BOOL	SOPlayer::RecvWarp()
{
	BOOL			bSuccess		=	FALSE;
	OnReqWarpMsg	*pReqWarpMsg;
	strWarpInfo		*pstWarpInfo;

	pReqWarpMsg	=	(OnReqWarpMsg*)cMsg;

	pstWarpInfo	=	pMyWorld->m_clWarpParser.GetWarpInfo( pReqWarpMsg->uiWarpCode );

	// 錶Щй 議葛攪曖 嬴檜蛤陛 濠褐曖 嬴檜蛤檣 唳辦
	bSuccess	=	Warp( pstWarpInfo->uiMapIndex, pstWarpInfo->uiX, pstWarpInfo->uiY );

	if( bSuccess )		SendFieldMsg( ON_RESPONSE_WARP, SO_SFM_ONLYME, LPVOID(ON_RET_OK) );
	else				SendFieldMsg( ON_RESPONSE_WARP, SO_SFM_ONLYME, LPVOID(ON_RET_NO) );

	return	bSuccess;
}


//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	錶Щ蒂 ж堅 輿嬪曖 等檜顫蒂 嫡朝棻.
//				檣濠煎 菟橫螃朝 siX, siY朝 晦遽檜 腎朝 嬪纂檜雖
//				奩萄衛 斜 嬪纂煎 錶Щ陛 腎朝 匙擎 嬴棲棻.
//				斜 嬪纂陛 嶸�褲� 嬪纂陛 嬴棍 唳辦縑朝 斜 輿嬪曖 嬪纂煎 錶Щ 腆 熱紫 氈棻.
//				錶Щ縑 撩奢 ц擊 唳辦縑虜 檜 л熱頂縑憮 п渡 Ы溯檜橫縑啪 錶Щ 詭衛雖蒂 爾頂堅
//				錶Щ縑 褒ぬ ц擊 唳辦縑朝 檜 л熱蒂 ��轎 ж朝 睡碟縑憮 詭衛雖蒂 爾頂遽棻.
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
BOOL	SOPlayer::Warp(UI16 uimapid, SI32 six, SI32 siy)
{
	UI16							uiCharFItemInfo[2];			// 0廓擎 議葛攪 熱, 1廓擎 嬴檜蠱 熱,
	UI16							uiCharUniIDs[ON_STATUS_AREA_XSIZE * ON_STATUS_AREA_YSIZE];	
	OnlineReceiveFieldItemInfo		FieldItemInfo[ON_STATUS_AREA_XSIZE * ON_STATUS_AREA_YSIZE];
	BOOL							bWarpSuccess = FALSE;
	UI16							uiPrevMapID;
	SI32							siPrevX, siPrevY;	
	POINT							ptWarpXY;

	// ⑷營 奩萄衛 в萄 鼻縑 氈橫撿 и棻.
	if( CHECKSTATUS( siStatus, ON_PLAYER_STATUS_INFIELD ) == FALSE )	return	FALSE;
	//if(siStatus	!= ON_PLAYER_STATUS_INFIELD)				return FALSE;

	// 錶Щй 裘曖 奩萄衛 嶸�褲媥� и棻.
	if(pMyWorld->clISOMap.IsValidMapID(uimapid) == FALSE)	return	FALSE;

	// 錶Щй 夠擊 瓊朝棻.
	ptWarpXY	=	pMyWorld->clISOMap.SearchMap(uimapid, six, siy, ON_MAP_SI_CHAR, uiMoveableTileInfo);

	// 錶Щй 夠縑 錶Щй 濠葬蒂 瓊雖 跤п憮 褒ぬц棻.				
	if((ptWarpXY.x == -1) && (ptWarpXY.y == -1))	return FALSE;

	// ⑷營 裘曖 ID蒂 盪濰и棻.
	uiPrevMapID		=	uiMapID;
	siPrevX			=	siX;
	siPrevY			=	siY;	
	
	// 裘曖 ⑷營 嬪纂縑憮 頂 ID蒂 雖遴棻.
	if( DelIDOnMap( 4 ) == FALSE)
		writeInfoToFile("DelIDOnMap Failed.txt", "4 [%u]", uiAccount);
//		pMyWorld->clServer->WriteInfo("..\\DelIDOnMap Failed.txt", "4 [%u]", uiAccount);

	// 辦摹 裘擊 滲唳и棻.
	// 滲唳ж溥朝 裘檜 橈戲賊 FALSE蒂 葬欐и棻.
	// 餌辨濠朝 ChangeMap擊 奩萄衛 ��轎п輿橫撿 и棻.
	if(ChangeMap(uimapid, SI16(ptWarpXY.x), SI16(ptWarpXY.y)) == TRUE)			
	{		
		// 裘曖 億煎遴 嬪纂縑 頂 ID蒂 撲薑и棻.
		if(SetIDOnMap( 5 ) == FALSE)
			writeInfoToFile("SetIDOnMap Failed.txt", "6");		
//			pMyWorld->clServer->WriteInfo("..\\SetIDOnMap Failed.txt", "6");		
		
		// 頂 輿嬪縑 氈朝 議葛攪曖 ID諦 嬴檜蠱擊 橢橫螞棻.
		uiCharFItemInfo[0]		=	pMyWorld->clISOMap.GetNearSectorPlayerList(uiMapID, siX, siY, uiCharUniIDs, ON_MAP_CHARID_FIELD);
		uiCharFItemInfo[1]		=	pMyWorld->clISOMap.GetNearSectorItemList(uiMapID, siX, siY, FieldItemInfo);				

		// 辦摹 釭縑啪 頂 輿嬪曖 議葛攪諦 嬴檜蠱曖 薑爾蒂 爾頂遽棻.
		SendFieldMsg(ON_RESPONSE_WARP, SO_SFM_ONLYME, 
						LPVOID(ON_RET_OK),
						LPVOID(uiCharFItemInfo), 
						LPVOID(uiCharUniIDs), 
						LPVOID(FieldItemInfo));		

		bWarpSuccess	=	TRUE;	
	}

	if(bWarpSuccess == FALSE)
	{
		// 億煎遴 雖薄戲煎 錶Щж朝等 褒ぬц棻賊
		// 錳楚 濠葬煎 棻衛 錶Щи棻.
		BaseChar::Warp(uiPrevMapID, siPrevX, siPrevY);
	}

	return bWarpSuccess;
}

//----------------------------------------------------------------------------------------------------------------
//	撲貲	:	か薑 Ы溯檜橫曖 薑爾蒂 蹂羶и棻
//----------------------------------------------------------------------------------------------------------------
VOID	SOPlayer::RecvPlayerInfo()
{
	OnReqPlayerInfoMsg		*pReqPlayerInfoMsg;
	UI16					*puiCharsID;
	UI16					uiCharIDs[20];					// и廓縑 20貲 檜鼻擎 爾頂輿雖 彊朝棻.
	UI16					uiCharNum;
	SI32					i;

	// 薑爾蒂 爾頂遽棻.
	pReqPlayerInfoMsg		=	(OnReqPlayerInfoMsg*)cMsg;	
	puiCharsID				=	(UI16*)((CHAR*)pReqPlayerInfoMsg + sizeof(OnReqPlayerInfoMsg));
	
	ZeroMemory(uiCharIDs, sizeof(uiCharIDs));
	
	// ⑷營 嶸�褲� 議葛攪虜 蹺轎и棻.
	for(uiCharNum = 0, i = 0; i < pReqPlayerInfoMsg->uiCharNum; i++)
	{
		if(IsValidID(puiCharsID[i]) == TRUE)
		{			
			uiCharIDs[uiCharNum]	=	puiCharsID[i];
			uiCharNum++;

			// ⑷營朝 20貲戲煎 薯иж棻.
			if(uiCharNum >= 20)
				break;
		}
	}

	// 詭衛雖蒂 爾頂遽棻.
	SendFieldMsg(ON_RESPONSE_PLAYER_INFO, SO_SFM_ONLYME, LPVOID(uiCharNum), LPVOID(uiCharIDs), LPVOID(uiGuildID));	
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	議葛攪曖 鼻撮 薑爾蒂 蹂羶и棻朝 詭衛雖蒂 嫡懊棻.
//	熱薑 橾濠	:	2002-08-15 螃�� 3:05:51 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
VOID	SOPlayer::RecvCharDetailInfo()
{
	OnReqCharDetailInfoMsg		*pReqCharDetailInfoMsg;
	SOPlayer					*pPlayer;

	pReqCharDetailInfoMsg		=	(OnReqCharDetailInfoMsg*)cMsg;

	if(ISPLAYER(pReqCharDetailInfoMsg->uiAccount) == TRUE)
	{
		if((pPlayer = pMyWorld->GetPlayer(pReqCharDetailInfoMsg->uiAccount)) != NULL)
		{
			// Ы溯檜橫陛 嶸�褲炴�.
			// 議葛攪曖 鼻撮 薑爾蒂 爾頂遽棻.
			SendFieldMsg(ON_RESPONSE_CHAR_DETAILINFO, SO_SFM_ONLYME, LPVOID(ON_RET_OK), LPVOID(pPlayer));
		}
		else
		{
			// Ы溯檜橫陛 嶸�褲狫� 彊棻.
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

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	葆擊縑 菟橫除棻朝 詭衛雖蒂 嫡懊棻.
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
VOID SOPlayer::RecvEnterVillage()
{
	OnReqGotoVillageMsg		*pReqGotoVillageMsg;	
	DWORD					dwResponse = ON_RET_NO;
	UI32					uiVillageGuildID;
	cltGuild*				pclGuild;

	pclGuild = pMyWorld->clIGuild.GetGuildByID(uiGuildID);
	pReqGotoVillageMsg	=	(OnReqGotoVillageMsg*)cMsg;
		
	// ⑷營 頂陛 瞪癱 渠晦醞檜剪釭 瞪癱醞縑 氈戲賊 菟橫陛雖 跤и棻.
	if( GetBattle() != NULL )			goto	END;	

	// ⑷營 頂陛 寡縑 鱉蝓醞檜賊 菟橫陛雖 跤и棻.
	if( uiBoardShipAccount != 0)		goto	END;

	// 辦摹 嶸�褲� 葆擊檣雖 憲嬴螞棻.
	stAccountInfo.pVillage	=	pMyWorld->GetVillageByCode(pReqGotoVillageMsg->uiVillageCode);				// 葆擊擊 瓊朝棻.
	if(stAccountInfo.pVillage == NULL)	goto	END;

	if( (stAccountInfo.pVillage->uiMapID != uiMapID) ||
		( (abs(stAccountInfo.pVillage->siVillageX - siX) > 10) && (abs(stAccountInfo.pVillage->siVillageY - siY) > 10) ) )
	{
		writeInfoToFile( "WarpHacking.txt", "DBAccount(%d)  szID(%s)  Player MapIndex(%d) X(%d) Y(%d) Try to Enter village(%s)",
						stAccountInfo.siDBAccount, szID, uiMapID, siX, siY, stAccountInfo.pVillage->GetVillageName() );
		goto	END;
	}

	// 葆擊曖 Guild ID蒂 掘и棻.
	uiVillageGuildID		=	stAccountInfo.pVillage->GetGuildNID();

	// 瞳 葆擊曖 鼻欽檜賊 菟橫陞 熱 橈棻.
	if((uiGuildID != 0) && (uiVillageGuildID != 0) && (siClassInGuild != pMyWorld->clIGuildClass.GetJWJCode()))
	{
		if(pMyWorld->clIGuild.IsEnemyRelationWar(uiGuildID,uiVillageGuildID) == TRUE || pMyWorld->clIGuild.IsEnemyGuildBoth(uiGuildID, uiVillageGuildID) == TRUE)
		{
			dwResponse	=	ON_RET_ENEMY_GUILD_VILLAGE;
			goto	END;
		}
	}

	
	// 裘縑憮 頂 ID蒂 雖遴棻.
	// 奩萄衛 葆擊縑 菟橫陛晦瞪縑 п輿橫撿 и棻.
	if(DelIDOnMap( 6 ) == FALSE)
		writeInfoToFile("DelIDOnMap Failed.txt", "5 [%u] [%d] [%d] [%d]", uiAccount, uiMapID, siX, siY);
//		pMyWorld->clServer->WriteInfo("..\\DelIDOnMap Failed.txt", "5 [%u] [%d] [%d] [%d]", uiAccount, uiMapID, siX, siY);

	// 葆擊縑 菟橫除棻.
	EnterVillage();	

	dwResponse	=	ON_RET_OK;
END:
	// 葆擊縑 菟橫陞 熱 橈棻.
	switch(dwResponse)
	{
	case ON_RET_OK:
		// 輿嬪 Ы溯檜橫縑啪 辦摹 憲溥遽棻.
		SendFieldMsg(ON_RESPONSE_ENTER_VILLAGE, SO_SFM_ONLYNEARCHAR, LPVOID(dwResponse));	
			
		// 斜葬堅 釭縑啪紫 憲溥遽棻.		
		SendFieldMsg(ON_RESPONSE_ENTER_VILLAGE, SO_SFM_ONLYME, LPVOID(dwResponse), LPVOID(stAccountInfo.pVillage));
		break;
	case ON_RET_NO:
	case ON_RET_ENEMY_GUILD_VILLAGE:		
		SendFieldMsg(ON_RESPONSE_ENTER_VILLAGE, SO_SFM_ONLYME, LPVOID(dwResponse));				
		break;
	}
	
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	葆擊縑 菟橫除棻
//	熱薑 橾濠	:	2002-09-28 螃瞪 11:35:00 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
BOOL	SOPlayer::EnterVillage( BOOL bIsVillageField )
{
	if(stAccountInfo.pVillage->EnterVillage( this ) == TRUE)
	{
		// 葆擊縑 菟橫除 鼻鷓煎 siStatus蒂 撲薑и棻.
		//siStatus							=	ON_PLAYER_STATUS_INVILLAGE;		
		if( bIsVillageField == TRUE)
		{
			// 盪濕剪葬檜棻.
			ADDSTATUS( siStatus, ON_PLAYER_STATUS_INVILLAGE );
		}
		else
		{
			// 濠濕剪葬陛 嬴棲棻.
			SETSTATUS( siStatus, ON_PLAYER_STATUS_INVILLAGE );
		}			
		
		stAccountInfo.uiStatusInVillage		=	INVILLAGE_STATUS_NONE;	

		// 檜瞪縑 盪濰и雖 5碟檜 雖陬棻賊 DB縑 撲薑п遽棻.
		if( (int)(pMyWorld->dwtimeGetTime - m_latestSavedPosTime) > 300000 ) savePosToDB();		

		// 掖羲璽擊 殘朝棻.
		CloseTrade();

		// 顫 嶸盪縑啪 だじ 褐羶醞檜剪釭 
		// 棻艇 嶸盪縑 だじ 褐羶擊 嫡懊棻賊 檜蒂 鏃模и棻.
		CloseRequestingJoinParty();

		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	葆擊縑憮 集陪棻.
//	熱薑 橾濠	:	2002-09-28 螃瞪 11:38:37 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
VOID SOPlayer::LeaveVillage( BOOL bIsVillageField )
{	
	// в萄縑 氈朝 鼻鷓煎 siStatus蒂 撲薑и棻.
	//siStatus							=	ON_PLAYER_STATUS_INFIELD;	
	if( bIsVillageField == TRUE)
	{
		// 盪濕剪葬檜棻.
		if( CHECKSTATUS( siStatus, ON_PLAYER_STATUS_INVILLAGE ) == TRUE)
		{
			REMOVESTATUS( siStatus, ON_PLAYER_STATUS_INVILLAGE );
		}
	}
	else
	{
		// 濠濕剪葬陛 嬴棲棻.
		SETSTATUS( siStatus, ON_PLAYER_STATUS_INFIELD );
	}			
	
	stAccountInfo.pVillage				=	NULL;	
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	嬴檜蠱擊 幗萼棻朝 詭衛雖蒂 嫡懊棻.
//	熱薑 橾濠	:	2002-11-19 螃瞪 9:21:47 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
VOID	SOPlayer::RecvThrowItem()
{
	OnReqThrowItemMsg		*pReqThrowItemMsg;
	BOOL					bSuccess = FALSE;
	POINT					ptThrowXY;
	CHAR					szDBArg[ 1024 ];

	pReqThrowItemMsg	=	(OnReqThrowItemMsg*)cMsg;

	//pReqThrowItemMsg->

	// 嬴檜蠱擊 幗萵 熱 氈朝 濠葬陛 氈朝雖 っ欽и棻.
	// ---- 20050216 - 彌艙渠 熱薑
	ptThrowXY	=	pMyWorld->clISOMap.SearchMap( uiMapID, siX, siY, ON_MAP_SI_ITEM, pReqThrowItemMsg->uiItemID, pReqThrowItemMsg->uiQuantity );
	// ----
//	ptThrowXY	=	pMyWorld->clISOMap.SearchMap( uiMapID, siX, siY, ON_MAP_SI_ITEM, pReqThrowItemMsg->uiItemID );

	// 嬴檜蠱擊 褕擊 熱 氈朝 謝ル蒂 瓊雖 跤ц棻.
	if( ptThrowXY.x == -1 && ptThrowXY.y == -1 )	goto	END;

	// 褒薯 斜 睡ж陛 氈朝雖, 斜 睡ж陛 斜 嬴檜蠱擊 陛雖堅 氈朝雖朝 餉薯п爾賊 寰棻.
	// 虜擒 餉薯 撩奢ц棻賊 嬴檜蠱擊 幗葬朝等 撩奢и棻.
	if( IFollower.DelItem( pReqThrowItemMsg->uiFollowerID, pReqThrowItemMsg->uiItemID,
							pReqThrowItemMsg->uiQuantity ) == ONLINEFOLLOWER_DELITEMRETURNTYPE_FALSE )	goto	END;	
	
	// 裘縑 斜 嬴檜蠱擊 撲薑и棻.
	pMyWorld->clISOMap.SetItem( uiMapID, ptThrowXY.x, ptThrowXY.y, pReqThrowItemMsg->uiItemID, pReqThrowItemMsg->uiQuantity, FALSE );

	// DB縑 盪濰и棻.
	ZeroMemory( szDBArg, sizeof( szDBArg ) );
	sprintf( szDBArg, "%d, %d, %d, %d, %d, %d, %u, %u", stAccountInfo.siDBAccount, stAccountInfo.uiSlot, 
														pReqThrowItemMsg->uiFollowerID, uiMapID, ptThrowXY.x, ptThrowXY.y,
														pReqThrowItemMsg->uiItemID, pReqThrowItemMsg->uiQuantity );
	pMyWorld->clDatabase->writeDBQuery( SO_DB_INFOKIND_DROPITEM, LPVOID( szDBArg ), strlen( szDBArg) + 1 );			
	

	bSuccess	=	TRUE;

END:

	if( bSuccess == TRUE )
	{			
		
		
		
		// 輿嬪曖 Ы溯檜橫縑啪 滲唳脹 в萄曖 嬴檜蠱 薑爾蒂 爾魚棻.
		
		OnChangeFieldItemMsg Msg;
		
		Msg.dwMsgType = ON_CHANGE_FIELDITEM;
		Msg.siX = ptThrowXY.x;
		Msg.siY = ptThrowXY.y;
		//Msg.uiItemID = pReqThrowItemMsg->uiItemID;
		//Msg.uiItemQuantity = pReqThrowItemMsg->uiQuantity;

		// 輿曖: 錳楚 裘縑 氈湍 嬴檜蠱婁 м藥螺擊 熱紫 氈戲嘎煎 億煎 熱榆擊 橢橫螞棻.
		pMyWorld->clISOMap.GetItemID( uiMapID, ptThrowXY.x, ptThrowXY.y, &Msg.uiItemID, &Msg.uiItemQuantity );

		
		UI16 uiCharUniIDs[ON_STATUS_AREA_XSIZE * ON_STATUS_AREA_YSIZE];
		
		
		SI32 siSendCharNum;
		
		siSendCharNum	=	pMyWorld->clISOMap.GetNearSectorPlayerList(uiMapID, Msg.siX, Msg.siY, uiCharUniIDs, ON_MAP_CHARID_ALLPLAYER);
		
		
		
		SendFieldMsg( ON_CHANGE_FIELDITEM, SO_SFM_CUSTOM, LPVOID( siSendCharNum ), LPVOID( uiCharUniIDs ), 
			LPVOID( &Msg), NULL );

		// 嬴檜蠱擊 幗葬朝等 撩奢ц棻.
		
		SendFieldMsg( ON_RESPONSE_THROWITEM, SO_SFM_ONLYME, LPVOID( ON_RET_OK ) );	
		
		
			/*		SendFieldMsg( ON_CHANGE_FIELDITEM, SO_SFM_MEANDNEARCHAR, LPVOID( ptThrowXY.x ), LPVOID( ptThrowXY.y ), 
		LPVOID( pReqThrowItemMsg->uiItemID ), LPVOID( pReqThrowItemMsg->uiQuantity ) );		*/
	}
	else
	{
		// 嬴檜蠱擊 幗葬朝等 褒ぬц棻.
		SendFieldMsg( ON_RESPONSE_THROWITEM, SO_SFM_ONLYME, LPVOID( ON_RET_NO ) );
	}
}


//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	嬴檜蠱擊 鄹朝棻.
//	熱薑 橾濠	:	2002-11-19 螃瞪 10:54:18 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
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
	

	// 嬴檜蠱擊 檣漸饜葬縑 蹺陛 衛鑑棻.
	ItemData.uiID				=	pReqPickUpItemMsg->uiItemID;
	ItemData.uiQuantity			=	pReqPickUpItemMsg->uiQuantity;
	ItemData.siPosInInventory	=	SI16( pReqPickUpItemMsg->uiPosInInventory );
	IFollower.AddItem( pReqPickUpItemMsg->uiFollowerID, &ItemData );

	// 嬴檜蠱曖 薑爾蒂 機等檜お и棻.
	uiItemQuantity				=	uiItemQuantity - pReqPickUpItemMsg->uiQuantity;
	
	// 辦摹 裘縑憮 嬴檜蠱擊 橈撻棻.
	pMyWorld->clISOMap.DelItem( this, uiMapID, pReqPickUpItemMsg->uiX, pReqPickUpItemMsg->uiY, FALSE );

	// 嬴霜 嬴檜蠱檜 陴嬴 氈棻賊 陴擎 嬴檜蠱擊 棻衛 撲薑п遽棻.
	if( uiItemQuantity != 0 )
	{
		// 嬴檜蠱擊 橾睡虜 陛螳鬲棻.
		// 裘縑 雲橫霞 嬴檜蠱曖 薑爾蒂 機等檜おи棻.
		pMyWorld->clISOMap.SetItem( uiMapID, pReqPickUpItemMsg->uiX, pReqPickUpItemMsg->uiY, pReqPickUpItemMsg->uiItemID, uiItemQuantity, FALSE );
	}				

	// DB縑 盪濰и棻.
	ZeroMemory( szDBArg, sizeof( szDBArg ) );
	sprintf( szDBArg, "%d, %d, %d, %d, %d, %d, %d, %u, %u", stAccountInfo.siDBAccount, stAccountInfo.uiSlot, 
															pReqPickUpItemMsg->uiFollowerID, ItemData.siPosInInventory, 
															uiMapID, pReqPickUpItemMsg->uiX, pReqPickUpItemMsg->uiY,
															pReqPickUpItemMsg->uiItemID, pReqPickUpItemMsg->uiQuantity );
	pMyWorld->clDatabase->writeDBQuery( SO_DB_INFOKIND_PICKUPITEM, LPVOID( szDBArg ), strlen( szDBArg) + 1 );			

	// 嬴檜蠱擊 鄹朝等 撩奢ц棻.
	bSuccess	=	TRUE;

END:

	if(bSuccess == TRUE)
	{
		
		// 輿嬪曖 Ы溯檜橫縑啪 滲唳脹 в萄曖 嬴檜蠱 薑爾蒂 爾魚棻.
		
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

		// 嬴檜蠱擊 鄹朝等 撩奢ц棻.
		
		SendFieldMsg( ON_RESPONSE_PICKUPITEM, SO_SFM_ONLYME, LPVOID( ON_RET_OK ) );



		
/*		SendFieldMsg( ON_CHANGE_FIELDITEM, SO_SFM_MEANDNEARCHAR, LPVOID( pReqPickUpItemMsg->uiX ), LPVOID( pReqPickUpItemMsg->uiY ), 
						LPVOID( uiItemID ), LPVOID( uiItemQuantity ) );		*/
	}
	else
	{
		// 嬴檜蠱擊 鄹朝等 褒ぬц棻.
		SendFieldMsg( ON_RESPONSE_PICKUPITEM, SO_SFM_ONLYME, LPVOID( ON_RET_NO ) );
	}
}





//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	蹂羶 嫡懊擊陽 嬴檜蠱擊 雜辨и棻.
//	熱薑 橾濠	:	2002-07-13 螃�� 2:34:46 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
VOID	SOPlayer::RecvWearItem()
{
	OnReqWearItemMsg			*pWearItemMsg;	
	BOOL						bResult = FALSE;

	pWearItemMsg	=	(OnReqWearItemMsg*)	cMsg;

	// 剪楚醞橾陽朝 嬴檜蠱擊 雜辨й 熱 橈紫煙 и棻.
	if( IsTrading() == TRUE )						goto	END;

	// 剪楚 醞橾 陽朝 嬴檜蠱擊 雜辨й 熱 橈紫煙 и棻.
	bResult = WearItem( pWearItemMsg->uiFollowerID, pWearItemMsg->uiWearLoc, pWearItemMsg->uiItemID, pWearItemMsg->uiPosInventory );	

END:

	if( bResult == TRUE )
	{
		SendFieldMsg( ON_RESPONSE_WEARITEM, SO_SFM_ONLYME, LPVOID( ON_RET_WEARITEM_OK ) );

		if(pWearItemMsg->uiFollowerID == 0)			//	輿檣奢議葛檜 犒濰擊 夥脣擊陽.
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

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	嬴檜蠱擊 雜辨и棻.
//	熱薑 橾濠	:	2002-07-13 螃�� 2:32:09 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
BOOL	SOPlayer::WearItem(UI08 uiFollowerID, UI08 uiWearLoc, UI16 uiItemID, SI16 siPosInInventory)
{
	DBIKWearItem				stWearItem;
	MyItemData					ItemData;
	SI08						siPutOnItemErrorCode;
	
	// 嬴檜蠱擊 雜辨й 熱 氈朝雖 憲嬴螞棻.
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

	// DB縑 盪濰и棻.
	stWearItem.siDBAccount			=	stAccountInfo.siDBAccount;
	stWearItem.uiSlot				=	stAccountInfo.uiSlot;
	stWearItem.uiFollowerID			=	uiFollowerID;
	stWearItem.uiWearLoc			=	uiWearLoc;
	stWearItem.uiItemID				=	uiItemID;						// !!! ItemData.uiID蒂 餌辨п憮朝 寰脹棻.
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
		stWearItem.siStartTime			=	atoi(szTemp);	//⑷營 衛除擊 盪濰и棻.
		sprintf(szTemp,"%04d%02d%02d",siEndYear,siEndMon,siEndDay);
		stWearItem.siEndTime			=	atoi(szTemp);	//⑷營 衛除擊 盪濰и棻.



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

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	蹂羶擊 嫡懊擊陽 雜辨и 嬴檜蠱擊 п羹и棻.
//	熱薑 橾濠	:	2002-07-13 螃�� 2:39:24 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
VOID	SOPlayer::RecvStripItem()
{	
	OnReqStripItemMsg			*pWearItemMsg;	
	BOOL						bResult = FALSE;
	
	pWearItemMsg	=	(OnReqStripItemMsg*)	cMsg;

	// 剪楚醞橾陽朝 嬴檜蠱擊 慇擊 熱 橈紫煙 и棻.
	if( IsTrading() == TRUE )						goto	END;

	bResult	=	StripItem(pWearItemMsg->uiFollowerID,pWearItemMsg->uiWearLoc,pWearItemMsg->uiItemID,pWearItemMsg->uiPosInventory);
	
END:

	if( bResult == TRUE )
	{
		SendFieldMsg(ON_RESPONSE_STRIPITEM, SO_SFM_ONLYME, LPVOID(ON_RET_WEARITEM_OK));
		
		if(pWearItemMsg->uiFollowerID == 0)			//	輿檣奢議葛檜 犒濰擊 夥脣擊陽.
			SendFieldMsg(ON_CHANGE_CLOTHES, SO_SFM_ONLYNEARCHAR, LPVOID(this));
	}
	else
	{

		SendFieldMsg(ON_RESPONSE_STRIPITEM, SO_SFM_ONLYME, LPVOID(ON_RET_WEARITEM_NO));
	}
}


//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	雜辨и 嬴檜蠱擊 п羹и棻.
//	熱薑 橾濠	:	2002-07-13 螃�� 12:35:31 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
BOOL	SOPlayer::StripItem(UI08 uiFollowerID, UI08 uiWearLoc, UI16 uiItemID, SI16 siPosInventory)
{
	DBIKWearItem				stWearItem;
	MyItemData					ItemData;
	
	if( siPosInventory < 0 )
		writeInfoToFile("StripPosError.txt", "StripItem 1 :: [%u], [%u], [%d], [%d]", uiFollowerID, uiWearLoc, 
																						uiItemID, siPosInventory );
//		pMyWorld->clServer->WriteInfo("..\\StripPosError.txt", "StripItem 1 :: [%u], [%u], [%d], [%d]", uiFollowerID, uiWearLoc, 

	// 嬴檜蠱擊 慇擊 熱 氈朝雖 憲嬴螞棻.
	if(IFollower.CanTakeOffItem(uiFollowerID, uiWearLoc, uiItemID, siPosInventory) == FALSE	)				return	FALSE;
	
	// 嬴檜蠱擊 慇朝棻.
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
	
	// DB縑 盪濰и棻.
	stWearItem.siDBAccount			=	stAccountInfo.siDBAccount;
	stWearItem.uiSlot				=	stAccountInfo.uiSlot;
	stWearItem.uiFollowerID			=	uiFollowerID;
	stWearItem.uiWearLoc			=	uiWearLoc;
	stWearItem.uiItemID				=	0;								// !!! 奩萄衛 0擊 厥橫輿橫撿и棻.
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
	
	// 嬴檜蠱擊 慇朝棻.
	IFollower.DisAppearItem(uiFollowerID, uiWearLoc, uiItemID, siPosInventory, &ItemData);	

	// DB縑 盪濰и棻.
	stWearItem.siDBAccount			=	stAccountInfo.siDBAccount;
	stWearItem.uiSlot				=	stAccountInfo.uiSlot;
	stWearItem.uiFollowerID			=	uiFollowerID;
	stWearItem.uiWearLoc			=	uiWearLoc;
	stWearItem.uiItemID				=	0;								// !!! 奩萄衛 0擊 厥橫輿橫撿и棻.
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


	// 0. ⑷營 嬪纂縑憮 嫦唯й熱 氈朝 嬴檜蠱 跡煙擊 賅舒 陛螳 螞棻 
	//----------------------------------------------------------------------------------
	while( pItem = clServer->clWorld->m_buriedItemParser.findItem (uiMapID) )
	{	
		
		foundItems[numFoundItem++] = pItem;		
	
		if( numFoundItem >= 20 ) break;
	}

	
	
	// 1. 嫦唯и 嬴檜蠱檜 氈擊 唳辦 跡煙醞縑憮 ж釭蒂 鷗ж堅 檣漸饜葬縑 厥橫遽棻.
	//    嬴檜蠱 蹺陛縑 撩奢ц棻賊 詭衛雖蒂 爾頂遽棻 
	//----------------------------------------------------------------------------------		
	if( numFoundItem > 0 )
	{
		int s = rand() % numFoundItem; // 跡煙醞 摹鷗и 嬴檜蠱 


		
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

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	NPC蒂 摹鷗ц擊陽
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
VOID	SOPlayer::RecvReqQuestSelectNPC()
{
	OnReqQuestSelectNPC*			lpOnReqQuestSelectNPC;
	SI16							TempSlot;
	SI16							TempQuest;

	lpOnReqQuestSelectNPC = (OnReqQuestSelectNPC*)cMsg;

	stQuestInfo.m_QuestData_BookCount = lpOnReqQuestSelectNPC->siBookCount;

	// 檜 NPC縑 婦溼脹 Quest蒂 ж堅 氈戲賊
	TempSlot = Quest_FindPlayQuestNPC(lpOnReqQuestSelectNPC->siNPCCode);
	Quest_AddQuestObject_Npc(lpOnReqQuestSelectNPC->siNPCCode);

	if(TempSlot != -1)
	{
		// 氈戲賊, 蠡蝶お蒂 諫猿й熱 氈朝雖 匐餌
		if(Quest_CheckCompleteQuest(TempSlot))
		{
			if(stQuestInfo.m_sPlayingQuestHeader[TempSlot]->m_siQuestEvent == QUEST_EVENT_PLAY)
			{
				//歜衛 囀萄.-_-; 橾欽 虞嬴爾濠.
				if(pMyWorld->m_bQuestEventUpdate == TRUE) 
				{
					SendFieldMsg(ON_RESPONSE_QUEST_SELECTNPC, SO_SFM_ONLYME, LPVOID(ON_RET_QUEST_SELECTNPC_PLAYINGQUEST), LPVOID(stQuestInfo.m_sPlayingQuest[TempSlot].m_QuestCode));
				}
				else
					Quest_CheckEventItem(TempSlot);
			}
			else
				Quest_EndQuest(TempSlot);
			// 諫猿橾陽, End
			
		}
		else
		{
			// 嬴霜 嘐 諫猿橾陽, Playing
			SendFieldMsg(ON_RESPONSE_QUEST_SELECTNPC, SO_SFM_ONLYME, LPVOID(ON_RET_QUEST_SELECTNPC_PLAYINGQUEST), LPVOID(stQuestInfo.m_sPlayingQuest[TempSlot].m_QuestCode));
		}
	}
	else
	{
		// 橈戲賊, 檜 NPC縑 婦溼脹 嬴霜 ж雖 跤и Quest蒂 匐餌
		TempQuest = Quest_FindNextQuest(lpOnReqQuestSelectNPC->siNPCCode);
		if(TempQuest > 0)
		{
			// ж雖 跤и Quest陛 氈擊唳辦, 斜 Quest蒂 й 褻勒(Need, Skip Test)檜 腋朝雖 匐餌
			if(Quest_CheckPlayQuestAbility(TempQuest) == TRUE)
			{
				if(Quest_CheckSkipQuestAbility(TempQuest) == FALSE)
				{
					// й 褻勒檜 腋朝 唳辦, Start
					SendFieldMsg(ON_RESPONSE_QUEST_SELECTNPC, SO_SFM_ONLYME, LPVOID(ON_RET_QUEST_SELECTNPC_STARTQUEST), LPVOID(TempQuest));
				}
				else
				{
					// Skip檜 腋朝 唳辦
					SendFieldMsg(ON_RESPONSE_QUEST_SELECTNPC, SO_SFM_ONLYME, LPVOID(ON_RET_QUEST_SELECTNPC_ENDAFTERQUEST), LPVOID(TempQuest));
				}
			}
			else
			{
				// й 褻勒檜 跤腋朝 唳辦, StartBefore
				SendFieldMsg(ON_RESPONSE_QUEST_SELECTNPC, SO_SFM_ONLYME, LPVOID(ON_RET_QUEST_SELECTNPC_STARTBEFOREQUEST), LPVOID(TempQuest));
			}
		}
		else
		{
			// ж雖 跤и Quest陛 橈擊唳辦, 葆雖虞戲煎 檜 NPC Quest蒂 瓊朝棻.
			TempQuest = Quest_FindLastPlayQuest(lpOnReqQuestSelectNPC->siNPCCode);
			if(TempQuest > 0)
			{
				// 瓊擊唳辦, EndAfter
				SendFieldMsg(ON_RESPONSE_QUEST_SELECTNPC, SO_SFM_ONLYME, LPVOID(ON_RET_QUEST_SELECTNPC_ENDAFTERQUEST), LPVOID(TempQuest));
			}
			else
			{
				// 瓊雖 跤й唳辦, Error
				SendFieldMsg(ON_RESPONSE_QUEST_SELECTNPC, SO_SFM_ONLYME, LPVOID(ON_RET_QUEST_SELECTNPC_ERROR));
			}
		}
	}
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	Quest蒂 熱塊ц擊陽
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
VOID	SOPlayer::RecvReqQuestAcceptQuest()
{
	OnReqQuestAcceptQuest*			lpOnReqQuestAcceptQuest;
	SI16							TempSlot;
	DBIKQuestSetPlayQuestData		TempDBIKQuestSetPlayQuestData;

	lpOnReqQuestAcceptQuest	=	(OnReqQuestAcceptQuest*)cMsg;
	
	// 蕨瞪縑 и 蠡蝶お檣雖 匐餌
	if(Quest_FindCompletedQuest(lpOnReqQuestAcceptQuest->m_QuestCode) == FALSE)
	{
		// 蕨瞪縑 и瞳檜 橈擊陽, ⑷營 Quest蒂 й 棟溘檜 腋朝雖 匐餌
		if(Quest_CheckPlayQuestAbility(lpOnReqQuestAcceptQuest->m_QuestCode))
		{
			// 棟溘檜 腋賊, Skip 褻勒檜 腋朝雖 匐餌
			if(Quest_CheckSkipQuestAbility(lpOnReqQuestAcceptQuest->m_QuestCode) == FALSE)
			{
				// Skip檜 寰腆陽, 綴 Quest Slot擊 瓊朝棻.
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
				// Skip 陛棟衛
				SendFieldMsg(ON_RESPONSE_QUEST_ACCEPTQUEST, SO_SFM_ONLYME, LPVOID(ON_RET_NO), LPVOID(lpOnReqQuestAcceptQuest->m_QuestCode));
			}
		}
		else
		{
			// 棟溘檜 寰腋賊
			SendFieldMsg(ON_RESPONSE_QUEST_ACCEPTQUEST, SO_SFM_ONLYME, LPVOID(ON_RET_NO), LPVOID(lpOnReqQuestAcceptQuest->m_QuestCode));
		}
	}
	else
	{
		// 蕨瞪縑 и 蠡蝶お 橾陽
		SendFieldMsg(ON_RESPONSE_QUEST_ACCEPTQUEST, SO_SFM_ONLYME, LPVOID(ON_RET_NO), LPVOID(lpOnReqQuestAcceptQuest->m_QuestCode));
	}
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	霞ч醞檣 Quest蒂 鏃模ц擊陽
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
VOID	SOPlayer::RecvReqQuestCancelQuest()
{
	OnReqQuestCancelQuest*			lpOnReqQuestCancelQuest;
	SI16							TempSlot;
	DBIKQuestCancelQuest			TempDBIKQuestCancelQuest;

	lpOnReqQuestCancelQuest	=	(OnReqQuestCancelQuest*)cMsg;

	// 斜 蠡蝶お陛 ⑷營 霞ч醞檣雖 匐餌
	TempSlot = Quest_FindQuest(lpOnReqQuestCancelQuest->m_QuestCode);
	if(TempSlot != -1)
	{
		// 霞ч醞橾陽 Quest 鏃模
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
		// 霞ч醞檜 嬴棍陽
		SendFieldMsg(ON_RESPONSE_QUEST_CANCELQUEST, SO_SFM_ONLYME, LPVOID(ON_RET_NO), LPVOID(lpOnReqQuestCancelQuest->m_QuestCode));
	}
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	霞ч醞檣 Quest List 蹂羶
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
VOID	SOPlayer::RecvReqQuestGetPlayQuest()
{
	// Quest Data 爾鹵
	SendFieldMsg(ON_RESPONSE_QUEST_GETPLAYQUEST, SO_SFM_ONLYME, LPVOID(NULL), LPVOID(ON_QUSETDATA_MAXPLAYINGQUEST), LPVOID(ON_QUSETDATA_MAXCOMLETEDQUEST), LPVOID(stQuestInfo.m_sPlayingQuest), LPVOID(stQuestInfo.m_siCompletedQuest));
}


//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	檜 NPC縑 婦溼脹橫 Playж堅 氈朝 Quest蒂 瓊朝棻.
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
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

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	檜 Quest陛 諫猿腆熱 氈朝雖 匐餌
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
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

	//楠渾 ж啪 給溥憮 橫雯摹僭檜 釭螃賊.

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
	SI16 siEventItemID;		 //DB曖 纔檜綰縑 氈朝 嬴檜蛤.
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

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	Quest蒂 謙猿и棻.(DB縑 盪濰ж堅, 爾鼻擊 嫡戲貊, Client煎 詭撮雖 瞪歎梱雖 и棻.)
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
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

	// DB縑 盪濰и棻.	

	//if(stQuestInfo.m_sPlayingQuestHeader[QuestSlot]->siQuestCode != DOKDO_QUEST_CODE1 && 
	//  stQuestInfo.m_sPlayingQuestHeader[QuestSlot]->siQuestCode != DOKDO_QUEST_CODE2)
	if(stQuestInfo.m_sPlayingQuestHeader[QuestSlot]->m_siQuestEvent != QUEST_EVENT_PLAY)
	{	
		TempDBIKQuestCompleteQuest.DBAccount  = stAccountInfo.siDBAccount;
		TempDBIKQuestCompleteQuest.uiSlot     = stAccountInfo.uiSlot;
		TempDBIKQuestCompleteQuest.QuestIndex = QuestSlot;
		pMyWorld->clDatabase->writeDBQuery(SO_DB_INFOKIND_QUEST_COMPLETEQUEST, LPVOID(&TempDBIKQuestCompleteQuest), sizeof(TempDBIKQuestCompleteQuest));
	}

	// 爾鼻擊 嫡朝棻.
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
							// 偽擎 嬴檜蠱檜 氈朝雖 匐餌
							lpMyItemData = IFollower.GetItemByID(TempCount1, stQuestInfo.m_sPlayingQuestHeader[QuestSlot]->sQuestPay[TempCount].m_QuestPayCode);
							if(lpMyItemData)
							{
								// 氈戲賊 斜 嬴檜蠱縑 蹺陛
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
								// 橈戲賊 綴蘊檜 氈朝雖 匐餌
								TempItemSlot = IFollower.FindEmptyItemBuffer(TempCount1);
								if((TempItemSlot != -1) && (TempItemSlot != 0xffff))
								{
									// 綴蘊檜 氈戲賊 蹺陛
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
						// 偽擎 嬴檜蠱檜 氈朝雖 匐餌
						lpMyItemData = IFollower.GetItemByID(TempCount1, stQuestInfo.m_sPlayingQuestHeader[QuestSlot]->sQuestPay[TempCount].m_QuestPayCode);
						if(lpMyItemData)
						{
							// 氈戲賊 斜 嬴檜蠱縑 蹺陛
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
							// 橈戲賊 綴蘊檜 氈朝雖 匐餌
							TempItemSlot = IFollower.FindEmptyItemBuffer(TempCount1);
							if((TempItemSlot != -1) && (TempItemSlot != 0xffff))
							{
								// 綴蘊檜 氈戲賊 蹺陛
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

	// 虜擒 Quest縑 в蹂и Item檜 氈歷戲賊, 斜 嬴檜蠱擊 餉薯и棻.
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

	// Client縑 憲萼棻.
	SendFieldMsg(ON_RESPONSE_QUEST_SELECTNPC, SO_SFM_ONLYME, 
		LPVOID(ON_RET_QUEST_SELECTNPC_ENDQUEST), 
		LPVOID(stQuestInfo.m_sPlayingQuest[QuestSlot].m_QuestCode), LPVOID(TempPayData), LPVOID(TempDelItemCount), LPVOID(TempMercenaryItemData), LPVOID(&TempDeleteMoney));

/*	// п渡 嶸盪陛 絮紫 蠡蝶お陛 諫猿ж艘棻朝 匙擊 賅萇 嶸盪縑啪 憲溥遽棻.
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
		// Data蒂 蟾晦�� и棻.
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

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	檜 NPC縑啪憮 霞чй熱 氈朝 棻擠 Quest蒂 瓊朝棻.
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
SI16	SOPlayer::Quest_FindNextQuest(SI16 NPCCode)
{
	SI32				TempCount;
	SI32				TempSmallCode;
	SI32				TempMinCode;
	SI32				TempSlot;

	if(NPCCode >= pMyWorld->clQuestParser.m_NPCSortData_MaxCount) return 0;

	// 霞чи 譆模 囀萄蒂 瓊朝棻.
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

	// 擅戲煎 й熱 氈朝 Quest蒂 瓊朝棻.
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

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	Quest蒂 Startй熱 氈朝雖 匐餌
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
BOOL	SOPlayer::Quest_CheckPlayQuestAbility(SI16 QuestCode)
{
	SI32				TempCount;
	SI32				TempCount1;
	SQuestHeader*		lpSQuestHeader;
	SI16				TempSI16;
	BOOL				fEmptyBuffer;

	// ⑷營 霞ч醞檣雖 匐餌
	for(TempCount = 0; TempCount < ON_QUSETDATA_MAXPLAYINGQUEST; TempCount++)
	{
		if(stQuestInfo.m_sPlayingQuest[TempCount].m_QuestCode == QuestCode)
		{
			return FALSE;
		}
	}

	// 綴 Slot檜 氈朝雖 匐餌
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

	// 褻勒檜 蜃朝雖 匐餌
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

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	Quest蒂 Skipй熱 氈朝雖 匐餌
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
BOOL	SOPlayer::Quest_CheckSkipQuestAbility(SI16 QuestCode)
{
	SI32				TempCount;
	SI32				TempCount1;
	SQuestHeader*		lpSQuestHeader;
	SI16				TempSI16;

	// 褻勒檜 蜃朝雖 匐餌
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

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	檜 NPC縑啪憮 葆雖虞戲煎 Playи Quest匐餌
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
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

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	蕨瞪縑 и瞳檜 氈朝 Quest檣雖 匐餌
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
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

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	綴 Quest Slot擊 瓊擠
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
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

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	霞ч醞檣 Quest蒂 瓊擠
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
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

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	賅萇 Quest 蟾晦��, 梃莠ж啪
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
VOID	SOPlayer::Quest_AllClear(void)
{
	ZeroMemory(stQuestInfo.m_siCompletedQuest,       sizeof(SI16) * ON_QUSETDATA_MAXCOMLETEDQUEST);
	ZeroMemory(stQuestInfo.m_siCompletedQuestHeader, sizeof(SQuestHeader*) * ON_QUSETDATA_MAXCOMLETEDQUEST);
	
	ZeroMemory(stQuestInfo.m_sPlayingQuest,       sizeof(SPlayingQuest) * ON_QUSETDATA_MAXPLAYINGQUEST);
	ZeroMemory(stQuestInfo.m_sPlayingQuestHeader, sizeof(SQuestHeader*) * ON_QUSETDATA_MAXPLAYINGQUEST);

	SendFieldMsg(ON_RESPONSE_QUEST_ALLCLEARQUEST, SO_SFM_ONLYME);
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	避檣 跨蝶攪 Data
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
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
 
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	濠褐檜 и橾
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
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

