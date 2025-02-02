// 熱薑 橾濠 : 2002-02-14 螃�� 2:24:41 - 曄薑賅

#include "SOMain.h"
#include "SODefine.h"
#include "SOWorld.h"
#include "SOPlayer.h"
#include "SODatabase.h"
#include "XPlayer.h"
#include "XGateServerMgr.h"
#include "..\\XCommon\\XPacketCmd.h"
#include "Skill.h"
#include "SKillMgr.h"


#include "AuctionList.h"
#include "AuctionMarket.h"
#include "AuctionMarketProc.h"



#ifdef _GAMEGUARD
#include "CSAuth.h"
#endif

#include "..\\JWCommon\\JWSocket.h"
char cMsg2[ 80000 ];

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
// 儅撩濠
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
SOPlayer::SOPlayer() : BaseChar()
{
	clServer	= NULL;	

	dwPrevSavePosTime	=	0;

	// 陝謙 滲熱菟曖 蟾晦��
	InitEnv();

	uiState							=	ON_STATE_LOGOUT;
	uiStructerInPos                 =   STRUCTERPOS_NONE;

	m_pXPlayer						=	NULL;

	clPrivateMarket.Init(this);
	
	logininfo				= new LoginInfo;
	m_pAuctionMarketProc	= new CAuctionMarketProc(this);	

	m_pCSAuth				= NULL;

#ifdef _GAMEGUARD
	m_pCSAuth				= new CCSAuth();
	ASSERT(m_pCSAuth);

	m_pCSAuth->Init();
#endif
	m_bCheckPass			= TRUE;

	cMsg = &pmsg[ 3 ];
}



//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
// 模資濠
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
SOPlayer::~SOPlayer()
{
#ifdef _GAMEGUARD
	if(m_pCSAuth != NULL ) delete m_pCSAuth;
#endif
	delete		logininfo;
	delete		m_pAuctionMarketProc;

	return;
}


//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
VOID SOPlayer::InitEnv()
{
	SI32 i = ON_MAX_PLAYERHAVEITEM;

	// 滲熱菟擊 蟾晦�� и棻.
	BaseChar::Init();
			
	ZeroMemory(&stAccountInfo, sizeof(stAccountInfo));


	USGrade							=	ON_USERGRADE_NORMAL;
	
	SETSTATUS( siStatus, ON_PLAYER_STATUS_INFIELD );

	uiStructerInPos                 =   STRUCTERPOS_NONE;	

	siDayCredit						=	0;
	uiGuildID						=	0;
	m_bFieldAlive					=	TRUE;
	m_siFieldHp						=   0; 
	m_siFieldMaxHP                  =   0;

	m_siFieldMP                     =   0;
	m_siFieldMaxMP                  =   0;
	
	m_bCaptainUnit					=   FALSE;
//	m_siSupplyCharge                =   0;
	m_uiPrevKind					=	0;
	m_dwDeadSec						=	0;

	dwPrevSavePosTime						=	timeGetTime();
	bSuccessLogin							=	FALSE;
	bGuildInfoChanged						=	TRUE;
	uiBoardShipAccount						=	0;
	siNearestHospitalVillageIndexInDungeon	=	-1;

	for( int j=0; i<ON_HEARTBEAT_SAVE_NUM; ++j )
	{
		dwHeartbeatTime[j]	=	0;
	}
	
	IFollower.Init();	
	clPrivateMarket.Init();
	clTrade.Init();


	//----------------------------------------
	m_satiety			  =	0;
	m_orgSatiety		  = 0;

	m_tempBiddingPrice    = 0;

	m_latestSavedMapID	  = 0;
	m_latestSavedPos.x    = 0;
	m_latestSavedPos.y    = 0;
	m_latestSavedPosTime  = 0;
	m_bCheckPass		  = TRUE;
	m_lastGameGuardCheckTime = 0;
	m_bInvestmentPower	  = FALSE;
	m_bIsWarUnit          = FALSE;
	m_dwAttackDelay       = 0;
	m_siKillNum           = 0;
	//----------------------------------------

	m_bIsFirstDefence = FALSE;
	m_dwChargeTime    = 0;
	m_dwDefenceTime   = 0;
		
	m_SpeedChecker.Init( 30000, 600000 );
	m_bIsSpeedHackUser		=	FALSE;


	m_mainState.set( NONE_STATE );

	//0衛縑 衛除 羹觼蒂 ж晦嬪и Ы楚斜.(衛除 嬴檜蠱)
	m_bDayChange			 = FALSE;
	m_bFieldUnitUpdate		 = FALSE;
	m_bPlayerVillageInLimit  = FALSE;
//	ZeroMemory(&m_sWearItem,sizeof(stWearItem)*12);

	m_dwUserhour = rand()%2;
	m_dwUserMin	 = rand()%50 + 1;//pMyWorld->pRealTime->tm_min;
	m_dwUserSec	 = rand()%50 + 1;//pMyWorld->pRealTime->tm_sec;

	ZeroMemory(m_stTimerItemInfo,sizeof(OnTimerItemInfo)*12*9);
	ZeroMemory(m_pTimerItemInfo,sizeof(OnTimerItemInfo)*12);

	//だじ 蝶溯檜お 蹺陛
	m_PartyState.Init();
}



//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
// 億煎 菟橫螞 Ы溯檜橫曖 模鰍擊 撲薑и棻.
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
VOID SOPlayer::Init()
{
	// 滲熱菟擊 蟾晦�倆挬�.
	InitEnv();

	// Ы溯檜橫曖 模鰍
	
	stAccountInfo.dwIP	=	0;
	dwHeartbeatTime[0]	=	pMyWorld->dwtimeGetTime;

	return;
}


//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
// 歎褐 幗ぷ縑 等檜顫蒂 噹棻.
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
SI32 SOPlayer::Write(const VOID *pMsg, SI32 iBytes)
{	
	if( iBytes < 0 || iBytes + 15 > JXPACKET_MAX_SIZE ) {
		LogOut();
		return 0;
	}
	
	cCheckSum = 0;

/*
	for( int i = 0; i < iBytes; ++i ) {
		cCheckSum ^=  *((char *)pMsg + i );
	}
*/
	usSize = iBytes + 3;
	
	memcpy( &cMsg2[ 3 ], pMsg, iBytes );
	cMsg2[ 2 ] = 0;
	memcpy( &cMsg2[ 0 ], &usSize, 2 );

	m_pXPlayer->Write( cMsg2 );

	return 1;
}



//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
// Ы溯檜橫蒂 儅撩и棻.
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
VOID	SOPlayer::Create( SOServer *clTempServer )
{
	// Ы溯檜橫陛 儅撩腎歷棻.( 蕾樓檜 檜瑞橫 雖棻 )

	uiState = ON_STATE_CONNECT;

	// 贗溯蝶 翱唸
	clServer	= clTempServer;	

	clServer->clWorld->pPlayerOnAccount[ uiAccount ] = this;

	clearCharsInfo();

	memcpy( stAccountInfo.IP, inet_ntoa( ((JWSocket *)(m_pXPlayer->GetFriend()))->m_sockaddr.sin_addr ), 16 );
	stAccountInfo.IP[ 15 ] = 0;

	siDayCredit		= 0;
	m_siFieldHp     = 0; 
	m_bCaptainUnit  = FALSE;
//	m_siSupplyCharge= 0;
	m_uiPrevKind	= 0;
	m_dwDeadSec		= 0;
	m_bFieldAlive	= TRUE;
	m_bCheckPass	= TRUE;

	InterlockedExchange( (long *)&bProcessLogin, FALSE );

}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
// 餌辨濠縑 渠и 賅萇 翱骯 籀葬
//	輿曖 : <詭撮雖 嫡晦 -> 籀葬 -> 詭撮雖 爾頂晦> 牖縑 評塭...
//	熱薑 橾濠	:	2002-08-05 螃�� 2:09:01 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
VOID SOPlayer::Poll()
{

	if( m_mainState.match( USER_IN_GAME ) )
	{

		if( bProcessLogin ) {
			ProcessLogin();
			return;
		}

		DWORD				dwRead = 0;	
		DBIKPlayerDBInfo	PlayerInfo;	
		MONEY				AddMoney = 0;
		OnlineFollower		*pFollower;		

		// 衛除 (HOUR)
		if(pMyWorld->bHourChanged == TRUE)
		{		
			// 憮幗曖 衛除檜 滲唳腎歷棻賊 衛除擊 爾頂遽棻		
			SendFieldMsg(ON_CHANGED_TIME, SO_SFM_ONLYME);
			
			// 陳 (DAY)
			if(pMyWorld->bDayChanged == TRUE)		
			{
				// 喇 (YEAR)
				if(pMyWorld->bYearChanged == TRUE)
				{				
					// ⑷營 蕾樓腎橫 氈朝 Ы溯檜橫朝 DB縑憮 翱除 熱櫛擊 橢橫諦憮 爾頂遽棻.
					PlayerInfo.DBAccount	=	stAccountInfo.siDBAccount;
					PlayerInfo.uiSlot		=	stAccountInfo.uiSlot;
					
					pMyWorld->clDatabase->GetDBInfo(SO_DB_INFOKIND_GETADDMONEY, &PlayerInfo, LPVOID(&AddMoney));
					
					if(AddMoney != 0)			
						GiveBonusMoney(AddMoney, ON_RET_CHNGEMONEY_INVESTMENT, FALSE);
				}

				siDayCredit		=	0;			
			}
		}
		
		//晦除檜 棻 脹 衛除薯 嬴檜蠱擊 餉薯и棻.
		DeleteTimerItem();
		
		//10衛陛 剩戲賊 奢撩嶸棉曖 薑爾蒂 餉薯п遽棻.
		DeleteFiledUnit();
		
		//虜擒 奢撩嶸棉檜 避橫憮 ぬ割じ蒂 嫡堅 氈朝 鼻鷓檜賊.
/*		if(!IsFieldAliveWarUnit())
		{
		
			//衛除檜 棻 腎橫 ぬ割じ陛 ヴ溜棻.
			if((pMyWorld->dwtimeGetTime - m_dwDeadTime) >= m_dwDeadSec)
			{

				m_bFieldAlive = TRUE;
				m_dwDeadTime  = 0;
				m_dwDeadSec	  = 0;
				SendFieldMsg(ON_CHANGED_PLAYER_DEAD_PENALTY,SO_SFM_ONLYME);

				DBIKDelteWarCharactor	TempDBIKDelteWarCharactor;
				
				TempDBIKDelteWarCharactor.siDBAccount	=	stAccountInfo.siDBAccount;
				TempDBIKDelteWarCharactor.uiDBSlot		=	stAccountInfo.uiSlot;

				pMyWorld->clDatabase->writeDBQuery(SO_DB_INFOKIND_DELETE_CHARACTOR_WAR, &TempDBIKDelteWarCharactor, sizeof(DBIKDelteWarCharactor));
			}
		}
*/
		// 菟橫螞 詭衛雖菟擊 籀葬и棻.
		ProcessMessage();	
		
		// 橾薑 衛除 輿晦煎 滲唳脹 辨煽菟曖 薑爾蒂 DB縑 盪濰и棻.	
		
		if( pMyWorld->clDatabase->getGateQueueSize() < 100000 )
		{
			for(int i = 0; i < ON_MAX_FOLLOWER_NUM; ++i ) 
			{			

				if( ( pFollower = IFollower.GetFollower(i)) != NULL && 
					  pFollower->ParametaBox.IP.isCycleSaveToDB() )
				{
					
//					if( (int)(pMyWorld->dwtimeGetTime - pFollower->dwSaveTime)  >= ON_PARAM_SAVETIME )					
					{ 																	
						
						// 盪濰 и棻 									
						saveFollowerDataToDB(SO_DB_INFOKIND_SAVEALLPARAM, i); 											
					}							
				}
			}	
		}
		


	}
}



