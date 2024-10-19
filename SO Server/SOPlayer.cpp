// ���� ���� : 2002-02-14 ���� 2:24:41 - ������

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

//��������������������������������������������������������������������������������������������������������������������������������������������������������������
// ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������
SOPlayer::SOPlayer() : BaseChar()
{
	clServer	= NULL;	

	dwPrevSavePosTime	=	0;

	// ���� �������� �ʱ�ȭ
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



//��������������������������������������������������������������������������������������������������������������������������������������������������������������
// �Ҹ���
//��������������������������������������������������������������������������������������������������������������������������������������������������������������
SOPlayer::~SOPlayer()
{
#ifdef _GAMEGUARD
	if(m_pCSAuth != NULL ) delete m_pCSAuth;
#endif
	delete		logininfo;
	delete		m_pAuctionMarketProc;

	return;
}


//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//
//��������������������������������������������������������������������������������������������������������������������������������������������������������������
VOID SOPlayer::InitEnv()
{
	SI32 i = ON_MAX_PLAYERHAVEITEM;

	// �������� �ʱ�ȭ �Ѵ�.
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

	//0�ÿ� �ð� üũ�� �ϱ����� �÷���.(�ð� ������)
	m_bDayChange			 = FALSE;
	m_bFieldUnitUpdate		 = FALSE;
	m_bPlayerVillageInLimit  = FALSE;
//	ZeroMemory(&m_sWearItem,sizeof(stWearItem)*12);

	m_dwUserhour = rand()%2;
	m_dwUserMin	 = rand()%50 + 1;//pMyWorld->pRealTime->tm_min;
	m_dwUserSec	 = rand()%50 + 1;//pMyWorld->pRealTime->tm_sec;

	ZeroMemory(m_stTimerItemInfo,sizeof(OnTimerItemInfo)*12*9);
	ZeroMemory(m_pTimerItemInfo,sizeof(OnTimerItemInfo)*12);

	//��Ƽ ������Ʈ �߰�
	m_PartyState.Init();
}



//��������������������������������������������������������������������������������������������������������������������������������������������������������������
// ���� ���� �÷��̾��� ������ �����Ѵ�.
//��������������������������������������������������������������������������������������������������������������������������������������������������������������
VOID SOPlayer::Init()
{
	// �������� �ʱ�ȭ�Ѵ�.
	InitEnv();

	// �÷��̾��� ����
	
	stAccountInfo.dwIP	=	0;
	dwHeartbeatTime[0]	=	pMyWorld->dwtimeGetTime;

	return;
}


//��������������������������������������������������������������������������������������������������������������������������������������������������������������
// �۽� ���ۿ� ����Ÿ�� ����.
//��������������������������������������������������������������������������������������������������������������������������������������������������������������
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



//��������������������������������������������������������������������������������������������������������������������������������������������������������������
// �÷��̾ �����Ѵ�.
//��������������������������������������������������������������������������������������������������������������������������������������������������������������
VOID	SOPlayer::Create( SOServer *clTempServer )
{
	// �÷��̾ �����Ǿ���.( ������ �̷�� ���� )

	uiState = ON_STATE_CONNECT;

	// Ŭ���� ����
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

//��������������������������������������������������������������������������������������������������������������������������������������������������������������
// ����ڿ� ���� ��� ���� ó��
//	���� : <�޼��� �ޱ� -> ó�� -> �޼��� ������> ���� ����...
//	���� ����	:	2002-08-05 ���� 2:09:01 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������
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

		// �ð� (HOUR)
		if(pMyWorld->bHourChanged == TRUE)
		{		
			// ������ �ð��� ����Ǿ��ٸ� �ð��� �����ش�		
			SendFieldMsg(ON_CHANGED_TIME, SO_SFM_ONLYME);
			
			// �� (DAY)
			if(pMyWorld->bDayChanged == TRUE)		
			{
				// �� (YEAR)
				if(pMyWorld->bYearChanged == TRUE)
				{				
					// ���� ���ӵǾ� �ִ� �÷��̾�� DB���� ���� ������ ���ͼ� �����ش�.
					PlayerInfo.DBAccount	=	stAccountInfo.siDBAccount;
					PlayerInfo.uiSlot		=	stAccountInfo.uiSlot;
					
					pMyWorld->clDatabase->GetDBInfo(SO_DB_INFOKIND_GETADDMONEY, &PlayerInfo, LPVOID(&AddMoney));
					
					if(AddMoney != 0)			
						GiveBonusMoney(AddMoney, ON_RET_CHNGEMONEY_INVESTMENT, FALSE);
				}

				siDayCredit		=	0;			
			}
		}
		
		//�Ⱓ�� �� �� �ð��� �������� �����Ѵ�.
		DeleteTimerItem();
		
		//10�ð� ������ ���������� ������ �������ش�.
		DeleteFiledUnit();
		
		//���� ���������� �׾ �г�Ƽ�� �ް� �ִ� �����̸�.
/*		if(!IsFieldAliveWarUnit())
		{
		
			//�ð��� �� �Ǿ� �г�Ƽ�� Ǯ�ȴ�.
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
		// ���� �޽������� ó���Ѵ�.
		ProcessMessage();	
		
		// ���� �ð� �ֱ�� ����� �뺴���� ������ DB�� �����Ѵ�.	
		
		if( pMyWorld->clDatabase->getGateQueueSize() < 100000 )
		{
			for(int i = 0; i < ON_MAX_FOLLOWER_NUM; ++i ) 
			{			

				if( ( pFollower = IFollower.GetFollower(i)) != NULL && 
					  pFollower->ParametaBox.IP.isCycleSaveToDB() )
				{
					
//					if( (int)(pMyWorld->dwtimeGetTime - pFollower->dwSaveTime)  >= ON_PARAM_SAVETIME )					
					{ 																	
						
						// ���� �Ѵ� 									
						saveFollowerDataToDB(SO_DB_INFOKIND_SAVEALLPARAM, i); 											
					}							
				}
			}	
		}
		


	}
}



//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	ĳ������ ������ ���´�.	
//	���� ����	:	2002-07-22 ���� 7:41:57 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������
DWORD	SOPlayer::GetInfo(OnCharPlayerInfo *pPlayerInfo)
{
	DWORD							dwUsedSize	=	sizeof(OnCharPlayerInfo);			// ĳ������ �⺻ ũ�⸦ �����Ѵ�.
	OnBaseBoothItem					*pBaseBoothItem;
	
	// �����͸� �������ش�.
	pBaseBoothItem										=	(OnBaseBoothItem*)((CHAR*)pPlayerInfo + sizeof(OnCharPlayerInfo));

	// ĳ������ �⺻ ������ ���´�.
	BaseChar::GetInfo((OnCharBaseInfo*)pPlayerInfo);
	
	memset(pPlayerInfo->uiEquipWeapon, 0, sizeof(pPlayerInfo->uiEquipWeapon));
	
	pPlayerInfo->siGuildInGrade							=	siClassInGuild;
	pPlayerInfo->siFightingPower						=	GetFightingPower();
	pPlayerInfo->siDBAccount							=	stAccountInfo.siDBAccount;
	pPlayerInfo->uiEquipWeapon[ON_WEAR_HELMET_ITEM]		=	IFollower.GetHero()->Weapon.GetWearItem(ON_WEAR_HELMET_ITEM);	//	����
	pPlayerInfo->uiEquipWeapon[ON_WEAR_WEAPON_ITEM]		=	IFollower.GetHero()->Weapon.GetWearItem(ON_WEAR_WEAPON_ITEM);	//	����
	pPlayerInfo->uiEquipWeapon[ON_WEAR_ARMOR_ITEM]		=	IFollower.GetHero()->Weapon.GetWearItem(ON_WEAR_ARMOR_ITEM);	//	����		
	pPlayerInfo->uiEquipWeapon[ON_WEAR_SHOES_ITEM]		=	IFollower.GetHero()->Weapon.GetWearItem(ON_WEAR_SHOES_ITEM);	//	�Ź�
	pPlayerInfo->uiEquipWeapon[ON_WEAR_GUARDIAN_ITEM]	=	IFollower.GetHero()->Weapon.GetWearItem(ON_WEAR_GUARDIAN_ITEM);	//	��ȣ��
	pPlayerInfo->siIsOverWeight							=	(SI08)IFollower.IsOverWeight();			// ���� �ʰ� �������� �˻�
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
	pPlayerInfo->bAttackMode	=	 GetAttackMode();		// ��������ΰ�?
	
	// ���� ����ΰ�?
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

//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	ĳ������ �� ������ ���´�.
//	���� ����	:	2002-08-15 ���� 3:29:41 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
DWORD	SOPlayer::GetDetailInfo(OnCharPlayerDetailInfo *pDetailInfo, UI32 uiRefGuildID, SI32 siRefGuildInClass, DWORD siRefDBAccount )
{
	// �޸� �ʱ�ȭ.
	ZeroMemory(pDetailInfo, sizeof(OnCharPlayerDetailInfo));	
	
	// ĳ�������̸�.
	strncpy(pDetailInfo->szID, szID, ON_ID_LENGTH);			
	
	if(uiGuildID != 0)
	{
		strncpy(pDetailInfo->szGuild, pMyWorld->clIGuild.GetGuildName(uiGuildID), ON_GUILDNAME_LENGTH);	
		pDetailInfo->siGuildFlag						=	siFlag;

		// ���� ���ڷ� �Ѱ��� ��ܰ��� ���Ը� �˾ƿ´�.
		pDetailInfo->uiIsHostilityGuild					=	FALSE;
		if(pMyWorld->clIGuildClass.GetJWJCode() != siClassInGuild)
		{
			// ���� �Ҽ��� ����� �ְ�
			// ��ܳ������� ������ '������'�� �ƴϿ��� �Ѵ�.
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

//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	10�ʸ��� Heartbeat�� ������.
//��������������������������������������������������������������������������������������������������������������������������������������������������������������
VOID SOPlayer::SendHeartbeat()
{
	DWORD cSendData[4];
	cSendData[0] = ON_HEARTBEAT;
	Write(cSendData, 4);
}



//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	���ڿ� ip�� DWORD �������� �ٲ��ش�.
//��������������������������������������������������������������������������������������������������������������������������������������������������������������
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



//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	���� ���� �ִ��� �˻��Ѵ�.
//��������������������������������������������������������������������������������������������������������������������������������������������������������������
BOOL	SOPlayer::EnoughMoney(MONEY mnEnoughMoney)
{
	if( siMoney < mnEnoughMoney)
		return FALSE;
	return TRUE;
}



//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	���� ���ϰų� ����.
//	���� ����	:	2002-08-05 ���� 3:01:41 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������
VOID	SOPlayer::ChangeMoney(MONEY siChangeMoney, BOOL bSendToDB)
{
	if( siChangeMoney > 0 )
		IncreaseMoney(siChangeMoney, bSendToDB);
	else if(siChangeMoney < 0)
		DecreaseMoney(-siChangeMoney, bSendToDB);
}



//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	���� ������Ų��.
//	���� ����	:	2002-08-05 ���� 3:02:17 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������
MONEY	SOPlayer::IncreaseMoney(MONEY siIncreaseMoney, BOOL bSendToDB)
{
	DBIKSaveMoney		SaveMoney;
	MONEY				mnPrevMoney;

	if( siIncreaseMoney == 0 )		return	0;


	mnPrevMoney		=	siMoney;

	if((ON_MAX_MONEY - siMoney) >=  siIncreaseMoney)
		siMoney		+=	siIncreaseMoney;
	else
		siMoney		=	ON_MAX_MONEY;    // 10������ �ϸ� ��922��(922 3372 0368 5477 5807)	

	if(bSendToDB == TRUE)
	{
		// DB�� �����Ѵ�.
		SaveMoney.DBAccount	=	stAccountInfo.siDBAccount;
		SaveMoney.uiSlot	=	stAccountInfo.uiSlot;
		SaveMoney.Money		=	siMoney;
				
		pMyWorld->clDatabase->writeDBQuery(SO_DB_INFOKIND_SAVEMONEY, &SaveMoney, sizeof(SaveMoney));
	}

	return	MONEY( siMoney - mnPrevMoney );
}



//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	���� ����.
//	���� ����	:	2002-08-05 ���� 3:02:26 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������
BOOL	SOPlayer::DecreaseMoney(MONEY siDecreaseMoney, BOOL bSendToDB)
{
	DBIKSaveMoney		SaveMoney;
	BOOL				bResult = FALSE;

	if( siDecreaseMoney == 0 )				return	TRUE;

	if(siMoney >= siDecreaseMoney)
	{
		siMoney		-=	siDecreaseMoney;
		bResult		=	TRUE;
		
		// DB�� �����Ѵ�.		
		SaveMoney.DBAccount	=	stAccountInfo.siDBAccount;
		SaveMoney.uiSlot	=	stAccountInfo.uiSlot;
		SaveMoney.Money		=	siMoney;
	
		if(bSendToDB == TRUE)
		{
			// DB�� �����Ѵ�.		
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

//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	���� �����Ѵ�
//	���� ����	:	2002-11-12 ���� 9:57:41 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������
BOOL	SOPlayer::SaveMoneyAtBank(MONEY siChangeMoney)
{

	// �����ҵ��� ���������� ������ ����
	if (siMoney < siChangeMoney) return FALSE;
	// �����ҵ��� �����Ѱ躸�� ���Ƶ� ����
	if (m_MoneySaving + siChangeMoney > ON_MAX_SAVING) return FALSE;

	// ���ݾ��� ���ϰ� �ڱ⵷���� ����.
	m_MoneySaving += siChangeMoney;
	siMoney -= siChangeMoney;

	

	// db�� �˸���.
	DBIKSaveToBank     SaveToBank;

	
	SaveToBank.DBAccount	=	stAccountInfo.siDBAccount;
	SaveToBank.uiSlot	=	stAccountInfo.uiSlot;
	SaveToBank.uiVillageCode = stAccountInfo.pVillage->uiVillageCode;
	SaveToBank.moSaveMoney = siChangeMoney;
		
	pMyWorld->clDatabase->writeDBQuery(SO_DB_INFOKIND_SAVETOBANK, &SaveToBank, sizeof(SaveToBank));

	return TRUE;
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	���� �����Ѵ�.
//	���� ����	:	2002-11-12 ���� 9:57:41 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������
BOOL	SOPlayer::WithdrawMoneyAtBank(MONEY siChangeMoney)
{

	// �����ҵ��� ���ݾ׺��� ������ ����
	if (m_MoneySaving < siChangeMoney) return FALSE;
	// �����ҵ��� �����Ѱ躸�� ���Ƶ� ����
	if (siMoney + siChangeMoney > ON_MAX_MONEY) return FALSE;

	// ���ݾ��� ���� �ڱ⵷�� ���Ѵ�.
	m_MoneySaving -= siChangeMoney;
	siMoney += siChangeMoney;

	

	// db�� �˸���.

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




//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	���� ĳ���͸� ���� �ǿ��� ������.
//��������������������������������������������������������������������������������������������������������������������������������������������������������������
VOID	SOPlayer::SendDeadPlayerToVillageHospital()
{
	SI32				siNearestHospitalVillageIndex;	
	OnlineFollower*		lpOnlineFollower;
	

	// ���� �ȿ� ������ �̹� ���� ����� ������ �˰� �ִ�.
	if(siNearestHospitalVillageIndexInDungeon != -1)
		siNearestHospitalVillageIndex	=	siNearestHospitalVillageIndexInDungeon;
	else
		siNearestHospitalVillageIndex	=	pMyWorld->GetNearestHospitalVillageIndex(uiMapID, siX, siY);
	
	// ���� �ε����� �ùٸ���?
	if(siNearestHospitalVillageIndex < 0 || siNearestHospitalVillageIndex >= pMyWorld->siTotalVillageNum)
		siNearestHospitalVillageIndex = 0;

	// ��ȿ�� �������� �˾ƿ´�.
	stAccountInfo.pVillage	=	&pMyWorld->pclVillage[siNearestHospitalVillageIndex];

	// �� ������ �ű��.
	// �ݵ�� ������ ������ ������ �Ŀ� �ؾ� �Ѵ�.
	ChangeMap(0, stAccountInfo.pVillage->siVillageX, stAccountInfo.pVillage->siVillageY);

	// ������ ����.
	EnterVillage();	

	// �� ĳ���Ϳ��� ������ ���ٰ� �˷��ش�.
	SendFieldMsg(ON_RESPONSE_RETURNONLINE, SO_SFM_ONLYME, 
					LPVOID(ON_RET_RETURNONLINE_VILLAGE),
					LPVOID(stAccountInfo.pVillage));


	// ĳ���͸� ȸ�� ��Ų��.
	lpOnlineFollower = IFollower.GetFollower(0);	
	lpOnlineFollower->ParametaBox.IP.SetLife(IFollower.GetFollowerParameterMaxLife(0));
	lpOnlineFollower->ParametaBox.IP.SetMana(IFollower.GetFollowerParameterMaxMana(0), 1);
	
	SendFieldMsg(ON_RESPONSE_HOSPITAL_CURESOLDER, SO_SFM_ONLYME, LPVOID(ON_RET_AUTOCUREOK), NULL, 
				 0, LPVOID(IFollower.GetFollowerParameterMaxLife(0)), LPVOID(IFollower.GetFollowerParameterMaxMana(0)));
}



//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	�迡 ź��.
//��������������������������������������������������������������������������������������������������������������������������������������������������������������
VOID	SOPlayer::BoardShip(SI32 uiboardshipaccount)
{
	uiBoardShipAccount	=	uiboardshipaccount;
}



//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	�踦 Ÿ�� ������ �����Ͽ���.
//	���� ����	:	2002-04-04 ���� 11:02:10 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������
VOID	SOPlayer::ArriveVillageByShip(SOVillage *pVillage)
{
	// ���� Ÿ�� �谡 ���ٰ� �����Ѵ�.
	uiBoardShipAccount		=	0;
	stAccountInfo.pVillage	=	pVillage;

	// �� ������ �ű��.
	ChangeMap(0, stAccountInfo.pVillage->siVillageX, stAccountInfo.pVillage->siVillageY);

	// ������ ����.
	EnterVillage();

	// �迡�� �����ٰ� �˷��ش�.
	SendFieldMsg(ON_GETOUTSHIP, SO_SFM_ONLYME, LPVOID(pVillage));
}



//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	�ʱ�ȭ�� �Ѵ�.
//��������������������������������������������������������������������������������������������������������������������������������������������������������������
VOID	SOPlayer::Init(UI16 uiAccount, UI16 uiMapID, SI16 siX, SI16 siY, UI16 uiKind, CHAR *pszID)
{
	BaseChar::Init(uiAccount, uiMapID, siX, siY, uiKind, pszID);
	
	// 2003.10.7 �߰�, ������ 
	m_PartyState.Init();
}



//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	���� �����Ѵ�.
//��������������������������������������������������������������������������������������������������������������������������������������������������������������
BOOL	SOPlayer::ChangeMap(UI16 uimapid, SI16 six, SI16 siy)
{
	BOOL	bSuccess = FALSE;

	// �̵��Ϸ��� ���� uimapid�� ��ȿ�� ���� �ƴϸ� FALSE�� �����Ѵ�.
	if(pMyWorld->clISOMap.IsValidMapID(uimapid) == FALSE)	goto	END;		

	// ���� ���� �ʰ� �̵��Ϸ��� ���� �ٸ��ٸ� �׿� ���� ó���� ���ش�.
	if(uiMapID != uimapid)
	{
		// ������ ��� ���������� ������ ���ٴ� �����Ͽ��� �ۼ��Ѵ�
		// �׸��� �� �Ӽ��� ������ �ƴ϶�� �� ���̴�.

		// �� �� -> ����
		if((pMyWorld->clISOMap.IsDungeon(uiMapID) == FALSE) && (pMyWorld->clISOMap.IsDungeon(uimapid) == TRUE))
		{	
			SetNearestHospitalVillageIndexInDungeon(pMyWorld->GetNearestHospitalVillageIndex(uiMapID, siX, siY));
		}

		// ���� -> ����  -- ������ siNearestHospitalVillageIndexInDungeon�� ���� �״�� ������ ������ �ȴ�.
		else if((pMyWorld->clISOMap.IsDungeon(uiMapID) == TRUE) && (pMyWorld->clISOMap.IsDungeon(uimapid) == TRUE));
			
		// ??? -> �� ��
		else if(pMyWorld->clISOMap.IsDungeon(uimapid) == FALSE) 
		{
			SetNearestHospitalVillageIndexInDungeon(-1);		
		}	
	}
	
	// Ÿ �������� ��Ƽ ��û���̰ų� 
	// �ٸ� ������ ��Ƽ ��û�� �޾Ҵٸ� �̸� ����Ѵ�.
	CloseRequestingJoinParty();

	bSuccess	=	TRUE;

END:
	if(bSuccess == TRUE)
	{
		// ���� �����Ѵ�.
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

//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	���� ���� �����ش�.
//	���� ����	:	2002-08-17 ���� 7:13:39 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
VOID	SOPlayer::SendMoney(DWORD dwKind)
{
	SendFieldMsg(ON_CHANGED_MONEY, SO_SFM_ONLYME, LPVOID(dwKind));
}



//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	���ʽ� ���� �ش�. ���� ���� �ִ�.
//	���� ����	:	2002-08-05 ���� 2:59:52 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������
VOID	SOPlayer::GiveBonusMoney(MONEY Bonus, DWORD dwKind, BOOL bSendToDB)
{
	ChangeMoney(Bonus, bSendToDB);
	SendMoney(dwKind);
}



//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	���ʽ��� ���������ش�.
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
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

//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	�ſ뵵�� ���������ش�.
//	���� ����	:	2002-08-12 ���� 2:24:04 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
BOOL	SOPlayer::IncreaseCredit(SI32 siCredit)
{
	IncreaseTradeCredit(siCredit);
	return	TRUE;
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	���������� �ſ뵵�� ���Ѵ�.
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
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



//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	�̸��� �̻��� ���ڰ� �ִ��� Ȯ���Ѵ�. TRUE : ��밡��, FALSE : ��� �Ұ�
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
BOOL	SOPlayer::CheckForbiddenName(char *szName)
{
	CHAR	*pszToken;
	CHAR	szTempBuffer[4096];

	ZeroMemory(szTempBuffer, sizeof(szTempBuffer));
	strcpy(szTempBuffer, szName);

	// ���� ��Ƽ ����Ʈ ĳ���Ͱ� �ִ��� üũ�Ѵ�.
	if( !CheckLeadByte(szTempBuffer) )		return FALSE;

	pszToken	=	strtok(szName, ON_FORBIDDEN_NAME);
	if(pszToken == NULL)	return	FALSE;

	if(strlen(szTempBuffer) == strlen(pszToken))
		return	TRUE;
	else
		return	FALSE;
}



//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	�������� �ø���.
//	���� ����	:	2002-05-27 ���� 10:54:31 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
VOID	SOPlayer::ChangeSatiety(SI32 siValue)
{	

	SI32					i;	
	UI08					uiFollowerNum;	
	BOOL					bMinusHealth = FALSE;
	strHungryFollowerInfo	stHFI[ON_MAX_FOLLOWER_NUM];

	// �������� ���� �Ѵ� 
	if((m_satiety + siValue) < 0)	bMinusHealth	=	TRUE;		
	m_satiety		=	min(max(m_satiety + siValue, 0), ON_MAX_HEALTH);	

	if(bMinusHealth == FALSE)
	{	
		// �޽����� �����ش�.
		SendFieldMsg(ON_HUNGRY, SO_SFM_ONLYME, LPVOID(ON_RET_HUR_CHANGE_HEALTH), LPVOID(m_satiety));
	}
	else
	{
		// ���� ������ �ִ� ��� ĳ������ ü���� �A�´�.	
		IFollower.AllCharacterDeleteLife(10);				

		uiFollowerNum	=	0;
		for(i = 0; i < ON_MAX_FOLLOWER_NUM; i++)
		{
			if(IFollower.IsValidFollower(i) == TRUE)
			{			
				stHFI[uiFollowerNum].uiFollowerID		=	i;
				stHFI[uiFollowerNum].siLife			=	IFollower.GetFollowerParameterLife(i);								

				// ���� ���������� ���������ش�.
				uiFollowerNum++;
			}
		}
		
		// �޽����� �����ش�.
		SendFieldMsg(ON_HUNGRY, SO_SFM_ONLYME, LPVOID(ON_RET_HUR_CHANGE_HP), LPVOID(uiFollowerNum), LPVOID(stHFI));
	}		

}



//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	HeartBeat�� �޴´�.
//	���� ����	:	2002-03-21 ���� 10:10:50 - �̼���
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
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

//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	��� ����� �����Ѵ�.
//	���� ����	:	2002-04-04 ���� 3:21:42 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
VOID	SOPlayer::ChangeGuildFlag(SI32 siflag)
{
	siFlag	=	siflag;

	// �޽����� ������.
	SendFieldMsg(ON_RESPONSE_CHANGE_GUILD_FLAG, SO_SFM_ONLYME, LPVOID(ON_RET_OK), LPVOID(siflag));			
}



//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	�������� �������� �׾ ���� �Ա����� ���� ����� �׾ ������ �ε����� �����Ѵ�.
//	���� ����	:	2002-04-15 ���� 4:35:18 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
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
	
	// DB�� �����Ѵ�.	
	pMyWorld->clDatabase->writeDBQuery(SO_DB_INFOKIND_NEARESTHVCODE, &NearestHVCode, sizeof(NearestHVCode));
}



//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	��� Character�� Life�� ����.
//	���� ����	:	2002-05-27 ���� 10:44:24 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
VOID	SOPlayer::AllCharacterDeleteLifePoint(SI32 DeleteLife)
{
	IFollower.AllCharacterDeleteLifePoint(DeleteLife);	
}





//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	�ſ� ����� ���´�.
//	���� ����	:	2002-06-26 ���� 2:54:52 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
SI32	SOPlayer::GetTradeGrade()
{
	const TradeHeader* pTradeHeader	=	pMyWorld->pTradeParser->GetHeader(uiTradeCredit);	

	if(pTradeHeader != NULL)	return pTradeHeader->uiTradeGrade;	
	else						return	0;	
}




//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	���� ������ �����Ѱ�?
//	���� ����	:	2002-07-04 ���� 10:45:07 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
BOOL	SOPlayer::CanTrade()
{
	// ���� �ʵ忡 ���� �ʴٸ� FALSE�� �����Ѵ�.
	if( CHECKSTATUS( siStatus, ON_PLAYER_STATUS_INFIELD ) == FALSE )	return	FALSE;
	//if(siStatus != ON_PLAYER_STATUS_INFIELD)	return	FALSE;

	// ���� ���� ������̰ų� �����߿� �ִٸ� FALSE�� �����Ѵ�.
	if(GetBattle() != NULL)						return	FALSE;

	// ���� �ٸ� �÷��̾�� ���� ���̸� FALSE�� �����Ѵ�.
	if(IsTrading() == TRUE)						return	FALSE;

	// ���� ���� ����̸� FALSE�� �����Ѵ�.
	if(clPrivateMarket.IsOpen() == TRUE)		return	FALSE;

	return	TRUE;
}



//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	���� �ٸ� �÷��̾�� ���������� �˾ƿ´�.
//	���� ����	:	2002-07-06 ���� 2:02:49 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
BOOL	SOPlayer::IsTrading()
{
	return clTrade.IsTrading();
}



//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	�÷��̾��� ���ڷ� �̷���� IP�� �޾ƿ´�.
//	���� ����	:	2002-07-04 ���� 1:20:13 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
DWORD	SOPlayer::GetIPInNumeric()
{
	return	stAccountInfo.dwIP;
}



//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	������ �������� �����ߴ��� �˾ƿ´�.
//	���� ����	:	2002-07-06 ���� 8:56:44 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
BOOL	SOPlayer::IsChoseTradeItem(UI16 uiAccount)
{
	return clTrade.IsChoseTradeItem(uiAccount);
}



//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	������ �������� �����ߴ� ���� ����Ѵ�.
//	���� ����	:	2002-07-06 ���� 10:40:59 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
BOOL	SOPlayer::CancelChooseTradeItem()
{
	return	clTrade.CancelChooseTradeItem();
}



//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	�����ߴ� �����۵��� ������ ������ ���� �����Ѵ�.
//	���� ����	:	2002-07-07 ���� 12:46:22 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
BOOL	SOPlayer::DecideTrade()
{
	return	clTrade.DecideTrade();
}



//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	������ �����Ѵ�.
//	���� ����	:	2002-07-08 ���� 8:43:28 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
VOID	SOPlayer::PrepareBattle()
{	
	CloseTrade();

	// Ÿ �������� ��Ƽ ��û���̰ų� 
	// �ٸ� ������ ��Ƽ ��û�� �޾Ҵٸ� �̸� ����Ѵ�.
	CloseRequestingJoinParty();

	BaseChar::PrepareBattle();
}



//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	������ �����Ѵ�.
//	���� ����	:	2002-05-14 ���� 2:26:56 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
VOID	SOPlayer::EndBattle()
{
	// �켱 �ݵ�� ������ �����Ѵ�.	
	BaseChar::EndBattle();
}



//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	����� ������ �ִ��� �˾ƿ´�.
//	���� ����	:	2002-07-15 ���� 9:39:28 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
BOOL	SOPlayer::HaveGeneral()
{
	SI32	i;

	for(i = 0; i < ON_MAX_FOLLOWER_NUM; i++)
	{
		if(IsGeneral(i) == TRUE)	return	TRUE;
	}

	return	FALSE;
}



//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	������� �˾ƿ´�.
//	���� ����	:	2002-07-15 ���� 9:40:25 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
BOOL	SOPlayer::IsGeneral(UI08 uiFollowerSlot)
{
	UI16	uiCharKind;

	if((uiCharKind = IFollower.GetFollowerCharID(uiFollowerSlot)) == 0)	return	FALSE;
	
	if(pMyWorld->clSOCharKI.GetGeneral(uiCharKind) == 1)	  return	TRUE;
	else if(pMyWorld->clSOCharKI.GetGeneral(uiCharKind) == 3) return	TRUE;
	else													return	FALSE;
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	���� ĳ���Ͱ� ������������ �˾ƿ´�.
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
BOOL	SOPlayer::IsFieldAttackUnit()
{
//	UI16	uiCharkind;

//	if((uiCharkind = IFollower.GetFollowerCharID(0)) == 0)	return	FALSE;

	return CSiegeUnitTable::GetInstance()->IsFieldAttackUnit(uiKind);
	
//	return pMyWorld->clSOCharKI.IsFieldAttackUnit(uiKind);

}



//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	�뺴�� �������� �˾ƿ´�.
//	���� ����	:	2003-02-24 ���� 8:55 - �����
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
BOOL	SOPlayer::IsMonster( UI08 uiFollowerSlot )
{
	UI16	uiCharKind;

	if( (uiCharKind = IFollower.GetFollowerCharID(uiFollowerSlot)) == 0 )	return	FALSE;
	
	// siGeneral(�����) : �Ϲݿ뺴(0), ���(1), ����(2)
	if( pMyWorld->clSOCharKI.GetGeneral(uiCharKind) == 2 )	return	TRUE;
	else													return	FALSE;
}


//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	���� �����ϰ� �ִ� ���� ���� ���´�.
//	���� ����	:	2003-02-24 ���� 8:55 - �����
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
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


//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	�÷��̾ ���� �� �ִ� �ִ� �뺴�� ��.(���ΰ� ����)
//	���� ����	:	2002-07-25 ���� 4:39:34 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
SI16	SOPlayer::GetMaxFollowerNum()
{
	return pMyWorld->clMercenaryParser.GetMaxFollowerNum(GetTradeGrade(), IFollower.GetFollowerParameterLevel(0));
}


//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	�÷��̾ ������� �˾ƿ´�.
//	���� ����	:	2002-08-05 ���� 1:49:58 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
BOOL	SOPlayer::IsAdminPlayer()
{
	if(USGrade != ON_USERGRADE_NORMAL)	return	TRUE;
	else								return	FALSE;
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	���� �뺴�� �߰��� �� �ִ��� �˾ƿ´�.
//	���� ����	:	2002-09-24 ���� 5:56:41 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
BOOL	SOPlayer::CanAddFollower( UI16 uiFollowerNum )
{
	// IFollower ��ü���� �뺴�� �߰��� �� �ִ��� �˾ƿ´�.
	if( IFollower.CanAddFollower( uiFollowerNum ) == FALSE )						return	FALSE;
	
	// �ִ� ���� �� �ִ� �뺴���� �� ���� �뺴�� �ַ��� �ϴ��� �˾ƿ´�.
	// -1 �� �ϴ� ������ GetMaxFollowerNum() �Լ��� 
	// ���ΰ��� ������ �뺴�� ���� �����ϱ� �����̴�.
	// ������ IFollower.GetFollowerNum() �Լ��� ���ΰ��� ������ �뺴�� ���� �˷��ش�.
	if( GetMaxFollowerNum() < ( IFollower.GetFollowerNum() + uiFollowerNum - 1 ) )		return	FALSE;
	
	return	TRUE;
}



bool	SOPlayer::expUp(int followerIndex, int value)
{				
	// ��ȿ�� �뺴�� ��� 
	if( IFollower.GetFollower(followerIndex) )
	{
		_ImportantParameta* pParam  = &IFollower.GetFollower(followerIndex)->ParametaBox.IP;
		
		if(value > 10000)
			writeInfoToFile("ExpLog.txt", "[charName: %s][IP: %s][EXP:%d]", szID, stAccountInfo.IP, value);//����ġ �α׸� �����.

		// ����ġ�� ���� ��Ų�� 
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

//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	���濡�� ���� �ش�.
//	���� ����	:	2002-09-30 ���� 5:04:28 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
BOOL	SOPlayer::GiveMoney( MONEY *pMoney, SOPlayer *pOppPlayer )
{
	MONEY	mnMaxMoneyToGet;

	// �ִ� ���� �� �ִ� ���� ���Ѵ�.
	mnMaxMoneyToGet	=	pOppPlayer->GetMaxMoneyToGet();

	// �ִ� ���� �� �ִ� ������ ���� ���� �� ���� ����.
	if( mnMaxMoneyToGet < *pMoney )							return	FALSE;

	if( DecreaseMoney( *pMoney, FALSE ) == FALSE )			return	FALSE;

	// �� ���� ���濡�� �ش�.
	pOppPlayer->IncreaseMoney( *pMoney, FALSE ); 

	return	TRUE;
}


//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	���� �� �ִ� �ִ뵷�� ���´�.
//	���� ����	:	2002-09-30 ���� 5:19:01 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
MONEY	SOPlayer::GetMaxMoneyToGet()
{	
	return	(ON_MAX_MONEY - siMoney);
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	���� ���� �ȿ� �ִ��� �˾ƿ´�.
//	���� ����	:	2002-10-02 ���� 7:55:24 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
BOOL	SOPlayer::IsInVillage()
{
	if( stAccountInfo.pVillage == NULL )		return	FALSE;
	
	
	return	TRUE;
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	���� ������ �ǹ� �ȿ� ������ �˻��Ѵ�.
//	���� ����	:	2002-10-02 ���� 7:55:37 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
BOOL	SOPlayer::IsEnteredStructure()
{
	return	FALSE;
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	���� ��,���ݾ��� ���´�.
//	���� ����	:	2002-11-22 ���� 4:05:?? - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������

MONEY	SOPlayer::GetMoney()
{	
	return	siMoney;
}
MONEY	SOPlayer::GetSaving()
{	
	return	m_MoneySaving;
}







//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
// kojimodo
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������

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
// Desc : �ڱ��� ���� ��ġ�� DB�� �����Ѵ�
//--------------------------------------------------------------------------------------------
void SOPlayer::savePosToDB()
{
	// ������ �ʾ��̵� Ʋ���ų� ���� ���� ��ġ�� 20�̻� ���̰� �� ��쿡�� ���� �Ѵ� 
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
// Desc : ��� �뺴�� ���� ������ DB�� �����Ѵ� (!! �α� �ƿ��ÿ��� ��� �� !!)
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
// Desc : �� �뺴�� ���� ������ DB�� �����Ѵ� 
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
// Desc : �α׾ƿ��� �ʿ��� ������ DB�� ���� �Ѵ� 
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
			LogoutWarInfo.PrevKind	=	(WORD)uiKind;		// �������϶� �������� ĳ���͸� �����Ѵ�.

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
// Desc : �α׾ƿ��� ���� �ϴ� �÷��׸� ���� �Ѵ� 
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
// Desc : �������� DB�� ���� �Ѵ�
//--------------------------------------------------------------------------------------------
void SOPlayer::saveSatietyToDB()
{
	// ���� �������� �ٸ� ��쿡�� ���� �Ѵ� 
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
// Desc : �� ������ ��ġ�� ��żҸ� ���´� ( ��żҿ� ��ġ���� �ʾ��� ��� NULL )
//--------------------------------------------------------------------------------------------
CAuctionMarket* SOPlayer::getAuctionMarket()
{
	return pMyWorld->m_pAuctionMarket;
}



//--------------------------------------------------------------------------------------------
// Name :
// Desc : �� ������ �����ȿ��� ���� �Ѵ� 
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
// Desc : �� ������ ��żҾȿ��� ���� �Ѵ� 
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
// Desc : �� ������ ���Ӿȿ��� ���� �Ѵ� 
//--------------------------------------------------------------------------------------------
VOID SOPlayer::removeSelfInGame()
{
	// ���ӿ� ���������� �α��� �ߴٸ� ���ӿ� ���õ� �κп����� �������ش�.
	if(bSuccessLogin == TRUE)
	{
		// �ʵ���.
		// �ʿ��� �ش� ĳ������ id�� �����Ѵ�.
		// �켱 �ʿ� �����Ǿ� �ִ°� �ڽ��� �´��� Ȯ���Ѵ�.		
		if(siStatus	& ON_PLAYER_STATUS_INFIELD)
		{
			// ���� �迡 Ÿ�� �ִٸ� �迡 �¼��� ����Ʈ�� �ִ� ���� �����Ѵ�.
			if(uiBoardShipAccount != 0)
			{
				pMyWorld->clSOShip.LogOutPlayer(uiBoardShipAccount, uiAccount, stAccountInfo.siDBAccount);
			}
			else
			{
				// �ʿ��� ID�� �����Ѵ�.
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
		
		else if(siStatus == ON_PLAYER_STATUS_INVILLAGE)	// �����̶��...
		{
			stAccountInfo.pVillage->PlayerLogOut(this);
		}


		// �������� ���´� 
		removeSelfInBattle();

		// ��żҿ��� ���´� 
		removeSelfInAuctionMarket();

		// ����â�� �ݴ´�.
		CloseTrade();

		// ���� ģ���� ����� ����鿡�� ���� �α׾ƿ����� �˸���.
		NotifyFriendsOfLoginState( FALSE );
				
		// Ÿ �������� ��Ƽ ��û���̰ų� 
		// �ٸ� ������ ��Ƽ ��û�� �޾Ҵٸ� �̸� ����Ѵ�.
		CloseRequestingJoinParty();

		// ��Ƽ���̶�� ��Ƽ���� Ż���Ѵ�.
		if( IsPartyMember() == TRUE )
		{
			LeaveParty();
		}

		// ���� ��ܿ��� ��� ��� �����ڿ��� �α׾ƿ� �Ͽ��ٰ� �˷��ش�.
		if( ( uiGuildID != 0 ) && ( siClassInGuild != pMyWorld->clIGuildClass.GetJWJCode() ) )
		{
			pMyWorld->clIGuild.GuildMemberLogout( uiGuildID );
		}

	}
	else
	{
		// �α����� ���� �ʾҴ�.
		printf("[%s] didn't login.\n", szID);
	}		
}



//--------------------------------------------------------------------------------------------
// Name :
// Desc : ��ȿ�� GUID �ΰ�?
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



//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	�������� �̵� ��Ų��.
//	���� ����	:	2002-11-12 ���� 1:16:38 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
BOOL	SOPlayer::MoveItem( UI08 uiFollowerSlot1, UI16 uiItemID, UI16 uiItemQuantity, UI08 uiFollowerSlot2, SI16 siPosInInventory )
{	
	CHAR		szDBArgList[ 1024 ];	

	if( IFollower.MoveItem( uiFollowerSlot1, uiItemID, uiItemQuantity, uiFollowerSlot2, &siPosInInventory ) == FALSE )		return	FALSE;

	// DB�� �����Ѵ�.
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

//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	
//	���� ����	:	2003-01-04 ���� 1:40:19 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
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


//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	������� / ��ȭ��� ����
//	���� ����	:	2003-09-22 ���� 1:50 - �����
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
VOID	SOPlayer::RecvSetAttackMode()
{
	OnReqSetAttackMode			*pRecvMsg;

	pRecvMsg		=	(OnReqSetAttackMode*)cMsg;

	SetAttackMode( pRecvMsg->bAttackMode );
	SendFieldMsg( ON_RESPONSE_SET_ATTACK_MODE, SO_SFM_ONLYNEARCHAR, LPVOID(pRecvMsg->bAttackMode) );
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	����� ��� ������ ��´�.
//	���� ����	:	2004-01-05 PM __Lee_min_su__
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
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
				UnSetGuildInfoChanged();	// ��������� ���� Flag�� 
											// FALSE�� �����Ѵ�.
		}
		// ���������� ������ ��� ������ ���Ѱ� ������..
		else
		{
			SendFieldMsg(ON_RESPONSE_AUDIT_GUILD_INFO, \
				SO_SFM_ONLYME, LPVOID(0), LPVOID(0), LPVOID(-1) );
		}
	}
	else
	{
		// ��ܿ��� �ƴϸ�...
		SendFieldMsg(ON_RESPONSE_AUDIT_GUILD_INFO, \
			SO_SFM_ONLYME, LPVOID(0), LPVOID(0), LPVOID(-2));
	}	
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	��ũ�θ� ����� ������ ������ �����Ѵ�.
//	���� ����	:	2004-04-10 PM __Lee_min_su__
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
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

				//Ŭ���̾�Ʈ�� ���� ������ ����.
				memcpy(&m_stTimerItemInfo[TempTimerItemInfo.siMercenarySlot][TempTimerItemInfo.siLoc],&TempTimerItemInfo,sizeof(OnTimerItemInfo));

				if(TempTimerItemInfo.bDeleteTimerItem)
				{
					siTimerItemUseCount++;
					TempTimerItemInfo.siItemID = 0;
				}
				
				//������ ������ ������ �����Ѵ�.
				pFollower->SetWearItem(i,TempTimerItemInfo.siItemID,TempTimerItemInfo.siRestTime);

				//���� �Ǵ� �������� ��� DB�� �˷��ش�.
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

//������ �߰� ���� �ð��� ���ϰ� ������ ���������� �ƴ��� �Ǵ��Ѵ�.
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
	//�̰͵� ���� �Ű� �ȼ���.
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
	//���� �Ű� �Ƚ���.
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
						//üũ
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
							//üũ ����� ����.
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
	//DB���� �ϰ������� ĳ������ ���� ���� ������ �������� 
	//�������� ����� Ŭ���̾�Ʈ�� �˸���.
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
					
					if(m_bPlayerVillageInLimit) //���� �������̽��� �����ִ� �����ΰ��
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
