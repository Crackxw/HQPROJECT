#ifndef SOPLAYER_H
#define SOPLAYER_H

#include <time.h>
#include <winsock2.h>

#include "SOUtility.h"
#include "SOCommonHeader.h"
#include "SOBaseChar.h"
#include "OnlineCommonStruct.h"
#include "IOnlineFollower.h"
#include "OnlineGameMsg.h"
#include "SODefine.h"
#include "PrivateMarket.h"
#include "Trade.h"
#include "SpeedChecker.h"
#include "SOPlayer_ItemBankList.h"
#include "Common.h"
#include "../SCCommon/Defines.h"
#include "SODBStructure.h"

#include "../AuthServer/_LoginDefine.h"

struct LoginInfo;

class   CSkillMgr;

class	XPlayer;
		
class	SOBattle;
class	SOServer;
class	SOGate;
class	SOVillage;
struct	stFarmData;
class   CAuctionMarket;
#include "..\\XCommon\\JXPacket.h"
#include "./party/IPartyMgr.h"

#include "CSiegeUniteTable.h"
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
// 플레이어의 정보
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
typedef struct
{
	char			id			[LOGIN::ID_MAX_LENGTH];
	char			password	[LOGIN::PW_MAX_LENGTH];

	char			IP[16];							//	ip주소(char 형)
	DWORD			dwIP;							//	DWORD형.
	SI32			siDBAccount;					//	플레이어를 구분하는 데이터베이스상의 유일한 번호
	UI08			uiSlot;							//	케릭터의 슬롯번호.
	UI16			uiStatusInVillage;				// 마을 안에서 들어가 있는 건물.
	//UI16			uiChatRoomIDInInn;				// 마을, 주점의 대화방에 참여하고 있는 방의 인덱스.

	SOBattle		*pBattle;						// 현재 참가하고있는 전투방.
	SOVillage		*pVillage;
	stFarmData		*pStructure;					// 현재 일하고 있는 건물.

} ACCOUNT_INFO;



//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
// 플레이어의 배표 정보.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
typedef struct
{
	SI32		siTicketKind;					// Ticket Kind
} TICKET_INFO;



//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
// 플레이어의 자산 정보.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
typedef struct
{
	MONEY		siTotalProperty;				//	총재산
	MONEY		siMoneyProperty;				//	현금
	MONEY		siSaveProperty;					//	전장 잔고 (+)값
	MONEY		siLoanProperty;					//	전장 대출 (-)값
	MONEY		siItemProperty;					//	아이템을 환산한 돈
	UI32		uiRanking;						//	자신의 랭킹
} PROPERTY_INFO;

extern char cMsg2[ 80000 ];

//=========================================================================================
enum MAIN_STATE
{
	NONE_STATE			=   0 ,

	USER_IN_AUTH			  ,
	USER_IN_LOGIN			  ,
	USER_IN_GAME			  ,
	
	USER_MUST_LOGOUT_IN_AUTH  ,
	USER_MUST_LOGOUT_IN_LOGIN ,
	USER_MUST_LOGOUT_IN_GAME  ,

	USER_WAIT_FOR_LOGOUT

};

class CMainState
{
private:
	long			m_state;	

public:
	void			set(long state)    { InterlockedExchange(&m_state, state); }
	int				get()			   { return m_state; }
	bool			match(long state)  { return get() == state ? true : false; }
	void			logout()		   
	{ 
		switch( get() )
		{
			case USER_IN_AUTH:			
				set( USER_MUST_LOGOUT_IN_AUTH );				
				break;

			case USER_IN_LOGIN:
				set( USER_MUST_LOGOUT_IN_LOGIN );
				break;

			case USER_IN_GAME:
				set( USER_MUST_LOGOUT_IN_GAME );
				break;
		}
	}
};
//=========================================================================================

class CCSAuth;

class CAuctionMarketProc;
class CBattle;
class SOPlayer : public BaseChar
{
	// 순수 게임에서의 데이타.
public:

	///////////////////////////////////////////
	// 캐릭터 슬롯 정보
	///////////////////////////////////////////

	LOGIN::sCharInfo	m_charsInfo[ LOGIN::CHAR_MAX_NUM ];

	void				clearCharsInfo();
	void				setSlot( int slot );
	void				delSlot( int slot );
	int					getEmptySlot();
	void				sendCharsInfo();

	////////////////////////////////////////////


	CMainState			m_mainState;

	DWORD				m_lastTick;

	// --  로그인 관련 
	void		ProcessLogin();
	LoginInfo  *logininfo;
	BOOL		bSuccessLogin;								// 로그인을 성공적으로 했는가?
	BOOL		bProcessLogin;
		
	UserGrade	USGrade;									// 신용 등급.
	MONEY		siMoney;									// 돈
	
	MONEY		investProfit;								// 투자 수익 
	MONEY		additionProfit;								// 부가 수익 (건물 팔때)
		
	UI32		uiTradeCredit;								// 무역 신용도
	SI32		siDayCredit;								// 하루에 오른 신용도
	
	UI32		uiGuildID;									// 상단의 ID
	SI32		siClassInGuild;								// 길드내에서의 직급
	SI32		siFlag;										// 깃발	

	SI32		siNearestHospitalVillageIndexInDungeon;		// 던전에서 가장 가까운 의원을 가지고 있는 마을의 인덱스
	
	DWORD		dwPrevSavePosTime;							// 이전에 내 위치를 DB에 저장한 시간
	DWORD		dwHeartbeatTime[ON_HEARTBEAT_SAVE_NUM];		// 플레이어가 살아있는지 검사할 시간


	cltPrivateMarket	clPrivateMarket;					// 개인 시장
	cltTrade			clTrade;							// 교역

	CSpeedHackChecker	m_SpeedChecker;
	BOOL				m_bIsSpeedHackUser;		
	
	SI08		m_siMaxGeneral;								// 고용 가능한 장수의 수
	BOOL		m_bCompareWearItem;

	SI16		m_siTimerItemUseCount;

// gguo
// -----------------------------------------------------------------------------------------------------

private:
	SI32                m_siChallengeMode;                          // 도전 108계단 모드.
	ItemBankList		m_ItemBankList;

	// GameGuard관련
	DWORD				m_lastGameGuardCheckTime;
	CCSAuth				*m_pCSAuth;
	BOOL				m_bCheckPass;

	DWORD				m_dwCondLaborTime;							// 특정 조건을 만족하는 이전 노동 시간

public:
	MONEY				m_MoneySaving;                              // 저축해놓은 돈.

// kojimodo 
//------------------------------------------------------------------------------------------------------
private:
	int					m_satiety;									// 포만감
	int					m_orgSatiety;								// 원본 포만감 
	
	WORD				m_latestSavedMapID;							// 최근에 저장한 MapID
	POINT				m_latestSavedPos;							// 최근에 저장한 위치 
	DWORD				m_latestSavedPosTime;						// 최근에 위치를 저장한 시간 

	CSkillMgr*			m_pSkillMgr;								// 스킬 관리자 


