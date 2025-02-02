#include "SOMain.h"
#include "SOWorld.h"
#include "SOPlayer.h"
#include "Xplayer.h"

#ifdef _GAMEGUARD
#include "CSAuth.h"
#endif

#include "..\\JWCommon\\JWSocket.h"

#include "CSiegeUniteTable.h"

void SOPlayer::clearCharsInfo()
{
	int i; 

	for( i = 0; i < LOGIN::CHAR_MAX_NUM; ++i )
	{
		m_charsInfo[ i ].slot = 0;
	}
}

void SOPlayer::setSlot( int slot )
{
	if( slot < 1 || slot > LOGIN::CHAR_MAX_NUM ) return;

	m_charsInfo[ slot - 1 ].slot = slot;
}

void SOPlayer::delSlot( int slot )
{
	if( slot < 1 || slot > LOGIN::CHAR_MAX_NUM ) return;

	m_charsInfo[ slot - 1 ].slot = 0;
}

int SOPlayer::getEmptySlot()
{
	int ret = 0;
	int i;

	for( i = 0; i < LOGIN::CHAR_MAX_NUM; ++i )
	{
		if( m_charsInfo[ i ].slot == 0 ) {
			ret = i + 1;
			break;
		}
	}

	return ret;
}

void SOPlayer::sendCharsInfo()
{
	int i;
	LOGIN::sResponseCharsInfo charsinfo;

	charsinfo.command = LOGIN::RESPONSE_CHARS_INFO;
	charsinfo.numChar = 0;
	
	for( i = 0; i < LOGIN::CHAR_MAX_NUM; ++i ) {

		if( m_charsInfo[ i ].slot != 0 ) {
			
			charsinfo.charsInfo[ charsinfo.numChar ] = m_charsInfo[ i ];

			++charsinfo.numChar;
		}
	}

	Write( &charsinfo, sizeof( LOGIN::sResponseCharsInfoFront ) + sizeof( LOGIN::sCharInfo ) * charsinfo.numChar );

}


