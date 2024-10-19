#ifndef SOWORLD_H
#define SOWORLD_H

#include "Directives.h"
#include "Defines.h"
#include "OnlineProductStructure.h"
#include "OnlineEconomy.h"
#include "OnLineProtocol.h"
#include "SOUtility.h"
#include "SOServer.h"
#include "SOGate.h"
#include "IOnlineMap.h"
#include "SOPlayer.h"
#include "SODatabase.h"  
#include "OnlineCommonStruct.h"
#include "OnlineGameMsg.h"
#include "OnlineMap-Bf.h"
#include "SOVillage.h"
#include "SOCharKI.h"
#include "ISOMonster.h"
#include "OnlineBattleMsg.h"
#include "IFieldMonster.h"
#include "IPortal.h"
#include "OnlineText.h"
#include "IOnlineShipKI.h"
#include "SOFindPath.h"
#include "OnlineShipMoveInfo.h"
#include "ISOShip.h"
#include "Command.h"
#include "IProfit.h"
#include "IGuildClass.h"
#include "IGuild.h"
#include <onlinebattlemsg.h>
#include "Game.h"
#include "Element.h"
#include "Battle.h"
#include "BattleMgr.h"
#include "OnlineTradeParser.h"
#include "OnlineQuest-Parser.h"
#include "BuriedItemParser.h"
#include "OnlineSmithParser.h"
#include "ItemUpgradeParser.h"
#include "OnlineWarpParser.h"
#include "OnlineItem.h"
#include "OnlineChar-Parser.h"
#include "OnlineTicket-Parser.h"
#include "OnlineGeneral-Parser.h"
#include "OnlineMercenary-Parser.h"
#include "OnlineSoilderList-Parser.h"
#include "OnlineVillage-Parser.h"
#include "QuestEventItemParser.h"
#include "WarTimeParser.h"
#include "OnlineCredit.h"
#include "ExpoRank.h"
#include "SOGMList.h"
#include "FieldBattleParser.h"
#include "WarpParser.h"
#include "LogTool1.h"
#include "Smith.h"

class CAuctionMarket;
class CQuestManager;

#define		DOKDO_QUEST_CODE1		22110
#define		DOKDO_QUEST_CODE2		22100

class SOWorld
{
private:
	// 대화방의 카운트(마을에서 몇번이나 대화방을 만들었는가?)
	DWORD				dwChatRoomInVillCounter;

	BOOL				m_IsAttackVillageTimeZone;

public:
	SOGate		*clGate;
	SODatabase	*clDatabase;
	SODatabase  *clLoginDatabase;
	SODatabase	*clItemDatabase;
	SOServer	*clServer;
	
	CBattleMgr			m_BattleMgr;
	
	CAuctionMarket*		m_pAuctionMarket;	
    CSmith        *     m_pSmith;

	
	
public:
	//==================================
	// Time 상태 변수 
	//==================================
	tm					*pRealTime;				//	실제 시간.	
	time_t				stClock;
	OnRealWorldTime		RealWorldTime;			// 실제 시간.
	 
	BOOL			bPopulationUpdate;
	BOOL			bUpdateTime;
	SI32			siCurrentDayOfYear;			// 0 ~ 365 일중 하루    
	SI32			siCurrentDayOfMonth;		// 0 ~ 31 일중 하루    
	SI32			siCurrentDayOfWeek;			// 요일 (0 ~ 6; Sunday = 0)
	SI32			siCurrentHour;				// 현재 시 (0 ~ 23)	

	SI32			siTotalVillageNum;			// 전체 마을 개수.

	DWORD			dwtimeGetTime;				// 시간.		

	BOOL			bPowerInvestmentEnd;		// 24시가 되면 폭투권한이 사라진다

	CHAR			szBattleServerIP[20];		// 배틀 서버 IP.

	
	char			packetbuf[ 30000 ];
	

	BOOL            m_bFlag;
	BOOL            m_bGiveUp;

public:
	SOPlayer*				clPlayer[ ON_MAX_CONNECTION ];				// 사용자 클래스	
	SOVillage*				pclVillage;									// 마을 클래스	
	CQuestManager*			m_pQuestManager;							// 독도 퀘스트 메니져	
	UI32					m_uiDokdoQuestCompletedUserCount;			// 독도 케스트 완료한 유저 수
	
	IMonster				clMonster;									// 몬스터 클래스	
	IFieldMonster			clFieldMonster;								// 필드 몬스터 관리 클래스.
	