	CAuctionMarketProc* m_pAuctionMarketProc;						// 경매소 패킷 처리 클래스 
	DWORD				m_tempBiddingPrice;							// 경매에서 임시로 입찰가격을 보관하는 변수 
	BOOL				bGuildInfoChanged;							// 길드 정보 변경했는지?
	

public:
	void				SetGuildInfoChanged()	{ bGuildInfoChanged = TRUE; }
	void				UnSetGuildInfoChanged()	{ bGuildInfoChanged = FALSE; }

	void				setTempBiddingPrice( DWORD price );			
	DWORD				getTempBiddingPrice();

	CAuctionMarket*		getAuctionMarket();
			
	void				saveSatietyToDB();							
	void				saveLogoutInfoToDB();
	void				saveLogoutStartToDB();
	void				saveAllFollowerDataToDB();								
	void				saveLogountWarInfoToDB();
	
	void				removeSelfInGame();
	

private:
	void				removeSelfInAuctionMarket();	
	void				removeSelfInBattle();

	void				savePosToDB();
	void				saveFollowerDataToDB( DWORD dbCmd, BYTE followerSlot );		
	
	bool				isValidGUID(char* guid);
// semarine
//------------------------------------------------------------------------------------------------------

public:
	SI32				m_numWin;
	SI32				m_numLose;
	SI32				m_siPoint;
	SI32				m_siRank;



//------------------------------------------------------------------------------------------------------


protected:
	BOOL		Trade( SOPlayer *pTargetPlayer );			// 교역중인 품목에 대해서 거래를 하라고 알려준다.

public:	
	SOPlayer();												// 생성자	
	~SOPlayer();											// 소멸자

public:
	//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
	// Time 상태 변수 
	//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
	struct tm			*pNewTime;
	time_t				stClock;

public:
	SOServer	*clServer;		

public:
	// Packing에 관련된 버퍼와 길이
	char		pmsg[30000];
	char		*cMsg;							
	SI16		siMsgLength;							// 메세지의 크기

	JXPacket	local_packet;
public:
	SI32		Write(const VOID *, SI32 Length);
	SI32		ReadFromBattle(SI32 siLength);
	SI32		WriteToBattle(const VOID *, SI32 siLength);
	

public:
	EquipItemInfo	a;
	// 플레이어의 상태를 나타낸다. (미접속인가?, 접속중인가?, 대기실인가?, 대화방인가?, 게임중인가?)
	UI08			uiState;

	// 건물안에서 자신의 위치
	UI32			uiStructerInPos;

	// 플레이어의 표 정보
	TICKET_INFO		stTicketInfo;

	// Quest 정보
	SPlayerQuest	stQuestInfo;

	// 플레이어의 접속 정보
	ACCOUNT_INFO	stAccountInfo;	
	// 플레이어의 자산 정보 
	PROPERTY_INFO	stPropertyData;

	SI08			siVMercenarySlot;				// 따라다니는 부하 Slot

	// 신용 대출 한도
	UI32			uiMaxLoan;

	SI32			uiBoardShipAccount;				// 현재 타고 있는 배의 Account

	XPlayer			*m_pXPlayer;					// 제완 추가..

	UI16			m_uiRecvItemNum;

	BOOL			m_bDayChange;					

	//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
	//시간제 아이템 관련 변수.
	//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
	DWORD			m_dwUserhour;
	DWORD			m_dwUserMin;
	DWORD			m_dwUserSec;

	OnTimerItemInfo		m_stTimerItemInfo[12][9];
	OnTimerItemInfo		m_pTimerItemInfo[12];		// 일단 배열로.-_-쩝.


	//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
	// 공성 캐릭터에 관련 변수.
	//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
	SI16			m_siFieldHp;					// 공성전시 획득하는 Hp.
	SI16            m_siFieldMaxHP;                 // 공성전시 획득하는 최대 hp 

	SI32            m_siFieldMP;                    // 공성전시 획득하는 MP
	SI32            m_siFieldMaxMP;                 // 공성전시 획득하는 최대MP

//	SI32			m_siSupply;						// 공성전시 획득하는 보급.
//	SI32			m_siSupplyCharge;				// 공성전 충전 게이지. (보급수레만 얻을수 있다.)
	BOOL			m_bFieldAlive;					// 공성전시 살았나 죽었나.죽으면 필드에 그대로.5분간 공성유닛으로 변신불가.
	UI16			m_uiPrevKind;					// 공성유닛으로 변하기전의 캐릭터 Kind
	BOOL			m_bCaptainUnit;					// 대장유닛인가.

	DWORD			m_dwDeadTime;					// 공성시 죽은 시간 기록. 로그 아웃 했을 경우 
	DWORD			m_dwDeadSec;					// 죽었을때 패널티로 받은 시간.

	BOOL			m_bInvestmentPower;
	BOOL			m_bFieldUnitUpdate;
	BOOL			m_bPlayerVillageInLimit;
	SI16            m_siVilligeCode;                // 수성시 수성을 하고 있는 마을코드
	
	BOOL            m_bIsFirstDefence;              // 성 방어력을 처음 올렸나.
	DWORD           m_dwDefenceTime;                // 성방어력을 올린시간...
	DWORD           m_dwChargeTime;                 // 공성시 보급수레의 마나가 회복된 시간.
	DWORD           m_dwMessageTime;                //
	BOOL            m_bIsWarUnit;                   //
	DWORD           m_dwAttackDelay;                // 공격 딜레이 시간
	SI16            m_siKillNum;                    // 죽인 수 
	//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
	// 네트워크 관련함수
	//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
public:
	// 캐릭터 초기화
	VOID	Init(UI16 uiAccount, UI16 uiMapID, SI16 siX, SI16 siY, UI16 uiKind, CHAR *pszID);					
 
	// 플레이어의 소켓 초기화
	VOID	Init();

	// 문자열 ip를 DWORD 형식으로 바꿔준다.
	DWORD	ChangeIPStr(char* lpIP);

	// 플레이어의 각종 변수 초기화
	VOID	InitEnv();

	// 플레이어와 관련한 모든 연산 수행
	VOID	Poll(VOID);
	
	// 모든 메세지를 처리한다.
	VOID	ProcessMessage(VOID);
	BOOL	ProcessMessageInField(VOID);
	BOOL	ProcessMessageInVillage(VOID);
	BOOL	ProcessMessageInBattle(VOID);
	BOOL	ProcessMessageCommonly(VOID);

	// 로그아웃 한다.
	VOID	LogOut(VOID);
		
	// 플레이어를 생성한다.
	VOID	Create( SOServer *clTempServer );					// 플레이어를 생성한다.

public:
	// 로그인 처리
	VOID	RecvLogIn(char *pMsg);
	VOID	SendLogIn(BOOL bSuccess);


public:
	//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
	// 게임 데이타 처리.
	//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
	// 플레이어에게 메시지를 보낸다.
	BOOL	SendFieldMsg(SI32 siMsg, SI32 siSendInfo, LPVOID pParam1 = NULL, LPVOID pParam2 = NULL, LPVOID pParam3 = NULL, LPVOID pParam4 = NULL, LPVOID pParam5 = NULL, LPVOID pParam6 = NULL);	