//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	Name :	
//	Desc :	褒薯煎 煎斜檣擊 籀葬и棻 
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
void SOPlayer::ProcessLogin()
{	
	InterlockedExchange( (long *)&bProcessLogin, FALSE );

	if( uiState == ON_STATE_LOGOUT ) return;

	bSuccessLogin = FALSE;

	POINT	 					ptPlayerPos;
	SI32						i, j;
	OnFollowerInfo				TempFollowerInfo;
	MercenaryInfo				*pMercenaryInfoInDB;
	ItemInfo					*pItemInfo;
	CharHead*					lpCharHead;			
	OnlineFollower				*pFollower;
	MyItemData					ItemData;
	SI32						siFriendCount		=	0;
	SI16						siTimerItemUseCount	=	0;
	SI16					   siDeleteItemCount    =   0;


	m_bCompareWearItem = FALSE;

	memset(&TempFollowerInfo, 0, sizeof(TempFollowerInfo));
		
	if(logininfo->charBaseInfo.account == 0) goto END;
	
	// 虜擒 濠褐曖 啗薑檜 蕾樓п 氈棻賊 煎斜嬴醒 衛鑑棻.
	for(i = 0; i < ON_MAX_CONNECTION; i++)
	{
		if( pMyWorld->clPlayer[i] == this )		continue;

		if( pMyWorld->clPlayer[i]->stAccountInfo.siDBAccount == logininfo->charBaseInfo.account )
		{
//			pMyWorld->clServer->WriteInfo("..\\Already Logined.txt", "[ %u ] ", uiAccount );
			writeInfoToFile("Already Logined.txt", "[ %u ] ", uiAccount );			
			pMyWorld->clPlayer[i]->LogOut();				
			break;
		}
	}		

	// for debug
	if( logininfo->charBaseInfo.mapID == 0 && logininfo->charBaseInfo.x_pos < 100 && logininfo->charBaseInfo.y_pos < 100 )
	{
//		pMyWorld->clServer->WriteInfo( "SendToHanYang.txt", "DBAccount(%8d)  szID(%10s)  MapID(%2d)  XY(%2d, %2d)  Pet(%d)",
		writeInfoToFile( "SendToHanYang.txt", "DBAccount(%8d)  szID(%10s)  MapID(%2d)  XY(%2d, %2d)  Pet(%d)",
										logininfo->charBaseInfo.account,
										logininfo->charBaseInfo.name,
										logininfo->charBaseInfo.mapID,
										logininfo->charBaseInfo.x_pos,
										logininfo->charBaseInfo.y_pos,
										logininfo->charBaseInfo.VMercenary );		
		logininfo->charBaseInfo.x_pos	=	1100;
		logininfo->charBaseInfo.y_pos	=	620;
	}

	// 晦獄 瞳檣 餌辨濠曖 薑爾蒂 瓣錶厥朝棻.
	Init(uiAccount, logininfo->charBaseInfo.mapID, logininfo->charBaseInfo.x_pos, logininfo->charBaseInfo.y_pos, logininfo->charBaseInfo.kind, logininfo->charBaseInfo.name);	

	stAccountInfo.siDBAccount				=	logininfo->charBaseInfo.account;
	siMoney									=	logininfo->charBaseInfo.money;	  // 絲.
	m_MoneySaving                           =	logininfo->charBaseInfo.saving;   // 盪蹴擋
	investProfit							=   logininfo->charBaseInfo.investProfit; 										// 癱濠 熱櫛
	additionProfit							=   logininfo->charBaseInfo.additionProfit;										// 睡陛 熱櫛 		

	uiGuildID								=	logininfo->charBaseInfo.guildID;											// Guild ID.
	siClassInGuild							=	logininfo->charBaseInfo.guildGrade;											// Guild Class.
	
	m_satiety								=	logininfo->charBaseInfo.satiety;	// ん虜馬.		
	m_orgSatiety							=	m_satiety;

	uiTradeCredit							=	logininfo->charBaseInfo.credit_trade;										// 鼠羲 褐辨紫.
	siDayCredit								=	logininfo->charBaseInfo.credit_daily;										// 螃棺 ж瑞縑 螃艇 褐辨紫.		
	siNearestHospitalVillageIndexInDungeon	=	pMyWorld->GetVillageIndexByCode(logininfo->charBaseInfo.nearVillage);		// 避歷擊陽 斬籀煎 陛朝 葆擊.
	USGrade									=	UserGrade(logininfo->charBaseInfo.userGrade);														

	siVMercenarySlot						=	logininfo->charBaseInfo.VMercenary;
	m_siMaxGeneral							=	logininfo->charBaseInfo.maxGeneral;											// 堅辨 陛棟и 譆渠 濰熱熱

	m_dwCondLaborTime						=	0;
	
	// 鼻欽曖 梓嫦擊 橢橫螞棻.	
	if(uiGuildID != 0)		siFlag			=	pMyWorld->clIGuild.GetFlag(uiGuildID);		
	else					siFlag			=	0;

	
	//雖旎檜 奢撩 衛除檣雖 羹觼
	if(pMyWorld->m_WarTimeParser.IsWarDay(pMyWorld->pRealTime->tm_wday))
	{
		if(pMyWorld->m_WarTimeParser.IsWarTime(pMyWorld->pRealTime->tm_hour))
		{
			m_uiPrevKind							=	(UI16)logininfo->charWarInfo.PrevKind;
			m_siFieldHp								=	(SI32)logininfo->charWarInfo.Hp;
			m_siFieldMP        						=	(SI32)logininfo->charWarInfo.Mp;
			m_bCaptainUnit							=	(BOOL)logininfo->charWarInfo.bCaptain;
			m_dwDeadSec								=	(DWORD)logininfo->charWarInfo.Delay;

			if(m_dwDeadSec > 0 )
			{
				m_dwDeadTime  = pMyWorld->dwtimeGetTime;
				m_dwDeadSec   = 1000 * 30;
				m_bFieldAlive = FALSE;
			}
		}
	}

	// 辨煽菟曖 薑爾蒂 瓣錶厥朝棻.
	IFollower.Init();
		
	for(i = 0; i < ON_MAX_FOLLOWER_NUM; i++)
	{
		pMercenaryInfoInDB	=	&logininfo->mercenaryInfos[i];
		pItemInfo			=	(ItemInfo*)&logininfo->mercenaryItemInfos[i];

		if(pMercenaryInfoInDB->kind != 0)
		{	
			// 嶸�褲� 辨煽檜棻.
			if(m_uiPrevKind != 0 && i == 0)
				TempFollowerInfo.uiKind = logininfo->charBaseInfo.kind;
			else
				TempFollowerInfo.uiKind							=	pMercenaryInfoInDB->kind;
			strncpy(TempFollowerInfo.Name, pMercenaryInfoInDB->name, ON_ID_LENGTH);
			TempFollowerInfo.siExp								=	pMercenaryInfoInDB->Exp;
			TempFollowerInfo.siLevel							=	pMercenaryInfoInDB->Level;
			TempFollowerInfo.siStr								=	pMercenaryInfoInDB->Str;
			TempFollowerInfo.siDex								=	pMercenaryInfoInDB->Dex;
			TempFollowerInfo.siVit								=	pMercenaryInfoInDB->Vit;
			TempFollowerInfo.siInt								=	pMercenaryInfoInDB->Int;				
			TempFollowerInfo.siLife								=	pMercenaryInfoInDB->Life;
			TempFollowerInfo.siMana								=	pMercenaryInfoInDB->Mana;
			TempFollowerInfo.siBonus							=	pMercenaryInfoInDB->Bonus;
			TempFollowerInfo.siAC								=	pMercenaryInfoInDB->AC;
			TempFollowerInfo.siDamageRegist						=	pMercenaryInfoInDB->DamageRegist;
			TempFollowerInfo.siMagicRegist						=	pMercenaryInfoInDB->MagicRegist;

			if( ( lpCharHead = pMyWorld->GetCharHeader( pMercenaryInfoInDB->kind ) ) == NULL)
			{
//				pMyWorld->clServer->WriteInfo("..\\InvalidFollower.txt", "[ DBAccount : %u, DBSlot : %u, Kind : %u ] ", 
				writeInfoToFile("InvalidFollower.txt", "[ DBAccount : %u, DBSlot : %u, Kind : %u ] ", 
							 stAccountInfo.siDBAccount, stAccountInfo.uiSlot, pMercenaryInfoInDB->kind );				

				goto	END;
			}
			else
			{
				TempFollowerInfo.siLiftConstant		=	lpCharHead->siCon;
			}
			
			TempFollowerInfo.siEquipMinDamage					=	pMercenaryInfoInDB->EquipMinDamage;
			TempFollowerInfo.siEquipMaxDamage					=	pMercenaryInfoInDB->EquipMaxDamage;

			// 辨煽擊 蟾晦�� и棻.
			IFollower.InitFollower(pMercenaryInfoInDB->slot, &TempFollowerInfo, lpCharHead->siNation, &pMyWorld->clItem);


			// 辨煽菟曖 嬴檜蠱擊 瓣錶厥朝棻.
			for(j = 0; j < ON_MYDATA_MAX_ITEM_NUM; j++)
			{
				if(pItemInfo[j].itemID != 0)
				{
					// 嶸�褲� 嬴檜蠱檜棻.
					ItemData.uiID				=	pItemInfo[j].itemID;
					ItemData.siPosInInventory	=	pItemInfo[j].posInInventory;
					ItemData.uiQuantity			=	pItemInfo[j].quantity;
		
					IFollower.AddItem(pMercenaryInfoInDB->slot, &ItemData);
				}
			}
			pFollower		=	IFollower.GetFollower(pMercenaryInfoInDB->slot);		

			// 辨煽菟檜 濰雜ж堅 氈朝 嬴檜蠱菟擊 瓣錶厥朝棻.
			SetEquipItems(&logininfo->mercenaryEquipItemInfos[pMercenaryInfoInDB->slot],pMercenaryInfoInDB->slot,siTimerItemUseCount,pFollower);

			// 辨煽菟曖 棟溘纂蒂 唸薑и棻.
			pFollower->ParametaBox.Action(&pMyWorld->clItem, pFollower->Weapon);
			
			// 蠡蝶お蒂 盪濰и棻.
			CopyMemory(stQuestInfo.m_siCompletedQuest, logininfo->completeQuests, sizeof(SI16) * ON_QUSETDATA_MAXCOMLETEDQUEST);
			CopyMemory(stQuestInfo.m_sPlayingQuest, logininfo->progressQuests, sizeof(SPlayingQuest) * ON_QUSETDATA_MAXPLAYINGQUEST);
			for(j = 0; j < ON_QUSETDATA_MAXCOMLETEDQUEST; j++)
			{
				stQuestInfo.m_siCompletedQuestHeader[j] = pMyWorld->clQuestParser.GetQuest(stQuestInfo.m_siCompletedQuest[j]);
			}
			for(j = 0; j < ON_QUSETDATA_MAXPLAYINGQUEST; j++)
			{
				stQuestInfo.m_sPlayingQuestHeader[j] = pMyWorld->clQuestParser.GetQuest(stQuestInfo.m_sPlayingQuest[j].m_QuestCode);
			}
		}
	}
	
	m_latestSavedPosTime		=		timeGetTime();

	// 寡ル 薑爾蒂 橢橫螞棻.
	stTicketInfo.siTicketKind = logininfo->charBaseInfo.ticketKind;

	siDeleteItemCount = GetClientSendDeleteTimerItem(m_pTimerItemInfo);
	if(siDeleteItemCount > 0)				m_bCompareWearItem = TRUE;

	// DB煎睡攪 橢橫螞 耀掘 葬蝶お蒂 撢たи棻.
	m_clFriendList.DeleteAllFriend();
	while( strcmp(logininfo->friendInfos[siFriendCount].szID, "") != 0 && siFriendCount < MAX_FRIEND_NUM )
	{
		BOOL bLogin	= pMyWorld->GetPlayer( logininfo->friendInfos[siFriendCount].szID ) == NULL ? FALSE : TRUE;
		m_clFriendList.AddFriend( this->szID, logininfo->friendInfos[siFriendCount].szID, bLogin );
		siFriendCount++;
	}

	// 盪濠剪葬縑 氈棻賊 葆擊縑 樓п氈朝 匙戲煎 撲薑и棻.
	stAccountInfo.pVillage		=	pMyWorld->GetVillageByCode( pMyWorld->clISOMap.GetVillageCode(uiMapID) );	
	if( stAccountInfo.pVillage != NULL)
		EnterVillage( TRUE );

	
	// ⑷營 議葛攪蒂 裘縑 撲薑й 熱 氈朝雖 憲嬴螞棻.
	// �公� 棻艇 議葛攪陛陛 ⑷ 嬪纂縑 場擊 熱 橈朝 鼻�窕恔�...
	// ⑷營 嬪纂蒂 晦遽戲煎 裘縑 褕擊 謝ル蒂 橢橫螞棻.
	ptPlayerPos	= pMyWorld->clISOMap.SearchMap( logininfo->charBaseInfo.mapID, 
											    logininfo->charBaseInfo.x_pos, 
											    logininfo->charBaseInfo.y_pos, 
											    ON_MAP_SI_CHAR, uiMoveableTileInfo);	

	// 釭螢 濠葬陛 橈棻賊 煎斜檣衛 褒ぬи匙檜棻.
	if((ptPlayerPos.x == -1) && (ptPlayerPos.y == -1))					goto	END;	

	// 嬪纂蒂 棻衛 熱薑и棻.
	siX							=		ptPlayerPos.x;
	siY							=		ptPlayerPos.y;			

	m_latestSavedMapID			=		uiMapID;
	m_latestSavedPos.x			=		siX;
	m_latestSavedPos.y			=		siY;
	

	// 裘縑棻陛 濠晦曖 嬴檜蛤蒂 撲薑и棻.
	if( SetIDOnMap( 7 ) == FALSE )										goto	END;

	// 薑衝 鼻欽錳橾 唳辦 鼻欽 婦葬濠縑啪 煎斜檣 ж艘棻堅 憲溥遽棻.
	if( ( uiGuildID != 0 ) && ( siClassInGuild != pMyWorld->clIGuildClass.GetJWJCode() ) )
	{
		pMyWorld->clIGuild.GuildMemberLogin( uiGuildID );
	}


	// 煎斜檣擊 撩奢ц棻
	bSuccessLogin				=		TRUE;

END:
	// 煎斜檣 薑爾蒂 爾頂遽棻.
	if(bSuccessLogin == TRUE)
	{
		m_lastTick = GetTickCount();
		
		// 煎斜檣檜 撩奢ц棻堅 爾頂遽棻.
		SendLogIn(TRUE);

		if( m_uiRecvItemNum > 0 ) SendItemCount();

		// 濠褐曖 蠡蝶お Data蒂 瞪歎и棻.
		SendFieldMsg(ON_RESPONSE_QUEST_GETPLAYQUEST, SO_SFM_ONLYME, LPVOID(NULL), LPVOID(ON_QUSETDATA_MAXPLAYINGQUEST), LPVOID(ON_QUSETDATA_MAXCOMLETEDQUEST), LPVOID(stQuestInfo.m_sPlayingQuest), LPVOID(stQuestInfo.m_siCompletedQuest));

		// 陳噢 薑爾蒂 爾頂遽棻.
		SendFieldMsg(ON_CHANGE_WEATHER, SO_SFM_ONLYME, LPVOID(pMyWorld->bNowWeather));

		SendFieldMsg(ON_RESOPNSE_REAL_TIME, SO_SFM_ONLYME, LPVOID(pMyWorld->pRealTime->tm_year + 1900),LPVOID(pMyWorld->pRealTime->tm_mon + 1),LPVOID(pMyWorld->pRealTime->tm_mday));

		// 耀掘 薑爾蒂 爾頂遽棻.
		RecvAuditFriendList();

		// 檜 Ы溯檜橫蒂 耀掘煎 蛔煙и 餌塋菟縑啪 檜 Ы溯檜橫陛 煎斜檣ц擠擊 憲萼棻.
		NotifyFriendsOfLoginState( TRUE );
		
		//奢撩ж朝 陳檜堅 雖旎檜 奢撩 衛除檜賊 
		if(pMyWorld->m_WarTimeParser.IsWarCheck(pMyWorld->pRealTime->tm_wday, pMyWorld->pRealTime->tm_hour))
//		if(pMyWorld->m_WarTimeParser.IsWarTime(pMyWorld->pRealTime->tm_hour))	
		{
			cltGuild *pcltGuild = NULL;
			pcltGuild = pMyWorld->clIGuild.GetGuildByID(uiGuildID);
			if(pcltGuild != NULL)
			{
				int nGiveUpFlag = pcltGuild->GetGiveUpFlag();
				if( pcltGuild->GetgiveUpVillageCode() < 0 ) return;
				
				//渠寞檣陛
				if(siClassInGuild == pMyWorld->clIGuildClass.GetNumericClassCode(SO_GUILD_SC_DAI))
				{
					if( nGiveUpFlag == ON_GUILD_GIVEUP_OK || nGiveUpFlag == ON_GUILD_GIVEUP_NORMAL ) return;
					if( nGiveUpFlag == ON_GUILD_GIVEUP_NOT_ANSWER)
					{
						UI16 uiVillageCode;
						MONEY mnMoney;
						pcltGuild->GetGiveUpInfo(&uiVillageCode, &mnMoney);
						SendFieldMsg(ON_ASK_GIVEUP, SO_SFM_ONLYME, LPVOID(uiVillageCode), LPVOID(mnMoney));
					}
				}
				else
				{
					if(nGiveUpFlag == ON_GUILD_GIVEUP_OK)
					{
						SendFieldMsg(ON_RESPONE_GIVEUP, SO_SFM_ONLYME);
						
						//奢撩 嶸棉檣 唳辦虜 錳鼻犒敝 衛鑑棻.
						if(IsFieldAttackUnit())  ChangeNormalChar();
					}
				}
			}
		}
	}
	else
	{	
		SendLogIn(FALSE);

		LogOut();
	}	
}