	OnlineCharParser		clCharParser;								// Character Info	
	OnlineTicketParser		clTicketParser;								// Ticket Info		
	OnlineQuestParser		clQuestParser;								// Quest Info	
	OnlineGeneralParser		clGeneralParser;							// General Info	
	OnlineGeneralParser		clGeneralExParser;
	OnlineMercenaryParser	clMercenaryParser;							// Mercenary Info	
	OnlineSolderListParser	clSolderListParser;							// Solder List
	OnlineSmithParser       clSmithParser;      
                        // 제련 아이템 리스트
	CBuriedItemParser		m_buriedItemParser;							// Item For Field
	CQuestEventItemParser	m_QuestEventItemParser;
//	CQuestEventItemParser*	m_pQuestEventItemParser;					// QuestEventItemParser 
	CQuestEventItemMgr*		m_QuestEventItemMgr;
	CFieldBattleParser		m_FieldBattleParser;						//
	CAttackPowerParser		m_AttackPowerParser;						// 공성시 전투력 측정
	CWarpParser				m_WarpParser;
	CWarTimeParser			m_WarTimeParser;
	
	
	cltIProfit				clIProfit;									// 시장에서 물건을 이윤 정보를 알려주는 클래스.	
	cltIGuildClass			clIGuildClass;								// Guild Class(직급) 매니저	
	cltIGuild				clIGuild;									// Guild 매니저	
	OnlineTradeParser		*pTradeParser;								// OnlineTradeParser

//	BOOL                    bChangeTime;
//	tm                      *pChangeTime;
	

public:	
	BOOL			bSendServerData;									// 서버에 데이타를 보내야하는지를 설정한다.	
	SI32			siErrCode;											// 셧다운 변수	
	SI32			siLogOnNumber;										// 현재 로그온한 사용자 수	
	UI32			uiTotalRegAccount;									// 등록되어 있는 전체 캐릭터 수	
	UI32			uiTotalRegCharactor;								// 등록되어 있는 전체 계정 수	
	SOPlayer		*pPlayerOnAccount[ ON_MAX_CONNECTION + 1 ];			// 현재 로그온한 사용자의 일렬번호	
	ISOMap			clISOMap;											// 맵	
	CItem			clItem;												// 아이템.
	



	CItemUpgradeParser	m_ItemUpgradeParser;							// 아이템 업그레이드에 대해서.. (3월 31일 이상민 추가 )
	cltOnlineWarpParser	m_clWarpParser;									// Warp Info



	OnlineBuilding	clBuilding;											// 생산 건물 정보
	OnlineEconomy	clOnlineEconomy;									// 은행 정보	
	OnlineCredit	clCredit;											// 신용도 정보


	// NPC Kind Info
	cltSOCharKI		clSOCharKI;
	IOnlineShipKI	clIOnlineShipKI;				
	cltSOFindPath	clSOFindPath;
	cltOnlineSMI	clOnlineSMI;
	ISOShip			clSOShip;


	// IPortal
	IPortal			Portal;


	// OnlineText
	OnlineText		clOnlineText;


	cltCommand		clCommand;				// Command (Command)
	
	COnlineTimer	Timer;
	SOnlineTime		PrevCurrentTime, CurrentTime;
	SOnlineTime		PrevSaveTimeToDBTime;	
	BOOL			bHourChanged, bDayChanged, bMonthChanged, bYearChanged;

	// 마지막으로 아이템의 정보를 보냈던 시간, 마지막으로 어느 아이템의 정보를 보냈는가
	DWORD			dwLastSendPayingBusinessTime;
	UI32			uiLastSendPayingBusinessItem;		

	SI32			siWeatherChangeTime;
	BOOL			bNowWeather;

	DWORD			dwLastThunderCheckTime;

	CLogTool		m_LogMgr;

	// Expo Rank...
	ExpoRank		m_ExpoRankingSystem;
	DWORD			m_dwExpoUpdateRankingTime;

	DWORD			m_dwExpoSendRankingMsgTime;

	cltGMManager	m_clGMManager;

	// GameGuard관련
	BOOL			m_bGameGuard;
	BOOL			m_bWarEnd;

	BOOL			m_bQuestEventUpdate;
	SI16			m_siEventItemCount;

	DBIKQuestEventItemInfoMgr*	m_pstQuestEventItemInfoMgr;

public:	
	SOWorld(SOServer *);					// 생성자	
	~SOWorld();								// 소멸자


public:
	SI32		Poll();						// 모든 연산 처리
	VOID		DownServer(BOOL);			// ShutDown 요구에 대한 처리

	// 소켓 번호를 검사한다.
	BOOL		CheckPlayerSocket(SI32 siSocketNumber);


public:		
	VOID		GetItemDBPrice();
	VOID		SetItemDBPrice();