	// 돈을 보낸다.
	VOID		SendMoney(DWORD dwKind);
	VOID		GiveBonusMoney(MONEY Bonus, DWORD dwKind, BOOL bSendToDB = TRUE);		// 돈을 준다.

	// 지역탐사 요청을 받았다 
	void	recvProbeArea();

	// 이모티콘 받았다.
	VOID	RecvImoticon();
	 
	// 플레이어의 정보를 보낸다.
	// 플레이어의 위치를 설정한다.
	VOID	RecvPosition();
	
	// 특정 플레이어의 위치를 요청한다.
	// 특정 플레이어의 위치를 보낸다.
	// 특정 플레이어의 정보를 요청한다
	VOID	RecvPlayerInfo();

	VOID	RecvCharDetailInfo();								// 캐릭터의 상세 정보를 요청한다는 메시지를 받았다.
	
	// 아이템을 소비했다.
	VOID	RecvSpendItem();
	BOOL	RecvWarp();											// 워프 한다는 메시지를 받았다.			
	BOOL	Warp(UI16 uiMapID, SI32 siX, SI32 siY);				// 워프를 하고 주위의 데이타를 받는다.

//	VOID	RecvDeleteTimerItem();

	// 마을에 들어간다.
	VOID	RecvEnterVillage();
	BOOL	EnterVillage( BOOL bIsVillageField = FALSE );		// 마을에 들어간다
	VOID	LeaveVillage( BOOL bIsVillageField = FALSE );		// 마을에서 떠난다.

	VOID	RecvEnterStructure();								// 마을에서 특정 건물에 들어간다는 메시지를 받았다.
	VOID	RecvLeaveStructure();								// 마을에서 특정 건물에서 나간다는 메시지를 받았다.

	VOID	RecvEnterFactory();									// 마을 산업 단지의 공장에 들어간다.
	VOID	RecvLeaveFactory();									// 마을 산업 단지의 공장에서 나간다.
	

	// 필드상에 플레이어가 위치할 수 있는 좌표를 기준점을 중심으로 찾는다.
	// 전투를 요청한다.
	VOID	RecvBattle(VOID);
	VOID	SendDeadPlayerToVillageHospital();					// 죽은 캐릭터를 마을 의원에 보낸다.
	VOID	NotifyFriendsOfLoginState( BOOL bLogin );			// 나를 친구로 등록한 사람들에게 로그인했음을 알린다.

	VOID	RecvSetAttackMode();									// 전투모드 / 평화모드 설정

	//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
	// Common
	//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
	VOID	RecvHeartBeat();
	// 플레이어소유의 아이템리스트를 요청한다.
	// 플레이어소유의 아이템의 체크섬을 요청한다.
	
	// 아이템을 이동 시킨다.
	VOID	RecvShareItem();
	// 아이템을 버린다.
	VOID	RecvThrowItem();
	// 아이템을 줍는다.
	VOID	RecvPickUpItem();
	
	
	VOID	RecvWearItem();
	VOID	RecvStripItem();
	
	BOOL	GetWearItemInfo(UI08 uiFollowerID);
	BOOL	StripItem(	UI08 uiFollowerID, UI08 uiWearLoc, UI16 uiItemID, SI16 uiPosInventory);
	BOOL	WearItem(	UI08 uiFollowerID, UI08 uiWearLoc, UI16 uiItemID, SI16 uiPosInventory);
	BOOL	SpendItem( UI08 uiFollowerID, UI16 uiItemID, UI16 uiPosInventory, UI16 uiQuantity = 1);
	BOOL	DisAppearItem( UI08 uiFollowerID, UI08 uiWearLoc, UI16 uiItemID, SI16 uiPosInventory);
	// 플레이어소유의 아이템리스트의 패킷을 만든다. (패킷의 길이를 리턴한다)
	// 플레이어소유의 아이템들의 해당마을에서의 물가 패킷을 만든다. (패킷의 길이를 리턴한다)
	// 아이템의 체크섬을 체크한다.
	// 체크섬값을 만든다.	

	//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
	// GameGuard관련
	//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
	VOID	CheckGameGuard(VOID);
	VOID	RecvRequestCheckGameGuard(VOID);
	VOID	RecvUserInfoGameGuard(VOID);

	//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
	// 저자 거리에서의 처리
	//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
	VOID	RecvChargeBooth();
	VOID	RecvLeaveBooth();
	VOID	RecvDisplayBooth();
	VOID	RecvSetBoothItem();
	VOID	RecvCancelBoothItem();
	VOID	RecvBoothInfo();
	VOID	RecvBuyBoothItem();

	//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
	// 마을안에서의 처리
	//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

	// 주막에서 아이템 리스트, take out
	VOID	RecvItemList();
	VOID	RecvTakeOutItem();
	VOID	RecvSaveItemIntoItemBank();


	// 마을에서 떠난다.
	VOID RecvLeaveVillage();
	// 플레이어의 소지금을 변경한다.
	

	//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
	// 마을의 시장에서의 처리.
	//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
	VOID RecvEnterMarket();					// 시장에 들어간다.	
	VOID RecvLeaveMarket();					// 시장에서 나간다.
	// 상점에서 아이템의 리스트를 요구한다.
	VOID RecvListItem();
	// 아이템을 산다.
	VOID RecvBuyItem();
	// 아이템을 판다.
	VOID RecvSellItem();

	VOID SendReloadWearItem();
	// 플레이어가 소지하고있는 아이템들의 현재 마을에서의 물가를 구한다.

	// 해당 아이템을 살 수 있는지 검사한다.

	// 플레이어가 아이템을 소지하고있는지 찾는다.
	// 아이템을 찾았으면 배열에서의 배열번호를 못찾으면 -1을 리턴한다.
	// 플레이어의 아이템을 등록한다.
	// 플레이어의 아이템을 삭제한다.
	// 플레이어의 아이템을 증가시킨다. (아이템이 기존에 있다면 증가를, 없다면 생성을)
	// 해당아이템의 플레이어가 소유하고있는 갯수를 얻는다.
	// 플레이어의 아이템을 감소시킨다. (아이템이 기존에 있다면 감소를, 0이되면 삭제를)
	// 마을안에서의 플레이어의 상태
	
	/*
	VOID	RecvEnterInn();							// 주점에 들어간다는 메시지를 받았다.
	VOID	RecvLeaveInn();							// 주점에서 나간다는 메시지를 받았다.
	VOID	RecvEnterWaitRoom();					// 주점의 대기실에 들어간다는 메시지를 받았다.
	VOID	RecvCreateChatRoomInInn();				// 주점에서 대화방을 만든다.
	VOID	RecvJoinInnChatRoom();					// 주점 대화방에 들어간다는 메시지를 받았다.
	VOID	RecvLeaveInnChatRoom();					// 주점 대화방에서 나간다는 메시지를 받았다.
	VOID	RecvEnterNoticeBoard();					// 주점 게시판에 들어간다는 메시지를 받았다.
	VOID	RecvRegistNotice();						// 게시물을 등록한다는 메시지를 받았다.
	VOID	RecvDeleteNotice();						// 게시물을 삭제한다.
	VOID	RecvNoticesList();						// 게시물의 리스트를 얻어온다.
	VOID	RecvViewNotice();						// 게시물을 본다.	
	VOID	RecvModifyNotice();						// 게시물을 수정한다.
	VOID	RecvInnWorkerEnter();					// 짐꾼 고용란에 들어간다.
	VOID	RecvInnWorkerBuy();						// 짐꾼을 산다.
	*/

