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


//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	Name :	
//	Desc :	������ �α����� ó���Ѵ� 
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
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
	
	// ���� �ڽ��� ������ ������ �ִٸ� �α׾ƿ� ��Ų��.
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

	// �⺻ ���� ������� ������ ä���ִ´�.
	Init(uiAccount, logininfo->charBaseInfo.mapID, logininfo->charBaseInfo.x_pos, logininfo->charBaseInfo.y_pos, logininfo->charBaseInfo.kind, logininfo->charBaseInfo.name);	

	stAccountInfo.siDBAccount				=	logininfo->charBaseInfo.account;
	siMoney									=	logininfo->charBaseInfo.money;	  // ��.
	m_MoneySaving                           =	logininfo->charBaseInfo.saving;   // �����
	investProfit							=   logininfo->charBaseInfo.investProfit; 										// ���� ����
	additionProfit							=   logininfo->charBaseInfo.additionProfit;										// �ΰ� ���� 		

	uiGuildID								=	logininfo->charBaseInfo.guildID;											// Guild ID.
	siClassInGuild							=	logininfo->charBaseInfo.guildGrade;											// Guild Class.
	
	m_satiety								=	logininfo->charBaseInfo.satiety;	// ������.		
	m_orgSatiety							=	m_satiety;

	uiTradeCredit							=	logininfo->charBaseInfo.credit_trade;										// ���� �ſ뵵.
	siDayCredit								=	logininfo->charBaseInfo.credit_daily;										// ���� �Ϸ翡 ���� �ſ뵵.		
	siNearestHospitalVillageIndexInDungeon	=	pMyWorld->GetVillageIndexByCode(logininfo->charBaseInfo.nearVillage);		// �׾����� ��ó�� ���� ����.
	USGrade									=	UserGrade(logininfo->charBaseInfo.userGrade);														

	siVMercenarySlot						=	logininfo->charBaseInfo.VMercenary;
	m_siMaxGeneral							=	logininfo->charBaseInfo.maxGeneral;											// ��� ������ �ִ� �����

	m_dwCondLaborTime						=	0;
	
	// ����� ����� ���´�.	
	if(uiGuildID != 0)		siFlag			=	pMyWorld->clIGuild.GetFlag(uiGuildID);		
	else					siFlag			=	0;

	
	//������ ���� �ð����� üũ
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

	// �뺴���� ������ ä���ִ´�.
	IFollower.Init();
		
	for(i = 0; i < ON_MAX_FOLLOWER_NUM; i++)
	{
		pMercenaryInfoInDB	=	&logininfo->mercenaryInfos[i];
		pItemInfo			=	(ItemInfo*)&logininfo->mercenaryItemInfos[i];

		if(pMercenaryInfoInDB->kind != 0)
		{	
			// ��ȿ�� �뺴�̴�.
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

			// �뺴�� �ʱ�ȭ �Ѵ�.
			IFollower.InitFollower(pMercenaryInfoInDB->slot, &TempFollowerInfo, lpCharHead->siNation, &pMyWorld->clItem);


			// �뺴���� �������� ä���ִ´�.
			for(j = 0; j < ON_MYDATA_MAX_ITEM_NUM; j++)
			{
				if(pItemInfo[j].itemID != 0)
				{
					// ��ȿ�� �������̴�.
					ItemData.uiID				=	pItemInfo[j].itemID;
					ItemData.siPosInInventory	=	pItemInfo[j].posInInventory;
					ItemData.uiQuantity			=	pItemInfo[j].quantity;
		
					IFollower.AddItem(pMercenaryInfoInDB->slot, &ItemData);
				}
			}
			pFollower		=	IFollower.GetFollower(pMercenaryInfoInDB->slot);		

			// �뺴���� �����ϰ� �ִ� �����۵��� ä���ִ´�.
			SetEquipItems(&logininfo->mercenaryEquipItemInfos[pMercenaryInfoInDB->slot],pMercenaryInfoInDB->slot,siTimerItemUseCount,pFollower);

			// �뺴���� �ɷ�ġ�� �����Ѵ�.
			pFollower->ParametaBox.Action(&pMyWorld->clItem, pFollower->Weapon);
			
			// ����Ʈ�� �����Ѵ�.
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

	// ��ǥ ������ ���´�.
	stTicketInfo.siTicketKind = logininfo->charBaseInfo.ticketKind;

	siDeleteItemCount = GetClientSendDeleteTimerItem(m_pTimerItemInfo);
	if(siDeleteItemCount > 0)				m_bCompareWearItem = TRUE;

	// DB�κ��� ���� ģ�� ����Ʈ�� �����Ѵ�.
	m_clFriendList.DeleteAllFriend();
	while( strcmp(logininfo->friendInfos[siFriendCount].szID, "") != 0 && siFriendCount < MAX_FRIEND_NUM )
	{
		BOOL bLogin	= pMyWorld->GetPlayer( logininfo->friendInfos[siFriendCount].szID ) == NULL ? FALSE : TRUE;
		m_clFriendList.AddFriend( this->szID, logininfo->friendInfos[siFriendCount].szID, bLogin );
		siFriendCount++;
	}

	// ���ڰŸ��� �ִٸ� ������ �����ִ� ������ �����Ѵ�.
	stAccountInfo.pVillage		=	pMyWorld->GetVillageByCode( pMyWorld->clISOMap.GetVillageCode(uiMapID) );	
	if( stAccountInfo.pVillage != NULL)
		EnterVillage( TRUE );

	
	// ���� ĳ���͸� �ʿ� ������ �� �ִ��� �˾ƿ´�.
	// Ȥ�� �ٸ� ĳ���Ͱ��� �� ��ġ�� ���� �� ���� ��Ȳ����...
	// ���� ��ġ�� �������� �ʿ� ���� ��ǥ�� ���´�.
	ptPlayerPos	= pMyWorld->clISOMap.SearchMap( logininfo->charBaseInfo.mapID, 
											    logininfo->charBaseInfo.x_pos, 
											    logininfo->charBaseInfo.y_pos, 
											    ON_MAP_SI_CHAR, uiMoveableTileInfo);	

	// ���� �ڸ��� ���ٸ� �α��ν� �����Ѱ��̴�.
	if((ptPlayerPos.x == -1) && (ptPlayerPos.y == -1))					goto	END;	

	// ��ġ�� �ٽ� �����Ѵ�.
	siX							=		ptPlayerPos.x;
	siY							=		ptPlayerPos.y;			

	m_latestSavedMapID			=		uiMapID;
	m_latestSavedPos.x			=		siX;
	m_latestSavedPos.y			=		siY;
	

	// �ʿ��ٰ� �ڱ��� ���̵� �����Ѵ�.
	if( SetIDOnMap( 7 ) == FALSE )										goto	END;

	// ���� ��ܿ��� ��� ��� �����ڿ��� �α��� �Ͽ��ٰ� �˷��ش�.
	if( ( uiGuildID != 0 ) && ( siClassInGuild != pMyWorld->clIGuildClass.GetJWJCode() ) )
	{
		pMyWorld->clIGuild.GuildMemberLogin( uiGuildID );
	}


	// �α����� �����ߴ�
	bSuccessLogin				=		TRUE;

END:
	// �α��� ������ �����ش�.
	if(bSuccessLogin == TRUE)
	{
		m_lastTick = GetTickCount();
		
		// �α����� �����ߴٰ� �����ش�.
		SendLogIn(TRUE);

		if( m_uiRecvItemNum > 0 ) SendItemCount();

		// �ڽ��� ����Ʈ Data�� �����Ѵ�.
		SendFieldMsg(ON_RESPONSE_QUEST_GETPLAYQUEST, SO_SFM_ONLYME, LPVOID(NULL), LPVOID(ON_QUSETDATA_MAXPLAYINGQUEST), LPVOID(ON_QUSETDATA_MAXCOMLETEDQUEST), LPVOID(stQuestInfo.m_sPlayingQuest), LPVOID(stQuestInfo.m_siCompletedQuest));

		// ���� ������ �����ش�.
		SendFieldMsg(ON_CHANGE_WEATHER, SO_SFM_ONLYME, LPVOID(pMyWorld->bNowWeather));

		SendFieldMsg(ON_RESOPNSE_REAL_TIME, SO_SFM_ONLYME, LPVOID(pMyWorld->pRealTime->tm_year + 1900),LPVOID(pMyWorld->pRealTime->tm_mon + 1),LPVOID(pMyWorld->pRealTime->tm_mday));

		// ģ�� ������ �����ش�.
		RecvAuditFriendList();

		// �� �÷��̾ ģ���� ����� ����鿡�� �� �÷��̾ �α��������� �˸���.
		NotifyFriendsOfLoginState( TRUE );
		
		//�����ϴ� ���̰� ������ ���� �ð��̸� 
		if(pMyWorld->m_WarTimeParser.IsWarCheck(pMyWorld->pRealTime->tm_wday, pMyWorld->pRealTime->tm_hour))
//		if(pMyWorld->m_WarTimeParser.IsWarTime(pMyWorld->pRealTime->tm_hour))	
		{
			cltGuild *pcltGuild = NULL;
			pcltGuild = pMyWorld->clIGuild.GetGuildByID(uiGuildID);
			if(pcltGuild != NULL)
			{
				int nGiveUpFlag = pcltGuild->GetGiveUpFlag();
				if( pcltGuild->GetgiveUpVillageCode() < 0 ) return;
				
				//����ΰ�
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
						
						//���� ������ ��츸 ���󺹱� ��Ų��.
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



//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	Name :	
//	Desc :	�α��� ��û�� ó���Ѵ� 
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
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
	

	// ĳ���� ����(1 ~ 3)�� �ùٸ���?
	if( pGameServerLogInMsg->uiCharSlot < 1 || pGameServerLogInMsg->uiCharSlot > 3 ) 
	{
		writeInfoToFile( "LoginError.txt", "Invalid CharacterSlot(%d) has been detected", pGameServerLogInMsg->uiCharSlot );
		return;
	}

	// DB���� �α��� ������ ���´�.
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


	// ������ ������ ������ ���´�
	m_uiRecvItemNum = 0;
	
	DBIKPlayerDBInfo playerinfo;

	playerinfo.DBAccount = logininfo->charBaseInfo.account;

	if( pMyWorld->clItemDatabase->GetDBInfo2( SO_DB_INFOKIND_GETITEMCOUNT_ITEMBANK, &playerinfo, &m_uiRecvItemNum ) == FALSE ) {
		m_uiRecvItemNum = 0;
	}

	if( m_uiRecvItemNum > 0 ) {
		// ������ ����Ʈ�� ���´�.

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

//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	
//	���� ����	:	2002-02-26 ���� 5:09:28 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
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
		// �α��ο� �����ߴ�.
		uiState			=	ON_STATE_LOGIN;				

		// ������ �ð��� �����ش�.
		pGameServerLogInMsgData->Time			=	pMyWorld->CurrentTime;		

		// ���� ������ �ð��� ���´�.
		pGameServerLogInMsgData->RealTime		=	pMyWorld->RealWorldTime;			


//		RecvAuditTotalInfo();		

		// ������ ĳ���Ϳ� �������� ������ ���´�.
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

		// ĳ������ ������ ���´�.		
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



		// �ڱ��� �̸��� ���´�.
		ZeroMemory(pGameServerLogInMsgData->szID, sizeof(pGameServerLogInMsgData->szID));
		strncpy(pGameServerLogInMsgData->szID, szID, ON_ID_LENGTH);
		
		pGameServerLogInMsgData->uiDokdoQuestCompletedUserCount = pMyWorld->m_uiDokdoQuestCompletedUserCount;

		// ����� ��ȿ���� �˻��Ѵ�.
		if( uiGuildID != 0 )
		{
			pszGuildName	=	pMyWorld->clIGuild.GetGuildName(uiGuildID);

			if(pszGuildName != NULL)			
			{
				// ����� ��ȿ�ϴ�.
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

				// ����� ��ȿ���� �ʴ�.
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

		// �ʵ��� �������� ä�� �ִ´�.
		pSendFieldItemInfo	=	(OnlineReceiveFieldItemInfo*)((CHAR*)pSendPlayerInfo + uiPlayersInfoMsgLength);	
		memcpy(pSendFieldItemInfo, FieldItemInfo, sizeof(OnlineReceiveFieldItemInfo) * pGameServerLogInMsgData->uiFieldItemNum);		

		// �뺴���� ������ ���´�.
		pSendFollowerInfo	=	(OnFollowerInfo*)((CHAR*)pSendFieldItemInfo + sizeof(OnlineReceiveFieldItemInfo) * pGameServerLogInMsgData->uiFieldItemNum);
		siFollowerSendSize	=	GetAllFollowerInfo(pSendFollowerInfo, &pGameServerLogInMsgData->uiFollowerNum);

		// ������ �ϴ� ����� �����Ѵ�.
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



//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
// �α׾ƿ��Ѵ�.
// ���� ���� : 2002-02-19 ���� 11:36:02 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
VOID SOPlayer::LogOut(VOID)
{
	printf("LogOut..\n");

	if( uiState != ON_STATE_LOGOUT )
	{
		// �α� �ƿ� �׼�!
		m_pXPlayer->SetAction();
	}	

}


//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
// �ڽ��� ģ���� ����� ����鿡�� �ڽ��� �α���/�α׾ƿ� ������ �˸���.
// ���� ���� : 2003-04-21 ���� 12:02 - �����
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
VOID SOPlayer::NotifyFriendsOfLoginState( BOOL bLogin )
{
	for( int i = 1; i < ON_MAX_CONNECTION + 1; i++ )
	{
		if( pMyWorld->pPlayerOnAccount[i] != NULL )
		{			
			if( pMyWorld->pPlayerOnAccount[i]->m_clFriendList.GetFriend(szID) != NULL )		// NULL�� �ƴϸ� �� ģ���� ����Ʈ�� ���� �ִ�.
			{
				// �� ģ���� ����Ʈ���� ���� �α���/�α׾ƿ� ���·� �����.
				pMyWorld->pPlayerOnAccount[i]->m_clFriendList.SetLogin( szID, bLogin );
				// �� ģ������ �α���/�ƿ� ������ �˸��� �޽����� �����ش�.
				pMyWorld->pPlayerOnAccount[i]->SendFieldMsg(ON_FRIEND_LOGGEDINOUT, SO_SFM_ONLYME, LPVOID(bLogin), LPVOID(szID));
			}
		}
	}
}