//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	Name :	
//	Desc :	煎斜檣 蹂羶擊 籀葬и棻 
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
VOID SOPlayer::RecvLogIn( char *pMsg )
{
	if( uiState == ON_STATE_LOGOUT ) {
		printf( "RecvLogin failed\n" );
		return;
	}

//	printf("RecvLoginok..\n" );

	OnReqGameServerLogInMsg	    *pGameServerLogInMsg;	
	char						cGUID[ON_MAX_GUID_LENGTH+1];	

	pGameServerLogInMsg	= (OnReqGameServerLogInMsg*)pMsg;
	
	memset( cGUID, 0, sizeof(cGUID) );	
	

	// 議葛攪 蝸煜(1 ~ 3)檜 螢夥艇陛?
	if( pGameServerLogInMsg->uiCharSlot < 1 || pGameServerLogInMsg->uiCharSlot > 3 ) 
	{
		writeInfoToFile( "LoginError.txt", "Invalid CharacterSlot(%d) has been detected", pGameServerLogInMsg->uiCharSlot );
		return;
	}

	// DB縑憮 煎斜檣 薑爾蒂 橢橫螞棻.
	memset( logininfo, 0, sizeof(LoginInfo) );
	if(pMyWorld->clLoginDatabase->GetLoginInfo( stAccountInfo.siDBAccount, pGameServerLogInMsg->uiCharSlot, stAccountInfo.IP, logininfo ) == FALSE )
	{
		return;
	}

	if(pMyWorld->m_WarTimeParser.IsWarDay(pMyWorld->pRealTime->tm_wday))
	{
		if(pMyWorld->m_WarTimeParser.IsWarTime(pMyWorld->pRealTime->tm_hour))
		{	
			DBIKPlayerDBInfo TempPlayerInfo;
			memset(&TempPlayerInfo, 0, sizeof(TempPlayerInfo));

			TempPlayerInfo.DBAccount = stAccountInfo.siDBAccount;
			TempPlayerInfo.uiSlot	 = pGameServerLogInMsg->uiCharSlot;
			pMyWorld->clLoginDatabase->GetDBInfo(SO_DB_INFOKIND_WAR_LOGIN,&TempPlayerInfo,logininfo);
		}
	}

	stAccountInfo.uiSlot		=	pGameServerLogInMsg->uiCharSlot;


	// 掘殮и 嬴檜蠱 偃熱蒂 橢橫螞棻
	m_uiRecvItemNum = 0;
	
	DBIKPlayerDBInfo playerinfo;

	playerinfo.DBAccount = logininfo->charBaseInfo.account;

	if( pMyWorld->clItemDatabase->GetDBInfo2( SO_DB_INFOKIND_GETITEMCOUNT_ITEMBANK, &playerinfo, &m_uiRecvItemNum ) == FALSE ) {
		m_uiRecvItemNum = 0;
	}

	if( m_uiRecvItemNum > 0 ) {
		// 嬴檜蠱 葬蝶お蒂 橢橫螞棻.

		char	buf[ 1024 ];

		UI16	count = 0;

		if( pMyWorld->clItemDatabase->GetDBInfo2( SO_DB_INFOKIND_ITEMLIST_ITEMBANK, &playerinfo, buf ) == FALSE ) {
			memcpy( buf, &count, 2 );
		} else {
			memcpy( &count, buf, 2 );
		}

		m_ItemBankList.SetList( count, &buf[ 2 ] );
	}

	InterlockedExchange( (long *)&bProcessLogin, TRUE );

}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	
//	熱薑 橾濠	:	2002-02-26 螃�� 5:09:28 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
VOID SOPlayer::SendLogIn(BOOL bSuccess)
{		
	char						cSendData[ON_MAX_LOCALBUFFER];				
	OnlineReceiveFieldItemInfo	*pSendFieldItemInfo;
	OnFollowerInfo				*pSendFollowerInfo;
	UI16						uiCharUniIDs[ON_STATUS_AREA_XSIZE * ON_STATUS_AREA_YSIZE];
	OnlineReceiveFieldItemInfo	FieldItemInfo[ON_STATUS_AREA_XSIZE * ON_STATUS_AREA_YSIZE];
	UI32						uiPlayersInfoMsgLength;
	CHAR						*pSendPlayerInfo;
	CHAR						*pszGuildName;
	DWORD						siFollowerSendSize;
	OnGameServerLogInMsg		*pGameServerLogInMsg;
	OnGameServerLogInMsgData	*pGameServerLogInMsgData;
	SI32						siMsgLength;

	ZeroMemory( cSendData, sizeof( cSendData ) );
	pGameServerLogInMsg		=	( OnGameServerLogInMsg* ) cSendData;
	pGameServerLogInMsgData	=	( OnGameServerLogInMsgData* ) ( ( CHAR* ) pGameServerLogInMsg + sizeof( OnGameServerLogInMsg ) );

	pGameServerLogInMsg->dwMsgType									=	ON_RESPONSE_LOGIN;
	if( bSuccess == TRUE)	pGameServerLogInMsg->dwResponse			=	ON_RET_OK;
	else					pGameServerLogInMsg->dwResponse			=	ON_RET_NO;	

	if( pGameServerLogInMsg->dwResponse == ON_RET_OK )
	{	
		// 煎斜檣縑 撩奢ц棻.
		uiState			=	ON_STATE_LOGIN;				

		// 憮幗曖 衛除擊 爾頂遽棻.
		pGameServerLogInMsgData->Time			=	pMyWorld->CurrentTime;		

		// 褒薯 撮啗曖 衛除擊 橢橫螞棻.
		pGameServerLogInMsgData->RealTime		=	pMyWorld->RealWorldTime;			


//		RecvAuditTotalInfo();		

		// 輿嬪縑 議葛攪諦 嬴檜蠱曖 薑爾蒂 橢橫螞棻.
		pGameServerLogInMsgData->uiPlayerNum		=	pMyWorld->clISOMap.GetNearSectorPlayerList(uiMapID, siX, siY, uiCharUniIDs, ON_MAP_CHARID_FIELD);
/*		{
			int		siDataCounter;

			for( siDataCounter = 0; siDataCounter < pGameServerLogInMsgData->uiPlayerNum; siDataCounter++)
			{
				if( uiCharUniIDs[ siDataCounter ] == 0 )
				{
					pMyWorld->clServer->WriteInfo("..\\ZeroPlayerID.txt", "Login1 - [ %u, %d, %d ]", uiMapID, siX, siY );							
				}
			}
		}
*/
		
		pGameServerLogInMsgData->uiFieldItemNum		=	pMyWorld->clISOMap.GetNearSectorItemList(uiMapID, siX, siY, FieldItemInfo);
/*		{
			int		siDataCounter;

			for( siDataCounter = 0; siDataCounter < pGameServerLogInMsgData->uiPlayerNum; siDataCounter++)
			{
				if( uiCharUniIDs[ siDataCounter ] == 0 )
				{
					pMyWorld->clServer->WriteInfo("..\\ZeroPlayerID.txt", "Login2 - [ %u, %d, %d, %u ]", uiMapID, siX, siY, 
																pGameServerLogInMsgData->uiFieldItemNum );							
				}
			}
		}*/

		// 議葛攪曖 薑爾蒂 橢橫螞棻.		
		pGameServerLogInMsgData->uiMyCharUniqueID	=	uiAccount;
		pGameServerLogInMsgData->siMoney			=	siMoney;
		pGameServerLogInMsgData->moSaving           =   m_MoneySaving;
		pGameServerLogInMsgData->uiTradeCredit		=	uiTradeCredit;
		pGameServerLogInMsgData->uiMapID			=	uiMapID;
		pGameServerLogInMsgData->USGrade			=	USGrade;
		pGameServerLogInMsgData->YearProfit			=   investProfit;
		pGameServerLogInMsgData->SoldFacilityPrice	=   additionProfit;
		pGameServerLogInMsgData->siVMercenaySlot	=	siVMercenarySlot;

		ZeroMemory(pGameServerLogInMsgData->szGuildID, ON_ID_LENGTH);
		pGameServerLogInMsgData->siGuildClass		=	0;
		pGameServerLogInMsgData->siHealth			=	m_satiety;
		pGameServerLogInMsgData->siMaxGeneral		=	m_siMaxGeneral;

		pGameServerLogInMsgData->bCompareWearItem   =  m_bCompareWearItem;
		pGameServerLogInMsgData->bEvent				=  g_GameServerInfoParser.IsGameEvent();

		pGameServerLogInMsgData->siEventRanking		=	pMyWorld->m_ExpoRankingSystem.GetRank( stAccountInfo.siDBAccount );
		pGameServerLogInMsgData->siEventScore		=	pMyWorld->m_ExpoRankingSystem.GetScore( stAccountInfo.siDBAccount );

		pGameServerLogInMsgData->m_siFieldHp		=	m_siFieldHp;
		pGameServerLogInMsgData->m_bCaptainUnit		=	m_bCaptainUnit;
		pGameServerLogInMsgData->m_siSupplyCharge	=	m_siFieldMP;
		pGameServerLogInMsgData->m_dwDeadSec		=	m_dwDeadSec;
		pGameServerLogInMsgData->m_uiPrevKind		=	m_uiPrevKind;



		// 濠晦曖 檜葷擊 橢橫螞棻.
		ZeroMemory(pGameServerLogInMsgData->szID, sizeof(pGameServerLogInMsgData->szID));
		strncpy(pGameServerLogInMsgData->szID, szID, ON_ID_LENGTH);
		
		pGameServerLogInMsgData->uiDokdoQuestCompletedUserCount = pMyWorld->m_uiDokdoQuestCompletedUserCount;

		// 鼻欽檜 嶸�褲捘� 匐餌и棻.
		if( uiGuildID != 0 )
		{
			pszGuildName	=	pMyWorld->clIGuild.GetGuildName(uiGuildID);

			if(pszGuildName != NULL)			
			{
				// 鼻欽檜 嶸�褲炴�.
				strncpy(pGameServerLogInMsgData->szGuildID, pszGuildName, ON_ID_LENGTH);		
				pGameServerLogInMsgData->siGuildClass	=	siClassInGuild;
				pGameServerLogInMsgData->siFlag			=	siFlag;
				if(pGameServerLogInMsgData->siGuildClass != pMyWorld->clIGuildClass.GetJWJCode())
					pGameServerLogInMsgData->m_bGuildWarDay =   pMyWorld->clIGuild.GetGuildWarCheck(uiGuildID);
				else
					pGameServerLogInMsgData->m_bGuildWarDay = FALSE;

			}
			else
			{ 

				// 鼻欽檜 嶸�褲狫� 彊棻.
				SetGuild( 0 );

			}
			
		}

/*		{
			int		siDataCounter;

			for( siDataCounter = 0; siDataCounter < pGameServerLogInMsgData->uiPlayerNum; siDataCounter++)
			{
				if( uiCharUniIDs[ siDataCounter ] == 0 )
				{
					pMyWorld->clServer->WriteInfo("..\\ZeroPlayerID.txt", "Login3 - [ %u, %d, %d ]", uiMapID, siX, siY );							
				}
			}
		}*/

		pSendPlayerInfo			=	( CHAR* ) ( ( ( CHAR* ) pGameServerLogInMsgData ) + sizeof( OnGameServerLogInMsgData ) );
		uiPlayersInfoMsgLength	=	MakeOnReceivedPlayerInfoPacket(pSendPlayerInfo, pGameServerLogInMsgData->uiPlayerNum, uiCharUniIDs, &pGameServerLogInMsgData->uiPlayerNum, 8 );

		// в萄曖 嬴檜蠱擊 瓣錶 厥朝棻.
		pSendFieldItemInfo	=	(OnlineReceiveFieldItemInfo*)((CHAR*)pSendPlayerInfo + uiPlayersInfoMsgLength);	
		memcpy(pSendFieldItemInfo, FieldItemInfo, sizeof(OnlineReceiveFieldItemInfo) * pGameServerLogInMsgData->uiFieldItemNum);		

		// 辨煽菟曖 薑爾蒂 橢橫螞棻.
		pSendFollowerInfo	=	(OnFollowerInfo*)((CHAR*)pSendFieldItemInfo + sizeof(OnlineReceiveFieldItemInfo) * pGameServerLogInMsgData->uiFieldItemNum);
		siFollowerSendSize	=	GetAllFollowerInfo(pSendFollowerInfo, &pGameServerLogInMsgData->uiFollowerNum);

		// 爾頂撿 ж朝 餌檜鍔蒂 唸薑и棻.
		siMsgLength	=	sizeof( OnGameServerLogInMsg ) + sizeof( OnGameServerLogInMsgData ) + uiPlayersInfoMsgLength + 
						( sizeof( OnlineReceiveFieldItemInfo ) * pGameServerLogInMsgData->uiFieldItemNum ) + siFollowerSendSize;		
		
       
		//--------------------------
//		if(pMyWorld->m_FieldBattleParser.IsFieldAttackUnit(uiKind) == TRUE)
		if(CSiegeUnitTable::GetInstance()->IsFieldAttackUnit(uiKind) == TRUE)
		{
			int BP = GetFightingPower();
			SI16 siUnitCode = CSiegeUnitTable::GetInstance()->GetKindUnitCode(uiKind);
			if(siUnitCode < 0) return;

			SI16 siHP = CSiegeUnitTable::GetInstance()->GetHP(siUnitCode, BP);
			
			if(m_bCaptainUnit)
				SetFieldMaxHP(siHP * 2);
			else
				SetFieldMaxHP(siHP);

			if(siUnitCode == FIELD_BATTLE_SUPPLY)
			{
				SI16 siMP = CSiegeUnitTable::GetInstance()->GetMP(siUnitCode, BP);
				SetFieldMaxMP(siMP);
			}
		}

	}
	else
	{
		siMsgLength	=	sizeof( OnGameServerLogInMsg );
	}	

	Write( pGameServerLogInMsg, siMsgLength );	
}