	VOID	RecvBarrackGetSolderList();				// 용병 List를 요청한다.
	VOID	RecvBarrackBuySolder();					// 용병을 고용한다.
	VOID	RecvBarrackGetDismissalSolderList();	// 해고할 용병 List를 요청한다.
	VOID	RecvBarrackDismissalSolder();			// 용병을 해고한다.

	VOID	RecvGBarrackGetSolderList();			// 장수용병 List를 요청한다.
	VOID	RecvGBarrackBuySolder();				// 장수용병을 고용한다.
	VOID	RecvGBarrackGetChangeJobSolderList();	// 전직할 장수용병 List를 요청한다.
	VOID	RecvGBarrackChangeJobSolder();			// 장수용병을 전직한다.
	VOID	RecvGBarrackGetDismissalSolderList();	// 해고할 장수용병 List를 요청한다.
	VOID	RecvGBarrackDismissalSolder();			// 장수용병을 해고한다.

	VOID    RecvGBarrackGetGeneralExList();			// 2차 장수 List 요청
	VOID	RecvGBarrackBuyGeneralEx();				// 2차 장수 고용한다.
	VOID	RecvGBarrackGetChangeGeneralExList();	// 2차 장수 전직 List 요청
	VOID	RecvGBarrackChangeGeneralEx();			// 2차 장수 전직한다.
	VOID	RecvGBarrackGetDismissalGeneralExList();// 2차 장수 해고 List 요청.
	VOID	RecvGBarrackDismissalGeneralEx();		// 2차 장수 해고한다.

	VOID	RecvMBarrackGetMonsterList();			// 몬스터 List를 요청한다.
	VOID	RecvMBarrackBuyMonster();				// 몬스터를 고용한다.
	VOID	RecvMBarrackGetDismissMonsterList();	// 해고할 몬스터 List를 요청한다.
	VOID	RecvMBarrackDismissMonster();			// 몬스터를 해고한다.

	VOID	RecvHospitalGetCureList();				// 용병 치료List를 요청한다.
	VOID	RecvHospitalCureSolder();				// 용병을 치료한다.
	VOID	RecvHospitalCureAllSolder();			// 모든 용병을 치료한다.

	VOID	RecvWharfEnter();						// 부두에 들어감
	VOID	RecvWharfBuyTicket();					// 표를 삼
	VOID	RecvWharfExit();						// 부두를 나감
	VOID	RecvWharfWaitRoomEnter();				// 부두 대기실에 들어감
	VOID	RecvWharfWaitRoomExit();				// 부두 대기실을 나감
	VOID	WharfGetDB();							// DB에서 Ticket 정보를 얻어옴

	// 주식 처리
	VOID	RecvGovernmentEnter();
	VOID	RecvGovernmentLeave();
	VOID	RecvGovernmentInvestmentEnter();
	VOID	RecvGovernmentInvestmentLeave();
	VOID	RecvGovernmentInvestmentGetList();
	VOID	RecvGovernmentInvestmentSetInvestment();
	VOID	RecvGovernmentVillageInfo();				// 관청, 마을정보 얻기
	VOID	RecvProfitStatistics();
	VOID	RecvAuditProfitStatistics();
	VOID	RecvReqMaxVillageDefenceInvestment();
	VOID	RecvReqSetVillageDefenceInvestment();
	VOID	RecvGovernmentInvestmentGetOut();			// 관청, 투자한돈 가지고 나가기

	VOID	RecvGuildChangeFlag();						// 상단의 깃발을 변경한다.

	// 전투를 끝내고 다시 온라인으로 돌아간다.
	VOID	RecvReturnOnline();
	// 10초마다 Heartbeat를 보낸다.
	VOID	SendHeartbeat();
	
	BOOL			CheckWeight(UI08 uiFollowerID, UI16 uiID, UI16 uiQuantity);
	BOOL			IsOverWeight();																	// 무거워서 걷기 힘든 상태인가?

	BOOL			EnoughMoney(MONEY siEnoughMoney);												// 돈을 갖고 있는지 확인한다.
	VOID			ChangeMoney(MONEY siChangeMoney, BOOL bSendToDB = TRUE);						// 돈의 액수를 바꾼다.
	MONEY			IncreaseMoney(MONEY siIncreaseMoney, BOOL bSendToDB = TRUE);					// 돈을 증가시킨다.
	BOOL			DecreaseMoney(MONEY uiDecreaseMoney, BOOL bSendToDB = TRUE);					// 돈을 뺀다.
	MONEY			DecreaseMoneyReturnMoney(MONEY uiDecreaseMoney);					// 돈을 뺀다.

	BOOL            SaveMoneyAtBank(MONEY siMoney);                                                 // 은행에서 저금한다.
	BOOL            WithdrawMoneyAtBank(MONEY siMoney);                                             // 은행에서 인출한다.
	MONEY           ModifySaving(MONEY siMoney);                                                    //  
	MONEY           GetSaving();

	MONEY			GetMoney();																		// 돈을 얻어온다.
	MONEY			GetMaxMoneyToGet();																// 가질 수 있는 최대돈을 얻어온다.

	BOOL			IncreaseBonus(UI08 uiFollowerID, SI32 siBonus);									// 보너스를 증가시켜준다.
	BOOL			IncreaseCredit(SI32	siCredit);													// 신용도를 증가시켜준다.

	UI32			IncreaseTradeCredit( UI32 uiIncreaseCredit, BOOL bSendToDB = TRUE );			// 무역 신용도를 올린다.
	VOID			DecreaseTradeCredit( UI32 uiDecreaseCredit, BOOL bSendToDB = TRUE );			// 무역 신용도를 감소시킨다.
	UI32			GetTradeCredit();																// 무역 신용도를 얻어온다.
	SI32			GetGiveCredit(MONEY TradeMoney);												// 변동되는 신용도를 구한다.
	BOOL			CheckForbiddenName(char *szName);
	SI32			GetTradeGrade();																// 신용 등급을 얻어온다.

	VOID			RecvReadDescript();											// 마을 안내문을 받는다.
	VOID			RecvWriteDescript();										// 마을 안내문을 쓴다.

	VOID			RecvVillageNation();

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명	:	Quest에 관련된 처리
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
	VOID			RecvReqQuestSelectNPC();									// NPC를 선택했을때
	VOID			RecvReqQuestAcceptQuest();									// Quest를 수락했을때
	VOID			RecvReqQuestCancelQuest();									// 진행중인 Quest를 취소했을때
	VOID			RecvReqQuestGetPlayQuest();									// 진행중인 Quest List 요청

