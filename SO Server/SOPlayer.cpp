// 수정 일자 : 2002-02-14 오후 2:24:41 - 양정모

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

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
// 생성자
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
SOPlayer::SOPlayer() : BaseChar()
{
	clServer	= NULL;	

	dwPrevSavePosTime	=	0;

	// 각종 변수들의 초기화
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



//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
// 소멸자
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
SOPlayer::~SOPlayer()
{
#ifdef _GAMEGUARD
	if(m_pCSAuth != NULL ) delete m_pCSAuth;
#endif
	delete		logininfo;
	delete		m_pAuctionMarketProc;

	return;
}


//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
VOID SOPlayer::InitEnv()
{
	SI32 i = ON_MAX_PLAYERHAVEITEM;

	// 변수들을 초기화 한다.
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

	//0시에 시간 체크를 하기위한 플래그.(시간 아이템)
	m_bDayChange			 = FALSE;
	m_bFieldUnitUpdate		 = FALSE;
	m_bPlayerVillageInLimit  = FALSE;
//	ZeroMemory(&m_sWearItem,sizeof(stWearItem)*12);

	m_dwUserhour = rand()%2;
	m_dwUserMin	 = rand()%50 + 1;//pMyWorld->pRealTime->tm_min;
	m_dwUserSec	 = rand()%50 + 1;//pMyWorld->pRealTime->tm_sec;

	ZeroMemory(m_stTimerItemInfo,sizeof(OnTimerItemInfo)*12*9);
	ZeroMemory(m_pTimerItemInfo,sizeof(OnTimerItemInfo)*12);

	//파티 스레이트 추가
	m_PartyState.Init();
}



//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
// 새로 들어온 플레이어의 소켓을 설정한다.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
VOID SOPlayer::Init()
{
	// 변수들을 초기화한다.
	InitEnv();

	// 플레이어의 소켓
	
	stAccountInfo.dwIP	=	0;
	dwHeartbeatTime[0]	=	pMyWorld->dwtimeGetTime;

	return;
}


//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
// 송신 버퍼에 데이타를 쓴다.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
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



//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
// 플레이어를 생성한다.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
VOID	SOPlayer::Create( SOServer *clTempServer )
{
	// 플레이어가 생성되었다.( 접속이 이루어 지다 )

	uiState = ON_STATE_CONNECT;

	// 클레스 연결
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

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
// 사용자에 대한 모든 연산 처리
//	주의 : <메세지 받기 -> 처리 -> 메세지 보내기> 순에 따라...
//	수정 일자	:	2002-08-05 오후 2:09:01 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
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

		// 시간 (HOUR)
		if(pMyWorld->bHourChanged == TRUE)
		{		
			// 서버의 시간이 변경되었다면 시간을 보내준다		
			SendFieldMsg(ON_CHANGED_TIME, SO_SFM_ONLYME);
			
			// 날 (DAY)
			if(pMyWorld->bDayChanged == TRUE)		
			{
				// 년 (YEAR)
				if(pMyWorld->bYearChanged == TRUE)
				{				
					// 현재 접속되어 있는 플레이어는 DB에서 연간 수익을 얻어와서 보내준다.
					PlayerInfo.DBAccount	=	stAccountInfo.siDBAccount;
					PlayerInfo.uiSlot		=	stAccountInfo.uiSlot;
					
					pMyWorld->clDatabase->GetDBInfo(SO_DB_INFOKIND_GETADDMONEY, &PlayerInfo, LPVOID(&AddMoney));
					
					if(AddMoney != 0)			
						GiveBonusMoney(AddMoney, ON_RET_CHNGEMONEY_INVESTMENT, FALSE);
				}

				siDayCredit		=	0;			
			}
		}
		
		//기간이 다 된 시간제 아이템을 삭제한다.
		DeleteTimerItem();
		
		//10시가 넘으면 공성유닛의 정보를 삭제해준다.
		DeleteFiledUnit();
		
		//만약 공성유닛이 죽어서 패널티를 받고 있는 상태이면.
/*		if(!IsFieldAliveWarUnit())
		{
		
			//시간이 다 되어 패널티가 풀렸다.
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
		// 들어온 메시지들을 처리한다.
		ProcessMessage();	
		
		// 일정 시간 주기로 변경된 용병들의 정보를 DB에 저장한다.	
		
		if( pMyWorld->clDatabase->getGateQueueSize() < 100000 )
		{
			for(int i = 0; i < ON_MAX_FOLLOWER_NUM; ++i ) 
			{			

				if( ( pFollower = IFollower.GetFollower(i)) != NULL && 
					  pFollower->ParametaBox.IP.isCycleSaveToDB() )
				{
					
//					if( (int)(pMyWorld->dwtimeGetTime - pFollower->dwSaveTime)  >= ON_PARAM_SAVETIME )					
					{ 																	
						
						// 저장 한다 									
						saveFollowerDataToDB(SO_DB_INFOKIND_SAVEALLPARAM, i); 											
					}							
				}
			}	
		}
		


	}
}



//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	캐릭터의 정보를 얻어온다.	
//	수정 일자	:	2002-07-22 오후 7:41:57 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
DWORD	SOPlayer::GetInfo(OnCharPlayerInfo *pPlayerInfo)
{
	DWORD							dwUsedSize	=	sizeof(OnCharPlayerInfo);			// 캐릭터의 기본 크기를 설정한다.
	OnBaseBoothItem					*pBaseBoothItem;
	
	// 포인터를 설정해준다.
	pBaseBoothItem										=	(OnBaseBoothItem*)((CHAR*)pPlayerInfo + sizeof(OnCharPlayerInfo));

	// 캐릭터의 기본 정보를 얻어온다.
	BaseChar::GetInfo((OnCharBaseInfo*)pPlayerInfo);
	
	memset(pPlayerInfo->uiEquipWeapon, 0, sizeof(pPlayerInfo->uiEquipWeapon));
	
	pPlayerInfo->siGuildInGrade							=	siClassInGuild;
	pPlayerInfo->siFightingPower						=	GetFightingPower();
	pPlayerInfo->siDBAccount							=	stAccountInfo.siDBAccount;
	pPlayerInfo->uiEquipWeapon[ON_WEAR_HELMET_ITEM]		=	IFollower.GetHero()->Weapon.GetWearItem(ON_WEAR_HELMET_ITEM);	//	투구
	pPlayerInfo->uiEquipWeapon[ON_WEAR_WEAPON_ITEM]		=	IFollower.GetHero()->Weapon.GetWearItem(ON_WEAR_WEAPON_ITEM);	//	무기
	pPlayerInfo->uiEquipWeapon[ON_WEAR_ARMOR_ITEM]		=	IFollower.GetHero()->Weapon.GetWearItem(ON_WEAR_ARMOR_ITEM);	//	갑옷		
	pPlayerInfo->uiEquipWeapon[ON_WEAR_SHOES_ITEM]		=	IFollower.GetHero()->Weapon.GetWearItem(ON_WEAR_SHOES_ITEM);	//	신발
	pPlayerInfo->uiEquipWeapon[ON_WEAR_GUARDIAN_ITEM]	=	IFollower.GetHero()->Weapon.GetWearItem(ON_WEAR_GUARDIAN_ITEM);	//	수호부
	pPlayerInfo->siIsOverWeight							=	(SI08)IFollower.IsOverWeight();			// 무게 초과 상태인지 검사
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
	pPlayerInfo->bAttackMode	=	 GetAttackMode();		// 전투모드인가?
	
	// 상점 모드인가?
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

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	캐릭터의 상세 정보를 얻어온다.
//	수정 일자	:	2002-08-15 오후 3:29:41 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
DWORD	SOPlayer::GetDetailInfo(OnCharPlayerDetailInfo *pDetailInfo, UI32 uiRefGuildID, SI32 siRefGuildInClass, DWORD siRefDBAccount )
{
	// 메모리 초기화.
	ZeroMemory(pDetailInfo, sizeof(OnCharPlayerDetailInfo));	
	
	// 캐릭터의이름.
	strncpy(pDetailInfo->szID, szID, ON_ID_LENGTH);			
	
	if(uiGuildID != 0)
	{
		strncpy(pDetailInfo->szGuild, pMyWorld->clIGuild.GetGuildName(uiGuildID), ON_GUILDNAME_LENGTH);	
		pDetailInfo->siGuildFlag						=	siFlag;

		// 나와 인자로 넘겨준 상단과의 관게를 알아온다.
		pDetailInfo->uiIsHostilityGuild					=	FALSE;
		if(pMyWorld->clIGuildClass.GetJWJCode() != siClassInGuild)
		{
			// 현재 소속한 상단이 있고
			// 상단내에서의 직급은 '지원자'가 아니여야 한다.
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

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명	:	10초마다 Heartbeat를 보낸다.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
VOID SOPlayer::SendHeartbeat()
{
	DWORD cSendData[4];
	cSendData[0] = ON_HEARTBEAT;
	Write(cSendData, 4);
}



//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명	:	문자열 ip를 DWORD 형식으로 바꿔준다.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
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



//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명	:	돈을 갖고 있는지 검사한다.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
BOOL	SOPlayer::EnoughMoney(MONEY mnEnoughMoney)
{
	if( siMoney < mnEnoughMoney)
		return FALSE;
	return TRUE;
}



//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	돈을 더하거나 뺀다.
//	수정 일자	:	2002-08-05 오후 3:01:41 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
VOID	SOPlayer::ChangeMoney(MONEY siChangeMoney, BOOL bSendToDB)
{
	if( siChangeMoney > 0 )
		IncreaseMoney(siChangeMoney, bSendToDB);
	else if(siChangeMoney < 0)
		DecreaseMoney(-siChangeMoney, bSendToDB);
}



//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	돈을 증가시킨다.
//	수정 일자	:	2002-08-05 오후 3:02:17 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
MONEY	SOPlayer::IncreaseMoney(MONEY siIncreaseMoney, BOOL bSendToDB)
{
	DBIKSaveMoney		SaveMoney;
	MONEY				mnPrevMoney;

	if( siIncreaseMoney == 0 )		return	0;


	mnPrevMoney		=	siMoney;

	if((ON_MAX_MONEY - siMoney) >=  siIncreaseMoney)
		siMoney		+=	siIncreaseMoney;
	else
		siMoney		=	ON_MAX_MONEY;    // 10진수로 하면 약922경(922 3372 0368 5477 5807)	

	if(bSendToDB == TRUE)
	{
		// DB에 설정한다.
		SaveMoney.DBAccount	=	stAccountInfo.siDBAccount;
		SaveMoney.uiSlot	=	stAccountInfo.uiSlot;
		SaveMoney.Money		=	siMoney;
				
		pMyWorld->clDatabase->writeDBQuery(SO_DB_INFOKIND_SAVEMONEY, &SaveMoney, sizeof(SaveMoney));
	}

	return	MONEY( siMoney - mnPrevMoney );
}



//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명	:	돈을 뺀다.
//	수정 일자	:	2002-08-05 오후 3:02:26 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
BOOL	SOPlayer::DecreaseMoney(MONEY siDecreaseMoney, BOOL bSendToDB)
{
	DBIKSaveMoney		SaveMoney;
	BOOL				bResult = FALSE;

	if( siDecreaseMoney == 0 )				return	TRUE;

	if(siMoney >= siDecreaseMoney)
	{
		siMoney		-=	siDecreaseMoney;
		bResult		=	TRUE;
		
		// DB에 설정한다.		
		SaveMoney.DBAccount	=	stAccountInfo.siDBAccount;
		SaveMoney.uiSlot	=	stAccountInfo.uiSlot;
		SaveMoney.Money		=	siMoney;
	
		if(bSendToDB == TRUE)
		{
			// DB에 설정한다.		
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

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	돈을 저금한다
//	수정 일자	:	2002-11-12 오전 9:57:41 - 박준태
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
BOOL	SOPlayer::SaveMoneyAtBank(MONEY siChangeMoney)
{

	// 저금할돈이 가진돈보다 많으면 실패
	if (siMoney < siChangeMoney) return FALSE;
	// 저금할돈이 저축한계보다 많아도 실패
	if (m_MoneySaving + siChangeMoney > ON_MAX_SAVING) return FALSE;

	// 저금액을 더하고 자기돈에서 뺀다.
	m_MoneySaving += siChangeMoney;
	siMoney -= siChangeMoney;

	

	// db에 알린다.
	DBIKSaveToBank     SaveToBank;

	
	SaveToBank.DBAccount	=	stAccountInfo.siDBAccount;
	SaveToBank.uiSlot	=	stAccountInfo.uiSlot;
	SaveToBank.uiVillageCode = stAccountInfo.pVillage->uiVillageCode;
	SaveToBank.moSaveMoney = siChangeMoney;
		
	pMyWorld->clDatabase->writeDBQuery(SO_DB_INFOKIND_SAVETOBANK, &SaveToBank, sizeof(SaveToBank));

	return TRUE;
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	돈을 인출한다.
//	수정 일자	:	2002-11-12 오전 9:57:41 - 박준태
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
BOOL	SOPlayer::WithdrawMoneyAtBank(MONEY siChangeMoney)
{

	// 인출할돈이 저금액보다 많으면 실패
	if (m_MoneySaving < siChangeMoney) return FALSE;
	// 인출할돈이 소지한계보다 많아도 실패
	if (siMoney + siChangeMoney > ON_MAX_MONEY) return FALSE;

	// 저금액을 빼고 자기돈에 더한다.
	m_MoneySaving -= siChangeMoney;
	siMoney += siChangeMoney;

	

	// db에 알린다.

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




//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명	:	죽은 캐릭터를 마을 의원에 보낸다.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
VOID	SOPlayer::SendDeadPlayerToVillageHospital()
{
	SI32				siNearestHospitalVillageIndex;	
	OnlineFollower*		lpOnlineFollower;
	

	// 던전 안에 있으면 이미 가장 가까운 마을을 알고 있다.
	if(siNearestHospitalVillageIndexInDungeon != -1)
		siNearestHospitalVillageIndex	=	siNearestHospitalVillageIndexInDungeon;
	else
		siNearestHospitalVillageIndex	=	pMyWorld->GetNearestHospitalVillageIndex(uiMapID, siX, siY);
	
	// 마을 인덱스가 올바른가?
	if(siNearestHospitalVillageIndex < 0 || siNearestHospitalVillageIndex >= pMyWorld->siTotalVillageNum)
		siNearestHospitalVillageIndex = 0;

	// 유효한 마을인지 알아온다.
	stAccountInfo.pVillage	=	&pMyWorld->pclVillage[siNearestHospitalVillageIndex];

	// 주 맵으로 옮긴다.
	// 반드시 위에서 마을을 설정한 후에 해야 한다.
	ChangeMap(0, stAccountInfo.pVillage->siVillageX, stAccountInfo.pVillage->siVillageY);

	// 마을에 들어간다.
	EnterVillage();	

	// 내 캐릭터에게 마을에 들어갔다고 알려준다.
	SendFieldMsg(ON_RESPONSE_RETURNONLINE, SO_SFM_ONLYME, 
					LPVOID(ON_RET_RETURNONLINE_VILLAGE),
					LPVOID(stAccountInfo.pVillage));


	// 캐릭터를 회복 시킨다.
	lpOnlineFollower = IFollower.GetFollower(0);	
	lpOnlineFollower->ParametaBox.IP.SetLife(IFollower.GetFollowerParameterMaxLife(0));
	lpOnlineFollower->ParametaBox.IP.SetMana(IFollower.GetFollowerParameterMaxMana(0), 1);
	
	SendFieldMsg(ON_RESPONSE_HOSPITAL_CURESOLDER, SO_SFM_ONLYME, LPVOID(ON_RET_AUTOCUREOK), NULL, 
				 0, LPVOID(IFollower.GetFollowerParameterMaxLife(0)), LPVOID(IFollower.GetFollowerParameterMaxMana(0)));
}



//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명	:	배에 탄다.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
VOID	SOPlayer::BoardShip(SI32 uiboardshipaccount)
{
	uiBoardShipAccount	=	uiboardshipaccount;
}



//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	배를 타고 마을에 도착하였다.
//	수정 일자	:	2002-04-04 오전 11:02:10 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
VOID	SOPlayer::ArriveVillageByShip(SOVillage *pVillage)
{
	// 현재 타는 배가 없다고 설정한다.
	uiBoardShipAccount		=	0;
	stAccountInfo.pVillage	=	pVillage;

	// 주 맵으로 옮긴다.
	ChangeMap(0, stAccountInfo.pVillage->siVillageX, stAccountInfo.pVillage->siVillageY);

	// 마을에 들어간다.
	EnterVillage();

	// 배에서 내린다고 알려준다.
	SendFieldMsg(ON_GETOUTSHIP, SO_SFM_ONLYME, LPVOID(pVillage));
}



//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명	:	초기화를 한다.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
VOID	SOPlayer::Init(UI16 uiAccount, UI16 uiMapID, SI16 siX, SI16 siY, UI16 uiKind, CHAR *pszID)
{
	BaseChar::Init(uiAccount, uiMapID, siX, siY, uiKind, pszID);
	
	// 2003.10.7 추가, 정성운 
	m_PartyState.Init();
}



//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명	:	맵을 변경한다.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
BOOL	SOPlayer::ChangeMap(UI16 uimapid, SI16 six, SI16 siy)
{
	BOOL	bSuccess = FALSE;

	// 이동하려는 맵이 uimapid가 유효한 맵이 아니면 FALSE를 리턴한다.
	if(pMyWorld->clISOMap.IsValidMapID(uimapid) == FALSE)	goto	END;		

	// 만약 현재 맵과 이동하려는 맵이 다르다면 그에 따른 처리를 해준다.
	if(uiMapID != uimapid)
	{
		// 아직은 모든 던전에서는 마을이 없다는 가정하에서 작성한다
		// 그리고 맵 속성이 던전이 아니라면 주 맵이다.

		// 주 맵 -> 던전
		if((pMyWorld->clISOMap.IsDungeon(uiMapID) == FALSE) && (pMyWorld->clISOMap.IsDungeon(uimapid) == TRUE))
		{	
			SetNearestHospitalVillageIndexInDungeon(pMyWorld->GetNearestHospitalVillageIndex(uiMapID, siX, siY));
		}

		// 던전 -> 던전  -- 이전의 siNearestHospitalVillageIndexInDungeon의 값을 그대로 가지고 있으면 된다.
		else if((pMyWorld->clISOMap.IsDungeon(uiMapID) == TRUE) && (pMyWorld->clISOMap.IsDungeon(uimapid) == TRUE));
			
		// ??? -> 주 맵
		else if(pMyWorld->clISOMap.IsDungeon(uimapid) == FALSE) 
		{
			SetNearestHospitalVillageIndexInDungeon(-1);		
		}	
	}
	
	// 타 유저에게 파티 신청중이거나 
	// 다른 유저에 파티 신청을 받았다면 이를 취소한다.
	CloseRequestingJoinParty();

	bSuccess	=	TRUE;

END:
	if(bSuccess == TRUE)
	{
		// 맵을 변경한다.
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

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	현재 돈을 보내준다.
//	수정 일자	:	2002-08-17 오후 7:13:39 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
VOID	SOPlayer::SendMoney(DWORD dwKind)
{
	SendFieldMsg(ON_CHANGED_MONEY, SO_SFM_ONLYME, LPVOID(dwKind));
}



//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	보너스 돈을 준다. 뺏을 수도 있다.
//	수정 일자	:	2002-08-05 오후 2:59:52 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
VOID	SOPlayer::GiveBonusMoney(MONEY Bonus, DWORD dwKind, BOOL bSendToDB)
{
	ChangeMoney(Bonus, bSendToDB);
	SendMoney(dwKind);
}



//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명	:	보너스를 증가시켜준다.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
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

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	신용도를 증가시켜준다.
//	수정 일자	:	2002-08-12 오후 2:24:04 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
BOOL	SOPlayer::IncreaseCredit(SI32 siCredit)
{
	IncreaseTradeCredit(siCredit);
	return	TRUE;
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명	:	증가시켜줄 신용도를 구한다.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
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



//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명	:	이름에 이상한 문자가 있는지 확인한다. TRUE : 사용가능, FALSE : 사용 불가
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
BOOL	SOPlayer::CheckForbiddenName(char *szName)
{
	CHAR	*pszToken;
	CHAR	szTempBuffer[4096];

	ZeroMemory(szTempBuffer, sizeof(szTempBuffer));
	strcpy(szTempBuffer, szName);

	// 깨진 멀티 바이트 캐릭터가 있는지 체크한다.
	if( !CheckLeadByte(szTempBuffer) )		return FALSE;

	pszToken	=	strtok(szName, ON_FORBIDDEN_NAME);
	if(pszToken == NULL)	return	FALSE;

	if(strlen(szTempBuffer) == strlen(pszToken))
		return	TRUE;
	else
		return	FALSE;
}



//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	포만감을 늘린다.
//	수정 일자	:	2002-05-27 오전 10:54:31 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
VOID	SOPlayer::ChangeSatiety(SI32 siValue)
{	

	SI32					i;	
	UI08					uiFollowerNum;	
	BOOL					bMinusHealth = FALSE;
	strHungryFollowerInfo	stHFI[ON_MAX_FOLLOWER_NUM];

	// 포만감을 변경 한다 
	if((m_satiety + siValue) < 0)	bMinusHealth	=	TRUE;		
	m_satiety		=	min(max(m_satiety + siValue, 0), ON_MAX_HEALTH);	

	if(bMinusHealth == FALSE)
	{	
		// 메시지를 보내준다.
		SendFieldMsg(ON_HUNGRY, SO_SFM_ONLYME, LPVOID(ON_RET_HUR_CHANGE_HEALTH), LPVOID(m_satiety));
	}
	else
	{
		// 내가 가지고 있는 모든 캐릭터의 체력을 갂는다.	
		IFollower.AllCharacterDeleteLife(10);				

		uiFollowerNum	=	0;
		for(i = 0; i < ON_MAX_FOLLOWER_NUM; i++)
		{
			if(IFollower.IsValidFollower(i) == TRUE)
			{			
				stHFI[uiFollowerNum].uiFollowerID		=	i;
				stHFI[uiFollowerNum].siLife			=	IFollower.GetFollowerParameterLife(i);								

				// 제일 마지막으로 증가시켜준다.
				uiFollowerNum++;
			}
		}
		
		// 메시지를 보내준다.
		SendFieldMsg(ON_HUNGRY, SO_SFM_ONLYME, LPVOID(ON_RET_HUR_CHANGE_HP), LPVOID(uiFollowerNum), LPVOID(stHFI));
	}		

}



//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	HeartBeat를 받는다.
//	수정 일자	:	2002-03-21 오전 10:10:50 - 이성재
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
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

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	상단 깃발을 변경한다.
//	수정 일자	:	2002-04-04 오후 3:21:42 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
VOID	SOPlayer::ChangeGuildFlag(SI32 siflag)
{
	siFlag	=	siflag;

	// 메시지를 보낸다.
	SendFieldMsg(ON_RESPONSE_CHANGE_GUILD_FLAG, SO_SFM_ONLYME, LPVOID(ON_RET_OK), LPVOID(siflag));			
}



//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	던전으로 들어가기전에 죽어서 던전 입구에서 제일 가까운 죽어서 마을의 인덱스를 저장한다.
//	수정 일자	:	2002-04-15 오후 4:35:18 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
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
	
	// DB에 저장한다.	
	pMyWorld->clDatabase->writeDBQuery(SO_DB_INFOKIND_NEARESTHVCODE, &NearestHVCode, sizeof(NearestHVCode));
}



//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	모든 Character의 Life를 뺀다.
//	수정 일자	:	2002-05-27 오전 10:44:24 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
VOID	SOPlayer::AllCharacterDeleteLifePoint(SI32 DeleteLife)
{
	IFollower.AllCharacterDeleteLifePoint(DeleteLife);	
}





//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	신용 등급을 얻어온다.
//	수정 일자	:	2002-06-26 오후 2:54:52 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
SI32	SOPlayer::GetTradeGrade()
{
	const TradeHeader* pTradeHeader	=	pMyWorld->pTradeParser->GetHeader(uiTradeCredit);	

	if(pTradeHeader != NULL)	return pTradeHeader->uiTradeGrade;	
	else						return	0;	
}




//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	현재 교역이 가능한가?
//	수정 일자	:	2002-07-04 오전 10:45:07 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
BOOL	SOPlayer::CanTrade()
{
	// 현재 필드에 있지 않다면 FALSE를 리턴한다.
	if( CHECKSTATUS( siStatus, ON_PLAYER_STATUS_INFIELD ) == FALSE )	return	FALSE;
	//if(siStatus != ON_PLAYER_STATUS_INFIELD)	return	FALSE;

	// 현재 전투 대기중이거나 전투중에 있다면 FALSE를 리턴한다.
	if(GetBattle() != NULL)						return	FALSE;

	// 현재 다른 플레이어와 교역 중이면 FALSE를 리턴한다.
	if(IsTrading() == TRUE)						return	FALSE;

	// 개인 상점 모드이면 FALSE를 리턴한다.
	if(clPrivateMarket.IsOpen() == TRUE)		return	FALSE;

	return	TRUE;
}



//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	현재 다른 플레이어와 교역중인지 알아온다.
//	수정 일자	:	2002-07-06 오후 2:02:49 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
BOOL	SOPlayer::IsTrading()
{
	return clTrade.IsTrading();
}



//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	플레이어의 숫자로 이루어진 IP를 받아온다.
//	수정 일자	:	2002-07-04 오후 1:20:13 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
DWORD	SOPlayer::GetIPInNumeric()
{
	return	stAccountInfo.dwIP;
}



//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	교역할 아이템을 선택했는지 알아온다.
//	수정 일자	:	2002-07-06 오후 8:56:44 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
BOOL	SOPlayer::IsChoseTradeItem(UI16 uiAccount)
{
	return clTrade.IsChoseTradeItem(uiAccount);
}



//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	교역할 아이템을 선택했던 것을 취소한다.
//	수정 일자	:	2002-07-06 오후 10:40:59 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
BOOL	SOPlayer::CancelChooseTradeItem()
{
	return	clTrade.CancelChooseTradeItem();
}



//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	선택했던 아이템들을 가지고 교역할 것을 결정한다.
//	수정 일자	:	2002-07-07 오전 12:46:22 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
BOOL	SOPlayer::DecideTrade()
{
	return	clTrade.DecideTrade();
}



//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	전투를 시작한다.
//	수정 일자	:	2002-07-08 오후 8:43:28 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
VOID	SOPlayer::PrepareBattle()
{	
	CloseTrade();

	// 타 유저에게 파티 신청중이거나 
	// 다른 유저에 파티 신청을 받았다면 이를 취소한다.
	CloseRequestingJoinParty();

	BaseChar::PrepareBattle();
}



//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	전투를 종료한다.
//	수정 일자	:	2002-05-14 오전 2:26:56 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
VOID	SOPlayer::EndBattle()
{
	// 우선 반드시 전투를 종료한다.	
	BaseChar::EndBattle();
}



//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	장수를 가지고 있는지 알아온다.
//	수정 일자	:	2002-07-15 오후 9:39:28 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
BOOL	SOPlayer::HaveGeneral()
{
	SI32	i;

	for(i = 0; i < ON_MAX_FOLLOWER_NUM; i++)
	{
		if(IsGeneral(i) == TRUE)	return	TRUE;
	}

	return	FALSE;
}



//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	장수인지 알아온다.
//	수정 일자	:	2002-07-15 오후 9:40:25 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
BOOL	SOPlayer::IsGeneral(UI08 uiFollowerSlot)
{
	UI16	uiCharKind;

	if((uiCharKind = IFollower.GetFollowerCharID(uiFollowerSlot)) == 0)	return	FALSE;
	
	if(pMyWorld->clSOCharKI.GetGeneral(uiCharKind) == 1)	  return	TRUE;
	else if(pMyWorld->clSOCharKI.GetGeneral(uiCharKind) == 3) return	TRUE;
	else													return	FALSE;
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	현재 캐릭터가 공성유닛인지 알아온다.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
BOOL	SOPlayer::IsFieldAttackUnit()
{
//	UI16	uiCharkind;

//	if((uiCharkind = IFollower.GetFollowerCharID(0)) == 0)	return	FALSE;

	return CSiegeUnitTable::GetInstance()->IsFieldAttackUnit(uiKind);
	
//	return pMyWorld->clSOCharKI.IsFieldAttackUnit(uiKind);

}



//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	용병이 몬스터인지 알아온다.
//	수정 일자	:	2003-02-24 오후 8:55 - 백상현
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
BOOL	SOPlayer::IsMonster( UI08 uiFollowerSlot )
{
	UI16	uiCharKind;

	if( (uiCharKind = IFollower.GetFollowerCharID(uiFollowerSlot)) == 0 )	return	FALSE;
	
	// siGeneral(장수값) : 일반용병(0), 장수(1), 몬스터(2)
	if( pMyWorld->clSOCharKI.GetGeneral(uiCharKind) == 2 )	return	TRUE;
	else													return	FALSE;
}


//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	현재 보유하고 있는 몬스터 수를 얻어온다.
//	수정 일자	:	2003-02-24 오후 8:55 - 백상현
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
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


//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	플레이어가 가질 수 있는 최대 용병의 수.(주인공 포함)
//	수정 일자	:	2002-07-25 오후 4:39:34 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
SI16	SOPlayer::GetMaxFollowerNum()
{
	return pMyWorld->clMercenaryParser.GetMaxFollowerNum(GetTradeGrade(), IFollower.GetFollowerParameterLevel(0));
}


//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	플레이어가 운영자인지 알아온다.
//	수정 일자	:	2002-08-05 오후 1:49:58 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
BOOL	SOPlayer::IsAdminPlayer()
{
	if(USGrade != ON_USERGRADE_NORMAL)	return	TRUE;
	else								return	FALSE;
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	내가 용병을 추가할 수 있는지 알아온다.
//	수정 일자	:	2002-09-24 오후 5:56:41 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
BOOL	SOPlayer::CanAddFollower( UI16 uiFollowerNum )
{
	// IFollower 객체에게 용병을 추가할 수 있는지 알아온다.
	if( IFollower.CanAddFollower( uiFollowerNum ) == FALSE )						return	FALSE;
	
	// 최대 가질 수 있는 용병보다 더 많은 용병을 주려고 하는지 알아온다.
	// -1 을 하는 이유는 GetMaxFollowerNum() 함수가 
	// 주인공을 제외한 용병의 수를 리턴하기 때문이다.
	// 하지만 IFollower.GetFollowerNum() 함수는 주인공을 포함한 용병의 수를 알려준다.
	if( GetMaxFollowerNum() < ( IFollower.GetFollowerNum() + uiFollowerNum - 1 ) )		return	FALSE;
	
	return	TRUE;
}



bool	SOPlayer::expUp(int followerIndex, int value)
{				
	// 유효한 용병일 경우 
	if( IFollower.GetFollower(followerIndex) )
	{
		_ImportantParameta* pParam  = &IFollower.GetFollower(followerIndex)->ParametaBox.IP;
		
		if(value > 10000)
			writeInfoToFile("ExpLog.txt", "[charName: %s][IP: %s][EXP:%d]", szID, stAccountInfo.IP, value);//경험치 로그를 남긴다.

		// 경험치를 증가 시킨다 
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

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	상대방에게 돈을 준다.
//	수정 일자	:	2002-09-30 오후 5:04:28 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
BOOL	SOPlayer::GiveMoney( MONEY *pMoney, SOPlayer *pOppPlayer )
{
	MONEY	mnMaxMoneyToGet;

	// 최대 가질 수 있는 돈을 구한다.
	mnMaxMoneyToGet	=	pOppPlayer->GetMaxMoneyToGet();

	// 최대 가질 수 있는 돈보다 많은 돈을 줄 수는 없다.
	if( mnMaxMoneyToGet < *pMoney )							return	FALSE;

	if( DecreaseMoney( *pMoney, FALSE ) == FALSE )			return	FALSE;

	// 뺀 돈을 상대방에게 준다.
	pOppPlayer->IncreaseMoney( *pMoney, FALSE ); 

	return	TRUE;
}


//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	가질 수 있는 최대돈을 얻어온다.
//	수정 일자	:	2002-09-30 오후 5:19:01 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
MONEY	SOPlayer::GetMaxMoneyToGet()
{	
	return	(ON_MAX_MONEY - siMoney);
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	현재 마을 안에 있는지 알아온다.
//	수정 일자	:	2002-10-02 오후 7:55:24 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
BOOL	SOPlayer::IsInVillage()
{
	if( stAccountInfo.pVillage == NULL )		return	FALSE;
	
	
	return	TRUE;
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	현재 마을의 건물 안에 들어갔는지 검사한다.
//	수정 일자	:	2002-10-02 오후 7:55:37 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
BOOL	SOPlayer::IsEnteredStructure()
{
	return	FALSE;
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	현재 돈,저금액을 얻어온다.
//	수정 일자	:	2002-11-22 오후 4:05:?? - 박준태
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

MONEY	SOPlayer::GetMoney()
{	
	return	siMoney;
}
MONEY	SOPlayer::GetSaving()
{	
	return	m_MoneySaving;
}







//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
// kojimodo
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

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
// Desc : 자기의 현재 위치를 DB에 저장한다
//--------------------------------------------------------------------------------------------
void SOPlayer::savePosToDB()
{
	// 이전과 맵아이디가 틀리거나 이전 저장 위치와 20이상 차이가 날 경우에만 저장 한다 
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
// Desc : 모든 용병의 현재 정보를 DB에 저장한다 (!! 로그 아웃시에만 써야 함 !!)
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
// Desc : 한 용병의 현재 정보를 DB에 저장한다 
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
// Desc : 로그아웃시 필요한 정보를 DB에 저장 한다 
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
			LogoutWarInfo.PrevKind	=	(WORD)uiKind;		// 공성중일때 공성중인 캐릭터를 저장한다.

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
// Desc : 로그아웃을 시작 하는 플래그를 설정 한다 
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
// Desc : 포만감을 DB에 저장 한다
//--------------------------------------------------------------------------------------------
void SOPlayer::saveSatietyToDB()
{
	// 원본 포만감과 다를 경우에만 저장 한다 
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
// Desc : 이 유저가 위치한 경매소를 얻어온다 ( 경매소에 위치하지 않았을 경우 NULL )
//--------------------------------------------------------------------------------------------
CAuctionMarket* SOPlayer::getAuctionMarket()
{
	return pMyWorld->m_pAuctionMarket;
}



//--------------------------------------------------------------------------------------------
// Name :
// Desc : 이 유저를 전투안에서 제거 한다 
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
// Desc : 이 유저를 경매소안에서 제거 한다 
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
// Desc : 이 유저를 게임안에서 제거 한다 
//--------------------------------------------------------------------------------------------
VOID SOPlayer::removeSelfInGame()
{
	// 게임에 성공적으로 로그인 했다면 게임에 관련된 부분에서도 삭제해준다.
	if(bSuccessLogin == TRUE)
	{
		// 필드라면.
		// 맵에서 해당 캐릭터의 id를 삭제한다.
		// 우선 맵에 설정되어 있는게 자신이 맞는지 확인한다.		
		if(siStatus	& ON_PLAYER_STATUS_INFIELD)
		{
			// 현재 배에 타고 있다면 배에 승선한 리스트에 있는 나를 제거한다.
			if(uiBoardShipAccount != 0)
			{
				pMyWorld->clSOShip.LogOutPlayer(uiBoardShipAccount, uiAccount, stAccountInfo.siDBAccount);
			}
			else
			{
				// 맵에서 ID를 삭제한다.
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
		
		else if(siStatus == ON_PLAYER_STATUS_INVILLAGE)	// 마을이라면...
		{
			stAccountInfo.pVillage->PlayerLogOut(this);
		}


		// 전투에서 나온다 
		removeSelfInBattle();

		// 경매소에서 나온다 
		removeSelfInAuctionMarket();

		// 교역창을 닫는다.
		CloseTrade();

		// 나를 친구로 등록한 사람들에게 내가 로그아웃함을 알린다.
		NotifyFriendsOfLoginState( FALSE );
				
		// 타 유저에게 파티 신청중이거나 
		// 다른 유저에 파티 신청을 받았다면 이를 취소한다.
		CloseRequestingJoinParty();

		// 파티원이라면 파티에서 탈퇴한다.
		if( IsPartyMember() == TRUE )
		{
			LeaveParty();
		}

		// 정식 상단원일 경우 상단 관리자에게 로그아웃 하였다고 알려준다.
		if( ( uiGuildID != 0 ) && ( siClassInGuild != pMyWorld->clIGuildClass.GetJWJCode() ) )
		{
			pMyWorld->clIGuild.GuildMemberLogout( uiGuildID );
		}

	}
	else
	{
		// 로그인을 하지 않았다.
		printf("[%s] didn't login.\n", szID);
	}		
}



//--------------------------------------------------------------------------------------------
// Name :
// Desc : 유효한 GUID 인가?
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



//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	아이템을 이동 시킨다.
//	수정 일자	:	2002-11-12 오후 1:16:38 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
BOOL	SOPlayer::MoveItem( UI08 uiFollowerSlot1, UI16 uiItemID, UI16 uiItemQuantity, UI08 uiFollowerSlot2, SI16 siPosInInventory )
{	
	CHAR		szDBArgList[ 1024 ];	

	if( IFollower.MoveItem( uiFollowerSlot1, uiItemID, uiItemQuantity, uiFollowerSlot2, &siPosInInventory ) == FALSE )		return	FALSE;

	// DB에 저장한다.
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

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	
//	수정 일자	:	2003-01-04 오후 1:40:19 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
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


//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	전투모드 / 평화모드 설정
//	수정 일자	:	2003-09-22 오후 1:50 - 백상현
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
VOID	SOPlayer::RecvSetAttackMode()
{
	OnReqSetAttackMode			*pRecvMsg;

	pRecvMsg		=	(OnReqSetAttackMode*)cMsg;

	SetAttackMode( pRecvMsg->bAttackMode );
	SendFieldMsg( ON_RESPONSE_SET_ATTACK_MODE, SO_SFM_ONLYNEARCHAR, LPVOID(pRecvMsg->bAttackMode) );
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	장부의 상단 정보를 얻는다.
//	수정 일자	:	2004-01-05 PM __Lee_min_su__
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
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
				UnSetGuildInfoChanged();	// 상단정보가 변경 Flag를 
											// FALSE로 변경한다.
		}
		// 최종적으로 보내준 길드 정보와 변한게 없으면..
		else
		{
			SendFieldMsg(ON_RESPONSE_AUDIT_GUILD_INFO, \
				SO_SFM_ONLYME, LPVOID(0), LPVOID(0), LPVOID(-1) );
		}
	}
	else
	{
		// 상단원이 아니면...
		SendFieldMsg(ON_RESPONSE_AUDIT_GUILD_INFO, \
			SO_SFM_ONLYME, LPVOID(0), LPVOID(0), LPVOID(-2));
	}	
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	매크로를 사용한 유저의 정보를 저장한다.
//	수정 일자	:	2004-04-10 PM __Lee_min_su__
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
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

				//클라이언트에 보낼 정보를 저장.
				memcpy(&m_stTimerItemInfo[TempTimerItemInfo.siMercenarySlot][TempTimerItemInfo.siLoc],&TempTimerItemInfo,sizeof(OnTimerItemInfo));

				if(TempTimerItemInfo.bDeleteTimerItem)
				{
					siTimerItemUseCount++;
					TempTimerItemInfo.siItemID = 0;
				}
				
				//서버의 아이템 정보를 저장한다.
				pFollower->SetWearItem(i,TempTimerItemInfo.siItemID,TempTimerItemInfo.siRestTime);

				//삭제 되는 아이템의 경우 DB에 알려준다.
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

//아이템 추가 서비스 시간을 더하고 삭제할 아이템인지 아닌지 판단한다.
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
	//이것도 윤달 신경 안섰음.
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
	//윤달 신경 안썼음.
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
						//체크
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
							//체크 결과에 따라.
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
	//DB에서 일괄적으로 캐릭터의 공성 관련 정보를 지워준후 
	//서버에서 지우고 클라이언트에 알린다.
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
					
					if(m_bPlayerVillageInLimit) //수성 인터페이스에 들어와있는 상태인경우
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