//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
// 煎斜嬴醒и棻.
// 熱薑 橾濠 : 2002-02-19 螃瞪 11:36:02 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
VOID SOPlayer::LogOut(VOID)
{
	printf("LogOut..\n");

	if( uiState != ON_STATE_LOGOUT )
	{
		// 煎斜 嬴醒 擋暮!
		m_pXPlayer->SetAction();
	}	

}


//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
// 濠褐擊 耀掘煎 蛔煙и 餌塋菟縑啪 濠褐檜 煎斜檣/煎斜嬴醒 ц擠擊 憲萼棻.
// 熱薑 橾濠 : 2003-04-21 螃瞪 12:02 - 寥鼻⑷
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
VOID SOPlayer::NotifyFriendsOfLoginState( BOOL bLogin )
{
	for( int i = 1; i < ON_MAX_CONNECTION + 1; i++ )
	{
		if( pMyWorld->pPlayerOnAccount[i] != NULL )
		{			
			if( pMyWorld->pPlayerOnAccount[i]->m_clFriendList.GetFriend(szID) != NULL )		// NULL檜 嬴棲賊 斜 耀掘曖 葬蝶お縑 頂陛 氈棻.
			{
				// 斜 耀掘曖 葬蝶お縑憮 釭蒂 煎斜檣/煎斜嬴醒 鼻鷓煎 虜萇棻.
				pMyWorld->pPlayerOnAccount[i]->m_clFriendList.SetLogin( szID, bLogin );
				// 斜 耀掘縑啪 煎斜檣/嬴醒 ц擠擊 憲葬朝 詭衛雖蒂 爾頂遽棻.
				pMyWorld->pPlayerOnAccount[i]->SendFieldMsg(ON_FRIEND_LOGGEDINOUT, SO_SFM_ONLYME, LPVOID(bLogin), LPVOID(szID));
			}
		}
	}
}