	SI16			Quest_FindPlayQuestNPC(SI16 NPCCode);						// 이 NPC에 관련된어 Play하고 있는 Quest를 찾는다.
	BOOL			Quest_CheckCompleteQuest(SI16 QuestSlot);					// 이 Quest가 완료될수 있는지 검사
	VOID			Quest_EndQuest(SI16 QuestSlot);								// Quest를 종료한다.(DB에 저장하고, 보상을 받으며, Client로 메세지 전송까지 한다.)
	SI16			Quest_FindNextQuest(SI16 NPCCode);							// 이 NPC에게서 진행할수 있는 다음 Quest를 찾는다.
	BOOL			Quest_CheckPlayQuestAbility(SI16 QuestCode);				// Quest를 Start할수 있는지 검사
	BOOL			Quest_CheckSkipQuestAbility(SI16 QuestCode);				// Quest를 Skip할수 있는지 검사
	SI16			Quest_FindLastPlayQuest(SI16 NPCCode);						// 이 NPC에게서 마지막으로 Play한 Quest검사
	BOOL			Quest_FindCompletedQuest(SI16 QuestCode);					// 예전에 한적이 있는 Quest인지 검사
	SI16			Quest_FindEmptyQuest(void);									// 빈 Quest Slot을 찾음
	SI16			Quest_FindQuest(SI16 QuestCode);							// 진행중인 Quest를 찾음
	VOID			Quest_AllClear(void);										// 모든 Quest 초기화, 깔끔하게
	VOID			Quest_CheckEventItem(SI16 siQuestSlot);			
	VOID			Quest_SetEventItem(SI16 siEventItemID,SI16 QuestSlot);

	VOID			Quest_AddQuestObject_KillMonster(SI32 MonsterCode, SI32 Count);		// 죽인 몬스터 Data
	VOID			Quest_AddQuestObject_Work(SI32 siWork);								// 자신이 한일
	VOID			Quest_AddQuestObject_PortalUse(SI32 siNum);
	VOID			Quest_AddQuestObject_Npc(SI32 siNpc);
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명	:	창고에 관련된 처리
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
	VOID			RecvStorageInfo();					//	창고에 있는 아이템을 보내준다.
	VOID			RecvStorageInputItem();				//	아이템 넣기
	VOID			RecvStorageOutputItem();			//	아이템 꺼내기
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명	:	농장에 관련된 처리
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
	stFarmData*		GetOwnFactory();					// 플레이어가 소유하고 있는 시설물을 얻어온다.
	VOID			RecvFarmInfo();						//	농장 정보
	VOID			RecvFarmBuy();						//	농장 구입
	VOID			RecvFarmSell();						//	농장 팔기
	VOID			RecvFarmSlotInfo();					//	슬롯 정보
	VOID			RecvFarmUpgradeSlot();				//	슬롯 업그레이드
	VOID			RecvFarmBuildStruct();				//	건물 건설
	VOID			RecvFarmExpandSlot();				//	슬롯 확장
	VOID			RecvFarmDestroyStruct();			//	건물 철거
	VOID			RecvFarmSetEquipment();				//	시설물 설치
	VOID			RecvFarmItemProduct();				//	아이템 생산
	VOID			RecvFarmWorkStart();
	VOID			RecvFarmWork();						//	일하기
	VOID			RecvFarmWorkHack();

	BOOL			CheckLaborHack(UI08 uiType, SI08 siStep, UI32 uiQuantity);		//단계에 맞는 일인지 확인
	BOOL			CheckLaborHack1(SI08 siStep, UI32 uiQuantity);
	BOOL			CheckLaborHack2(SI08 siStep, UI32 uiQuantity);
	BOOL			CheckLaborHack3(SI08 siStep, UI32 uiQuantity);
	UI32			SumOfWorkQuantity(SI08& siStep, UI32& uiQuantity);		//리커시브로 일량 체크
	VOID			RecvFarmWorkInfo();					//	공장의 정보를 요청했다.
	VOID			RecvPlantChangPay();
	VOID			RecvPlantCancelPlan();
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명	:	장부에 관련된 처리
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
	BOOL			GetTotalProperty(PROPERTY_INFO &PropertyInfo);			//	자신의 총 자산을 얻는다.
	MONEY			CalculateItemValue(MONEY &siItemMoney);					//	자신의 아이템의 가치를 구한다.

	BOOL			GetAuditAccountList();									// 장부의 계좌목록 얻는다.
	BOOL			GetAuditOwnList();										// 장부의 부동산소유 얻는다.
	BOOL			GetAuditVillageInfo();									// 장부의 마을정보 얻는다.
	BOOL			GetAuditStatus();										// 장부의 무장정보 얻는다.
	VOID			RecvAuditSetName();										//	장부에서 용병 이름을 입력한다.

	VOID			RecvAuditTotalInfo();									// 장부의 총재산 요구를 받았다.
	VOID			RecvAuditAccountList();									// 장부의 계좌목록 요구를 받았다.
	VOID			RecvAuditOwnList();										// 장부의 부동산소유 요구를 받았다.
	VOID			RecvAuditVillageList();									// 장부 마을 list 요구를 받았다.
	VOID			RecvAuditFriendList();									// 장부 친구 list 요구를 받았다.
	VOID			RecvAuditFriendSetDenial();								// 장부 친구를 수신거부/해제 한다는 요구를 받았다.
	VOID			RecvAuditFriendAddOne();								// 장부 친구를 추가한다.
	VOID			RecvAuditFriendRemoveSome();							// 장부에서 친구를 삭제한다는 요구를 받았다.
	VOID			RecvAuditFriendRemoveAll();								// 장부에서 친구 전체를 삭제한다는 요구를 받았다.
	VOID			RecvAuditVillageInfo();									// 장부의 한 마을정보 요구를 받았다.
	VOID			RecvAuditStatus();										// 장부의 무장정보 요구를 받았다.
	VOID			RecvAuditFollowerStatus();								// 장부의 한 무장에 대한 정보 요구를 받았다.

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명	:	통장에 관련된 처리
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
	
//--------------------------------------------------------------------------------------------
// 마을의 전장에서의 처리.
//--------------------------------------------------------------------------------------------
	
	VOID			RecvBankbookList();																// 통장 리스트를 달라는 요구를 받았다.
	VOID			RecvBankbookListCount();														// 통장 리스트의 수를 달라는 요구를 받았다.
	VOID			RecvBankInfo();																	// 은행의 정보를 얻어온다.
	VOID			RecvBankDeposit();																// 은행에 예금을 한다.
	VOID			RecvBankDrawing();																// 은행에서 출금을 한다.	
	VOID			RecvOpenSavingAccount();														// 은행 계좌를 만든다는 메시지를 받았다.
	VOID			RecvBankCollateral();
	VOID			RecvBankCollateralList();
	VOID			RecvBankTakeCollateral();
	VOID			RecvMaxCreditLoan();

