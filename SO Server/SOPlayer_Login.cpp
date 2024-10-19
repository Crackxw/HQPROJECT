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


//¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬
//	Name :	
//	Desc :	½ÇÁ¦·Î ·Î±×ÀÎÀ» Ã³¸®ÇÑ´Ù 
//¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬
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
	
	// ¸¸¾à ÀÚ½ÅÀÇ °èÁ¤ÀÌ Á¢¼ÓÇØ ÀÖ´Ù¸é ·Î±×¾Æ¿ô ½ÃÅ²´Ù.
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

	// ±âº» ÀûÀÎ »ç¿ëÀÚÀÇ Á¤º¸¸¦ Ã¤¿ö³Ö´Â´Ù.
	Init(uiAccount, logininfo->charBaseInfo.mapID, logininfo->charBaseInfo.x_pos, logininfo->charBaseInfo.y_pos, logininfo->charBaseInfo.kind, logininfo->charBaseInfo.name);	

	stAccountInfo.siDBAccount				=	logininfo->charBaseInfo.account;
	siMoney									=	logininfo->charBaseInfo.money;	  // µ·.
	m_MoneySaving                           =	logininfo->charBaseInfo.saving;   // ÀúÃà¾×
	investProfit							=   logininfo->charBaseInfo.investProfit; 										// ÅõÀÚ ¼öÀÍ
	additionProfit							=   logininfo->charBaseInfo.additionProfit;										// ºÎ°¡ ¼öÀÍ 		

	uiGuildID								=	logininfo->charBaseInfo.guildID;											// Guild ID.
	siClassInGuild							=	logininfo->charBaseInfo.guildGrade;											// Guild Class.
	
	m_satiety								=	logininfo->charBaseInfo.satiety;	// Æ÷¸¸°¨.		
	m_orgSatiety							=	m_satiety;

	uiTradeCredit							=	logininfo->charBaseInfo.credit_trade;										// ¹«¿ª ½Å¿ëµµ.
	siDayCredit								=	logininfo->charBaseInfo.credit_daily;										// ¿À´Ã ÇÏ·ç¿¡ ¿À¸¥ ½Å¿ëµµ.		
	siNearestHospitalVillageIndexInDungeon	=	pMyWorld->GetVillageIndexByCode(logininfo->charBaseInfo.nearVillage);		// Á×¾úÀ»¶§ ±ÙÃ³·Î °¡´Â ¸¶À».
	USGrade									=	UserGrade(logininfo->charBaseInfo.userGrade);														

	siVMercenarySlot						=	logininfo->charBaseInfo.VMercenary;
	m_siMaxGeneral							=	logininfo->charBaseInfo.maxGeneral;											// °í¿ë °¡´ÉÇÑ ÃÖ´ë Àå¼ö¼ö

	m_dwCondLaborTime						=	0;
	
	// »ó´ÜÀÇ ±ê¹ßÀ» ¾ò¾î¿Â´Ù.	
	if(uiGuildID != 0)		siFlag			=	pMyWorld->clIGuild.GetFlag(uiGuildID);		
	else					siFlag			=	0;

	
	//Áö±ÝÀÌ °ø¼º ½Ã°£ÀÎÁö Ã¼Å©
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

	// ¿ëº´µéÀÇ Á¤º¸¸¦ Ã¤¿ö³Ö´Â´Ù.
	IFollower.Init();
		
	for(i = 0; i < ON_MAX_FOLLOWER_NUM; i++)
	{
		pMercenaryInfoInDB	=	&logininfo->mercenaryInfos[i];
		pItemInfo			=	(ItemInfo*)&logininfo->mercenaryItemInfos[i];

		if(pMercenaryInfoInDB->kind != 0)
		{	
			// À¯È¿ÇÑ ¿ëº´ÀÌ´Ù.
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

			// ¿ëº´À» ÃÊ±âÈ­ ÇÑ´Ù.
			IFollower.InitFollower(pMercenaryInfoInDB->slot, &TempFollowerInfo, lpCharHead->siNation, &pMyWorld->clItem);


			// ¿ëº´µéÀÇ ¾ÆÀÌÅÛÀ» Ã¤¿ö³Ö´Â´Ù.
			for(j = 0; j < ON_MYDATA_MAX_ITEM_NUM; j++)
			{
				if(pItemInfo[j].itemID != 0)
				{
					// À¯È¿ÇÑ ¾ÆÀÌÅÛÀÌ´Ù.
					ItemData.uiID				=	pItemInfo[j].itemID;
					ItemData.siPosInInventory	=	pItemInfo[j].posInInventory;
					ItemData.uiQuantity			=	pItemInfo[j].quantity;
		
					IFollower.AddItem(pMercenaryInfoInDB->slot, &ItemData);
				}
			}
			pFollower		=	IFollower.GetFollower(pMercenaryInfoInDB->slot);		

			// ¿ëº´µéÀÌ ÀåÂøÇÏ°í ÀÖ´Â ¾ÆÀÌÅÛµéÀ» Ã¤¿ö³Ö´Â´Ù.
			SetEquipItems(&logininfo->mercenaryEquipItemInfos[pMercenaryInfoInDB->slot],pMercenaryInfoInDB->slot,siTimerItemUseCount,pFollower);

			// ¿ëº´µéÀÇ ´É·ÂÄ¡¸¦ °áÁ¤ÇÑ´Ù.
			pFollower->ParametaBox.Action(&pMyWorld->clItem, pFollower->Weapon);
			
			// Äù½ºÆ®¸¦ ÀúÀåÇÑ´Ù.
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

	// ¹èÇ¥ Á¤º¸¸¦ ¾ò¾î¿Â´Ù.
	stTicketInfo.siTicketKind = logininfo->charBaseInfo.ticketKind;

	siDeleteItemCount = GetClientSendDeleteTimerItem(m_pTimerItemInfo);
	if(siDeleteItemCount > 0)				m_bCompareWearItem = TRUE;

	// DB·ÎºÎÅÍ ¾ò¾î¿Â Ä£±¸ ¸®½ºÆ®¸¦ ¼ÂÆÃÇÑ´Ù.
	m_clFriendList.DeleteAllFriend();
	while( strcmp(logininfo->friendInfos[siFriendCount].szID, "") != 0 && siFriendCount < MAX_FRIEND_NUM )
	{
		BOOL bLogin	= pMyWorld->GetPlayer( logininfo->friendInfos[siFriendCount].szID ) == NULL ? FALSE : TRUE;
		m_clFriendList.AddFriend( this->szID, logininfo->friendInfos[siFriendCount].szID, bLogin );
		siFriendCount++;
	}

	// ÀúÀÚ°Å¸®¿¡ ÀÖ´Ù¸é ¸¶À»¿¡ ¼ÓÇØÀÖ´Â °ÍÀ¸·Î ¼³Á¤ÇÑ´Ù.
	stAccountInfo.pVillage		=	pMyWorld->GetVillageByCode( pMyWorld->clISOMap.GetVillageCode(uiMapID) );	
	if( stAccountInfo.pVillage != NULL)
		EnterVillage( TRUE );

	
	// ÇöÀç Ä³¸¯ÅÍ¸¦ ¸Ê¿¡ ¼³Á¤ÇÒ ¼ö ÀÖ´ÂÁö ¾Ë¾Æ¿Â´Ù.
	// È¤½Ã ´Ù¸¥ Ä³¸¯ÅÍ°¡°¡ Çö À§Ä¡¿¡ ³õÀ» ¼ö ¾ø´Â »óÈ²ÀÎÁö...
	// ÇöÀç À§Ä¡¸¦ ±âÁØÀ¸·Î ¸Ê¿¡ ½ÉÀ» ÁÂÇ¥¸¦ ¾ò¾î¿Â´Ù.
	ptPlayerPos	= pMyWorld->clISOMap.SearchMap( logininfo->charBaseInfo.mapID, 
											    logininfo->charBaseInfo.x_pos, 
											    logininfo->charBaseInfo.y_pos, 
											    ON_MAP_SI_CHAR, uiMoveableTileInfo);	

	// ³ª¿Ã ÀÚ¸®°¡ ¾ø´Ù¸é ·Î±×ÀÎ½Ã ½ÇÆÐÇÑ°ÍÀÌ´Ù.
	if((ptPlayerPos.x == -1) && (ptPlayerPos.y == -1))					goto	END;	

	// À§Ä¡¸¦ ´Ù½Ã ¼öÁ¤ÇÑ´Ù.
	siX							=		ptPlayerPos.x;
	siY							=		ptPlayerPos.y;			

	m_latestSavedMapID			=		uiMapID;
	m_latestSavedPos.x			=		siX;
	m_latestSavedPos.y			=		siY;
	

	// ¸Ê¿¡´Ù°¡ ÀÚ±âÀÇ ¾ÆÀÌµð¸¦ ¼³Á¤ÇÑ´Ù.
	if( SetIDOnMap( 7 ) == FALSE )										goto	END;

	// Á¤½Ä »ó´Ü¿øÀÏ °æ¿ì »ó´Ü °ü¸®ÀÚ¿¡°Ô ·Î±×ÀÎ ÇÏ¿´´Ù°í ¾Ë·ÁÁØ´Ù.
	if( ( uiGuildID != 0 ) && ( siClassInGuild != pMyWorld->clIGuildClass.GetJWJCode() ) )
	{
		pMyWorld->clIGuild.GuildMemberLogin( uiGuildID );
	}


	// ·Î±×ÀÎÀ» ¼º°øÇß´Ù
	bSuccessLogin				=		TRUE;

END:
	// ·Î±×ÀÎ Á¤º¸¸¦ º¸³»ÁØ´Ù.
	if(bSuccessLogin == TRUE)
	{
		m_lastTick = GetTickCount();
		
		// ·Î±×ÀÎÀÌ ¼º°øÇß´Ù°í º¸³»ÁØ´Ù.
		SendLogIn(TRUE);

		if( m_uiRecvItemNum > 0 ) SendItemCount();

		// ÀÚ½ÅÀÇ Äù½ºÆ® Data¸¦ Àü¼ÛÇÑ´Ù.
		SendFieldMsg(ON_RESPONSE_QUEST_GETPLAYQUEST, SO_SFM_ONLYME, LPVOID(NULL), LPVOID(ON_QUSETDATA_MAXPLAYINGQUEST), LPVOID(ON_QUSETDATA_MAXCOMLETEDQUEST), LPVOID(stQuestInfo.m_sPlayingQuest), LPVOID(stQuestInfo.m_siCompletedQuest));

		// ³¯¾¾ Á¤º¸¸¦ º¸³»ÁØ´Ù.
		SendFieldMsg(ON_CHANGE_WEATHER, SO_SFM_ONLYME, LPVOID(pMyWorld->bNowWeather));

		SendFieldMsg(ON_RESOPNSE_REAL_TIME, SO_SFM_ONLYME, LPVOID(pMyWorld->pRealTime->tm_year + 1900),LPVOID(pMyWorld->pRealTime->tm_mon + 1),LPVOID(pMyWorld->pRealTime->tm_mday));

		// Ä£±¸ Á¤º¸¸¦ º¸³»ÁØ´Ù.
		RecvAuditFriendList();

		// ÀÌ ÇÃ·¹ÀÌ¾î¸¦ Ä£±¸·Î µî·ÏÇÑ »ç¶÷µé¿¡°Ô ÀÌ ÇÃ·¹ÀÌ¾î°¡ ·Î±×ÀÎÇßÀ½À» ¾Ë¸°´Ù.
		NotifyFriendsOfLoginState( TRUE );
		
		//°ø¼ºÇÏ´Â ³¯ÀÌ°í Áö±ÝÀÌ °ø¼º ½Ã°£ÀÌ¸é 
		if(pMyWorld->m_WarTimeParser.IsWarCheck(pMyWorld->pRealTime->tm_wday, pMyWorld->pRealTime->tm_hour))
//		if(pMyWorld->m_WarTimeParser.IsWarTime(pMyWorld->pRealTime->tm_hour))	
		{
			cltGuild *pcltGuild = NULL;
			pcltGuild = pMyWorld->clIGuild.GetGuildByID(uiGuildID);
			if(pcltGuild != NULL)
			{
				int nGiveUpFlag = pcltGuild->GetGiveUpFlag();
				if( pcltGuild->GetgiveUpVillageCode() < 0 ) return;
				
				//´ë¹æÀÎ°¡
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
						
						//°ø¼º À¯´ÖÀÎ °æ¿ì¸¸ ¿ø»óº¹±Í ½ÃÅ²´Ù.
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



//¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬
//	Name :	
//	Desc :	·Î±×ÀÎ ¿äÃ»À» Ã³¸®ÇÑ´Ù 
//¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬
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
	

	// Ä³¸¯ÅÍ ½½·Ô(1 ~ 3)ÀÌ ¿Ã¹Ù¸¥°¡?
	if( pGameServerLogInMsg->uiCharSlot < 1 || pGameServerLogInMsg->uiCharSlot > 3 ) 
	{
		writeInfoToFile( "LoginError.txt", "Invalid CharacterSlot(%d) has been detected", pGameServerLogInMsg->uiCharSlot );
		return;
	}

	// DB¿¡¼­ ·Î±×ÀÎ Á¤º¸¸¦ ¾ò¾î¿Â´Ù.
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


	// ±¸ÀÔÇÑ ¾ÆÀÌÅÛ °³¼ö¸¦ ¾ò¾î¿Â´Ù
	m_uiRecvItemNum = 0;
	
	DBIKPlayerDBInfo playerinfo;

	playerinfo.DBAccount = logininfo->charBaseInfo.account;

	if( pMyWorld->clItemDatabase->GetDBInfo2( SO_DB_INFOKIND_GETITEMCOUNT_ITEMBANK, &playerinfo, &m_uiRecvItemNum ) == FALSE ) {
		m_uiRecvItemNum = 0;
	}

	if( m_uiRecvItemNum > 0 ) {
		// ¾ÆÀÌÅÛ ¸®½ºÆ®¸¦ ¾ò¾î¿Â´Ù.

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

//¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬
//	¼³¸í		:	
//	¼öÁ¤ ÀÏÀÚ	:	2002-02-26 ¿ÀÈÄ 5:09:28 - ¾çÁ¤¸ð
//¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬
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
		// ·Î±×ÀÎ¿¡ ¼º°øÇß´Ù.
		uiState			=	ON_STATE_LOGIN;				

		// ¼­¹öÀÇ ½Ã°£À» º¸³»ÁØ´Ù.
		pGameServerLogInMsgData->Time			=	pMyWorld->CurrentTime;		

		// ½ÇÁ¦ ¼¼°èÀÇ ½Ã°£À» ¾ò¾î¿Â´Ù.
		pGameServerLogInMsgData->RealTime		=	pMyWorld->RealWorldTime;			


//		RecvAuditTotalInfo();		

		// ÁÖÀ§¿¡ Ä³¸¯ÅÍ¿Í ¾ÆÀÌÅÛÀÇ Á¤º¸¸¦ ¾ò¾î¿Â´Ù.
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

		// Ä³¸¯ÅÍÀÇ Á¤º¸¸¦ ¾ò¾î¿Â´Ù.		
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



		// ÀÚ±âÀÇ ÀÌ¸§À» ¾ò¾î¿Â´Ù.
		ZeroMemory(pGameServerLogInMsgData->szID, sizeof(pGameServerLogInMsgData->szID));
		strncpy(pGameServerLogInMsgData->szID, szID, ON_ID_LENGTH);
		
		pGameServerLogInMsgData->uiDokdoQuestCompletedUserCount = pMyWorld->m_uiDokdoQuestCompletedUserCount;

		// »ó´ÜÀÌ À¯È¿ÇÑÁö °Ë»çÇÑ´Ù.
		if( uiGuildID != 0 )
		{
			pszGuildName	=	pMyWorld->clIGuild.GetGuildName(uiGuildID);

			if(pszGuildName != NULL)			
			{
				// »ó´ÜÀÌ À¯È¿ÇÏ´Ù.
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

				// »ó´ÜÀÌ À¯È¿ÇÏÁö ¾Ê´Ù.
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

		// ÇÊµåÀÇ ¾ÆÀÌÅÛÀ» Ã¤¿ö ³Ö´Â´Ù.
		pSendFieldItemInfo	=	(OnlineReceiveFieldItemInfo*)((CHAR*)pSendPlayerInfo + uiPlayersInfoMsgLength);	
		memcpy(pSendFieldItemInfo, FieldItemInfo, sizeof(OnlineReceiveFieldItemInfo) * pGameServerLogInMsgData->uiFieldItemNum);		

		// ¿ëº´µéÀÇ Á¤º¸¸¦ ¾ò¾î¿Â´Ù.
		pSendFollowerInfo	=	(OnFollowerInfo*)((CHAR*)pSendFieldItemInfo + sizeof(OnlineReceiveFieldItemInfo) * pGameServerLogInMsgData->uiFieldItemNum);
		siFollowerSendSize	=	GetAllFollowerInfo(pSendFollowerInfo, &pGameServerLogInMsgData->uiFollowerNum);

		// º¸³»¾ß ÇÏ´Â »çÀÌÁî¸¦ °áÁ¤ÇÑ´Ù.
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



//¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬
// ·Î±×¾Æ¿ôÇÑ´Ù.
// ¼öÁ¤ ÀÏÀÚ : 2002-02-19 ¿ÀÀü 11:36:02 - ¾çÁ¤¸ð
//¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬
VOID SOPlayer::LogOut(VOID)
{
	printf("LogOut..\n");

	if( uiState != ON_STATE_LOGOUT )
	{
		// ·Î±× ¾Æ¿ô ¾×¼Ç!
		m_pXPlayer->SetAction();
	}	

}


//¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬
// ÀÚ½ÅÀ» Ä£±¸·Î µî·ÏÇÑ »ç¶÷µé¿¡°Ô ÀÚ½ÅÀÌ ·Î±×ÀÎ/·Î±×¾Æ¿ô ÇßÀ½À» ¾Ë¸°´Ù.
// ¼öÁ¤ ÀÏÀÚ : 2003-04-21 ¿ÀÀü 12:02 - ¹é»óÇö
//¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬
VOID SOPlayer::NotifyFriendsOfLoginState( BOOL bLogin )
{
	for( int i = 1; i < ON_MAX_CONNECTION + 1; i++ )
	{
		if( pMyWorld->pPlayerOnAccount[i] != NULL )
		{			
			if( pMyWorld->pPlayerOnAccount[i]->m_clFriendList.GetFriend(szID) != NULL )		// NULLÀÌ ¾Æ´Ï¸é ±× Ä£±¸ÀÇ ¸®½ºÆ®¿¡ ³»°¡ ÀÖ´Ù.
			{
				// ±× Ä£±¸ÀÇ ¸®½ºÆ®¿¡¼­ ³ª¸¦ ·Î±×ÀÎ/·Î±×¾Æ¿ô »óÅÂ·Î ¸¸µç´Ù.
				pMyWorld->pPlayerOnAccount[i]->m_clFriendList.SetLogin( szID, bLogin );
				// ±× Ä£±¸¿¡°Ô ·Î±×ÀÎ/¾Æ¿ô ÇßÀ½À» ¾Ë¸®´Â ¸Þ½ÃÁö¸¦ º¸³»ÁØ´Ù.
				pMyWorld->pPlayerOnAccount[i]->SendFieldMsg(ON_FRIEND_LOGGEDINOUT, SO_SFM_ONLYME, LPVOID(bLogin), LPVOID(szID));
			}
		}
	}
}