	stFarmData*	GetOwnFactory( SI32 siPlayerDBAccount, UI08 uiPlayerDBSlot );		// 플레이어가 소유하고 있는 시설물을 얻어온다.

	BOOL		InitGuildFromDB();			// DB에서 길드를 읽어와서 설정한다.
	
	
	// Event
	BOOL		GetEventDataFromDDB();		// DB에서 이벤트에 대한 기본 정보를 얻어온다.
	
public:

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	마을 관리
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
	BOOL		InitGuildAndVillage();							// 상단과 마을을 초기화 한다.
	SOVillage*	GetVillageByCode(UI16 uiVillageCode);			// 마을의 Index Code를 사용하여 마을의 포인터를 얻어온다.
	SOVillage*	GetVillage(CHAR *pszVillageName);				// 마을의 이름을 사용하여 마을의 포인터를 얻어온다.			
	SI32		GetNearestHospitalVillageIndex(UI16 uiMapID, SI32 siX, SI32 siY);// 특정 위치에서 제일 가까운 의원을 가지 마을을 찾는다.		
	SI32		GetVillageIndexByCode(UI16 uiVillageCode);		// 해당 Code를 가지고 있는 마을의 Village Index를 알아온다.

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	공지 사항
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
	BOOL		OfficialAnnouncement(DWORD dwResponse,
									LPARAM lParam1 = 0,
									LPARAM lParam2 = 0,
									LPARAM lParam3 = 0,
									LPARAM lParam4 = 0,
									LPARAM lParam5 = 0,
									LPARAM lParam6 = 0,
									LPARAM lParam7 = 0);		// 현재 접속된 사용자에게 공지 사항을 알려준다.


	// 기본 캐릭터를 찾아주는 함수.
	BaseChar*	GetBaseChar(UI16 uiAccount);					// 기본 캐릭터의 포인터를 얻어온다.

	// 플레이어를 찾아주는 함수.	
	SOPlayer*	GetPlayer(CHAR* pszName);										// 플레이어의 포인터를 얻어온다.
	SOPlayer*	GetPlayer(UI16 uiAccount);										// 플레이어의 포인터를 얻어온다.
	SOPlayer*	GetPlayerByDBAccount( DWORD dwDBAccount );						// DB Account를 이용하여 플레이어의 포인터를 얻어온다.
	SOPlayer*	GetPlayerByDBAccount( DWORD dwDBAccount, UI08 uiDBSlot );		// DB Account와 DBSlot을 이용하여 플레이어의 포인터를 얻어온다.
	
	SI32		GetHighestGuildClass(CHAR *pszID);				// 캐릭터의 최고 상단 직급을 알아온다.
	SI32		GetHighestGuildClassInVillage(CHAR *pszID);		// 마을에서 캐릭터의 최고 상단 직급을 알아온다.
	SI32		GetOwnVillageNum( CHAR *pszID );				// 캐릭터가 보유한 마을의 개수를 알아온다.

	VOID		DeleteGuildClassInVillage(CHAR *pszName);		// 이 마을에서 이 캐릭터가 가지고 있는 직급을 초기화 한다.
	VOID		GetRealTime(OnRealWorldTime *pRealWorldTime);	// 실제 세계의 시간을 얻어온다.

	VOID		SendPayingBusinessAnnoucement();				// 이익이 되는 아이템의 정보를 공지해 준다
	
	CharHead*	GetCharHeader(UI16 uiCharKind);					// 캐릭터 정보의 헤더를 구한다.

	DWORD		IncChatRoomInVillCounter();						// 마을의 주점에서 대화방을 만든 횟수를 증가시켜준다.
	DWORD		LogOutAllPlayer();

	BOOL		IsIndustrialComplexOwner( SOPlayer *pPlayer );	// 플레이어가 산업 단지를 가지고 있는지 알아온다.
    SOVillage_IndustrialComplex  *GetIndustrialComplex( SOPlayer *pPlayer );
	BOOL        SellIndustrialComplex(SOPlayer *pOldOwner,SOPlayer *pNewOwner);

	VOID		EnterAttackVillageTimeZone();					// 마을 공성이 시작되었다.
	VOID		LeaveAttackVillageTimeZone();					// 마을 공성이 끝났다.
	BOOL		IsAttackVillageTimeZone();						// 현재 마을 공성중인지 알아온다.	

//────────────────────────────────────────────────────────────────────────
	VOID		ChangeSatietyByWeather();						// 필드에 있는 모든 유져들의 포만감을 감소시킨다.
	VOID		MakeThunder();									// 우천 지역에 랜덤하게 번개를 생성한다.
};



#endif