	VOID            RecvBankSave();
	VOID            RecvBankWithdraw();

	SI32			GetBankBootNum();																				// 통장의 총 개수를 얻어온다.
	DWORD			GetInfo(OnCharPlayerInfo *pPlayerInfo);															// 캐릭터의 정보를 얻어온다.	
	DWORD			GetDetailInfo(OnCharPlayerDetailInfo *pDetailInfo, UI32 uiRefGuildID, SI32 siRefGuildInClass, DWORD siRefDBAccount );	// 캐릭터의 상세 정보를 얻어온다.

	VOID			RecvReqGuildCreateCost();									// 상단 생성에 필요한 금액을 요청하는 메시지를 받았다.
	VOID			RecvCreateGuild();											// 상단을 생성한다는 메시지를 받았다.
	VOID			RecvDeleteGuild();											// 상단을 삭제한다는 메시지를 받았다
	VOID			RecvJoinLeaveGuild();										// 상단에 참여하거나 떠난다.
	VOID			RecvReqGuildWaitingJoinMembersList();						// 상단에 참여하기를 기다리는 사용자들의 리스트를 얻어온다.
	VOID			RecvJoinGuildOkNo();										// 상단에 참여를 허락 및 거부한다.
	VOID			RecvGuildDetailInfo();										// 상단의 대한 상세 정보를 얻어온다.	
	VOID			RecvReqGuildMembersList();									// 상단원 들의 리스트를 얻어온다.
	VOID			RecvGuildDismissal();										// 상단원을 해고한다.
	VOID			RecvReqGuildPromotion();									// 상단원을 특정 직급으로 임명한다.
	VOID			RecvReqGuildDemotion();										// 상단원을 특정 직급에서 해임한다.
	VOID			RecvGuildList();											// 길드의 리스트를 얻어온다.
	VOID			SetGuild(UI32 uiguildid);									// 상단을 설정한다.	
	VOID			SetClassInGuild(SI32 siclassinguild);						// 직급을 설정한다.
	SI32			GetClassInGuild();											// 직급을 얻어온다.
	VOID			RecvReqUsedFlagList();										// 사용된 깃발의 리스트를 얻어간다.
	VOID			RecvReqPIInMinimap();										// 미니맵에서 플레이어의 정보를 요구한다.



	VOID			ChangeGuildFlag(SI32 siFlag);								// 상단 깃발을 변경한다.
	VOID			ChangeSatiety(SI32 siValue);								// 포만감을 변경한다.

//--------------------------------------------------------------------------------------------
// 맵을 이동할 때의 처리
//--------------------------------------------------------------------------------------------	
	VOID			RecvMoveMap();												// 맵을 이동하는 메시지를 받았다.
	BOOL			ChangeMap(UI16 uimapid, SI16 six, SI16 siy);				// 맵을 변경한다.
	VOID			SetNearestHospitalVillageIndexInDungeon(SI32 siVillageIndex);	// 던전으로 들어가기전에 죽어서 던전 입구에서 제일 가까운 죽어서 마을의 인덱스를 저장한다.
	
	VOID			BoardShip(SI32 uiBoardShipAccount);							// 배에 탄다.
	VOID			ArriveVillageByShip(SOVillage *pVillage);					// 배를 타고 마을에 도착하였다.	

//--------------------------------------------------------------------------------------------
// 운영자 관련 메시지 처리
//--------------------------------------------------------------------------------------------	
	VOID			RecvCommand();												// 운영자 명령 메시지를 받았다.


//--------------------------------------------------------------------------------------------
// 보너스를 이용하여 능력치를 올리는 메시지 처리
//--------------------------------------------------------------------------------------------	
	VOID			RecvIncAbilityByBonus();									// 보너스를 사용하여 능력치를 올린다는 메시지를 받았다.

//--------------------------------------------------------------------------------------------
// 전투 종료 메시지
//--------------------------------------------------------------------------------------------	
	VOID			PrepareBattle();											// 전투를 시작한다.
	VOID			EndBattle();												// 전투를 종료한다.

	VOID			AllCharacterDeleteLifePoint(SI32 DeleteLife);				// 모든 Character의 Life를 뺀다.


//--------------------------------------------------------------------------------------------
// 따라다니는 용병
//--------------------------------------------------------------------------------------------
	VOID			RecvVMercenaryChange(SI08 siSlot);							// 따라다니는 용병 Change
	VOID			UpdateVMercenary(void);										// 따라다니는 용병 Change


//--------------------------------------------------------------------------------------------
// 노점상 메시지
//--------------------------------------------------------------------------------------------	
	VOID			RecvPMOpen();												// 노점상을 연다.
	VOID			RecvPMClose();												// 노점상을 닫는다.
	VOID			RecvPMBuyItem();											// 노점상에게 아이템을 산다.
	VOID			RecvPMReqItemInfo();										// 노점상이 가지고 있는 아이템의 정보를 요구한다.
	VOID			RecvPMChangeItem();											// 노점상이 가지고 있는 아이템을 변경한다.		

	
	BOOL			CanBuyPMItem(UI08 uiBoothSlot, UI16 uiItemQuantity, MONEY Money);													// 개인 상점에서 아이템을 살 수 있는가?		
	BOOL			BuyPMItem(UI08 uiBoothSlot, UI16 uiItemQuantity, MONEY Money, UI16 *puiRemainQuantity, MONEY *pTotalItemPrice,SI16 *pMercenarySlot);		// 개인 상점에서 아이템을 산다.

	
	VOID			RecvAskTrade();															// Trade를 하자고 요청한다. 
	VOID			RecvAcceptTrade();														// Trade를 하자고 요청한것에 대한 상대방의 응답을 받았다.
	VOID			RecvFinishChooseTradeItem();											// Trade시 아이템 선택을 모두 끝냈다.
	VOID			RecvDecideTrade();														// Trade시 교역을 한다는 것을 결정한다.
	VOID			RecvChangeTradeGoods();													// Trade시 교역중인 아이템을 수정한다.
	VOID			RecvCancelTrade();														// Trade시 현재 진행중인 Trade를 취소한다.
	VOID			RecvCompleteTradeBook();												// Trade시 장부 거래가 완료되었다.
	VOID            RecvRequestPersonalInfo();
	BOOL			OnUseDonkeyItem( UI08 uiFollowerID, UI16 uiItemID );					// 당나귀 아이템 사용시의 처리

	VOID			RecvRunProgramList();
	