//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	議葛攪曖 薑爾蒂 橢橫螞棻.	
//	熱薑 橾濠	:	2002-07-22 螃�� 7:41:57 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
DWORD	SOPlayer::GetInfo(OnCharPlayerInfo *pPlayerInfo)
{
	DWORD							dwUsedSize	=	sizeof(OnCharPlayerInfo);			// 議葛攪曖 晦獄 觼晦蒂 撲薑и棻.
	OnBaseBoothItem					*pBaseBoothItem;
	
	// ん檣攪蒂 撲薑п遽棻.
	pBaseBoothItem										=	(OnBaseBoothItem*)((CHAR*)pPlayerInfo + sizeof(OnCharPlayerInfo));

	// 議葛攪曖 晦獄 薑爾蒂 橢橫螞棻.
	BaseChar::GetInfo((OnCharBaseInfo*)pPlayerInfo);
	
	memset(pPlayerInfo->uiEquipWeapon, 0, sizeof(pPlayerInfo->uiEquipWeapon));
	
	pPlayerInfo->siGuildInGrade							=	siClassInGuild;
	pPlayerInfo->siFightingPower						=	GetFightingPower();
	pPlayerInfo->siDBAccount							=	stAccountInfo.siDBAccount;
	pPlayerInfo->uiEquipWeapon[ON_WEAR_HELMET_ITEM]		=	IFollower.GetHero()->Weapon.GetWearItem(ON_WEAR_HELMET_ITEM);	//	癱掘
	pPlayerInfo->uiEquipWeapon[ON_WEAR_WEAPON_ITEM]		=	IFollower.GetHero()->Weapon.GetWearItem(ON_WEAR_WEAPON_ITEM);	//	鼠晦
	pPlayerInfo->uiEquipWeapon[ON_WEAR_ARMOR_ITEM]		=	IFollower.GetHero()->Weapon.GetWearItem(ON_WEAR_ARMOR_ITEM);	//	骨褡		
	pPlayerInfo->uiEquipWeapon[ON_WEAR_SHOES_ITEM]		=	IFollower.GetHero()->Weapon.GetWearItem(ON_WEAR_SHOES_ITEM);	//	褐嫦
	pPlayerInfo->uiEquipWeapon[ON_WEAR_GUARDIAN_ITEM]	=	IFollower.GetHero()->Weapon.GetWearItem(ON_WEAR_GUARDIAN_ITEM);	//	熱�ㄩ�
	pPlayerInfo->siIsOverWeight							=	(SI08)IFollower.IsOverWeight();			// 鼠啪 蟾婁 鼻鷓檣雖 匐餌
	pPlayerInfo->siVMercenaryKind						=	0;
	pPlayerInfo->siFieldHp								=	m_siFieldHp;
	pPlayerInfo->bCaptain								=   m_bCaptainUnit;
	pPlayerInfo->siSupplyCharge							=	m_siFieldMP;
	pPlayerInfo->bVillageInLimit						=	m_bPlayerVillageInLimit;

	if(siVMercenarySlot > 0)
	{
		if(IFollower.IsValidFollower(siVMercenarySlot))
		{
			pPlayerInfo->siVMercenaryKind = IFollower.GetFollowerParameterKind(siVMercenarySlot);
		}
	}
	pPlayerInfo->bAttackMode	=	 GetAttackMode();		// 瞪癱賅萄檣陛?
	
	// 鼻薄 賅萄檣陛?
	if(clPrivateMarket.IsOpen() == FALSE)
	{
		pPlayerInfo->siPrivateMarketItemNum				=	-1;
	}
	else
	{	

		dwUsedSize	+=	clPrivateMarket.GetBaseItems(pBaseBoothItem, &pPlayerInfo->siPrivateMarketItemNum);
	}	

	return	dwUsedSize;
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	議葛攪曖 鼻撮 薑爾蒂 橢橫螞棻.
//	熱薑 橾濠	:	2002-08-15 螃�� 3:29:41 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
DWORD	SOPlayer::GetDetailInfo(OnCharPlayerDetailInfo *pDetailInfo, UI32 uiRefGuildID, SI32 siRefGuildInClass, DWORD siRefDBAccount )
{
	// 詭賅葬 蟾晦��.
	ZeroMemory(pDetailInfo, sizeof(OnCharPlayerDetailInfo));	
	
	// 議葛攪曖檜葷.
	strncpy(pDetailInfo->szID, szID, ON_ID_LENGTH);			
	
	if(uiGuildID != 0)
	{
		strncpy(pDetailInfo->szGuild, pMyWorld->clIGuild.GetGuildName(uiGuildID), ON_GUILDNAME_LENGTH);	
		pDetailInfo->siGuildFlag						=	siFlag;

		// 釭諦 檣濠煎 剩啖遽 鼻欽婁曖 婦啪蒂 憲嬴螞棻.
		pDetailInfo->uiIsHostilityGuild					=	FALSE;
		if(pMyWorld->clIGuildClass.GetJWJCode() != siClassInGuild)
		{
			// ⑷營 模樓и 鼻欽檜 氈堅
			// 鼻欽頂縑憮曖 霜晝擎 '雖錳濠'陛 嬴棲罹撿 и棻.
			if((uiRefGuildID != 0) && (pMyWorld->clIGuildClass.GetJWJCode() != siRefGuildInClass))
			{
				pDetailInfo->uiIsHostilityGuild			=	pMyWorld->clIGuild.IsHostilityRelation(uiRefGuildID, uiGuildID);

				if (FALSE == pDetailInfo->uiIsHostilityGuild)
					pDetailInfo->uiIsHostilityGuild			=	pMyWorld->clIGuild.IsEnemyRelationWar(uiRefGuildID, uiGuildID);
			}

		}			
	}	

	if( IsEvent() )
	{
		pDetailInfo->uiIsHostilityGuild	=	pMyWorld->m_ExpoRankingSystem.IsInvalidBattle( stAccountInfo.siDBAccount, siRefDBAccount );
		pDetailInfo->siEventScore		=	pMyWorld->m_ExpoRankingSystem.GetScore( stAccountInfo.siDBAccount );
		pDetailInfo->siEventRanking		=	pMyWorld->m_ExpoRankingSystem.GetRank( stAccountInfo.siDBAccount );
	}

	return	sizeof(OnCharPlayerDetailInfo);
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	10蟾葆棻 Heartbeat蒂 爾魚棻.
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
VOID SOPlayer::SendHeartbeat()
{
	DWORD cSendData[4];
	cSendData[0] = ON_HEARTBEAT;
	Write(cSendData, 4);
}



//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	僥濠翮 ip蒂 DWORD ⑽衝戲煎 夥脯遽棻.
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
DWORD SOPlayer::ChangeIPStr(char* lpIP)
{
	DWORD	TempIP;
	UI08	*pIP;
	SI32	siIP[8];

	pIP		=	(UI08*)&TempIP;	
	sscanf(lpIP, "%d.%d.%d.%d", &siIP[0], &siIP[1], &siIP[2], &siIP[3]);	
	pIP[0]	=	UI08(siIP[0]);
	pIP[1]	=	UI08(siIP[1]);
	pIP[2]	=	UI08(siIP[2]);
	pIP[3]	=	UI08(siIP[3]);

	return TempIP;
}



//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	絲擊 乾堅 氈朝雖 匐餌и棻.
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
BOOL	SOPlayer::EnoughMoney(MONEY mnEnoughMoney)
{
	if( siMoney < mnEnoughMoney)
		return FALSE;
	return TRUE;
}



//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	絲擊 渦ж剪釭 賓棻.
//	熱薑 橾濠	:	2002-08-05 螃�� 3:01:41 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
VOID	SOPlayer::ChangeMoney(MONEY siChangeMoney, BOOL bSendToDB)
{
	if( siChangeMoney > 0 )
		IncreaseMoney(siChangeMoney, bSendToDB);
	else if(siChangeMoney < 0)
		DecreaseMoney(-siChangeMoney, bSendToDB);
}



//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	絲擊 隸陛衛鑑棻.
//	熱薑 橾濠	:	2002-08-05 螃�� 3:02:17 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
MONEY	SOPlayer::IncreaseMoney(MONEY siIncreaseMoney, BOOL bSendToDB)
{
	DBIKSaveMoney		SaveMoney;
	MONEY				mnPrevMoney;

	if( siIncreaseMoney == 0 )		return	0;


	mnPrevMoney		=	siMoney;

	if((ON_MAX_MONEY - siMoney) >=  siIncreaseMoney)
		siMoney		+=	siIncreaseMoney;
	else
		siMoney		=	ON_MAX_MONEY;    // 10霞熱煎 ж賊 擒922唳(922 3372 0368 5477 5807)	

	if(bSendToDB == TRUE)
	{
		// DB縑 撲薑и棻.
		SaveMoney.DBAccount	=	stAccountInfo.siDBAccount;
		SaveMoney.uiSlot	=	stAccountInfo.uiSlot;
		SaveMoney.Money		=	siMoney;
				
		pMyWorld->clDatabase->writeDBQuery(SO_DB_INFOKIND_SAVEMONEY, &SaveMoney, sizeof(SaveMoney));
	}

	return	MONEY( siMoney - mnPrevMoney );
}



//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	絲擊 賓棻.
//	熱薑 橾濠	:	2002-08-05 螃�� 3:02:26 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
BOOL	SOPlayer::DecreaseMoney(MONEY siDecreaseMoney, BOOL bSendToDB)
{
	DBIKSaveMoney		SaveMoney;
	BOOL				bResult = FALSE;

	if( siDecreaseMoney == 0 )				return	TRUE;

	if(siMoney >= siDecreaseMoney)
	{
		siMoney		-=	siDecreaseMoney;
		bResult		=	TRUE;
		
		// DB縑 撲薑и棻.		
		SaveMoney.DBAccount	=	stAccountInfo.siDBAccount;
		SaveMoney.uiSlot	=	stAccountInfo.uiSlot;
		SaveMoney.Money		=	siMoney;
	
		if(bSendToDB == TRUE)
		{
			// DB縑 撲薑и棻.		
			pMyWorld->clDatabase->writeDBQuery(SO_DB_INFOKIND_SAVEMONEY, &SaveMoney, sizeof(SaveMoney));
		}
	}

	return bResult;
}

MONEY	SOPlayer::DecreaseMoneyReturnMoney(MONEY siDecreaseMoney)
{


	MONEY prevMoney = siMoney;
	

	if( siDecreaseMoney == 0 )				return	0;

	if(siMoney >= siDecreaseMoney)
	{
		siMoney		-=	siDecreaseMoney;
		
		
		
	}

	return (siMoney - prevMoney);

	
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	絲擊 盪旎и棻
//	熱薑 橾濠	:	2002-11-12 螃瞪 9:57:41 - 夢遽鷓
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
BOOL	SOPlayer::SaveMoneyAtBank(MONEY siChangeMoney)
{

	// 盪旎й絲檜 陛霞絲爾棻 號戲賊 褒ぬ
	if (siMoney < siChangeMoney) return FALSE;
	// 盪旎й絲檜 盪蹴и啗爾棻 號嬴紫 褒ぬ
	if (m_MoneySaving + siChangeMoney > ON_MAX_SAVING) return FALSE;

	// 盪旎擋擊 渦ж堅 濠晦絲縑憮 賓棻.
	m_MoneySaving += siChangeMoney;
	siMoney -= siChangeMoney;

	

	// db縑 憲萼棻.
	DBIKSaveToBank     SaveToBank;

	
	SaveToBank.DBAccount	=	stAccountInfo.siDBAccount;
	SaveToBank.uiSlot	=	stAccountInfo.uiSlot;
	SaveToBank.uiVillageCode = stAccountInfo.pVillage->uiVillageCode;
	SaveToBank.moSaveMoney = siChangeMoney;
		
	pMyWorld->clDatabase->writeDBQuery(SO_DB_INFOKIND_SAVETOBANK, &SaveToBank, sizeof(SaveToBank));

	return TRUE;
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	絲擊 檣轎и棻.
//	熱薑 橾濠	:	2002-11-12 螃瞪 9:57:41 - 夢遽鷓
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
BOOL	SOPlayer::WithdrawMoneyAtBank(MONEY siChangeMoney)
{

	// 檣轎й絲檜 盪旎擋爾棻 號戲賊 褒ぬ
	if (m_MoneySaving < siChangeMoney) return FALSE;
	// 檣轎й絲檜 模雖и啗爾棻 號嬴紫 褒ぬ
	if (siMoney + siChangeMoney > ON_MAX_MONEY) return FALSE;

	// 盪旎擋擊 貍堅 濠晦絲縑 渦и棻.
	m_MoneySaving -= siChangeMoney;
	siMoney += siChangeMoney;

	

	// db縑 憲萼棻.

	DBIKWithdrawFromBank     WithdrawFromBank;

	
	WithdrawFromBank.DBAccount	=	stAccountInfo.siDBAccount;
	WithdrawFromBank.uiSlot	=	stAccountInfo.uiSlot;
	WithdrawFromBank.uiVillageCode = stAccountInfo.pVillage->uiVillageCode;
	WithdrawFromBank.moDrawMoney = siChangeMoney;
		
	pMyWorld->clDatabase->writeDBQuery(SO_DB_INFOKIND_WITHDRAWFROMBANK, &WithdrawFromBank, sizeof(WithdrawFromBank));


	return TRUE;


}

MONEY	SOPlayer::ModifySaving(MONEY siMoney)
{
	MONEY PrevSaving = m_MoneySaving;
	m_MoneySaving += siMoney;

	m_MoneySaving	=	m_MoneySaving > ON_MAX_SAVING ? ON_MAX_SAVING : m_MoneySaving;

	//m_MoneySaving  = m_MoneySaving < 0 ? 0 : m_MoneySaving; 

	return (m_MoneySaving - PrevSaving);
}




//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	避擎 議葛攪蒂 葆擊 曖錳縑 爾魚棻.
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
VOID	SOPlayer::SendDeadPlayerToVillageHospital()
{
	SI32				siNearestHospitalVillageIndex;	
	OnlineFollower*		lpOnlineFollower;
	

	// 湍瞪 寰縑 氈戲賊 檜嘐 陛濰 陛梱遴 葆擊擊 憲堅 氈棻.
	if(siNearestHospitalVillageIndexInDungeon != -1)
		siNearestHospitalVillageIndex	=	siNearestHospitalVillageIndexInDungeon;
	else
		siNearestHospitalVillageIndex	=	pMyWorld->GetNearestHospitalVillageIndex(uiMapID, siX, siY);
	
	// 葆擊 檣策蝶陛 螢夥艇陛?
	if(siNearestHospitalVillageIndex < 0 || siNearestHospitalVillageIndex >= pMyWorld->siTotalVillageNum)
		siNearestHospitalVillageIndex = 0;

	// 嶸�褲� 葆擊檣雖 憲嬴螞棻.
	stAccountInfo.pVillage	=	&pMyWorld->pclVillage[siNearestHospitalVillageIndex];

	// 輿 裘戲煎 衡曹棻.
	// 奩萄衛 嬪縑憮 葆擊擊 撲薑и �醴� п撿 и棻.
	ChangeMap(0, stAccountInfo.pVillage->siVillageX, stAccountInfo.pVillage->siVillageY);

	// 葆擊縑 菟橫除棻.
	EnterVillage();	

	// 頂 議葛攪縑啪 葆擊縑 菟橫鬲棻堅 憲溥遽棻.
	SendFieldMsg(ON_RESPONSE_RETURNONLINE, SO_SFM_ONLYME, 
					LPVOID(ON_RET_RETURNONLINE_VILLAGE),
					LPVOID(stAccountInfo.pVillage));


	// 議葛攪蒂 �蛹� 衛鑑棻.
	lpOnlineFollower = IFollower.GetFollower(0);	
	lpOnlineFollower->ParametaBox.IP.SetLife(IFollower.GetFollowerParameterMaxLife(0));
	lpOnlineFollower->ParametaBox.IP.SetMana(IFollower.GetFollowerParameterMaxMana(0), 1);
	
	SendFieldMsg(ON_RESPONSE_HOSPITAL_CURESOLDER, SO_SFM_ONLYME, LPVOID(ON_RET_AUTOCUREOK), NULL, 
				 0, LPVOID(IFollower.GetFollowerParameterMaxLife(0)), LPVOID(IFollower.GetFollowerParameterMaxMana(0)));
}



//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	寡縑 驕棻.
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
VOID	SOPlayer::BoardShip(SI32 uiboardshipaccount)
{
	uiBoardShipAccount	=	uiboardshipaccount;
}



//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	寡蒂 顫堅 葆擊縑 紫雜ж艘棻.
//	熱薑 橾濠	:	2002-04-04 螃瞪 11:02:10 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
VOID	SOPlayer::ArriveVillageByShip(SOVillage *pVillage)
{
	// ⑷營 顫朝 寡陛 橈棻堅 撲薑и棻.
	uiBoardShipAccount		=	0;
	stAccountInfo.pVillage	=	pVillage;

	// 輿 裘戲煎 衡曹棻.
	ChangeMap(0, stAccountInfo.pVillage->siVillageX, stAccountInfo.pVillage->siVillageY);

	// 葆擊縑 菟橫除棻.
	EnterVillage();

	// 寡縑憮 頂萼棻堅 憲溥遽棻.
	SendFieldMsg(ON_GETOUTSHIP, SO_SFM_ONLYME, LPVOID(pVillage));
}



//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	蟾晦�飛� и棻.
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
VOID	SOPlayer::Init(UI16 uiAccount, UI16 uiMapID, SI16 siX, SI16 siY, UI16 uiKind, CHAR *pszID)
{
	BaseChar::Init(uiAccount, uiMapID, siX, siY, uiKind, pszID);
	
	// 2003.10.7 蹺陛, 薑撩遴 
	m_PartyState.Init();
}



//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	裘擊 滲唳и棻.
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
BOOL	SOPlayer::ChangeMap(UI16 uimapid, SI16 six, SI16 siy)
{
	BOOL	bSuccess = FALSE;

	// 檜翕ж溥朝 裘檜 uimapid陛 嶸�褲� 裘檜 嬴棲賊 FALSE蒂 葬欐и棻.
	if(pMyWorld->clISOMap.IsValidMapID(uimapid) == FALSE)	goto	END;		

	// 虜擒 ⑷營 裘婁 檜翕ж溥朝 裘檜 棻腦棻賊 斜縑 評艇 籀葬蒂 п遽棻.
	if(uiMapID != uimapid)
	{
		// 嬴霜擎 賅萇 湍瞪縑憮朝 葆擊檜 橈棻朝 陛薑ж縑憮 濛撩и棻
		// 斜葬堅 裘 樓撩檜 湍瞪檜 嬴棲塭賊 輿 裘檜棻.

		// 輿 裘 -> 湍瞪
		if((pMyWorld->clISOMap.IsDungeon(uiMapID) == FALSE) && (pMyWorld->clISOMap.IsDungeon(uimapid) == TRUE))
		{	
			SetNearestHospitalVillageIndexInDungeon(pMyWorld->GetNearestHospitalVillageIndex(uiMapID, siX, siY));
		}

		// 湍瞪 -> 湍瞪  -- 檜瞪曖 siNearestHospitalVillageIndexInDungeon曖 高擊 斜渠煎 陛雖堅 氈戲賊 脹棻.
		else if((pMyWorld->clISOMap.IsDungeon(uiMapID) == TRUE) && (pMyWorld->clISOMap.IsDungeon(uimapid) == TRUE));
			
		// ??? -> 輿 裘
		else if(pMyWorld->clISOMap.IsDungeon(uimapid) == FALSE) 
		{
			SetNearestHospitalVillageIndexInDungeon(-1);		
		}	
	}
	
	// 顫 嶸盪縑啪 だじ 褐羶醞檜剪釭 
	// 棻艇 嶸盪縑 だじ 褐羶擊 嫡懊棻賊 檜蒂 鏃模и棻.
	CloseRequestingJoinParty();

	bSuccess	=	TRUE;

END:
	if(bSuccess == TRUE)
	{
		// 裘擊 滲唳и棻.
		uiMapID	=	uimapid;
		siX		=	six;
		siY		=	siy;
	}
	
	return	bSuccess;
}

VOID	SOPlayer::SendReloadWearItem()
{
	//SI16 siFollowerNum	=	IFollower.GetFollowerNum();

	SI16 siCount = GetClientSendDeleteTimerItem(m_pTimerItemInfo);

	SendFieldMsg(ON_RELOAD_WEAR_ITEM,SO_SFM_ONLYME, LPVOID(m_pTimerItemInfo),LPVOID(siCount));
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	⑷營 絲擊 爾頂遽棻.
//	熱薑 橾濠	:	2002-08-17 螃�� 7:13:39 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
VOID	SOPlayer::SendMoney(DWORD dwKind)
{
	SendFieldMsg(ON_CHANGED_MONEY, SO_SFM_ONLYME, LPVOID(dwKind));
}



//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	爾傘蝶 絲擊 遽棻. 趙擊 熱紫 氈棻.
//	熱薑 橾濠	:	2002-08-05 螃�� 2:59:52 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
VOID	SOPlayer::GiveBonusMoney(MONEY Bonus, DWORD dwKind, BOOL bSendToDB)
{
	ChangeMoney(Bonus, bSendToDB);
	SendMoney(dwKind);
}



//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	爾傘蝶蒂 隸陛衛麵遽棻.
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
BOOL	SOPlayer::IncreaseBonus(UI08 uiFollowerID, SI32 siBonus)
{	
	SI32			siCurBonus;

	siCurBonus	=	IFollower.GetFollowerBaseParameterBonus(uiFollowerID);	

	if(IFollower.SetFollowerBaseParameterBonus(uiFollowerID, siCurBonus + siBonus) == TRUE)
	{	
		return TRUE;	
	}
	else
		return FALSE;
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	褐辨紫蒂 隸陛衛麵遽棻.
//	熱薑 橾濠	:	2002-08-12 螃�� 2:24:04 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
BOOL	SOPlayer::IncreaseCredit(SI32 siCredit)
{
	IncreaseTradeCredit(siCredit);
	return	TRUE;
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	隸陛衛麵還 褐辨紫蒂 掘и棻.
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
SI32	SOPlayer::GetGiveCredit(MONEY TradeMoney)
{
	UI32		Credit;
	UI32		MaxCredit;

	for(int i=pMyWorld->clCredit.Num;i>0; i--)
	{
		if( uiTradeCredit >= pMyWorld->clCredit.pCreditIndex[i-1].MinCredit )
		{
			Credit		=	pMyWorld->clCredit.pCreditIndex[i-1].GetAddCredit(TradeMoney);
			MaxCredit	=	pMyWorld->clCredit.pCreditIndex[i-1].GetMaxCredit(uiTradeCredit);
			
			if(siDayCredit < MaxCredit)
			{
				if(	(siDayCredit + Credit) > MaxCredit )
				{
					Credit			=	MaxCredit - siDayCredit;
					siDayCredit		=	MaxCredit;
					return Credit;
				}
				else
				{
					siDayCredit		+=	Credit;
					return Credit;
				}
			}
			break;
		}
	}
	return 0;
}



//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	檜葷縑 檜鼻и 僥濠陛 氈朝雖 �挫恉挬�. TRUE : 餌辨陛棟, FALSE : 餌辨 碳陛
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
BOOL	SOPlayer::CheckForbiddenName(char *szName)
{
	CHAR	*pszToken;
	CHAR	szTempBuffer[4096];

	ZeroMemory(szTempBuffer, sizeof(szTempBuffer));
	strcpy(szTempBuffer, szName);

	// 梟霞 詩じ 夥檜お 議葛攪陛 氈朝雖 羹觼и棻.
	if( !CheckLeadByte(szTempBuffer) )		return FALSE;

	pszToken	=	strtok(szName, ON_FORBIDDEN_NAME);
	if(pszToken == NULL)	return	FALSE;

	if(strlen(szTempBuffer) == strlen(pszToken))
		return	TRUE;
	else
		return	FALSE;
}



//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	ん虜馬擊 棺萼棻.
//	熱薑 橾濠	:	2002-05-27 螃瞪 10:54:31 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
VOID	SOPlayer::ChangeSatiety(SI32 siValue)
{	

	SI32					i;	
	UI08					uiFollowerNum;	
	BOOL					bMinusHealth = FALSE;
	strHungryFollowerInfo	stHFI[ON_MAX_FOLLOWER_NUM];

	// ん虜馬擊 滲唳 и棻 
	if((m_satiety + siValue) < 0)	bMinusHealth	=	TRUE;		
	m_satiety		=	min(max(m_satiety + siValue, 0), ON_MAX_HEALTH);	

	if(bMinusHealth == FALSE)
	{	
		// 詭衛雖蒂 爾頂遽棻.
		SendFieldMsg(ON_HUNGRY, SO_SFM_ONLYME, LPVOID(ON_RET_HUR_CHANGE_HEALTH), LPVOID(m_satiety));
	}
	else
	{
		// 頂陛 陛雖堅 氈朝 賅萇 議葛攪曖 羹溘擊 �A朝棻.	
		IFollower.AllCharacterDeleteLife(10);				

		uiFollowerNum	=	0;
		for(i = 0; i < ON_MAX_FOLLOWER_NUM; i++)
		{
			if(IFollower.IsValidFollower(i) == TRUE)
			{			
				stHFI[uiFollowerNum].uiFollowerID		=	i;
				stHFI[uiFollowerNum].siLife			=	IFollower.GetFollowerParameterLife(i);								

				// 薯橾 葆雖虞戲煎 隸陛衛麵遽棻.
				uiFollowerNum++;
			}
		}
		
		// 詭衛雖蒂 爾頂遽棻.
		SendFieldMsg(ON_HUNGRY, SO_SFM_ONLYME, LPVOID(ON_RET_HUR_CHANGE_HP), LPVOID(uiFollowerNum), LPVOID(stHFI));
	}		

}



//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	HeartBeat蒂 嫡朝棻.
//	熱薑 橾濠	:	2002-03-21 螃瞪 10:10:50 - 檜撩營
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
VOID	SOPlayer::RecvHeartBeat()
{
	OnHeartBeatMsg				*pHeartBeatMsg;	
	pHeartBeatMsg			=	(OnHeartBeatMsg *)	cMsg;

	BOOL	bSpeedHack	=	m_SpeedChecker.Signaled( pHeartBeatMsg->uiType );

	if( m_bIsSpeedHackUser == TRUE )						return;

	if( bSpeedHack )
	{
//		clServer->WriteInfo( "..\\SpeedHackUserList.txt","SpeedHackUser: DBAccount(%d)\tCharacterID(%s)\tHeartBeatType(%d)",
//							stAccountInfo.siDBAccount, szID, pHeartBeatMsg->uiType );

		writeInfoToFile( "SpeedHackUserList.txt","SpeedHackUser: DBAccount(%d)\tCharacterID(%s)\tHeartBeatType(%d)",
							stAccountInfo.siDBAccount, szID, pHeartBeatMsg->uiType );

		SendFieldMsg( ON_REQUEST_RUN_PROGRAM_LIST, SO_SFM_ONLYME, (LPVOID)RUN_PROG_LIST_SPEEDHACK );

		m_bIsSpeedHackUser	=	TRUE;

		LogOut();

	}

	/*
	for( i = ON_HEARTBEAT_SAVE_NUM - 1; i > 0; i-- )
	{
		dwHeartbeatTime[i]	=	dwHeartbeatTime[i-1];
	}
	dwHeartbeatTime[0]			=	pMyWorld->dwtimeGetTime;
	
	for( i = 0; i < ON_HEARTBEAT_SAVE_NUM - 1; i++ )
	{
		if(GETTIMEGAP(dwHeartbeatTime[i],dwHeartbeatTime[i+1]) > ON_HEARTBEAT_LOW_LIMIT )
			return;
		if(dwHeartbeatTime[ON_HEARTBEAT_SAVE_NUM+1] == 0 )
			return;
	}
	clServer->WriteInfo("..\\BlackList.txt","[SpeedHack User:\t%s\t]",szID);
	LogOut();
	return;
	*/

}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	鼻欽 梓嫦擊 滲唳и棻.
//	熱薑 橾濠	:	2002-04-04 螃�� 3:21:42 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
VOID	SOPlayer::ChangeGuildFlag(SI32 siflag)
{
	siFlag	=	siflag;

	// 詭衛雖蒂 爾魚棻.
	SendFieldMsg(ON_RESPONSE_CHANGE_GUILD_FLAG, SO_SFM_ONLYME, LPVOID(ON_RET_OK), LPVOID(siflag));			
}



//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	湍瞪戲煎 菟橫陛晦瞪縑 避橫憮 湍瞪 殮掘縑憮 薯橾 陛梱遴 避橫憮 葆擊曖 檣策蝶蒂 盪濰и棻.
//	熱薑 橾濠	:	2002-04-15 螃�� 4:35:18 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
VOID	SOPlayer::SetNearestHospitalVillageIndexInDungeon(SI32 siVillageIndex)
{
	DBIKNearestHVCode	NearestHVCode;	

	siNearestHospitalVillageIndexInDungeon	=	siVillageIndex;

	NearestHVCode.DBAccount			=	stAccountInfo.siDBAccount;
	NearestHVCode.uiSlot			=	stAccountInfo.uiSlot;

	if(siVillageIndex != -1)
	{
		NearestHVCode.uiVillageCode		=	pMyWorld->pclVillage[siVillageIndex].uiVillageCode;		
	}
	else
	{
		NearestHVCode.uiVillageCode		=	0;
	}	
	
	// DB縑 盪濰и棻.	
	pMyWorld->clDatabase->writeDBQuery(SO_DB_INFOKIND_NEARESTHVCODE, &NearestHVCode, sizeof(NearestHVCode));
}



//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	賅萇 Character曖 Life蒂 賓棻.
//	熱薑 橾濠	:	2002-05-27 螃瞪 10:44:24 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
VOID	SOPlayer::AllCharacterDeleteLifePoint(SI32 DeleteLife)
{
	IFollower.AllCharacterDeleteLifePoint(DeleteLife);	
}





//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	褐辨 蛔晝擊 橢橫螞棻.
//	熱薑 橾濠	:	2002-06-26 螃�� 2:54:52 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
SI32	SOPlayer::GetTradeGrade()
{
	const TradeHeader* pTradeHeader	=	pMyWorld->pTradeParser->GetHeader(uiTradeCredit);	

	if(pTradeHeader != NULL)	return pTradeHeader->uiTradeGrade;	
	else						return	0;	
}




//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	⑷營 掖羲檜 陛棟и陛?
//	熱薑 橾濠	:	2002-07-04 螃瞪 10:45:07 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
BOOL	SOPlayer::CanTrade()
{
	// ⑷營 в萄縑 氈雖 彊棻賊 FALSE蒂 葬欐и棻.
	if( CHECKSTATUS( siStatus, ON_PLAYER_STATUS_INFIELD ) == FALSE )	return	FALSE;
	//if(siStatus != ON_PLAYER_STATUS_INFIELD)	return	FALSE;

	// ⑷營 瞪癱 渠晦醞檜剪釭 瞪癱醞縑 氈棻賊 FALSE蒂 葬欐и棻.
	if(GetBattle() != NULL)						return	FALSE;

	// ⑷營 棻艇 Ы溯檜橫諦 掖羲 醞檜賊 FALSE蒂 葬欐и棻.
	if(IsTrading() == TRUE)						return	FALSE;

	// 偃檣 鼻薄 賅萄檜賊 FALSE蒂 葬欐и棻.
	if(clPrivateMarket.IsOpen() == TRUE)		return	FALSE;

	return	TRUE;
}



//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	⑷營 棻艇 Ы溯檜橫諦 掖羲醞檣雖 憲嬴螞棻.
//	熱薑 橾濠	:	2002-07-06 螃�� 2:02:49 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
BOOL	SOPlayer::IsTrading()
{
	return clTrade.IsTrading();
}



//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	Ы溯檜橫曖 璋濠煎 檜瑞橫霞 IP蒂 嫡嬴螞棻.
//	熱薑 橾濠	:	2002-07-04 螃�� 1:20:13 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
DWORD	SOPlayer::GetIPInNumeric()
{
	return	stAccountInfo.dwIP;
}



//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	掖羲й 嬴檜蠱擊 摹鷗ц朝雖 憲嬴螞棻.
//	熱薑 橾濠	:	2002-07-06 螃�� 8:56:44 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
BOOL	SOPlayer::IsChoseTradeItem(UI16 uiAccount)
{
	return clTrade.IsChoseTradeItem(uiAccount);
}



//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	掖羲й 嬴檜蠱擊 摹鷗ц湍 匙擊 鏃模и棻.
//	熱薑 橾濠	:	2002-07-06 螃�� 10:40:59 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
BOOL	SOPlayer::CancelChooseTradeItem()
{
	return	clTrade.CancelChooseTradeItem();
}



//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	摹鷗ц湍 嬴檜蠱菟擊 陛雖堅 掖羲й 匙擊 唸薑и棻.
//	熱薑 橾濠	:	2002-07-07 螃瞪 12:46:22 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
BOOL	SOPlayer::DecideTrade()
{
	return	clTrade.DecideTrade();
}



//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	瞪癱蒂 衛濛и棻.
//	熱薑 橾濠	:	2002-07-08 螃�� 8:43:28 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
VOID	SOPlayer::PrepareBattle()
{	
	CloseTrade();

	// 顫 嶸盪縑啪 だじ 褐羶醞檜剪釭 
	// 棻艇 嶸盪縑 だじ 褐羶擊 嫡懊棻賊 檜蒂 鏃模и棻.
	CloseRequestingJoinParty();

	BaseChar::PrepareBattle();
}



//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	瞪癱蒂 謙猿и棻.
//	熱薑 橾濠	:	2002-05-14 螃瞪 2:26:56 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
VOID	SOPlayer::EndBattle()
{
	// 辦摹 奩萄衛 瞪癱蒂 謙猿и棻.	
	BaseChar::EndBattle();
}



//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	濰熱蒂 陛雖堅 氈朝雖 憲嬴螞棻.
//	熱薑 橾濠	:	2002-07-15 螃�� 9:39:28 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
BOOL	SOPlayer::HaveGeneral()
{
	SI32	i;

	for(i = 0; i < ON_MAX_FOLLOWER_NUM; i++)
	{
		if(IsGeneral(i) == TRUE)	return	TRUE;
	}

	return	FALSE;
}



//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	濰熱檣雖 憲嬴螞棻.
//	熱薑 橾濠	:	2002-07-15 螃�� 9:40:25 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
BOOL	SOPlayer::IsGeneral(UI08 uiFollowerSlot)
{
	UI16	uiCharKind;

	if((uiCharKind = IFollower.GetFollowerCharID(uiFollowerSlot)) == 0)	return	FALSE;
	
	if(pMyWorld->clSOCharKI.GetGeneral(uiCharKind) == 1)	  return	TRUE;
	else if(pMyWorld->clSOCharKI.GetGeneral(uiCharKind) == 3) return	TRUE;
	else													return	FALSE;
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	⑷營 議葛攪陛 奢撩嶸棉檣雖 憲嬴螞棻.
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
BOOL	SOPlayer::IsFieldAttackUnit()
{
//	UI16	uiCharkind;

//	if((uiCharkind = IFollower.GetFollowerCharID(0)) == 0)	return	FALSE;

	return CSiegeUnitTable::GetInstance()->IsFieldAttackUnit(uiKind);
	
//	return pMyWorld->clSOCharKI.IsFieldAttackUnit(uiKind);

}



//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	辨煽檜 跨蝶攪檣雖 憲嬴螞棻.
//	熱薑 橾濠	:	2003-02-24 螃�� 8:55 - 寥鼻⑷
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
BOOL	SOPlayer::IsMonster( UI08 uiFollowerSlot )
{
	UI16	uiCharKind;

	if( (uiCharKind = IFollower.GetFollowerCharID(uiFollowerSlot)) == 0 )	return	FALSE;
	
	// siGeneral(濰熱高) : 橾奩辨煽(0), 濰熱(1), 跨蝶攪(2)
	if( pMyWorld->clSOCharKI.GetGeneral(uiCharKind) == 2 )	return	TRUE;
	else													return	FALSE;
}


//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	⑷營 爾嶸ж堅 氈朝 跨蝶攪 熱蒂 橢橫螞棻.
//	熱薑 橾濠	:	2003-02-24 螃�� 8:55 - 寥鼻⑷
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
SI32	SOPlayer::GetMonsterNum()
{
	SI32	i;
	SI32	siMonsterCount	=	0;

	for( i = 0; i < ON_MAX_FOLLOWER_NUM; i++ )
	{
		if( IsMonster( i ) )
		{
			siMonsterCount++;
		}
	}
	return siMonsterCount;
}


//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	Ы溯檜橫陛 陛韓 熱 氈朝 譆渠 辨煽曖 熱.(輿檣奢 んл)
//	熱薑 橾濠	:	2002-07-25 螃�� 4:39:34 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
SI16	SOPlayer::GetMaxFollowerNum()
{
	return pMyWorld->clMercenaryParser.GetMaxFollowerNum(GetTradeGrade(), IFollower.GetFollowerParameterLevel(0));
}


//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	Ы溯檜橫陛 遴艙濠檣雖 憲嬴螞棻.
//	熱薑 橾濠	:	2002-08-05 螃�� 1:49:58 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
BOOL	SOPlayer::IsAdminPlayer()
{
	if(USGrade != ON_USERGRADE_NORMAL)	return	TRUE;
	else								return	FALSE;
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	頂陛 辨煽擊 蹺陛й 熱 氈朝雖 憲嬴螞棻.
//	熱薑 橾濠	:	2002-09-24 螃�� 5:56:41 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
BOOL	SOPlayer::CanAddFollower( UI16 uiFollowerNum )
{
	// IFollower 偌羹縑啪 辨煽擊 蹺陛й 熱 氈朝雖 憲嬴螞棻.
	if( IFollower.CanAddFollower( uiFollowerNum ) == FALSE )						return	FALSE;
	
	// 譆渠 陛韓 熱 氈朝 辨煽爾棻 渦 號擎 辨煽擊 輿溥堅 ж朝雖 憲嬴螞棻.
	// -1 擊 ж朝 檜嶸朝 GetMaxFollowerNum() л熱陛 
	// 輿檣奢擊 薯諼и 辨煽曖 熱蒂 葬欐ж晦 陽僥檜棻.
	// ж雖虜 IFollower.GetFollowerNum() л熱朝 輿檣奢擊 んли 辨煽曖 熱蒂 憲溥遽棻.
	if( GetMaxFollowerNum() < ( IFollower.GetFollowerNum() + uiFollowerNum - 1 ) )		return	FALSE;
	
	return	TRUE;
}



bool	SOPlayer::expUp(int followerIndex, int value)
{				
	// 嶸�褲� 辨煽橾 唳辦 
	if( IFollower.GetFollower(followerIndex) )
	{
		_ImportantParameta* pParam  = &IFollower.GetFollower(followerIndex)->ParametaBox.IP;
		
		if(value > 10000)
			writeInfoToFile("ExpLog.txt", "[charName: %s][IP: %s][EXP:%d]", szID, stAccountInfo.IP, value);//唳я纂 煎斜蒂 陴曹棻.

		// 唳я纂蒂 隸陛 衛鑑棻 
		if(pParam->IncreaseExp(value) == TRUE)				
		{						
			pParam->SetLevel(pParam->GetLevel()+1);		
			pParam->SetBonus(pParam->GetBonus()+4);		
			pParam->SetLife(pParam->GetMaxLifeByIP());		
			pParam->SetMana(pParam->GetMaxManaByIP(), 2);
		}						

		return TRUE;
	}

	return FALSE;
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	鼻渠寞縑啪 絲擊 遽棻.
//	熱薑 橾濠	:	2002-09-30 螃�� 5:04:28 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
BOOL	SOPlayer::GiveMoney( MONEY *pMoney, SOPlayer *pOppPlayer )
{
	MONEY	mnMaxMoneyToGet;

	// 譆渠 陛韓 熱 氈朝 絲擊 掘и棻.
	mnMaxMoneyToGet	=	pOppPlayer->GetMaxMoneyToGet();

	// 譆渠 陛韓 熱 氈朝 絲爾棻 號擎 絲擊 還 熱朝 橈棻.
	if( mnMaxMoneyToGet < *pMoney )							return	FALSE;

	if( DecreaseMoney( *pMoney, FALSE ) == FALSE )			return	FALSE;

	// 賓 絲擊 鼻渠寞縑啪 遽棻.
	pOppPlayer->IncreaseMoney( *pMoney, FALSE ); 

	return	TRUE;
}


//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	陛韓 熱 氈朝 譆渠絲擊 橢橫螞棻.
//	熱薑 橾濠	:	2002-09-30 螃�� 5:19:01 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
MONEY	SOPlayer::GetMaxMoneyToGet()
{	
	return	(ON_MAX_MONEY - siMoney);
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	⑷營 葆擊 寰縑 氈朝雖 憲嬴螞棻.
//	熱薑 橾濠	:	2002-10-02 螃�� 7:55:24 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
BOOL	SOPlayer::IsInVillage()
{
	if( stAccountInfo.pVillage == NULL )		return	FALSE;
	
	
	return	TRUE;
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	⑷營 葆擊曖 勒僭 寰縑 菟橫鬲朝雖 匐餌и棻.
//	熱薑 橾濠	:	2002-10-02 螃�� 7:55:37 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
BOOL	SOPlayer::IsEnteredStructure()
{
	return	FALSE;
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	⑷營 絲,盪旎擋擊 橢橫螞棻.
//	熱薑 橾濠	:	2002-11-22 螃�� 4:05:?? - 夢遽鷓
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收

MONEY	SOPlayer::GetMoney()
{	
	return	siMoney;
}
MONEY	SOPlayer::GetSaving()
{	
	return	m_MoneySaving;
}







//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
// kojimodo
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收

//--------------------------------------------------------------------------------------------
// Name :
// Desc :
//--------------------------------------------------------------------------------------------
DWORD SOPlayer::getTempBiddingPrice()
{
	return m_tempBiddingPrice;
}



//--------------------------------------------------------------------------------------------
// Name :
// Desc :
//--------------------------------------------------------------------------------------------
void SOPlayer::setTempBiddingPrice( DWORD price )
{
	m_tempBiddingPrice = price;
}



//--------------------------------------------------------------------------------------------
// Name :
// Desc : 濠晦曖 ⑷營 嬪纂蒂 DB縑 盪濰и棻
//--------------------------------------------------------------------------------------------
void SOPlayer::savePosToDB()
{
	// 檜瞪婁 裘嬴檜蛤陛 ぎ葬剪釭 檜瞪 盪濰 嬪纂諦 20檜鼻 離檜陛 陳 唳辦縑虜 盪濰 и棻 
	if( m_latestSavedMapID != uiMapID || 
		(abs(m_latestSavedPos.x - siX) + abs(m_latestSavedPos.y - siY)) > 20 )
	{
		DBIKPlayerPos				PlayerPos;
		PlayerPos.DBAccount		=	stAccountInfo.siDBAccount;
		PlayerPos.uiSlot		=	stAccountInfo.uiSlot;
		PlayerPos.siX			=	siX;
		PlayerPos.siY			=	siY;
		PlayerPos.MapID			=	uiMapID;
		
		pMyWorld->clDatabase->writeDBQuery(SO_DB_INFOKIND_SAVEPLAYERPOS, &PlayerPos, sizeof(PlayerPos));
		
		m_latestSavedMapID		=	uiMapID;
		m_latestSavedPos.x		=	siX;
		m_latestSavedPos.y		=	siY;
		m_latestSavedPosTime    =	pMyWorld->dwtimeGetTime;
	}	
}



//--------------------------------------------------------------------------------------------
// Name :
// Desc : 賅萇 辨煽曖 ⑷營 薑爾蒂 DB縑 盪濰и棻 (!! 煎斜 嬴醒衛縑虜 賦撿 л !!)
//--------------------------------------------------------------------------------------------
void SOPlayer::saveAllFollowerDataToDB()
{
	for( int i=0; i< ON_MAX_FOLLOWER_NUM; ++i )
	{
		saveFollowerDataToDB( SO_DB_INFOKIND_LOGOUTSAVEALLPARAM, i );
	}	
}



//--------------------------------------------------------------------------------------------
// Name :
// Desc : и 辨煽曖 ⑷營 薑爾蒂 DB縑 盪濰и棻 
//--------------------------------------------------------------------------------------------
void SOPlayer::saveFollowerDataToDB( DWORD dbCmd, BYTE FollowerSlot )
{
	if( stAccountInfo.siDBAccount != 0 && stAccountInfo.uiSlot != 0)
	{
		OnlineFollower		    *pFollower;	
		DBIKFollowerParam		FollowerParam;	
		
		pFollower	=	IFollower.GetFollower(FollowerSlot);
		
		if( pFollower)						
		{
			FollowerParam.DBAccount			=	stAccountInfo.siDBAccount;
			FollowerParam.uiSlot			=	stAccountInfo.uiSlot;
			FollowerParam.uiFollowerSlot	=	FollowerSlot;

			//FollowerParam.Kind			=   pFollower->uiCharID;
//			if(pMyWorld->m_FieldBattleParser.IsFieldAttackUnit(pFollower->uiCharID) == TRUE)
			
			if(pMyWorld->m_WarTimeParser.IsWarTime(pMyWorld->pRealTime->tm_hour))
			{
				if(CSiegeUnitTable::GetInstance()->IsFieldAttackUnit(pFollower->uiCharID))
				{
					if(m_uiPrevKind != 0)
						FollowerParam.Kind  =   m_uiPrevKind;
					else
						FollowerParam.Kind	=   pFollower->uiCharID;
				}
				else
					FollowerParam.Kind	    =   pFollower->uiCharID;
			}

			FollowerParam.Exp			=	pFollower->ParametaBox.IP.GetCurExp();
			FollowerParam.Level			=	pFollower->ParametaBox.IP.GetLevel();
			FollowerParam.Str			=	pFollower->ParametaBox.IP.GetStr();
			FollowerParam.Dex			=	pFollower->ParametaBox.IP.GetDex();
			FollowerParam.Vit			=	pFollower->ParametaBox.IP.GetVit();
			FollowerParam.Int			=	pFollower->ParametaBox.IP.GetInt();
			FollowerParam.Life			=	pFollower->ParametaBox.IP.GetLife();
			FollowerParam.Mana			=	pFollower->ParametaBox.IP.GetMana();
			FollowerParam.AC			=	pFollower->ParametaBox.IP.GetAC();
			FollowerParam.Bonus			=	pFollower->ParametaBox.IP.GetBonus();
			FollowerParam.DmResist		=	pFollower->ParametaBox.IP.DamageResist;
			FollowerParam.MgResist		=	pFollower->ParametaBox.IP.MagicResist;
			FollowerParam.EquipMin		=	pFollower->ParametaBox.IP.EquipMinDamage;
			FollowerParam.EquipMax		=	pFollower->ParametaBox.IP.EquipMaxDamage;
					
			pMyWorld->clDatabase->writeDBQuery(dbCmd, &FollowerParam, sizeof(FollowerParam));

			pFollower->dwSaveTime =	pMyWorld->dwtimeGetTime;
			pFollower->ParametaBox.IP.setCycleSaveToDB(FALSE);
			pFollower->ParametaBox.IP.setSaveToDB(FALSE);
		}	
	}
}



//--------------------------------------------------------------------------------------------
// Name :
// Desc : 煎斜嬴醒衛 в蹂и 薑爾蒂 DB縑 盪濰 и棻 
//--------------------------------------------------------------------------------------------
void SOPlayer::saveLogoutInfoToDB()
{
	if( stAccountInfo.siDBAccount != 0 && stAccountInfo.uiSlot != 0 ) 	
	{

		sDB_LogoutInfo LogoutInfo;
		
		
		LogoutInfo.dbAccount	=	stAccountInfo.siDBAccount;
		LogoutInfo.slot			=	stAccountInfo.uiSlot;
		LogoutInfo.xpos			=	siX;
		LogoutInfo.ypos			=	siY;
		LogoutInfo.mapID		=	uiMapID;

		memcpy( LogoutInfo.IP, stAccountInfo.IP, 16 );
		LogoutInfo.IP[ 15 ] = 0;
		
		pMyWorld->clDatabase->writeDBQuery(SO_DB_INFOKIND_LOGOUTINFO, &LogoutInfo, sizeof(LogoutInfo));

	}
}

void SOPlayer::saveLogountWarInfoToDB()
{
//	if(pMyWorld->m_WarTimeParser.IsWarTime(pMyWorld->pRealTime->tm_hour) == FALSE) return; 
	if(pMyWorld->m_WarTimeParser.IsWarCheck(pMyWorld->pRealTime->tm_wday, pMyWorld->pRealTime->tm_hour) == FALSE) return;

	OnlineFollower		    *pFollower;
	pFollower	=	IFollower.GetFollower(0);
	if(pFollower == NULL) return;

	if( stAccountInfo.siDBAccount != 0 && stAccountInfo.uiSlot != 0 ) 	
	{
		sDB_LogoutWarInfo LogoutWarInfo;

		LogoutWarInfo.dbAccount	=	stAccountInfo.siDBAccount;
		LogoutWarInfo.slot		=	stAccountInfo.uiSlot;

		if(m_dwDeadSec > 0 || m_siFieldHp <= 0)
			LogoutWarInfo.PrevKind = 0;
		else
			LogoutWarInfo.PrevKind	=	(WORD)uiKind;		// 奢撩醞橾陽 奢撩醞檣 議葛攪蒂 盪濰и棻.

		LogoutWarInfo.Hp		=	(int)m_siFieldHp;
//		LogoutWarInfo.Supply	=	(int)m_siSupplyCharge;
		LogoutWarInfo.MP        =   (int)m_siFieldMP;
		LogoutWarInfo.bCaptain	=   (BYTE)m_bCaptainUnit;
	/*	if(pMyWorld->dwtimeGetTime - m_dwDeadTime < m_dwDeadSec)
		{
			m_dwDeadSec = m_dwDeadSec - (pMyWorld->dwtimeGetTime - m_dwDeadTime);
		}
		else
			m_dwDeadSec = 0;
		*/

		LogoutWarInfo.siDelay = (SI32)m_dwDeadSec;
		
		if(m_bPlayerVillageInLimit)
		{
			SOVillage*              pVillage = NULL;
			pVillage				=   pMyWorld->GetVillageByCode(m_siVilligeCode);
			if(pVillage != NULL)
			{
				pVillage->DeletCharName(szID);
				m_bPlayerVillageInLimit = FALSE;
				m_siVilligeCode = -1;
			}

		}

		pMyWorld->clDatabase->writeDBQuery(SO_DB_INFOKIND_WARINFO,&LogoutWarInfo,sizeof(LogoutWarInfo));
	}	
}


//--------------------------------------------------------------------------------------------
// Name :
// Desc : 煎斜嬴醒擊 衛濛 ж朝 Ы楚斜蒂 撲薑 и棻 
//--------------------------------------------------------------------------------------------
void SOPlayer::saveLogoutStartToDB()
{
	if( stAccountInfo.siDBAccount != 0 ) 	
	{		

		sDB_LogoutStart LogoutStart;

		LogoutStart.dbAccount	=	stAccountInfo.siDBAccount;
		

		memcpy( LogoutStart.id,			stAccountInfo.id,		LOGIN::ID_MAX_LENGTH );
		memcpy( LogoutStart.password,	stAccountInfo.password, LOGIN::PW_MAX_LENGTH );
		memcpy( LogoutStart.ip,			stAccountInfo.IP,		16 );

	
		pMyWorld->clDatabase->writeDBQuery(SO_DB_INFOKIND_LOGOUTSTART, &LogoutStart, sizeof(LogoutStart));

	}
}





//--------------------------------------------------------------------------------------------
// Name :
// Desc : ん虜馬擊 DB縑 盪濰 и棻
//--------------------------------------------------------------------------------------------
void SOPlayer::saveSatietyToDB()
{
	// 錳獄 ん虜馬婁 棻蒂 唳辦縑虜 盪濰 и棻 
	if( stAccountInfo.siDBAccount != 0 && m_orgSatiety != m_satiety && stAccountInfo.uiSlot != 0)
	{
		DBIKSatiety				DBSatiety;

		DBSatiety.DBAccount	=	stAccountInfo.siDBAccount;
		DBSatiety.uiSlot	=	stAccountInfo.uiSlot;
		DBSatiety.siSatiety	=	m_satiety;

		pMyWorld->clDatabase->writeDBQuery(SO_DB_INFOKIND_SETSATIETY, &DBSatiety, sizeof(DBSatiety));
	}
}



//--------------------------------------------------------------------------------------------
// Name :
// Desc : 檜 嶸盪陛 嬪纂и 唳衙模蒂 橢橫螞棻 ( 唳衙模縑 嬪纂ж雖 彊懊擊 唳辦 NULL )
//--------------------------------------------------------------------------------------------
CAuctionMarket* SOPlayer::getAuctionMarket()
{
	return pMyWorld->m_pAuctionMarket;
}



//--------------------------------------------------------------------------------------------
// Name :
// Desc : 檜 嶸盪蒂 瞪癱寰縑憮 薯剪 и棻 
//--------------------------------------------------------------------------------------------
void SOPlayer::removeSelfInBattle()
{	
	if( GetBattle() != NULL )
	{
		if(GetBattle()->RemovePlayer(this) == -1) 
		{
			GetBattleMgr()->DestroyBattle(GetBattleKey());
		}						

		EndBattle();
	}
}



//--------------------------------------------------------------------------------------------
// Name :
// Desc : 檜 嶸盪蒂 唳衙模寰縑憮 薯剪 и棻 
//--------------------------------------------------------------------------------------------
void SOPlayer::removeSelfInAuctionMarket()
{	
	if( getAuctionMarket() )
	{		
		getAuctionMarket()->removeAudience( this );
		getAuctionMarket()->removeBidder( this );		
	}
}



//--------------------------------------------------------------------------------------------
// Name :
// Desc : 檜 嶸盪蒂 啪歜寰縑憮 薯剪 и棻 
//--------------------------------------------------------------------------------------------
VOID SOPlayer::removeSelfInGame()
{
	// 啪歜縑 撩奢瞳戲煎 煎斜檣 ц棻賊 啪歜縑 婦溼脹 睡碟縑憮紫 餉薯п遽棻.
	if(bSuccessLogin == TRUE)
	{
		// в萄塭賊.
		// 裘縑憮 п渡 議葛攪曖 id蒂 餉薯и棻.
		// 辦摹 裘縑 撲薑腎橫 氈朝啪 濠褐檜 蜃朝雖 �挫恉挬�.		
		if(siStatus	& ON_PLAYER_STATUS_INFIELD)
		{
			// ⑷營 寡縑 顫堅 氈棻賊 寡縑 蝓摹и 葬蝶お縑 氈朝 釭蒂 薯剪и棻.
			if(uiBoardShipAccount != 0)
			{
				pMyWorld->clSOShip.LogOutPlayer(uiBoardShipAccount, uiAccount, stAccountInfo.siDBAccount);
			}
			else
			{
				// 裘縑憮 ID蒂 餉薯и棻.
				if(DelIDOnMap( 8 ) == FALSE)
						writeInfoToFile("DelIDOnMap Failed.txt", "2 [uiDBAccount : %u, uiSlot : %d, Name:[%s]] [siStatus: %d] [m_battleKey: %d], [uiMapID : %2d] [siX : %2d] [siY : %2d], [uiState : %d], [mainState : %d] [ IP : %s ]", 
													stAccountInfo.siDBAccount, stAccountInfo.uiSlot, szID, 
													siStatus, 
													m_battleKey,
													uiMapID, 
													siX,
													siY,
													uiState,
													m_mainState.get(),
													stAccountInfo.IP);

/*					
					pMyWorld->clServer->WriteInfo("..\\DelIDOnMap Failed.txt", "2 [uiDBAccount : %u, uiSlot : %d, Name:[%s]] [siStatus: %d] [m_battleKey: %d], [uiMapID : %2d] [siX : %2d] [siY : %2d], [uiState : %d], [mainState : %d] [ IP : %s ]", 
													stAccountInfo.siDBAccount, stAccountInfo.uiSlot, szID, 
													siStatus, 
													m_battleKey,
													uiMapID, 
													siX,
													siY,
													uiState,
													m_mainState.get(),
													stAccountInfo.IP);
*/			
			}
		}	
		
		else if(siStatus == ON_PLAYER_STATUS_INVILLAGE)	// 葆擊檜塭賊...
		{
			stAccountInfo.pVillage->PlayerLogOut(this);
		}


		// 瞪癱縑憮 釭螞棻 
		removeSelfInBattle();

		// 唳衙模縑憮 釭螞棻 
		removeSelfInAuctionMarket();

		// 掖羲璽擊 殘朝棻.
		CloseTrade();

		// 釭蒂 耀掘煎 蛔煙и 餌塋菟縑啪 頂陛 煎斜嬴醒л擊 憲萼棻.
		NotifyFriendsOfLoginState( FALSE );
				
		// 顫 嶸盪縑啪 だじ 褐羶醞檜剪釭 
		// 棻艇 嶸盪縑 だじ 褐羶擊 嫡懊棻賊 檜蒂 鏃模и棻.
		CloseRequestingJoinParty();

		// だじ錳檜塭賊 だじ縑憮 驍黴и棻.
		if( IsPartyMember() == TRUE )
		{
			LeaveParty();
		}

		// 薑衝 鼻欽錳橾 唳辦 鼻欽 婦葬濠縑啪 煎斜嬴醒 ж艘棻堅 憲溥遽棻.
		if( ( uiGuildID != 0 ) && ( siClassInGuild != pMyWorld->clIGuildClass.GetJWJCode() ) )
		{
			pMyWorld->clIGuild.GuildMemberLogout( uiGuildID );
		}

	}
	else
	{
		// 煎斜檣擊 ж雖 彊懊棻.
		printf("[%s] didn't login.\n", szID);
	}		
}



//--------------------------------------------------------------------------------------------
// Name :
// Desc : 嶸�褲� GUID 檣陛?
//--------------------------------------------------------------------------------------------
bool SOPlayer::isValidGUID(char* guid)
{
	if( strlen(guid) != 36 ) return FALSE;
		
	for(int i = 0; i < 36; i++ )
	{
		
		if( (isalpha( guid[i] ) != 0) || 
			(isdigit( guid[i] ) != 0) ||
			(guid[i] == '-' ) )
		{
			;				
		}
		else
		{
			return FALSE;
		}		
	}

	return TRUE;
}



//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	嬴檜蠱擊 檜翕 衛鑑棻.
//	熱薑 橾濠	:	2002-11-12 螃�� 1:16:38 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
BOOL	SOPlayer::MoveItem( UI08 uiFollowerSlot1, UI16 uiItemID, UI16 uiItemQuantity, UI08 uiFollowerSlot2, SI16 siPosInInventory )
{	
	CHAR		szDBArgList[ 1024 ];	

	if( IFollower.MoveItem( uiFollowerSlot1, uiItemID, uiItemQuantity, uiFollowerSlot2, &siPosInInventory ) == FALSE )		return	FALSE;

	// DB縑 盪濰и棻.
	ZeroMemory( szDBArgList, sizeof( szDBArgList ) );
	sprintf( szDBArgList, "%d, %d, %d, %d, %d, %u, %u", stAccountInfo.siDBAccount, stAccountInfo.uiSlot, 
														uiFollowerSlot1, uiFollowerSlot2, siPosInInventory, 
														uiItemID, uiItemQuantity );
						

	pMyWorld->clDatabase->writeDBQuery( SO_DB_INFOKIND_MOVEITEM, szDBArgList, strlen( szDBArgList ) + 1 );


	return	TRUE;
}


void	SOPlayer::SendItemCount()
{
	char buf[ 128 ];
	
	OnResItemReceivedMsg *pMsg = (OnResItemReceivedMsg *)buf;
		
	pMsg->dwMsgType = ON_RESPONSE_ITEMRECEIVED;
	pMsg->uiCount = m_uiRecvItemNum;

	Write( buf, 8 );
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	
//	熱薑 橾濠	:	2003-01-04 螃�� 1:40:19 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
VOID	SOPlayer::RecvRunProgramList()
{	
	OnResRunProgramListMsg	*pRunProgramListMsg;
	SI32					i;
	CHAR					*pszProgramName;
	CHAR					szProgramName[ 1024 ];

	pRunProgramListMsg	=	( OnResRunProgramListMsg* )cMsg;
	pszProgramName		=	( ( CHAR* )pRunProgramListMsg ) + sizeof( OnResRunProgramListMsg );

	CHAR					szLogFileName[128];
	switch( pRunProgramListMsg->dwReason )
	{
	case RUN_PROG_LIST_SPEEDHACK:
//		strcpy( szLogFileName, "..\\SpeedHackUser.txt" );
		strcpy( szLogFileName, "SpeedHackUser.txt" );
		break;
	case RUN_PROG_LIST_LABORHACK:
//		strcpy( szLogFileName, "..\\LaborHackUser.txt" );
		strcpy( szLogFileName, "LaborHackUser.txt" );
		break;
	}
//	clServer->WriteInfo(szLogFileName, "---------------- UserID : [%s]  ---------------- ", szID);	
	writeInfoToFile(szLogFileName, "---------------- UserID : [%s]  ---------------- ", szID);	
	for( i = 0; i < pRunProgramListMsg->siRunProgramNum; i++ )
	{		
		ZeroMemory( szProgramName, sizeof( szProgramName ) );
		strncpy( szProgramName, pszProgramName, 254 );
		pszProgramName	+=	255;

//		clServer->WriteInfo(szLogFileName, "[%s]", szProgramName);	
		writeInfoToFile(szLogFileName, "[%s]", szProgramName);	
	}

//	clServer->WriteInfo(szLogFileName, "------------------------------------------------ ", szID);	
	writeInfoToFile(szLogFileName, "------------------------------------------------ ", szID);	
}


//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	瞪癱賅萄 / ゎ�飛藒� 撲薑
//	熱薑 橾濠	:	2003-09-22 螃�� 1:50 - 寥鼻⑷
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
VOID	SOPlayer::RecvSetAttackMode()
{
	OnReqSetAttackMode			*pRecvMsg;

	pRecvMsg		=	(OnReqSetAttackMode*)cMsg;

	SetAttackMode( pRecvMsg->bAttackMode );
	SendFieldMsg( ON_RESPONSE_SET_ATTACK_MODE, SO_SFM_ONLYNEARCHAR, LPVOID(pRecvMsg->bAttackMode) );
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	濰睡曖 鼻欽 薑爾蒂 橢朝棻.
//	熱薑 橾濠	:	2004-01-05 PM __Lee_min_su__
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
VOID	SOPlayer::RecvGetAuditGuildInfo()
{
	cltGuild* pGuild = NULL;
	if(uiGuildID != 0)
	{
		if(bGuildInfoChanged == TRUE)
		{
			if((pGuild = pMyWorld->clIGuild.GetGuildByID(uiGuildID)) == NULL)
				return;

			if(pGuild->GetAuditGuildInfo(this) == TRUE)
				UnSetGuildInfoChanged();	// 鼻欽薑爾陛 滲唳 Flag蒂 
											// FALSE煎 滲唳и棻.
		}
		// 譆謙瞳戲煎 爾頂遽 望萄 薑爾諦 滲и啪 橈戲賊..
		else
		{
			SendFieldMsg(ON_RESPONSE_AUDIT_GUILD_INFO, \
				SO_SFM_ONLYME, LPVOID(0), LPVOID(0), LPVOID(-1) );
		}
	}
	else
	{
		// 鼻欽錳檜 嬴棲賊...
		SendFieldMsg(ON_RESPONSE_AUDIT_GUILD_INFO, \
			SO_SFM_ONLYME, LPVOID(0), LPVOID(0), LPVOID(-2));
	}	
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	衙觼煎蒂 餌辨и 嶸盪曖 薑爾蒂 盪濰и棻.
//	熱薑 橾濠	:	2004-04-10 PM __Lee_min_su__
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
VOID SOPlayer::RecvMacroUsingUserInfo()
{
	OnReqMacroUsingUserInfo			*pRecvMsg;
	pRecvMsg		=	(OnReqMacroUsingUserInfo*)cMsg;

	writeInfoToFile("MacroUsingUserList.txt", "[Prog Name : %s] Account ID : %s, Character Name : %s, IP : %s", 
		pRecvMsg->szMacroProgName, stAccountInfo.id, logininfo->charBaseInfo.name, stAccountInfo.IP);
}

VOID	SOPlayer::SetEquipItems(EquipItemInfo* TempItemInfo,SI16 siMercenarySlot,SI16& siTimerItemUseCount,OnlineFollower* pFollower)
{	
	SI16			siRestTime;
	OnTimerItemInfo	TempTimerItemInfo;

	for(int i = 0; i < WEAR_ITEM_NUM; i++)
	{
		if(TempItemInfo->id[i])
		{
			if(pMyWorld->clItem.IsTimeLimitItem(TempItemInfo->id[i]))
			{
				siRestTime = GetRestTime(TempItemInfo->startTime[i],TempItemInfo->endTime[i]);

				TempTimerItemInfo.siItemID		  = TempItemInfo->id[i];
				TempTimerItemInfo.siLoc			  = i;
				TempTimerItemInfo.siMercenarySlot = siMercenarySlot;
				TempTimerItemInfo.siRestTime	  = siRestTime;
				TempTimerItemInfo.bDeleteTimerItem=	IsDeleteTimerItem(TempTimerItemInfo.siRestTime,pMyWorld->clItem.GetItemUsingDay(TempItemInfo->id[i]));

				//贗塭檜樹お縑 爾鳥 薑爾蒂 盪濰.
				memcpy(&m_stTimerItemInfo[TempTimerItemInfo.siMercenarySlot][TempTimerItemInfo.siLoc],&TempTimerItemInfo,sizeof(OnTimerItemInfo));

				if(TempTimerItemInfo.bDeleteTimerItem)
				{
					siTimerItemUseCount++;
					TempTimerItemInfo.siItemID = 0;
				}
				
				//憮幗曖 嬴檜蠱 薑爾蒂 盪濰и棻.
				pFollower->SetWearItem(i,TempTimerItemInfo.siItemID,TempTimerItemInfo.siRestTime);

				//餉薯 腎朝 嬴檜蠱曖 唳辦 DB縑 憲溥遽棻.
				if(TempTimerItemInfo.bDeleteTimerItem)
				{
					DBIKWearItem			stWearItem;

					stWearItem.siDBAccount			=	stAccountInfo.siDBAccount;
					stWearItem.uiSlot				=	stAccountInfo.uiSlot;
					stWearItem.uiFollowerID			=	siMercenarySlot;
					stWearItem.uiWearLoc			=   i;
					stWearItem.uiItemID				=	0;
					stWearItem.siItemPosInInventory	=	0;
					stWearItem.uiQuantity			=	0;
					stWearItem.siLimitTime			=	pMyWorld->clItem.GetItemUsingDay(TempItemInfo->id[i]);
					stWearItem.siStartTime			=	TempItemInfo->startTime[i];
					stWearItem.siEndTime			=	TempItemInfo->endTime[i];

					pMyWorld->clDatabase->writeDBQuery(SO_DB_INFOKIND_CHANGE_WEAR_INFO, &stWearItem, sizeof(DBIKWearItem));	
				}
			}
			else
				pFollower->SetWearItem(i,TempItemInfo->id[i],0);
				
		}
	}			
}

BOOL	SOPlayer::CheckGetTimeLimitItem()
{
	for(int i = 0 ; i< ON_MAX_FOLLOWER_NUM; i++)
	{
		for(int j = 0 ; j < WEAR_ITEM_NUM ; j++)
		{
			if(m_stTimerItemInfo[i][j].siItemID)
			{
				return TRUE;
			}
		}
	}
	return FALSE;
}

SI16	SOPlayer::GetRestTime(SI32 start,SI32 end)
{
	SI16 siNowYear,siNowMon,siNowDay;
	SI16 siEndYear,siEndMon,siEndDay;
	char szTemp[10];
	char szTemp2[10];
	char szTemp3[10];
	char szTemp4[10];


	sprintf(szTemp,"%d",end);

	siNowYear = pMyWorld->RealWorldTime.uiYear;
	siNowMon  = pMyWorld->RealWorldTime.uiMonth;
	siNowDay  = pMyWorld->RealWorldTime.uiDay;
	
	siEndYear = atoi(strncpy(szTemp2,szTemp,4));
	siEndMon  = atoi(strncpy(szTemp3,&szTemp[4],2));
	siEndDay  = atoi(strncpy(szTemp4,&szTemp[6],2));

	return GetComPareDate(siNowYear,siNowMon,siNowDay,siEndYear,siEndMon,siEndDay);
}

//嬴檜蠱 蹺陛 憮綠蝶 衛除擊 渦ж堅 餉薯й 嬴檜蠱檣雖 嬴棋雖 っ欽и棻.
BOOL	SOPlayer::IsDeleteTimerItem(SI16 siRestTime,SI16 siMaxTime)
{
	SI16 siTemp;
	if(siMaxTime < 10)
		siTemp = 1;
	else if(siMaxTime >= 10)
		siTemp = (SI16)siMaxTime/10;

	if(siRestTime + siTemp < 0)
		return TRUE;
	else
		return FALSE;
}

SI16 SOPlayer::GetClientSendDeleteTimerItem(OnTimerItemInfo* pTimerItemInfo)
{
	SI16 siCount = 0;

	ZeroMemory(pTimerItemInfo,sizeof(OnTimerItemInfo)*12);

	for(SI16 i=0 ;i<12 ;i++)
	{
		for(SI16 j=0 ;j<9 ;j++)
		{
			if(m_stTimerItemInfo[i][j].bDeleteTimerItem)
			{
				memcpy(&pTimerItemInfo[siCount],&m_stTimerItemInfo[i][j],sizeof(OnTimerItemInfo));
				ZeroMemory(&m_stTimerItemInfo[i][j],sizeof(OnTimerItemInfo));
				siCount++;
			}
		}
	}
	return siCount;
}



SI16 SOPlayer::GetComPareDate(SI16 siNowYear, SI16 siNowMon, SI16 siNowDay,SI16 siEndYear,SI16 siEndMon,SI16 siEndDay)
{
	SI16 siCount;
	siCount = 0;
	
	SI16 siTempYear = siEndYear - siNowYear;
	//檜匙紫 彌殖 褐唳 寰撕擠.
	if(siTempYear > 0)
		siCount += siTempYear * 365;
	else
		siCount += siTempYear * 365;

	SI16 siTempMon = siEndMon - siNowMon;
	if(siTempMon > 0)
	{
		while(siEndMon != siNowMon)
		{
			siEndMon--;
			siCount += GetMDay(siEndMon);
		}
	}
	else
	{
		while(siEndMon != siNowMon)
		{
			siNowMon--;
			siCount -= GetMDay(siNowMon);
		}
	}
	SI16 siTempDay = siEndDay - siNowDay;
		siCount += siTempDay;

	return siCount;

}

VOID SOPlayer::GetEndDate(SI16 siItemTime, SI16 siNowYear, SI16 siNowMon, SI16 siNowDay,SI16& siEndYear,SI16& siEndMon,SI16& siEndDay)
{
	SI16 siTempItemTime = 0;
	SI16 siTempCount = 0;

	for(SI16 i = 1 ; i < siItemTime + 2;i++)
	{
		siNowDay++;
		if(GetMDay(siNowMon) == (siNowDay -1))
		{
			if(siNowMon >= 12)
			{
				siNowYear++;
				siNowMon = 1;
				siNowDay = 1;
			}
			else
			{
				siNowMon++;
				siNowDay = 1;
			}
		}
	}
	siEndYear = siNowYear;
	siEndMon  = siNowMon;
	siEndDay  = siNowDay;

}

SI16 SOPlayer::GetMDay(SI16 siMon)
{
	//彌殖 褐唳 寰質擠.
	if(siMon == 1 || siMon == 3 || siMon == 5 || siMon == 7 || siMon == 8 || siMon == 10 || siMon == 12)
		return 31;
	else if(siMon == 4 || siMon == 6 || siMon == 9 || siMon == 11)
		return 30;
	else if(siMon == 2)
	{
		//if(siYear%4 == 0)
		//	return 29;
		//else 
			return 28;
	}
	return 0;
}

VOID SOPlayer::DeleteTimerItem()
{
	OnlineFollower		*pFollower;	

	if(pMyWorld->pRealTime->tm_hour == 23)
	{
		if(m_bDayChange == FALSE)
			m_bDayChange = TRUE;
	}

	if( pMyWorld->pRealTime->tm_hour == 0)
	{
		if(m_bDayChange == TRUE)
		{
			//if((pMyWorld->pRealTime->tm_hour)	==	m_dwUserhour)
			{
				if(pMyWorld->pRealTime->tm_min >= m_dwUserMin)
				{
					if(pMyWorld->pRealTime->tm_sec >= m_dwUserSec)
					{
						//羹觼
						if(CheckGetTimeLimitItem() && CHECKSTATUS( siStatus, ON_PLAYER_STATUS_INFIELD ) == TRUE)
						{											
							for(int i = 0; i < ON_MAX_FOLLOWER_NUM; i++ ) 
							{
								pFollower = IFollower.GetFollower(m_stTimerItemInfo[i][0].siMercenarySlot);
								if(pFollower != NULL)
								{
									for(int j = 0 ; j < 9 ; j++)
									{
										if(m_stTimerItemInfo[i][j].siItemID)
										{	
											m_stTimerItemInfo[i][j].siRestTime-=1;
											m_stTimerItemInfo[i][j].bDeleteTimerItem = IsDeleteTimerItem(m_stTimerItemInfo[i][j].siRestTime,pMyWorld->clItem.GetItemUsingDay(m_stTimerItemInfo[i][j].siItemID));

											if(m_stTimerItemInfo[i][j].bDeleteTimerItem == TRUE)
												DisAppearItem(m_stTimerItemInfo[i][0].siMercenarySlot,m_stTimerItemInfo[i][j].siLoc,m_stTimerItemInfo[i][j].siItemID,0);
										}
									}
								}
							}
							//羹觼 唸婁縑 評塭.
							SendReloadWearItem();
						}
						m_bDayChange = FALSE;
					}//if(pMyWorld->pRealTime->tm_sec >= m_dwUserSec)
				}//if(pMyWorld->pRealTime->tm_min >= m_dwUserMin)
			}//if((pMyWorld->pRealTime->tm_hour)	==	m_dwUserhour)	
		}//if(m_bDayChange == TRUE)
	}
}


VOID SOPlayer::DeleteFiledUnit()
{
	//DB縑憮 橾婪瞳戲煎 議葛攪曖 奢撩 婦溼 薑爾蒂 雖錶遽�� 
	//憮幗縑憮 雖辦堅 贗塭檜樹お縑 憲萼棻.
	if( pMyWorld->pRealTime->tm_hour == pMyWorld->m_WarTimeParser.GetWarUpdate() && 
	    pMyWorld->pRealTime->tm_min == 5 && pMyWorld->pRealTime->tm_sec < 10)
	{
		if(m_bFieldUnitUpdate == FALSE)
		{
			m_bFieldUnitUpdate = TRUE;
		}
	}

	if( pMyWorld->pRealTime->tm_hour == pMyWorld->m_WarTimeParser.GetWarUpdate() && 
		pMyWorld->pRealTime->tm_min == 5 && pMyWorld->pRealTime->tm_sec >= 10)
	{
		if(m_bFieldUnitUpdate == TRUE)
		{
			if(uiGuildID != 0)
			{
//				if(pMyWorld->m_FieldBattleParser.IsFieldAttackUnit(uiKind) == TRUE)
				if(CSiegeUnitTable::GetInstance()->IsFieldAttackUnit(uiKind) == TRUE)
				{
					DWORD dwRes = 0;
					OnlineFollower*		lpOnlineFollower;

					lpOnlineFollower           = IFollower.GetFollower(0);
					uiKind				       = GetPrevKind();
					lpOnlineFollower->uiCharID = GetPrevKind();
					
					SetFieldHp(0);
					SetFieldMP(0);
					SetPrevKind(GetPrevKind());
					SetCaptainUnit(FALSE);					
					
					m_bFieldAlive = TRUE;
					m_dwDeadTime = m_dwDeadSec = 0;
					m_bIsWarUnit = FALSE;
					
					if(m_bPlayerVillageInLimit) //熱撩 檣攪む檜蝶縑 菟橫諦氈朝 鼻鷓檣唳辦
					{	
						SOVillage *pVillage	= pMyWorld->GetVillageByCode(m_siVilligeCode);
						if(pVillage != NULL)
						{						
							if( pVillage->DeletCharName(szID) < 0)
							{
								SendFieldMsg(ON_RESPONSE_VILLAGE_DEFFNCE_STOP, SO_SFM_ONLYME, LPVOID(dwRes));
								return;
							}

							dwRes = 1;
							m_bPlayerVillageInLimit = FALSE;
							m_siVilligeCode = -1;

							SendFieldMsg(ON_RESPONSE_VILLAGE_DEFFNCE_STOP, SO_SFM_ONLYNEARCHAR, LPVOID(dwRes));
						}
					}
					/*
					if (IsInVillage())  
						SendFieldMsg(ON_CHANGED_WAR_END,SO_SFM_ONLYME,LPVOID(GetPrevKind()),LPVOID(this));
					else
						*/
					
					SendFieldMsg(ON_CHANGED_WAR_END,SO_SFM_MEANDNEARCHAR,LPVOID(GetPrevKind()),LPVOID(this));
				}
				else
				{
					//if (IsInVillage()) 
					//	SendFieldMsg(ON_CHANGED_WAR_END,SO_SFM_ONLYME,LPVOID(uiKind),LPVOID(this));
					//else
					
					SetFieldHp(0);
					SetCaptainUnit(FALSE);
					SetFieldMP(0);

					m_bFieldAlive = TRUE;
					m_dwDeadTime = m_dwDeadSec = 0;
					SendFieldMsg(ON_CHANGED_WAR_END,SO_SFM_ONLYME,LPVOID(uiKind),LPVOID(this));
				}
				
				for(SI16 i = 0 ;i < ON_MAX_FOLLOWER_NUM ; i++)
				{
					OnlineFollower*	 lpOnlineFollower = IFollower.GetFollower(i);
					
					if(lpOnlineFollower != NULL)
					{
						for(SI16 j = 0; j < FIELD_BATTLE_ATTACK_UNIT_NUM ; j++)
						{
							MyItemData*		pMyItemData;
							SI16 siItemID = (SI16)pMyWorld->m_FieldBattleParser.GetItemID(j);
							
							if((pMyItemData = lpOnlineFollower->GetItem(siItemID)) != NULL)
							{
								DelItemNoQuery(i,pMyItemData->uiID,pMyItemData->uiQuantity,pMyItemData->siPosInInventory);
							}
						}
					}
				}
			}
			
			m_bFieldUnitUpdate = FALSE;
		}
	}
}