	BOOL			CanTrade();																// 현재 교역이 가능한가?
	BOOL			IsTrading();															// 현재 다른 플레이어와 교역중인지 알아온다.	
	BOOL			IsAcceptedTrading(UI16 uiAccount = 0);									// 현재 다른 플레이어가 교역을 수락해서 교역을 하고 있는지 알아온다.
	BOOL			AcceptedTrade( UI32 uiOppGuildID, SI32 siOppClassInGuild, SI32 siOppLevel );												// 교역을 수용했다.
	BOOL			NotAcceptedTrade();														// 교역을 수용하지 않았다.
	BOOL			CancelTrade();															// 현재 진행중인 교역을 취소한다.
	BOOL			IsWaitingAcceptTrade(UI16 uiAccount = 0);								// 상대방이 교역을 승낙하는 것을 대기하는지 알아온다.
	BOOL			ChooseTradeItem();														// 교역할 아이템을 선택했다.
	BOOL			CancelChooseTradeItem();												// 교역할 아이템을 선택했던 것을 취소한다.
	BOOL			IsChoseTradeItem(UI16 uiAccount = 0);									// 교역할 아이템을 선택했는지 알아온다.
	BOOL			DecideTrade();															// 선택했던 아이템들을 가지고 교역할 것을 결정한다.
	BOOL			CancelDecideTrading();													// 교역하려고 했던 것을 취소한다.	
	BOOL			IsDecideTrading(UI16 uiAccount = 0);									// 교역을 하려고 결정을 했는지 알아온다.	
	BOOL			AddTradeGoods(UI08 uiSlotID, TradeGoodsKind Kind, LPARAM lParam1, LPARAM lParam2,
								OnFollowerInfo *pTradeFollowerInfo, DWORD *pdwFollowerDataSize, OnIndustrialComplexTradeInfo *pIndustrialComplexTradeInfo );	// 교역 물품을 추가한다.
	BOOL			CanReceiveGoods(TradeGoodsKind Kind, LPARAM lParam1, LPARAM lParam2);	// 상대방이 주려고 하는 물품들을 모두 받을 수 있는지 체크한다.
	VOID			TradeGoods( SOPlayer *pTargetPlayer, DWORD *pdwFollowerDataSize, OnFollowerInfo *pFollowerInfo, UI16 *puiFollowerNum, 
							  DWORD *pdwTradeItemDataSize, UI16 puiTradeItem[ ON_MYDATA_MAX_ITEM_NUM * 2 ], UI16 *puiTradeItemNum, CHAR *pszDBTradeArg );		// 상대방과 교역을 한다.
	DWORD			TradeFollower(SOPlayer *pTradePlayer, UI16 uiFollowerID, OnFollowerInfo *pFollowerInfo, UI08 *puiOppFollowerID);	// pTradePlayer에게 uiFollowerID의 용병을 준다.
	VOID			CloseTrade();															// 교역창을 닫는다.
	BOOL			IsValidTradeGoods();													// 내가 교역하려는 품목이 아직 유효한지 검사한다
	BOOL			CanAddFollower( UI16 uiFollowerNum );									// 내가 용병을 추가할 수 있는지 알아온다.

	BOOL			HaveGeneral();															// 장수를 가지고 있는지 알아온다.
	BOOL			IsGeneral(UI08 uiFollowerSlot);											// 장수인지 알아온다.
	BOOL			IsFieldAttackUnit();

	DWORD			GetIPInNumeric();														// 플레이어의 숫자로 이루어진 IP를 받아온다.

	SI16			GetMaxFollowerNum();													// 플레이어가 가질 수 있는 최대 용병의 수.(주인공 포함)
	BOOL			IsMonster( UI08 uiFollowerSlot );										// 특정 슬롯이 몬스터인지 검사한다.
	SI32			GetMonsterNum();														// 현재 보유하고 있는 몬스터수를 얻어온다.
	BOOL			IsAdminPlayer();														// 플레이어가 운영자인지 알아온다.

	bool			expUp(int followerIndex, int value);									// 경험치를 증가 시킨다 

	VOID			RecvRequestVillageInfo();												// 특정 마을의 정보를 요청한다.

	BOOL			GiveMoney( MONEY *pMoney, SOPlayer *pOppPlayer );						// 상대방에게 돈을 준다.

	BOOL			IsInVillage();															// 현재 마을 안에 있는지 알아온다.
	BOOL			IsEnteredStructure();													// 현재 마을의 건물 안에 들어갔는지 검사한다.

	BOOL			MoveItem( UI08 uiFollowerSlot1, UI16 uiItemID, UI16 uiItemQuantity, UI08 uiFollowerSlot2, SI16 siPosInInventory );	// 아이템을 이동 시킨다.


	// 아이템 구입 관련 

	void			SendItemCount();

	// 대장장이 관련

	VOID            RecvRefineItem();

	// 도전 108계단모드.

	SI32            GetChallengeLevel();                        // 이 플레이어가 몇번째 레벨까지 갔는지 알아온다.
	VOID            SetChallengeLevel(SI32 siChallengeLevel);   // 레벨을 설정한다.

	// 2003.10.7 추가됨, 정성운

	BOOL			ProcessPartyMessage(VOID);

	BOOL			CanJoinParty();
	BOOL			CreateParty();	
	BOOL			LeaveParty();
	UI16			GetPartyId();		
	BOOL			JoinParty( UI16 uiPartyID );					// 파티에 들어간다.
	VOID			PartyDeleted();									// 파티가 삭제되었다.

	BOOL			PartyChat( CHAR *pszChatMsg );					// 파티원들에게 채팅 메시지를 보낸다.

	VOID			CloseRequestingJoinParty();									// 나에게 파티가 들어온거에 대해서나 내가 파티 신청한거에 대해서 거부 및 취소한다.
	BOOL			AcceptRequestingJoinParty();								// 나에게 들어온 파티 신청을 허락했다.
	BOOL			RejectRequestingJoinParty();								// 나에게 들어온 파티 신청을 거부한다.
	BOOL			CancelRequestingParty( BOOL bProcessByRequest = FALSE);		// 파티 신청 하였던것을 취소한다.

	BOOL			IsPartyLeader();
	BOOL			IsRequestingJoinParty( UI16 uiAccount = 0 );	
	BOOL			IsPartyMember();
	BOOL			IsReceivedRequestJoinParty();
	
	BOOL			CanRequestJoinParty();							// 내가 상대방에게 파티를 요청할 수 있는지 알아온다.
	BOOL			CanReceiveRequestJoinParty();					// 현재 파티 요청을 받을 수 있는지 알아온다.
	VOID			RequestJoinParty( UI16 uiAccount );				// 해당 유저에게 파티에 가입 요청할 것을 요청했다.
	VOID			ReceiveRequestJoinParty( UI16 uiAccount );		// 해당 유저에게 파티에 가입 요청할 것을 받았다.
	VOID			SetPartyLeader();								// 파티장이 되었다.	

	// 2003.10.7 추가됨, 정성운
	// party message 처리 펑션 

	VOID			RecvRequestParty();
	VOID			RecvCancelRequestingParty();
	VOID			RecvRequestAcceptParty();
	VOID			RecvRequestLeaveParty(); 
	VOID			RecvCancelRequestParty();	

	VOID			RecvGetAuditGuildInfo();
	VOID			RecvMacroUsingUserInfo();

	VOID			SetEquipItems(EquipItemInfo* ItemInfom,SI16 siMercenarySlot,SI16& siTimerItemUseCount,OnlineFollower* pFollower);
	BOOL			CheckGetTimeLimitItem();
	SI16			GetRestTime(SI32 start,SI32 end);
	BOOL			IsDeleteTimerItem(SI16 siRestTime,SI16 siMaxTime);
	SI16		    GetClientSendDeleteTimerItem(OnTimerItemInfo* pTimerItemInfo);
	VOID			DeleteTimerItem();											// 시간제 아이템들 삭제한다.

	SI16			GetMDay(SI16 siMon);
	SI16			GetComPareDate(SI16 siNowYear, SI16 siNowMon, SI16 siNowDay,SI16 siEndYear,SI16 siEndMon,SI16 siEndDay);
	VOID			GetEndDate(SI16 siItemTime, SI16 siNowYear, SI16 siNowMon, SI16 siNowDay,SI16& siEndYear,SI16& siEndMon,SI16& siEndDay);

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
// 공성 관련 캐릭터에 관한 메세지 처리 함수.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
	VOID			RecvFieldAttackReady();										// 플레이어 공격대기
	VOID			RecvFieldAttackAction();									// 플레이어 총알 맞는다.			
	VOID			RecvFieldAttackDead();										// 플레이어가 죽는다.

	VOID			RecvLongDistanceAttackVillageReady();						// 플레이어가 마을을 때린다.대기(원거리)
	VOID			RecvLongDistanceAttackVillageAction();						// 플레이어가 마을을 때린다.공격(원거리)

	VOID			RecvAttackVillage();										// 마을을 공격한다는 메시지를 받았다.(근거리)

	VOID			RecvReqWarVillageDefence();									// 필드 -> 수성 인터페이스
	VOID			RecvReqWarVillageIn();										// 수성 인터페이스 -> 마을

	VOID			RecvPlayerVillageDefenceUp();								// 수성 마을의 방어력을 올린다.
	VOID            RecvReqWarVillageDefenceStop();                             // 수성 인터페이스를 종료한다.

	VOID			RecvIncreaseGuildUnit();									// 상단의 공성유닛을 증가.(생산)
	VOID			RecvDecreaseGuildUnit();									// 상단의 공성유닛을 감소.(차출)	
	VOID			RecvGetGuildUnitList();										// 상단의 공성유닛의 리스트를 받아온다.

	VOID            RecvHeal();                                                 // 보급수레가 유닛을 힐링해준당.
	VOID            RecvChargeMP();                                             // 보급수레의 마나가 회복되었다.

	VOID			RecvGuildSetRelation();										// 길드간의 관게를 설정한다는 메시지를 받았다.

	VOID			RecvFriendlyMessageOffer();									// 화친제의 메세지를 보낸다.
	VOID			RecvFriendlyMessageAccept();								// 화친제의 메세지를 받아들인다.
	VOID			RecvFreidnlyMessageCancel();								// 화친제의 메세지를 씹는다.

	VOID			RecvVillageAttackResult();									// 공성에 대한 결과.
	VOID			RecvChoicePlayer();											// 마을리스트에서 플레이어를 선택한다.
	VOID			RecvInvestmentPower();

	VOID			RecvWarWarp();												// 공성전중 워프를 한다.
	VOID			RecvNormalChar();											// 자신의 공성 캐릭을 일반 캐릭으로 만든다.

	VOID            RecvChangeUnit();                                           // 공성유닛으로 변신요청
	VOID            RecvGiveUp();                                               // 전투포기를 하였다. 

	

	
//	VOID			RecvVillageAttackTypeCheck();								// 마을 공격타입을 바꿀수 있는지 체크한다.
//	VOID			RecvVillagePlayerOilAttack();								// 수성 마을의 유닛 공격방법(기름)
//	VOID			RecvVillagePlayerStoneAttack();								// 수성 마을의 유닛 공격방법(돌)

//	VOID			RecvIncreaseGuildSupply();									// 대방이나 대행수가 마을 보급품의 수를 증가시킨다.
//	VOID			RecvDecreaseGuildSupply();									// 보급수레가 보급품을 가져간다.
//	VOID			RecvGetGuildSupplyList();
//	VOID			RecvGetSupplyAtGuildUnit();									// 보급수레에게 보급품을 채운다.

//	VOID			RecvIncreaseVillageDefenceSoldier();						// 수비병의 수를 증가 시킨다.
//	VOID			RecvGetVillageDefenceSoldier();								// 수비병의 수를 얻어온다.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
// 공성 관련 캐릭터에 관한 함수 
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
	SI16			GetFieldHp();
	void			SetFieldHp(SI16 siFieldHp);

	SI16            GetFieldMaxHP();
	void            SetFieldMaxHP(SI16 siMaxHP);

	void			IncreaseFieldHp(SI16 siFieldHp);
	void			DecreaseFieldHp(SI16 siFieldHp);

	BOOL			IsFieldAlive();

	SI32            GetFieldMP();
	void            SetFieldMP(SI32 siMP);

	SI16            GetFieldMaxMP();
	void            SetFieldMaxMP(SI32 siMaxMP);

	void			IncreaseFieldMP(SI16 siFieldMP);
	void			DecreaseFieldMP(SI16 siFieldMP);

//	SI32			GetFieldSupply();
//	void			SetFieldSupply(SI32 siFieldSupply);
//	SI32			GetFieldSupplyCharge();							   //보급수레가 가지고 있는 보급햘을 얻어온다.
//	void			SetFieldSupplyCharge(SI32 siFieldSupplyCharge);	   //보급수레에게 보급품을 넣는다.(마을->보급수레)
//	void			DecreaseSupplyCharge(SI32 siDecreaseSupplyCharge); //보급수레의 보급품을 가져간다.
//	void			InitFieldSupplyCharge(SI32 siFieldSuppyCharge);

	void		    SetPrevKind(UI16 uiPrevKind);
	UI16			GetPrevKind();

	void		    SetCaptainUnit(BOOL	bCaptain);
	BOOL			GetCaptainUnit();
	
	BOOL			IsFieldAliveWarUnit();

	VOID			SendGuildItemDeleteMessage();
	VOID			DeleteFiledUnit();											// 10시에 공성유닛을 삭제한다.



	BOOL            FieldAttackDead();
	UI16            ChangeUnit(UI16 uiCode, UI16 uiQuantity);//공성유닛으로 변신하다...
	VOID            ChangeNormalChar();
	
	UI16            GetDir(SI32 siSX, SI32 siSY, SI32 siEX, SI32 siEY);
	UI16            GetSpTargetIDOFSpear(UI16 uiDir, SI32 siSX, SI32 siSY, UI16 *puiIDs); //창병인경우 스플레쉬 데미지를 입는 캐릭터 아이디를 구한다
    SI32            GetSpTargetIDOFSiege(SI32 siSX, SI32 siSY, SI32 siEX, SI32 siEY, UI16 *puiIDs); //발석거인경우
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

// write 함수에서 지역 변수로 쓰임..
private:
	UI16    usPacketSize;

	UI16	usSize;
	UI32	uiSize;
	
	char	cCheckSum;

	UI32	uiCmd;

//  2003.20.07 파티 시스템 추가됌 - 정성운 
	
	party_state_t	m_PartyState;

};	

#endif	